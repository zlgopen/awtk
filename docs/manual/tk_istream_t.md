## tk\_istream\_t
### 概述
![image](images/tk_istream_t_0.png)

输入流的接口。
----------------------------------
### 函数
<p id="tk_istream_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_istream_t_tk_istream_eos">tk\_istream\_eos</a> | 判断数据是否读完。 |
| <a href="#tk_istream_t_tk_istream_flush">tk\_istream\_flush</a> | 刷新数据。 |
| <a href="#tk_istream_t_tk_istream_read">tk\_istream\_read</a> | 读取数据。 |
| <a href="#tk_istream_t_tk_istream_read_double">tk\_istream\_read\_double</a> | 写入double数据。 |
| <a href="#tk_istream_t_tk_istream_read_float">tk\_istream\_read\_float</a> | 写入float数据。 |
| <a href="#tk_istream_t_tk_istream_read_int16">tk\_istream\_read\_int16</a> | 写入int16数据。 |
| <a href="#tk_istream_t_tk_istream_read_int32">tk\_istream\_read\_int32</a> | 写入int32数据。 |
| <a href="#tk_istream_t_tk_istream_read_int64">tk\_istream\_read\_int64</a> | 写入int64数据。 |
| <a href="#tk_istream_t_tk_istream_read_int8">tk\_istream\_read\_int8</a> | 写入int8数据。 |
| <a href="#tk_istream_t_tk_istream_read_len">tk\_istream\_read\_len</a> | 读取指定长度的数据。 |
| <a href="#tk_istream_t_tk_istream_read_line">tk\_istream\_read\_line</a> | 读取一行数据。 |
| <a href="#tk_istream_t_tk_istream_read_line_str">tk\_istream\_read\_line\_str</a> | 读取一行数据。 |
| <a href="#tk_istream_t_tk_istream_read_uint16">tk\_istream\_read\_uint16</a> | 写入uint16数据。 |
| <a href="#tk_istream_t_tk_istream_read_uint32">tk\_istream\_read\_uint32</a> | 写入uint32数据。 |
| <a href="#tk_istream_t_tk_istream_read_uint64">tk\_istream\_read\_uint64</a> | 写入uint64数据。 |
| <a href="#tk_istream_t_tk_istream_read_uint8">tk\_istream\_read\_uint8</a> | 写入uint8数据。 |
| <a href="#tk_istream_t_tk_istream_seek">tk\_istream\_seek</a> | 设置偏移量。 |
| <a href="#tk_istream_t_tk_istream_tell">tk\_istream\_tell</a> | 获取当前读取位置。 |
| <a href="#tk_istream_t_tk_istream_unref">tk\_istream\_unref</a> | 引用计数减1。引用计数为0时，销毁对象。 |
| <a href="#tk_istream_t_tk_istream_wait_for_data">tk\_istream\_wait\_for\_data</a> | 等待数据。 |
#### tk\_istream\_eos 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_eos">判断数据是否读完。

* 函数原型：

```
bool_t tk_istream_eos (tk_istream_t* stream);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示数据读完，否则表示还有数据。 |
| stream | tk\_istream\_t* | istream对象。 |
#### tk\_istream\_flush 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_flush">刷新数据。

* 函数原型：

```
ret_t tk_istream_flush (tk_istream_t* stream);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
#### tk\_istream\_read 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read">读取数据。

* 函数原型：

```
int32_t tk_istream_read (tk_istream_t* stream, void* buff, uint32_t max_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回负数表示读取失败，否则返回实际读取数据的长度。 |
| stream | tk\_istream\_t* | istream对象。 |
| buff | void* | 返回数据的缓冲区。 |
| max\_size | uint32\_t | 缓冲区的大小。 |
#### tk\_istream\_read\_double 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_double">写入double数据。

* 函数原型：

```
ret_t tk_istream_read_double (tk_istream_t* stream, double* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| value | double* | 数据。 |
#### tk\_istream\_read\_float 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_float">写入float数据。

* 函数原型：

```
ret_t tk_istream_read_float (tk_istream_t* stream, float* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| value | float* | 数据。 |
#### tk\_istream\_read\_int16 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_int16">写入int16数据。

* 函数原型：

```
ret_t tk_istream_read_int16 (tk_istream_t* stream, int16_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| value | int16\_t* | 数据。 |
#### tk\_istream\_read\_int32 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_int32">写入int32数据。

* 函数原型：

```
ret_t tk_istream_read_int32 (tk_istream_t* stream, int32_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| value | int32\_t* | 数据。 |
#### tk\_istream\_read\_int64 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_int64">写入int64数据。

* 函数原型：

```
ret_t tk_istream_read_int64 (tk_istream_t* stream, int64_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| value | int64\_t* | 数据。 |
#### tk\_istream\_read\_int8 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_int8">写入int8数据。

* 函数原型：

```
ret_t tk_istream_read_int8 (tk_istream_t* stream, int8_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| value | int8\_t* | 数据。 |
#### tk\_istream\_read\_len 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_len">读取指定长度的数据。

* 函数原型：

```
int32_t tk_istream_read_len (tk_istream_t* stream, void* buff, uint32_t max_size, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回负数表示读取失败，否则返回实际读取数据的长度。 |
| stream | tk\_istream\_t* | istream对象。 |
| buff | void* | 返回数据的缓冲区。 |
| max\_size | uint32\_t | 缓冲区的大小。 |
| timeout\_ms | uint32\_t | timeout. |
#### tk\_istream\_read\_line 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_line">读取一行数据。

* 函数原型：

```
int32_t tk_istream_read_line (tk_istream_t* stream, void* buff, uint32_t max_size, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回负数表示读取失败，否则返回实际读取数据的长度。 |
| stream | tk\_istream\_t* | istream对象。 |
| buff | void* | 返回数据的缓冲区。 |
| max\_size | uint32\_t | 缓冲区的大小。 |
| timeout\_ms | uint32\_t | timeout. |
#### tk\_istream\_read\_line\_str 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_line_str">读取一行数据。
>istream必须支持随机读写(seek/tell)。

* 函数原型：

```
ret_t tk_istream_read_line_str (tk_istream_t* stream, str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| str | str\_t* | 返回数据的str对象。 |
#### tk\_istream\_read\_uint16 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_uint16">写入uint16数据。

* 函数原型：

```
ret_t tk_istream_read_uint16 (tk_istream_t* stream, uint16_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| value | uint16\_t* | 数据。 |
#### tk\_istream\_read\_uint32 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_uint32">写入uint32数据。

* 函数原型：

```
ret_t tk_istream_read_uint32 (tk_istream_t* stream, uint32_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| value | uint32\_t* | 数据。 |
#### tk\_istream\_read\_uint64 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_uint64">写入uint64数据。

* 函数原型：

```
ret_t tk_istream_read_uint64 (tk_istream_t* stream, uint64_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| value | uint64\_t* | 数据。 |
#### tk\_istream\_read\_uint8 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_read_uint8">写入uint8数据。

* 函数原型：

```
ret_t tk_istream_read_uint8 (tk_istream_t* stream, uint8_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| value | uint8\_t* | 数据。 |
#### tk\_istream\_seek 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_seek">设置偏移量。

* 函数原型：

```
ret_t tk_istream_seek (tk_istream_t* stream, uint32_t offset);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| offset | uint32\_t | 偏移量。 |
#### tk\_istream\_tell 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_tell">获取当前读取位置。

* 函数原型：

```
int32_t tk_istream_tell (tk_istream_t* stream);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回负数表示失败，否则返回当前读取位置。 |
| stream | tk\_istream\_t* | istream对象。 |
#### tk\_istream\_unref 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_unref">引用计数减1。引用计数为0时，销毁对象。

* 函数原型：

```
ret_t tk_istream_unref (tk_istream_t* stream);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
#### tk\_istream\_wait\_for\_data 函数
-----------------------

* 函数功能：

> <p id="tk_istream_t_tk_istream_wait_for_data">等待数据。

* 函数原型：

```
ret_t tk_istream_wait_for_data (tk_istream_t* stream, uint32_t timeout_ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| stream | tk\_istream\_t* | istream对象。 |
| timeout\_ms | uint32\_t | 超时时间。 |
