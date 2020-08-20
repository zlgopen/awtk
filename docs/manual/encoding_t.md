## encoding\_t
### 概述
encoding conversion

示例：

```c
```
----------------------------------
### 函数
<p id="encoding_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#encoding_t_encoding_convert">encoding\_convert</a> | 字符集转换。 |
| <a href="#encoding_t_encoding_gbk_to_utf8">encoding\_gbk\_to\_utf8</a> | 将gbk转换成utf8编码。 |
| <a href="#encoding_t_encoding_utf8_to_gbk">encoding\_utf8\_to\_gbk</a> | 将utf8转换成gbk编码。 |
#### encoding\_convert 函数
-----------------------

* 函数功能：

> <p id="encoding_t_encoding_convert">字符集转换。

* 函数原型：

```
ret_t encoding_convert (encoding_name_t from, const char* from_str, uint32_t from_size, encoding_name_t to, const char* , uint32_t to_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| from | encoding\_name\_t | 源字符串的编码名称。 |
| from\_str | const char* | 源字符串。 |
| from\_size | uint32\_t | 源字符串的长度。 |
| to | encoding\_name\_t | 目标字符串的编码名称。 |
|  | const char* | 。 |
| to\_size | uint32\_t | 目标字符串的最大长度。 |
#### encoding\_gbk\_to\_utf8 函数
-----------------------

* 函数功能：

> <p id="encoding_t_encoding_gbk_to_utf8">将gbk转换成utf8编码。

* 函数原型：

```
ret_t encoding_gbk_to_utf8 (const char* from_str, uint32_t from_size, const char* , uint32_t to_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| from\_str | const char* | 源字符串(gbk)。 |
| from\_size | uint32\_t | 源字符串的长度。 |
|  | const char* | 。 |
| to\_size | uint32\_t | 目标字符串的最大长度。 |
#### encoding\_utf8\_to\_gbk 函数
-----------------------

* 函数功能：

> <p id="encoding_t_encoding_utf8_to_gbk">将utf8转换成gbk编码。

* 函数原型：

```
ret_t encoding_utf8_to_gbk (const char* from_str, uint32_t from_size, const char* to_str, uint32_t to_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| from\_str | const char* | 源字符串(utf8)。 |
| from\_size | uint32\_t | 源字符串的长度。 |
| to\_str | const char* | 目标字符串(gbk)。 |
| to\_size | uint32\_t | 目标字符串的最大长度。 |
