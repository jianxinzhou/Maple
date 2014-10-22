#include "EpollPoller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

// 匿名空间中存有一些辅助函数
namespace
{

int createEpollFd()
{
    int epollfd = ::epoll_create1(0);
    if(epollfd == -1)
    {
        perror("create epoll fd error");
        exit(EXIT_FAILURE);
    }

    return epollfd;
}

void addEpollReadFd(int epollfd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
        perror("add epoll fd error");
        exit(EXIT_FAILURE);
    }
}

void delEpollReadFd(int epollfd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    //ev.events = EPOLLIN;
    if(epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev) == -1)
    {
        perror("del epoll fd error");
        exit(EXIT_FAILURE);
    }
}

int acceptConnFd(int listenfd)
{
    int peerfd = ::accept(listenfd, NULL, NULL);
    if(peerfd == -1)
    {
        perror("accept conn fd");
        exit(EXIT_FAILURE);
    }
    return peerfd;
}

//预览数据
ssize_t recvPeek(int sockfd, void *buf, size_t len)
{
    int nread;
    do
    {
        nread = ::recv(sockfd, buf, len, MSG_PEEK);
    }
    while(nread == -1 && errno == EINTR);

    return nread;
}

//通过预览数据 判断conn是否关闭
bool isConnectionClosed(int sockfd)
{
    char buf[1024];
    ssize_t nread = recvPeek(sockfd, buf, sizeof buf);
    if(nread == -1)
    {
        perror("recvPeek");
        exit(EXIT_FAILURE);
    }

    return (nread == 0);
}

}
// end namespace



EpollPoller::EpollPoller(int listenfd)
: epollfd_(createEpollFd()),
  listenfd_(listenfd),
  isLooping_(false),
  events_(1024)
{
    addEpollReadFd(epollfd_, listenfd);
}

EpollPoller::~EpollPoller()
{
    ::close(epollfd_);
}


void EpollPoller::waitEpollFd()
{
    int nready;
    do
    {
        nready = ::epoll_wait(epollfd_, 
                              &*events_.begin(), 
                              static_cast<int>(events_.size()), 
                              5000);
    }while(nready == -1 && errno == EINTR);
    
    if(nready == -1)
    {
        perror("epoll wait error");
        exit(EXIT_FAILURE);
    }
    else if(nready == 0)
    {
        printf("epoll timeout.\n");
    }
    else
    {
        //当vector满时，扩充内存
        if(nready == static_cast<int>(events_.size()))
        {
            events_.resize(events_.size() * 2);
        }


        for(int ix = 0; ix != nready; ++ix)
        {
            if(events_[ix].data.fd == listenfd_)
            {
                if(events_[ix].events & EPOLLIN)
                    handleConnection();
            }
            else
            {
                if(events_[ix].events & EPOLLIN)
                    handleMessage(events_[ix].data.fd);
            }
        }
    }

}

void EpollPoller::handleConnection()
{
    int peerfd = acceptConnFd(listenfd_);
    addEpollReadFd(epollfd_, peerfd);

    std::pair<ConnectionList::iterator, bool> ret;

    // 注意一个TcpConnection对象，就代表一个全新的连接，其拥有端口号，自己的地址，对方的地址
    TcpConnectionPtr conn(new TcpConnection(peerfd));
    conn->setConnectCallback(onConnectCallback_);       // 外界通过EpollPoller对象的set方法设置回调函数给EpollPoller
    conn->setMessageCallback(onMessageCallback_);       // 将EpollPoller对象中的回调函数传给TcpConnection，        
    conn->setCloseCallback(onCloseCallback_);           // 作为TcpConnection的回调函数

    // 之前之所以在TcpConnection类中使用shared_ptr来管理TcpConnection对象，
    // 是因为此处我们要将peerfd连同其对应的TcpConnection对象，插入map中（这是从语法的角度来讲）
    // 实际上，如果不是从堆上开辟一个TcpConnection对象（并使用智能指针管理），是管理不了生命周期的
    ret = lists_.insert(std::make_pair(peerfd, conn));
    assert(ret.second == true); // 断言插入成功
    (void)ret;                  // 在release版本下，没有assert，因此此行的作用是：消除ret未使用的warning

    conn->handleConnectCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
    bool isClosed = isConnectionClosed(peerfd);
    ConnectionList::iterator it = lists_.find(peerfd);
    assert(it != lists_.end());

    if(isClosed)
    {
        //调用conn的close事件handleCloseCalback
        it->second->handleCloseCallback();
        delEpollReadFd(epollfd_, peerfd);
        lists_.erase(it);
    }
    else
    {
        it->second->handleMessageCallback();
    }
}

void EpollPoller::loop()
{
    isLooping_ = true;

    while(isLooping_)
    {
        waitEpollFd();
    }

    printf("Loop quit safely\n");
}

void EpollPoller::unloop()
{
    isLooping_ = false;

}
