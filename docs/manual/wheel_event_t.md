## wheel\_event\_t
### 概述
![image](images/wheel_event_t_0.png)

滚轮事件。
----------------------------------
### 函数
<p id="wheel_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#wheel_event_t_wheel_event_cast">wheel\_event\_cast</a> | 把event对象转wheel_event_t对象，主要给脚本语言使用。 |
| <a href="#wheel_event_t_wheel_event_init">wheel\_event\_init</a> | 初始化事件。 |
### 属性
<p id="wheel_event_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#wheel_event_t_alt">alt</a> | bool\_t | alt键是否按下。 |
| <a href="#wheel_event_t_ctrl">ctrl</a> | bool\_t | ctrl键是否按下。 |
| <a href="#wheel_event_t_dy">dy</a> | int32\_t | 滚轮的y值。 |
| <a href="#wheel_event_t_shift">shift</a> | bool\_t | shift键是否按下。 |
#### wheel\_event\_cast 函数
-----------------------

* 函数功能：

> <p id="wheel_event_t_wheel_event_cast">把event对象转wheel_event_t对象，主要给脚本语言使用。

* 函数原型：

```
wheel_event_t* wheel_event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wheel\_event\_t* | event对象。 |
| event | event\_t* | event对象。 |
#### wheel\_event\_init 函数
-----------------------

* 函数功能：

> <p id="wheel_event_t_wheel_event_init">初始化事件。

* 函数原型：

```
event_t* wheel_event_init (wheel_event_t* event, void* target, uint32_t type, int32_t dy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | event对象。 |
| event | wheel\_event\_t* | event对象。 |
| target | void* | 事件目标。 |
| type | uint32\_t | 事件类型。 |
| dy | int32\_t | 滚轮的y值。 |
#### alt 属性
-----------------------
> <p id="wheel_event_t_alt">alt键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### ctrl 属性
-----------------------
> <p id="wheel_event_t_ctrl">ctrl键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### dy 属性
-----------------------
> <p id="wheel_event_t_dy">滚轮的y值。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### shift 属性
-----------------------
> <p id="wheel_event_t_shift">shift键是否按下。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
