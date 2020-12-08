## rlog\_t
### 概述
循环记录日志(支持多线程访问)。
如果我们把日志写入文件中，随着时间的推移，日志文件会越来越大，最终占满这个磁盘，出现不可预料的错误。
rlog提供一个循环记录日志的功能，让日志文件始终不会超出指定的大小，达到指定大小后再从头开始写。

> 为了实现简单，我们把日志文件分成0和1两个文件，先写文件0，到达指定最大值的一半时，再写文件1。
> 文件1到达指定最大值的一半时，删除文件0，并将文件1改名为文件0，重新创建文件1，继续写文件1，重复此过程。
----------------------------------
### 函数
<p id="rlog_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#rlog_t_rlog_create">rlog\_create</a> | 创建rlog对象。 |
| <a href="#rlog_t_rlog_destroy">rlog\_destroy</a> | 销毁日志对象。 |
| <a href="#rlog_t_rlog_print">rlog\_print</a> | 写入一条日志记录。 |
| <a href="#rlog_t_rlog_write">rlog\_write</a> | 写入一条日志记录。 |
#### rlog\_create 函数
-----------------------

* 函数功能：

> <p id="rlog_t_rlog_create">创建rlog对象。

```c
rlog_t* log = rlog_create("./logs/%d.log", 1020*1024, 256);
rlog_write(log, "hello\n");
rlog_destroy(log);
```

* 函数原型：

```
rlog_t* rlog_create (const char* filename_pattern, uint32_t max_size, uint32_t buff_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rlog\_t* | 返回rlog对象。 |
| filename\_pattern | const char* | 用来确定文件名的路径和文件名。 |
| max\_size | uint32\_t | log文件占用最大磁盘空间(字节)。 |
| buff\_size | uint32\_t | 用于指定print时的buff大小。 |
#### rlog\_destroy 函数
-----------------------

* 函数功能：

> <p id="rlog_t_rlog_destroy">销毁日志对象。

* 函数原型：

```
ret_t rlog_destroy (rlog_t* log);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| log | rlog\_t* | 日志对象。 |
#### rlog\_print 函数
-----------------------

* 函数功能：

> <p id="rlog_t_rlog_print">写入一条日志记录。

* 函数原型：

```
ret_t rlog_print (rlog_t* log, const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| log | rlog\_t* | 日志对象。 |
| format | const char* | 格式字符串。 |
#### rlog\_write 函数
-----------------------

* 函数功能：

> <p id="rlog_t_rlog_write">写入一条日志记录。

* 函数原型：

```
ret_t rlog_write (rlog_t* log, const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| log | rlog\_t* | 日志对象。 |
| str | const char* | 日志字符串。 |
