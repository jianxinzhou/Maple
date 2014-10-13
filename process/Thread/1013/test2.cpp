#include "Thread.h"
#include <stdio.h>
#include <unistd.h>
using namespace std;

class Foo
{
public:
    void foo(int i)
    {
        while(1)
        {
            printf("foo %d\n", i++);
            sleep(1);
        }
    }
};



int main(int argc, char const *argv[])
{
    Foo f;
    int i = 34;
    Thread t(bind(&Foo::foo, &f, i)); // 传入函数地址，并绑定类对象的隐式指针this，以及我们所用的i，
                                      // 因为类中的回调函数是无参的

    t.start();
    t.join();

    return 0;
}


/*
Outout:

foo 34
foo 35
foo 36
foo 37
foo 38
foo 39
...

*/
