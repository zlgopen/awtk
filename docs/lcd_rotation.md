## LCD 旋转（横屏与竖屏）

有时开发板上接的 LCD 方向和我们需要的不同，比如 LCD 缺省是横屏显示的，但我们需要竖屏的效果。如果无法通过修改硬件来实现旋转，就只能用软件来实现了。AWKT 目前对双帧缓冲的情况有完善的支持，对基于寄存器的 LCD 需要在驱动中进行配置。

> 旋转最好由硬件或驱动来做，否则性能会有大弧度下降。本文档提供的方案，主要用于开发前期使用。

### 一、使用方法

1. 首先在初始化时，用 tk\_init 指定 LCD 的大小（这里 LCD 的大小是实际大小，不是旋转之后的大小）。

```
/**
 * @method tk_init
 * 初始化 TK。
 * @global
 * @scriptable no
 * @param {wh_t} w LCD 宽度。
 * @param {wh_t} h LCD 高度。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t tk_init(wh_t w, wh_t h);
```

2. 然后用 tk\_set\_lcd\_orientation 来设置 LCD 的旋转角度。

```
/**
 * @method tk_set_lcd_orientation
 * 设置屏幕的旋转方向 (XXX: 目前仅支持 0 度和 90 度）。
 * @global
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t tk_set_lcd_orientation(lcd_orientation_t orientation);
```

> 参考：demos/demo_main.c

_____

AWTK 目前支持 3 种 LCD 实现方式，不同的实现方式对旋转的支持有所不同，下面我们一一介绍。

### 二、基于寄存器的 LCD(lcd\_reg)

基于寄存器的 LCD(lcd\_reg)，调用 tk\_set\_lcd\_orientation 只是做了两件事：

* 1. 对触摸事件的坐标进行转换。

* 2. 设置窗口管理器的大小为旋转之后的大小。

一般的 LCD 器件可以在驱动中设置像素的扫描顺序，在初始化时设置一下即可（驱动我不太熟悉，没有测试过），所以 AWTK 并没有对像素进行相应旋转。

> 参考：http://www.cnblogs.com/amanlikethis/p/3872515.html

### 二、基于 FrameBuffer 的 LCD(lcd\_mem)

基于 FrameBuffer 的 LCD(lcd\_mem)，AWTK 对旋转 90/180/270 度做了支持。调用 tk\_set\_lcd\_orientation 后 AWTK 会做以下几件事：

* 1. 对触摸事件的坐标进行转换。

* 2. 设置窗口管理器的大小为旋转之后的大小。

* 3.offline fb 为旋转之后的大小，online fb 为原始大小。在 lcd flush 时需要进行旋转（在没有硬件旋转减速的情况下，会增加一点性能开销）。

* 4. 在进行平移窗口动画时。AWTK 将图片直接贴到 online fb 上，处于性能的考虑，在截图时进行旋转，在贴图时直接拷贝。所以在 lcd\_mem\_draw\_image 和 lcd\_mem\_take\_snapshot 两个函数中做了处理。

### 三、基于 VGCanvas 的 LCD(lcd\_vgcanvas)

> 最新版本已经支持旋转。
