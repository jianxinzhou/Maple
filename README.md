#Maple Networking Library
Hello everyone! My name is Jianxin Zhou and welcome to Maple Networking Library!    

Maple is a multithreaded C++ network library based on the reactor pattern which is referred to MUDUO.      
It runs on Linux with kernel version >= 2.6.28.           

This library is called Maple, which was given this name because I wrote this library just in autumn.  
I'm fond of autumn cause this season has brought an outbreak of good sense in Suzhou which is my hometown.   

Ok, I will show you this networking library. For convenience, I will use Chinese for the next.

##开发环境
<pre><code>1. ArchLinux
2. clang version 3.5.0     
   Target: i386-pc-linux-gnu      
   Thread model: posix</code></pre>

##编程模式
<pre><code>1.单线程Reactor模式（Non-Blocking IO + IO multipling）
2.Reactor + ThreadPool （主要是把计算任务交给线程池，  
  IO线程只负责IO任务</code></pre>



