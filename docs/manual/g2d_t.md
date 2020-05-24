## g2d\_t
### 概述
2D加速接口。
----------------------------------
### 函数
<p id="g2d_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#g2d_t_g2d_blend_image">g2d\_blend\_image</a> | 把图片指定的区域渲染到framebuffer指定的区域，src的大小和dst的大小不一致则进行缩放。 |
| <a href="#g2d_t_g2d_copy_image">g2d\_copy\_image</a> | 把图片指定的区域拷贝到framebuffer中。 |
| <a href="#g2d_t_g2d_fill_rect">g2d\_fill\_rect</a> | 用颜色填充指定的区域。 |
| <a href="#g2d_t_g2d_rotate_image">g2d\_rotate\_image</a> | 把图片指定的区域进行旋转并拷贝到framebuffer相应的区域，本函数主要用于辅助实现横屏和竖屏的切换，一般支持90度旋转即可。 |
#### g2d\_blend\_image 函数
-----------------------

* 函数功能：

> <p id="g2d_t_g2d_blend_image">把图片指定的区域渲染到framebuffer指定的区域，src的大小和dst的大小不一致则进行缩放。
1.硬件不支持缩放，则返回NOT_IMPL。
2.硬件不支持全局alpha，global_alpha!=0xff时返回NOT_IMPL。

* 函数原型：

```
ret_t g2d_blend_image (bitmap_t* fb, bitmap_t* img, rect_t* dst, rect_t* src, uint8_t global_alpha);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败，返回失败则上层用软件实现。 |
| fb | bitmap\_t* | framebuffer对象。 |
| img | bitmap\_t* | 图片对象。 |
| dst | rect\_t* | 目的区域。 |
| src | rect\_t* | 源区域。 |
| global\_alpha | uint8\_t | 全局alpha。 |
#### g2d\_copy\_image 函数
-----------------------

* 函数功能：

> <p id="g2d_t_g2d_copy_image">把图片指定的区域拷贝到framebuffer中。

* 函数原型：

```
ret_t g2d_copy_image (bitmap_t* fb, bitmap_t* img, rect_t* src, xy_t dx, xy_t dy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败，返回失败则上层用软件实现。 |
| fb | bitmap\_t* | framebuffer对象。 |
| img | bitmap\_t* | 图片对象。 |
| src | rect\_t* | 要拷贝的区域。 |
| dx | xy\_t | 目标位置的x坐标。 |
| dy | xy\_t | 目标位置的y坐标。 |
#### g2d\_fill\_rect 函数
-----------------------

* 函数功能：

> <p id="g2d_t_g2d_fill_rect">用颜色填充指定的区域。

* 函数原型：

```
ret_t g2d_fill_rect (bitmap_t* fb, rect_t* dst, color_t c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败，返回失败则上层用软件实现。 |
| fb | bitmap\_t* | framebuffer对象。 |
| dst | rect\_t* | 要填充的目标区域。 |
| c | color\_t | 颜色。 |
#### g2d\_rotate\_image 函数
-----------------------

* 函数功能：

> <p id="g2d_t_g2d_rotate_image">把图片指定的区域进行旋转并拷贝到framebuffer相应的区域，本函数主要用于辅助实现横屏和竖屏的切换，一般支持90度旋转即可。

* 函数原型：

```
ret_t g2d_rotate_image (bitmap_t* fb, bitmap_t* img, rect_t* src, lcd_orientation_t o);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败，返回失败则上层用软件实现。 |
| fb | bitmap\_t* | framebuffer对象。 |
| img | bitmap\_t* | 图片对象。 |
| src | rect\_t* | 要旋转并拷贝的区域。 |
| o | lcd\_orientation\_t | 旋转角度(一般支持90度即可)。 |
