## tab\_control\_t
### 概述
![image](images/tab_control_t_0.png)

 标签控件。
### 函数
<p id="tab_control_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tab_control_t_tab_control_cast">tab\_control\_cast</a> |  转换tab_control对象(供脚本语言使用)。 |
| <a href="#tab_control_t_tab_control_create">tab\_control\_create</a> |  创建tab_control对象 |
### 属性
<p id="tab_control_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="tab_control_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### tab\_control\_cast 函数
* 函数原型：

```
widget_t* tab_control_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | tab\_control对象。 |
| widget | widget\_t* | tab\_control对象。 |
* 函数功能：

> <p id="tab_control_t_tab_control_cast"> 转换tab_control对象(供脚本语言使用)。



#### tab\_control\_create 函数
* 函数原型：

```
widget_t* tab_control_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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

> <p id="tab_control_t_tab_control_create"> 创建tab_control对象



