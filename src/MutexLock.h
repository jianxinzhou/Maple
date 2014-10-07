#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H 

#include "NonCopyable.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TINY_CHECK(exp) \
if(!(exp)) \
{ \
    fprintf(stderr, "File:%s Line:%d [ #exp ] is true, abort. \n", __FILE__, __LINE__); \
    abort(); \
}

class MutexLock : NonCopyable
{
    friend class Condition;
    public:
        MutexLock();
        ~MutexLock();

        void lock();
        void unlock();

        pthread_mutex_t *getMutexPtr();
        bool isLocking() const;

    private:
        void restoreMutexStatus();

        pthread_mutex_t mutex_;
        bool isLocking_;
};

inline pthread_mutex_t * MutexLock::getMutexPtr()
{
    return &mutex_;
}

inline bool MutexLock::isLocking() const
{
    return isLocking_;
}

inline void MutexLock::restoreMutexStatus()
{
    isLocking_ = true;
}




class MutexLockGuard : NonCopyable
{
    public:
        MutexLockGuard(MutexLock &mutex_);
        ~MutexLockGuard();
    private:
        MutexLock &mutex_;
};


#endif  /*MUTEXLOCK_H*/
