#ifndef THREAD_H
#define THREAD_H 

#include "NonCopyable.h"
#include <pthread.h>
#include <functional>

class Thread : NonCopyable
{
    public:
        typedef std::function<void ()>
            ThreadCallback;
        
        // 构造函数使用c++11风格，使用Thread(ThreadCallback callback)代替如下：
        // 常量语义：Thread(const ThreadCallback &callback)
        // 移动语义：Thread(ThreadCallback &&callback)
        Thread(ThreadCallback);
        virtual ~Thread();

        void start();
        void join();

        pthread_t getThreadId() const;

    private:
        static void *runInThread(void *);

        pthread_t threadId_;
        bool isRunning_;
        ThreadCallback callback_; // 回调函数，处理用户逻辑
};

inline pthread_t Thread::getThreadId() const
{
    return threadId_;
}

#endif  /*THREAD_H*/
