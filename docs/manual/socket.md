## socket
### 概述

----------------------------------
### 函数
<p id="socket_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#socket_socket_bind">socket\_bind</a> | 绑定到指定端口。 |
| <a href="#socket_socket_close">socket\_close</a> | 关闭socket句柄。 |
| <a href="#socket_socket_deinit">socket\_deinit</a> | tcp/udp 平台~初始化。 |
| <a href="#socket_socket_init">socket\_init</a> | tcp/udp 平台初始化。 |
| <a href="#socket_socket_resolve">socket\_resolve</a> | 解析主机名，并初始化addr对象。 |
| <a href="#socket_socket_set_blocking">socket\_set\_blocking</a> | 设置为阻塞或非阻塞模式。 |
| <a href="#socket_socket_wait_for_data">socket\_wait\_for\_data</a> | 等待数据。 |
#### socket\_bind 函数
-----------------------

* 函数功能：

> <p id="socket_socket_bind">绑定到指定端口。

* 函数原型：

```
ret_t socket_bind (int sock, int port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sock | int | socket句柄。 |
| port | int | 端口号。 |
#### socket\_close 函数
-----------------------

* 函数功能：

> <p id="socket_socket_close">关闭socket句柄。

* 函数原型：

```
ret_t socket_close (int sock);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sock | int | socket句柄。 |
#### socket\_deinit 函数
-----------------------

* 函数功能：

> <p id="socket_socket_deinit">tcp/udp 平台~初始化。

* 函数原型：

```
ret_t socket_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### socket\_init 函数
-----------------------

* 函数功能：

> <p id="socket_socket_init">tcp/udp 平台初始化。

* 函数原型：

```
ret_t socket_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### socket\_resolve 函数
-----------------------

* 函数功能：

> <p id="socket_socket_resolve">解析主机名，并初始化addr对象。

* 函数原型：

```
struct sockaddr* socket_resolve (int sock, bool_t , struct sockaddr_in* addr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | struct sockaddr* | 返回地址对象。 |
| sock | int | socket句柄。 |
|  | bool\_t | g |
| addr | struct sockaddr\_in* | 地址对象。 |
#### socket\_set\_blocking 函数
-----------------------

* 函数功能：

> <p id="socket_socket_set_blocking">设置为阻塞或非阻塞模式。

* 函数原型：

```
ret_t socket_set_blocking (int sock, bool_t blocking);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sock | int | socket句柄。 |
| blocking | bool\_t | 是否阻塞。 |
#### socket\_wait\_for\_data 函数
-----------------------

* 函数功能：

> <p id="socket_socket_wait_for_data">等待数据。

* 函数原型：

```
ret_t socket_wait_for_data (int sock, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sock | int | socket句柄。 |
| timeout\_ms | uint32\_t | 等待时间(ms)。 |
