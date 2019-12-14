## file\_sender\_t
### 概述
文件发送方。

> 可用注册EVT_PROGRESS事件，以获取传输的进度状态。
----------------------------------
### 函数
<p id="file_sender_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#file_sender_t_file_sender_create">file\_sender\_create</a> | 创建sender对象。 |
| <a href="#file_sender_t_file_sender_destroy">file\_sender\_destroy</a> | 销毁sender对象。 |
| <a href="#file_sender_t_file_sender_run">file\_sender\_run</a> | 开始传输文件。 |
#### file\_sender\_create 函数
-----------------------

* 函数功能：

> <p id="file_sender_t_file_sender_create">创建sender对象。

* 函数原型：

```
file_sender_t* file_sender_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | file\_sender\_t* | 返回sender对象。 |
#### file\_sender\_destroy 函数
-----------------------

* 函数功能：

> <p id="file_sender_t_file_sender_destroy">销毁sender对象。

* 函数原型：

```
ret_t file_sender_destroy (file_sender_t* sender);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sender | file\_sender\_t* | sender对象。 |
#### file\_sender\_run 函数
-----------------------

* 函数功能：

> <p id="file_sender_t_file_sender_run">开始传输文件。

* 函数原型：

```
ret_t file_sender_run (file_sender_t* sender, tk_iostream_t* io, const char* filename);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| sender | file\_sender\_t* | sender对象。 |
| io | tk\_iostream\_t* | 数据传输流。 |
| filename | const char* | 文件名。 |
