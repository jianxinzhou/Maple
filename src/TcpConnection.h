#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "NonCopyable.h"
#include <memory>
#include <functional>
#include "Socket.h"
#include "SocketIO.h"

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection : NonCopyable,
                      public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::function<void (const TcpConnectionPtr &)> TcpConnectionCallback;

    explicit TcpConnection(int sockfd);
    ~TcpConnection();

    void setConnectCallback(TcpConnectionCallback cb)
    { onConnectCallback_ = std::move(cb); }
    void setMessageCallback(TcpConnectionCallback cb)
    { onMessageCallback_ = std::move(cb); }
    void setCloseCallback(TcpConnectionCallback cb)
    { onCloseCallback_ = std::move(cb); }

    void handleConnectCalback();
    void handleMessageCalback();
    void handleCloseCalback();


    void shutdown()
    { sockfd_.shutdownWrite(); isShutdownWrite_ = true; }

    ssize_t readn(char *buf, size_t count);
    ssize_t writen(const char *buf, size_t count);
    ssize_t readLine(char *usrbuf, size_t maxlen);
    std::string receive();
    void send(const std::string &s);

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
