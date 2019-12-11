## tk\_cond\_var\_t
### 概述
简化版的条件变量。
----------------------------------
### 函数
<p id="tk_cond_var_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_cond_var_t_tk_cond_var_awake">tk\_cond\_var\_awake</a> | 唤醒。 |
| <a href="#tk_cond_var_t_tk_cond_var_create">tk\_cond\_var\_create</a> | 创建cond_var。 |
| <a href="#tk_cond_var_t_tk_cond_var_destroy">tk\_cond\_var\_destroy</a> | 销毁cond_var对象。 |
| <a href="#tk_cond_var_t_tk_cond_var_wait">tk\_cond\_var\_wait</a> | 等待。 |
#### tk\_cond\_var\_awake 函数
-----------------------

* 函数功能：

> <p id="tk_cond_var_t_tk_cond_var_awake">唤醒。

* 函数原型：

```
ret_t tk_cond_var_awake (tk_cond_var_t* cond_var);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cond\_var | tk\_cond\_var\_t* | cond\_var对象。 |
#### tk\_cond\_var\_create 函数
-----------------------

* 函数功能：

> <p id="tk_cond_var_t_tk_cond_var_create">创建cond_var。

* 函数原型：

```
tk_cond_var_t* tk_cond_var_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_cond\_var\_t* | cond\_var对象。 |
#### tk\_cond\_var\_destroy 函数
-----------------------

* 函数功能：

> <p id="tk_cond_var_t_tk_cond_var_destroy">销毁cond_var对象。

* 函数原型：

```
ret_t tk_cond_var_destroy (tk_cond_var_t* cond_var);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cond\_var | tk\_cond\_var\_t* | cond\_var对象。 |
#### tk\_cond\_var\_wait 函数
-----------------------

* 函数功能：

> <p id="tk_cond_var_t_tk_cond_var_wait">等待。

* 函数原型：

```
ret_t tk_cond_var_wait (tk_cond_var_t* cond_var, uint32_t* timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| cond\_var | tk\_cond\_var\_t* | cond\_var对象。 |
| timeout\_ms | uint32\_t* | 最长等待时间。 |
