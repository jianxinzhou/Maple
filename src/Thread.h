#ifndef THREAD_H
#define THREAD_H 

#include "NonCopyable.h"
#include <pthread.h>

class Thread : NonCopyable
{
    public:
        Thread();
        virtual ~Thread();

        void start();
        void join();

        virtual void run() = 0;

        pthread_t getThreadId() const;

    private:
        static void *runInThread(void *);

        pthread_t threadId_;
        bool isRunning_;
};

inline pthread_t Thread::getThreadId() const
{
    return threadId_;
}

#endif  /*THREAD_H*/
