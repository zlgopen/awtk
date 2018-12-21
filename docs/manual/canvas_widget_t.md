## canvas\_widget\_t
### 概述
![image](images/canvas_widget_t_0.png)

 画布控件。
### 函数
<p id="canvas_widget_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#canvas_widget_t_canvas_widget_cast">canvas\_widget\_cast</a> |  转换为canvas_widget对象(供脚本语言使用)。 |
| <a href="#canvas_widget_t_canvas_widget_create">canvas\_widget\_create</a> |  创建canvas_widget对象 |
### 属性
<p id="canvas_widget_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="canvas_widget_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### canvas\_widget\_cast 函数
* 函数原型：

```
widget_t* canvas_widget_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | canvas\_widget对象。 |
| widget | widget\_t* | canvas\_widget对象。 |
* 函数功能：

> <p id="canvas_widget_t_canvas_widget_cast"> 转换为canvas_widget对象(供脚本语言使用)。



#### canvas\_widget\_create 函数
* 函数原型：

```
widget_t* canvas_widget_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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

> <p id="canvas_widget_t_canvas_widget_create"> 创建canvas_widget对象



