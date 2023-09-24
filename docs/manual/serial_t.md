## serial\_t
### 概述

----------------------------------
### 函数
<p id="serial_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#serial_t_serial_bytesize_from_str">serial\_bytesize\_from\_str</a> | 字符串转bytesize |
| <a href="#serial_t_serial_close">serial\_close</a> | 关闭串口 |
| <a href="#serial_t_serial_config">serial\_config</a> | 配置串口 |
| <a href="#serial_t_serial_flowcontrol_from_str">serial\_flowcontrol\_from\_str</a> | 字符串转flowcontrol |
| <a href="#serial_t_serial_handle_get_dev">serial\_handle\_get\_dev</a> | 获取设备句柄。 |
| <a href="#serial_t_serial_handle_get_fd">serial\_handle\_get\_fd</a> | 获取文件描述符。 |
| <a href="#serial_t_serial_iflush">serial\_iflush</a> | 刷新input缓冲区。 |
| <a href="#serial_t_serial_oflush">serial\_oflush</a> | 刷新output缓冲区。 |
| <a href="#serial_t_serial_open">serial\_open</a> | 打开串口 |
| <a href="#serial_t_serial_parity_from_str">serial\_parity\_from\_str</a> | 字符串转parity |
| <a href="#serial_t_serial_read">serial\_read</a> | 串口读数据 |
| <a href="#serial_t_serial_stopbits_from_str">serial\_stopbits\_from\_str</a> | 字符串转stopbits |
| <a href="#serial_t_serial_wait_for_data">serial\_wait\_for\_data</a> | 等待数据。 |
| <a href="#serial_t_serial_write">serial\_write</a> | 串口写数据 |
#### serial\_bytesize\_from\_str 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_bytesize_from_str">字符串转bytesize

* 函数原型：

```
bytesize_t serial_bytesize_from_str (const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bytesize\_t | 返回bytesize。 |
| str | const char* | 字符串。 |
#### serial\_close 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_close">关闭串口

* 函数原型：

```
ret_t serial_close (serial_handle_t handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| handle | serial\_handle\_t | 串口句柄。 |
#### serial\_config 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_config">配置串口

* 函数原型：

```
ret_t serial_config (serial_handle_t handle, uint32_t baudrate, bytesize_t bytesize, stopbits_t stopbits, flowcontrol_t flowcontrol, parity_t parity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| handle | serial\_handle\_t | 串口句柄。 |
| baudrate | uint32\_t | 波特率。 |
| bytesize | bytesize\_t | 数据位。 |
| stopbits | stopbits\_t | 停止位 |
| flowcontrol | flowcontrol\_t | 流控。 |
| parity | parity\_t | 校验位。 |
#### serial\_flowcontrol\_from\_str 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_flowcontrol_from_str">字符串转flowcontrol

* 函数原型：

```
flowcontrol_t serial_flowcontrol_from_str (const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | flowcontrol\_t | 返回flowcontrol。 |
| str | const char* | 字符串。 |
#### serial\_handle\_get\_dev 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_handle_get_dev">获取设备句柄。

* 函数原型：

```
serial_dev_t serial_handle_get_dev (serial_handle_t handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | serial\_dev\_t | 返回设备句柄。 |
| handle | serial\_handle\_t | 串口句柄。 |
#### serial\_handle\_get\_fd 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_handle_get_fd">获取文件描述符。

* 函数原型：

```
int serial_handle_get_fd (serial_handle_t handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回文件描述符。 |
| handle | serial\_handle\_t | 串口句柄。 |
#### serial\_iflush 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_iflush">刷新input缓冲区。

* 函数原型：

```
ret_t serial_iflush (serial_handle_t handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| handle | serial\_handle\_t | 串口句柄。 |
#### serial\_oflush 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_oflush">刷新output缓冲区。

* 函数原型：

```
ret_t serial_oflush (serial_handle_t handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| handle | serial\_handle\_t | 串口句柄。 |
#### serial\_open 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_open">打开串口
> Windows下，需要在应用程序初始化时，调用 tk_socket_init。

* 函数原型：

```
serial_handle_t serial_open (const char* port);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | serial\_handle\_t | 失败返回NULL。 |
| port | const char* | 串口号。 |
#### serial\_parity\_from\_str 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_parity_from_str">字符串转parity

* 函数原型：

```
parity_t serial_parity_from_str (const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | parity\_t | 返回parity。 |
| str | const char* | 字符串。 |
#### serial\_read 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_read">串口读数据

* 函数原型：

```
int32_t serial_read (serial_handle_t handle, uint8_t* buff, uint32_t max_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回实际读取的字节数。 |
| handle | serial\_handle\_t | 串口句柄。 |
| buff | uint8\_t* | 数据缓冲区。 |
| max\_size | uint32\_t | 数据长度。 |
#### serial\_stopbits\_from\_str 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_stopbits_from_str">字符串转stopbits

* 函数原型：

```
stopbits_t serial_stopbits_from_str (const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | stopbits\_t | 返回stopbits。 |
| str | const char* | 字符串。 |
#### serial\_wait\_for\_data 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_wait_for_data">等待数据。

* 函数原型：

```
ret_t serial_wait_for_data (serial_handle_t handle, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| handle | serial\_handle\_t | 串口句柄。 |
| timeout\_ms | uint32\_t | 等待时间(ms)。 |
#### serial\_write 函数
-----------------------

* 函数功能：

> <p id="serial_t_serial_write">串口写数据

* 函数原型：

```
int32_t serial_write (serial_handle_t handle, const uint8_t* buff, uint32_t max_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回实际写入的字节数。 |
| handle | serial\_handle\_t | 串口句柄。 |
| buff | const uint8\_t* | 数据缓冲区。 |
| max\_size | uint32\_t | 数据长度。 |
