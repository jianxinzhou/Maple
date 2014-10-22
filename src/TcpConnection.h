#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost/noncopyable.hpp>
#include <memory>
#include <functional>
#include "Socket.h"
#include "SocketIO.h"


// 通过智能指针shared_ptr来管理TcpConnection对象
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;




class TcpConnection : boost::noncopyable,
                      public std::enable_shared_from_this<TcpConnection>
{
public:
    // 回调函数的参数为：管理TcpConnection对象的智能指针shared_ptr的常量引用，
    // 之所以需要这个参数，是因为外部传入的回调函数需要用到TcpConnection对象对外提供的函数接口
    typedef std::function<void (const TcpConnectionPtr &)> TcpConnectionCallback;

    explicit TcpConnection(int sockfd);
    ~TcpConnection();

    // 外界传入回调函数
    // C++11 move semantics
    void setConnectCallback(TcpConnectionCallback cb)
    { onConnectCallback_ = std::move(cb); }
    void setMessageCallback(TcpConnectionCallback cb)
    { onMessageCallback_ = std::move(cb); }
    void setCloseCallback(TcpConnectionCallback cb)
    { onCloseCallback_ = std::move(cb); }

    // 执行回调函数
    void handleConnectCallback();
    void handleMessageCallback();
    void handleCloseCallback();


    void shutdown()
    { sockfd_.shutdownWrite(); isShutdownWrite_ = true; }

    ssize_t readn(char *buf, size_t count);
    ssize_t writen(const char *buf, size_t count);
    ssize_t readLine(char *usrbuf, size_t maxlen);
    std::string receive();
    void send(const std::string &s);


    const InetAddress &getLocalAddr() const
    { return localAddr_; }
    const InetAddress &getPeerAddr() const
    { return peerAddr_; }

    std::string toString() const;

private:
    Socket sockfd_;
    SocketIO sockIO_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    bool isShutdownWrite_;  //是否关闭写端

    TcpConnectionCallback onConnectCallback_;
    TcpConnectionCallback onMessageCallback_;
    TcpConnectionCallback onCloseCallback_;
};




#endif //TCP_CONNECTION_H