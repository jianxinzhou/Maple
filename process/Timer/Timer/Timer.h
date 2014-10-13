#ifndef TIME_H
#define TIME_H 

#include "NonCopyable.h"
#include <functional>

class Timer : NonCopyable
{
    public:
        typedef std::function<void ()> TimerCallback;
        
        // 构造函数中第三个参数使用C++11风格 TimerCallback cb，用来替代
        // const TimerCallback &cb (常量语义)
        // TimerCallback &&cb      (移动语义)
        Timer(int val, int interval_, TimerCallback cb);
        ~Timer();

        void start(); // 开启定时器
        void stop();    // 停止定时器
    
    private:
        int timerfd_;
        int val_;
        int interval_;
        TimerCallback callback_; // 用户逻辑
        bool isStarted_;         // 定时器是否开启
};

#endif  /*TIME_H*/
