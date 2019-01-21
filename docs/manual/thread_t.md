## thread\_t
### 概述
 线程对象。


----------------------------------
### 函数
<p id="thread_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#thread_t_thread_create">thread\_create</a> | 创建thread对象。 |
| <a href="#thread_t_thread_destroy">thread\_destroy</a> | 销毁thread对象。 |
| <a href="#thread_t_thread_join">thread\_join</a> | 等待线程退出。 |
| <a href="#thread_t_thread_start">thread\_start</a> | 启动线程。 |
#### thread\_create 函数
-----------------------

* 函数功能：

> <p id="thread_t_thread_create"> 创建thread对象。




* 函数原型：

```
thread_t* thread_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | thread\_t* | thread对象。 |
#### thread\_destroy 函数
-----------------------

* 函数功能：

> <p id="thread_t_thread_destroy"> 销毁thread对象。




* 函数原型：

```
ret_t thread_destroy (thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | thread\_t* | thread对象。 |
#### thread\_join 函数
-----------------------

* 函数功能：

> <p id="thread_t_thread_join"> 等待线程退出。




* 函数原型：

```
ret_t thread_join (thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | thread\_t* | thread对象。 |
#### thread\_start 函数
-----------------------

* 函数功能：

> <p id="thread_t_thread_start"> 启动线程。




* 函数原型：

```
ret_t thread_start (thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | thread\_t* | thread对象。 |
