## 优化技巧

### 一、优化提示

* memcpy是最快的贴图方式，所以对于不透明的图片，尽量保持与Framebuffer一致的格式，这样可以直接使用memcpy贴图。

> 比如FrameBuffer的格式是BGR565，请定义宏WITH\_BITMAP\_BGR565。

* 背景图片使用JPG格式或者不透明的PNG文件。

* 旋转对于性能影响非常大，除非支持硬件加速旋转，最好不要使用旋转功能。

* 缩放对于性能影响较大，尽量按按原图大小显示。

* 如果支持硬件加速，确保(半)透明的图片解码成硬件支持的格式。

> 比如对于PXP/DMA2D硬件加速，请定义宏WITH\_BITMAP\_BGRA。

* memcpy的速度对于AWTK非常重要。如果显示速度不理想，请测试一下memcpy的速度。

> 如果memcpy的速度不理想，请检查一下GUI使用的内存是否配置正确(如cache等)。

* 在窗口动画中，平移动画是最快的动画。所以请优先采用水平平移、垂直平移、底部弹出和顶部弹出几种动画。在没有硬件加速时，不要使用缩放和淡入的窗口动画。

* 在控件动画中，平移动画和值动画是最快的，淡入/淡出其次，缩放和旋转是最慢的。PXP/DMA2D加速不能用于加速缩放和旋转，所以除非有GPU，否则缩放和旋转动画只适用于少量小图片。

* 在不旋转和缩放的情况下，SVG图片比普通图片慢，但在旋转和缩放的情况下，SVG比普通图片快数倍。在仪表指针等需要旋转场景中，使用SVG图片可以显著提高性能。

* 绘制一帧需要的时间是一个重要指标。

> 可以在src/base/window\_manager.c:window\_manager\_paint\_normal中查看wm->last\_paint\_cost的值。

* 排除法有时能快速定位性能瓶颈。

> 1.怀疑字体渲染太慢? 让lcd\_draw\_glyph直接返回，看看效果如何。

> 2.怀疑图片渲染太慢? lcd\_draw\_image直接返回，看看效果如何。

> 3.怀疑填充颜色太慢? lcd\_fill\_rect直接返回，看看效果如何。

> 4.怀疑硬件加速没有效果，用纯软件渲染试试，对比一下渲染一帧需要的时间。

* 使用3个framebuffer可以避免GUI等待和拷贝，能够有效提高帧率，有条件时尽量启用。

* 猜测往往是错误的，请用函数time\_now\_ms去度量你怀疑的地方。

### 二、工具

如果定义全局的宏(在工程中定义)ENABLE\_PERFORMANCE\_PROFILE，会使用lcd\_profile对全部绘制函数(图片、文字和填充等等)进行时间统计。当绘制一帧需要的时间超过25ms，会打印一些信息:

```
#####################################
src/base/lcd_profile.c:326
-------------------------------------
  total_cost=30
  draw_image_cost=18 times=1
  draw_text_cost=0 times=15
  fill_cost=0 times=15
  stroke_cost=0 times=15
  end_frame_cost=10
-------------------------------------
```


也可以在lcd\_profile\_end\_frame函数里设置一个断点，从profile对象上能查看到更详细的统计信息：

```
  uint32_t total_cost;
  uint32_t swap_cost;
  uint32_t flush_cost;

  uint32_t draw_image_cost;
  uint32_t draw_image_times;
  uint32_t draw_image_pixels;

  uint32_t draw_text_cost;
  uint32_t draw_text_times;
  uint32_t draw_text_chars;

  uint32_t fill_cost;
  uint32_t fill_times;
  uint32_t fill_pixels;

  uint32_t stroke_cost;
  uint32_t stroke_times;
  uint32_t stroke_pixels;
```

>参考[lcd_profile.c](https://github.com/zlgopen/awtk/blob/master/src/base/lcd_profile.c)









