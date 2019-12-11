## event\_source\_manager\_t
### 概述
事件源管理器。
----------------------------------
### 函数
<p id="event_source_manager_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#event_source_manager_t_event_source_manager_add">event\_source\_manager\_add</a> | 增加事件源对象。 |
| <a href="#event_source_manager_t_event_source_manager_deinit">event\_source\_manager\_deinit</a> | ~初始化。 |
| <a href="#event_source_manager_t_event_source_manager_destroy">event\_source\_manager\_destroy</a> | 销毁事件源管理器。 |
| <a href="#event_source_manager_t_event_source_manager_dispatch">event\_source\_manager\_dispatch</a> | 分发事件。 |
| <a href="#event_source_manager_t_event_source_manager_get_wakeup_time">event\_source\_manager\_get\_wakeup\_time</a> | 获取wakeup时间(ms) |
| <a href="#event_source_manager_t_event_source_manager_init">event\_source\_manager\_init</a> | 初始化。 |
| <a href="#event_source_manager_t_event_source_manager_remove">event\_source\_manager\_remove</a> | 移除事件源对象。 |
| <a href="#event_source_manager_t_event_source_manager_remove_by_tag">event\_source\_manager\_remove\_by\_tag</a> | 移除所有tag相同的事件源对象。 |
#### event\_source\_manager\_add 函数
-----------------------

* 函数功能：

> <p id="event_source_manager_t_event_source_manager_add">增加事件源对象。

* 函数原型：

```
ret_t event_source_manager_add (event_source_manager_t* manager, event_source_t* source);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| manager | event\_source\_manager\_t* | event\_source\_manager对象。 |
| source | event\_source\_t* | event\_source对象。 |
#### event\_source\_manager\_deinit 函数
-----------------------

* 函数功能：

> <p id="event_source_manager_t_event_source_manager_deinit">~初始化。

* 函数原型：

```
ret_t event_source_manager_deinit (event_source_manager_t* manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| manager | event\_source\_manager\_t* | event\_source\_manager对象。 |
#### event\_source\_manager\_destroy 函数
-----------------------

* 函数功能：

> <p id="event_source_manager_t_event_source_manager_destroy">销毁事件源管理器。

* 函数原型：

```
ret_t event_source_manager_destroy (event_source_manager_t* manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| manager | event\_source\_manager\_t* | event\_source\_manager对象。 |
#### event\_source\_manager\_dispatch 函数
-----------------------

* 函数功能：

> <p id="event_source_manager_t_event_source_manager_dispatch">分发事件。

* 函数原型：

```
ret_t event_source_manager_dispatch (event_source_manager_t* manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| manager | event\_source\_manager\_t* | event\_source\_manager对象。 |
#### event\_source\_manager\_get\_wakeup\_time 函数
-----------------------

* 函数功能：

> <p id="event_source_manager_t_event_source_manager_get_wakeup_time">获取wakeup时间(ms)

* 函数原型：

```
uint32_t event_source_manager_get_wakeup_time (event_source_manager_t* manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回wakeup时间(ms)。 |
| manager | event\_source\_manager\_t* | event\_source\_manager对象。 |
#### event\_source\_manager\_init 函数
-----------------------

* 函数功能：

> <p id="event_source_manager_t_event_source_manager_init">初始化。

* 函数原型：

```
ret_t event_source_manager_init (event_source_manager_t* manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| manager | event\_source\_manager\_t* | event\_source\_manager对象。 |
#### event\_source\_manager\_remove 函数
-----------------------

* 函数功能：

> <p id="event_source_manager_t_event_source_manager_remove">移除事件源对象。

* 函数原型：

```
ret_t event_source_manager_remove (event_source_manager_t* manager, event_source_t* source);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| manager | event\_source\_manager\_t* | event\_source\_manager对象。 |
| source | event\_source\_t* | event\_source对象。 |
#### event\_source\_manager\_remove\_by\_tag 函数
-----------------------

* 函数功能：

> <p id="event_source_manager_t_event_source_manager_remove_by_tag">移除所有tag相同的事件源对象。

* 函数原型：

```
ret_t event_source_manager_remove_by_tag (event_source_manager_t* manager, void* tag);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| manager | event\_source\_manager\_t* | event\_source\_manager对象。 |
| tag | void* | tag。 |
