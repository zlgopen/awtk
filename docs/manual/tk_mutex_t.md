## tk\_mutex\_t
### 概述
互斥锁。
----------------------------------
### 函数
<p id="tk_mutex_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_mutex_t_tk_mutex_create">tk\_mutex\_create</a> | 创建mutex。 |
| <a href="#tk_mutex_t_tk_mutex_destroy">tk\_mutex\_destroy</a> | 销毁mutex对象。 |
| <a href="#tk_mutex_t_tk_mutex_lock">tk\_mutex\_lock</a> | 加锁。 |
| <a href="#tk_mutex_t_tk_mutex_try_lock">tk\_mutex\_try\_lock</a> | 尝试加锁。 |
| <a href="#tk_mutex_t_tk_mutex_unlock">tk\_mutex\_unlock</a> | 解锁。 |
#### tk\_mutex\_create 函数
-----------------------

* 函数功能：

> <p id="tk_mutex_t_tk_mutex_create">创建mutex。

* 函数原型：

```
tk_mutex_t* tk_mutex_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_mutex\_t* | mutex对象。 |
#### tk\_mutex\_destroy 函数
-----------------------

* 函数功能：

> <p id="tk_mutex_t_tk_mutex_destroy">销毁mutex对象。

* 函数原型：

```
ret_t tk_mutex_destroy (tk_mutex_t* mutex);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| mutex | tk\_mutex\_t* | mutex对象。 |
#### tk\_mutex\_lock 函数
-----------------------

* 函数功能：

> <p id="tk_mutex_t_tk_mutex_lock">加锁。

* 函数原型：

```
ret_t tk_mutex_lock (tk_mutex_t* mutex);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| mutex | tk\_mutex\_t* | mutex对象。 |
#### tk\_mutex\_try\_lock 函数
-----------------------

* 函数功能：

> <p id="tk_mutex_t_tk_mutex_try_lock">尝试加锁。

* 函数原型：

```
ret_t tk_mutex_try_lock (tk_mutex_t* mutex);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| mutex | tk\_mutex\_t* | mutex对象。 |
#### tk\_mutex\_unlock 函数
-----------------------

* 函数功能：

> <p id="tk_mutex_t_tk_mutex_unlock">解锁。

* 函数原型：

```
ret_t tk_mutex_unlock (tk_mutex_t* mutex);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| mutex | tk\_mutex\_t* | mutex对象。 |
