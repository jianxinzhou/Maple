#include "Thread.h"
#include <stdio.h>
#include <unistd.h> // sleep
using namespace std;

class Foo
{
public:

    Foo()
    : thread_(bind(&Foo::foo, this))
    {

    }

    void start()
    {
        thread_.start();
        thread_.join();
    }

    void foo()
    {
        while(1)
        {
            printf("foo\n");
            sleep(1);
        }
    }
private:
    Thread thread_;
};



int main(int argc, char const *argv[])
{
    Foo f;
    f.start();

    return 0;
}

/*
Output:

foo
foo
foo
foo
foo
...

*/
