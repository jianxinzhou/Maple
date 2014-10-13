#include "Thread.h"
#include <stdio.h>
#include <unistd.h>  // sleep
using namespace std;

void foo()
{
    while(1)
    {
        printf("foo\n");
        sleep(1);
    }
}



int main(int argc, char const *argv[])
{
    Thread t(&foo); // Thread类中定义的回调函数没有参数，因此将函数地址传入即可

    t.start();
    t.join();

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
foo
...

*/
