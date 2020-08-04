## idle\_t
### 概述
idle可以看作是duration为0的定时器。

> idle可以用来实现一些异步处理。

示例：

```c
static ret_t something_on_idle(const idle_info_t* info) {
widget_t* widget = WIDGET(info->ctx);
edit_t* edit = EDIT(widget);
...
return RET_REMOVE;
}

...

idle_add(something_on_idle, edit);

```

> 在非GUI线程请用idle\_queue。
----------------------------------
### 函数
<p id="idle_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#idle_t_idle_add">idle\_add</a> | 增加一个idle。 |
| <a href="#idle_t_idle_count">idle\_count</a> | 返回idle的个数。 |
| <a href="#idle_t_idle_dispatch">idle\_dispatch</a> | 调用全部idle的函数。 |
| <a href="#idle_t_idle_queue">idle\_queue</a> | 用于非GUI线程增加一个idle，本函数向主循环的事件队列中发送一个增加idle的请求。 |
| <a href="#idle_t_idle_remove">idle\_remove</a> | 删除指定的idle。 |
| <a href="#idle_t_idle_set_on_destroy">idle\_set\_on\_destroy</a> | 设置一个回调函数，在idle被销毁时调用(方便脚本语言去释放回调函数)。 |
#### idle\_add 函数
-----------------------

* 函数功能：

> <p id="idle_t_idle_add">增加一个idle。

* 函数原型：

```
uint32_t idle_add (idle_func_t on_idle, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回idle的ID，0表示失败。 |
| on\_idle | idle\_func\_t | idle回调函数，回调函数返回RET\_REPEAT，则下次继续执行，否则自动移出。 |
| ctx | void* | idle回调函数的上下文。 |
#### idle\_count 函数
-----------------------

* 函数功能：

> <p id="idle_t_idle_count">返回idle的个数。

* 函数原型：

```
uint32_t idle_count ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回idle的个数。 |
#### idle\_dispatch 函数
-----------------------

* 函数功能：

> <p id="idle_t_idle_dispatch">调用全部idle的函数。

* 函数原型：

```
ret_t idle_dispatch ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### idle\_queue 函数
-----------------------

* 函数功能：

> <p id="idle_t_idle_queue">用于非GUI线程增加一个idle，本函数向主循环的事件队列中发送一个增加idle的请求。

* 函数原型：

```
ret_t idle_queue (idle_func_t on_idle, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| on\_idle | idle\_func\_t | idle回调函数。 |
| ctx | void* | idle回调函数的上下文。 |
#### idle\_remove 函数
-----------------------

* 函数功能：

> <p id="idle_t_idle_remove">删除指定的idle。

* 函数原型：

```
ret_t idle_remove (uint32_t idle_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_id | uint32\_t | idleID。 |
#### idle\_set\_on\_destroy 函数
-----------------------

* 函数功能：

> <p id="idle_t_idle_set_on_destroy">设置一个回调函数，在idle被销毁时调用(方便脚本语言去释放回调函数)。

* 函数原型：

```
ret_t idle_set_on_destroy (uint32_t idle_id, tk_destroy_t on_destroy, void* on_destroy_ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| idle\_id | uint32\_t | idleID。 |
| on\_destroy | tk\_destroy\_t | 回调函数。 |
| on\_destroy\_ctx | void* | 回调函数上下文。 |
