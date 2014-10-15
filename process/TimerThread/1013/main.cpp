#include "TimerThread.h"
#include <stdio.h>
#include <unistd.h>

void foo()
{
    printf("foo\n");
}



int main(int argc, char const *argv[])
{
    TimerThread t(3, 1, &foo);

    t.start();

    sleep(10); // 因为主线程和线程是来回切换的，此处睡眠10秒可以让线程多跑一会
               // 不然可能马上会运行主线程的 t.stop()
    
    t.stop();  // 由主线程关闭定时器，关闭定时器后，线程退出定时器中的while循环，随后退出线程，join

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
