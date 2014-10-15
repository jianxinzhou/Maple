#ifndef THREADPOOL_H
#define THREADPOOL_H 

#include "NonCopyable.h"
#include "MutexLock.h"
#include "Condition.h"
#include "Thread.h"
#include <queue>
#include <vector>
#include <functional>
#include <memory>         // unique_ptr


class ThreadPool : NonCopyable
{
    public:
        typedef std::function<void ()> Task;

        ThreadPool(size_t queueSize, size_t threadsNum);
        ~ThreadPool();

        void start();
        void stop();

        void addTask(Task task); // 使用c++11风格
        Task getTask();

        bool isStarted() const;

        void runInThread();

    private:
        mutable MutexLock mutex_;
        Condition empty_; // 等待空间
        Condition full_;  // 等待产品（任务）

        size_t queueSize_; // 任务数不固定
        std::queue<Task> queue_;

        const size_t threadsNum_; // 线程数固定
        std::vector<std::unique_ptr<Thread> > threads_; // 线程对象禁止赋值和复制，我们传入指针

        bool isStarted_;

};

inline bool ThreadPool::isStarted() const
{
    return isStarted_;
}

#endif  /*THREADPOOL_H*/
