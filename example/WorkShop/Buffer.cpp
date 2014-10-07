#include "Buffer.h"


Buffer::Buffer(size_t size)
    : full_(mutex_),
      empty_(mutex_),
      size_(size)
{ }

void Buffer::push(int val)
{
    // lock
    // wait
    // push
    // lock
    // notify
    {
        //  当对象lock离开该作用域时，会执行析构函数
        //  我们之前已经在MutexLockGuard的析构函数中封装了解锁的操作
        MutexLockGuard lock(mutex_);
        while(queue_.size() >= size_)
            empty_.wait();
        queue_.push(val);
    }
    full_.notify();
    
    // 注意，此处也可以先notify，之后在解锁。不过我们推荐先解锁，再notify。
    // 因为如果先notify，唤醒一个线程B，但是由于A还未解锁，此时如果线程切换至刚唤醒的线程B，
    // B马上尝试抢锁，但是必然失败，然后激素阻塞，这将增加一次线程切换的开销
}

int Buffer::pop()
{
    int temp = 0;
    {
        MutexLockGuard lock(mutex_);
        while(queue_.empty())
            full_.wait();
        temp = queue_.front();
        queue_.pop();
    }
    empty_.notify();

    return temp;
}

bool Buffer::empty() const
{
    // 加锁
    // 加锁会改变锁的状态，但是我们已经给mutex_设置了mutable属性了
    MutexLockGuard lock(mutex_);
    return queue_.empty();
}

size_t Buffer::size() const
{
    MutexLockGuard lock(mutex_);
    return queue_.size();
}

// 注意我们在 push 和 pop 的while循环中，用的都是queue的标准库函数
// 如果使用我们自己定义的size()和empty()，会导致重复加锁
