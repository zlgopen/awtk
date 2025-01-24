## tk\_client\_t
### 概述
客户端接口。
----------------------------------
### 函数
<p id="tk_client_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_client_t_tk_client_deinit">tk\_client\_deinit</a> | 释放资源(仅供子类使用)。 |
| <a href="#tk_client_t_tk_client_download_file">tk\_client\_download\_file</a> | 客户端下载文件。 |
| <a href="#tk_client_t_tk_client_init">tk\_client\_init</a> | 初始化(仅供子类使用)。 |
| <a href="#tk_client_t_tk_client_read_notify">tk\_client\_read\_notify</a> | 客户端读取通知。 |
| <a href="#tk_client_t_tk_client_read_resp">tk\_client\_read\_resp</a> | 客户端读取响应。 |
| <a href="#tk_client_t_tk_client_request">tk\_client\_request</a> | 客户端发送请求，并读取响应。 |
| <a href="#tk_client_t_tk_client_send_req">tk\_client\_send\_req</a> | 客户端发送请求。 |
| <a href="#tk_client_t_tk_client_set_retry_times">tk\_client\_set\_retry\_times</a> | 设置重试次数。 |
| <a href="#tk_client_t_tk_client_set_timeout_ms">tk\_client\_set\_timeout\_ms</a> | 设置超时时间。 |
| <a href="#tk_client_t_tk_client_upload_file">tk\_client\_upload\_file</a> | 客户端上传文件。 |
### 属性
<p id="tk_client_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#tk_client_t_io">io</a> | tk\_iostream\_t* | IO对象。 |
| <a href="#tk_client_t_wb">wb</a> | wbuffer\_t | 用于接收/发送数据打包。 |
#### tk\_client\_deinit 函数
-----------------------

* 函数功能：

> <p id="tk_client_t_tk_client_deinit">释放资源(仅供子类使用)。

* 函数原型：

```
ret_t tk_client_deinit (tk_client_t* client);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| client | tk\_client\_t* | 服务对象。 |
#### tk\_client\_download\_file 函数
-----------------------

* 函数功能：

> <p id="tk_client_t_tk_client_download_file">客户端下载文件。

* 函数原型：

```
ret_t tk_client_download_file (tk_client_t* client, const char* remote_file, const char* local_file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| client | tk\_client\_t* | client对象。 |
| remote\_file | const char* | 远程文件。 |
| local\_file | const char* | 本地文件。 |
#### tk\_client\_init 函数
-----------------------

* 函数功能：

> <p id="tk_client_t_tk_client_init">初始化(仅供子类使用)。

* 函数原型：

```
ret_t tk_client_init (tk_client_t* client, tk_iostream_t* io, tk_client_on_notify_t on_notify);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| client | tk\_client\_t* | 服务对象。 |
| io | tk\_iostream\_t* | io对象。 |
| on\_notify | tk\_client\_on\_notify\_t | 通知回调。 |
#### tk\_client\_read\_notify 函数
-----------------------

* 函数功能：

> <p id="tk_client_t_tk_client_read_notify">客户端读取通知。

* 函数原型：

```
ret_t tk_client_read_notify (tk_client_t* client, uint32_t timeout);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| client | tk\_client\_t* | client对象。 |
| timeout | uint32\_t | 超时时间(毫秒)。 |
#### tk\_client\_read\_resp 函数
-----------------------

* 函数功能：

> <p id="tk_client_t_tk_client_read_resp">客户端读取响应。

* 函数原型：

```
ret_t tk_client_read_resp (tk_client_t* client, tk_msg_header_t* header, wbuffer_t* wb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| client | tk\_client\_t* | client对象。 |
| header | tk\_msg\_header\_t* | 返回消息头。 |
| wb | wbuffer\_t* | 返回读取的数据。 |
#### tk\_client\_request 函数
-----------------------

* 函数功能：

> <p id="tk_client_t_tk_client_request">客户端发送请求，并读取响应。

* 函数原型：

```
ret_t tk_client_request (tk_client_t* client, uint32_t type, uint32_t data_type, wbuffer_t* wb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| client | tk\_client\_t* | client对象。 |
| type | uint32\_t | 消息类型。 |
| data\_type | uint32\_t | 数据类型。 |
| wb | wbuffer\_t* | 要发送的数据/返回读取的数据。 |
#### tk\_client\_send\_req 函数
-----------------------

* 函数功能：

> <p id="tk_client_t_tk_client_send_req">客户端发送请求。

* 函数原型：

```
ret_t tk_client_send_req (tk_client_t* client, uint32_t type, uint32_t data_type, wbuffer_t* wb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| client | tk\_client\_t* | client对象。 |
| type | uint32\_t | 消息类型。 |
| data\_type | uint32\_t | 数据类型。 |
| wb | wbuffer\_t* | 要发送的数据。 |
#### tk\_client\_set\_retry\_times 函数
-----------------------

* 函数功能：

> <p id="tk_client_t_tk_client_set_retry_times">设置重试次数。

* 函数原型：

```
ret_t tk_client_set_retry_times (tk_client_t* client, uint32_t retry_times);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| client | tk\_client\_t* | client对象。 |
| retry\_times | uint32\_t | 重试次数。 |
#### tk\_client\_set\_timeout\_ms 函数
-----------------------

* 函数功能：

> <p id="tk_client_t_tk_client_set_timeout_ms">设置超时时间。

* 函数原型：

```
ret_t tk_client_set_timeout_ms (tk_client_t* client, uint32_t wtimeout_ms, uint32_t rtimeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| client | tk\_client\_t* | client对象。 |
| wtimeout\_ms | uint32\_t | 读数据超时时间。 |
| rtimeout\_ms | uint32\_t | 写数据超时时间。 |
#### tk\_client\_upload\_file 函数
-----------------------

* 函数功能：

> <p id="tk_client_t_tk_client_upload_file">客户端上传文件。

* 函数原型：

```
ret_t tk_client_upload_file (tk_client_t* client, const char* remote_file, const char* local_file);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| client | tk\_client\_t* | client对象。 |
| remote\_file | const char* | 远程文件。 |
| local\_file | const char* | 本地文件。 |
#### io 属性
-----------------------
> <p id="tk_client_t_io">IO对象。

* 类型：tk\_iostream\_t*

#### wb 属性
-----------------------
> <p id="tk_client_t_wb">用于接收/发送数据打包。

* 类型：wbuffer\_t

