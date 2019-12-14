## file\_receiver\_t
### 概述
文件接收方。

> 可用注册EVT_PROGRESS事件，以获取传输的进度状态。
----------------------------------
### 函数
<p id="file_receiver_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#file_receiver_t_file_receiver_create">file\_receiver\_create</a> | 创建receiver对象。 |
| <a href="#file_receiver_t_file_receiver_destroy">file\_receiver\_destroy</a> | 销毁receiver对象。 |
| <a href="#file_receiver_t_file_receiver_run">file\_receiver\_run</a> | 开始传输文件。 |
#### file\_receiver\_create 函数
-----------------------

* 函数功能：

> <p id="file_receiver_t_file_receiver_create">创建receiver对象。

* 函数原型：

```
file_receiver_t* file_receiver_create (uint32_t block_size, const char* productid);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | file\_receiver\_t* | 返回receiver对象。 |
| block\_size | uint32\_t | 一次传输块的大小。 |
| productid | const char* | 产品名称(有时可用于发送方发送不同的固件，暂时没用)。 |
#### file\_receiver\_destroy 函数
-----------------------

* 函数功能：

> <p id="file_receiver_t_file_receiver_destroy">销毁receiver对象。

* 函数原型：

```
ret_t file_receiver_destroy (file_receiver_t* receiver);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| receiver | file\_receiver\_t* | receiver对象。 |
#### file\_receiver\_run 函数
-----------------------

* 函数功能：

> <p id="file_receiver_t_file_receiver_run">开始传输文件。

* 函数原型：

```
ret_t file_receiver_run (file_receiver_t* receiver, tk_iostream_t* io, tk_ostream_t* target, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| receiver | file\_receiver\_t* | receiver对象。 |
| io | tk\_iostream\_t* | 数据传输流。 |
| target | tk\_ostream\_t* | 用于写入接收到的文件数据。 |
| filename | const char* | 文件名(期望接收的文件名)。 |
