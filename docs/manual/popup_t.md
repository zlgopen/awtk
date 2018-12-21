## popup\_t
### 概述
![image](images/popup_t_0.png)

 popup窗口。
### 函数
<p id="popup_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#popup_t_popup_cast">popup\_cast</a> | 转换为popup对象(供脚本语言使用)。 |
| <a href="#popup_t_popup_create">popup\_create</a> | 创建popup对象。 |
### 属性
<p id="popup_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#popup_t_close_when_click">close\_when\_click</a> | bool_t | 点击时是否关闭窗口。 |
| <a href="#popup_t_close_when_click_outside">close\_when\_click\_outside</a> | bool_t | 点击到窗口外时是否关闭窗口。 |
### 事件
<p id="popup_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### popup\_cast 函数
* 函数原型：

```
widget_t* popup_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | popup对象。 |
| widget | widget\_t* | popup对象。 |
* 函数功能：

> <p id="popup_t_popup_cast"> 转换为popup对象(供脚本语言使用)。



#### popup\_create 函数
* 函数原型：

```
widget_t* popup_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | popup对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
* 函数功能：

> <p id="popup_t_popup_create"> 创建popup对象。



#### close\_when\_click 属性
-----------------------
> <p id="popup_t_close_when_click"> 点击时是否关闭窗口。


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
#### close\_when\_click\_outside 属性
-----------------------
> <p id="popup_t_close_when_click_outside"> 点击到窗口外时是否关闭窗口。


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
