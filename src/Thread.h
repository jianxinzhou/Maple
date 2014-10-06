#ifndef THREAD_H
#define THREAD_H 

#include "NonCopyable.h"
#include <pthread.h>
#include <sys/types.h>

class Thread : NonCopyable
{
    public:
        Thread();
        ~Thread();

        void start();
        void join();
        
        virtual void run() = 0;

        pthread_t getThreadId() const;

    private:
        pthread_t threadId_;
        bool isRunning_;

        static void *runInThread(void *arg);
};

inline pthread_t Thread::getThreadId() const
{ return threadId_; }

#endif  /*THREAD_H*/
