## debugger\_client\_t
### 概述
调试器客户端。
----------------------------------
### 函数
<p id="debugger_client_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#debugger_client_t_debugger_client_cast">debugger\_client\_cast</a> | 类型转换。 |
| <a href="#debugger_client_t_debugger_client_create">debugger\_client\_create</a> | 创建调试器对象。 |
| <a href="#debugger_client_t_debugger_client_dispatch">debugger\_client\_dispatch</a> | 分发一个服务端推送的事件。 |
| <a href="#debugger_client_t_debugger_client_wait_for_completed">debugger\_client\_wait\_for\_completed</a> | 等待完成事件。 |
### 属性
<p id="debugger_client_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#debugger_client_t_io">io</a> | tk\_iostream\_t* | 与服务器通信的stream对象。 |
#### debugger\_client\_cast 函数
-----------------------

* 函数功能：

> <p id="debugger_client_t_debugger_client_cast">类型转换。

* 函数原型：

```
debugger_client_t* debugger_client_cast (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_client\_t* | 返回debugger对象。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_client\_create 函数
-----------------------

* 函数功能：

> <p id="debugger_client_t_debugger_client_create">创建调试器对象。

* 函数原型：

```
debugger_t* debugger_client_create (tk_iostream_t* io);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_t* | 返回debugger对象。 |
| io | tk\_iostream\_t* | io对象。 |
#### debugger\_client\_dispatch 函数
-----------------------

* 函数功能：

> <p id="debugger_client_t_debugger_client_dispatch">分发一个服务端推送的事件。

* 函数原型：

```
ret_t debugger_client_dispatch (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_client\_wait\_for\_completed 函数
-----------------------

* 函数功能：

> <p id="debugger_client_t_debugger_client_wait_for_completed">等待完成事件。

* 函数原型：

```
ret_t debugger_client_wait_for_completed (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
#### io 属性
-----------------------
> <p id="debugger_client_t_io">与服务器通信的stream对象。

* 类型：tk\_iostream\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
