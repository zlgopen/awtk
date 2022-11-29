# LCD 旋转

有时开发板上接的 LCD 方向和我们需要的不同，比如 LCD 缺省是横屏显示的，但我们需要竖屏的效果。如果无法通过修改硬件来实现旋转，就只能用软件来实现了。AWKT 目前对双帧缓冲的情况有完善的支持，对基于寄存器的 LCD 需要在驱动中进行配置。

> 注：旋转最好由硬件或驱动来做，否则性能会有大幅度下降。本文档提供的方案，主要用于开发前期使用。

## 1 使用方法

### 1.1 使用 AWTK Designer 实现

在 AWTK Designer 左下角点击"项目设置"，在"LCD 旋转设置"一栏中选择旋转角度后点击"确认"即可。

### 1.2 使用 C 代码实现

在 `src/main.c`文件中定义一个宏，示例与宏的取值如下：

| 宏                  | 作用           |
| ------------------- | -------------- |
| LCD_ORIENTATION_90  | 逆时针旋转90°  |
| LCD_ORIENTATION_180 | 逆时针旋转180° |
| LCD_ORIENTATION_270 | 逆时针旋转270° |

```c
/* 逆时针旋转90° */
#define APP_LCD_ORIENTATION LCD_ORIENTATION_90
```

> 该宏需要在"#include "awtk_main.inc""前进行定义。

### 1.3 快速旋转模式

AWTK 内部还提供了快速旋转模式，该机制能在旋转时保持较高的效率（和没有旋转时的运行效率几乎一样），但是兼容性比较差，具体如何使用请参考 [如何使用高效的屏幕旋转](https://github.com/zlgopen/awtk/tree/master/docs/how_to_use_fast_lcd_portrait.md)

## 2 不同 LCD 实现方式对旋转的支持

AWTK 目前支持 3 种 LCD 实现方式，不同的实现方式对旋转的支持有所不同，下面我们一一介绍。

### 2.1 基于寄存器的 LCD(lcd\_reg)

1. 对触摸事件的坐标进行转换。
2. 设置窗口管理器的大小为旋转之后的大小。

一般的 LCD 器件可以在驱动中设置像素的扫描顺序，在初始化时设置一下即可（未测试过），所以 AWTK 并没有对像素进行相应旋转。

> 注：参考：http://www.cnblogs.com/amanlikethis/p/3872515.html

### 2.2 基于 FrameBuffer 的 LCD(lcd\_mem)

1. 对触摸事件的坐标进行转换。
2. 设置窗口管理器的大小为旋转之后的大小。
3. offline fb 为旋转之后的大小，online fb 为原始大小。在 lcd flush 时需要进行旋转（在没有硬件旋转减速的情况下，会增加一点性能开销）。
4. 在进行平移窗口动画时。AWTK 将图片直接贴到 online fb 上，处于性能的考虑，在截图时进行旋转，在贴图时直接拷贝。所以在 lcd\_mem\_draw\_image 和 lcd\_mem\_take\_snapshot 两个函数中做了处理。

### 2.3 基于 VGCanvas 的 LCD(lcd\_vgcanvas)

1. 对触摸事件的坐标进行转换。
2. 矢量画布绘制时，旋转坐标。
