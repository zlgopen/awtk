## tk\_service\_t
### 概述
服务接口。
----------------------------------
### 函数
<p id="tk_service_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_service_t_tk_service_destroy">tk\_service\_destroy</a> | 销毁服务对象。 |
| <a href="#tk_service_t_tk_service_dispatch">tk\_service\_dispatch</a> | 处理服务器请求。 |
| <a href="#tk_service_t_tk_service_download_file">tk\_service\_download\_file</a> | 处理下载文件。 |
| <a href="#tk_service_t_tk_service_init">tk\_service\_init</a> | 初始化服务对象(仅供子类使用)。 |
| <a href="#tk_service_t_tk_service_lock">tk\_service\_lock</a> | 加锁。 |
| <a href="#tk_service_t_tk_service_read_req">tk\_service\_read\_req</a> | 服务端读取请求。 |
| <a href="#tk_service_t_tk_service_send_resp">tk\_service\_send\_resp</a> | 服务端发送响应。 |
| <a href="#tk_service_t_tk_service_set_retry_times">tk\_service\_set\_retry\_times</a> | 设置重试次数。 |
| <a href="#tk_service_t_tk_service_start">tk\_service\_start</a> | 启动服务。 |
| <a href="#tk_service_t_tk_service_unlock">tk\_service\_unlock</a> | 解锁。 |
| <a href="#tk_service_t_tk_service_upload_file">tk\_service\_upload\_file</a> | 处理上传文件。 |
### 属性
<p id="tk_service_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#tk_service_t_io">io</a> | tk\_iostream\_t* | IO对象。 |
| <a href="#tk_service_t_wb">wb</a> | wbuffer\_t | 用于接收/发送数据打包。 |
#### tk\_service\_destroy 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_destroy">销毁服务对象。
> 服务负责销毁IO对象。

* 函数原型：

```
ret_t tk_service_destroy (tk_service_t* service);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_service\_t* | 服务对象。 |
#### tk\_service\_dispatch 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_dispatch">处理服务器请求。
> 返回非RET_OK，停止服务器，并销毁service对象。

* 函数原型：

```
ret_t tk_service_dispatch (tk_service_t* service);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_service\_t* | 服务对象。 |
#### tk\_service\_download\_file 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_download_file">处理下载文件。

* 函数原型：

```
ret_t tk_service_download_file (tk_service_t* service, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_service\_t* | service对象。 |
| filename | const char* | 文件名。 |
#### tk\_service\_init 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_init">初始化服务对象(仅供子类使用)。

* 函数原型：

```
ret_t tk_service_init (tk_service_t* service, tk_iostream_t* io);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_service\_t* | 服务对象。 |
| io | tk\_iostream\_t* | IO对象。 |
#### tk\_service\_lock 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_lock">加锁。

* 函数原型：

```
ret_t tk_service_lock (tk_service_t* service);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_service\_t* | 服务对象。 |
#### tk\_service\_read\_req 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_read_req">服务端读取请求。

* 函数原型：

```
ret_t tk_service_read_req (tk_service_t* service, tk_msg_header_t* header, wbuffer_t* wb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_service\_t* | service对象。 |
| header | tk\_msg\_header\_t* | 返回消息头。 |
| wb | wbuffer\_t* | 返回对其的数据。 |
#### tk\_service\_send\_resp 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_send_resp">服务端发送响应。

* 函数原型：

```
ret_t tk_service_send_resp (tk_service_t* service, uint32_t type, uint32_t data_type, uint32_t resp_code, wbuffer_t* wb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_service\_t* | service对象。 |
| type | uint32\_t | 消息类型。 |
| data\_type | uint32\_t | 数据类型。 |
| resp\_code | uint32\_t | 响应码。 |
| wb | wbuffer\_t* | 要发送的数据。 |
#### tk\_service\_set\_retry\_times 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_set_retry_times">设置重试次数。

* 函数原型：

```
ret_t tk_service_set_retry_times (tk_service_t* service, uint32_t retry_times);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_service\_t* | service对象。 |
| retry\_times | uint32\_t | 重试次数。 |
#### tk\_service\_start 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_start">启动服务。

* 函数原型：

```
ret_t tk_service_start (event_source_manager_t* esm, const char* url, tk_service_create_t create, void* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| esm | event\_source\_manager\_t* | 事件源管理器。 |
| url | const char* | 服务地址。 |
| create | tk\_service\_create\_t | 创建服务对象的函数。 |
| args | void* | 参数(对于TCP服务，该参数必须持续有效，使用全局或静态变量)。 |
#### tk\_service\_unlock 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_unlock">解锁。

* 函数原型：

```
ret_t tk_service_unlock (tk_service_t* service);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_service\_t* | 服务对象。 |
#### tk\_service\_upload\_file 函数
-----------------------

* 函数功能：

> <p id="tk_service_t_tk_service_upload_file">处理上传文件。

* 函数原型：

```
ret_t tk_service_upload_file (tk_service_t* service, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_service\_t* | service对象。 |
| filename | const char* | 文件名。 |
#### io 属性
-----------------------
> <p id="tk_service_t_io">IO对象。

* 类型：tk\_iostream\_t*

#### wb 属性
-----------------------
> <p id="tk_service_t_wb">用于接收/发送数据打包。

* 类型：wbuffer\_t

