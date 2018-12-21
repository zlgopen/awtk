## group\_box\_t
### 概述
![image](images/group_box_t_0.png)

 分组控件。
### 函数
<p id="group_box_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#group_box_t_group_box_cast">group\_box\_cast</a> |  转换为group_box对象(供脚本语言使用)。 |
| <a href="#group_box_t_group_box_create">group\_box\_create</a> |  创建group_box对象 |
### 属性
<p id="group_box_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="group_box_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### group\_box\_cast 函数
* 函数原型：

```
widget_t* group_box_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | group\_box对象。 |
| widget | widget\_t* | group\_box对象。 |
* 函数功能：

> <p id="group_box_t_group_box_cast"> 转换为group_box对象(供脚本语言使用)。



#### group\_box\_create 函数
* 函数原型：

```
widget_t* group_box_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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

> <p id="group_box_t_group_box_create"> 创建group_box对象



