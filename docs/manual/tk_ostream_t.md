## tk\_ostream\_t
### 概述
![image](images/tk_ostream_t_0.png)

输出流的接口。
----------------------------------
### 函数
<p id="tk_ostream_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_ostream_t_tk_ostream_flush">tk\_ostream\_flush</a> | 刷新数据。 |
| <a href="#tk_ostream_t_tk_ostream_printf">tk\_ostream\_printf</a> | 写入格式化字符串。 |
| <a href="#tk_ostream_t_tk_ostream_seek">tk\_ostream\_seek</a> | 设置偏移量。 |
| <a href="#tk_ostream_t_tk_ostream_tell">tk\_ostream\_tell</a> | 获取当前读取位置。 |
| <a href="#tk_ostream_t_tk_ostream_unref">tk\_ostream\_unref</a> | 引用计数减1。引用计数为0时，销毁对象。 |
| <a href="#tk_ostream_t_tk_ostream_write">tk\_ostream\_write</a> | 写入数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_byte">tk\_ostream\_write\_byte</a> | 写入一个字节的数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_double">tk\_ostream\_write\_double</a> | 写入double数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_float">tk\_ostream\_write\_float</a> | 写入float数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_int16">tk\_ostream\_write\_int16</a> | 写入int16数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_int32">tk\_ostream\_write\_int32</a> | 写入int32数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_int64">tk\_ostream\_write\_int64</a> | 写入int64数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_int8">tk\_ostream\_write\_int8</a> | 写入int8数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_len">tk\_ostream\_write\_len</a> | 写入指定长度的数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_str">tk\_ostream\_write\_str</a> | 写入字符串。 |
| <a href="#tk_ostream_t_tk_ostream_write_uint16">tk\_ostream\_write\_uint16</a> | 写入uint16数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_uint32">tk\_ostream\_write\_uint32</a> | 写入uint32数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_uint64">tk\_ostream\_write\_uint64</a> | 写入uint64数据。 |
| <a href="#tk_ostream_t_tk_ostream_write_uint8">tk\_ostream\_write\_uint8</a> | 写入uint8数据。 |
#### tk\_ostream\_flush 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_flush">刷新数据。

* 函数原型：

```
ret_t tk_ostream_flush (tk_ostream_t* stream);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
#### tk\_ostream\_printf 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_printf">写入格式化字符串。
> 长度不超过1024。

* 函数原型：

```
ret_t tk_ostream_printf (tk_ostream_t* stream, const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| format | const char* | 格式化字符串。 |
#### tk\_ostream\_seek 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_seek">设置偏移量。

* 函数原型：

```
ret_t tk_ostream_seek (tk_ostream_t* stream, uint32_t offset);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| offset | uint32\_t | 偏移量。 |
#### tk\_ostream\_tell 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_tell">获取当前读取位置。

* 函数原型：

```
int32_t tk_ostream_tell (tk_ostream_t* stream);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回负数表示失败，否则返回当前读取位置。 |
| stream | tk\_ostream\_t* | ostream对象。 |
#### tk\_ostream\_unref 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_unref">引用计数减1。引用计数为0时，销毁对象。

* 函数原型：

```
ret_t tk_ostream_unref (tk_ostream_t* stream);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
#### tk\_ostream\_write 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write">写入数据。

* 函数原型：

```
int32_t tk_ostream_write (tk_ostream_t* stream, const void* buff, uint32_t max_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回负数表示写入失败，否则返回实际写入数据的长度。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| buff | const void* | 返回数据的缓冲区。 |
| max\_size | uint32\_t | 缓冲区的大小。 |
#### tk\_ostream\_write\_byte 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_byte">写入一个字节的数据。

* 函数原型：

```
ret_t tk_ostream_write_byte (tk_ostream_t* stream, uint8_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | uint8\_t | 数据。 |
#### tk\_ostream\_write\_double 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_double">写入double数据。

* 函数原型：

```
ret_t tk_ostream_write_double (tk_ostream_t* stream, double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | double | 数据。 |
#### tk\_ostream\_write\_float 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_float">写入float数据。

* 函数原型：

```
ret_t tk_ostream_write_float (tk_ostream_t* stream, float value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | float | 数据。 |
#### tk\_ostream\_write\_int16 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_int16">写入int16数据。

* 函数原型：

```
ret_t tk_ostream_write_int16 (tk_ostream_t* stream, int16_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | int16\_t | 数据。 |
#### tk\_ostream\_write\_int32 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_int32">写入int32数据。

* 函数原型：

```
ret_t tk_ostream_write_int32 (tk_ostream_t* stream, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | int32\_t | 数据。 |
#### tk\_ostream\_write\_int64 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_int64">写入int64数据。

* 函数原型：

```
ret_t tk_ostream_write_int64 (tk_ostream_t* stream, int64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | int64\_t | 数据。 |
#### tk\_ostream\_write\_int8 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_int8">写入int8数据。

* 函数原型：

```
ret_t tk_ostream_write_int8 (tk_ostream_t* stream, int8_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | int8\_t | 数据。 |
#### tk\_ostream\_write\_len 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_len">写入指定长度的数据。

* 函数原型：

```
int32_t tk_ostream_write_len (tk_ostream_t* stream, const void* buff, uint32_t max_size, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回负数表示写入失败，否则返回实际写入数据的长度。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| buff | const void* | 数据的缓冲区。 |
| max\_size | uint32\_t | 缓冲区的大小。 |
| timeout\_ms | uint32\_t | timeout. |
#### tk\_ostream\_write\_str 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_str">写入字符串。

* 函数原型：

```
ret_t tk_ostream_write_str (tk_ostream_t* stream, const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| str | const char* | 字符串。 |
#### tk\_ostream\_write\_uint16 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_uint16">写入uint16数据。

* 函数原型：

```
ret_t tk_ostream_write_uint16 (tk_ostream_t* stream, uint16_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | uint16\_t | 数据。 |
#### tk\_ostream\_write\_uint32 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_uint32">写入uint32数据。

* 函数原型：

```
ret_t tk_ostream_write_uint32 (tk_ostream_t* stream, uint32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | uint32\_t | 数据。 |
#### tk\_ostream\_write\_uint64 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_uint64">写入uint64数据。

* 函数原型：

```
ret_t tk_ostream_write_uint64 (tk_ostream_t* stream, uint64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | uint64\_t | 数据。 |
#### tk\_ostream\_write\_uint8 函数
-----------------------

* 函数功能：

> <p id="tk_ostream_t_tk_ostream_write_uint8">写入uint8数据。

* 函数原型：

```
ret_t tk_ostream_write_uint8 (tk_ostream_t* stream, uint8_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_ostream\_t* | ostream对象。 |
| value | uint8\_t | 数据。 |
