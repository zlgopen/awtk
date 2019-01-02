## slide\_view\_t
### 概述
![image](images/slide_view_t_0.png)

 slide view。
### 函数
<p id="slide_view_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#slide_view_t_slide_view_cast">slide\_view\_cast</a> | 转换为slide_view对象(供脚本语言使用)。 |
| <a href="#slide_view_t_slide_view_create">slide\_view\_create</a> | 创建slide_view对象 |
| <a href="#slide_view_t_slide_view_set_active">slide\_view\_set\_active</a> | 设置当前页的序号。 |
| <a href="#slide_view_t_slide_view_set_auto_play">slide\_view\_set\_auto\_play</a> | 设置为自动播放模式。 |
| <a href="#slide_view_t_slide_view_set_vertical">slide\_view\_set\_vertical</a> | 设置为上下滑动(缺省为左右滑动)。 |
### 属性
<p id="slide_view_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#slide_view_t_auto_play">auto\_play</a> | uint16\_t | 自动播放。0表示禁止自动播放，非0表示自动播放时每一页播放的时间。 |
| <a href="#slide_view_t_vertical">vertical</a> | bool\_t | 是否为上下滑动模式。 |
### 事件
<p id="slide_view_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
| EVT\_VALUE\_WILL\_CHANGE | event\_t | 值(当前页)即将改变事件。 |
| EVT\_VALUE\_CHANGED | event\_t | 值(当前页)改变事件。 |
#### slide\_view\_cast 函数
-----------------------

* 函数功能：

> <p id="slide_view_t_slide_view_cast"> 转换为slide_view对象(供脚本语言使用)。



* 函数原型：

```
widget_t* slide_view_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | slide\_view对象。 |
| widget | widget\_t* | slide\_view对象。 |
#### slide\_view\_create 函数
-----------------------

* 函数功能：

> <p id="slide_view_t_slide_view_create"> 创建slide_view对象



* 函数原型：

```
widget_t* slide_view_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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
#### slide\_view\_set\_active 函数
-----------------------

* 函数功能：

> <p id="slide_view_t_slide_view_set_active"> 设置当前页的序号。



* 函数原型：

```
ret_t slide_view_set_active (widget_t* widget, uint32_t index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | slide\_view对象。 |
| index | uint32\_t | 当前页的序号。 |
#### slide\_view\_set\_auto\_play 函数
-----------------------

* 函数功能：

> <p id="slide_view_t_slide_view_set_auto_play"> 设置为自动播放模式。



* 函数原型：

```
ret_t slide_view_set_auto_play (widget_t* widget, uint16_t auto_play);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | slide\_view对象。 |
| auto\_play | uint16\_t | 0表示禁止自动播放，非0表示自动播放时每一页播放的时间。 |
#### slide\_view\_set\_vertical 函数
-----------------------

* 函数功能：

> <p id="slide_view_t_slide_view_set_vertical"> 设置为上下滑动(缺省为左右滑动)。



* 函数原型：

```
ret_t slide_view_set_vertical (widget_t* widget, bool_t vertical);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | slide\_view对象。 |
| vertical | bool\_t | TRUE表示上下滑动，FALSE表示左右滑动。 |
#### auto\_play 属性
-----------------------
> <p id="slide_view_t_auto_play"> 自动播放。0表示禁止自动播放，非0表示自动播放时每一页播放的时间。


* 类型：uint16\_t

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
#### vertical 属性
-----------------------
> <p id="slide_view_t_vertical"> 是否为上下滑动模式。


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
