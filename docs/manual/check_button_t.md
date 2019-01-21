## check\_button\_t
### 概述
![image](images/check_button_t_0.png)

 勾选控件。

----------------------------------
### 函数
<p id="check_button_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#check_button_t_check_button_cast">check\_button\_cast</a> | 转换check_button对象(供脚本语言使用)。 |
| <a href="#check_button_t_check_button_create">check\_button\_create</a> | 创建check_button对象 |
| <a href="#check_button_t_check_button_create_radio">check\_button\_create\_radio</a> | 创建check_button对象 |
| <a href="#check_button_t_check_button_set_value">check\_button\_set\_value</a> | 设置控件的值。 |
### 属性
<p id="check_button_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#check_button_t_value">value</a> | bool\_t | 值。 |
### 事件
<p id="check_button_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
| EVT\_VALUE\_WILL\_CHANGE | event\_t | 值(勾选状态)即将改变事件。 |
| EVT\_VALUE\_CHANGED | event\_t | 值(勾选状态)改变事件。 |
#### check\_button\_cast 函数
-----------------------

* 函数功能：

> <p id="check_button_t_check_button_cast"> 转换check_button对象(供脚本语言使用)。



* 函数原型：

```
widget_t* check_button_cast (widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | widget\_t* | check\_button对象。 |
| widget | widget\_t* | check\_button对象。 |
#### check\_button\_create 函数
-----------------------

* 函数功能：

> <p id="check_button_t_check_button_create"> 创建check_button对象



* 函数原型：

```
widget_t* check_button_create (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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
#### check\_button\_create\_radio 函数
-----------------------

* 函数功能：

> <p id="check_button_t_check_button_create_radio"> 创建check_button对象



* 函数原型：

```
widget_t* check_button_create_radio (widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
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
#### check\_button\_set\_value 函数
-----------------------

* 函数功能：

> <p id="check_button_t_check_button_set_value"> 设置控件的值。



* 函数原型：

```
ret_t check_button_set_value (widget_t* widget, bool_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| widget | widget\_t* | check\_button对象。 |
| value | bool\_t | 值 |
#### value 属性
-----------------------
> <p id="check_button_t_value"> 值。


* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 是 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 是 |
| 可在XML中设置 | 是 |
| 可通过widget\_get\_prop读取 | 是 |
| 可通过widget\_set\_prop修改 | 是 |
