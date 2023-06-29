## offset\_change\_event\_t
### 概述
![image](images/offset_change_event_t_0.png)

值变化事件。
----------------------------------
### 函数
<p id="offset_change_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#offset_change_event_t_offset_change_event_cast">offset\_change\_event\_cast</a> | 把event对象转offset_change_event_t对象。 |
| <a href="#offset_change_event_t_offset_change_event_init">offset\_change\_event\_init</a> | 初始化事件。 |
### 属性
<p id="offset_change_event_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#offset_change_event_t_new_offset">new\_offset</a> | float\_t | 新值。 |
| <a href="#offset_change_event_t_old_offset">old\_offset</a> | float\_t | 旧值。 |
#### offset\_change\_event\_cast 函数
-----------------------

* 函数功能：

> <p id="offset_change_event_t_offset_change_event_cast">把event对象转offset_change_event_t对象。

* 函数原型：

```
offset_change_event_t* offset_change_event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | offset\_change\_event\_t* | event对象。 |
| event | event\_t* | event对象。 |
#### offset\_change\_event\_init 函数
-----------------------

* 函数功能：

> <p id="offset_change_event_t_offset_change_event_init">初始化事件。

* 函数原型：

```
event_t* offset_change_event_init (offset_change_event_t* event, uint32_t type, void* target, float_t old_offset, float_t new_offset);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | event对象。 |
| event | offset\_change\_event\_t* | event对象。 |
| type | uint32\_t | 事件类型。 |
| target | void* | 事件目标。 |
| old\_offset | float\_t | 旧的偏移数据。 |
| new\_offset | float\_t | 新的偏移数据。 |
#### new\_offset 属性
-----------------------
> <p id="offset_change_event_t_new_offset">新值。

* 类型：float\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### old\_offset 属性
-----------------------
> <p id="offset_change_event_t_old_offset">旧值。

* 类型：float\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
