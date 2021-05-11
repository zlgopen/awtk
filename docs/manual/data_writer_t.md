## data\_writer\_t
### 概述
数据写入接口。

>对可写的媒介，如内存、文件、flash和其它媒介提供一个统一的写入接口。
----------------------------------
### 函数
<p id="data_writer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#data_writer_t_data_writer_clear">data\_writer\_clear</a> | 清除指定URL的数据。 |
| <a href="#data_writer_t_data_writer_destroy">data\_writer\_destroy</a> | 销毁writer对象。 |
| <a href="#data_writer_t_data_writer_flush">data\_writer\_flush</a> | flush数据。 |
| <a href="#data_writer_t_data_writer_truncate">data\_writer\_truncate</a> | 截去指定长度之后的数据。 |
| <a href="#data_writer_t_data_writer_write">data\_writer\_write</a> | 在指定位置写入数据。 |
#### data\_writer\_clear 函数
-----------------------

* 函数功能：

> <p id="data_writer_t_data_writer_clear">清除指定URL的数据。

* 函数原型：

```
ret_t data_writer_clear (const char* url);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| url | const char* | URL。 |
#### data\_writer\_destroy 函数
-----------------------

* 函数功能：

> <p id="data_writer_t_data_writer_destroy">销毁writer对象。

* 函数原型：

```
ret_t data_writer_destroy (data_writer_t* writer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | data\_writer\_t* | writer对象。 |
#### data\_writer\_flush 函数
-----------------------

* 函数功能：

> <p id="data_writer_t_data_writer_flush">flush数据。

* 函数原型：

```
ret_t data_writer_flush (data_writer_t* writer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | data\_writer\_t* | writer对象。 |
#### data\_writer\_truncate 函数
-----------------------

* 函数功能：

> <p id="data_writer_t_data_writer_truncate">截去指定长度之后的数据。

* 函数原型：

```
ret_t data_writer_truncate (data_writer_t* writer, uint64_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | data\_writer\_t* | writer对象。 |
| size | uint64\_t | 保留长度。 |
#### data\_writer\_write 函数
-----------------------

* 函数功能：

> <p id="data_writer_t_data_writer_write">在指定位置写入数据。

* 函数原型：

```
int32_t data_writer_write (data_writer_t* writer, uint64_t offset, const void* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回实际读取数据的长度。 |
| writer | data\_writer\_t* | writer对象。 |
| offset | uint64\_t | 偏移量。 |
| data | const void* | 数据缓冲区。 |
| size | uint32\_t | 数据长度。 |
