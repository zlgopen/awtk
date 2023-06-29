## action\_darray\_thread\_t
### 概述
执行action的线程。

> 每个线程都有一个action darray，可以是共享的darray，也可以是私有的darray。
----------------------------------
### 函数
<p id="action_darray_thread_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#action_darray_thread_t_action_darray_thread_create">action\_darray\_thread\_create</a> | 创建action_darray_thread对象。 |
| <a href="#action_darray_thread_t_action_darray_thread_create_ex">action\_darray\_thread\_create\_ex</a> | 创建action_darray_thread对象。 |
| <a href="#action_darray_thread_t_action_darray_thread_create_with_darray">action\_darray\_thread\_create\_with\_darray</a> | 创建action_darray_thread对象。 |
| <a href="#action_darray_thread_t_action_darray_thread_create_with_darray_ex">action\_darray\_thread\_create\_with\_darray\_ex</a> | 创建action_darray_thread对象。 |
| <a href="#action_darray_thread_t_action_darray_thread_destroy">action\_darray\_thread\_destroy</a> | 销毁。 |
| <a href="#action_darray_thread_t_action_darray_thread_exec">action\_darray\_thread\_exec</a> | 让线程执行action。 |
| <a href="#action_darray_thread_t_action_darray_thread_set_on_idle">action\_darray\_thread\_set\_on\_idle</a> | 设置空闲时的回调函数。 |
| <a href="#action_darray_thread_t_action_darray_thread_set_on_quit">action\_darray\_thread\_set\_on\_quit</a> | 设置退出时的回调函数。 |
| <a href="#action_darray_thread_t_action_darray_thread_set_strategy">action\_darray\_thread\_set\_strategy</a> | 设置策略 |
| <a href="#action_darray_thread_t_action_queue_create">action\_queue\_create</a> | 创建action_queue对象。 |
| <a href="#action_darray_thread_t_action_queue_destroy">action\_queue\_destroy</a> | 销毁。 |
| <a href="#action_darray_thread_t_action_queue_recv">action\_queue\_recv</a> | 接收一个请求。 |
| <a href="#action_darray_thread_t_action_queue_send">action\_queue\_send</a> | 发送一个请求。 |
### 属性
<p id="action_darray_thread_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#action_darray_thread_t_darray">darray</a> | waitable\_action\_darray\_t* | action darray。 |
| <a href="#action_darray_thread_t_executed_actions_nr">executed\_actions\_nr</a> | uint32\_t | 已经执行action的个数。 |
| <a href="#action_darray_thread_t_thread">thread</a> | tk\_thread\_t* | 线程对象。 |
#### action\_darray\_thread\_create 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_darray_thread_create">创建action_darray_thread对象。

* 函数原型：

```
action_darray_thread_t* action_darray_thread_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | action\_darray\_thread\_t* | action\_darray\_thread对象。 |
#### action\_darray\_thread\_create\_ex 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_darray_thread_create_ex">创建action_darray_thread对象。

* 函数原型：

```
action_darray_thread_t* action_darray_thread_create_ex (const char* name, uint32_t stack_size, tk_thread_priority_t priority);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | action\_darray\_thread\_t* | action\_darray\_thread对象。 |
| name | const char* | 名称。 |
| stack\_size | uint32\_t | 栈的大小。 |
| priority | tk\_thread\_priority\_t | 优先级。 |
#### action\_darray\_thread\_create\_with\_darray 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_darray_thread_create_with_darray">创建action_darray_thread对象。

* 函数原型：

```
action_darray_thread_t* action_darray_thread_create_with_darray (waitable_action_darray_t* darray);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | action\_darray\_thread\_t* | action\_darray\_thread对象。 |
| darray | waitable\_action\_darray\_t* | darray对象。 |
#### action\_darray\_thread\_create\_with\_darray\_ex 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_darray_thread_create_with_darray_ex">创建action_darray_thread对象。

* 函数原型：

```
action_darray_thread_t* action_darray_thread_create_with_darray_ex (waitable_action_darray_t* darray, const char* name, uint32_t stack_size, tk_thread_priority_t priority);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | action\_darray\_thread\_t* | action\_darray\_thread对象。 |
| darray | waitable\_action\_darray\_t* | darray对象。 |
| name | const char* | 名称。 |
| stack\_size | uint32\_t | 栈的大小。 |
| priority | tk\_thread\_priority\_t | 优先级。 |
#### action\_darray\_thread\_destroy 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_darray_thread_destroy">销毁。

* 函数原型：

```
ret_t action_darray_thread_destroy (action_darray_thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | action\_darray\_thread\_t* | action\_darray\_thread对象。 |
#### action\_darray\_thread\_exec 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_darray_thread_exec">让线程执行action。

* 函数原型：

```
ret_t action_darray_thread_exec (action_darray_thread_t* thread, qaction_t* action);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | action\_darray\_thread\_t* | action\_darray\_thread对象。 |
| action | qaction\_t* | action对象。 |
#### action\_darray\_thread\_set\_on\_idle 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_darray_thread_set_on_idle">设置空闲时的回调函数。

* 函数原型：

```
ret_t action_darray_thread_set_on_idle (action_darray_thread_t* thread, action_darray_thread_on_idle_t on_idle, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | action\_darray\_thread\_t* | action\_darray\_thread对象。 |
| on\_idle | action\_darray\_thread\_on\_idle\_t | 空闲时的回调函数。 |
| ctx | void* | 回调函数的上下文。 |
#### action\_darray\_thread\_set\_on\_quit 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_darray_thread_set_on_quit">设置退出时的回调函数。

* 函数原型：

```
ret_t action_darray_thread_set_on_quit (action_darray_thread_t* thread, action_darray_thread_on_quit_t on_quit, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | action\_darray\_thread\_t* | action\_darray\_thread对象。 |
| on\_quit | action\_darray\_thread\_on\_quit\_t | 退出时的回调函数。 |
| ctx | void* | 回调函数的上下文。 |
#### action\_darray\_thread\_set\_strategy 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_darray_thread_set_strategy">设置策略

* 函数原型：

```
ret_t action_darray_thread_set_strategy (action_darray_thread_t* thread, action_darray_thread_strategy_t strategy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回 ret\_t值 |
| thread | action\_darray\_thread\_t* | action\_darray\_thread对象。 |
| strategy | action\_darray\_thread\_strategy\_t | 策略 |
#### action\_queue\_create 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_queue_create">创建action_queue对象。

* 函数原型：

```
action_queue_t* action_queue_create (uint16_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | action\_queue\_t* | action\_queue对象。 |
| capacity | uint16\_t | action的容量。 |
#### action\_queue\_destroy 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_queue_destroy">销毁。

* 函数原型：

```
ret_t action_queue_destroy (action_queue_t* q);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | action\_queue\_t* | action\_queue对象。 |
#### action\_queue\_recv 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_queue_recv">接收一个请求。

* 函数原型：

```
ret_t action_queue_recv (action_queue_t* q, qaction_t** action);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | action\_queue\_t* | action\_queue对象。 |
| action | qaction\_t** | 用于返回action对象。 |
#### action\_queue\_send 函数
-----------------------

* 函数功能：

> <p id="action_darray_thread_t_action_queue_send">发送一个请求。

* 函数原型：

```
ret_t action_queue_send (action_queue_t* q, qaction_t* action);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | action\_queue\_t* | action\_queue对象。 |
| action | qaction\_t* | action对象。 |
#### darray 属性
-----------------------
> <p id="action_darray_thread_t_darray">action darray。

* 类型：waitable\_action\_darray\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### executed\_actions\_nr 属性
-----------------------
> <p id="action_darray_thread_t_executed_actions_nr">已经执行action的个数。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### thread 属性
-----------------------
> <p id="action_darray_thread_t_thread">线程对象。

* 类型：tk\_thread\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
