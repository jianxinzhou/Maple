本定时器实现的原理如下：
imerfd_XXX系列函数，指定val和interval，在事件触发时，采用fd可读作为通知方式。

该Timer类实现了对其的封装。
