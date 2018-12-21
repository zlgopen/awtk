## spin\_box\_t
### 概述
![image](images/spin_box_t_0.png)

 spinbox控件。
### 函数
<p id="spin_box_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#spin_box_t_spin_box_cast">spin\_box\_cast</a> |  转换为spin_box对象(供脚本语言使用)。 |
| <a href="#spin_box_t_spin_box_create">spin\_box\_create</a> |  创建spin_box对象 |
### 属性
<p id="spin_box_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="spin_box_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### spin\_box\_cast 函数
* 函数原型：

```
widget_t* spin_box_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | spin\_box对象。 |
| widget | widget\_t* | spin\_box对象。 |
* 函数功能：

> <p id="spin_box_t_spin_box_cast"> 转换为spin_box对象(供脚本语言使用)。



#### spin\_box\_create 函数
* 函数原型：

```
widget_t* spin_box_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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

> <p id="spin_box_t_spin_box_create"> 创建spin_box对象



