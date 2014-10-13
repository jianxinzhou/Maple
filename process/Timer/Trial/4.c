#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void sigint_handler(int sig)
{
    printf("foo\n");
}


void nano_sleep(double val)
{
    struct timespec tv;
    tv.tv_sec = val; //取整
    tv.tv_nsec = (val - tv.tv_sec) * 1000 * 1000 * 1000;

    int ret;
    do
    {
        ret = nanosleep(&tv, &tv);
    }while(ret == -1 && errno == EINTR);
}

int main(int argc, const char *argv[])
{
    if(signal(SIGINT, sigint_handler) == SIG_ERR)
        ERR_EXIT("signal");

    // struct timespec tv;
    // tv.tv_sec = 3;
    // tv.tv_nsec = 0;
    // nanosleep(&tv, NULL);

    nano_sleep(4.5);
    
   

    return 0;
}
