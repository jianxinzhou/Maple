#ifndef Thread_H_
#define Thread_H_

#include <pthread.h>
#include "NonCopyable.h"
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
        ~Thread();

        void start();
        void join();

        static void *runInThread(void *);

    private:
        pthread_t threadId_;
        bool isRunning_;
        ThreadCallback callback_; // 回调函数，处理用户逻辑
};


#endif  /*Thread_H_*/
