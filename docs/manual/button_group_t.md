## button\_group\_t
### 概述
![image](images/button_group_t_0.png)

 Button Group控件。
### 函数
<p id="button_group_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#button_group_t_button_group_cast">button\_group\_cast</a> | 转换为button_group对象(供脚本语言使用)。 |
| <a href="#button_group_t_button_group_create">button\_group\_create</a> | 创建button_group对象 |
### 属性
<p id="button_group_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="button_group_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### button\_group\_cast 函数
* 函数原型：

```
widget_t* button_group_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | button\_group对象。 |
| widget | widget\_t* | button\_group对象。 |
* 函数功能：

> <p id="button_group_t_button_group_cast"> 转换为button_group对象(供脚本语言使用)。



#### button\_group\_create 函数
* 函数原型：

```
widget_t* button_group_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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

> <p id="button_group_t_button_group_create"> 创建button_group对象



