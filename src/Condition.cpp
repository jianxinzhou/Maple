#include "Condition.h"
#include "MutexLock.h"
#include <assert.h>

Condition::Condition(MutexLock &mutex)
    : mutex_(mutex)
{
    TINY_CHECK(!pthread_cond_init(&cond_, NULL));
}

Condition::~Condition()
{
    TINY_CHECK(!pthread_cond_destroy(&cond_));
}

void Condition::wait()
{
    assert(mutex_.isLocking());
    TINY_CHECK(pthread_cond_wait(&cond_, mutex_.getMutexPtr()));
    // 如果阻塞在 pthread_cond_wait
    // 其他线程抢到锁，再解锁后，bool变量isLocking_将置为false
    // 如果此时该阻塞线程抢到锁，如果经while循环后再执行Condition::wait
    // 此时由于isLocking_为false，断言出错。
    // 因此我们必须在阻塞线程抢到锁后，第一时间恢复锁的状态为加锁状态
    mutex_.restoreMutexStatus();
}

void Condition::notify()
{
    TINY_CHECK(!pthread_cond_signal(&cond_));
}

void Condition::notifyAll()
{
    TINY_CHECK(!pthread_cond_broadcast(&cond_));
}
