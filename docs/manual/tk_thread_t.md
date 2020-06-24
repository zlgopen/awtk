## tk\_thread\_t
### 概述
线程对象。
----------------------------------
### 函数
<p id="tk_thread_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_thread_t_tk_thread_create">tk\_thread\_create</a> | 创建thread对象。 |
| <a href="#tk_thread_t_tk_thread_destroy">tk\_thread\_destroy</a> | 销毁thread对象。 |
| <a href="#tk_thread_t_tk_thread_get_args">tk\_thread\_get\_args</a> | 获取线程的参数。 |
| <a href="#tk_thread_t_tk_thread_join">tk\_thread\_join</a> | 等待线程退出。 |
| <a href="#tk_thread_t_tk_thread_self">tk\_thread\_self</a> | 获取当前线程的原生句柄。 |
| <a href="#tk_thread_t_tk_thread_set_name">tk\_thread\_set\_name</a> | 设置线程的名称。 |
| <a href="#tk_thread_t_tk_thread_set_priority">tk\_thread\_set\_priority</a> | 设置线程的优先级。 |
| <a href="#tk_thread_t_tk_thread_set_stack_size">tk\_thread\_set\_stack\_size</a> | 设置线程的栈大小。 |
| <a href="#tk_thread_t_tk_thread_start">tk\_thread\_start</a> | 启动线程。 |
#### tk\_thread\_create 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_create">创建thread对象。

* 函数原型：

```
tk_thread_t* tk_thread_create (tk_thread_entry_t entry, void* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_thread\_t* | thread对象。 |
| entry | tk\_thread\_entry\_t | 线程函数。 |
| args | void* | 线程函数的上下文。 |
#### tk\_thread\_destroy 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_destroy">销毁thread对象。

* 函数原型：

```
ret_t tk_thread_destroy (tk_thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | tk\_thread\_t* | thread对象。 |
#### tk\_thread\_get\_args 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_get_args">获取线程的参数。

* 函数原型：

```
void* tk_thread_get_args (tk_thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 返回线程的参数。 |
| thread | tk\_thread\_t* | thread对象。 |
#### tk\_thread\_join 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_join">等待线程退出。

* 函数原型：

```
ret_t tk_thread_join (tk_thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | tk\_thread\_t* | thread对象。 |
#### tk\_thread\_self 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_self">获取当前线程的原生句柄。

* 函数原型：

```
uint64_t tk_thread_self ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint64\_t | 返回当前线程的原生句柄。 |
#### tk\_thread\_set\_name 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_set_name">设置线程的名称。

> 需要在调用start之前调用本函数。

* 函数原型：

```
ret_t tk_thread_set_name (tk_thread_t* thread, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | tk\_thread\_t* | thread对象。 |
| name | const char* | 名称。 |
#### tk\_thread\_set\_priority 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_set_priority">设置线程的优先级。

> 部分平台支持。

* 函数原型：

```
ret_t tk_thread_set_priority (tk_thread_t* thread, uint32_t priority);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | tk\_thread\_t* | thread对象。 |
| priority | uint32\_t | 优先级。 |
#### tk\_thread\_set\_stack\_size 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_set_stack_size">设置线程的栈大小。

> 需要在调用start之前调用本函数。

* 函数原型：

```
ret_t tk_thread_set_stack_size (tk_thread_t* thread, uint32_t stack_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | tk\_thread\_t* | thread对象。 |
| stack\_size | uint32\_t | 栈的大小。 |
#### tk\_thread\_start 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_start">启动线程。

* 函数原型：

```
ret_t tk_thread_start (tk_thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | tk\_thread\_t* | thread对象。 |
