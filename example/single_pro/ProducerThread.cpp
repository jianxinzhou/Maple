#include "ProducerThread.h"
#include "Buffer.h"
#include <stdlib.h> // srand
#include <iostream> // cout
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
        cout << "push a num " << num << " and now size equal to " << buffer_.size() << endl;

        ::sleep(2);
    }
}
