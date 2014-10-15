#include "ThreadPool.h"
#include <assert.h>

ThreadPool::ThreadPool(size_t queueSize, size_t threadsNum)
    : empty_(mutex_),
      full_(mutex_),
      queueSize_(queueSize),
      threadsNum_(threadsNum),
      isStarted_(false)
{ }

ThreadPool::~ThreadPool()
{
    if(isStarted_)
        stop();
}

void ThreadPool::start()
{
    isStarted_ = true;
    
    // 线程池中的每个线程扮演消费者的角色
    // 初始化线程（回调函数runInThread:取任务，执行任务）
    for(size_t ix = 0;
        ix != threadsNum_;
        ++ix)
    {
        threads_.push_back(std::unique_ptr<Thread>(
                           new Thread(
                           std::bind(&ThreadPool::runInThread, this))));
    }

    //  启动线程池中的线程
    for(size_t ix = 0;
        ix != threadsNum_;
        ++ix)
    {
        threads_[ix]->start();
    }
}

void ThreadPool::stop()
{
    if(isStarted_ == false)
        return;

    {
        MutexLockGuard lock(mutex_);
        isStarted_ = false;
        // 清空任务
        while(!queue_.empty())
            queue_.pop();
    }
    // 激活所有线程（通知状态改变），防止线程池中的部分线程卡在getTask的wait上
    // ps: 线程池中的任务就是取任务，执行任务
    // 因此我们传给线程池中各个线程的回调函数runInThread就是取任务，执行任务
    full_.notifyAll();
    // 激活添加任务的线程（我的测试程序添加任务使用的是主线程）
    // 如果不激活的话，那么等到任务队列满后，
    // 由于线程池关闭，线程池中取任务的线程关闭，
    // 添加任务的线程会一直卡在empty_.wait()
    empty_.notifyAll();

    for(size_t ix = 0;
        ix != threadsNum_;
        ++ix)
    {
        threads_[ix]->join();
    }

    threads_.clear(); // 清空线程数组
}

void ThreadPool::addTask(Task task)
{
    if(!isStarted_)
        return;
    
    {
        MutexLockGuard lock(mutex_);
        
        while(isStarted_ && queue_.size() >= queueSize_)
        {
            empty_.wait();
        }
        // 添加这步，是为了防止线程池关闭时，添加任务的线程池以外的线程卡在wait上
        if(!isStarted_) // 线程池关闭
        {
            return;
        }
        else
        {
            queue_.push(std::move(task));
        }
    }
    
    full_.notify();

}

ThreadPool::Task ThreadPool::getTask()
{
    Task task;
    
    {
        MutexLockGuard lock(mutex_);
    
        while(isStarted_ && queue_.empty())
        {
            full_.wait();
        }

        if(!isStarted_)
        {
            return Task();
        }
        else
        {
            assert(!queue_.empty());
            task = queue_.front();
            queue_.pop();
        }
    }
    
    empty_.notify();
    
    return task; // 返回的是右值，如果Task拥有移动构造的能力，
                 // 就会执行移动构造
}

void ThreadPool::runInThread()
{
    while(isStarted_)
    {
        Task task(getTask()); // 取任务
        if(task)              // 执行任务，注意函数适配器内部实现定义了operator bool()
        {
            task();
        }
    }
}
