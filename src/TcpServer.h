#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <boost/noncopyable.hpp>
#include "EpollPoller.h"
#include "Socket.h"

namespace Tiny
{
class TcpServer : boost::noncopyable
{
public:
    typedef EpollPoller::EpollCallback TcpServerCallback;

    explicit TcpServer(const InetAddress &addr);

    void start()
    { poller_.loop(); }

    void setConnectCallback(TcpServerCallback cb)
    { poller_.setConnectCallback(std::move(cb)); }
    void setMessageCallback(TcpServerCallback cb)
    { poller_.setMessageCallback(std::move(cb)); }
    void setCloseCallback(TcpServerCallback cb)
    { poller_.setCloseCallback(std::move(cb)); }

private:
    Socket sockfd_;
    EpollPoller poller_;
};

}


#endif //TCP_SERVER_H