## rbuffer\_t
### 概述
Read Buffer。用于数据解包。

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
----------------------------------
### 函数
<p id="rbuffer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#rbuffer_t_rbuffer_has_more">rbuffer\_has\_more</a> | 判断是否还有数据可读。 |
| <a href="#rbuffer_t_rbuffer_init">rbuffer\_init</a> | 初始rbuffer对象。 |
| <a href="#rbuffer_t_rbuffer_peek_uint16">rbuffer\_peek\_uint16</a> | 读取uint16数据，但不改变cursor的位置。 |
| <a href="#rbuffer_t_rbuffer_peek_uint32">rbuffer\_peek\_uint32</a> | 读取uint32数据，但不改变cursor的位置。 |
| <a href="#rbuffer_t_rbuffer_peek_uint8">rbuffer\_peek\_uint8</a> | 读取uint8数据，但不改变cursor的位置。 |
| <a href="#rbuffer_t_rbuffer_read_binary">rbuffer\_read\_binary</a> | 读取指定长度的二进制数据。 |
| <a href="#rbuffer_t_rbuffer_read_float">rbuffer\_read\_float</a> | 读取float数据。 |
| <a href="#rbuffer_t_rbuffer_read_string">rbuffer\_read\_string</a> | 读取字符串。 |
| <a href="#rbuffer_t_rbuffer_read_uint16">rbuffer\_read\_uint16</a> | 读取uint16数据。 |
| <a href="#rbuffer_t_rbuffer_read_uint32">rbuffer\_read\_uint32</a> | 读取uint32数据。 |
| <a href="#rbuffer_t_rbuffer_read_uint8">rbuffer\_read\_uint8</a> | 读取uint8数据。 |
| <a href="#rbuffer_t_rbuffer_skip">rbuffer\_skip</a> | 跳过指定的长度。 |
### 属性
<p id="rbuffer_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#rbuffer_t_capacity">capacity</a> | uint32\_t | 缓存区的容量。 |
| <a href="#rbuffer_t_cursor">cursor</a> | uint32\_t | 当前读取位置。 |
| <a href="#rbuffer_t_data">data</a> | uint8\_t* | 数据缓冲区。 |
#### rbuffer\_has\_more 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_has_more">判断是否还有数据可读。

* 函数原型：

```
bool_t rbuffer_has_more (rbuffer_t* rbuffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示还有数据可读，否则表示无数据可读。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
#### rbuffer\_init 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_init">初始rbuffer对象。

* 函数原型：

```
rbuffer_t* rbuffer_init (rbuffer_t* rbuffer, uint8_t* data, uint16_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | rbuffer\_t* | rbuffer对象本身。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| data | uint8\_t* | 缓冲区。 |
| capacity | uint16\_t | 缓冲区的容量。 |
#### rbuffer\_peek\_uint16 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_peek_uint16">读取uint16数据，但不改变cursor的位置。

* 函数原型：

```
ret_t rbuffer_peek_uint16 (rbuffer_t* rbuffer, uint16_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| value | uint16\_t* | 返回读取的数据。 |
#### rbuffer\_peek\_uint32 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_peek_uint32">读取uint32数据，但不改变cursor的位置。

* 函数原型：

```
ret_t rbuffer_peek_uint32 (rbuffer_t* rbuffer, uint32_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| value | uint32\_t* | 读取的数据。 |
#### rbuffer\_peek\_uint8 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_peek_uint8">读取uint8数据，但不改变cursor的位置。

* 函数原型：

```
ret_t rbuffer_peek_uint8 (rbuffer_t* rbuffer, uint8_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| value | uint8\_t* | 返回读取的数据。 |
#### rbuffer\_read\_binary 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_read_binary">读取指定长度的二进制数据。

* 函数原型：

```
ret_t rbuffer_read_binary (rbuffer_t* rbuffer, void* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| data | void* | 返回读取的数据。 |
| size | uint32\_t | 读取的数据长度。 |
#### rbuffer\_read\_float 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_read_float">读取float数据。

* 函数原型：

```
ret_t rbuffer_read_float (rbuffer_t* rbuffer, float_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| value | float\_t* | 返回读取的数据。 |
#### rbuffer\_read\_string 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_read_string">读取字符串。

* 函数原型：

```
ret_t rbuffer_read_string (rbuffer_t* rbuffer, char** str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| str | char** | 返回字符串。 |
#### rbuffer\_read\_uint16 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_read_uint16">读取uint16数据。

* 函数原型：

```
ret_t rbuffer_read_uint16 (rbuffer_t* rbuffer, uint16_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| value | uint16\_t* | 读取的数据。 |
#### rbuffer\_read\_uint32 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_read_uint32">读取uint32数据。

* 函数原型：

```
ret_t rbuffer_read_uint32 (rbuffer_t* rbuffer, uint32_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| value | uint32\_t* | 返回读取的数据。 |
#### rbuffer\_read\_uint8 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_read_uint8">读取uint8数据。

* 函数原型：

```
ret_t rbuffer_read_uint8 (rbuffer_t* rbuffer, uint8_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| value | uint8\_t* | 返回读取的数据。 |
#### rbuffer\_skip 函数
-----------------------

* 函数功能：

> <p id="rbuffer_t_rbuffer_skip">跳过指定的长度。

* 函数原型：

```
ret_t rbuffer_skip (rbuffer_t* rbuffer, int32_t offset);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rbuffer | rbuffer\_t* | rbuffer对象。 |
| offset | int32\_t | 跳过的偏移量，正数往前负数往回跳。 |
#### capacity 属性
-----------------------
> <p id="rbuffer_t_capacity">缓存区的容量。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### cursor 属性
-----------------------
> <p id="rbuffer_t_cursor">当前读取位置。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### data 属性
-----------------------
> <p id="rbuffer_t_data">数据缓冲区。

* 类型：uint8\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
