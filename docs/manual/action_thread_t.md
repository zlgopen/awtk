## action\_thread\_t
### 概述
执行action的线程。

> 每个线程都有一个action queue，可以是共享的queue，也可以是私有的queue。
----------------------------------
### 函数
<p id="action_thread_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#action_thread_t_action_thread_create">action\_thread\_create</a> | 创建action_thread对象。 |
| <a href="#action_thread_t_action_thread_create_with_queue">action\_thread\_create\_with\_queue</a> | 创建action_thread对象。 |
| <a href="#action_thread_t_action_thread_destroy">action\_thread\_destroy</a> | 销毁。 |
| <a href="#action_thread_t_action_thread_exec">action\_thread\_exec</a> | 让线程执行action。 |
| <a href="#action_thread_t_action_thread_set_on_idle">action\_thread\_set\_on\_idle</a> | 设置空闲时的回调函数。 |
| <a href="#action_thread_t_action_thread_set_on_quit">action\_thread\_set\_on\_quit</a> | 设置退出时的回调函数。 |
### 属性
<p id="action_thread_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#action_thread_t_executed_actions_nr">executed\_actions\_nr</a> | uint32\_t | 已经执行action的个数。 |
| <a href="#action_thread_t_queue">queue</a> | waitable\_action\_queue\_t* | action queue。 |
| <a href="#action_thread_t_thread">thread</a> | tk\_thread\_t* | 线程对象。 |
#### action\_thread\_create 函数
-----------------------

* 函数功能：

> <p id="action_thread_t_action_thread_create">创建action_thread对象。

* 函数原型：

```
action_thread_t* action_thread_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | action\_thread\_t* | action\_thread对象。 |
#### action\_thread\_create\_with\_queue 函数
-----------------------

* 函数功能：

> <p id="action_thread_t_action_thread_create_with_queue">创建action_thread对象。

* 函数原型：

```
action_thread_t* action_thread_create_with_queue (waitable_action_queue_t* queue);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | action\_thread\_t* | action\_thread对象。 |
| queue | waitable\_action\_queue\_t* | queue对象。 |
#### action\_thread\_destroy 函数
-----------------------

* 函数功能：

> <p id="action_thread_t_action_thread_destroy">销毁。

* 函数原型：

```
ret_t action_thread_destroy (action_thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | action\_thread\_t* | action\_thread对象。 |
#### action\_thread\_exec 函数
-----------------------

* 函数功能：

> <p id="action_thread_t_action_thread_exec">让线程执行action。

* 函数原型：

```
ret_t action_thread_exec (action_thread_t* thread, qaction_t* action);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | action\_thread\_t* | action\_thread对象。 |
| action | qaction\_t* | action对象。 |
#### action\_thread\_set\_on\_idle 函数
-----------------------

* 函数功能：

> <p id="action_thread_t_action_thread_set_on_idle">设置空闲时的回调函数。

* 函数原型：

```
ret_t action_thread_set_on_idle (action_thread_t* thread, action_thread_on_idle_t on_idle, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | action\_thread\_t* | action\_thread对象。 |
| on\_idle | action\_thread\_on\_idle\_t | 空闲时的回调函数。 |
| ctx | void* | 回调函数的上下文。 |
#### action\_thread\_set\_on\_quit 函数
-----------------------

* 函数功能：

> <p id="action_thread_t_action_thread_set_on_quit">设置退出时的回调函数。

* 函数原型：

```
ret_t action_thread_set_on_quit (action_thread_t* thread, action_thread_on_quit_t on_quit, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | action\_thread\_t* | action\_thread对象。 |
| on\_quit | action\_thread\_on\_quit\_t | 退出时的回调函数。 |
| ctx | void* | 回调函数的上下文。 |
#### executed\_actions\_nr 属性
-----------------------
> <p id="action_thread_t_executed_actions_nr">已经执行action的个数。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### queue 属性
-----------------------
> <p id="action_thread_t_queue">action queue。

* 类型：waitable\_action\_queue\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### thread 属性
-----------------------
> <p id="action_thread_t_thread">线程对象。

* 类型：tk\_thread\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
