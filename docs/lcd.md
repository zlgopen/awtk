## LCD 接口的四种实现方式

LCD 是对显示设备的抽象，提供了基本的绘图函数。自己去实现 LCD 接口虽然不难，但是需要花费不少功夫，AWTK 提供了几种缺省的实现，利用这些缺省的实现，在移植到新的平台时，一般只需要很少的代码就行了。

下面我们介绍一下几种常见的 LCD 实现方式：

### 一、基于寄存器实现的 LCD

在低端的嵌入式平台上，内存只有几十 KB，没有足够的内存使用 framebuffer，通常直接向寄存器中写入坐标和颜色数据。lcd\_reg.inc 提供了基于寄存器实现的 LCD，用它实现不同平台的 LCD 时，只需要提供两个宏即可：

* set\_window\_func 设置要写入颜色数据的区域，相对于每次设置坐标而言，可以极大提高工作效率。
* write\_data\_func 写入颜色数据。

下面是 STMF103ze 上 LCD 的实现，这里把 set\_window\_func 定义为 TFT\_SetWindow，把 write\_data\_func 定义为 TFT\_WriteData:

```
#include "tftlcd.h"
#include "tkc/mem.h"
#include "lcd/lcd_reg.h"

typedef uint16_t pixel_t;

#define LCD_FORMAT BITMAP_FMT_BGR565
#define pixel_from_rgb(r, g, b)                                                \
  ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
#define pixel_to_rgba(p)                                                       \
  { (0xff & ((p >> 11) << 3)), (0xff & ((p >> 5) << 2)), (0xff & (p << 3)) }

#define set_window_func LCD_Set_Window
#define write_data_func LCD_WriteData_Color

#include "base/pixel.h"
#include "blend/pixel_ops.inc"
#include "lcd/lcd_reg.inc"
```

基于寄存器实现的实现有几个限制：

* 由于内存和 CPU 性能的问题，不提供任何类型的动画。
* 由于读取 LCD 当前内容速度很慢，所以需要与底色进行混合时，由 GUI 自己处理 (APP 无需关心）。
* 屏幕大时会出现闪烁。

> 在 AWTK 中，不再推荐此方法，基于片段帧缓冲实现的 LCD 是更好的选择。

### 二、基于片段帧缓冲实现的 LCD

在低端的嵌入式平台上，内存只有几十 KB，没有足够的内存创建一屏的帧缓冲，而使用基于寄存器的方式屏幕容易闪烁。

比较好的办法是，创建一小块帧缓冲，把屏幕分成很多小块，一次只绘制一小块。由于有脏矩形机制，除了打开新窗口时，在正常情况下，绘制速度仍然很快，可以有效的解决闪速问题。

lcd\_mem_fragment.inc 提供了基于片段帧缓冲实现的 LCD，用它实现不同平台的 LCD 时，只需要提供两个宏即可：

* set\_window\_func 设置要写入颜色数据的区域，相对于每次设置坐标而言，可以极大提高工作效率。
* write\_data\_func 写入颜色数据。

下面是 STMF103ze 上 LCD 的实现，这里把 set\_window\_func 定义为 TFT\_SetWindow，把 write\_data\_func 定义为 TFT\_WriteData:

```
#include "tftlcd.h"

#include "tkc/mem.h"
#include "lcd/lcd_mem_fragment.h"

typedef uint16_t pixel_t;

#define LCD_FORMAT BITMAP_FMT_BGR565
#define pixel_from_rgb(r, g, b)                                                \
  ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
#define pixel_to_rgba(p)                                                       \
  { (0xff & ((p >> 11) << 3)), (0xff & ((p >> 5) << 2)), (0xff & (p << 3)) }

#define set_window_func LCD_Set_Window
#define write_data_func LCD_WriteData_Color

#include "base/pixel.h"
#include "blend/pixel_ops.inc"
#include "lcd/lcd_mem_fragment.inc"
```

> 完整示例请参考：https://github.com/zlgopen/awtk-stm32f103ze-raw/blob/master/awtk-port/lcd_stm32_raw.c

### 三、基于 framebuffer 实现的 LCD

这是在嵌入式平台上最常见的方式。一般有两个 framebuffer，一个称为 online framebuffer，一个称为 offline framebuffer。online framebuffer 是当前现实的内容，offline framebuffer 是 GUI 当前正在绘制的内容。lcd\_mem\_rgb565 提供了 rgb565 格式的 LCD 实现，lcd\_mem\_rgba8888 提供了 rgba8888 格式的 LCD 实现，它们都是在 lcd\_mem.inc 基础上实现的，要增加新的格式也是很方便的。

下面是 STMF767 上 LCD 的实现：

```
#define FB_ADDR (uint8_t*)0XC0000000

static uint8_t* s_framebuffers[2];

lcd_t* stm32f767_create_lcd(wh_t w, wh_t h) {
  lcd_t* lcd = NULL;
  uint32_t size = w * h * lcdltdc.pixsize;
  s_framebuffers[0] = FB_ADDR;
  s_framebuffers[1] = FB_ADDR + size;

#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 
  lcd = lcd_mem_bgra8888_create_double_fb(w, h, s_framebuffers[0], s_framebuffers[1]);
#else
  lcd = lcd_mem_bgr565_create_double_fb(w, h, s_framebuffers[0], s_framebuffers[1]);
#endif /*LCD_PIXFORMAT*/
	
  return lcd;
}
```

> online\_fb\_addr 一定要是系统显存的地址，offline\_fb\_addr 可以是任意一块内存。

> 请参考： https://github.com/zlgopen/awtk-stm32f767igtx-raw/blob/master/USER/main.c

#### online framebuffer 和 offline framebuffer

* **online framebuffer** 相当于系统显存，一般更新 online framebuffer，图像就会显示到屏幕上。

* **offline framebuffer** 是 GUI 绘制时使用的 framebuffer，GUI 会把控件绘制到 offline framebuffer 上，offline framebuffer 不会直接显示到显示到屏幕上。

一般通过下面几种方式将 offline framebuffer 上的图像显示到 online framebuffer 上：

* swap 的方式。一般重载 lcd 的 swap 函数，通过系统调用切换 online framebuffer 和 offline framebuffer 的角色。一般适用于 linux 2/3 framebuffer 系统。

> 参考： https://github.com/zlgopen/awtk-linux-fb/blob/master/awtk-port/lcd\_linux\_fb.c

* 缺省 flush 的方式。一般由 lcd 的 flush 函数把 offline framebuffer 中的图像（脏矩形内的部分） 拷贝到 online framebuffer。这是缺省实现，一般不需修改。在这种情况下，一般用 lcd_mem_xxx_create_double_fb 创建 lcd 对象，注意 online framebuffer 参数一定要是系统显存。

> 如果要做显示同步，也就是等显卡把数据真正显示到屏幕上，可以重载 lcd 的 sync 函数。一般适用于 linux 单 framebuffer 的情况。参考： https://github.com/zlgopen/awtk-linux-fb/blob/master/awtk-port/lcd\_linux\_fb.c

* 自定义 flush 的方式。有的系统没有 online framebuffer，只有 offline framebuffer。比如显示屏与 MCU 之间用 SPI 连接，那就需要重载 flush 函数，把 offline framebuffer 中的图像（脏矩形内的部分） 数据传输到显示屏。在这种情况下一般用 lcd_mem_xxx_create_single_fb 创建 lcd 对象，并重载 lcd 的 flush 函数。

在嵌入式系统中，如果希望提供显示帧率，可以使用 3 framebuffer，这 3 个 framebuffer 的角色为：

* **online framebuffer** 当前显示的 framebuffer。

* **offline framebuffer** 当前绘制的 framebuffer。

* **next framebuffer** 绘制就绪的 framebuffer，下一个周期切换成 online framebuffer。

3 framebuffer 一般需要配合中断使用，实现的比较复杂，新手请不要使用。

> 请参考：https://github.com/zlgopen/awtk-stm32f429igtx-raw/blob/master/USER/main.c

### 四、基于 vgcanvas 实现的 LCD

在支持 OpenGL 3D 硬件加速的平台上（如 PC 和手机），我们使用 nanovg 把 OpenGL 封装成 vgcanvas 的接口，在 vgcanvas 基础之上实现 LCD。lcd\_vgcanvas.inc 将 vgcanvas 封装成 LCD 的接口，这里出于可移植性考虑，并没有直接基于 nanovg 的函数，而是基于 vgcanvas 的接口，所以在没有 GPU 时，如果 CPU 够强大，也是可以基于 agg/picasso 去实现的 LCD。

> 这种方式实现，一般不会在嵌入平台上使用，读者不需要关注它。

### 总结

以上几种实现方式，基本上涵盖了最常用的场景，所以在移植 AWTK 到新的平台时，并不需要在实现 LCD 接口上费多少功夫。
