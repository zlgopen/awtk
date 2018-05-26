## LCD接口的三种实现方式

LCD是对显示设备的抽象，提供了基本的绘图函数。自己实现一个LCD虽然不难，但是需要花费不少功夫，所以AWTK提供了几种缺省的实现，利用这些缺省的实现，在移植到新的平台时，一般只需要很少的代码就行了。下面我们介绍一下几种常见的LCD实现方式：

### 一、基于寄存器实现的LCD

在低端的嵌入式平台上，内存只有几十K，没有足够的内存使用framebuffer，通常直接向寄存器中写入坐标和颜色数据。lcd\_reg.inc提供了基于寄存器实现的LCD，用它实现不同平台的LCD时，只需要提供两个宏即可：

* set\_window\_func 设置要写入颜色数据的区域，相对于每次设置坐标而言，可以极大提高工作效率。
* write\_data\_func 写入颜色数据。

下面是STMF103ze上LCD的实现，这里把set\_window\_func定义为TFT\_SetWindow，把write\_data\_func定义为TFT\_WriteData:

```
#include "gui.h"
#include "lcd_driver.h"

#include "base/mem.h"
#include "lcd/lcd_reg.h"

typedef uint16_t pixel_t;
#define set_window_func TFT_SetWindow
#define write_data_func TFT_WriteData

#include "blend/rgb565.inc"
#include "blend/pixel_ops.inc"
#include "lcd/lcd_reg.inc"
```

基于寄存器实现的实现有几个限制：

* 由于内存和CPU性能的问题，不提供任何类型的动画。
* 由于读取LCD当前内容速度很慢，所以需要与底色进行混合时，由GUI自己处理(APP无需关心)。

### 二、基于framebuffer实现的LCD

这是在嵌入式平台上最常见的方式。一般有两个framebuffer，一个称为online framebuffer，一个称为offline framebuffer。online framebuffer是当前现实的内容，offline framebuffer是GUI当前正在绘制的内容。lcd\_mem\_rgb565提供了rgb565格式的LCD实现，lcd\_mem\_rgba8888提供了rgba8888格式的LCD实现，它们都是在lcd\_mem.inc基础上实现的，要增加新的格式也是很方便的。

下面是STMF429上LCD的实现：

```
extern u32 *ltdc_framebuf[2];
#define online_fb_addr (uint8_t*)ltdc_framebuf[0]
#define offline_fb_addr (uint8_t*)ltdc_framebuf[1]

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
  return lcd_mem_rgb565_create_double_fb(w, h, online_fb_addr, offline_fb_addr);
}
```

### 三、基于vgcanvas实现的LCD

在支持OpenGL 3D硬件加速的平台上(如PC和手机)，我们使用nanovg把OpenGL封装成vgcanvas的接口，在vgcanvas基础之上实现LCD。lcd\_vgcanvas.inc将vgcanvas封装成LCD的接口，这里出于可移植性考虑，并没有直接基于nanovg的函数，而是基于vgcanvas的接口，所以在没有GPU时，如果CPU够强大，也是可以基于agg/picasso去实现的LCD。

> 这种方式实现，一般不会在嵌入平台上使用，读者不需要关注它。

### 总结

以上几种实现方式，基本上涵盖了最常用的场景，所以在移植到新的平台时，并不需要在实现LCD接口上费多少功夫。



