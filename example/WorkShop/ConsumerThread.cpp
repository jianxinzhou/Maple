#include "ConsumerThread.h"
#include "Buffer.h"
#include <stdio.h>
#include <unistd.h> // sleep
using namespace std;

ConsumerThread::ConsumerThread(Buffer &buffer)
    : buffer_(buffer)
{ }

void ConsumerThread::run()
{
    while(1)
    {
        int num = buffer_.pop();
        printf("pop a num %d and now size equal to %d \n", num, buffer_.size());

        ::sleep(3);
    }
}
