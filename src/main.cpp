#include "ThreadPool.h"
#include "TimerThread.h"
#include <time.h>
#include <stdio.h>
#include <errno.h>

void nano_sleep(double val);

void foo()
{
    printf("foo\n");
}

void stopPool(ThreadPool *pool) //用于停止线程池
{
    pool->stop();
}

int main(int argc, char const *argv[])
{
    ThreadPool pool(100, 4);
    TimerThread timer(4.5, 0, std::bind(&stopPool, &pool));
    pool.start();
    timer.start(); //开启定时器，10s后停止线程池

    while(pool.isStarted())
    {
        pool.addTask(&foo);
        nano_sleep(0.5);
    }

    timer.stop();
    pool.stop();     //wanzhengxing
    
    return 0;
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


/*
Output:

foo
foo
foo
foo
foo
foo
foo
foo
foo
timeout

*/
