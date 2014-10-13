#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/timerfd.h>
#include <stdint.h>
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void foo()
{
    printf("foo\n");
}


int main(int argc, const char *argv[])
{
    //创建fd

    int timerfd = timerfd_create(CLOCK_REALTIME, 0);
    if(timerfd == -1)
        ERR_EXIT("timerfd_create");

    //设置时间
    struct itimerspec tv;
    memset(&tv, 0, sizeof tv);
    tv.it_value.tv_sec = 3;
    tv.it_interval.tv_sec = 1;
    if(timerfd_settime(timerfd, 0, &tv, NULL) == -1)
        ERR_EXIT("timerfd_settime");

    //判断fd可读
    uint64_t val;
    int ret;
    while(1)
    {
        ret = read(timerfd, &val, sizeof val);
        if(ret == -1 && errno == EINTR)
            continue;
        printf("ret = %d\n", ret);
        foo();
    }

    close(timerfd);

    return 0;
}
