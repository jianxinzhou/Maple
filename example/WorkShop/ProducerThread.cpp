#include "ProducerThread.h"
#include "Buffer.h"
#include <stdio.h>
#include <stdlib.h> // srand
#include <unistd.h> // sleep
using namespace std;

ProducerThread::ProducerThread(Buffer &buffer)
    : buffer_(buffer)
{
    ::srand(12345);
}

void ProducerThread::run()
{
    while(1)
    {
        int num = rand() % 100;
        buffer_.push(num);
        // 之所以输入不会混乱，是因为执行buffer_.size()也要抢锁，而自身是抢锁抢的最快的
        printf("push a num %d and now size equal to %d \n", num, buffer_.size());

        ::sleep(2);
    }
}
