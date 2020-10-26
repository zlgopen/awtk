## udp\_t
### 概述

----------------------------------
### 函数
<p id="udp_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#udp_t_udp_connect">udp\_connect</a> | 连接到指定服务器。 |
| <a href="#udp_t_udp_listen">udp\_listen</a> | 监听指定端口，成功返回sock句柄。 |
#### udp\_connect 函数
-----------------------

* 函数功能：

> <p id="udp_t_udp_connect">连接到指定服务器。

* 函数原型：

```
int udp_connect (const char* host, int port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回sock句柄。 |
| host | const char* | 主机名或IP地址。 |
| port | int | 端口号。 |
#### udp\_listen 函数
-----------------------

* 函数功能：

> <p id="udp_t_udp_listen">监听指定端口，成功返回sock句柄。

* 函数原型：

```
int udp_listen (int port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回sock句柄。 |
| port | int | 端口号。 |
