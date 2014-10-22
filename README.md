#Maple Networking Library
Hello everyone! My name is Jianxin Zhou and welcome to Maple Networking Library!    

Maple is a multithreaded C++ network library based on the reactor pattern which is referred to MUDUO.      
It runs on Linux with kernel version >= 2.6.28.           

This library is called Maple, which was given this name because I wrote this library just in autumn.  
I'm fond of autumn cause this season has brought an outbreak of good sense in Suzhou which is my hometown.   

Ok, I will show you this networking library. For convenience, I will use Chinese for the next.

##开发环境
<pre><code>1.ArchLinux
2.clang version 3.5.0     
  Target: i386-pc-linux-gnu      
  Thread model: posix</code></pre>

##编程模式
<pre><code>1.单线程Reactor模式（Non-Blocking IO + IO multipling）
2.Reactor + ThreadPool （主要是把计算任务交给线程池，IO线程只负责IO任务）</code></pre>

##概要说明
<pre><code>1.Maple体现了现代C++的两大风格，
一是事件回调，  
我们使用C++11的function/bind实现回调机制，  
用户只需要向其注册回调函数即可，  
必要时可以封装成class，  
二是采用智能指针进行资源管理，  
例如TcpConnection使用了shared_ptr管理其生命周期，  
而其他类则采用了unique_ptr来管理生命周期。
2.Maple的不足之处：
    a)对错误的处理比较粗糙
    b)没有使用一个比较规范的日志 
    c)Maple没有实现复用同一个Poller的功能  
    b)Maple采用的是阻塞IO，如果网络拥塞，IO会阻塞在read函数上，从而降低整个系统的并发能力</code></pre> 

##源码分析
<pre><code>1.NonCopyable、Copyable表示对象是否具有value语义（复制和赋值），  
为了避免潜在的BUG，Maple中除了InetAddress之外，其余均禁用掉了value语义，  
这是。
2.ThreadPool系列，   
主要包含MutexLock、Condition、Thread、ThreadPool。  
其中大量采用了RAII技术，避免资源的泄露，  
对于Thread和ThreadPool，我们采用了function作为泛型技术，  
用户只需注册回调函数。
3.Timer，内部采用timerfd系列的定时器，  
不使用信号，而是使用fd可读作为定时器的触发事件，  
这使得Timer可以加入到IO复用模型，  
此处采用的是Poll模型。  
也可以单独把Timer放到一个线程，可以使用TimerThread。
4.TcpServer系列：
    a)Robust封装了网络编程中的三大函数readn、readLine、writen（readLine使用用户缓冲区实现）
      SocketIo同样封装了网络编程中的三大函数readn、readLine、writen（readLine使用从套接字缓冲区预读的方式实现）  
      具体使用哪个，It's up to you!
    b)Socket则封装了基本的socket操作，但是不包含读写。    
    c)InetAddress包装了sockaddr_in结构体，使之更易使用。     
    d)TcpConnection则包装了Tcp连接的各种操作，    
    主要是数据的收发以及获取Tcp连接的信息。     
    TcpConnection采用shared_ptr管理其生存期，  
    还继承了enable_shared_from_this，  
    用于在类的内部获取自己的智能指针。
    e)PollPoller封装了Poll模型，  
    内部存在一个 map<int, TcpConnectionPtr>实现从文件描述符到Tcp连接的映射。
    f)TcpServer则是对以上组件的组合。
5.用户注册事件与回调流程：
    a)先注册给TcpServer，  
    然后是PollPoller，之后是TcpConnection，这样完成了事件的注册
    b)回调函数由PollPoller触发，  
    通过map寻找到Tcp连接，然后调用里面的回调函数。
6.TcpServer实质是一个IO复用模型，ThreadPool则是代表多线程，  
用户在使用时，可以只选择其一。  
如果计算任务负担较重，  
可以将计算任务与Tcp回发封装成函数，交给线程池去计算。
7.此时，运行TcpServer的线程是一个IO线程， 
ThreadPool里面的线程专注于CPU密集型计算。</code></pre>

##使用方法
<pre><code>1.安装    
执行make，生成Maple头文件夹和静态库libMaple.a  
将Maple安装到/usr/include/下，将libMaple.a放置/usr/lib/下  
编译的时候需要加上-std=c++11 -lMaple
2.仅仅挑选需要使用的文件</code></pre>

##Tips
<pre><code>1.exampl文件中有部分使用例子
2.process文件夹中记录了我一些开发的过程，你们不必理会。</code></pre>

##下一步计划
<pre><code>增加非阻塞IO</code></pre>


