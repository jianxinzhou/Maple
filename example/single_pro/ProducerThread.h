#ifndef PRODUCER_THREAD_H_
#define PRODUCER_THREAD_H_ 

#include "Thread.h"

class Buffer;

class ProducerThread : public Thread
{
    public:
        ProducerThread(Buffer &buffer);
        void run(); // 覆盖基类的虚方法
    private:
        Buffer &buffer_;
};


#endif  /*PRODUCER_THREAD_H_*/
