## waitable\_ring\_buffer\_t
### 概述
waitable ring buffer
----------------------------------
### 函数
<p id="waitable_ring_buffer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#waitable_ring_buffer_t_waitable_ring_buffer_create">waitable\_ring\_buffer\_create</a> | 创建waitable_ring_buffer对象。 |
| <a href="#waitable_ring_buffer_t_waitable_ring_buffer_destroy">waitable\_ring\_buffer\_destroy</a> | 销毁。 |
| <a href="#waitable_ring_buffer_t_waitable_ring_buffer_read">waitable\_ring\_buffer\_read</a> | 读取数据。 |
| <a href="#waitable_ring_buffer_t_waitable_ring_buffer_write">waitable\_ring\_buffer\_write</a> | 写入数据。 |
#### waitable\_ring\_buffer\_create 函数
-----------------------

* 函数功能：

> <p id="waitable_ring_buffer_t_waitable_ring_buffer_create">创建waitable_ring_buffer对象。

* 函数原型：

```
waitable_ring_buffer_t* waitable_ring_buffer_create (uint32_t capacity, uint32_t block_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | waitable\_ring\_buffer\_t* | waitable\_ring\_buffer对象。 |
| capacity | uint32\_t | 容量。 |
| block\_size | uint32\_t | 块的大小。 |
#### waitable\_ring\_buffer\_destroy 函数
-----------------------

* 函数功能：

> <p id="waitable_ring_buffer_t_waitable_ring_buffer_destroy">销毁。

* 函数原型：

```
ret_t waitable_ring_buffer_destroy (waitable_ring_buffer_t* rb);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rb | waitable\_ring\_buffer\_t* | waitable\_ring\_buffer对象。 |
#### waitable\_ring\_buffer\_read 函数
-----------------------

* 函数功能：

> <p id="waitable_ring_buffer_t_waitable_ring_buffer_read">读取数据。

* 函数原型：

```
ret_t waitable_ring_buffer_read (waitable_ring_buffer_t* rb, void* buff, uint32_t size, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rb | waitable\_ring\_buffer\_t* | waitable\_ring\_buffer对象。 |
| buff | void* | 接收数据的buff。 |
| size | uint32\_t | 读取数据的长度(必须等于 block\_size)。 |
| timeout\_ms | uint32\_t | 超时时间(ms) |
#### waitable\_ring\_buffer\_write 函数
-----------------------

* 函数功能：

> <p id="waitable_ring_buffer_t_waitable_ring_buffer_write">写入数据。

* 函数原型：

```
ret_t waitable_ring_buffer_write (waitable_ring_buffer_t* rb, const void* buff, uint32_t size, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rb | waitable\_ring\_buffer\_t* | waitable\_ring\_buffer对象。 |
| buff | const void* | 要写入的数据。 |
| size | uint32\_t | 数据的长度(必须等于 block\_size)。 |
| timeout\_ms | uint32\_t | 超时时间(ms) |
