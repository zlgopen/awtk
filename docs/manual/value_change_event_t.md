## value\_change\_event\_t
### 概述
![image](images/value_change_event_t_0.png)

值变化事件。
----------------------------------
### 函数
<p id="value_change_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#value_change_event_t_value_change_event_cast">value\_change\_event\_cast</a> | 把event对象转value_change_event_t对象，主要给脚本语言使用。 |
| <a href="#value_change_event_t_value_change_event_init">value\_change\_event\_init</a> | 初始化事件。 |
### 属性
<p id="value_change_event_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#value_change_event_t_new_value">new\_value</a> | value\_t | 新值。 |
| <a href="#value_change_event_t_old_value">old\_value</a> | value\_t | 旧值。 |
#### value\_change\_event\_cast 函数
-----------------------

* 函数功能：

> <p id="value_change_event_t_value_change_event_cast">把event对象转value_change_event_t对象，主要给脚本语言使用。

* 函数原型：

```
value_change_event_t* value_change_event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | value\_change\_event\_t* | event对象。 |
| event | event\_t* | event对象。 |
#### value\_change\_event\_init 函数
-----------------------

* 函数功能：

> <p id="value_change_event_t_value_change_event_init">初始化事件。

* 函数原型：

```
event_t* value_change_event_init (value_change_event_t* event, void* target, uint32_t type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | event对象。 |
| event | value\_change\_event\_t* | event对象。 |
| target | void* | 事件目标。 |
| type | uint32\_t | 事件类型。 |
#### new\_value 属性
-----------------------
> <p id="value_change_event_t_new_value">新值。

* 类型：value\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### old\_value 属性
-----------------------
> <p id="value_change_event_t_old_value">旧值。

* 类型：value\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
