## 目录介绍

* common 公共代码。

* raw 基于裸系统的实现(无RTOS的嵌入式系统)。

* 基于[freertos](https://www.freertos.org/)的实现。	

* rtt 基于[RT-Thread](https://www.rt-thread.org/)的实现。

* tos 基于腾讯[tiny OS](https://github.com/Tencent/TencentOS-tiny)的实现。

* cmsis\_os 基于cmsis\_os抽象接口的实现。

* pc 基于PC(SDL/Posix)的实现，主要用于Windows, MacOS, Linux, Android和iOS等平台。

> 对于嵌入式系统，使用RTOS比裸系统，内存开销多一点点，但是性能会有明显提升，因为sleep/delay函数只会阻塞当前线程，而不会阻塞整个系统。

