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

### 三、基于 framebuffer 实现的 LCD

这是在嵌入式平台上最常见的方式。一般有两个 framebuffer，一个称为 online framebuffer，一个称为 offline framebuffer。online framebuffer 是当前现实的内容，offline framebuffer 是 GUI 当前正在绘制的内容。lcd\_mem\_rgb565 提供了 rgb565 格式的 LCD 实现，lcd\_mem\_rgba8888 提供了 rgba8888 格式的 LCD 实现，它们都是在 lcd\_mem.inc 基础上实现的，要增加新的格式也是很方便的。

下面是 STMF429 上 LCD 的实现：

```
extern u32 *ltdc_framebuf[2];
#define online_fb_addr (uint8_t*)ltdc_framebuf[0]
#define offline_fb_addr (uint8_t*)ltdc_framebuf[1]

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
  return lcd_mem_rgb565_create_double_fb(w, h, online_fb_addr, offline_fb_addr);
}
```

### 四、基于 vgcanvas 实现的 LCD

在支持 OpenGL 3D 硬件加速的平台上（如 PC 和手机），我们使用 nanovg 把 OpenGL 封装成 vgcanvas 的接口，在 vgcanvas 基础之上实现 LCD。lcd\_vgcanvas.inc 将 vgcanvas 封装成 LCD 的接口，这里出于可移植性考虑，并没有直接基于 nanovg 的函数，而是基于 vgcanvas 的接口，所以在没有 GPU 时，如果 CPU 够强大，也是可以基于 agg/picasso 去实现的 LCD。

> 这种方式实现，一般不会在嵌入平台上使用，读者不需要关注它。

### 总结

以上几种实现方式，基本上涵盖了最常用的场景，所以在移植 AWTK 到新的平台时，并不需要在实现 LCD 接口上费多少功夫。
