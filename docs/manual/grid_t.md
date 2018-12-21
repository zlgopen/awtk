## grid\_t
### 概述
![image](images/grid_t_0.png)

 grid。一个简单的容器控件，用于网格排列一组控件。
 它本身不提供布局功能(可用layout参数指定)，和view功能一致，仅提供具有语义的标签，让xml更具有可读性。
### 函数
<p id="grid_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#grid_t_grid_cast">grid\_cast</a> | 转换为grid对象(供脚本语言使用)。 |
| <a href="#grid_t_grid_create">grid\_create</a> | 创建grid对象 |
### 属性
<p id="grid_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="grid_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### grid\_cast 函数
* 函数原型：

```
widget_t* grid_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | grid对象。 |
| widget | widget\_t* | grid对象。 |
* 函数功能：

> <p id="grid_t_grid_cast"> 转换为grid对象(供脚本语言使用)。



#### grid\_create 函数
* 函数原型：

```
widget_t* grid_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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

> <p id="grid_t_grid_create"> 创建grid对象



