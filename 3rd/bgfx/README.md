# bgfx

使用bgfx需要手工去下列软件包：

* bx：https://github.com/bkaradzic/bx.git 
* bimg：https://github.com/bkaradzic/bimg.git 
* bgfx: https://github.com/bkaradzic/bgfx.git

```
git clone https://github.com/bkaradzic/bx.git 
git clone https://github.com/bkaradzic/bimg.git 
git clone https://github.com/bkaradzic/bgfx.git

```

已知问题：

bgfx.h中的init函数，在部分编译器上有问题，可以按下列方式修改：


```
-   bool init(const Init& _init = {});
+ bool init(const Init& _init);
```

