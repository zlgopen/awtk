## utf8\_t
### 概述
wchar_t和char类型转换接口。

示例：

```c
const char* str = "hello";
const wchar_t* wstr = L"hello";
char res_str[128];
wchar_t res_wstr[128];

tk_utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr));
tk_utf8_from_utf16(wstr, res_str, ARRAY_SIZE(res_str));
```
----------------------------------
### 函数
<p id="utf8_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#utf8_t_tk_utf8_dup_utf16">tk\_utf8\_dup\_utf16</a> | 把ucs字符串转成UTF8字符串。 |
| <a href="#utf8_t_tk_utf8_from_utf16">tk\_utf8\_from\_utf16</a> | 把ucs字符串转成UTF8字符串。 |
| <a href="#utf8_t_tk_utf8_from_utf16_ex">tk\_utf8\_from\_utf16\_ex</a> | 把ucs字符串转成UTF8字符串。 |
| <a href="#utf8_t_tk_utf8_get_bytes_of_leading">tk\_utf8\_get\_bytes\_of\_leading</a> | 获取第一个字节为c的字符的字节数。 |
| <a href="#utf8_t_tk_utf8_to_utf16">tk\_utf8\_to\_utf16</a> | 将char类型转换为wchar_t类型。 |
| <a href="#utf8_t_tk_utf8_to_utf16_ex">tk\_utf8\_to\_utf16\_ex</a> | 将char类型转换为wchar_t类型。 |
| <a href="#utf8_t_tk_utf8_trim_invalid_char">tk\_utf8\_trim\_invalid\_char</a> | 如果字符串最后一个字符串是无效的，删掉该无效字符。 |
#### tk\_utf8\_dup\_utf16 函数
-----------------------

* 函数功能：

> <p id="utf8_t_tk_utf8_dup_utf16">把ucs字符串转成UTF8字符串。
> 由调用者释放返回的指针。

* 函数原型：

```
char* tk_utf8_dup_utf16 (const wchar_t* in, int32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回UTF8字符串。 |
| in | const wchar\_t* | 输入字符串。 |
| size | int32\_t | 字符串长度。 |
#### tk\_utf8\_from\_utf16 函数
-----------------------

* 函数功能：

> <p id="utf8_t_tk_utf8_from_utf16">把ucs字符串转成UTF8字符串。

* 函数原型：

```
char* tk_utf8_from_utf16 (const wchar_t* in, char* out, uint32_t out_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回UTF8字符串。 |
| in | const wchar\_t* | 输入字符串。 |
| out | char* | 输出字符串。 |
| out\_size | uint32\_t | 输出字符串的最大长度。 |
#### tk\_utf8\_from\_utf16\_ex 函数
-----------------------

* 函数功能：

> <p id="utf8_t_tk_utf8_from_utf16_ex">把ucs字符串转成UTF8字符串。

* 函数原型：

```
char* tk_utf8_from_utf16_ex (const wchar_t* in, uint32_t in_size, char* out, uint32_t out_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回UTF8字符串。 |
| in | const wchar\_t* | 输入字符串。 |
| in\_size | uint32\_t | 输入字符串的长度。 |
| out | char* | 输出字符串。 |
| out\_size | uint32\_t | 输出字符串的最大长度。 |
#### tk\_utf8\_get\_bytes\_of\_leading 函数
-----------------------

* 函数功能：

> <p id="utf8_t_tk_utf8_get_bytes_of_leading">获取第一个字节为c的字符的字节数。

* 函数原型：

```
uint32_t tk_utf8_get_bytes_of_leading (uint8_t c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回字符的字节数。 |
| c | uint8\_t | 字符的第一个字节。 |
#### tk\_utf8\_to\_utf16 函数
-----------------------

* 函数功能：

> <p id="utf8_t_tk_utf8_to_utf16">将char类型转换为wchar_t类型。

* 函数原型：

```
wchar_t* tk_utf8_to_utf16 (const char* str, wchar_t* out, uint32_t out_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wchar\_t* | 值。 |
| str | const char* | str。 |
| out | wchar\_t* | 返回结果缓冲区。 |
| out\_size | uint32\_t | 缓冲区大小。 |
#### tk\_utf8\_to\_utf16\_ex 函数
-----------------------

* 函数功能：

> <p id="utf8_t_tk_utf8_to_utf16_ex">将char类型转换为wchar_t类型。

* 函数原型：

```
wchar_t* tk_utf8_to_utf16_ex (const char* str, uint32_t size, wchar_t* out, uint32_t out_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wchar\_t* | 值。 |
| str | const char* | str。 |
| size | uint32\_t | 缓冲区大小。 |
| out | wchar\_t* | 返回结果缓冲区。 |
| out\_size | uint32\_t | 缓冲区大小。 |
#### tk\_utf8\_trim\_invalid\_char 函数
-----------------------

* 函数功能：

> <p id="utf8_t_tk_utf8_trim_invalid_char">如果字符串最后一个字符串是无效的，删掉该无效字符。

> 对于用strncpy/snprintf等生成字符串时，如果目标字符串内存大小不够，
> 可能最后一个字符被从中间截断，导致该字符是无效的，这可能会触发assert，
> 本函数可以用来删除最后一个无效字符。

* 函数原型：

```
char* tk_utf8_trim_invalid_char (char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回UTF8字符串。 |
| str | char* | 输入字符串。 |
