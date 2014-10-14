#include "RobustIo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

RobustIo::RobustIo(int fd)
    : fd_(fd),
      cnt_(0),
      ptr_(buffer_)
{
    ::memset(buffer_, 0, kBufferSize);
}

ssize_t RobustIo::read(char *usrbuf, size_t n)
{
    // 缓冲区为空，从内核缓冲区中read到用户缓冲区buffer_
    // 注意从内核缓冲区未必就一定能读到kBufferSize个字节
    while(cnt_ <= 0)
    {
        ssize_t nread =
            ::read(fd_, buffer_, kBufferSize);
        
        if(nread == -1)
        {
            if(errno == EINTR)
                continue; 
            return -1;       // ERROR
        }
        else if(nread == 0)  // 0 表示读到了末尾，
            return 0;        // 在socket中表示对方已经关闭了连接，这里接收到一个FIN请求，从而返回0

        // 正常读取
        cnt_ = nread;
        ptr_ = buffer_; //每次从内核缓冲区中read，需要重置指针
    }

    // 现有库存和用户要求的数量 取较小者
    int cnt = (cnt_ < n) ? cnt : n;
    ::memcpy(usrbuf, ptr_, cnt);  // 从用户缓冲区buffer_中读取cnt个字节到用户传进来的usrbuf中
    ptr_ += cnt; // buffer_缓冲区指针往后移cnt个位置
    cnt_ -= cnt; // buffer_缓冲区可用字节数减少cnt个

    return cnt;  // 返回成功读取的字节数 
}
      

ssize_t RobustIo::readn(char *usrbuf, size_t count)
{
    size_t  nleft = count; // 剩余的字节数
    ssize_t nread;         // 用作返回值
    char *bufp = usrbuf;   // 缓冲区的偏移量

    while(nleft > 0)
    {
        // 不会再频繁执行read系统调用
        // 直接从我们的用户缓冲区buffer_中读取了
        nread = this->read(bufp, nleft);
        if((nread = this->read(bufp, nleft)) == -1)
        {
            return -1; // 这里不需要处理EITER，在this->read()中已经处理过   
        }
        else if(nread == 0) // EOF
        {
            break;
        }

        nleft -= nread;
        bufp  += nread;
    }

    return (count - nleft); // 例如，你想读取10个字节，但可能可读的就只有6个字节
                            // 因为可能内核缓冲区中就只有6个字节
                            // 通过this->read读到用户缓冲区也就只有6个字节,并返回0
                            // 那么readn也就只能读6个字节
}

ssize_t RobustIo::readLine(char *usrbuf, size_t maxlen)
{
    int i; //计数
    int nread;

    char *bufp = usrbuf;
    char c; // 暂存字符

    for(i = 0; i < maxlen - 1; ++i)
    {
        if((nread = this->read(&c, 1)) == -1) // ERROR
        {
            return -1;
        }
        else if(nread == 0) // 第一个字符读取遇到EOF
        {
            if(i == 0)
                return 0;
            break;
        }

        *bufp++ = c;  // 放入usrbuf
        if(c == '\n') // 碰到换行符直接退出循环
            break;
    }
    
    *bufp = '\0';  
    return i;         // 返回成功读取的字符数，不包括最后的'\0'
}

ssize_t RobustIo::writen(int fd, const void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nwrite;
    const char *bufp = static_cast<const char*>(buf);

    while(nleft > 0)
    {
        if((nwrite = write(fd, bufp, nleft)) <= 0) //ERROR
        {
            if(nwrite == -1 && errno == EINTR)
                continue;
            return -1;
        }

        nleft -= nwrite;
        bufp  += nwrite;
    }

    return count; //这里不是count - nleft，必须是count
}
