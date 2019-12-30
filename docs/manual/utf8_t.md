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
| <a href="#utf8_t_tk_utf8_from_utf16">tk\_utf8\_from\_utf16</a> | 把ucs字符串转成UTF8字符串。 |
| <a href="#utf8_t_tk_utf8_from_utf16_ex">tk\_utf8\_from\_utf16\_ex</a> | 把ucs字符串转成UTF8字符串。 |
| <a href="#utf8_t_tk_utf8_to_utf16">tk\_utf8\_to\_utf16</a> | 将char类型转换为wchar_t类型。 |
#### tk\_utf8\_from\_utf16 函数
-----------------------

* 函数功能：

> <p id="utf8_t_tk_utf8_from_utf16">把ucs字符串转成UTF8字符串。

* 函数原型：

```
char* tk_utf8_from_utf16 (const wchar_t* in, const char* out, uint32_t out_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回UTF8字符串。 |
| in | const wchar\_t* | 输入字符串。 |
| out | const char* | 输出字符串。 |
| out\_size | uint32\_t | 输出字符串的最大长度。 |
#### tk\_utf8\_from\_utf16\_ex 函数
-----------------------

* 函数功能：

> <p id="utf8_t_tk_utf8_from_utf16_ex">把ucs字符串转成UTF8字符串。

* 函数原型：

```
char* tk_utf8_from_utf16_ex (const wchar_t* in, uint32_t in_size, const char* out, uint32_t out_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回UTF8字符串。 |
| in | const wchar\_t* | 输入字符串。 |
| in\_size | uint32\_t | 输入字符串的长度。 |
| out | const char* | 输出字符串。 |
| out\_size | uint32\_t | 输出字符串的最大长度。 |
#### tk\_utf8\_to\_utf16 函数
-----------------------

* 函数功能：

> <p id="utf8_t_tk_utf8_to_utf16">将char类型转换为wchar_t类型。

* 函数原型：

```
wchar_t* tk_utf8_to_utf16 (const char* str, const wchar_t* out, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wchar\_t* | 值。 |
| str | const char* | str。 |
| out | const wchar\_t* | 返回结果缓冲区。 |
| size | uint32\_t | 缓冲区大小。 |
