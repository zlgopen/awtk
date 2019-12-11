## ring\_buffer\_t
### 概述
循环缓存区。
----------------------------------
### 函数
<p id="ring_buffer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#ring_buffer_t_ring_buffer_capacity">ring\_buffer\_capacity</a> | 获取容量。 |
| <a href="#ring_buffer_t_ring_buffer_create">ring\_buffer\_create</a> | 创建ring_buffer对象。 |
| <a href="#ring_buffer_t_ring_buffer_destroy">ring\_buffer\_destroy</a> | 销毁ring_buffer。 |
| <a href="#ring_buffer_t_ring_buffer_ensure_write_space">ring\_buffer\_ensure\_write\_space</a> | 扩展buffer。 |
| <a href="#ring_buffer_t_ring_buffer_free_size">ring\_buffer\_free\_size</a> | 获取空闲空间的长度。 |
| <a href="#ring_buffer_t_ring_buffer_is_empty">ring\_buffer\_is\_empty</a> | 检查ring_buffer是否空。 |
| <a href="#ring_buffer_t_ring_buffer_is_full">ring\_buffer\_is\_full</a> | 检查ring_buffer是否满。 |
| <a href="#ring_buffer_t_ring_buffer_peek">ring\_buffer\_peek</a> | 读取数据(不修改读取位置)。 |
| <a href="#ring_buffer_t_ring_buffer_read">ring\_buffer\_read</a> | 读取数据。 |
| <a href="#ring_buffer_t_ring_buffer_read_len">ring\_buffer\_read\_len</a> | 读取指定长度数据，要么成功要么失败。 |
| <a href="#ring_buffer_t_ring_buffer_reset">ring\_buffer\_reset</a> | 重置ring_buffer为空。 |
| <a href="#ring_buffer_t_ring_buffer_set_read_cursor">ring\_buffer\_set\_read\_cursor</a> | 设置读取光标的位置。 |
| <a href="#ring_buffer_t_ring_buffer_set_read_cursor_delta">ring\_buffer\_set\_read\_cursor\_delta</a> | 设置读取光标的位置(delta)。 |
| <a href="#ring_buffer_t_ring_buffer_set_write_cursor">ring\_buffer\_set\_write\_cursor</a> | 设置写入光标的位置。 |
| <a href="#ring_buffer_t_ring_buffer_set_write_cursor_delta">ring\_buffer\_set\_write\_cursor\_delta</a> | 设置写入光标的位置(delta)。 |
| <a href="#ring_buffer_t_ring_buffer_size">ring\_buffer\_size</a> | 获取数据长度。 |
| <a href="#ring_buffer_t_ring_buffer_skip">ring\_buffer\_skip</a> | 跳过指定长度数据，要么成功要么失败。 |
| <a href="#ring_buffer_t_ring_buffer_write">ring\_buffer\_write</a> | 写入数据。 |
| <a href="#ring_buffer_t_ring_buffer_write_len">ring\_buffer\_write\_len</a> | 写入指定长度数据，要么成功要么失败。 |
### 属性
<p id="ring_buffer_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#ring_buffer_t_capacity">capacity</a> | uint32\_t | 当前容量。 |
| <a href="#ring_buffer_t_data">data</a> | uint8\_t* | 数据。 |
| <a href="#ring_buffer_t_full">full</a> | bool\_t | 是否满。 |
| <a href="#ring_buffer_t_max_capacity">max\_capacity</a> | uint32\_t | 最大容量。 |
| <a href="#ring_buffer_t_r">r</a> | uint32\_t | 读取位置。 |
| <a href="#ring_buffer_t_w">w</a> | uint32\_t | 写入位置。 |
#### ring\_buffer\_capacity 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_capacity">获取容量。

* 函数原型：

```
uint32_t ring_buffer_capacity (ring_buffer_t* ring_buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 容量。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
#### ring\_buffer\_create 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_create">创建ring_buffer对象。

* 函数原型：

```
ring_buffer_t* ring_buffer_create (uint32_t init_capacity, uint32_t max_capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ring\_buffer\_t* | ring\_buffer对象。 |
| init\_capacity | uint32\_t | 初始容量。 |
| max\_capacity | uint32\_t | 最大容量。 |
#### ring\_buffer\_destroy 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_destroy">销毁ring_buffer。

* 函数原型：

```
ret_t ring_buffer_destroy (ring_buffer_t* ring_buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
#### ring\_buffer\_ensure\_write\_space 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_ensure_write_space">扩展buffer。

* 函数原型：

```
ret_t ring_buffer_ensure_write_space (ring_buffer_t* ring_buffer, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| size | uint32\_t | 需要的大小。 |
#### ring\_buffer\_free\_size 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_free_size">获取空闲空间的长度。

* 函数原型：

```
uint32_t ring_buffer_free_size (ring_buffer_t* ring_buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 空闲空间的长度。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
#### ring\_buffer\_is\_empty 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_is_empty">检查ring_buffer是否空。

* 函数原型：

```
bool_t ring_buffer_is_empty (ring_buffer_t* ring_buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 是否空。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
#### ring\_buffer\_is\_full 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_is_full">检查ring_buffer是否满。

* 函数原型：

```
bool_t ring_buffer_is_full (ring_buffer_t* ring_buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 是否满。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
#### ring\_buffer\_peek 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_peek">读取数据(不修改读取位置)。

* 函数原型：

```
uint32_t ring_buffer_peek (ring_buffer_t* ring_buffer, void* buff, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回实际读取数据的长度。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| buff | void* | 缓冲区。 |
| size | uint32\_t | 最大长度。 |
#### ring\_buffer\_read 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_read">读取数据。

* 函数原型：

```
uint32_t ring_buffer_read (ring_buffer_t* ring_buffer, void* buff, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回实际读取数据的长度。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| buff | void* | 缓冲区。 |
| size | uint32\_t | 最大长度。 |
#### ring\_buffer\_read\_len 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_read_len">读取指定长度数据，要么成功要么失败。

* 函数原型：

```
ret_t ring_buffer_read_len (ring_buffer_t* ring_buffer, void* buff, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| buff | void* | 缓冲区。 |
| size | uint32\_t | 最大长度。 |
#### ring\_buffer\_reset 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_reset">重置ring_buffer为空。

* 函数原型：

```
ret_t ring_buffer_reset (ring_buffer_t* ring_buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
#### ring\_buffer\_set\_read\_cursor 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_set_read_cursor">设置读取光标的位置。

* 函数原型：

```
ret_t ring_buffer_set_read_cursor (ring_buffer_t* ring_buffer, uint32_t r);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| r | uint32\_t | 读取光标的位置。 |
#### ring\_buffer\_set\_read\_cursor\_delta 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_set_read_cursor_delta">设置读取光标的位置(delta)。

* 函数原型：

```
ret_t ring_buffer_set_read_cursor_delta (ring_buffer_t* ring_buffer, uint32_t r_delta);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| r\_delta | uint32\_t | 读取光标的位置(delta)。 |
#### ring\_buffer\_set\_write\_cursor 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_set_write_cursor">设置写入光标的位置。

* 函数原型：

```
ret_t ring_buffer_set_write_cursor (ring_buffer_t* ring_buffer, uint32_t w);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| w | uint32\_t | 写入光标的位置。 |
#### ring\_buffer\_set\_write\_cursor\_delta 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_set_write_cursor_delta">设置写入光标的位置(delta)。

* 函数原型：

```
ret_t ring_buffer_set_write_cursor_delta (ring_buffer_t* ring_buffer, uint32_t w_delta);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| w\_delta | uint32\_t | 写入光标的位置(delta)。 |
#### ring\_buffer\_size 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_size">获取数据长度。

* 函数原型：

```
uint32_t ring_buffer_size (ring_buffer_t* ring_buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 数据长度。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
#### ring\_buffer\_skip 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_skip">跳过指定长度数据，要么成功要么失败。

* 函数原型：

```
ret_t ring_buffer_skip (ring_buffer_t* ring_buffer, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| size | uint32\_t | 长度。 |
#### ring\_buffer\_write 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_write">写入数据。

* 函数原型：

```
uint32_t ring_buffer_write (ring_buffer_t* ring_buffer, const void* buff, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回实际写入数据的长度。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| buff | const void* | 缓冲区。 |
| size | uint32\_t | 最大长度。 |
#### ring\_buffer\_write\_len 函数
-----------------------

* 函数功能：

> <p id="ring_buffer_t_ring_buffer_write_len">写入指定长度数据，要么成功要么失败。

* 函数原型：

```
ret_t ring_buffer_write_len (ring_buffer_t* ring_buffer, const void* buff, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ring\_buffer | ring\_buffer\_t* | ring\_buffer对象。 |
| buff | const void* | 缓冲区。 |
| size | uint32\_t | 最大长度。 |
#### capacity 属性
-----------------------
> <p id="ring_buffer_t_capacity">当前容量。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### data 属性
-----------------------
> <p id="ring_buffer_t_data">数据。

* 类型：uint8\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### full 属性
-----------------------
> <p id="ring_buffer_t_full">是否满。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### max\_capacity 属性
-----------------------
> <p id="ring_buffer_t_max_capacity">最大容量。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### r 属性
-----------------------
> <p id="ring_buffer_t_r">读取位置。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### w 属性
-----------------------
> <p id="ring_buffer_t_w">写入位置。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
