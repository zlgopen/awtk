## graphic\_buffer\_t
### 概述
graphic_buffer。
----------------------------------
### 函数
<p id="graphic_buffer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#graphic_buffer_t_graphic_buffer_attach">graphic\_buffer\_attach</a> | 附件到指定的内存。 |
| <a href="#graphic_buffer_t_graphic_buffer_create_for_bitmap">graphic\_buffer\_create\_for\_bitmap</a> | 为位图创建缓冲区。 |
| <a href="#graphic_buffer_t_graphic_buffer_create_with_data">graphic\_buffer\_create\_with\_data</a> | 创建缓冲区。 |
| <a href="#graphic_buffer_t_graphic_buffer_destroy">graphic\_buffer\_destroy</a> | 销毁缓冲区。 |
| <a href="#graphic_buffer_t_graphic_buffer_is_valid_for">graphic\_buffer\_is\_valid\_for</a> | 用于检查graphic buffer的有效性。 |
| <a href="#graphic_buffer_t_graphic_buffer_lock_for_read">graphic\_buffer\_lock\_for\_read</a> | 为读取数据而锁定缓冲区。 |
| <a href="#graphic_buffer_t_graphic_buffer_lock_for_write">graphic\_buffer\_lock\_for\_write</a> | 为修改数据而锁定缓冲区。 |
| <a href="#graphic_buffer_t_graphic_buffer_unlock">graphic\_buffer\_unlock</a> | 解锁缓冲区。 |
#### graphic\_buffer\_attach 函数
-----------------------

* 函数功能：

> <p id="graphic_buffer_t_graphic_buffer_attach">附件到指定的内存。

* 函数原型：

```
ret_t graphic_buffer_attach (graphic_buffer_t* buffer, void* data, uint32_t w, uint32_t h);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| buffer | graphic\_buffer\_t* | 图像缓冲区对象。 |
| data | void* | 内存数据。 |
| w | uint32\_t | 宽度。 |
| h | uint32\_t | 宽度。 |
#### graphic\_buffer\_create\_for\_bitmap 函数
-----------------------

* 函数功能：

> <p id="graphic_buffer_t_graphic_buffer_create_for_bitmap">为位图创建缓冲区。

* 函数原型：

```
ret_t graphic_buffer_create_for_bitmap (bitmap_t* bitmap);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| bitmap | bitmap\_t* | 位图对象。 |
#### graphic\_buffer\_create\_with\_data 函数
-----------------------

* 函数功能：

> <p id="graphic_buffer_t_graphic_buffer_create_with_data">创建缓冲区。

> 为了兼容raw图像。

* 函数原型：

```
graphic_buffer_t* graphic_buffer_create_with_data (const uint8_t* data, uint32_t w, uint32_t h, bitmap_format_t format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | graphic\_buffer\_t* | 返回缓存区。 |
| data | const uint8\_t* | 数据。 |
| w | uint32\_t | 宽度。 |
| h | uint32\_t | 宽度。 |
| format | bitmap\_format\_t | 格式。 |
#### graphic\_buffer\_destroy 函数
-----------------------

* 函数功能：

> <p id="graphic_buffer_t_graphic_buffer_destroy">销毁缓冲区。

* 函数原型：

```
ret_t graphic_buffer_destroy (graphic_buffer_t* buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| buffer | graphic\_buffer\_t* | 图像缓冲区对象。 |
#### graphic\_buffer\_is\_valid\_for 函数
-----------------------

* 函数功能：

> <p id="graphic_buffer_t_graphic_buffer_is_valid_for">用于检查graphic buffer的有效性。

* 函数原型：

```
bool_t graphic_buffer_is_valid_for (graphic_buffer_t* buffer, bitmap_t* bitmap);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示有效，否则表示无效。 |
| buffer | graphic\_buffer\_t* | 图像缓冲区对象。 |
| bitmap | bitmap\_t* | 位图对象。 |
#### graphic\_buffer\_lock\_for\_read 函数
-----------------------

* 函数功能：

> <p id="graphic_buffer_t_graphic_buffer_lock_for_read">为读取数据而锁定缓冲区。

* 函数原型：

```
uint8_t* graphic_buffer_lock_for_read (graphic_buffer_t* buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t* | 返回缓存区的首地址。 |
| buffer | graphic\_buffer\_t* | 图像缓冲区对象。 |
#### graphic\_buffer\_lock\_for\_write 函数
-----------------------

* 函数功能：

> <p id="graphic_buffer_t_graphic_buffer_lock_for_write">为修改数据而锁定缓冲区。

* 函数原型：

```
uint8_t* graphic_buffer_lock_for_write (graphic_buffer_t* buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint8\_t* | 返回缓存区的首地址。 |
| buffer | graphic\_buffer\_t* | 图像缓冲区对象。 |
#### graphic\_buffer\_unlock 函数
-----------------------

* 函数功能：

> <p id="graphic_buffer_t_graphic_buffer_unlock">解锁缓冲区。

* 函数原型：

```
ret_t graphic_buffer_unlock (graphic_buffer_t* buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| buffer | graphic\_buffer\_t* | 图像缓冲区对象。 |
