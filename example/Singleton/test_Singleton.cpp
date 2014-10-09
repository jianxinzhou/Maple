#include "MutexLock.h"
#include "Thread.h"
#include <iostream>
using namespace std;

class Singleton
{
    public:
        static Singleton *getInstance();

    private:
        Singleton() { }

        static Singleton *pInstance_;
        static MutexLock mutex_;
};

Singleton *Singleton::pInstance_ = NULL;
MutexLock Singleton::mutex_;

//static
Singleton *Singleton::getInstance()
{
    if(pInstance_ == NULL)
    {
        mutex_.lock();
        if(pInstance_== NULL)
        {
            pInstance_ = new Singleton();
        }
        mutex_.unlock();
    }
    
    return pInstance_; 
}


class TestThread : public Thread
{
    public:
        void run();
};

void TestThread::run()
{
    const int KCount = 1000 * 1000;
    for(size_t ix = 0; ix != KCount; ++ix)
    {
        cout << Singleton::getInstance() << endl;
    }
}

int main(int argc, const char *argv[])
{
    //Singleton s; ERROR
    
    const int KSize = 100;
    TestThread threads[KSize];
    
    for(size_t ix = 0; ix != KSize; ++ix)
    {
        threads[ix].start();
    }

    for(size_t ix = 0; ix != KSize; ++ix)
    {
        threads[ix].join();
    }


    return 0;
}



/*
Output:

0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468
0xad200468    显然即使在多线程的情况下也只实例化了一个对象
...
*/
