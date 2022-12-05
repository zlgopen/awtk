## debugger\_global\_t
### 概述
@annotaion ["fake"]
调试器全局函数。
----------------------------------
### 函数
<p id="debugger_global_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#debugger_global_t_debugger_global_deinit">debugger\_global\_deinit</a> | 禁用调试器。 |
| <a href="#debugger_global_t_debugger_global_init">debugger\_global\_init</a> | 启用调试器。 |
#### debugger\_global\_deinit 函数
-----------------------

* 函数功能：

> <p id="debugger_global_t_debugger_global_deinit">禁用调试器。

* 函数原型：

```
ret_t debugger_global_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### debugger\_global\_init 函数
-----------------------

* 函数功能：

> <p id="debugger_global_t_debugger_global_init">启用调试器。

* 函数原型：

```
ret_t debugger_global_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
