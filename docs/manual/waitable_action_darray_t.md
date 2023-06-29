## waitable\_action\_darray\_t
### 概述
waitable actionqueue
----------------------------------
### 函数
<p id="waitable_action_darray_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#waitable_action_darray_t_waitable_action_darray_create">waitable\_action\_darray\_create</a> | 创建waitable_action_darray对象。 |
| <a href="#waitable_action_darray_t_waitable_action_darray_destroy">waitable\_action\_darray\_destroy</a> | 销毁。 |
| <a href="#waitable_action_darray_t_waitable_action_darray_find">waitable\_action\_darray\_find</a> | 查找 qaction |
| <a href="#waitable_action_darray_t_waitable_action_darray_find_ex">waitable\_action\_darray\_find\_ex</a> | 查找 qaction |
| <a href="#waitable_action_darray_t_waitable_action_darray_recv">waitable\_action\_darray\_recv</a> | 接收一个请求。 |
| <a href="#waitable_action_darray_t_waitable_action_darray_remove">waitable\_action\_darray\_remove</a> | 从 darray 中移除并销毁 qaction 对象 |
| <a href="#waitable_action_darray_t_waitable_action_darray_remove_ex">waitable\_action\_darray\_remove\_ex</a> | 从 darray 中移除并销毁 qaction 对象 |
| <a href="#waitable_action_darray_t_waitable_action_darray_replace">waitable\_action\_darray\_replace</a> | 替换 qaction，旧的 qaction 会被销毁 |
| <a href="#waitable_action_darray_t_waitable_action_darray_replace_ex">waitable\_action\_darray\_replace\_ex</a> | 替换 qaction |
| <a href="#waitable_action_darray_t_waitable_action_darray_send">waitable\_action\_darray\_send</a> | 发送一个请求。 |
#### waitable\_action\_darray\_create 函数
-----------------------

* 函数功能：

> <p id="waitable_action_darray_t_waitable_action_darray_create">创建waitable_action_darray对象。

* 函数原型：

```
waitable_action_darray_t* waitable_action_darray_create (uint16_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | waitable\_action\_darray\_t* | waitable\_action\_darray对象。 |
| capacity | uint16\_t | action的容量。 |
#### waitable\_action\_darray\_destroy 函数
-----------------------

* 函数功能：

> <p id="waitable_action_darray_t_waitable_action_darray_destroy">销毁。

* 函数原型：

```
ret_t waitable_action_darray_destroy (waitable_action_darray_t* q);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | waitable\_action\_darray\_t* | waitable\_action\_darray对象。 |
#### waitable\_action\_darray\_find 函数
-----------------------

* 函数功能：

> <p id="waitable_action_darray_t_waitable_action_darray_find">查找 qaction

* 函数原型：

```
qaction_t* waitable_action_darray_find (waitable_action_darray_t* q, qaction_exec_t exec);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | qaction\_t* | 返回 qaction\_t对象 |
| q | waitable\_action\_darray\_t* | waitable\_action\_darray对象。 |
| exec | qaction\_exec\_t | action 中的函数回调 |
#### waitable\_action\_darray\_find\_ex 函数
-----------------------

* 函数功能：

> <p id="waitable_action_darray_t_waitable_action_darray_find_ex">查找 qaction

* 函数原型：

```
qaction_t* waitable_action_darray_find_ex (waitable_action_darray_t* q, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | qaction\_t* | 返回 qaction\_t对象 |
| q | waitable\_action\_darray\_t* | waitable\_action\_darray对象。 |
| cmp | tk\_compare\_t | 比较函数 |
| ctx | void* | 比较函数参数 |
#### waitable\_action\_darray\_recv 函数
-----------------------

* 函数功能：

> <p id="waitable_action_darray_t_waitable_action_darray_recv">接收一个请求。

* 函数原型：

```
ret_t waitable_action_darray_recv (waitable_action_darray_t* q, qaction_t** action, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | waitable\_action\_darray\_t* | waitable\_action\_darray对象。 |
| action | qaction\_t** | 用于返回action对象。 |
| timeout\_ms | uint32\_t | 超时时间(ms) |
#### waitable\_action\_darray\_remove 函数
-----------------------

* 函数功能：

> <p id="waitable_action_darray_t_waitable_action_darray_remove">从 darray 中移除并销毁 qaction 对象

* 函数原型：

```
ret_t waitable_action_darray_remove (waitable_action_darray_t* q, qaction_exec_t exec);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回 ret\_t值 |
| q | waitable\_action\_darray\_t* | waitable\_action\_darray对象。 |
| exec | qaction\_exec\_t | action 中的函数回调 |
#### waitable\_action\_darray\_remove\_ex 函数
-----------------------

* 函数功能：

> <p id="waitable_action_darray_t_waitable_action_darray_remove_ex">从 darray 中移除并销毁 qaction 对象

* 函数原型：

```
ret_t waitable_action_darray_remove_ex (waitable_action_darray_t* q, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回 ret\_t值 |
| q | waitable\_action\_darray\_t* | waitable\_action\_darray对象。 |
| cmp | tk\_compare\_t | 比较函数 |
| ctx | void* | 比较函数参数 |
#### waitable\_action\_darray\_replace 函数
-----------------------

* 函数功能：

> <p id="waitable_action_darray_t_waitable_action_darray_replace">替换 qaction，旧的 qaction 会被销毁

* 函数原型：

```
ret_t waitable_action_darray_replace (waitable_action_darray_t* q, qaction_t* new_action);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回 ret\_t值 |
| q | waitable\_action\_darray\_t* | waitable\_action\_darray对象。 |
| new\_action | qaction\_t* | 新的 action 对象 |
#### waitable\_action\_darray\_replace\_ex 函数
-----------------------

* 函数功能：

> <p id="waitable_action_darray_t_waitable_action_darray_replace_ex">替换 qaction

* 函数原型：

```
ret_t waitable_action_darray_replace_ex (waitable_action_darray_t* q, qaction_exec_t exec, qaction_t* new_action);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回 ret\_t值 |
| q | waitable\_action\_darray\_t* | waitable\_action\_darray对象。 |
| exec | qaction\_exec\_t | 旧 action 中的函数回调 |
| new\_action | qaction\_t* | 新的 action 对象 |
#### waitable\_action\_darray\_send 函数
-----------------------

* 函数功能：

> <p id="waitable_action_darray_t_waitable_action_darray_send">发送一个请求。

* 函数原型：

```
ret_t waitable_action_darray_send (waitable_action_darray_t* q, qaction_t* action, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | waitable\_action\_darray\_t* | waitable\_action\_darray对象。 |
| action | qaction\_t* | action对象。 |
| timeout\_ms | uint32\_t | 超时时间(ms) |
