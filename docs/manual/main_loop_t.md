## main\_loop\_t
### 概述
主循环接口。
----------------------------------
### 函数
<p id="main_loop_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#main_loop_t_main_loop">main\_loop</a> | 获取当前main_loop对象 |
| <a href="#main_loop_t_main_loop_add_event_source">main\_loop\_add\_event\_source</a> | 添加event_source。 |
| <a href="#main_loop_t_main_loop_destroy">main\_loop\_destroy</a> | 销毁main_loop对象。 |
| <a href="#main_loop_t_main_loop_get_event_source_manager">main\_loop\_get\_event\_source\_manager</a> | 获取event source manager |
| <a href="#main_loop_t_main_loop_queue_event">main\_loop\_queue\_event</a> | 将事件加入到事件队列。 |
| <a href="#main_loop_t_main_loop_quit">main\_loop\_quit</a> | 退出主循环。 |
| <a href="#main_loop_t_main_loop_recv_event">main\_loop\_recv\_event</a> | 从事件队列中获取事件。 |
| <a href="#main_loop_t_main_loop_remove_event_source">main\_loop\_remove\_event\_source</a> | 删除event_source。 |
| <a href="#main_loop_t_main_loop_remove_event_source_by_tag">main\_loop\_remove\_event\_source\_by\_tag</a> | 删除event_source。 |
| <a href="#main_loop_t_main_loop_run">main\_loop\_run</a> | 运行主循环。 |
| <a href="#main_loop_t_main_loop_set">main\_loop\_set</a> | 设置当前main_loop对象 |
| <a href="#main_loop_t_main_loop_step">main\_loop\_step</a> | 执行一次迭代。 |
| <a href="#main_loop_t_main_loop_wakeup">main\_loop\_wakeup</a> | 唤醒主循环。 |
#### main\_loop 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop">获取当前main_loop对象

* 函数原型：

```
main_loop_t* main_loop ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | main\_loop\_t* | 返回main\_loop对象。 |
#### main\_loop\_add\_event\_source 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_add_event_source">添加event_source。

* 函数原型：

```
ret_t main_loop_add_event_source (main_loop_t* l, event_source_t* source);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
| source | event\_source\_t* | event\_source对象。 |
#### main\_loop\_destroy 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_destroy">销毁main_loop对象。

* 函数原型：

```
ret_t main_loop_destroy (main_loop_t* l);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
#### main\_loop\_get\_event\_source\_manager 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_get_event_source_manager">获取event source manager

* 函数原型：

```
event_source_manager_t* main_loop_get_event_source_manager (main_loop_t* l);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | event\_source\_manager\_t* | 返回event source manager对象。 |
| l | main\_loop\_t* | main\_loop对象。 |
#### main\_loop\_queue\_event 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_queue_event">将事件加入到事件队列。

* 函数原型：

```
ret_t main_loop_queue_event (main_loop_t* l, const event_queue_req_t* e);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
| e | const event\_queue\_req\_t* | 事件。 |
#### main\_loop\_quit 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_quit">退出主循环。

* 函数原型：

```
ret_t main_loop_quit (main_loop_t* l);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
#### main\_loop\_recv\_event 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_recv_event">从事件队列中获取事件。

* 函数原型：

```
ret_t main_loop_recv_event (main_loop_t* l, event_queue_req_t* r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
| r | event\_queue\_req\_t* | 事件。 |
#### main\_loop\_remove\_event\_source 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_remove_event_source">删除event_source。

* 函数原型：

```
ret_t main_loop_remove_event_source (main_loop_t* l, event_source_t* source);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
| source | event\_source\_t* | event\_source对象。 |
#### main\_loop\_remove\_event\_source\_by\_tag 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_remove_event_source_by_tag">删除event_source。

* 函数原型：

```
ret_t main_loop_remove_event_source_by_tag (main_loop_t* l, void* tag);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
| tag | void* | tag。 |
#### main\_loop\_run 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_run">运行主循环。

* 函数原型：

```
ret_t main_loop_run (main_loop_t* l);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
#### main\_loop\_set 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_set">设置当前main_loop对象

* 函数原型：

```
ret_t main_loop_set (main_loop_t* loop);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| loop | main\_loop\_t* | main\_loop对象。 |
#### main\_loop\_step 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_step">执行一次迭代。

* 函数原型：

```
ret_t main_loop_step (main_loop_t* l);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
#### main\_loop\_wakeup 函数
-----------------------

* 函数功能：

> <p id="main_loop_t_main_loop_wakeup">唤醒主循环。

* 函数原型：

```
ret_t main_loop_wakeup (main_loop_t* l);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| l | main\_loop\_t* | main\_loop对象。 |
