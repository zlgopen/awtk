## socket
### 概述

----------------------------------
### 函数
<p id="socket_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#socket_socket_last_io_has_error">socket\_last\_io\_has\_error</a> | 判断最后一次IO操作是否正常。 |
| <a href="#socket_socket_resolve">socket\_resolve</a> | 解析主机名，并初始化addr对象。 |
| <a href="#socket_tk_socket_bind">tk\_socket\_bind</a> | 绑定到指定端口。 |
| <a href="#socket_tk_socket_bind_ex">tk\_socket\_bind\_ex</a> | 绑定到指定IP和端口。 |
| <a href="#socket_tk_socket_close">tk\_socket\_close</a> | 关闭socket句柄。 |
| <a href="#socket_tk_socket_deinit">tk\_socket\_deinit</a> | tcp/udp 平台~初始化。 |
| <a href="#socket_tk_socket_init">tk\_socket\_init</a> | tcp/udp 平台初始化。 |
| <a href="#socket_tk_socket_set_blocking">tk\_socket\_set\_blocking</a> | 设置为阻塞或非阻塞模式。 |
| <a href="#socket_tk_socket_wait_for_data">tk\_socket\_wait\_for\_data</a> | 等待数据。 |
#### socket\_last\_io\_has\_error 函数
-----------------------

* 函数功能：

> <p id="socket_socket_last_io_has_error">判断最后一次IO操作是否正常。

* 函数原型：

```
bool_t socket_last_io_has_error ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示有错误，否则表示正常。 |
#### socket\_resolve 函数
-----------------------

* 函数功能：

> <p id="socket_socket_resolve">解析主机名，并初始化addr对象。

* 函数原型：

```
struct sockaddr* socket_resolve (const char* host, int port, struct sockaddr_in* addr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | struct sockaddr* | 返回地址对象。 |
| host | const char* | 主机名。 |
| port | int | 端口号。 |
| addr | struct sockaddr\_in* | 地址对象。 |
#### tk\_socket\_bind 函数
-----------------------

* 函数功能：

> <p id="socket_tk_socket_bind">绑定到指定端口。

* 函数原型：

```
ret_t tk_socket_bind (int sock, int port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sock | int | socket句柄。 |
| port | int | 端口号。 |
#### tk\_socket\_bind\_ex 函数
-----------------------

* 函数功能：

> <p id="socket_tk_socket_bind_ex">绑定到指定IP和端口。

* 函数原型：

```
ret_t tk_socket_bind_ex (int sock, const char* ip, int port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sock | int | socket句柄。 |
| ip | const char* | IP地址(可以为NULL)。 |
| port | int | 端口号。 |
#### tk\_socket\_close 函数
-----------------------

* 函数功能：

> <p id="socket_tk_socket_close">关闭socket句柄。

* 函数原型：

```
ret_t tk_socket_close (int sock);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sock | int | socket句柄。 |
#### tk\_socket\_deinit 函数
-----------------------

* 函数功能：

> <p id="socket_tk_socket_deinit">tcp/udp 平台~初始化。

* 函数原型：

```
ret_t tk_socket_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### tk\_socket\_init 函数
-----------------------

* 函数功能：

> <p id="socket_tk_socket_init">tcp/udp 平台初始化。

* 函数原型：

```
ret_t tk_socket_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### tk\_socket\_set\_blocking 函数
-----------------------

* 函数功能：

> <p id="socket_tk_socket_set_blocking">设置为阻塞或非阻塞模式。

* 函数原型：

```
ret_t tk_socket_set_blocking (int sock, bool_t blocking);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sock | int | socket句柄。 |
| blocking | bool\_t | 是否阻塞。 |
#### tk\_socket\_wait\_for\_data 函数
-----------------------

* 函数功能：

> <p id="socket_tk_socket_wait_for_data">等待数据。

* 函数原型：

```
ret_t tk_socket_wait_for_data (int sock, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sock | int | socket句柄。 |
| timeout\_ms | uint32\_t | 等待时间(ms)。 |
