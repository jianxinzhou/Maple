#include "Thread.h"
#include "MutexLock.h" // TINY_CHECK
#include <assert.h>

Thread::Thread(ThreadCallback callback)
    : threadId_(0),
      isRunning_(false),
      callback_(std::move(callback))
{ }

Thread::~Thread()
{
    if(isRunning_)
    {
        TINY_CHECK(!pthread_detach(threadId_));
    }
}

//static
void *Thread::runInThread(void *arg)
{
    Thread *pt = static_cast<Thread*>(arg);
    pt -> callback_(); // 调用回调函数

    return NULL;
}

void Thread::start()
{
    isRunning_ = true;
    TINY_CHECK(!pthread_create(&threadId_, NULL, runInThread, this));
}

void Thread::join()
{
    assert(isRunning_);
    TINY_CHECK(!pthread_join(threadId_, NULL));
    isRunning_ = false;
}
