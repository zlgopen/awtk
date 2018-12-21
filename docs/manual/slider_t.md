## slider\_t
### 概述
![image](images/slider_t_0.png)

 滑块控件。
### 函数
<p id="slider_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#slider_t_slider_cast">slider\_cast</a> |  转换为slider对象(供脚本语言使用)。 |
| <a href="#slider_t_slider_create">slider\_create</a> |  创建slider对象 |
| <a href="#slider_t_slider_set_max">slider\_set\_max</a> |  设置滑块的最大值。 |
| <a href="#slider_t_slider_set_min">slider\_set\_min</a> |  设置滑块的最小值。 |
| <a href="#slider_t_slider_set_step">slider\_set\_step</a> |  设置滑块的拖动的最小单位。 |
| <a href="#slider_t_slider_set_value">slider\_set\_value</a> |  设置滑块的值。 |
| <a href="#slider_t_slider_set_vertical">slider\_set\_vertical</a> |  设置滑块的方向。 |
### 属性
<p id="slider_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#slider_t_max">max</a> | uint16_t |  最大值。 |
| <a href="#slider_t_min">min</a> | uint16_t |  最小值。 |
| <a href="#slider_t_step">step</a> | uint16_t |  拖动的最小单位。 |
| <a href="#slider_t_value">value</a> | uint16_t |  值。 |
| <a href="#slider_t_vertical">vertical</a> | bool_t |  滑块的是否为垂直方向。 |
### 事件
<p id="slider_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
| EVT\_VALUE\_WILL\_CHANGE | event\_t | 值即将改变事件。 |
| EVT\_VALUE\_CHANGING | event\_t | 值正在改变事件(拖动中)。 |
| EVT\_VALUE\_CHANGED | event\_t | 值改变事件。 |
#### slider\_cast 函数
* 函数原型：

```
widget_t* slider_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | slider对象。 |
| widget | widget\_t* | slider对象。 |
* 函数功能：

> <p id="slider_t_slider_cast"> 转换为slider对象(供脚本语言使用)。



#### slider\_create 函数
* 函数原型：

```
widget_t* slider_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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

> <p id="slider_t_slider_create"> 创建slider对象



#### slider\_set\_max 函数
* 函数原型：

```
ret_t slider_set_max (widget_t* widget, uint16_t max);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| max | uint16\_t | 最大值 |
* 函数功能：

> <p id="slider_t_slider_set_max"> 设置滑块的最大值。



#### slider\_set\_min 函数
* 函数原型：

```
ret_t slider_set_min (widget_t* widget, uint16_t min);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| min | uint16\_t | 最小值 |
* 函数功能：

> <p id="slider_t_slider_set_min"> 设置滑块的最小值。



#### slider\_set\_step 函数
* 函数原型：

```
ret_t slider_set_step (widget_t* widget, uint16_t step);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| step | uint16\_t | 拖动的最小单位。 |
* 函数功能：

> <p id="slider_t_slider_set_step"> 设置滑块的拖动的最小单位。



#### slider\_set\_value 函数
* 函数原型：

```
ret_t slider_set_value (widget_t* widget, uint16_t value);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| value | uint16\_t | 值 |
* 函数功能：

> <p id="slider_t_slider_set_value"> 设置滑块的值。



#### slider\_set\_vertical 函数
* 函数原型：

```
ret_t slider_set_vertical (widget_t* widget, bool_t vertical);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | 控件对象。 |
| vertical | bool\_t | 是否为垂直方向。 |
* 函数功能：

> <p id="slider_t_slider_set_vertical"> 设置滑块的方向。



#### max 属性
-----------------------
> <p id="slider_t_max"> 最大值。


* 类型：uint16\_t

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
#### min 属性
-----------------------
> <p id="slider_t_min"> 最小值。


* 类型：uint16\_t

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
#### step 属性
-----------------------
> <p id="slider_t_step"> 拖动的最小单位。


* 类型：uint16\_t

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
#### value 属性
-----------------------
> <p id="slider_t_value"> 值。


* 类型：uint16\_t

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
#### vertical 属性
-----------------------
> <p id="slider_t_vertical"> 滑块的是否为垂直方向。


* 类型：bool\_t

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
