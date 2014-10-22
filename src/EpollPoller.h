#ifndef EPOLL_POLLER_H
#define EPOLL_POLLER_H

#include "NonCopyable.h"
#include <vector>
#include <map>
#include <sys/epoll.h>
#include "TcpConnection.h"



class EpollPoller : NonCopyable
{
public:
    // TcpConnection：typedef std::function<void (const TcpConnectionPtr &)> TcpConnectionCallback;
    // 回调函数的参数为：管理TcpConnection对象的智能指针shared_ptr的常量引用
    // 现在将TcpConnectionCallback重命名为EpollCallback
    typedef TcpConnection::TcpConnectionCallback EpollCallback;

    explicit EpollPoller(int listenfd);
    ~EpollPoller();

    void loop(); //启动epoll
    void unloop(); //关闭epoll

    // 从外界传入回调函数
    void setConnectCallback(EpollCallback cb)
    { onConnectCallback_ = std::move(cb); }
    void setMessageCallback(EpollCallback cb)
    { onMessageCallback_ = std::move(cb); }
    void setCloseCallback(EpollCallback cb)
    { onCloseCallback_ = std::move(cb); }

private:
    void waitEpollFd(); //循环
    void handleConnection(); //处理accept
    void handleMessage(int peerfd); //处理msg


    const int epollfd_;
    const int listenfd_; 
    bool isLooping_; //是否在运行

    typedef std::vector<struct epoll_event> EventList;
    EventList events_;  //保存活跃的fd

    typedef std::map<int, TcpConnectionPtr> ConnectionList;
    ConnectionList lists_; //实现fd到conn的映射（实现从文件描述符到Tcp连接的映射）

    EpollCallback onConnectCallback_;
    EpollCallback onMessageCallback_;
    EpollCallback onCloseCallback_;
};


#endif //EPOLL_POLLER_H