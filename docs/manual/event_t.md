## event\_t
### 概述
 事件基类。

### 函数
<p id="event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#event_t_event_cast">event\_cast</a> | 把event对象转wheel_event_t对象，主要给脚本语言使用。 |
### 属性
<p id="event_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#event_t_target">target</a> | void* | 事件发生的目标对象。 |
| <a href="#event_t_time">time</a> | int32\_t | 事件发生的时间。 |
| <a href="#event_t_type">type</a> | int32\_t | 类型。 |
### 事件
<p id="event_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### event\_cast 函数
-----------------------

* 函数功能：

> <p id="event_t_event_cast"> 把event对象转wheel_event_t对象，主要给脚本语言使用。




* 函数原型：

```
event_t* event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t* | 对象。 |
| event | event\_t* | event对象。 |
#### target 属性
-----------------------
> <p id="event_t_target"> 事件发生的目标对象。



* 类型：void*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 否 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 否 |
| 可在XML中设置 | 否 |
| 支通过widget_get_prop读取 | 否 |
| 支通过widget_set_prop修改 | 否 |
#### time 属性
-----------------------
> <p id="event_t_time"> 事件发生的时间。



* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 否 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 否 |
| 可在XML中设置 | 否 |
| 支通过widget_get_prop读取 | 否 |
| 支通过widget_set_prop修改 | 否 |
#### type 属性
-----------------------
> <p id="event_t_type"> 类型。



* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 否 |
| 可脚本化   | 是 |
| 可在IDE中设置 | 否 |
| 可在XML中设置 | 否 |
| 支通过widget_get_prop读取 | 否 |
| 支通过widget_set_prop修改 | 否 |
