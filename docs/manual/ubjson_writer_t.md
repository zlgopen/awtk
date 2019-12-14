## ubjson\_writer\_t
### 概述
ubjson writer
----------------------------------
### 函数
<p id="ubjson_writer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#ubjson_writer_t_ubjson_writer_init">ubjson\_writer\_init</a> | 初始化。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_array_begin">ubjson\_writer\_write\_array\_begin</a> | 写入数组开始。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_array_end">ubjson\_writer\_write\_array\_end</a> | 写入数组结束。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_char">ubjson\_writer\_write\_char</a> | 写入char。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_data">ubjson\_writer\_write\_data</a> | 写入二进制数据。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_false">ubjson\_writer\_write\_false</a> | 写入false。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_float32">ubjson\_writer\_write\_float32</a> | 写入float。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_float64">ubjson\_writer\_write\_float64</a> | 写入double。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_int">ubjson\_writer\_write\_int</a> | 写入int。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_int16">ubjson\_writer\_write\_int16</a> | 写入int16。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_int32">ubjson\_writer\_write\_int32</a> | 写入int32。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_int64">ubjson\_writer\_write\_int64</a> | 写入int64。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_int8">ubjson\_writer\_write\_int8</a> | 写入int8。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_key">ubjson\_writer\_write\_key</a> | 写入key。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_kv_bool">ubjson\_writer\_write\_kv\_bool</a> | 写入key和bool的值。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_kv_double">ubjson\_writer\_write\_kv\_double</a> | 写入key和double的值。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_kv_float">ubjson\_writer\_write\_kv\_float</a> | 写入key和float的值。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_kv_int">ubjson\_writer\_write\_kv\_int</a> | 写入key和int的值。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_kv_int64">ubjson\_writer\_write\_kv\_int64</a> | 写入key和int64_t的值。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_kv_object">ubjson\_writer\_write\_kv\_object</a> | 写key和对象。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_kv_object_begin">ubjson\_writer\_write\_kv\_object\_begin</a> | 写key和对象的开始。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_kv_str">ubjson\_writer\_write\_kv\_str</a> | 写入key和str的值。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_kv_str_len">ubjson\_writer\_write\_kv\_str\_len</a> | 写入key和str的值。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_kv_value">ubjson\_writer\_write\_kv\_value</a> | 写入key和value_t的值。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_noop">ubjson\_writer\_write\_noop</a> | 写入noop。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_null">ubjson\_writer\_write\_null</a> | 写入null。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_object">ubjson\_writer\_write\_object</a> | 写入obj。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_object_begin">ubjson\_writer\_write\_object\_begin</a> | 写入对象开始。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_object_end">ubjson\_writer\_write\_object\_end</a> | 写入对象结束。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_str">ubjson\_writer\_write\_str</a> | 写入str。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_str_len">ubjson\_writer\_write\_str\_len</a> | 写入str。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_true">ubjson\_writer\_write\_true</a> | 写入true。 |
| <a href="#ubjson_writer_t_ubjson_writer_write_uint8">ubjson\_writer\_write\_uint8</a> | 写入uint8。 |
#### ubjson\_writer\_init 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_init">初始化。

* 函数原型：

```
ubjson_writer_t* ubjson_writer_init (ubjson_writer_t* writer, ubjson_write_callback_t write, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ubjson\_writer\_t* | 返回writer对象。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| write | ubjson\_write\_callback\_t | 真正写入数据的函数。 |
| ctx | void* | write函数的上下文。 |
#### ubjson\_writer\_write\_array\_begin 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_array_begin">写入数组开始。
> 即： [

* 函数原型：

```
ret_t ubjson_writer_write_array_begin (ubjson_writer_t* writer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
#### ubjson\_writer\_write\_array\_end 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_array_end">写入数组结束。
> 即： ]

* 函数原型：

```
ret_t ubjson_writer_write_array_end (ubjson_writer_t* writer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
#### ubjson\_writer\_write\_char 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_char">写入char。

* 函数原型：

```
ret_t ubjson_writer_write_char (ubjson_writer_t* writer, char value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | char | 要写入的值。 |
#### ubjson\_writer\_write\_data 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_data">写入二进制数据。

* 函数原型：

```
ret_t ubjson_writer_write_data (ubjson_writer_t* writer, const void* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| data | const void* | 数据。 |
| size | uint32\_t | 数据长度。 |
#### ubjson\_writer\_write\_false 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_false">写入false。

* 函数原型：

```
ret_t ubjson_writer_write_false (ubjson_writer_t* writer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
#### ubjson\_writer\_write\_float32 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_float32">写入float。

* 函数原型：

```
ret_t ubjson_writer_write_float32 (ubjson_writer_t* writer, float value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | float | 要写入的值。 |
#### ubjson\_writer\_write\_float64 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_float64">写入double。

* 函数原型：

```
ret_t ubjson_writer_write_float64 (ubjson_writer_t* writer, double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | double | 要写入的值。 |
#### ubjson\_writer\_write\_int 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_int">写入int。

* 函数原型：

```
ret_t ubjson_writer_write_int (ubjson_writer_t* writer, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | int32\_t | 要写入的值。 |
#### ubjson\_writer\_write\_int16 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_int16">写入int16。

* 函数原型：

```
ret_t ubjson_writer_write_int16 (ubjson_writer_t* writer, int16_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | int16\_t | 要写入的值。 |
#### ubjson\_writer\_write\_int32 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_int32">写入int32。

* 函数原型：

```
ret_t ubjson_writer_write_int32 (ubjson_writer_t* writer, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | int32\_t | 要写入的值。 |
#### ubjson\_writer\_write\_int64 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_int64">写入int64。

* 函数原型：

```
ret_t ubjson_writer_write_int64 (ubjson_writer_t* writer, int64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | int64\_t | 要写入的值。 |
#### ubjson\_writer\_write\_int8 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_int8">写入int8。

* 函数原型：

```
ret_t ubjson_writer_write_int8 (ubjson_writer_t* writer, int8_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | int8\_t | 要写入的值。 |
#### ubjson\_writer\_write\_key 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_key">写入key。

* 函数原型：

```
ret_t ubjson_writer_write_key (ubjson_writer_t* writer, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | const char* | key的名称。 |
#### ubjson\_writer\_write\_kv\_bool 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_kv_bool">写入key和bool的值。

> 即："key": value

* 函数原型：

```
ret_t ubjson_writer_write_kv_bool (ubjson_writer_t* writer, const char* key, bool_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| key | const char* | 键名。 |
| value | bool\_t | 值。 |
#### ubjson\_writer\_write\_kv\_double 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_kv_double">写入key和double的值。

> 即："key": value

* 函数原型：

```
ret_t ubjson_writer_write_kv_double (ubjson_writer_t* writer, const char* key, double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| key | const char* | 键名。 |
| value | double | 值。 |
#### ubjson\_writer\_write\_kv\_float 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_kv_float">写入key和float的值。

> 即："key": value

* 函数原型：

```
ret_t ubjson_writer_write_kv_float (ubjson_writer_t* writer, const char* key, float value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| key | const char* | 键名。 |
| value | float | 值。 |
#### ubjson\_writer\_write\_kv\_int 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_kv_int">写入key和int的值。

> 即："key": value

* 函数原型：

```
ret_t ubjson_writer_write_kv_int (ubjson_writer_t* writer, const char* key, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| key | const char* | 键名。 |
| value | int32\_t | 值。 |
#### ubjson\_writer\_write\_kv\_int64 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_kv_int64">写入key和int64_t的值。

> 即："key": value

* 函数原型：

```
ret_t ubjson_writer_write_kv_int64 (ubjson_writer_t* writer, const char* key, int64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| key | const char* | 键名。 |
| value | int64\_t | 值。 |
#### ubjson\_writer\_write\_kv\_object 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_kv_object">写key和对象。

> 即："key": { object }

* 函数原型：

```
ret_t ubjson_writer_write_kv_object (ubjson_writer_t* writer, const char* key, object_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| key | const char* | 键名。 |
| value | object\_t* | 对象。 |
#### ubjson\_writer\_write\_kv\_object\_begin 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_kv_object_begin">写key和对象的开始。

> 即："key": {

* 函数原型：

```
ret_t ubjson_writer_write_kv_object_begin (ubjson_writer_t* writer, const char* key);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| key | const char* | 键值。 |
#### ubjson\_writer\_write\_kv\_str 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_kv_str">写入key和str的值。

> 即："key": value

* 函数原型：

```
ret_t ubjson_writer_write_kv_str (ubjson_writer_t* writer, const char* key, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| key | const char* | 键名。 |
| value | const char* | 字符串。 |
#### ubjson\_writer\_write\_kv\_str\_len 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_kv_str_len">写入key和str的值。

> 即："key": value

* 函数原型：

```
ret_t ubjson_writer_write_kv_str_len (ubjson_writer_t* writer, const char* key, const char* value, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| key | const char* | 键名。 |
| value | const char* | 字符串。 |
| len | uint32\_t | 字符串的长度。 |
#### ubjson\_writer\_write\_kv\_value 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_kv_value">写入key和value_t的值。

> 即："key": value

* 函数原型：

```
ret_t ubjson_writer_write_kv_value (ubjson_writer_t* writer, const char* key, value_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| key | const char* | 键名。 |
| value | value\_t* | 值。 |
#### ubjson\_writer\_write\_noop 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_noop">写入noop。

* 函数原型：

```
ret_t ubjson_writer_write_noop (ubjson_writer_t* writer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
#### ubjson\_writer\_write\_null 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_null">写入null。

* 函数原型：

```
ret_t ubjson_writer_write_null (ubjson_writer_t* writer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
#### ubjson\_writer\_write\_object 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_object">写入obj。

* 函数原型：

```
ret_t ubjson_writer_write_object (ubjson_writer_t* writer, object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| obj | object\_t* | 要写入的对象。 |
#### ubjson\_writer\_write\_object\_begin 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_object_begin">写入对象开始。
> 即： {

* 函数原型：

```
ret_t ubjson_writer_write_object_begin (ubjson_writer_t* writer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
#### ubjson\_writer\_write\_object\_end 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_object_end">写入对象结束。
> 即：}

* 函数原型：

```
ret_t ubjson_writer_write_object_end (ubjson_writer_t* writer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
#### ubjson\_writer\_write\_str 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_str">写入str。

* 函数原型：

```
ret_t ubjson_writer_write_str (ubjson_writer_t* writer, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | const char* | 要写入的字符串。 |
#### ubjson\_writer\_write\_str\_len 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_str_len">写入str。

* 函数原型：

```
ret_t ubjson_writer_write_str_len (ubjson_writer_t* writer, const char* value, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | const char* | 要写入的字符串。 |
| len | uint32\_t | 字符串的长度。 |
#### ubjson\_writer\_write\_true 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_true">写入true。

* 函数原型：

```
ret_t ubjson_writer_write_true (ubjson_writer_t* writer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
#### ubjson\_writer\_write\_uint8 函数
-----------------------

* 函数功能：

> <p id="ubjson_writer_t_ubjson_writer_write_uint8">写入uint8。

* 函数原型：

```
ret_t ubjson_writer_write_uint8 (ubjson_writer_t* writer, uint8_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| writer | ubjson\_writer\_t* | writer对象。 |
| value | uint8\_t | 要写入的值。 |
