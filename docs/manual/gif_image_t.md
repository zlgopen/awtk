## gif\_image\_t
### 概述
![image](images/gif_image_t_0.png)

 GIF图片控件。
### 函数
<p id="gif_image_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#gif_image_t_gif_image_cast">gif\_image\_cast</a> | 转换为gif_image对象(供脚本语言使用)。 |
| <a href="#gif_image_t_gif_image_create">gif\_image\_create</a> | 创建gif_image对象 |
### 属性
<p id="gif_image_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="gif_image_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### gif\_image\_cast 函数
* 函数原型：

```
widget_t* gif_image_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | gif\_image对象。 |
| widget | widget\_t* | gif\_image对象。 |
* 函数功能：

> <p id="gif_image_t_gif_image_cast"> 转换为gif_image对象(供脚本语言使用)。



#### gif\_image\_create 函数
* 函数原型：

```
widget_t* gif_image_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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

> <p id="gif_image_t_gif_image_create"> 创建gif_image对象



