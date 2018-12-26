## window\_event\_t
### 概述
![image](images/window_event_t_0.png)

 窗口事件，由窗口管理器触发。

### 函数
<p id="window_event_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#window_event_t_event_init">event\_init</a> | 初始化事件。 |
| <a href="#window_event_t_pointer_event_rotate">pointer\_event\_rotate</a> | 根据屏幕旋转方向修正pointer_event中的坐标。 |
| <a href="#window_event_t_window_event_cast">window\_event\_cast</a> | 把event对象转window_event_t对象。主要给脚本语言使用。 |
### 属性
<p id="window_event_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#window_event_t_window">window</a> | widget\_t* | canvas。 |
### 事件
<p id="window_event_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### event\_init 函数
-----------------------

* 函数功能：

> <p id="window_event_t_event_init"> 初始化事件。




* 函数原型：

```
event_t event_init (uint32_t type, void* target);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_t | 事件对象。 |
| type | uint32\_t | 事件类型。 |
| target | void* | 目标对象。 |
#### pointer\_event\_rotate 函数
-----------------------

* 函数功能：

> <p id="window_event_t_pointer_event_rotate"> 根据屏幕旋转方向修正pointer_event中的坐标。




* 函数原型：

```
ret_t pointer_event_rotate (pointer_event_t* evt, system_info_t* info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| evt | pointer\_event\_t* | 指针事件对象。 |
| info | system\_info\_t* | 系统信息。 |
#### window\_event\_cast 函数
-----------------------

* 函数功能：

> <p id="window_event_t_window_event_cast"> 把event对象转window_event_t对象。主要给脚本语言使用。




* 函数原型：

```
window_event_t* window_event_cast (event_t* event);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | window\_event\_t* | 对象。 |
| event | event\_t* | event对象。 |
#### window 属性
-----------------------
> <p id="window_event_t_window"> canvas。



* 类型：widget\_t*

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
