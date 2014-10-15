TimerThread的注意事项：             
a) 用户把callback注册给Timer              
b) Timer的start方法注册给Thread                
c) 当启动时，先启动线程，然后在线程内开启定时器                 
d) 当终止时，先终止定时器，然后线程会自动退出，然后只需join即可                    

如果采用OOP实现TimerThread类，那么：              
a) 需要多重继承                
b) 保留一个虚函数，给用户实现                   
c) thread的虚函数，需要实现，在里面调用Timer的start                   

注意：本定时器实现的原理如下： timerfd_XXX系列函数，指定val和interval，在事件触发时，采用fd可读作为通知方式。           
      而Timer类实现了对其的封装。                   
      
Update: 更新了定时器精度为double。                             
