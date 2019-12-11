## tk\_semaphore\_t
### 概述
信号量。
----------------------------------
### 函数
<p id="tk_semaphore_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_semaphore_t_tk_semaphore_create">tk\_semaphore\_create</a> | 创建信号量对象。 |
| <a href="#tk_semaphore_t_tk_semaphore_destroy">tk\_semaphore\_destroy</a> | 销毁信号量对象。 |
| <a href="#tk_semaphore_t_tk_semaphore_post">tk\_semaphore\_post</a> | 释放资源。 |
| <a href="#tk_semaphore_t_tk_semaphore_wait">tk\_semaphore\_wait</a> | 获取资源。 |
#### tk\_semaphore\_create 函数
-----------------------

* 函数功能：

> <p id="tk_semaphore_t_tk_semaphore_create">创建信号量对象。

* 函数原型：

```
tk_semaphore_t* tk_semaphore_create (uint32_t value, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_semaphore\_t* | semaphore对象。 |
| value | uint32\_t | 初始值。 |
| name | const char* | 名称。 |
#### tk\_semaphore\_destroy 函数
-----------------------

* 函数功能：

> <p id="tk_semaphore_t_tk_semaphore_destroy">销毁信号量对象。

* 函数原型：

```
ret_t tk_semaphore_destroy (tk_semaphore_t* semaphore);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| semaphore | tk\_semaphore\_t* | 信号量对象。 |
#### tk\_semaphore\_post 函数
-----------------------

* 函数功能：

> <p id="tk_semaphore_t_tk_semaphore_post">释放资源。

* 函数原型：

```
ret_t tk_semaphore_post (tk_semaphore_t* semaphore);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| semaphore | tk\_semaphore\_t* | 信号量对象。 |
#### tk\_semaphore\_wait 函数
-----------------------

* 函数功能：

> <p id="tk_semaphore_t_tk_semaphore_wait">获取资源。

* 函数原型：

```
ret_t tk_semaphore_wait (tk_semaphore_t* semaphore, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| semaphore | tk\_semaphore\_t* | 信号量对象。 |
| timeout\_ms | uint32\_t | 超时时间。 |
