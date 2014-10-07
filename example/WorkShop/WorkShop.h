#ifndef WORKSHOP_H
#define WORKSHOP_H 

#include "NonCopyable.h"
#include "Buffer.h"
#include <vector>

class ProducerThread;
class ConsumerThread;

class WorkShop : NonCopyable
{
    public:
        WorkShop(size_t bufferSize,
                 size_t producerSize,
                 size_t consumerSize);
        ~WorkShop();

        void startWorking();
    private:
        size_t bufferSize_;
        Buffer buffer_;

        size_t producerSize_;
        size_t consumerSize_;
        std::vector<ProducerThread*> producers_;
        std::vector<ConsumerThread*> consumers_;
};


#endif  /*WORKSHOP_H*/
