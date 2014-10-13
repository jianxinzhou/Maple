#ifndef TIMER_THREAD_H_
#define TIMER_THREAD_H_ 

#include "NonCopyable.h"
#include "Timer.h"
#include "Thread.h"
#include <functional>

class TimerThread : NonCopyable
{
    public:
        typedef std::function<void ()> Callback;
        
        TimerThread(int value,
                    int interval,
                    Callback cb);

        void start();
        void stop();
   
    private:
        Timer timer_;
        Thread thread_;
};

#endif  /*TIMER_THREAD_H_*/
