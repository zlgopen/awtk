## canvas\_t
### 概述
提供基本的绘图功能和状态管理。
----------------------------------
### 函数
<p id="canvas_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#canvas_t_canvas_begin_frame">canvas\_begin\_frame</a> | 绘制开始。 |
| <a href="#canvas_t_canvas_cast">canvas\_cast</a> | 转换为canvas对象(供脚本语言使用)。 |
| <a href="#canvas_t_canvas_draw_hline">canvas\_draw\_hline</a> | 画水平线。 |
| <a href="#canvas_t_canvas_draw_icon">canvas\_draw\_icon</a> | 绘制图标。 |
| <a href="#canvas_t_canvas_draw_image">canvas\_draw\_image</a> | 绘制图片。 |
| <a href="#canvas_t_canvas_draw_image_at">canvas\_draw\_image\_at</a> | 在指定位置画图。 |
| <a href="#canvas_t_canvas_draw_image_ex">canvas\_draw\_image\_ex</a> | 绘制图片。 |
| <a href="#canvas_t_canvas_draw_line">canvas\_draw\_line</a> | 画直线。 |
| <a href="#canvas_t_canvas_draw_points">canvas\_draw\_points</a> | 画多个点。 |
| <a href="#canvas_t_canvas_draw_text">canvas\_draw\_text</a> | 绘制文本。 |
| <a href="#canvas_t_canvas_draw_text_bidi_in_rect">canvas\_draw\_text\_bidi\_in\_rect</a> | 绘制文本(支持Unicode Bidirectional Algorithm)。 |
| <a href="#canvas_t_canvas_draw_text_in_rect">canvas\_draw\_text\_in\_rect</a> | 绘制文本。 |
| <a href="#canvas_t_canvas_draw_utf8">canvas\_draw\_utf8</a> | 绘制文本。 |
| <a href="#canvas_t_canvas_draw_utf8_in_rect">canvas\_draw\_utf8\_in\_rect</a> | 绘制文本。 |
| <a href="#canvas_t_canvas_draw_vline">canvas\_draw\_vline</a> | 画垂直线。 |
| <a href="#canvas_t_canvas_end_frame">canvas\_end\_frame</a> | 绘制结束。 |
| <a href="#canvas_t_canvas_fill_rect">canvas\_fill\_rect</a> | 填充矩形。 |
| <a href="#canvas_t_canvas_fill_rounded_rect">canvas\_fill\_rounded\_rect</a> | 填充区域。 |
| <a href="#canvas_t_canvas_get_clip_rect">canvas\_get\_clip\_rect</a> | 获取裁剪区。 |
| <a href="#canvas_t_canvas_get_font_height">canvas\_get\_font\_height</a> | 获取字体的高度。 |
| <a href="#canvas_t_canvas_get_height">canvas\_get\_height</a> | 获取画布的高度。 |
| <a href="#canvas_t_canvas_get_text_metrics">canvas\_get\_text\_metrics</a> | 获取当前字体的度量信息。 |
| <a href="#canvas_t_canvas_get_vgcanvas">canvas\_get\_vgcanvas</a> | 获取vgcanvas对象。 |
| <a href="#canvas_t_canvas_get_width">canvas\_get\_width</a> | 获取画布的宽度。 |
| <a href="#canvas_t_canvas_init">canvas\_init</a> | 初始化，系统内部调用。 |
| <a href="#canvas_t_canvas_measure_text">canvas\_measure\_text</a> | 计算文本所占的宽度。 |
| <a href="#canvas_t_canvas_measure_utf8">canvas\_measure\_utf8</a> | 计算文本所占的宽度。 |
| <a href="#canvas_t_canvas_reset">canvas\_reset</a> | 释放相关资源。 |
| <a href="#canvas_t_canvas_set_assets_manager">canvas\_set\_assets\_manager</a> | 设置canvas的assets_manager对象。 |
| <a href="#canvas_t_canvas_set_clip_rect">canvas\_set\_clip\_rect</a> | 设置裁剪区。 |
| <a href="#canvas_t_canvas_set_clip_rect_ex">canvas\_set\_clip\_rect\_ex</a> | 设置裁剪区。 |
| <a href="#canvas_t_canvas_set_fill_color">canvas\_set\_fill\_color</a> | 设置填充颜色。 |
| <a href="#canvas_t_canvas_set_fill_color_str">canvas\_set\_fill\_color\_str</a> | 设置填充颜色。 |
| <a href="#canvas_t_canvas_set_font">canvas\_set\_font</a> | 设置字体。 |
| <a href="#canvas_t_canvas_set_font_manager">canvas\_set\_font\_manager</a> | 设置canvas的font_manager对象。 |
| <a href="#canvas_t_canvas_set_global_alpha">canvas\_set\_global\_alpha</a> | 设置全局alpha值。 |
| <a href="#canvas_t_canvas_set_stroke_color">canvas\_set\_stroke\_color</a> | 设置线条颜色。 |
| <a href="#canvas_t_canvas_set_stroke_color_str">canvas\_set\_stroke\_color\_str</a> | 设置线条颜色。 |
| <a href="#canvas_t_canvas_set_text_align">canvas\_set\_text\_align</a> | 设置文本对齐方式。 |
| <a href="#canvas_t_canvas_set_text_color">canvas\_set\_text\_color</a> | 设置文本颜色。 |
| <a href="#canvas_t_canvas_set_text_color_str">canvas\_set\_text\_color\_str</a> | 设置文本颜色。 |
| <a href="#canvas_t_canvas_stroke_rect">canvas\_stroke\_rect</a> | 绘制矩形。 |
| <a href="#canvas_t_canvas_stroke_rounded_rect">canvas\_stroke\_rounded\_rect</a> | 绘制边框。 |
| <a href="#canvas_t_canvas_translate">canvas\_translate</a> | 平移原点坐标。 |
| <a href="#canvas_t_canvas_untranslate">canvas\_untranslate</a> | 反向平移原点坐标。 |
### 属性
<p id="canvas_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#canvas_t_assets_manager">assets\_manager</a> | assets\_manager\_t* | 资源管理器对象。 |
| <a href="#canvas_t_clip_bottom">clip\_bottom</a> | xy\_t | 当前裁剪矩形的底部位置。 |
| <a href="#canvas_t_clip_left">clip\_left</a> | xy\_t | 当前裁剪矩形的左边位置。 |
| <a href="#canvas_t_clip_right">clip\_right</a> | xy\_t | 当前裁剪矩形的右边位置。 |
| <a href="#canvas_t_clip_top">clip\_top</a> | xy\_t | 当前裁剪矩形的顶部位置。 |
| <a href="#canvas_t_font">font</a> | font\_t* | 字体对象。 |
| <a href="#canvas_t_font_manager">font\_manager</a> | font\_manager\_t* | 字体管理器对象。 |
| <a href="#canvas_t_font_name">font\_name</a> | char* | 当前字体名称。 |
| <a href="#canvas_t_font_size">font\_size</a> | uint16\_t | 当前字体大小。 |
| <a href="#canvas_t_fps">fps</a> | uint32\_t | 当前的帧率。 |
| <a href="#canvas_t_global_alpha">global\_alpha</a> | uint8\_t | 当前全局alpha。 |
| <a href="#canvas_t_lcd">lcd</a> | lcd\_t* | lcd对象。 |
| <a href="#canvas_t_ox">ox</a> | xy\_t | x坐标偏移。 |
| <a href="#canvas_t_oy">oy</a> | xy\_t | y坐标偏移。 |
| <a href="#canvas_t_show_fps">show\_fps</a> | bool\_t | 是否显示帧率。 |
| <a href="#canvas_t_text_align_h">text\_align\_h</a> | align\_h\_t | 文本水平对齐方式。 |
| <a href="#canvas_t_text_align_v">text\_align\_v</a> | align\_v\_t | 文本垂直对齐方式。 |
#### canvas\_begin\_frame 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_begin_frame">绘制开始。

* 函数原型：

```
ret_t canvas_begin_frame (canvas_t* c, rect_t* dirty_rect, lcd_draw_mode_t draw_mode);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| dirty\_rect | rect\_t* | 脏矩形。 |
| draw\_mode | lcd\_draw\_mode\_t | 绘制模式。 |
#### canvas\_cast 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_cast">转换为canvas对象(供脚本语言使用)。

* 函数原型：

```
canvas_t* canvas_cast (canvas_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | canvas\_t* | canvas对象。 |
| c | canvas\_t* | canvas对象。 |
#### canvas\_draw\_hline 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_hline">画水平线。

* 函数原型：

```
ret_t canvas_draw_hline (canvas_t* c, xy_t x, xy_t y, wh_t w);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| w | wh\_t | 宽度。 |
#### canvas\_draw\_icon 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_icon">绘制图标。

* 函数原型：

```
ret_t canvas_draw_icon (canvas_t* c, bitmap_t* img, xy_t cx, xy_t cy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| img | bitmap\_t* | 图片对象。 |
| cx | xy\_t | 中心点x坐标。 |
| cy | xy\_t | 中心点y坐标。 |
#### canvas\_draw\_image 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_image">绘制图片。

* 函数原型：

```
ret_t canvas_draw_image (canvas_t* c, bitmap_t* img, rect_t* src, rect_t* dst);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| img | bitmap\_t* | 图片对象。 |
| src | rect\_t* | 源区域。 |
| dst | rect\_t* | 目的区域。 |
#### canvas\_draw\_image\_at 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_image_at">在指定位置画图。

* 函数原型：

```
ret_t canvas_draw_image_at (canvas_t* c, bitmap_t* img, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| img | bitmap\_t* | 图片对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | w坐标。 |
#### canvas\_draw\_image\_ex 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_image_ex">绘制图片。

* 函数原型：

```
ret_t canvas_draw_image_ex (canvas_t* c, bitmap_t* img, image_draw_type_t draw_type, rect_t* dst);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| img | bitmap\_t* | 图片对象。 |
| draw\_type | image\_draw\_type\_t | 绘制类型。 |
| dst | rect\_t* | 目的区域。 |
#### canvas\_draw\_line 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_line">画直线。

* 函数原型：

```
ret_t canvas_draw_line (canvas_t* c, xy_t x1, xy_t y1, xy_t x2, xy_t y2);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| x1 | xy\_t | 起始点的x坐标。 |
| y1 | xy\_t | 起始点的y坐标。 |
| x2 | xy\_t | 结束点的x坐标。 |
| y2 | xy\_t | 结束点的y坐标。 |
#### canvas\_draw\_points 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_points">画多个点。

* 函数原型：

```
ret_t canvas_draw_points (canvas_t* c, const point_t* points, uint32_t nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| points | const point\_t* | 点数组。 |
| nr | uint32\_t | 点的个数。 |
#### canvas\_draw\_text 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_text">绘制文本。

* 函数原型：

```
ret_t canvas_draw_text (canvas_t* c, const wchar_t* str, uint32_t nr, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| str | const wchar\_t* | 字符串。 |
| nr | uint32\_t | 字符数。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
#### canvas\_draw\_text\_bidi\_in\_rect 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_text_bidi_in_rect">绘制文本(支持Unicode Bidirectional Algorithm)。

* 函数原型：

```
ret_t canvas_draw_text_bidi_in_rect (canvas_t* c, const wchar_t* str, uint32_t nr, const rect_t* r, const char* bidi_type, bool_t ellipses);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| str | const wchar\_t* | 字符串。 |
| nr | uint32\_t | 字符数。 |
| r | const rect\_t* | 矩形区域。 |
| bidi\_type | const char* | 类型。 |
| ellipses | bool\_t | 如果目标宽度不够，是否显示省略号。 |
#### canvas\_draw\_text\_in\_rect 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_text_in_rect">绘制文本。

* 函数原型：

```
ret_t canvas_draw_text_in_rect (canvas_t* c, const wchar_t* str, uint32_t nr, const rect_t* r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| str | const wchar\_t* | 字符串。 |
| nr | uint32\_t | 字符数。 |
| r | const rect\_t* | 矩形区域。 |
#### canvas\_draw\_utf8 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_utf8">绘制文本。

> 供脚本语言使用。

* 函数原型：

```
ret_t canvas_draw_utf8 (canvas_t* c, const char* str, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| str | const char* | 字符串。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
#### canvas\_draw\_utf8\_in\_rect 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_utf8_in_rect">绘制文本。

> 供脚本语言使用。

* 函数原型：

```
ret_t canvas_draw_utf8_in_rect (canvas_t* c, const char* str, const rect_t* r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| str | const char* | 字符串。 |
| r | const rect\_t* | 矩形区域。 |
#### canvas\_draw\_vline 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_draw_vline">画垂直线。

* 函数原型：

```
ret_t canvas_draw_vline (canvas_t* c, xy_t x, xy_t y, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| h | wh\_t | 高度。 |
#### canvas\_end\_frame 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_end_frame">绘制结束。

* 函数原型：

```
ret_t canvas_end_frame (canvas_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
#### canvas\_fill\_rect 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_fill_rect">填充矩形。

* 函数原型：

```
ret_t canvas_fill_rect (canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| w | wh\_t | 宽度。 |
| h | wh\_t | 高度。 |
#### canvas\_fill\_rounded\_rect 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_fill_rounded_rect">填充区域。

* 函数原型：

```
ret_t canvas_fill_rounded_rect (canvas_t* c, rect_t* r, color_t* color, uint32_t radius);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| r | rect\_t* | 矩形。 |
| color | color\_t* | 颜色。 |
| radius | uint32\_t | 圆角半径。 |
#### canvas\_get\_clip\_rect 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_get_clip_rect">获取裁剪区。

* 函数原型：

```
ret_t canvas_get_clip_rect (canvas_t* c, rect_t* r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| r | rect\_t* | rect对象。 |
#### canvas\_get\_font\_height 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_get_font_height">获取字体的高度。

* 函数原型：

```
float_t canvas_get_font_height (canvas_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float\_t | 返回字体的高度。 |
| c | canvas\_t* | canvas对象。 |
#### canvas\_get\_height 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_get_height">获取画布的高度。

* 函数原型：

```
wh_t canvas_get_height (canvas_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wh\_t | 返回画布的高度。 |
| c | canvas\_t* | canvas对象。 |
#### canvas\_get\_text\_metrics 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_get_text_metrics">获取当前字体的度量信息。

* 函数原型：

```
ret_t canvas_get_text_metrics (canvas_t* canvas, float_t* ascent, float_t* descent, float_t* line_hight);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| canvas | canvas\_t* | canvas对象。 |
| ascent | float\_t* | 用于返回ascent。 |
| descent | float\_t* | 用于返回descent。 |
| line\_hight | float\_t* | 用于返回line height。 |
#### canvas\_get\_vgcanvas 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_get_vgcanvas">获取vgcanvas对象。

* 函数原型：

```
vgcanvas_t* canvas_get_vgcanvas (canvas_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | vgcanvas\_t* | 返回vgcanvas对象。 |
| c | canvas\_t* | canvas对象。 |
#### canvas\_get\_width 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_get_width">获取画布的宽度。

* 函数原型：

```
wh_t canvas_get_width (canvas_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wh\_t | 返回画布的宽度。 |
| c | canvas\_t* | canvas对象。 |
#### canvas\_init 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_init">初始化，系统内部调用。

* 函数原型：

```
canvas_t* canvas_init (canvas_t* c, lcd_t* lcd, font_manager_t* font_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | canvas\_t* | 返回canvas对象本身。 |
| c | canvas\_t* | canvas对象。 |
| lcd | lcd\_t* | lcd对象。 |
| font\_manager | font\_manager\_t* | 字体管理器对象。 |
#### canvas\_measure\_text 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_measure_text">计算文本所占的宽度。

* 函数原型：

```
float_t canvas_measure_text (canvas_t* c, const wchar_t* str, uint32_t nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float\_t | 返回文本所占的宽度。 |
| c | canvas\_t* | canvas对象。 |
| str | const wchar\_t* | 字符串。 |
| nr | uint32\_t | 字符数。 |
#### canvas\_measure\_utf8 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_measure_utf8">计算文本所占的宽度。

> 供脚本语言使用。

* 函数原型：

```
float_t canvas_measure_utf8 (canvas_t* c, const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | float\_t | 返回文本所占的宽度。 |
| c | canvas\_t* | canvas对象。 |
| str | const char* | 字符串。 |
#### canvas\_reset 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_reset">释放相关资源。

* 函数原型：

```
ret_t canvas_reset (canvas_t* c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
#### canvas\_set\_assets\_manager 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_assets_manager">设置canvas的assets_manager对象。

* 函数原型：

```
ret_t canvas_set_assets_manager (canvas_t* c, assets_manager_t* assets_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| assets\_manager | assets\_manager\_t* | assets\_manager对象。 |
#### canvas\_set\_clip\_rect 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_clip_rect">设置裁剪区。

* 函数原型：

```
ret_t canvas_set_clip_rect (canvas_t* c, const rect_t* r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| r | const rect\_t* | rect对象。 |
#### canvas\_set\_clip\_rect\_ex 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_clip_rect_ex">设置裁剪区。

* 函数原型：

```
ret_t canvas_set_clip_rect_ex (canvas_t* c, const rect_t* r, bool_t translate);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| r | const rect\_t* | rect对象。 |
| translate | bool\_t | 是否将裁剪区的位置加上canvas当前的偏移。 |
#### canvas\_set\_fill\_color 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_fill_color">设置填充颜色。

* 函数原型：

```
ret_t canvas_set_fill_color (canvas_t* c, color_t color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| color | color\_t | 颜色。 |
#### canvas\_set\_fill\_color\_str 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_fill_color_str">设置填充颜色。

> 供脚本语言使用。

* 函数原型：

```
ret_t canvas_set_fill_color_str (canvas_t* c, const char* color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| color | const char* | 颜色。 |
#### canvas\_set\_font 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_font">设置字体。

* 函数原型：

```
ret_t canvas_set_font (canvas_t* c, const char* name, font_size_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| name | const char* | 字体名称。 |
| size | font\_size\_t | 字体大小。 |
#### canvas\_set\_font\_manager 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_font_manager">设置canvas的font_manager对象。

* 函数原型：

```
ret_t canvas_set_font_manager (canvas_t* c, font_manager_t* font_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| font\_manager | font\_manager\_t* | font\_manager对象。 |
#### canvas\_set\_global\_alpha 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_global_alpha">设置全局alpha值。

* 函数原型：

```
ret_t canvas_set_global_alpha (canvas_t* c, uint8_t alpha);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| alpha | uint8\_t | alpha值。 |
#### canvas\_set\_stroke\_color 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_stroke_color">设置线条颜色。

* 函数原型：

```
ret_t canvas_set_stroke_color (canvas_t* c, color_t color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| color | color\_t | 颜色。 |
#### canvas\_set\_stroke\_color\_str 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_stroke_color_str">设置线条颜色。

> 供脚本语言使用。

* 函数原型：

```
ret_t canvas_set_stroke_color_str (canvas_t* c, const char* color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| color | const char* | 颜色。 |
#### canvas\_set\_text\_align 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_text_align">设置文本对齐方式。

* 函数原型：

```
ret_t canvas_set_text_align (canvas_t* c, align_h_t align_h, align_v_t align_v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| align\_h | align\_h\_t | 水平对齐方式。 |
| align\_v | align\_v\_t | 垂直对齐方式。 |
#### canvas\_set\_text\_color 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_text_color">设置文本颜色。

* 函数原型：

```
ret_t canvas_set_text_color (canvas_t* c, color_t color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| color | color\_t | 颜色。 |
#### canvas\_set\_text\_color\_str 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_set_text_color_str">设置文本颜色。

> 供脚本语言使用。

* 函数原型：

```
ret_t canvas_set_text_color_str (canvas_t* c, const char* color);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| color | const char* | 颜色。 |
#### canvas\_stroke\_rect 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_stroke_rect">绘制矩形。

* 函数原型：

```
ret_t canvas_stroke_rect (canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
| w | wh\_t | 宽度。 |
| h | wh\_t | 高度。 |
#### canvas\_stroke\_rounded\_rect 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_stroke_rounded_rect">绘制边框。

* 函数原型：

```
ret_t canvas_stroke_rounded_rect (canvas_t* c, rect_t* r, color_t* color, uint32_t radius, uint32_t border_width);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| r | rect\_t* | 矩形。 |
| color | color\_t* | 颜色。 |
| radius | uint32\_t | 圆角半径。 |
| border\_width | uint32\_t | 边宽。 |
#### canvas\_translate 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_translate">平移原点坐标。

* 函数原型：

```
ret_t canvas_translate (canvas_t* c, xy_t dx, xy_t dy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| dx | xy\_t | x偏移。 |
| dy | xy\_t | y偏移。 |
#### canvas\_untranslate 函数
-----------------------

* 函数功能：

> <p id="canvas_t_canvas_untranslate">反向平移原点坐标。

* 函数原型：

```
ret_t canvas_untranslate (canvas_t* c, xy_t dx, xy_t dy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| c | canvas\_t* | canvas对象。 |
| dx | xy\_t | x偏移。 |
| dy | xy\_t | y偏移。 |
#### assets\_manager 属性
-----------------------
> <p id="canvas_t_assets_manager">资源管理器对象。

* 类型：assets\_manager\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### clip\_bottom 属性
-----------------------
> <p id="canvas_t_clip_bottom">当前裁剪矩形的底部位置。

* 类型：xy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### clip\_left 属性
-----------------------
> <p id="canvas_t_clip_left">当前裁剪矩形的左边位置。

* 类型：xy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### clip\_right 属性
-----------------------
> <p id="canvas_t_clip_right">当前裁剪矩形的右边位置。

* 类型：xy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### clip\_top 属性
-----------------------
> <p id="canvas_t_clip_top">当前裁剪矩形的顶部位置。

* 类型：xy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### font 属性
-----------------------
> <p id="canvas_t_font">字体对象。

* 类型：font\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### font\_manager 属性
-----------------------
> <p id="canvas_t_font_manager">字体管理器对象。

* 类型：font\_manager\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### font\_name 属性
-----------------------
> <p id="canvas_t_font_name">当前字体名称。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### font\_size 属性
-----------------------
> <p id="canvas_t_font_size">当前字体大小。

* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### fps 属性
-----------------------
> <p id="canvas_t_fps">当前的帧率。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### global\_alpha 属性
-----------------------
> <p id="canvas_t_global_alpha">当前全局alpha。

* 类型：uint8\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### lcd 属性
-----------------------
> <p id="canvas_t_lcd">lcd对象。

* 类型：lcd\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### ox 属性
-----------------------
> <p id="canvas_t_ox">x坐标偏移。

* 类型：xy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### oy 属性
-----------------------
> <p id="canvas_t_oy">y坐标偏移。

* 类型：xy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### show\_fps 属性
-----------------------
> <p id="canvas_t_show_fps">是否显示帧率。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### text\_align\_h 属性
-----------------------
> <p id="canvas_t_text_align_h">文本水平对齐方式。

* 类型：align\_h\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### text\_align\_v 属性
-----------------------
> <p id="canvas_t_text_align_v">文本垂直对齐方式。

* 类型：align\_v\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
