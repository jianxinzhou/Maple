定时器可以采用以下方式：    
a)	alarm和SIGALRM信号结合。    
b)	setitimer 用户指定val和interval，到达相应时刻就触发SIGALRM信号。    
c)	timerfd_XXX系列函数，指定val和interval，在事件触发时，采用fd可读作为通知方式。     


注意：sleep函数可能会被信号handler打断，返回值为剩余的时间。
