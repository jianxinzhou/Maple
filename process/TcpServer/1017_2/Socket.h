#ifndef SOCKET_H_
#define SOCKET_H_

#include <boost/noncopyable.hpp>
#include "InetAddress.h"

class Socket : boost::noncopyable
{
public:
    explicit Socket(int sockfd);
    ~Socket();

    int fd() const { return sockfd_; }
    void bindAddress(const InetAddress &addr);
    void listen();
    int accept();

    void shutdownWrite();

    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);


    //static
    static InetAddress getLocalAddr(int sockfd);
    static InetAddress getPeerAddr(int sockfd);

private:
    const int sockfd_;
};



#endif //SOCKET_H_