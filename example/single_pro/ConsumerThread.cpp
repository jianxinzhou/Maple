#include "ConsumerThread.h"
#include "Buffer.h"
#include <iostream> // cout
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
        cout << "pop a num " << num << " and now size equal to " << buffer_.size() << endl;

        ::sleep(3);
    }
}
