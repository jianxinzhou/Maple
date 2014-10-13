#include <stdio.h>
#include <stdlib.h>      // ERR_EXIT
#include <string.h>      // memset
#include <unistd.h>      // pause
#include <sys/time.h>    // setitimer 
#include <signal.h>      // signal SIG_ERR 
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void sigalrm_handler(int sig)
{
    printf("foo\n");
}


int main(int argc, const char *argv[])
{
    if(signal(SIGALRM, sigalrm_handler) == SIG_ERR)
        ERR_EXIT("signal");

    //3, 1

    struct timeval it_value; //初始值
    memset(&it_value, 0, sizeof it_value);
    it_value.tv_sec = 3;
    it_value.tv_usec = 0;

    struct timeval it_interval; //间隔值
    memset(&it_interval, 0, sizeof it_interval);
    it_interval.tv_sec = 1;
    it_interval.tv_usec = 0;

    struct itimerval val;
    val.it_value = it_value;
    val.it_interval = it_interval;


    //int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
    if(setitimer(ITIMER_REAL, &val, NULL) == -1)
        ERR_EXIT("setitimer");


    for(;;)
        pause();

    return 0;
}

/*
Output:

foo
foo
foo
foo
...
(第一次间隔3秒出现foo，之后间隔一秒出现foo)
*/
