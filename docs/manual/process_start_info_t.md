## process\_start\_info\_t
### 概述
子程序启动配置结构体。
----------------------------------
### 函数
<p id="process_start_info_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#process_start_info_t_process_create">process\_create</a> | 创建子进程。 |
| <a href="#process_start_info_t_process_destroy">process\_destroy</a> | 释放子进程。 |
| <a href="#process_start_info_t_process_handle_get_fd">process\_handle\_get\_fd</a> | 获取文件描述符。 |
| <a href="#process_start_info_t_process_is_broken">process\_is\_broken</a> | 子进程是否断开 |
| <a href="#process_start_info_t_process_kill">process\_kill</a> | 杀死子进程 |
| <a href="#process_start_info_t_process_read">process\_read</a> | 读子进程的数据（StdIntput 的方式） |
| <a href="#process_start_info_t_process_wait_for_data">process\_wait\_for\_data</a> | 等待数据。 |
| <a href="#process_start_info_t_process_write">process\_write</a> | 给子进程写数据（StdOutput 的方式） |
### 属性
<p id="process_start_info_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#process_start_info_t_work_dir">work\_dir</a> | char* | 子进程当前工作目录。（如果 NULL 的话，就为父进程的工作目录） |
#### process\_create 函数
-----------------------

* 函数功能：

> <p id="process_start_info_t_process_create">创建子进程。

* 函数原型：

```
process_handle_t process_create (const char* file_path, const char** args, uint32_t argc, const process_start_info_t* start_info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | process\_handle\_t | 返回子进程句柄。 |
| file\_path | const char* | 子进程程序路径。(如果为空的话，使用命令行来执行参数的内容) |
| args | const char** | 子进程参数。 |
| argc | uint32\_t | 子进程参数长度。 |
| start\_info | const process\_start\_info\_t* | 子进程启动信息。（如果 NULL，就使用默认是值） |
#### process\_destroy 函数
-----------------------

* 函数功能：

> <p id="process_start_info_t_process_destroy">释放子进程。

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

> <p id="process_start_info_t_process_handle_get_fd">获取文件描述符。

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

> <p id="process_start_info_t_process_is_broken">子进程是否断开

* 函数原型：

```
bool_t process_is_broken (process_handle_t handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 断开返回 true。 |
| handle | process\_handle\_t | 子进程句柄。 |
#### process\_kill 函数
-----------------------

* 函数功能：

> <p id="process_start_info_t_process_kill">杀死子进程

* 函数原型：

```
ret_t process_kill (process_handle_t handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| handle | process\_handle\_t | 子进程句柄。 |
#### process\_read 函数
-----------------------

* 函数功能：

> <p id="process_start_info_t_process_read">读子进程的数据（StdIntput 的方式）

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

> <p id="process_start_info_t_process_wait_for_data">等待数据。

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

> <p id="process_start_info_t_process_write">给子进程写数据（StdOutput 的方式）

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
#### work\_dir 属性
-----------------------
> <p id="process_start_info_t_work_dir">子进程当前工作目录。（如果 NULL 的话，就为父进程的工作目录）

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
