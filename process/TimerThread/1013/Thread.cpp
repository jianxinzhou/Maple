#include "Thread.h"
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
        pthread_detach(threadId_);
    }
}

void Thread::start()
{
    pthread_create(&threadId_, NULL, runInThread, this);
    isRunning_ = true;
}

void Thread::join()
{
    assert(isRunning_);
    pthread_join(threadId_, NULL);
    isRunning_ = false;
}

void *Thread::runInThread(void *arg)
{
    Thread *pt = static_cast<Thread*>(arg);
    pt->callback_(); // 调用回调函数
    
    return NULL; 
}
