#include "Timer.h"
#include <stdio.h>
#include <stdlib.h>      // exit
#include <string.h>
#include <stdint.h>      // uint64_t
#include <sys/timerfd.h> // timerfd_create / timerfd_settime
#include <time.h>        // struct itimerspec / struct timespec
#include <unistd.h>      // read / close
#include <poll.h>        // struct pollfd / poll
#include <errno.h>       // errno
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

namespace  // 匿名命名空间，static
{
    int createTimerfd()
    {
        int timerfd = ::timerfd_create(CLOCK_REALTIME, 0);
        if(timerfd == -1)
            ERR_EXIT("timerfd_create");
        return timerfd;
    }
    
    /*
    #include <time.h>

    struct timespec {
        time_t tv_sec;                // Seconds 
        long   tv_nsec;               // Nanoseconds 
    };

    struct itimerspec {
        struct timespec it_interval;  // Timer interval 
        struct timespec it_value;     // Initial expiration 
    };
    */
    void setTimerfd(int timerfd, int value, int interval) // 起始时间，间隔时间
    {
        struct itimerspec val;
        memset(&val, 0, sizeof val);
        val.it_value.tv_sec = value;
        val.it_interval.tv_sec = interval;

        if(::timerfd_settime(timerfd, 0, &val, NULL) == -1)
            ERR_EXIT("timerfd_settime");
    }

    void stopTimerfd(int timerfd)
    {   
        // 将起始时间和间隔时间都置为0，定时器就停止了，timerfd就不活动了，poll也就监测不到了
        setTimerfd(timerfd, 0, 0); 
    }

    void readTimerfd(int timerfd)
    {
       uint64_t howmany;
       // 必须将timerfd中的内容取走，不然poll会水平触发
       if(::read(timerfd, &howmany, sizeof howmany) != sizeof howmany )
       {
           ERR_EXIT("read timerfd error"); // 读不满8字节，说明出错，我的处理是直接挂掉程序 
       }
    }
}

// 构造函数
Timer::Timer(int val,
             int interval,
             TimerCallback cb)
    : timerfd_(createTimerfd()),
      val_(val),
      interval_(interval),
      callback_(std::move(cb)),
      isStarted_(false)
{ }

// 析构函数
Timer::~Timer()
{
    if(isStarted_)
    {
        stop();
        ::close(timerfd_);
    }
}


// 启动定时器
/*
struct pollfd {
    int   fd;         // file descriptor 
    short events;     // requested events 
    short revents;    // returned events 
};
*/
void Timer::start()
{
    setTimerfd(timerfd_, val_, interval_);
    isStarted_ = true;

    struct pollfd pfd;
    pfd.fd = timerfd_;
    pfd.events = POLLIN; // 监听输入事件

    uint64_t val;
    int ret;
    while(isStarted_)
    {
        ret = ::poll(&pfd, 1, 5000); // 可以认为pfd是大小为1的数组
        if(ret == -1)
        {
            if(errno == EINTR)
            {
                continue;  // 出错情况为遇到中断信号，继续poll
            }
            ERR_EXIT("poll");
        }
        else if (ret == 0) // 超时
        {
            printf("timeout \n");
        }
        else
        {
            if (pfd.revents == POLLIN) // 判断此fd是否是监听的read事件（这边较为严禁，实际上只监听一个描述符是可以不加这条判断语句）
            {
                readTimerfd(timerfd_); // 取走timerfd_的字节
                callback_();           // 用户逻辑              
            }
        }
    }
}

// 停止计时器
void Timer::stop()
{
    isStarted_ = false;
    stopTimerfd(timerfd_);
}
