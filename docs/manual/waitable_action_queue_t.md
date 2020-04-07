## waitable\_action\_queue\_t
### 概述
waitable actionqueue
----------------------------------
### 函数
<p id="waitable_action_queue_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#waitable_action_queue_t_waitable_action_queue_create">waitable\_action\_queue\_create</a> | 创建waitable_action_queue对象。 |
| <a href="#waitable_action_queue_t_waitable_action_queue_destroy">waitable\_action\_queue\_destroy</a> | 销毁。 |
| <a href="#waitable_action_queue_t_waitable_action_queue_recv">waitable\_action\_queue\_recv</a> | 接收一个请求。 |
| <a href="#waitable_action_queue_t_waitable_action_queue_send">waitable\_action\_queue\_send</a> | 发送一个请求。 |
#### waitable\_action\_queue\_create 函数
-----------------------

* 函数功能：

> <p id="waitable_action_queue_t_waitable_action_queue_create">创建waitable_action_queue对象。

* 函数原型：

```
waitable_action_queue_t* waitable_action_queue_create (uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | waitable\_action\_queue\_t* | waitable\_action\_queue对象。 |
| capacity | uint32\_t | action的容量。 |
#### waitable\_action\_queue\_destroy 函数
-----------------------

* 函数功能：

> <p id="waitable_action_queue_t_waitable_action_queue_destroy">销毁。

* 函数原型：

```
ret_t waitable_action_queue_destroy (waitable_action_queue_t* q);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | waitable\_action\_queue\_t* | waitable\_action\_queue对象。 |
#### waitable\_action\_queue\_recv 函数
-----------------------

* 函数功能：

> <p id="waitable_action_queue_t_waitable_action_queue_recv">接收一个请求。

* 函数原型：

```
ret_t waitable_action_queue_recv (waitable_action_queue_t* q, qaction_t** action, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | waitable\_action\_queue\_t* | waitable\_action\_queue对象。 |
| action | qaction\_t** | 用于返回action对象。 |
| timeout\_ms | uint32\_t | 超时时间(ms) |
#### waitable\_action\_queue\_send 函数
-----------------------

* 函数功能：

> <p id="waitable_action_queue_t_waitable_action_queue_send">发送一个请求。

* 函数原型：

```
ret_t waitable_action_queue_send (waitable_action_queue_t* q, qaction_t* action, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | waitable\_action\_queue\_t* | waitable\_action\_queue对象。 |
| action | qaction\_t* | action对象。 |
| timeout\_ms | uint32\_t | 超时时间(ms) |
