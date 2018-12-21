## system\_bar\_t
### 概述
![image](images/system_bar_t_0.png)

 system_bar窗口。独占LCD顶部区域，用来显示系统状态和当前窗口的标题等信息。
 system_bar对两个子控件会做特殊处理：
 * 1.名为"title"的label控件，自动显示当前主窗口的name或text。
 * 2.名为"close"的button控件，点击之后向当前主窗口发送EVT_REQUEST_CLOSE_WINDOW消息。
### 函数
<p id="system_bar_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#system_bar_t_system_bar_cast">system\_bar\_cast</a> |  转换为system_bar对象(供脚本语言使用)。 |
| <a href="#system_bar_t_system_bar_create">system\_bar\_create</a> |  创建system_bar对象。 |
### 属性
<p id="system_bar_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="system_bar_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### system\_bar\_cast 函数
* 函数原型：

```
widget_t* system_bar_cast (widget_t* widget);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | system\_bar对象。 |
| widget | widget\_t* | system\_bar对象。 |
* 函数功能：

> <p id="system_bar_t_system_bar_cast"> 转换为system_bar对象(供脚本语言使用)。



#### system\_bar\_create 函数
* 函数原型：

```
widget_t* system_bar_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | system\_bar对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
* 函数功能：

> <p id="system_bar_t_system_bar_create"> 创建system_bar对象。



