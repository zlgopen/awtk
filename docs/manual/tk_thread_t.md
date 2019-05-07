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
| <a href="#tk_thread_t_tk_thread_join">tk\_thread\_join</a> | 等待线程退出。 |
| <a href="#tk_thread_t_tk_thread_start">tk\_thread\_start</a> | 启动线程。 |
#### tk\_thread\_create 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_create"> 创建thread对象。




* 函数原型：

```
tk_thread_t* tk_thread_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_thread\_t* | thread对象。 |
#### tk\_thread\_destroy 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_destroy"> 销毁thread对象。




* 函数原型：

```
ret_t tk_thread_destroy (tk_thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | tk\_thread\_t* | thread对象。 |
#### tk\_thread\_join 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_join"> 等待线程退出。




* 函数原型：

```
ret_t tk_thread_join (tk_thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | tk\_thread\_t* | thread对象。 |
#### tk\_thread\_start 函数
-----------------------

* 函数功能：

> <p id="tk_thread_t_tk_thread_start"> 启动线程。




* 函数原型：

```
ret_t tk_thread_start (tk_thread_t* thread);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| thread | tk\_thread\_t* | thread对象。 |
