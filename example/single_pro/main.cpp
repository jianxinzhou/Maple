#include "Buffer.h"
#include "ProducerThread.h"
#include "ConsumerThread.h"

int main(int argc, const char *argv[])
{
    Buffer buffer(5);
    // 3 2

    ProducerThread p1(buffer);
    ProducerThread p2(buffer);
    ProducerThread p3(buffer);

    ConsumerThread c1(buffer);
    ConsumerThread c2(buffer);

    p1.start();
    p2.start();
    p3.start();

    c1.start();
    c2.start();

    p1.join();
    p2.join();
    p3.join();

    c1.join();
    c2.join();

    return 0;
}
