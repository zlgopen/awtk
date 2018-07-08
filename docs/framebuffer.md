## FrameBuffer的几种使用方式

### 一、单framebuffer。

系统中只有一个framebuffer，LCD使用该framebuffer进行显示，GUI使用该framebuffer进行绘制。

![1](images/fb1.png)

#### 优点：
* 只有一个framebuffer，内存需求减半。
* 无需额外内存拷贝，而且只需画变化的部分，所以性能很高耗电低。

#### 缺点

* GUI绘制的同时LCD在显示，这很容易出现屏幕闪烁。为了避免闪烁，可以在GUI绘制前禁用LCD同步数据(需等待LCD刷完后再禁用)，等绘制完成时再启用LCD同步数据。我在stm32f429igtx上测试，发现确实不闪烁了，但是在窗口动画时，颜色有些不正常。不知是不是LCD频繁启用/禁用，LCD时钟来不及稳定导致的。

> 如果硬件能够解决动画颜色不正常的问题或者不需要动画，这种方式是最好的选择。

### 二、双framebuffer，一个online一个offline，轮流切换显示。

* 1.GUI在offline的framebuffer上绘制。
* 2.LCD显示online的framebuffer。
* 3.GUI绘制完成后，交换两个framebuffer，online变offline，offline变online。

![2](images/fb2.png)

#### 优点：

* 无需内存拷贝，切换LCD的framebuffer地址即可。

#### 缺点

* GUI每次都需要进行完整的绘制，不能只绘制变化的部分。

### 三、双framebuffer，一个固定online供LCD显示，一个固定offline供GUI绘制。

![3](images/fb3.png)

#### 优点：

* 只需绘制变化的部分，绘制完成后，把变化的部分从offline的framebuffer拷贝到online的framebuffer即可。

#### 缺点：

* 窗口动画时，可能整个屏幕都在变化，所以拷贝的量比较大。优化方法：对于平移的动画，可以让GUI直接往online的framebuffer上绘制，减少一次内存拷贝，而且不会出现闪烁。

### 四、三个framebuffer，一个online供LCD显示，一个offline供GUI绘制，一个为下一个要显示的framebuffer。

![4](images/fb4.png)

第二种方式的双缓冲切换方法有一个重要的性能问题：因为并不是在任意时刻都可以切换切换缓冲区，而是只有在当前帧显示完成后才能切换，否则就会出现刚显示一部分就切换到下一帧的情况，这会导致闪烁。所以每次切换都需要等待显示完成，按每秒刷新60帧算，这需要等待16ms左右，如果绘制本身需要16ms，那帧率就只有30FPS了。如果使三个framebuffer，就不用等待切换完成了，帧率一下可以到达60FPS了。

* 1.GUI选取不是online和next的framebuffer作为offline的framebuffer，在offline的framebuffer上绘制。
* 2.GUI绘制完成后，把offline设置为next作为即将显示的framebuffer。
* 3.LCD在显示完成中断里，将next设置为online的framebuffer，并将next设置为空闲。

#### 优点：

* 显示速度大幅提升。

#### 缺点：

* 多一些内存开销。

> 以上方式各有优缺点，请根据具体情况进行选择，建议使用最后一种方式。



