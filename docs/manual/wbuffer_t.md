## wbuffer\_t
### 概述
 write buffer，用于数据打包。

### 函数
<p id="wbuffer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#wbuffer_t_wbuffer_init">wbuffer\_init</a> | 初始wbuffer对象。 |
| <a href="#wbuffer_t_wbuffer_skip">wbuffer\_skip</a> | 跳过指定的长度。 |
| <a href="#wbuffer_t_wbuffer_write_binary">wbuffer\_write\_binary</a> | 写入指定长度的二进制数据。 |
| <a href="#wbuffer_t_wbuffer_write_float">wbuffer\_write\_float</a> | 写入float数据。 |
| <a href="#wbuffer_t_wbuffer_write_string">wbuffer\_write\_string</a> | 写入字符串。 |
| <a href="#wbuffer_t_wbuffer_write_uint16">wbuffer\_write\_uint16</a> | 写入uint16数据。 |
| <a href="#wbuffer_t_wbuffer_write_uint32">wbuffer\_write\_uint32</a> | 写入uint32数据。 |
| <a href="#wbuffer_t_wbuffer_write_uint8">wbuffer\_write\_uint8</a> | 写入uint8数据。 |
### 属性
<p id="wbuffer_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#wbuffer_t_capacity">capacity</a> | uint32_t | 缓存区最大容量。 |
| <a href="#wbuffer_t_cursor">cursor</a> | uint32_t | 当前写入位置。 |
| <a href="#wbuffer_t_data">data</a> | uint8_t* | 缓存区。 |
### 事件
<p id="wbuffer_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### wbuffer\_init 函数
* 函数原型：

```
wbuffer_t* wbuffer_init (wbuffer_t* wbuffer, uint8_t* data, uint16_t capacity);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wbuffer\_t* | wbuffer对象本身。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| data | uint8\_t* | 缓冲区。 |
| capacity | uint16\_t | 缓冲区的容量。 |
* 函数功能：

> <p id="wbuffer_t_wbuffer_init"> 初始wbuffer对象。




#### wbuffer\_skip 函数
* 函数原型：

```
ret_t wbuffer_skip (wbuffer_t* wbuffer, int32_t offset);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| offset | int32\_t | 跳过的偏移量，正数往前负数往回跳。 |
* 函数功能：

> <p id="wbuffer_t_wbuffer_skip"> 跳过指定的长度。




#### wbuffer\_write\_binary 函数
* 函数原型：

```
ret_t wbuffer_write_binary (wbuffer_t* wbuffer, void* data, uint32_t size);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| data | void* | 写入的数据。 |
| size | uint32\_t | 写入的数据长度。 |
* 函数功能：

> <p id="wbuffer_t_wbuffer_write_binary"> 写入指定长度的二进制数据。




#### wbuffer\_write\_float 函数
* 函数原型：

```
ret_t wbuffer_write_float (wbuffer_t* wbuffer, float_t value);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| value | float\_t | 写入的数据。 |
* 函数功能：

> <p id="wbuffer_t_wbuffer_write_float"> 写入float数据。




#### wbuffer\_write\_string 函数
* 函数原型：

```
ret_t wbuffer_write_string (wbuffer_t* wbuffer, char* data);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| data | char* | 写入的字符串。 |
* 函数功能：

> <p id="wbuffer_t_wbuffer_write_string"> 写入字符串。




#### wbuffer\_write\_uint16 函数
* 函数原型：

```
ret_t wbuffer_write_uint16 (wbuffer_t* wbuffer, uint16_t value);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| value | uint16\_t | 写入的数据。 |
* 函数功能：

> <p id="wbuffer_t_wbuffer_write_uint16"> 写入uint16数据。




#### wbuffer\_write\_uint32 函数
* 函数原型：

```
ret_t wbuffer_write_uint32 (wbuffer_t* wbuffer, uint32_t value);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| value | uint32\_t | 写入的数据。 |
* 函数功能：

> <p id="wbuffer_t_wbuffer_write_uint32"> 写入uint32数据。




#### wbuffer\_write\_uint8 函数
* 函数原型：

```
ret_t wbuffer_write_uint8 (wbuffer_t* wbuffer, uint8_t value);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| wbuffer | wbuffer\_t* | wbuffer对象。 |
| value | uint8\_t | 写入的数据。 |
* 函数功能：

> <p id="wbuffer_t_wbuffer_write_uint8"> 写入uint8数据。




#### capacity 属性
-----------------------
> <p id="wbuffer_t_capacity"> 缓存区最大容量。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 否 |
| 可脚本化   | 否 |
| 可在IDE中设置 | 否 |
| 可在XML中设置 | 否 |
| 支通过widget_get_prop读取 | 否 |
| 支通过widget_set_prop修改 | 否 |
#### cursor 属性
-----------------------
> <p id="wbuffer_t_cursor"> 当前写入位置。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 否 |
| 可脚本化   | 否 |
| 可在IDE中设置 | 否 |
| 可在XML中设置 | 否 |
| 支通过widget_get_prop读取 | 否 |
| 支通过widget_set_prop修改 | 否 |
#### data 属性
-----------------------
> <p id="wbuffer_t_data"> 缓存区。



* 类型：uint8\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可持久化   | 否 |
| 可脚本化   | 否 |
| 可在IDE中设置 | 否 |
| 可在XML中设置 | 否 |
| 支通过widget_get_prop读取 | 否 |
| 支通过widget_set_prop修改 | 否 |
