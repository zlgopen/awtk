## debugger\_lldb\_t
### 概述
调试器LLDB客户端(使用DAP协议与lldb-vscode连接)。
----------------------------------
### 函数
<p id="debugger_lldb_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#debugger_lldb_t_debugger_lldb_create">debugger\_lldb\_create</a> | 创建调试器TCP客户端对象。 |
| <a href="#debugger_lldb_t_debugger_lldb_create_ex">debugger\_lldb\_create\_ex</a> | 创建调试器TCP客户端对象。 |
#### debugger\_lldb\_create 函数
-----------------------

* 函数功能：

> <p id="debugger_lldb_t_debugger_lldb_create">创建调试器TCP客户端对象。

* 函数原型：

```
debugger_t* debugger_lldb_create (const char* host, uint32_t port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_t* | 返回debugger对象。 |
| host | const char* | 目标主机。 |
| port | uint32\_t | 目标端口。 |
#### debugger\_lldb\_create\_ex 函数
-----------------------

* 函数功能：

> <p id="debugger_lldb_t_debugger_lldb_create_ex">创建调试器TCP客户端对象。

* 函数原型：

```
debugger_t* debugger_lldb_create_ex (const char* host, uint32_t port, uint32_t timeout);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_t* | 返回debugger对象。 |
| host | const char* | 目标主机。 |
| port | uint32\_t | 目标端口。 |
| timeout | uint32\_t | 超时时间。 |
