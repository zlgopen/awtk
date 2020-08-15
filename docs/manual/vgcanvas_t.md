## vgcanvas\_t
### 概述
矢量图画布抽象基类。

具体实现时可以使用agg，nanovg, cairo和skia等方式。

cairo和skia体积太大，不适合嵌入式平台，但在PC平台也是一种选择。

目前我们只提供了基于nanovg的实现，支持软件渲染和硬件渲染。

我们对nanovg进行了一些改进:

* 可以用agg/agge实现软件渲染(暂时不支持文本绘制)。

* 可以用bgfx使用DirectX(Windows平台)和Metal(iOS)平台硬件加速。

![image](images/vgcanvas_t_0.png)


示例：

```c
vgcanvas_t* vg = canvas_get_vgcanvas(c);
vgcanvas_save(vg);
vgcanvas_translate(vg, 0, 100);

vgcanvas_set_line_width(vg, 1);
vgcanvas_set_fill_color(vg, color_init(0xff, 0, 0, 0xff));
vgcanvas_rect(vg, 5, 5, 100, 100);
vgcanvas_fill(vg);
vgcanvas_restore(vg);
```

>请参考：https://www.w3schools.com/tags/ref_canvas.asp
----------------------------------
### 函数
<p id="vgcanvas_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#vgcanvas_t_vgcanvas_arc">vgcanvas\_arc</a> | 生成一条圆弧。 |
| <a href="#vgcanvas_t_vgcanvas_arc_to">vgcanvas\_arc\_to</a> | 生成一条圆弧路径到指定点。 |
| <a href="#vgcanvas_t_vgcanvas_begin_frame">vgcanvas\_begin\_frame</a> | 开始绘制，系统内部调用。 |
| <a href="#vgcanvas_t_vgcanvas_begin_path">vgcanvas\_begin\_path</a> | 清除之前的路径，并重新开始一条路径。 |
| <a href="#vgcanvas_t_vgcanvas_bezier_to">vgcanvas\_bezier\_to</a> | 生成一条三次贝塞尔曲线。 |
| <a href="#vgcanvas_t_vgcanvas_cast">vgcanvas\_cast</a> | 转换为vgcanvas对象(供脚本语言使用)。 |
| <a href="#vgcanvas_t_vgcanvas_clear_cache">vgcanvas\_clear\_cache</a> | 释放vgcanvas对象的缓冲数据。 |
| <a href="#vgcanvas_t_vgcanvas_clear_rect">vgcanvas\_clear\_rect</a> | 用颜色清除指定矩形区域。 |
| <a href="#vgcanvas_t_vgcanvas_clip_rect">vgcanvas\_clip\_rect</a> | 矩形裁剪。 |
| <a href="#vgcanvas_t_vgcanvas_close_path">vgcanvas\_close\_path</a> | 闭合路径。 |
| <a href="#vgcanvas_t_vgcanvas_create">vgcanvas\_create</a> | 创建vgcanvas。 |
| <a href="#vgcanvas_t_vgcanvas_destroy">vgcanvas\_destroy</a> | 销毁vgcanvas对象。 |
| <a href="#vgcanvas_t_vgcanvas_draw_icon">vgcanvas\_draw\_icon</a> | 绘制图标。 |
| <a href="#vgcanvas_t_vgcanvas_draw_image">vgcanvas\_draw\_image</a> | 绘制图片。 |
| <a href="#vgcanvas_t_vgcanvas_ellipse">vgcanvas\_ellipse</a> | 生成一个椭圆路径。 |
| <a href="#vgcanvas_t_vgcanvas_end_frame">vgcanvas\_end\_frame</a> | 结束绘制。系统内部调用。 |
| <a href="#vgcanvas_t_vgcanvas_fill">vgcanvas\_fill</a> | 填充多边形。 |
| <a href="#vgcanvas_t_vgcanvas_fill_text">vgcanvas\_fill\_text</a> | 绘制文本。 |
| <a href="#vgcanvas_t_vgcanvas_flush">vgcanvas\_flush</a> | flush |
| <a href="#vgcanvas_t_vgcanvas_get_height">vgcanvas\_get\_height</a> | 获取高度。 |
| <a href="#vgcanvas_t_vgcanvas_get_text_metrics">vgcanvas\_get\_text\_metrics</a> | 获取当前字体的度量信息。 |
| <a href="#vgcanvas_t_vgcanvas_get_width">vgcanvas\_get\_width</a> | 获取宽度。 |
| <a href="#vgcanvas_t_vgcanvas_intersect_clip_rect">vgcanvas\_intersect\_clip\_rect</a> | 设置一个与前一个裁剪区做交集的矩形裁剪区。 |
| <a href="#vgcanvas_t_vgcanvas_is_point_in_path">vgcanvas\_is\_point\_in\_path</a> | 检查点是否在当前路径中。 |
| <a href="#vgcanvas_t_vgcanvas_line_to">vgcanvas\_line\_to</a> | 生成一条线段(从当前点到目标点)。 |
| <a href="#vgcanvas_t_vgcanvas_measure_text">vgcanvas\_measure\_text</a> | 测量文本的宽度。 |
| <a href="#vgcanvas_t_vgcanvas_move_to">vgcanvas\_move\_to</a> | 移动当前点到指定点。 |
| <a href="#vgcanvas_t_vgcanvas_paint">vgcanvas\_paint</a> | 用图片填充/画多边形(可能存在可移植性问题，除非必要请勿使用)。 |
| <a href="#vgcanvas_t_vgcanvas_path_winding">vgcanvas\_path\_winding</a> | 设置路径填充实心与否。 |
| <a href="#vgcanvas_t_vgcanvas_quad_to">vgcanvas\_quad\_to</a> | 生成一条二次贝塞尔曲线。 |
| <a href="#vgcanvas_t_vgcanvas_rect">vgcanvas\_rect</a> | 生成一个矩形路径。 |
| <a href="#vgcanvas_t_vgcanvas_reinit">vgcanvas\_reinit</a> | 重新初始化，系统内部调用。 |
| <a href="#vgcanvas_t_vgcanvas_reset">vgcanvas\_reset</a> | 重置状态。 |
| <a href="#vgcanvas_t_vgcanvas_restore">vgcanvas\_restore</a> | 恢复上次save的状态。 |
| <a href="#vgcanvas_t_vgcanvas_rotate">vgcanvas\_rotate</a> | 旋转。 |
| <a href="#vgcanvas_t_vgcanvas_rounded_rect">vgcanvas\_rounded\_rect</a> | 生成一个圆角矩形路径。 |
| <a href="#vgcanvas_t_vgcanvas_save">vgcanvas\_save</a> | 保存当前的状态。如颜色和矩阵等信息。 |
| <a href="#vgcanvas_t_vgcanvas_scale">vgcanvas\_scale</a> | 缩放。 |
| <a href="#vgcanvas_t_vgcanvas_set_antialias">vgcanvas\_set\_antialias</a> | 设置是否启用反走样。 |
| <a href="#vgcanvas_t_vgcanvas_set_fill_color">vgcanvas\_set\_fill\_color</a> | 设置填充颜色。 |
| <a href="#vgcanvas_t_vgcanvas_set_fill_color_str">vgcanvas\_set\_fill\_color\_str</a> | 设置填充颜色。 |
| <a href="#vgcanvas_t_vgcanvas_set_fill_linear_gradient">vgcanvas\_set\_fill\_linear\_gradient</a> | 设置填充颜色为线性渐变色。 |
| <a href="#vgcanvas_t_vgcanvas_set_fill_radial_gradient">vgcanvas\_set\_fill\_radial\_gradient</a> | 设置填充颜色为径向渐变色。 |
| <a href="#vgcanvas_t_vgcanvas_set_font">vgcanvas\_set\_font</a> | 设置字体的名称。 |
| <a href="#vgcanvas_t_vgcanvas_set_font_size">vgcanvas\_set\_font\_size</a> | 设置字体的大小。 |
| <a href="#vgcanvas_t_vgcanvas_set_global_alpha">vgcanvas\_set\_global\_alpha</a> | 设置全局透明度。 |
| <a href="#vgcanvas_t_vgcanvas_set_line_cap">vgcanvas\_set\_line\_cap</a> | 设置line cap。 |
| <a href="#vgcanvas_t_vgcanvas_set_line_join">vgcanvas\_set\_line\_join</a> | 设置line join。 |
| <a href="#vgcanvas_t_vgcanvas_set_line_width">vgcanvas\_set\_line\_width</a> | 设置线条的宽度。 |
| <a href="#vgcanvas_t_vgcanvas_set_miter_limit">vgcanvas\_set\_miter\_limit</a> | 设置miter limit。 |
| <a href="#vgcanvas_t_vgcanvas_set_stroke_color">vgcanvas\_set\_stroke\_color</a> | 设置线条颜色。 |
| <a href="#vgcanvas_t_vgcanvas_set_stroke_color_str">vgcanvas\_set\_stroke\_color\_str</a> | 设置线条颜色。 |
| <a href="#vgcanvas_t_vgcanvas_set_stroke_linear_gradient">vgcanvas\_set\_stroke\_linear\_gradient</a> | 设置线条颜色为线性渐变色。 |
| <a href="#vgcanvas_t_vgcanvas_set_stroke_radial_gradient">vgcanvas\_set\_stroke\_radial\_gradient</a> | 设置线条颜色为径向渐变色。 |
| <a href="#vgcanvas_t_vgcanvas_set_text_align">vgcanvas\_set\_text\_align</a> | 设置文本水平对齐的方式。 |
| <a href="#vgcanvas_t_vgcanvas_set_text_baseline">vgcanvas\_set\_text\_baseline</a> | 设置文本垂直对齐的方式。 |
| <a href="#vgcanvas_t_vgcanvas_set_transform">vgcanvas\_set\_transform</a> | 设置变换矩阵。 |
| <a href="#vgcanvas_t_vgcanvas_stroke">vgcanvas\_stroke</a> | 画线。 |
| <a href="#vgcanvas_t_vgcanvas_transform">vgcanvas\_transform</a> | 变换矩阵。 |
| <a href="#vgcanvas_t_vgcanvas_translate">vgcanvas\_translate</a> | 平移。 |
### 属性
<p id="vgcanvas_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#vgcanvas_t_anti_alias">anti\_alias</a> | bool\_t | 是否启用反走样功能。 |
| <a href="#vgcanvas_t_fill_color">fill\_color</a> | color\_t | 填充颜色 |
| <a href="#vgcanvas_t_font">font</a> | char* | 字体。 |
| <a href="#vgcanvas_t_font_size">font\_size</a> | float\_t | 字体大小。 |
| <a href="#vgcanvas_t_global_alpha">global\_alpha</a> | float\_t | 全局alpha。 |
| <a href="#vgcanvas_t_h">h</a> | wh\_t | canvas的高度 |
| <a href="#vgcanvas_t_line_cap">line\_cap</a> | const char* | line\_cap。 |
| <a href="#vgcanvas_t_line_join">line\_join</a> | const char* | line\_join。 |
| <a href="#vgcanvas_t_line_width">line\_width</a> | float\_t | 线宽。 |
| <a href="#vgcanvas_t_miter_limit">miter\_limit</a> | float\_t | miter\_limit。 |
| <a href="#vgcanvas_t_ratio">ratio</a> | float\_t | 显示比例。 |
| <a href="#vgcanvas_t_stride">stride</a> | uint32\_t | 一行占的字节 |
| <a href="#vgcanvas_t_stroke_color">stroke\_color</a> | color\_t | 线条颜色 |
| <a href="#vgcanvas_t_text_align">text\_align</a> | const char* | 文本对齐方式。 |
| <a href="#vgcanvas_t_text_baseline">text\_baseline</a> | const char* | 文本基线。 |
| <a href="#vgcanvas_t_w">w</a> | wh\_t | canvas的宽度 |
#### vgcanvas\_arc 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_arc">生成一条圆弧。

* 函数原型：

```
ret_t vgcanvas_arc (vgcanvas_t* vg, float_t x, float_t y, float_t r, float_t start_angle, float_t end_angle, bool_t ccw);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | 原点x坐标。 |
| y | float\_t | 原点y坐标。 |
| r | float\_t | 半径。 |
| start\_angle | float\_t | 起始角度。 |
| end\_angle | float\_t | 结束角度。 |
| ccw | bool\_t | 是否逆时针。 |
#### vgcanvas\_arc\_to 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_arc_to">生成一条圆弧路径到指定点。

* 函数原型：

```
ret_t vgcanvas_arc_to (vgcanvas_t* vg, float_t x1, float_t y1, float_t x2, float_t y2, float_t r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x1 | float\_t | 起始点x坐标。 |
| y1 | float\_t | 起始点y坐标。 |
| x2 | float\_t | 结束点x坐标。 |
| y2 | float\_t | 结束点y坐标。 |
| r | float\_t | 半径。 |
#### vgcanvas\_begin\_frame 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_begin_frame">开始绘制，系统内部调用。

* 函数原型：

```
ret_t vgcanvas_begin_frame (vgcanvas_t* vg, rect_t* dirty_rect);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| dirty\_rect | rect\_t* | 需要绘制的区域。 |
#### vgcanvas\_begin\_path 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_begin_path">清除之前的路径，并重新开始一条路径。

* 函数原型：

```
ret_t vgcanvas_begin_path (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_bezier\_to 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_bezier_to">生成一条三次贝塞尔曲线。

* 函数原型：

```
ret_t vgcanvas_bezier_to (vgcanvas_t* vg, float_t cp1x, float_t cp1y, float_t cp2x, float_t cp2y, float_t x, float_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| cp1x | float\_t | 控制点1x坐标。 |
| cp1y | float\_t | 控制点1y坐标。 |
| cp2x | float\_t | 控制点2x坐标。 |
| cp2y | float\_t | 控制点3y坐标。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
#### vgcanvas\_cast 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_cast">转换为vgcanvas对象(供脚本语言使用)。

* 函数原型：

```
vgcanvas_t* vgcanvas_cast (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vgcanvas\_t* | vgcanvas对象。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_clear\_cache 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_clear_cache">释放vgcanvas对象的缓冲数据。

* 函数原型：

```
ret_t vgcanvas_clear_cache (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_clear\_rect 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_clear_rect">用颜色清除指定矩形区域。

* 函数原型：

```
ret_t vgcanvas_clear_rect (vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h, color_t c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
| w | float\_t | 宽度。 |
| h | float\_t | 高度。 |
| c | color\_t | 颜色。 |
#### vgcanvas\_clip\_rect 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_clip_rect">矩形裁剪。

* 函数原型：

```
ret_t vgcanvas_clip_rect (vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
| w | float\_t | 宽度。 |
| h | float\_t | 高度。 |
#### vgcanvas\_close\_path 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_close_path">闭合路径。

>闭合路径是指把起点和终点连接起来，形成一个封闭的多边形。

* 函数原型：

```
ret_t vgcanvas_close_path (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_create 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_create">创建vgcanvas。

* 函数原型：

```
vgcanvas_t vgcanvas_create (uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format, void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vgcanvas\_t | 返回vgcanvas |
| w | uint32\_t | 宽度 |
| h | uint32\_t | 高度 |
| stride | uint32\_t | 一行占用的字节数。 |
| format | bitmap\_format\_t | 如果data是framebuffer，format指定data的格式。 |
| data | void* | framebuffer或其它ctx。 |
#### vgcanvas\_destroy 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_destroy">销毁vgcanvas对象。

* 函数原型：

```
ret_t vgcanvas_destroy (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_draw\_icon 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_draw_icon">绘制图标。

绘制图标时会根据屏幕密度进行自动缩放，而绘制普通图片时不会。

* 函数原型：

```
ret_t vgcanvas_draw_icon (vgcanvas_t* vg, bitmap_t* img, float_t sx, float_t sy, float_t sw, float_t sh, float_t dx, float_t dy, float_t dw, float_t dh);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| img | bitmap\_t* | 图片。 |
| sx | float\_t | sx |
| sy | float\_t | sy |
| sw | float\_t | sw |
| sh | float\_t | sh |
| dx | float\_t | dx |
| dy | float\_t | dy |
| dw | float\_t | dw |
| dh | float\_t | dh |
#### vgcanvas\_draw\_image 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_draw_image">绘制图片。

* 函数原型：

```
ret_t vgcanvas_draw_image (vgcanvas_t* vg, bitmap_t* img, float_t sx, float_t sy, float_t sw, float_t sh, float_t dx, float_t dy, float_t dw, float_t dh);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| img | bitmap\_t* | 图片。 |
| sx | float\_t | sx |
| sy | float\_t | sy |
| sw | float\_t | sw |
| sh | float\_t | sh |
| dx | float\_t | dx |
| dy | float\_t | dy |
| dw | float\_t | dw |
| dh | float\_t | dh |
#### vgcanvas\_ellipse 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_ellipse">生成一个椭圆路径。

* 函数原型：

```
ret_t vgcanvas_ellipse (vgcanvas_t* vg, float_t x, float_t y, float_t rx, float_t ry);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
| rx | float\_t | 圆角半径。 |
| ry | float\_t | 圆角半径。 |
#### vgcanvas\_end\_frame 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_end_frame">结束绘制。系统内部调用。

* 函数原型：

```
ret_t vgcanvas_end_frame (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_fill 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_fill">填充多边形。

* 函数原型：

```
ret_t vgcanvas_fill (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_fill\_text 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_fill_text">绘制文本。

* 函数原型：

```
ret_t vgcanvas_fill_text (vgcanvas_t* vg, char* text, float_t x, float_t y, float_t max_width);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| text | char* | text |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
| max\_width | float\_t | 最大宽度。 |
#### vgcanvas\_flush 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_flush">flush

* 函数原型：

```
ret_t vgcanvas_flush (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_get\_height 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_get_height">获取高度。

* 函数原型：

```
wh_t vgcanvas_get_height (vgcanvas_t* vgcanvas);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wh\_t | 返回高度。 |
| vgcanvas | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_get\_text\_metrics 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_get_text_metrics">获取当前字体的度量信息。

* 函数原型：

```
ret_t vgcanvas_get_text_metrics (vgcanvas_t* vg, float_t* ascent, float_t* descent, float_t* line_hight);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| ascent | float\_t* | 用于返回ascent。 |
| descent | float\_t* | 用于返回descent。 |
| line\_hight | float\_t* | 用于返回line height。 |
#### vgcanvas\_get\_width 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_get_width">获取宽度。

* 函数原型：

```
wh_t vgcanvas_get_width (vgcanvas_t* vgcanvas);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wh\_t | 返回宽度。 |
| vgcanvas | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_intersect\_clip\_rect 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_intersect_clip_rect">设置一个与前一个裁剪区做交集的矩形裁剪区。
如果下面这种情况，则不能直接调用 rect_intersect 函数来做矩形交集和 vgcanvas_clip_rect 函数设置裁剪区，而采用本函数做交集。
由于缩放和旋转以及平移会导致 vg 的坐标系和上一个裁剪区的坐标系不同，
导致直接使用做交集的话，裁剪区会出错。

```
vgcanvas_clip_rect(vg, old_r.x, old_r.y, old_r.w, old_r.h);
vgcanvas_save(vg);
vgcanvas_scale(vg, scale_x, scale_y);
vgcanvas_rotate(vg, TK_D2R(15));
vgcanvas_intersect_clip_rect(vg, r.x, r.y, r.w, r.h);
..................
vgcanvas_restore(vg);
```

* 函数原型：

```
ret_t vgcanvas_intersect_clip_rect (vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
| w | float\_t | 宽度。 |
| h | float\_t | 高度。 |
#### vgcanvas\_is\_point\_in\_path 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_is_point_in_path">检查点是否在当前路径中。

* 函数原型：

```
bool_t vgcanvas_is_point_in_path (vgcanvas_t* vg, float_t x, float_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示在，否则表示不在。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
#### vgcanvas\_line\_to 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_line_to">生成一条线段(从当前点到目标点)。

* 函数原型：

```
ret_t vgcanvas_line_to (vgcanvas_t* vg, float_t x, float_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
#### vgcanvas\_measure\_text 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_measure_text">测量文本的宽度。

* 函数原型：

```
float_t vgcanvas_measure_text (vgcanvas_t* vg, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float\_t | 返回text的宽度。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| text | char* | text |
#### vgcanvas\_move\_to 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_move_to">移动当前点到指定点。

* 函数原型：

```
ret_t vgcanvas_move_to (vgcanvas_t* vg, float_t x, float_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
#### vgcanvas\_paint 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_paint">用图片填充/画多边形(可能存在可移植性问题，除非必要请勿使用)。
多边形的顶点必须在图片范围内，可以通过矩阵变化画到不同的位置。

* 函数原型：

```
ret_t vgcanvas_paint (vgcanvas_t* vg, bool_t stroke, bitmap_t* img);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| stroke | bool\_t | TRUE表示画线FALSE表示填充。 |
| img | bitmap\_t* | 图片。 |
#### vgcanvas\_path\_winding 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_path_winding">设置路径填充实心与否。

>CCW(1)为实心，CW(2)为镂空，设置其他则默认根据非零环绕规则判断(nonzero)。

* 函数原型：

```
ret_t vgcanvas_path_winding (vgcanvas_t* vg, bool_t dir);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| dir | bool\_t | 填充方法。 |
#### vgcanvas\_quad\_to 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_quad_to">生成一条二次贝塞尔曲线。

* 函数原型：

```
ret_t vgcanvas_quad_to (vgcanvas_t* vg, float_t cpx, float_t cpy, float_t x, float_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| cpx | float\_t | 控制点x坐标。 |
| cpy | float\_t | 控制点y坐标。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
#### vgcanvas\_rect 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_rect">生成一个矩形路径。

* 函数原型：

```
ret_t vgcanvas_rect (vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
| w | float\_t | 宽度。 |
| h | float\_t | 高度。 |
#### vgcanvas\_reinit 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_reinit">重新初始化，系统内部调用。

* 函数原型：

```
ret_t vgcanvas_reinit (vgcanvas_t* vg, uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format, void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| w | uint32\_t | 宽度 |
| h | uint32\_t | 高度 |
| stride | uint32\_t | 一行占用的字节数。 |
| format | bitmap\_format\_t | 如果data是framebuffer，format指定data的格式。 |
| data | void* | framebuffer或其它ctx。 |
#### vgcanvas\_reset 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_reset">重置状态。

* 函数原型：

```
ret_t vgcanvas_reset (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_restore 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_restore">恢复上次save的状态。

> save/restore必须配套使用，否则可能导致状态混乱。

* 函数原型：

```
ret_t vgcanvas_restore (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_rotate 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_rotate">旋转。

* 函数原型：

```
ret_t vgcanvas_rotate (vgcanvas_t* vg, float_t rad);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| rad | float\_t | 角度 |
#### vgcanvas\_rounded\_rect 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_rounded_rect">生成一个圆角矩形路径。

* 函数原型：

```
ret_t vgcanvas_rounded_rect (vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h, float_t r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x坐标。 |
| y | float\_t | y坐标。 |
| w | float\_t | 宽度。 |
| h | float\_t | 高度。 |
| r | float\_t | 圆角半径。 |
#### vgcanvas\_save 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_save">保存当前的状态。如颜色和矩阵等信息。

> save/restore必须配套使用，否则可能导致状态混乱。

* 函数原型：

```
ret_t vgcanvas_save (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_scale 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_scale">缩放。

* 函数原型：

```
ret_t vgcanvas_scale (vgcanvas_t* vg, float_t x, float_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x方向缩放比例。 |
| y | float\_t | y方向缩放比例。 |
#### vgcanvas\_set\_antialias 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_antialias">设置是否启用反走样。

* 函数原型：

```
ret_t vgcanvas_set_antialias (vgcanvas_t* vg, bool_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| value | bool\_t | 是否启用反走样。 |
#### vgcanvas\_set\_fill\_color 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_fill_color">设置填充颜色。

* 函数原型：

```
ret_t vgcanvas_set_fill_color (vgcanvas_t* vg, color_t color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| color | color\_t | 颜色。 |
#### vgcanvas\_set\_fill\_color\_str 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_fill_color_str">设置填充颜色。

* 函数原型：

```
ret_t vgcanvas_set_fill_color_str (vgcanvas_t* vg, const char* color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| color | const char* | 颜色。 |
#### vgcanvas\_set\_fill\_linear\_gradient 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_fill_linear_gradient">设置填充颜色为线性渐变色。

* 函数原型：

```
ret_t vgcanvas_set_fill_linear_gradient (vgcanvas_t* vg, float_t sx, float_t sy, float_t ex, float_t ey, color_t icolor, color_t ocolor);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| sx | float\_t | start x |
| sy | float\_t | start y |
| ex | float\_t | end x |
| ey | float\_t | end y |
| icolor | color\_t | 开始颜色。 |
| ocolor | color\_t | 结束颜色。 |
#### vgcanvas\_set\_fill\_radial\_gradient 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_fill_radial_gradient">设置填充颜色为径向渐变色。

* 函数原型：

```
ret_t vgcanvas_set_fill_radial_gradient (vgcanvas_t* vg, float_t cx, float_t cy, float_t inr, float_t outr, color_t icolor, color_t ocolor);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| cx | float\_t | center x |
| cy | float\_t | center y |
| inr | float\_t | 内环半径 |
| outr | float\_t | 外环半径。 |
| icolor | color\_t | 开始颜色。 |
| ocolor | color\_t | 结束颜色。 |
#### vgcanvas\_set\_font 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_font">设置字体的名称。

* 函数原型：

```
ret_t vgcanvas_set_font (vgcanvas_t* vg, char* font);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| font | char* | 字体名称。 |
#### vgcanvas\_set\_font\_size 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_font_size">设置字体的大小。

* 函数原型：

```
ret_t vgcanvas_set_font_size (vgcanvas_t* vg, float_t font);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| font | float\_t | 字体大小。 |
#### vgcanvas\_set\_global\_alpha 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_global_alpha">设置全局透明度。

* 函数原型：

```
ret_t vgcanvas_set_global_alpha (vgcanvas_t* vg, float_t alpha);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| alpha | float\_t | global alpha。 |
#### vgcanvas\_set\_line\_cap 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_line_cap">设置line cap。

* 函数原型：

```
ret_t vgcanvas_set_line_cap (vgcanvas_t* vg, char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| value | char* | 取值：butt|round|square，必须为常量字符串。 |
#### vgcanvas\_set\_line\_join 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_line_join">设置line join。

* 函数原型：

```
ret_t vgcanvas_set_line_join (vgcanvas_t* vg, char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| value | char* | 取值：bevel|round|miter，必须为常量字符串。 |
#### vgcanvas\_set\_line\_width 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_line_width">设置线条的宽度。

* 函数原型：

```
ret_t vgcanvas_set_line_width (vgcanvas_t* vg, float_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| value | float\_t | 线宽。 |
#### vgcanvas\_set\_miter\_limit 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_miter_limit">设置miter limit。

* 函数原型：

```
ret_t vgcanvas_set_miter_limit (vgcanvas_t* vg, float_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| value | float\_t | miter limit |
#### vgcanvas\_set\_stroke\_color 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_stroke_color">设置线条颜色。

* 函数原型：

```
ret_t vgcanvas_set_stroke_color (vgcanvas_t* vg, color_t color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| color | color\_t | 颜色。 |
#### vgcanvas\_set\_stroke\_color\_str 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_stroke_color_str">设置线条颜色。

* 函数原型：

```
ret_t vgcanvas_set_stroke_color_str (vgcanvas_t* vg, const char* color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| color | const char* | 颜色。 |
#### vgcanvas\_set\_stroke\_linear\_gradient 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_stroke_linear_gradient">设置线条颜色为线性渐变色。

* 函数原型：

```
ret_t vgcanvas_set_stroke_linear_gradient (vgcanvas_t* vg, float_t sx, float_t sy, float_t ex, float_t ey, color_t icolor, color_t ocolor);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| sx | float\_t | start x |
| sy | float\_t | start y |
| ex | float\_t | end x |
| ey | float\_t | end y |
| icolor | color\_t | 开始颜色。 |
| ocolor | color\_t | 结束颜色。 |
#### vgcanvas\_set\_stroke\_radial\_gradient 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_stroke_radial_gradient">设置线条颜色为径向渐变色。

* 函数原型：

```
ret_t vgcanvas_set_stroke_radial_gradient (vgcanvas_t* vg, float_t cx, float_t cy, float_t inr, float_t outr, color_t icolor, color_t ocolor);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| cx | float\_t | center x |
| cy | float\_t | center y |
| inr | float\_t | 内环半径 |
| outr | float\_t | 外环半径。 |
| icolor | color\_t | 开始颜色。 |
| ocolor | color\_t | 结束颜色。 |
#### vgcanvas\_set\_text\_align 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_text_align">设置文本水平对齐的方式。

* 函数原型：

```
ret_t vgcanvas_set_text_align (vgcanvas_t* vg, char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| value | char* | 取值：left|center|right，必须为常量字符串。 |
#### vgcanvas\_set\_text\_baseline 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_text_baseline">设置文本垂直对齐的方式。

* 函数原型：

```
ret_t vgcanvas_set_text_baseline (vgcanvas_t* vg, char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| value | char* | 取值：top|middle|bottom，必须为常量字符串。 |
#### vgcanvas\_set\_transform 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_set_transform">设置变换矩阵。

* 函数原型：

```
ret_t vgcanvas_set_transform (vgcanvas_t* vg, float_t a, float_t b, float_t c, float_t d, float_t e, float_t f);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| a | float\_t | a |
| b | float\_t | b |
| c | float\_t | c |
| d | float\_t | d |
| e | float\_t | e |
| f | float\_t | f |
#### vgcanvas\_stroke 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_stroke">画线。

* 函数原型：

```
ret_t vgcanvas_stroke (vgcanvas_t* vg);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
#### vgcanvas\_transform 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_transform">变换矩阵。

* 函数原型：

```
ret_t vgcanvas_transform (vgcanvas_t* vg, float_t a, float_t b, float_t c, float_t d, float_t e, float_t f);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| a | float\_t | a |
| b | float\_t | b |
| c | float\_t | c |
| d | float\_t | d |
| e | float\_t | e |
| f | float\_t | f |
#### vgcanvas\_translate 函数
-----------------------

* 函数功能：

> <p id="vgcanvas_t_vgcanvas_translate">平移。

* 函数原型：

```
ret_t vgcanvas_translate (vgcanvas_t* vg, float_t x, float_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vg | vgcanvas\_t* | vgcanvas对象。 |
| x | float\_t | x方向偏移。 |
| y | float\_t | y方向偏移。 |
#### anti\_alias 属性
-----------------------
> <p id="vgcanvas_t_anti_alias">是否启用反走样功能。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### fill\_color 属性
-----------------------
> <p id="vgcanvas_t_fill_color">填充颜色

* 类型：color\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### font 属性
-----------------------
> <p id="vgcanvas_t_font">字体。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### font\_size 属性
-----------------------
> <p id="vgcanvas_t_font_size">字体大小。

* 类型：float\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### global\_alpha 属性
-----------------------
> <p id="vgcanvas_t_global_alpha">全局alpha。

* 类型：float\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### h 属性
-----------------------
> <p id="vgcanvas_t_h">canvas的高度

* 类型：wh\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### line\_cap 属性
-----------------------
> <p id="vgcanvas_t_line_cap">line\_cap。
@see http://www.w3school.com.cn/tags/canvas_linecap.asp

* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### line\_join 属性
-----------------------
> <p id="vgcanvas_t_line_join">line\_join。
@see http://www.w3school.com.cn/tags/canvas_linejoin.asp

* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### line\_width 属性
-----------------------
> <p id="vgcanvas_t_line_width">线宽。

* 类型：float\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### miter\_limit 属性
-----------------------
> <p id="vgcanvas_t_miter_limit">miter\_limit。
@see http://www.w3school.com.cn/tags/canvas_miterlimit.asp

* 类型：float\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### ratio 属性
-----------------------
> <p id="vgcanvas_t_ratio">显示比例。

* 类型：float\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### stride 属性
-----------------------
> <p id="vgcanvas_t_stride">一行占的字节

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### stroke\_color 属性
-----------------------
> <p id="vgcanvas_t_stroke_color">线条颜色

* 类型：color\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### text\_align 属性
-----------------------
> <p id="vgcanvas_t_text_align">文本对齐方式。

@see http://www.w3school.com.cn/tags/canvas_textalign.asp

* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### text\_baseline 属性
-----------------------
> <p id="vgcanvas_t_text_baseline">文本基线。

@see http://www.w3school.com.cn/tags/canvas_textbaseline.asp

* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### w 属性
-----------------------
> <p id="vgcanvas_t_w">canvas的宽度

* 类型：wh\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
