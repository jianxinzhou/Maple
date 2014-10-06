#include "MutexLock.h"
#include <assert.h>

MutexLock::MutexLock()
    : isLocking_(false)
{
    TINY_CHECK(!pthread_mutex_init(&mutex_, NULL));
}

MutexLock::~MutexLock()
{
    assert(!isLocking()); // 确保解锁
    TINY_CHECK(!pthread_mutex_destroy(&mutex_));
}

void MutexLock::lock()
{
    TINY_CHECK(!pthread_mutex_lock(&mutex_));
    isLocking_ = true;
}

void MutexLock::unlock()
{
    TINY_CHECK(!pthread_mutex_unlock(&mutex_));
    isLocking_ = false;
}


