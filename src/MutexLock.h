#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H 

#include "NonCopyable.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*用于pthread系列函数的返回值检查*/
#define TINY_CHECK(exp) \
if(!(exp)) \
{   \
    fprintf(stderr, "File:%s, Line:%d Exp:[" #exp "] is true, abort.\n" \
            , __FILE__, __LINE__); \
    abort(); \
}

class MutexLock : NonCopyable
{
    public:
        MutexLock();
        ~MutexLock();

        void lock();
        void unlock();

        // 辅助函数
        bool isLocking() const;
        pthread_mutex_t *getMutexPtr();// 思考为什么不是const}
    
    private:
        pthread_mutex_t mutex_;
        bool isLocking_; // 是否上锁
};

inline bool MutexLock::isLocking() const
{ return isLocking_; }

inline pthread_mutex_t *MutexLock::getMutexPtr()
{ return &mutex_; }



#endif  /*MUTEXLOCK_H*/
