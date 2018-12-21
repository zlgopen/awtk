## calibration\_win\_t
### 概述
![image](images/calibration_win_t_0.png)

 电阻屏校准屏幕。
### 函数
<p id="calibration_win_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#calibration_win_t_calibration_win_create">calibration\_win\_create</a> | 创建calibration_win对象 |
| <a href="#calibration_win_t_calibration_win_set_on_click">calibration\_win\_set\_on\_click</a> | 设置校准点击事件的处理函数。 |
| <a href="#calibration_win_t_calibration_win_set_on_done">calibration\_win\_set\_on\_done</a> | 设置校准完成的处理函数。 |
### 属性
<p id="calibration_win_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="calibration_win_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### calibration\_win\_create 函数
* 函数原型：

```
widget_t* calibration_win_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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

> <p id="calibration_win_t_calibration_win_create"> 创建calibration_win对象



#### calibration\_win\_set\_on\_click 函数
* 函数原型：

```
ret_t calibration_win_set_on_click (widget_t* widget, calibration_win_on_click_t on_click, void* ctx);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| on\_click | calibration\_win\_on\_click\_t | 回调函数。 |
| ctx | void* | 回调函数的上下文。 |
* 函数功能：

> <p id="calibration_win_t_calibration_win_set_on_click"> 设置校准点击事件的处理函数。



#### calibration\_win\_set\_on\_done 函数
* 函数原型：

```
ret_t calibration_win_set_on_done (widget_t* widget, calibration_win_on_done_t on_done, void* ctx);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | widget对象。 |
| on\_done | calibration\_win\_on\_done\_t | 回调函数。 |
| ctx | void* | 回调函数的上下文。 |
* 函数功能：

> <p id="calibration_win_t_calibration_win_set_on_done"> 设置校准完成的处理函数。



