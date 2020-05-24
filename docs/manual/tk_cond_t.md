## tk\_cond\_t
### 概述
条件变量。
----------------------------------
### 函数
<p id="tk_cond_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_cond_t_tk_cond_create">tk\_cond\_create</a> | 创建cond。 |
| <a href="#tk_cond_t_tk_cond_destroy">tk\_cond\_destroy</a> | 销毁cond对象。 |
| <a href="#tk_cond_t_tk_cond_signal">tk\_cond\_signal</a> | 唤醒。 |
| <a href="#tk_cond_t_tk_cond_wait">tk\_cond\_wait</a> | 等待。 |
| <a href="#tk_cond_t_tk_cond_wait_timeout">tk\_cond\_wait\_timeout</a> | 等待指定时间。 |
#### tk\_cond\_create 函数
-----------------------

* 函数功能：

> <p id="tk_cond_t_tk_cond_create">创建cond。

* 函数原型：

```
tk_cond_t* tk_cond_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_cond\_t* | cond对象。 |
#### tk\_cond\_destroy 函数
-----------------------

* 函数功能：

> <p id="tk_cond_t_tk_cond_destroy">销毁cond对象。

* 函数原型：

```
ret_t tk_cond_destroy (tk_cond_t* cond);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cond | tk\_cond\_t* | cond对象。 |
#### tk\_cond\_signal 函数
-----------------------

* 函数功能：

> <p id="tk_cond_t_tk_cond_signal">唤醒。

* 函数原型：

```
ret_t tk_cond_signal (tk_cond_t* cond);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cond | tk\_cond\_t* | cond对象。 |
#### tk\_cond\_wait 函数
-----------------------

* 函数功能：

> <p id="tk_cond_t_tk_cond_wait">等待。

* 函数原型：

```
ret_t tk_cond_wait (tk_cond_t* cond, tk_mutex_t* mutex);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cond | tk\_cond\_t* | cond对象。 |
| mutex | tk\_mutex\_t* | mutex对象。 |
#### tk\_cond\_wait\_timeout 函数
-----------------------

* 函数功能：

> <p id="tk_cond_t_tk_cond_wait_timeout">等待指定时间。

* 函数原型：

```
ret_t tk_cond_wait_timeout (tk_cond_t* cond, tk_mutex_t* mutex, uint32_t* timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cond | tk\_cond\_t* | cond对象。 |
| mutex | tk\_mutex\_t* | mutex对象。 |
| timeout\_ms | uint32\_t* | 最长等待时间。 |
