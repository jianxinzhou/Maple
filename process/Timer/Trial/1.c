#include <stdio.h>
#include <stdlib.h>  // EXIT_FAILURE
#include <signal.h>  // signal SIG_ERR
#include <unistd.h>  // pause alarm
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void sigalrm_handler(int sig)
{
    printf("foo\n");
    alarm(1);
}

int main(int argc, const char *argv[])
{
    if(signal(SIGALRM, sigalrm_handler) == SIG_ERR)
        ERR_EXIT("signal");

    alarm(3); // 一次有效

    for(;;)
        pause(); // 不占用cpu
    
    return 0;
}


/*
Output:

foo
foo
foo
foo
foo
foo
...

(第一次间隔3秒后出现foo， 以后间隔1秒后出现foo)

*/
