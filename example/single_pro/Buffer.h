#ifndef BUFFER_H
#define BUFFER_H 

#include "NonCopyable.h"
#include <queue>
#include "MutexLock.h"
#include "Condition.h"

class Buffer
{
    public:
        Buffer(size_t size);
        void push(int val);
        int pop();

        bool empty() const;
        size_t size() const;

    private:
        mutable MutexLock mutex_;
        Condition full_;
        Condition empty_;

        size_t size_; //缓冲区大小
        std::queue<int> queue_;
};

#endif  /*BUFFER_H*/
