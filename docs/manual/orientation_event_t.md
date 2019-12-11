## orientation\_event\_t
### 概述
![image](images/orientation_event_t_0.png)

滚轮事件。
----------------------------------
### 函数
<p id="orientation_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#orientation_event_t_orientation_event_cast">orientation\_event\_cast</a> | 把event对象转orientation_event_t对象，主要给脚本语言使用。 |
| <a href="#orientation_event_t_orientation_event_init">orientation\_event\_init</a> | 初始化事件。 |
### 属性
<p id="orientation_event_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#orientation_event_t_orientation">orientation</a> | int32\_t | 屏幕方向。 |
#### orientation\_event\_cast 函数
-----------------------

* 函数功能：

> <p id="orientation_event_t_orientation_event_cast">把event对象转orientation_event_t对象，主要给脚本语言使用。

* 函数原型：

```
orientation_event_t* orientation_event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | orientation\_event\_t* | event对象。 |
| event | event\_t* | event对象。 |
#### orientation\_event\_init 函数
-----------------------

* 函数功能：

> <p id="orientation_event_t_orientation_event_init">初始化事件。

* 函数原型：

```
event_t* orientation_event_init (orientation_event_t* event, void* target, uint32_t type, int32_t dy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | event对象。 |
| event | orientation\_event\_t* | event对象。 |
| target | void* | 事件目标。 |
| type | uint32\_t | 事件类型。 |
| dy | int32\_t | 滚轮的y值。 |
#### orientation 属性
-----------------------
> <p id="orientation_event_t_orientation">屏幕方向。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
