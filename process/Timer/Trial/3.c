#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void sigint_handler(int sig)
{
    printf("foo\n");
}

int main(int argc, const char *argv[])
{
    if(signal(SIGINT, sigint_handler) == SIG_ERR)
        ERR_EXIT("signal");


    //alarm(4);

    int n = 100;
    do
    {
        n = sleep(n);
    }while(n > 0);

   

    return 0;
}
