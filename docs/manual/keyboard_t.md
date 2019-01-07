## keyboard\_t
### 概述
![image](images/keyboard_t_0.png)

 键盘。

### 函数
<p id="keyboard_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#keyboard_t_keyboard_close">keyboard\_close</a> | 关闭keyboard窗口。 |
| <a href="#keyboard_t_keyboard_create">keyboard\_create</a> | 创建keyboard对象 |
#### keyboard\_close 函数
-----------------------

* 函数功能：

> <p id="keyboard_t_keyboard_close"> 关闭keyboard窗口。



* 函数原型：

```
ret_t keyboard_close (widget_t* parent);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| parent | widget\_t* | keyboard对象。 |
#### keyboard\_create 函数
-----------------------

* 函数功能：

> <p id="keyboard_t_keyboard_create"> 创建keyboard对象



* 函数原型：

```
widget_t* keyboard_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | 对象。 |
| parent | widget\_t* | 父控件 |
| x | xy\_t | x坐标 |
| y | xy\_t | y坐标 |
| w | wh\_t | 宽度 |
| h | wh\_t | 高度 |
