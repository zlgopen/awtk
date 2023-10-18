## tk\_ostream\_process\_t
### 概述
基于串口实现的输出流。

input stream base on fd
----------------------------------
### 函数
<p id="tk_ostream_process_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_ostream_process_t_process_create">process\_create</a> | 创建子进程。 |
| <a href="#tk_ostream_process_t_process_destroy">process\_destroy</a> | 释放子进程。 |
| <a href="#tk_ostream_process_t_process_handle_get_fd">process\_handle\_get\_fd</a> | 获取文件描述符。 |
| <a href="#tk_ostream_process_t_process_is_broken">process\_is\_broken</a> | 子进程是否断开 |
| <a href="#tk_ostream_process_t_process_read">process\_read</a> | 读子进程的数据（StdIntput 的方式） |
| <a href="#tk_ostream_process_t_process_wait_for_data">process\_wait\_for\_data</a> | 等待数据。 |
| <a href="#tk_ostream_process_t_process_write">process\_write</a> | 给子进程写数据（StdOutput 的方式） |
| <a href="#tk_ostream_process_t_tk_ostream_process_create">tk\_ostream\_process\_create</a> | 创建ostream对象。 |
#### process\_create 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_process_t_process_create">创建子进程。

* 函数原型：

```
process_handle_t process_create (const char* file_path, const char** args, uint32_t argc);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | process\_handle\_t | 返回子进程句柄。 |
| file\_path | const char* | 子进程程序路径。(如果为空的话，使用命令行来执行参数的内容) |
| args | const char** | 子进程参数。 |
| argc | uint32\_t | 子进程参数长度。 |
#### process\_destroy 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_process_t_process_destroy">释放子进程。

* 函数原型：

```
ret_t process_destroy (process_handle_t handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| handle | process\_handle\_t | 子进程句柄。 |
#### process\_handle\_get\_fd 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_process_t_process_handle_get_fd">获取文件描述符。

* 函数原型：

```
int process_handle_get_fd (process_handle_t handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 返回文件描述符。 |
| handle | process\_handle\_t | 子进程句柄。 |
#### process\_is\_broken 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_process_t_process_is_broken">子进程是否断开

* 函数原型：

```
bool_t process_is_broken (process_handle_t handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 断开返回 true。 |
| handle | process\_handle\_t | 子进程句柄。 |
#### process\_read 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_process_t_process_read">读子进程的数据（StdIntput 的方式）

* 函数原型：

```
int32_t process_read (process_handle_t handle, uint8_t* buff, uint32_t max_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回实际读取的字节数。 |
| handle | process\_handle\_t | 子进程句柄。 |
| buff | uint8\_t* | 数据缓冲区。 |
| max\_size | uint32\_t | 数据长度。 |
#### process\_wait\_for\_data 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_process_t_process_wait_for_data">等待数据。

* 函数原型：

```
ret_t process_wait_for_data (process_handle_t handle, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| handle | process\_handle\_t | 子进程句柄。 |
| timeout\_ms | uint32\_t | 等待时间(ms)。 |
#### process\_write 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_process_t_process_write">给子进程写数据（StdOutput 的方式）

* 函数原型：

```
int32_t process_write (process_handle_t handle, const uint8_t* buff, uint32_t max_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回实际写入的字节数。 |
| handle | process\_handle\_t | 子进程句柄。 |
| buff | const uint8\_t* | 数据缓冲区。 |
| max\_size | uint32\_t | 数据长度。 |
#### tk\_ostream\_process\_create 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_process_t_tk_ostream_process_create">创建ostream对象。

* 函数原型：

```
tk_ostream_t* tk_ostream_process_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_ostream\_t* | 返回ostream对象。 |
