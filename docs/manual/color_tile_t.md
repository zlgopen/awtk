## color\_tile\_t
### 概述
![image](images/color_tile_t_0.png)

 色块控件。
### 函数
<p id="color_tile_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#color_tile_t_color_tile_cast">color\_tile\_cast</a> | 转换为color_tile对象(供脚本语言使用)。 |
| <a href="#color_tile_t_color_tile_create">color\_tile\_create</a> | 创建color_tile对象 |
| <a href="#color_tile_t_color_tile_set_bg_color">color\_tile\_set\_bg\_color</a> | 设置背景颜色。 |
| <a href="#color_tile_t_color_tile_set_border_color">color\_tile\_set\_border\_color</a> | 设置边框颜色。 |
| <a href="#color_tile_t_color_tile_set_value">color\_tile\_set\_value</a> | 设置背景颜色。 |
### 属性
<p id="color_tile_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#color_tile_t_bg_color">bg\_color</a> | const char* | 背景颜色。 |
| <a href="#color_tile_t_border_color">border\_color</a> | const char* | 边框颜色。 |
### 事件
<p id="color_tile_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### color\_tile\_cast 函数
* 函数原型：

```
widget_t* color_tile_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | color\_tile对象。 |
| widget | widget\_t* | color\_tile对象。 |
* 函数功能：

> <p id="color_tile_t_color_tile_cast"> 转换为color_tile对象(供脚本语言使用)。



#### color\_tile\_create 函数
* 函数原型：

```
widget_t* color_tile_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
* 函数功能：

> <p id="color_tile_t_color_tile_create"> 创建color_tile对象



#### color\_tile\_set\_bg\_color 函数
* 函数原型：

```
ret_t color_tile_set_bg_color (widget_t* widget, const char* color);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| color | const char* | 背景颜色。 |
* 函数功能：

> <p id="color_tile_t_color_tile_set_bg_color"> 设置背景颜色。



#### color\_tile\_set\_border\_color 函数
* 函数原型：

```
ret_t color_tile_set_border_color (widget_t* widget, const char* color);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| color | const char* | 边框颜色。 |
* 函数功能：

> <p id="color_tile_t_color_tile_set_border_color"> 设置边框颜色。



#### color\_tile\_set\_value 函数
* 函数原型：

```
ret_t color_tile_set_value (widget_t* widget, color_t color);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| color | color\_t | 背景颜色。 |
* 函数功能：

> <p id="color_tile_t_color_tile_set_value"> 设置背景颜色。



#### bg\_color 属性
-----------------------
> <p id="color_tile_t_bg_color"> 背景颜色。


* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
#### border\_color 属性
-----------------------
> <p id="color_tile_t_border_color"> 边框颜色。


* 类型：const char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 支通过widget_get_prop读取 | 是 |
| 支通过widget_set_prop修改 | 是 |
