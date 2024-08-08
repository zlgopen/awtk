## input\_device\_status\_t
### 概述
输入设备状态管理器。本类仅供窗口管理器内部使用。
----------------------------------
### 函数
<p id="input_device_status_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#input_device_status_t_input_device_status_abort_all_pressed_keys">input\_device\_status\_abort\_all\_pressed\_keys</a> | 取消全部已经按下的键。 |
| <a href="#input_device_status_t_input_device_status_deinit">input\_device\_status\_deinit</a> | 销毁输入设备状态管理器。 |
| <a href="#input_device_status_t_input_device_status_find_key_long_press_info">input\_device\_status\_find\_key\_long\_press\_info</a> | 查找按键的长按信息。 |
| <a href="#input_device_status_t_input_device_status_init">input\_device\_status\_init</a> | 初始化输入设备状态管理器。 |
| <a href="#input_device_status_t_input_device_status_on_ignore_input_event">input\_device\_status\_on\_ignore\_input\_event</a> | 对输入事件进行处理，已经按下的指针和键对应的抬起事件会分发给widget，其他的事件不会分发。 |
| <a href="#input_device_status_t_input_device_status_on_input_event">input\_device\_status\_on\_input\_event</a> | 对输入事件进行处理，然后分发给widget。 |
| <a href="#input_device_status_t_input_device_status_on_pointer_enter">input\_device\_status\_on\_pointer\_enter</a> | 处理指针进入事件。 |
| <a href="#input_device_status_t_input_device_status_on_pointer_leave">input\_device\_status\_on\_pointer\_leave</a> | 处理指针离开事件。 |
| <a href="#input_device_status_t_input_device_status_set_key_long_press_time">input\_device\_status\_set\_key\_long\_press\_time</a> | 设置按键长按时间。 |
#### input\_device\_status\_abort\_all\_pressed\_keys 函数
-----------------------

* 函数功能：

> <p id="input_device_status_t_input_device_status_abort_all_pressed_keys">取消全部已经按下的键。

窗口切换时，旧窗口按下的键，一直不松开，会持续触发PRESS，可能会干扰新窗口的用户功能。
所在窗口切换时，abort全部已经按下的键，直到按键松开。

* 函数原型：

```
ret_t input_device_status_abort_all_pressed_keys (input_device_status_t* ids);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ids | input\_device\_status\_t* | 输入设备状态管理器对象。 |
#### input\_device\_status\_deinit 函数
-----------------------

* 函数功能：

> <p id="input_device_status_t_input_device_status_deinit">销毁输入设备状态管理器。

* 函数原型：

```
ret_t input_device_status_deinit (input_device_status_t* ids);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ids | input\_device\_status\_t* | 输入设备状态管理器对象。 |
#### input\_device\_status\_find\_key\_long\_press\_info 函数
-----------------------

* 函数功能：

> <p id="input_device_status_t_input_device_status_find_key_long_press_info">查找按键的长按信息。

* 函数原型：

```
key_long_press_info_t* input_device_status_find_key_long_press_info (input_device_status_t* ids, uint32_t key);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | key\_long\_press\_info\_t* | 返回按键的长按信息。 |
| ids | input\_device\_status\_t* | 输入设备状态管理器对象。 |
| key | uint32\_t | 按键。 |
#### input\_device\_status\_init 函数
-----------------------

* 函数功能：

> <p id="input_device_status_t_input_device_status_init">初始化输入设备状态管理器。

* 函数原型：

```
input_device_status_t* input_device_status_init (input_device_status_t* ids);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | input\_device\_status\_t* | 返回输入设备状态管理器对象。 |
| ids | input\_device\_status\_t* | 输入设备状态管理器对象。 |
#### input\_device\_status\_on\_ignore\_input\_event 函数
-----------------------

* 函数功能：

> <p id="input_device_status_t_input_device_status_on_ignore_input_event">对输入事件进行处理，已经按下的指针和键对应的抬起事件会分发给widget，其他的事件不会分发。

* 函数原型：

```
ret_t input_device_status_on_ignore_input_event (input_device_status_t* ids, widget_t* widget, event_t* e);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ids | input\_device\_status\_t* | 输入设备状态管理器对象。 |
| widget | widget\_t* | 窗口管理器对象。 |
| e | event\_t* | 事件对象。 |
#### input\_device\_status\_on\_input\_event 函数
-----------------------

* 函数功能：

> <p id="input_device_status_t_input_device_status_on_input_event">对输入事件进行处理，然后分发给widget。

* 函数原型：

```
ret_t input_device_status_on_input_event (input_device_status_t* ids, widget_t* widget, event_t* e);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ids | input\_device\_status\_t* | 输入设备状态管理器对象。 |
| widget | widget\_t* | 窗口管理器对象。 |
| e | event\_t* | 事件对象。 |
#### input\_device\_status\_on\_pointer\_enter 函数
-----------------------

* 函数功能：

> <p id="input_device_status_t_input_device_status_on_pointer_enter">处理指针进入事件。

* 函数原型：

```
ret_t input_device_status_on_pointer_enter (input_device_status_t* ids, widget_t* widget, xy_t x, xy_t y);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ids | input\_device\_status\_t* | 输入设备状态管理器对象。 |
| widget | widget\_t* | 窗口管理器对象。 |
| x | xy\_t | x坐标。 |
| y | xy\_t | y坐标。 |
#### input\_device\_status\_on\_pointer\_leave 函数
-----------------------

* 函数功能：

> <p id="input_device_status_t_input_device_status_on_pointer_leave">处理指针离开事件。

* 函数原型：

```
ret_t input_device_status_on_pointer_leave (input_device_status_t* ids, widget_t* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ids | input\_device\_status\_t* | 输入设备状态管理器对象。 |
| widget | widget\_t* | 窗口管理器对象。 |
#### input\_device\_status\_set\_key\_long\_press\_time 函数
-----------------------

* 函数功能：

> <p id="input_device_status_t_input_device_status_set_key_long_press_time">设置按键长按时间。

* 函数原型：

```
ret_t input_device_status_set_key_long_press_time (input_device_status_t* ids, uint32_t key, uint32_t time);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ids | input\_device\_status\_t* | 输入设备状态管理器对象。 |
| key | uint32\_t | 按键。 |
| time | uint32\_t | 长按时间。 |
