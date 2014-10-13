#include "Timer.h"
#include "Thread.h"
#include <stdio.h>
#include <unistd.h> // sleep

/*
由于开启定时器后，程序会一直在while循环中
因此我们的测试需要另一个线程来关闭定时器
*/

void foo() // 传给定时器的回调函数，写入用户逻辑
{
    printf("foo \n");
}

Timer *g_timer = NULL; // 使得定时器被主线程和另一个线程均可访问

void anotherThread() // 传给线程的回调函数，写入用户逻辑
{
    sleep(10);
    g_timer->stop();
}

int main(int argc, const char *argv[])
{
    Timer t(3, 1, &foo);
    g_timer = &t;

    Thread thd(&anotherThread);
    thd.start(); // 开始线程

    t.start();   // 开启定时器
    
    thd.join();
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
foo
timeout

*/
