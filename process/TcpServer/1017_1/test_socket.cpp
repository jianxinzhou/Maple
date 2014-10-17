#include "Socket.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



/*
server启动后，客户端可以使用telnet 192.168.153.131 9999 进行测试
*/

int main(int argc, char const *argv[])
{
    int fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd == -1)
    {
        perror("create socket error");
        exit(EXIT_FAILURE);
    }

    InetAddress addr("192.168.153.131", 9999);

    Socket sock(fd);

    sock.setTcpNoDelay(false);
    sock.setReusePort(true);
    sock.setReuseAddr(true);
    sock.setKeepAlive(false);

    sock.bindAddress(addr);
    sock.listen();

    int peerfd = sock.accept();

    while(1)
    {
        char recvbuf[1024] = {0};
        ::read(peerfd, recvbuf, sizeof recvbuf);
        printf("receive msg : %s", recvbuf);
        ::write(peerfd, recvbuf, strlen(recvbuf));
    }


    return 0;
}
