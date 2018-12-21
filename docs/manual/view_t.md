## view\_t
### 概述
![image](images/view_t_0.png)

 一个通用的容器控件，也可以用作自绘。
### 函数
<p id="view_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#view_t_view_cast">view\_cast</a> | 转换为view对象(供脚本语言使用)。 |
| <a href="#view_t_view_create">view\_create</a> | 创建view对象 |
### 属性
<p id="view_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="view_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### view\_cast 函数
* 函数原型：

```
widget_t* view_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | view对象。 |
| widget | widget\_t* | view对象。 |
* 函数功能：

> <p id="view_t_view_cast"> 转换为view对象(供脚本语言使用)。



#### view\_create 函数
* 函数原型：

```
widget_t* view_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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

> <p id="view_t_view_create"> 创建view对象



