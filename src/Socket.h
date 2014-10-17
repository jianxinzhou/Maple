#ifndef SOCKET_H_
#define SOCKET_H_

#include "NonCopyable.h"
#include "InetAddress.h"

class Socket : NonCopyable
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
    static InetAddress getLocalAddress(int sockfd);
    static InetAddress getPeerAddress(int sockfd);

private:
    const int sockfd_;
};



#endif //SOCKET_H_
