## lcd\_t
### 概述
显示设备抽象基类。
----------------------------------
### 函数
<p id="lcd_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#lcd_t_lcd_begin_frame">lcd\_begin\_frame</a> | 准备绘制。 |
| <a href="#lcd_t_lcd_destroy">lcd\_destroy</a> | 销毁lcd对象。 |
| <a href="#lcd_t_lcd_draw_glyph">lcd\_draw\_glyph</a> | 绘制字符。如果实现了measure_text/draw_text则不需要实现本函数。 |
| <a href="#lcd_t_lcd_draw_hline">lcd\_draw\_hline</a> | 绘制一条水平线。 |
| <a href="#lcd_t_lcd_draw_image">lcd\_draw\_image</a> | 绘制图片。 |
| <a href="#lcd_t_lcd_draw_image_matrix">lcd\_draw\_image\_matrix</a> | 绘制图片。 |
| <a href="#lcd_t_lcd_draw_points">lcd\_draw\_points</a> | 绘制一组点。 |
| <a href="#lcd_t_lcd_draw_text">lcd\_draw\_text</a> | 绘制字符。 |
| <a href="#lcd_t_lcd_draw_vline">lcd\_draw\_vline</a> | 绘制一条垂直线。 |
| <a href="#lcd_t_lcd_end_frame">lcd\_end\_frame</a> | 完成绘制，同步到显示设备。 |
| <a href="#lcd_t_lcd_fill_rect">lcd\_fill\_rect</a> | 绘制实心矩形。 |
| <a href="#lcd_t_lcd_get_clip_rect">lcd\_get\_clip\_rect</a> | 获取裁剪区域。 |
| <a href="#lcd_t_lcd_get_desired_bitmap_format">lcd\_get\_desired\_bitmap\_format</a> | 获取期望的位图格式。绘制期望的位图格式可以提高绘制性能。 |
| <a href="#lcd_t_lcd_get_height">lcd\_get\_height</a> | 获取高度。 |
| <a href="#lcd_t_lcd_get_point_color">lcd\_get\_point\_color</a> | 获取指定点的颜色，对于基于非FrameBuffer的LCD，返回当前的fill_color。 |
| <a href="#lcd_t_lcd_get_text_metrics">lcd\_get\_text\_metrics</a> | 获取当前字体的度量信息。 |
| <a href="#lcd_t_lcd_get_vgcanvas">lcd\_get\_vgcanvas</a> | 获取矢量图canvas。 |
| <a href="#lcd_t_lcd_get_width">lcd\_get\_width</a> | 获取宽度。 |
| <a href="#lcd_t_lcd_is_swappable">lcd\_is\_swappable</a> | 判读lcd是否支持swap。 |
| <a href="#lcd_t_lcd_measure_text">lcd\_measure\_text</a> | 测量字符串占用的宽度。 |
| <a href="#lcd_t_lcd_resize">lcd\_resize</a> | 基于SDL的PC软件，在SDL窗口resize时，需要调用本函数resize lcd。 |
| <a href="#lcd_t_lcd_set_clip_rect">lcd\_set\_clip\_rect</a> | 设置裁剪区域。 |
| <a href="#lcd_t_lcd_set_fill_color">lcd\_set\_fill\_color</a> | 设置填充颜色。 |
| <a href="#lcd_t_lcd_set_font_name">lcd\_set\_font\_name</a> | 设置字体名称。 |
| <a href="#lcd_t_lcd_set_font_size">lcd\_set\_font\_size</a> | 设置字体大小。 |
| <a href="#lcd_t_lcd_set_global_alpha">lcd\_set\_global\_alpha</a> | 设置全局alpha。 |
| <a href="#lcd_t_lcd_set_stroke_color">lcd\_set\_stroke\_color</a> | 设置线条颜色。 |
| <a href="#lcd_t_lcd_set_text_color">lcd\_set\_text\_color</a> | 设置文本颜色。 |
| <a href="#lcd_t_lcd_stroke_rect">lcd\_stroke\_rect</a> | 绘制矩形。 |
| <a href="#lcd_t_lcd_take_snapshot">lcd\_take\_snapshot</a> | 拍摄快照，一般用于窗口动画，只有framebuffer模式，才支持。 |
### 属性
<p id="lcd_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#lcd_t_draw_mode">draw\_mode</a> | lcd\_draw\_mode\_t | 绘制模式。 |
| <a href="#lcd_t_fill_color">fill\_color</a> | color\_t | 填充颜色 |
| <a href="#lcd_t_font_name">font\_name</a> | char* | 字体名称。 |
| <a href="#lcd_t_font_size">font\_size</a> | uint32\_t | 字体大小。 |
| <a href="#lcd_t_global_alpha">global\_alpha</a> | uint8\_t | 全局alpha |
| <a href="#lcd_t_height">height</a> | wh\_t | 屏幕的高度 |
| <a href="#lcd_t_last_update_time">last\_update\_time</a> | uint64\_t | last update time |
| <a href="#lcd_t_ratio">ratio</a> | float\_t | 屏幕密度。 |
| <a href="#lcd_t_stroke_color">stroke\_color</a> | color\_t | 线条颜色 |
| <a href="#lcd_t_support_dirty_rect">support\_dirty\_rect</a> | bool\_t | 是否支持脏矩形。 |
| <a href="#lcd_t_text_color">text\_color</a> | color\_t | 文本颜色 |
| <a href="#lcd_t_type">type</a> | lcd\_type\_t | LCD的类型。 |
| <a href="#lcd_t_w">w</a> | wh\_t | 屏幕的宽度 |
#### lcd\_begin\_frame 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_begin_frame">准备绘制。

* 函数原型：

```
ret_t lcd_begin_frame (lcd_t* lcd, rect_t* dirty_rect, lcd_draw_mode_t anim_mode);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| dirty\_rect | rect\_t* | 需要绘制的区域。 |
| anim\_mode | lcd\_draw\_mode\_t | 动画模式，如果可能，直接画到显存而不是离线的framebuffer。 |
#### lcd\_destroy 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_destroy">销毁lcd对象。

* 函数原型：

```
ret_t lcd_destroy (lcd_t* lcd);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
#### lcd\_draw\_glyph 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_draw_glyph">绘制字符。如果实现了measure_text/draw_text则不需要实现本函数。

* 函数原型：

```
ret_t lcd_draw_glyph (lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| glyph | glyph\_t* | 字模 |
| src | rect\_t* | 只绘制指定区域的部分。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
#### lcd\_draw\_hline 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_draw_hline">绘制一条水平线。

* 函数原型：

```
ret_t lcd_draw_hline (lcd_t* lcd, xy_t x, xy_t y, xy_t w);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| w | xy\_t | 直线宽度。 |
#### lcd\_draw\_image 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_draw_image">绘制图片。

* 函数原型：

```
ret_t lcd_draw_image (lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| img | bitmap\_t* | 图片。 |
| src | rect\_t* | 只绘制指定区域的部分。 |
| dst | rect\_t* | 绘制到目标区域。 |
#### lcd\_draw\_image\_matrix 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_draw_image_matrix">绘制图片。

* 函数原型：

```
ret_t lcd_draw_image_matrix (lcd_t* lcd, draw_image_info_t* info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| info | draw\_image\_info\_t* | 绘制参数。 |
#### lcd\_draw\_points 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_draw_points">绘制一组点。

* 函数原型：

```
ret_t lcd_draw_points (lcd_t* lcd, point_t* points, uint32_t nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| points | point\_t* | 要绘制的点集合。 |
| nr | uint32\_t | 点的个数。 |
#### lcd\_draw\_text 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_draw_text">绘制字符。

* 函数原型：

```
ret_t lcd_draw_text (lcd_t* lcd, const wchar_t* str, uint32_t nr, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| str | const wchar\_t* | 字符串。 |
| nr | uint32\_t | 字符数。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
#### lcd\_draw\_vline 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_draw_vline">绘制一条垂直线。

* 函数原型：

```
ret_t lcd_draw_vline (lcd_t* lcd, xy_t x, xy_t y, xy_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| h | xy\_t | 直线高度。 |
#### lcd\_end\_frame 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_end_frame">完成绘制，同步到显示设备。

* 函数原型：

```
ret_t lcd_end_frame (lcd_t* lcd);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
#### lcd\_fill\_rect 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_fill_rect">绘制实心矩形。

* 函数原型：

```
ret_t lcd_fill_rect (lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| w | wh\_t | 宽度。 |
| h | wh\_t | 高度。 |
#### lcd\_get\_clip\_rect 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_get_clip_rect">获取裁剪区域。

* 函数原型：

```
ret_t lcd_get_clip_rect (lcd_t* lcd, rect_t* rect);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| rect | rect\_t* | 裁剪区域。 |
#### lcd\_get\_desired\_bitmap\_format 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_get_desired_bitmap_format">获取期望的位图格式。绘制期望的位图格式可以提高绘制性能。

* 函数原型：

```
bitmap_format_t lcd_get_desired_bitmap_format (lcd_t* lcd);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bitmap\_format\_t | 返回期望的位图格式。 |
| lcd | lcd\_t* | lcd对象。 |
#### lcd\_get\_height 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_get_height">获取高度。

* 函数原型：

```
wh_t lcd_get_height (lcd_t* lcd);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wh\_t | 返回高度。 |
| lcd | lcd\_t* | lcd对象。 |
#### lcd\_get\_point\_color 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_get_point_color">获取指定点的颜色，对于基于非FrameBuffer的LCD，返回当前的fill_color。

* 函数原型：

```
color_t lcd_get_point_color (lcd_t* lcd, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | color\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
#### lcd\_get\_text\_metrics 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_get_text_metrics">获取当前字体的度量信息。

* 函数原型：

```
ret_t lcd_get_text_metrics (lcd_t* lcd, float_t* ascent, float_t* descent, float_t* line_hight);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| ascent | float\_t* | 用于返回ascent。 |
| descent | float\_t* | 用于返回descent。 |
| line\_hight | float\_t* | 用于返回line height。 |
#### lcd\_get\_vgcanvas 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_get_vgcanvas">获取矢量图canvas。

* 函数原型：

```
vgcanvas_t* lcd_get_vgcanvas (lcd_t* lcd);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vgcanvas\_t* | 返回矢量图canvas。 |
| lcd | lcd\_t* | lcd对象。 |
#### lcd\_get\_width 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_get_width">获取宽度。

* 函数原型：

```
wh_t lcd_get_width (lcd_t* lcd);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wh\_t | 返回宽度。 |
| lcd | lcd\_t* | lcd对象。 |
#### lcd\_is\_swappable 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_is_swappable">判读lcd是否支持swap。

* 函数原型：

```
bool_t lcd_is_swappable (lcd_t* lcd);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回是否支持swap。 |
| lcd | lcd\_t* | lcd对象。 |
#### lcd\_measure\_text 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_measure_text">测量字符串占用的宽度。

* 函数原型：

```
float_t lcd_measure_text (lcd_t* lcd, const wchar_t* str, uint32_t nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float\_t | 返回字符串占用的宽度。 |
| lcd | lcd\_t* | lcd对象。 |
| str | const wchar\_t* | 字符串。 |
| nr | uint32\_t | 字符数。 |
#### lcd\_resize 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_resize">基于SDL的PC软件，在SDL窗口resize时，需要调用本函数resize lcd。
屏幕旋转时会调用本函数，调整LCD的大小。

* 函数原型：

```
ret_t lcd_resize (lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| w | wh\_t | 新的宽度。 |
| h | wh\_t | 新的高度。 |
| line\_length | uint32\_t | line\_length。 |
#### lcd\_set\_clip\_rect 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_set_clip_rect">设置裁剪区域。

* 函数原型：

```
ret_t lcd_set_clip_rect (lcd_t* lcd, rect_t* rect);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| rect | rect\_t* | 裁剪区域。 |
#### lcd\_set\_fill\_color 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_set_fill_color">设置填充颜色。

* 函数原型：

```
ret_t lcd_set_fill_color (lcd_t* lcd, color_t color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| color | color\_t | 颜色。 |
#### lcd\_set\_font\_name 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_set_font_name">设置字体名称。

* 函数原型：

```
ret_t lcd_set_font_name (lcd_t* lcd, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| name | const char* | 字体名称。 |
#### lcd\_set\_font\_size 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_set_font_size">设置字体大小。

* 函数原型：

```
ret_t lcd_set_font_size (lcd_t* lcd, uint32_t font_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| font\_size | uint32\_t | 字体大小。 |
#### lcd\_set\_global\_alpha 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_set_global_alpha">设置全局alpha。

* 函数原型：

```
ret_t lcd_set_global_alpha (lcd_t* lcd, uint8_t alpha);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| alpha | uint8\_t | 全局alpha。 |
#### lcd\_set\_stroke\_color 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_set_stroke_color">设置线条颜色。

* 函数原型：

```
ret_t lcd_set_stroke_color (lcd_t* lcd, color_t color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| color | color\_t | 颜色。 |
#### lcd\_set\_text\_color 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_set_text_color">设置文本颜色。

* 函数原型：

```
ret_t lcd_set_text_color (lcd_t* lcd, color_t color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| color | color\_t | 颜色。 |
#### lcd\_stroke\_rect 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_stroke_rect">绘制矩形。

* 函数原型：

```
ret_t lcd_stroke_rect (lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| w | wh\_t | 宽度。 |
| h | wh\_t | 高度。 |
#### lcd\_take\_snapshot 函数
-----------------------

* 函数功能：

> <p id="lcd_t_lcd_take_snapshot">拍摄快照，一般用于窗口动画，只有framebuffer模式，才支持。

* 函数原型：

```
ret_t lcd_take_snapshot (lcd_t* lcd, bitmap_t* img, bool_t auto_rotate);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lcd | lcd\_t* | lcd对象。 |
| img | bitmap\_t* | 返回快照图片。 |
| auto\_rotate | bool\_t | 是否根据LCD实际方向自动旋转。 |
#### draw\_mode 属性
-----------------------
> <p id="lcd_t_draw_mode">绘制模式。

* 类型：lcd\_draw\_mode\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### fill\_color 属性
-----------------------
> <p id="lcd_t_fill_color">填充颜色

* 类型：color\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### font\_name 属性
-----------------------
> <p id="lcd_t_font_name">字体名称。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### font\_size 属性
-----------------------
> <p id="lcd_t_font_size">字体大小。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### global\_alpha 属性
-----------------------
> <p id="lcd_t_global_alpha">全局alpha

* 类型：uint8\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### height 属性
-----------------------
> <p id="lcd_t_height">屏幕的高度

* 类型：wh\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### last\_update\_time 属性
-----------------------
> <p id="lcd_t_last_update_time">last update time

* 类型：uint64\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### ratio 属性
-----------------------
> <p id="lcd_t_ratio">屏幕密度。

* 类型：float\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### stroke\_color 属性
-----------------------
> <p id="lcd_t_stroke_color">线条颜色

* 类型：color\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### support\_dirty\_rect 属性
-----------------------
> <p id="lcd_t_support_dirty_rect">是否支持脏矩形。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### text\_color 属性
-----------------------
> <p id="lcd_t_text_color">文本颜色

* 类型：color\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### type 属性
-----------------------
> <p id="lcd_t_type">LCD的类型。

* 类型：lcd\_type\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### w 属性
-----------------------
> <p id="lcd_t_w">屏幕的宽度

* 类型：wh\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
