#ifndef CONSUNER_THREAD_H_
#define CONSUNER_THREAD_H_ 

#include "Thread.h"

class Buffer;

class ConsumerThread : public Thread
{
    public:
        ConsumerThread(Buffer &buffer);
        void run();
    private:
        Buffer &buffer_;
};

#endif  /*CONSUNER_THREAD_H_*/
