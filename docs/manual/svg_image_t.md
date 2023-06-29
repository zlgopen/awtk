## svg\_image\_t
### 概述
![image](images/svg_image_t_0.png)

SVG图片控件。

svg\_image\_t是[image\_base\_t](image_base_t.md)的子类控件，image\_base\_t的函数均适用于svg\_image\_t控件。

在xml中使用"svg"标签创建SVG图片控件。如：

```xml
<svg image="girl"/>
```

>更多用法请参考：[svg image](
https://github.com/zlgopen/awtk/blob/master/design/default/ui/svg_image.xml)

在c代码中使用函数svg\_image\_create创建SVG图片控件。如：

```c
widget_t* image = svg_image_create(win, 10, 10, 200, 200);
image_set_image(image, "girl");
```

> 创建之后: 需要用widget\_set\_image设置图片名称。

> 完整示例请参考：[svg image demo](
https://github.com/zlgopen/awtk-c-demos/blob/master/demos/svg_image.c)

可用通过style来设置控件的显示风格，如背景和边框等。如：

```xml
<svg>
<style name="default">
<normal border_color="green" fg_color="red" />
</style>
</svg>
```

> 更多用法请参考：[theme default](
https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml)
----------------------------------
### 函数
<p id="svg_image_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#svg_image_t_svg_image_cast">svg\_image\_cast</a> | 转换为svg_image对象(供脚本语言使用)。 |
| <a href="#svg_image_t_svg_image_create">svg\_image\_create</a> | 创建svg_image对象 |
| <a href="#svg_image_t_svg_image_get_widget_vtable">svg\_image\_get\_widget\_vtable</a> | 获取 svg_image 虚表。 |
| <a href="#svg_image_t_svg_image_set_cache_mode">svg\_image\_set\_cache\_mode</a> | 控件设置是否开启离线缓存渲染模式。 |
| <a href="#svg_image_t_svg_image_set_draw_type">svg\_image\_set\_draw\_type</a> | 控件设置svg图片绘制模式。 |
| <a href="#svg_image_t_svg_image_set_image">svg\_image\_set\_image</a> | 设置控件的图片名称。 |
### 属性
<p id="svg_image_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#svg_image_t_draw_type">draw\_type</a> | image\_draw\_type\_t | svg图片的绘制方式(支持旋转缩放, 目前仅支持scale、scale_auto模式)。 |
| <a href="#svg_image_t_is_cache_mode">is\_cache\_mode</a> | bool\_t | 离线缓存渲染模式。 |
#### svg\_image\_cast 函数
-----------------------

* 函数功能：

> <p id="svg_image_t_svg_image_cast">转换为svg_image对象(供脚本语言使用)。

* 函数原型：

```
widget_t* svg_image_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | svg\_image对象。 |
| widget | widget\_t* | svg\_image对象。 |
#### svg\_image\_create 函数
-----------------------

* 函数功能：

> <p id="svg_image_t_svg_image_create">创建svg_image对象

* 函数原型：

```
widget_t* svg_image_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
#### svg\_image\_get\_widget\_vtable 函数
-----------------------

* 函数功能：

> <p id="svg_image_t_svg_image_get_widget_vtable">获取 svg_image 虚表。

* 函数原型：

```
const widget_vtable_t* svg_image_get_widget_vtable ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const widget\_vtable\_t* | 成功返回 svg\_image 虚表。 |
#### svg\_image\_set\_cache\_mode 函数
-----------------------

* 函数功能：

> <p id="svg_image_t_svg_image_set_cache_mode">控件设置是否开启离线缓存渲染模式。

> 在确保svg图片不经常变化大小及状态的情况下，开启离线缓存渲染能够减少解析bsvg的开销，提高效率。

* 函数原型：

```
ret_t svg_image_set_cache_mode (widget_t* widget, bool_t is_cache_mode);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image对象。 |
| is\_cache\_mode | bool\_t | 是否开启缓存模式。 |
#### svg\_image\_set\_draw\_type 函数
-----------------------

* 函数功能：

> <p id="svg_image_t_svg_image_set_draw_type">控件设置svg图片绘制模式。

* 函数原型：

```
ret_t svg_image_set_draw_type (widget_t* widget, image_draw_type_t draw_type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image对象。 |
| draw\_type | image\_draw\_type\_t | 绘制模式。 |
#### svg\_image\_set\_image 函数
-----------------------

* 函数功能：

> <p id="svg_image_t_svg_image_set_image">设置控件的图片名称。

> 如果需要显示文件系统中的图片，只需将图片名称换成实际的文件名，并加上"file://"前缀即可。

* 函数原型：

```
ret_t svg_image_set_image (widget_t* widget, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | image对象。 |
| name | const char* | 图片名称，该图片必须存在于资源管理器。 |
#### draw\_type 属性
-----------------------
> <p id="svg_image_t_draw_type">svg图片的绘制方式(支持旋转缩放, 目前仅支持scale、scale_auto模式)。

* 类型：image\_draw\_type\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
#### is\_cache\_mode 属性
-----------------------
> <p id="svg_image_t_is_cache_mode">离线缓存渲染模式。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
