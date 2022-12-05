## debugger\_server\_t
### 概述
@annotaion ["fake"]
调试器服务器。
----------------------------------
### 函数
<p id="debugger_server_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#debugger_server_t_debugger_server_find_debugger">debugger\_server\_find\_debugger</a> | 查找调试器对象。 |
| <a href="#debugger_server_t_debugger_server_is_running">debugger\_server\_is\_running</a> | 判断服务是否在运行。 |
| <a href="#debugger_server_t_debugger_server_set_single_mode">debugger\_server\_set\_single\_mode</a> | 设置为单一debugger模式。 |
| <a href="#debugger_server_t_debugger_server_set_single_mode_ex">debugger\_server\_set\_single\_mode\_ex</a> | 设置为单一debugger模式。 |
| <a href="#debugger_server_t_debugger_server_start">debugger\_server\_start</a> | 启用调试器服务。 |
| <a href="#debugger_server_t_debugger_server_stop">debugger\_server\_stop</a> | 停用调试器服务。 |
| <a href="#debugger_server_t_debugger_server_wait">debugger\_server\_wait</a> | 等待调试器服务退出。 |
#### debugger\_server\_find\_debugger 函数
-----------------------

* 函数功能：

> <p id="debugger_server_t_debugger_server_find_debugger">查找调试器对象。

* 函数原型：

```
debugger_t* debugger_server_find_debugger (const char* code_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_t* | 返回debugger对象或者NULL。 |
| code\_id | const char* | 代码ID。 |
#### debugger\_server\_is\_running 函数
-----------------------

* 函数功能：

> <p id="debugger_server_t_debugger_server_is_running">判断服务是否在运行。

* 函数原型：

```
bool_t debugger_server_is_running ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示在运行，否则表示没有运行。 |
#### debugger\_server\_set\_single\_mode 函数
-----------------------

* 函数功能：

> <p id="debugger_server_t_debugger_server_set_single_mode">设置为单一debugger模式。

* 函数原型：

```
ret_t debugger_server_set_single_mode (bool_t single_mode);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| single\_mode | bool\_t | 单一debugger模式 |
#### debugger\_server\_set\_single\_mode\_ex 函数
-----------------------

* 函数功能：

> <p id="debugger_server_t_debugger_server_set_single_mode_ex">设置为单一debugger模式。

* 函数原型：

```
ret_t debugger_server_set_single_mode_ex (bool_t single_mode, const char* lang, const char* code_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| single\_mode | bool\_t | 单一debugger模式 |
| lang | const char* | 语言。 |
| code\_id | const char* | 代码ID。 |
#### debugger\_server\_start 函数
-----------------------

* 函数功能：

> <p id="debugger_server_t_debugger_server_start">启用调试器服务。

* 函数原型：

```
ret_t debugger_server_start (tk_iostream_t* io);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| io | tk\_iostream\_t* | IO对象，用于和客户端通信。 |
#### debugger\_server\_stop 函数
-----------------------

* 函数功能：

> <p id="debugger_server_t_debugger_server_stop">停用调试器服务。

* 函数原型：

```
ret_t debugger_server_stop ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### debugger\_server\_wait 函数
-----------------------

* 函数功能：

> <p id="debugger_server_t_debugger_server_wait">等待调试器服务退出。

* 函数原型：

```
ret_t debugger_server_wait ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
