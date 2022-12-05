## debugger\_client\_tcp\_t
### 概述
调试器TCP客户端。
----------------------------------
### 函数
<p id="debugger_client_tcp_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#debugger_client_tcp_t_debugger_client_tcp_create">debugger\_client\_tcp\_create</a> | 创建调试器TCP客户端对象。 |
#### debugger\_client\_tcp\_create 函数
-----------------------

* 函数功能：

> <p id="debugger_client_tcp_t_debugger_client_tcp_create">创建调试器TCP客户端对象。

* 函数原型：

```
debugger_t* debugger_client_tcp_create (const char* host, uint32_t port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_t* | 返回debugger对象。 |
| host | const char* | 目标主机。 |
| port | uint32\_t | 目标端口。 |
