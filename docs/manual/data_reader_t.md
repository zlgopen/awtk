## data\_reader\_t
### 概述
数据读取接口。

>用于抽象flash等外部设备。
----------------------------------
### 函数
<p id="data_reader_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#data_reader_t_data_reader_destroy">data\_reader\_destroy</a> | 销毁reader对象。 |
| <a href="#data_reader_t_data_reader_get_size">data\_reader\_get\_size</a> | 获取数据长度。 |
| <a href="#data_reader_t_data_reader_read">data\_reader\_read</a> | 在指定位置读取数据。 |
| <a href="#data_reader_t_data_reader_read_all">data\_reader\_read\_all</a> | 从指定的URL读取全部数据。 |
#### data\_reader\_destroy 函数
-----------------------

* 函数功能：

> <p id="data_reader_t_data_reader_destroy">销毁reader对象。

* 函数原型：

```
ret_t data_reader_destroy (data_reader_t* reader);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| reader | data\_reader\_t* | reader对象。 |
#### data\_reader\_get\_size 函数
-----------------------

* 函数功能：

> <p id="data_reader_t_data_reader_get_size">获取数据长度。

* 函数原型：

```
uint64_t data_reader_get_size (data_reader_t* reader);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint64\_t | 返回数据长度。 |
| reader | data\_reader\_t* | reader对象。 |
#### data\_reader\_read 函数
-----------------------

* 函数功能：

> <p id="data_reader_t_data_reader_read">在指定位置读取数据。

* 函数原型：

```
int32_t data_reader_read (data_reader_t* reader, uint64_t offset, void* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回实际读取数据的长度。 |
| reader | data\_reader\_t* | reader对象。 |
| offset | uint64\_t | 偏移量。 |
| data | void* | 用于读取数据的缓冲区。 |
| size | uint32\_t | 最大读取数据长度。 |
#### data\_reader\_read\_all 函数
-----------------------

* 函数功能：

> <p id="data_reader_t_data_reader_read_all">从指定的URL读取全部数据。

* 函数原型：

```
void* data_reader_read_all (const char* url, uint32_t* size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 返回全部数据，调用者需要调用TKMEM\_FREE释放返回值。 |
| url | const char* | URL。 |
| size | uint32\_t* | 返回数据长度。 |
