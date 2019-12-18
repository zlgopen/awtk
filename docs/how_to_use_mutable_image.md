# 如何使用 mutable\_image 控件

有时候，需要把一个视频或者摄像头的数据绘画到屏幕上面。

这个时候就可以使用 mutable\_image 控件来实现该功能了。

 
## 一、基本用法

一般情况下用户只需要通过 mutable\_image\_set\_prepare\_image 函数注册绘制 bitmap 的函数就可以了，例如下面示例中注册 mutable\_image\_prepare\_image 函数，该函数不断的给 mutable\_image 控件的 bitmap 填充红色，用户也可以在该函数中绘画其他的图像数据，就可以达到动态播放视频或者摄像头的效果。

#### 备注：

> 一般情况下 mutable\_image 控件的 bitmap 的位图格式是 lcd 的格式。

#### 示例：

```
#include "awtk.h"
#include "ext_widgets/mutable_image/mutable_image.h"

ret_t mutable_image_prepare_image(void* ctx, bitmap_t* image) {
  uint32_t i = 0;
  uint32_t bpp = 0;
  uint32_t size = 0;
  uint8_t* image_data = NULL;
  bpp = bitmap_get_bpp(image);
  size = image->h * bitmap_get_line_length(image);
  
#ifdef TK_GRAPHIC_BUFFER_H
  /* 新版本 AWTK 使用 bitmap_lock_buffer_for_write 函数来获取位图的数据 */
  image_data = bitmap_lock_buffer_for_write(image);
#else
  /* 旧版本的 AWTK 直接通过 bitmap 成员变量就可以获取位图的数据 */
  image_data = (uint8_t*)(image->data);
#endif

  /* 
   * 假设 lcd 的格式为 BGRA8888 ,那么 bpp 应该为4 
   * image_data 是 mutable_image 控件的 bitmap 的位图数据
   * 这里给 image_data 设置为蓝色，当然也可以设置摄像头数据或者视频数据
   */
  for(;i < size; i += bpp, image_data += bpp) {
      image_data[0] = 0xff;
      image_data[1] = 0x00;
      image_data[2] = 0x00;
      image_data[3] = 0xff;
  }
  
  return RET_OK;
}

ret_t application_init() {

  tk_ext_widgets_init();

  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  
  /* 创建 mutable_image 控件 */
  widget_t* mutable = mutable_image_create(win, 0, 0, win->w, win->h);
  
  /* 注册 mutable_image 控件的绘制图像回调函数 */
  mutable_image_set_prepare_image(mutable, mutable_image_prepare_image, NULL);

  return RET_OK;
}
```

> 完整的用法请参考：https://github.com/zlgopen/awtk-c-demos/blob/master/demos/mutable_image.c


## 二、扩展用法

在某些平台下，支持多个 lcd 的硬件 framebuffer ，这些硬件 framebuffer 可以在硬件绘画的时候做合成到 lcd ，这样子可以速度会软件合成要快的多，所以 mutable\_image 控件也支持该情况。

#### 备注：

如果调用了 mutable\_image\_set\_framebuffer 函数设置硬件 framebuffer 了， mutable\_image 控件就不会在 AWTK 的 GUI 上面显示了。

#### 注意：

该用法只适用于多个 lcd 硬件 framebuffer 的平台，如果单 framebuffer 的平台就只能使用上面提供的基本用法了。

#### 示例：

```
#include "awtk.h"
#include "ext_widgets/mutable_image/mutable_image.h"

/* 假设 DEVICE_FB 宏的地址为 lcd 的硬件 framebuffer 地址 */
#define DEVICE_FB (uint8_t*)0XC0000000

ret_t mutable_image_prepare_image(void* ctx, bitmap_t* image) {
  uint32_t i = 0;
  uint32_t bpp = 0;
  uint32_t size = 0;
  uint8_t* image_data = NULL;
  bpp = bitmap_get_bpp(image);
  size = image->h * bitmap_get_line_length(image);
  
#ifdef TK_GRAPHIC_BUFFER_H
  /* 新版本 AWTK 使用 bitmap_lock_buffer_for_write 函数来获取位图的数据 */
  image_data = bitmap_lock_buffer_for_write(image);
#else
  /* 旧版本的 AWTK 直接通过 bitmap 成员变量就可以获取位图的数据 */
  image_data = (uint8_t*)(image->data);
#endif

  /* 
   * 把带有透明度的蓝色填充到 lcd 的硬件 framebuffer 中
   * image_data 是 mutable_image 控件的 bitmap 的位图数据
   * 这里的 image 的位图格式和长宽分别，是在 mutable_image_set_framebuffer 函数中设置的
   * 这里给 image_data 设置为蓝色，当然也可以设置摄像头数据或者视频数据
   * 这里的 image_data 的地址就是上面 lcd 的硬件 framebuffer 地址
   */
  for(;i < size; i += bpp, image_data += bpp) {
      image_data[0] = 0xff;
      image_data[1] = 0x00;
      image_data[2] = 0x00;
      image_data[3] = 0xa0;
  }
  
  return RET_OK;
}

ret_t application_init() {

  tk_ext_widgets_init();

  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  
  /* 创建 mutable_image 控件 */
  widget_t* mutable = mutable_image_create(win, 0, 0, win->w, win->h);
  
  /* 注册 mutable_image 控件的绘制图像回调函数 */
  mutable_image_set_prepare_image(mutable, mutable_image_prepare_image, NULL);
  
  /* mutable_image 控件设置位图格式为 BGRA8888 的 lcd 的硬件 framebuffer，并且长宽分别为 win->w 和 win->h 。 */
  mutable_image_set_framebuffer(mutable, win->w, win->h, BITMAP_FMT_BGRA8888, DEVICE_FB)
  
  return RET_OK;
}
```

