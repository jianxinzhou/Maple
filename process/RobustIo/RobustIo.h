#ifndef ROBUST_IO_H
#define ROBUST_IO_H 

#include "NonCopyable.h"
#include "unistd.h"         // ssize_t

class RobustIo : NonCopyable
{
    public:
        explicit RobustIo(int fd);
        ssize_t readn(char *usrbuf, size_t n);
        ssize_t readLine(char *usrbuf, size_t maxlen);
        ssize_t writen(int fd, const void *buf, size_t count);

        // 我们将读的fd固定住
        // 写的fd可以自己传入

    private:
        ssize_t read(char *usrbuf, size_t n);

        static const int kBufferSize = 65536;

        int fd_;                   // fd
        int cnt_;                  // 缓冲区可用的字节数
        char *ptr_;                // 指向缓冲区可用的第一个字节
        char buffer_[kBufferSize]; // 缓冲区
};


#endif  /*ROBUST_IO_H*/
