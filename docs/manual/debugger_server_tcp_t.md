## debugger\_server\_tcp\_t
### 概述
调试器TCP服务端。
----------------------------------
### 函数
<p id="debugger_server_tcp_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#debugger_server_tcp_t_debugger_server_tcp_deinit">debugger\_server\_tcp\_deinit</a> | 停止调试器服务。 |
| <a href="#debugger_server_tcp_t_debugger_server_tcp_init">debugger\_server\_tcp\_init</a> | 初始化调试器服务。 |
| <a href="#debugger_server_tcp_t_debugger_server_tcp_is_inited">debugger\_server\_tcp\_is\_inited</a> | 检查是否已经初始化。 |
| <a href="#debugger_server_tcp_t_debugger_server_tcp_start">debugger\_server\_tcp\_start</a> | 启动调试器服务。 |
| <a href="#debugger_server_tcp_t_debugger_server_tcp_start_async">debugger\_server\_tcp\_start\_async</a> | 启动调试器服务。 |
#### debugger\_server\_tcp\_deinit 函数
-----------------------

* 函数功能：

> <p id="debugger_server_tcp_t_debugger_server_tcp_deinit">停止调试器服务。

* 函数原型：

```
ret_t debugger_server_tcp_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### debugger\_server\_tcp\_init 函数
-----------------------

* 函数功能：

> <p id="debugger_server_tcp_t_debugger_server_tcp_init">初始化调试器服务。

* 函数原型：

```
ret_t debugger_server_tcp_init (uint32_t port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| port | uint32\_t | 监听端口。 |
#### debugger\_server\_tcp\_is\_inited 函数
-----------------------

* 函数功能：

> <p id="debugger_server_tcp_t_debugger_server_tcp_is_inited">检查是否已经初始化。

* 函数原型：

```
bool_t debugger_server_tcp_is_inited ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示已经初始化，否则表示没有。 |
#### debugger\_server\_tcp\_start 函数
-----------------------

* 函数功能：

> <p id="debugger_server_tcp_t_debugger_server_tcp_start">启动调试器服务。
> 接收客户端请求，并启动服务。

* 函数原型：

```
ret_t debugger_server_tcp_start ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### debugger\_server\_tcp\_start\_async 函数
-----------------------

* 函数功能：

> <p id="debugger_server_tcp_t_debugger_server_tcp_start_async">启动调试器服务。
> 接收客户端请求，并启动服务。

* 函数原型：

```
ret_t debugger_server_tcp_start_async ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
