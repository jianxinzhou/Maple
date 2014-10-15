#include "TimerThread.h"

TimerThread::TimerThread(int value,
                         int interval,
                         Callback cb)
    : timer_(value, interval, std::move(cb)),
      thread_(std::bind(&Timer::start, &timer_))
{ }

void TimerThread::start()
{
    thread_.start();
}

void TimerThread::stop() // 提供给别的线程使用
{
    timer_.stop(); // 定时器关闭后，线程退出while循环后，随即退出线程
    thread_.join();
}
