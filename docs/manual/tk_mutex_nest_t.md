## tk\_mutex\_nest\_t
### 概述
嵌套互斥锁。允许同一个线程多次加锁。
----------------------------------
### 函数
<p id="tk_mutex_nest_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_mutex_nest_t_tk_mutex_nest_create">tk\_mutex\_nest\_create</a> | 创建嵌套互斥锁。 |
| <a href="#tk_mutex_nest_t_tk_mutex_nest_destroy">tk\_mutex\_nest\_destroy</a> | 销毁mutex_nest对象。 |
| <a href="#tk_mutex_nest_t_tk_mutex_nest_lock">tk\_mutex\_nest\_lock</a> | 加锁。 |
| <a href="#tk_mutex_nest_t_tk_mutex_nest_try_lock">tk\_mutex\_nest\_try\_lock</a> | 尝试加锁。 |
| <a href="#tk_mutex_nest_t_tk_mutex_nest_unlock">tk\_mutex\_nest\_unlock</a> | 解锁。只允许解锁当前线程所加的锁。 |
#### tk\_mutex\_nest\_create 函数
-----------------------

* 函数功能：

> <p id="tk_mutex_nest_t_tk_mutex_nest_create">创建嵌套互斥锁。

* 函数原型：

```
tk_mutex_nest_t* tk_mutex_nest_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_mutex\_nest\_t* | mutex\_nest对象。 |
#### tk\_mutex\_nest\_destroy 函数
-----------------------

* 函数功能：

> <p id="tk_mutex_nest_t_tk_mutex_nest_destroy">销毁mutex_nest对象。

* 函数原型：

```
ret_t tk_mutex_nest_destroy (tk_mutex_nest_t* mutex_nest);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| mutex\_nest | tk\_mutex\_nest\_t* | mutex\_nest对象。 |
#### tk\_mutex\_nest\_lock 函数
-----------------------

* 函数功能：

> <p id="tk_mutex_nest_t_tk_mutex_nest_lock">加锁。

* 函数原型：

```
ret_t tk_mutex_nest_lock (tk_mutex_nest_t* mutex_nest);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| mutex\_nest | tk\_mutex\_nest\_t* | mutex\_nest对象。 |
#### tk\_mutex\_nest\_try\_lock 函数
-----------------------

* 函数功能：

> <p id="tk_mutex_nest_t_tk_mutex_nest_try_lock">尝试加锁。

* 函数原型：

```
ret_t tk_mutex_nest_try_lock (tk_mutex_nest_t* mutex_nest);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| mutex\_nest | tk\_mutex\_nest\_t* | mutex\_nest对象。 |
#### tk\_mutex\_nest\_unlock 函数
-----------------------

* 函数功能：

> <p id="tk_mutex_nest_t_tk_mutex_nest_unlock">解锁。只允许解锁当前线程所加的锁。

* 函数原型：

```
ret_t tk_mutex_nest_unlock (tk_mutex_nest_t* mutex_nest);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| mutex\_nest | tk\_mutex\_nest\_t* | mutex\_nest对象。 |
