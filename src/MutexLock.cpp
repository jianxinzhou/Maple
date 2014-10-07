#include "MutexLock.h"
#include <assert.h>

MutexLock::MutexLock()
    :isLocking_(false)
{
    TINY_CHECK(!pthread_mutex_init(&mutex_, NULL));
}

MutexLock::~MutexLock()
{
    assert(isLocking());
    TINY_CHECK(!pthread_mutex_destroy(&mutex_));
}

void MutexLock::lock()
{
    TINY_CHECK(!pthread_mutex_lock(&mutex_));
    isLocking_ = true;
}

void MutexLock::unlock()
{
    isLocking_ = false;
    TINY_CHECK(!pthread_mutex_unlock(&mutex_));
}
// end MutexLock

MutexLockGuard::MutexLockGuard(MutexLock &mutex)
    :mutex_(mutex)
{
    mutex_.lock();
}

MutexLockGuard::~MutexLockGuard()
{
    mutex_.unlock();
}
// end MutexLockGuard
