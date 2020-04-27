## wbuffer\_t
### 概述
Write Buffer。用于数据打包。

示例：

```c
uint8_t buff[128];
wbuffer_t wbuffer;
rbuffer_t rbuffer;
const char* str = NULL;
wbuffer_init(&wbuffer, buff, sizeof(buff));

wbuffer_write_string(&wbuffer, "hello awtk");

rbuffer_init(&rbuffer, wbuffer.data, wbuffer.cursor);
rbuffer_read_string(&rbuffer, &str);
```

```c
wbuffer_t wbuffer;
wbuffer_init_extendable(&wbuffer);

wbuffer_write_string(&wbuffer, "hello awtk");

wbuffer_deinit(&wbuffer);
```
> 如果初始化为extendable，则最后需要调用wbuffer\_deinit释放资源。
----------------------------------
### 函数
<p id="wbuffer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#wbuffer_t_wbuffer_deinit">wbuffer\_deinit</a> | 释放资源。 |
| <a href="#wbuffer_t_wbuffer_extend_capacity">wbuffer\_extend\_capacity</a> | 扩展缓冲区至指定的大小。 |
| <a href="#wbuffer_t_wbuffer_has_room">wbuffer\_has\_room</a> | 是否还有空间。 |
| <a href="#wbuffer_t_wbuffer_init">wbuffer\_init</a> | 初始wbuffer对象。 |
| <a href="#wbuffer_t_wbuffer_init_extendable">wbuffer\_init\_extendable</a> | 初始wbuffer对象，容量不够时是否支持自动扩展，使用完成后需要调用wbuffer\_deinit释放资源。 |
| <a href="#wbuffer_t_wbuffer_skip">wbuffer\_skip</a> | 跳过指定的长度。 |
| <a href="#wbuffer_t_wbuffer_write_binary">wbuffer\_write\_binary</a> | 写入指定长度的二进制数据。 |
| <a href="#wbuffer_t_wbuffer_write_float">wbuffer\_write\_float</a> | 写入float数据。 |
| <a href="#wbuffer_t_wbuffer_write_string">wbuffer\_write\_string</a> | 写入字符串。 |
| <a href="#wbuffer_t_wbuffer_write_uint16">wbuffer\_write\_uint16</a> | 写入uint16数据。 |
| <a href="#wbuffer_t_wbuffer_write_uint32">wbuffer\_write\_uint32</a> | 写入uint32数据。 |
| <a href="#wbuffer_t_wbuffer_write_uint8">wbuffer\_write\_uint8</a> | 写入uint8数据。 |
### 属性
<p id="wbuffer_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#wbuffer_t_capacity">capacity</a> | uint32\_t | 数据缓冲区最大容量。 |
| <a href="#wbuffer_t_cursor">cursor</a> | uint32\_t | 当前写入位置。 |
| <a href="#wbuffer_t_data">data</a> | uint8\_t* | 数据缓冲区。用于保存写入的数据。 |
| <a href="#wbuffer_t_extendable">extendable</a> | bool\_t | 容量不够时是否支持自动扩展。 |
#### wbuffer\_deinit 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_deinit">释放资源。

* 函数原型：

```
ret_t wbuffer_deinit (wbuffer_t* wbuffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
#### wbuffer\_extend\_capacity 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_extend_capacity">扩展缓冲区至指定的大小。

* 函数原型：

```
ret_t wbuffer_extend_capacity (wbuffer_t* wbuffer, uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| capacity | uint32\_t | 缓冲区的容量。 |
#### wbuffer\_has\_room 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_has_room">是否还有空间。

* 函数原型：

```
bool_t wbuffer_has_room (wbuffer_t* wbuffer, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回FALSE表示无，否则表示有。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| size | uint32\_t | 空间大小。 |
#### wbuffer\_init 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_init">初始wbuffer对象。

* 函数原型：

```
wbuffer_t* wbuffer_init (wbuffer_t* wbuffer, uint8_t* data, uint16_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wbuffer\_t* | wbuffer对象本身。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| data | uint8\_t* | 缓冲区。 |
| capacity | uint16\_t | 缓冲区的容量。 |
#### wbuffer\_init\_extendable 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_init_extendable">初始wbuffer对象，容量不够时是否支持自动扩展，使用完成后需要调用wbuffer\_deinit释放资源。

* 函数原型：

```
wbuffer_t* wbuffer_init_extendable (wbuffer_t* wbuffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wbuffer\_t* | wbuffer对象本身。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
#### wbuffer\_skip 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_skip">跳过指定的长度。

* 函数原型：

```
ret_t wbuffer_skip (wbuffer_t* wbuffer, int32_t delta);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| delta | int32\_t | 跳过的偏移量，正数往前负数往回跳。 |
#### wbuffer\_write\_binary 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_write_binary">写入指定长度的二进制数据。

* 函数原型：

```
ret_t wbuffer_write_binary (wbuffer_t* wbuffer, void* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| data | void* | 要写入的数据。 |
| size | uint32\_t | 要写入的数据长度。 |
#### wbuffer\_write\_float 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_write_float">写入float数据。

* 函数原型：

```
ret_t wbuffer_write_float (wbuffer_t* wbuffer, float_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| value | float\_t | 要写入的数据。 |
#### wbuffer\_write\_string 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_write_string">写入字符串。

* 函数原型：

```
ret_t wbuffer_write_string (wbuffer_t* wbuffer, char* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| data | char* | 要写入的字符串。 |
#### wbuffer\_write\_uint16 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_write_uint16">写入uint16数据。

* 函数原型：

```
ret_t wbuffer_write_uint16 (wbuffer_t* wbuffer, uint16_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| value | uint16\_t | 要写入的数据。 |
#### wbuffer\_write\_uint32 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_write_uint32">写入uint32数据。

* 函数原型：

```
ret_t wbuffer_write_uint32 (wbuffer_t* wbuffer, uint32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| value | uint32\_t | 要写入的数据。 |
#### wbuffer\_write\_uint8 函数
-----------------------

* 函数功能：

> <p id="wbuffer_t_wbuffer_write_uint8">写入uint8数据。

* 函数原型：

```
ret_t wbuffer_write_uint8 (wbuffer_t* wbuffer, uint8_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| value | uint8\_t | 要写入的数据。 |
#### capacity 属性
-----------------------
> <p id="wbuffer_t_capacity">数据缓冲区最大容量。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### cursor 属性
-----------------------
> <p id="wbuffer_t_cursor">当前写入位置。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### data 属性
-----------------------
> <p id="wbuffer_t_data">数据缓冲区。用于保存写入的数据。

* 类型：uint8\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### extendable 属性
-----------------------
> <p id="wbuffer_t_extendable">容量不够时是否支持自动扩展。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
