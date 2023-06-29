## str\_t
### 概述
可变长度的UTF8字符串。

示例：

```c
str_t s;
str_init(&s, 0);

str_append(&s, "abc");
str_append(&s, "123");
log_debug("%s\n", s.str);

str_reset(&s);
```

> 先调str\_init进行初始化，最后调用str\_reset释放内存。
----------------------------------
### 函数
<p id="str_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#str_t_str_append">str\_append</a> | 追加字符串。 |
| <a href="#str_t_str_append_c_str">str\_append\_c\_str</a> | 追加一个字符串，字符串前后加英文双引号，并按C语言规则转义特殊字符。 |
| <a href="#str_t_str_append_char">str\_append\_char</a> | 追加一个字符。 |
| <a href="#str_t_str_append_double">str\_append\_double</a> | 追加一个浮点数。 |
| <a href="#str_t_str_append_escape">str\_append\_escape</a> | 对字符串s进行转义，并追加到str对象。 |
| <a href="#str_t_str_append_format">str\_append\_format</a> | 通过格式追加字符串。 |
| <a href="#str_t_str_append_int">str\_append\_int</a> | 追加一个整数。 |
| <a href="#str_t_str_append_int64">str\_append\_int64</a> | 追加一个int64整数。 |
| <a href="#str_t_str_append_json_bool_pair">str\_append\_json\_bool\_pair</a> | 追加bool格式的json键值对。 |
| <a href="#str_t_str_append_json_double_pair">str\_append\_json\_double\_pair</a> | 追加doube格式的json键值对。 |
| <a href="#str_t_str_append_json_int_pair">str\_append\_json\_int\_pair</a> | 追加int格式的json键值对。 |
| <a href="#str_t_str_append_json_str">str\_append\_json\_str</a> | 追加一个字符串，字符串前后加英文双引号，并按JSON规则转义特殊字符。 |
| <a href="#str_t_str_append_json_str_pair">str\_append\_json\_str\_pair</a> | 追加字符串格式的json键值对。 |
| <a href="#str_t_str_append_more">str\_append\_more</a> | 追加多个字符串。以NULL结束。 |
| <a href="#str_t_str_append_n_chars">str\_append\_n\_chars</a> | 同一个字符追加n次。 |
| <a href="#str_t_str_append_uint64">str\_append\_uint64</a> | 追加一个uint64整数。 |
| <a href="#str_t_str_append_unescape">str\_append\_unescape</a> | 对字符串s进行反转义，并追加到str对象。 |
| <a href="#str_t_str_append_with_len">str\_append\_with\_len</a> | 追加字符串。 |
| <a href="#str_t_str_attach">str\_attach</a> | 通过附加到一个buff来初始化str。 |
| <a href="#str_t_str_attach_with_size">str\_attach\_with\_size</a> | 通过附加到一个buff来初始化str。 |
| <a href="#str_t_str_clear">str\_clear</a> | 清除字符串内容。 |
| <a href="#str_t_str_common_prefix">str\_common\_prefix</a> | 计算str和other的共同前缀，并设置到str中。 |
| <a href="#str_t_str_count">str\_count</a> | 统计字串出现的次数。 |
| <a href="#str_t_str_create">str\_create</a> | 创建str对象。 |
| <a href="#str_t_str_decode_hex">str\_decode\_hex</a> | 把16进制格式的字符串解码成字符串。 |
| <a href="#str_t_str_decode_xml_entity">str\_decode\_xml\_entity</a> | 对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。 |
| <a href="#str_t_str_decode_xml_entity_with_len">str\_decode\_xml\_entity\_with\_len</a> | 对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。 |
| <a href="#str_t_str_destroy">str\_destroy</a> | 销毁str对象 |
| <a href="#str_t_str_encode_hex">str\_encode\_hex</a> | 把二进制的数据编码成16进制格式的字符串。 |
| <a href="#str_t_str_encode_xml_entity">str\_encode\_xml\_entity</a> | 对XML基本的entity进行编码，目前仅支持&lt;&gt;&quota;&amp;。 |
| <a href="#str_t_str_encode_xml_entity_with_len">str\_encode\_xml\_entity\_with\_len</a> | 对XML基本的entity进行编码，目前仅支持&lt;&gt;&quota;&amp;。 |
| <a href="#str_t_str_end_with">str\_end\_with</a> | 判断字符串是否以指定的子串结尾。 |
| <a href="#str_t_str_eq">str\_eq</a> | 判断两个字符串是否相等。 |
| <a href="#str_t_str_expand_vars">str\_expand\_vars</a> | 将字符串中的变量展开为obj中对应的属性值。 |
| <a href="#str_t_str_extend">str\_extend</a> | 扩展字符串到指定的容量。 |
| <a href="#str_t_str_format">str\_format</a> | 通过格式设置字符串。 |
| <a href="#str_t_str_from_float">str\_from\_float</a> | 用浮点数初始化字符串。 |
| <a href="#str_t_str_from_int">str\_from\_int</a> | 用整数初始化字符串。 |
| <a href="#str_t_str_from_int64">str\_from\_int64</a> | 用整数初始化字符串。 |
| <a href="#str_t_str_from_uint32">str\_from\_uint32</a> | 用整数初始化字符串。 |
| <a href="#str_t_str_from_uint64">str\_from\_uint64</a> | 用整数初始化字符串。 |
| <a href="#str_t_str_from_value">str\_from\_value</a> | 用value初始化字符串。 |
| <a href="#str_t_str_from_wstr">str\_from\_wstr</a> | 用value初始化字符串。 |
| <a href="#str_t_str_from_wstr_with_len">str\_from\_wstr\_with\_len</a> | 用value初始化字符串。 |
| <a href="#str_t_str_init">str\_init</a> | 初始化字符串对象。 |
| <a href="#str_t_str_insert">str\_insert</a> | 插入子字符串。 |
| <a href="#str_t_str_insert_with_len">str\_insert\_with\_len</a> | 插入子字符串。 |
| <a href="#str_t_str_pop">str\_pop</a> | 删除最后一个字符。 |
| <a href="#str_t_str_remove">str\_remove</a> | 删除子字符串。 |
| <a href="#str_t_str_replace">str\_replace</a> | 字符串替换。 |
| <a href="#str_t_str_reset">str\_reset</a> | 重置字符串为空。 |
| <a href="#str_t_str_reverse">str\_reverse</a> | 前后颠倒字符串。 |
| <a href="#str_t_str_set">str\_set</a> | 设置字符串。 |
| <a href="#str_t_str_set_with_len">str\_set\_with\_len</a> | 设置字符串。 |
| <a href="#str_t_str_start_with">str\_start\_with</a> | 判断字符串是否以指定的子串开头。 |
| <a href="#str_t_str_to_float">str\_to\_float</a> | 将字符串转成浮点数。 |
| <a href="#str_t_str_to_int">str\_to\_int</a> | 将字符串转成整数。 |
| <a href="#str_t_str_to_lower">str\_to\_lower</a> | 将字符串转成小写。 |
| <a href="#str_t_str_to_upper">str\_to\_upper</a> | 将字符串转成大写。 |
| <a href="#str_t_str_trim">str\_trim</a> | 去除首尾指定的字符。 |
| <a href="#str_t_str_trim_left">str\_trim\_left</a> | 去除首部指定的字符。 |
| <a href="#str_t_str_trim_right">str\_trim\_right</a> | 去除尾部指定的字符。 |
| <a href="#str_t_str_unescape">str\_unescape</a> | 对字符串进行反转义。如：把"\n"转换成'\n'。 |
### 属性
<p id="str_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#str_t_capacity">capacity</a> | uint32\_t | 容量。 |
| <a href="#str_t_size">size</a> | uint32\_t | 长度。 |
| <a href="#str_t_str">str</a> | char* | 字符串。 |
#### str\_append 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append">追加字符串。

* 函数原型：

```
ret_t str_append (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要追加的字符串。 |
#### str\_append\_c\_str 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_c_str">追加一个字符串，字符串前后加英文双引号，并按C语言规则转义特殊字符。

* 函数原型：

```
ret_t str_append_c_str (str_t* str, const char* c_str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| c\_str | const char* | 待追加的字符串。 |
#### str\_append\_char 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_char">追加一个字符。

* 函数原型：

```
ret_t str_append_char (str_t* str, char c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| c | char | 要追加的字符。 |
#### str\_append\_double 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_double">追加一个浮点数。

* 函数原型：

```
ret_t str_append_double (str_t* str, const char* format, double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| format | const char* | 格式。 |
| value | double | 要追加的浮点数。 |
#### str\_append\_escape 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_escape">对字符串s进行转义，并追加到str对象。

* 函数原型：

```
ret_t str_append_escape (str_t* str, const char* s, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| s | const char* | 字符串。 |
| size | uint32\_t | 字符串s的长度。 |
#### str\_append\_format 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_format">通过格式追加字符串。

* 函数原型：

```
ret_t str_append_format (str_t* str, uint32_t size, const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| size | uint32\_t | format生成的字符串的最大长度(用于预先分配内存)。 |
| format | const char* | 格式。 |
#### str\_append\_int 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_int">追加一个整数。

* 函数原型：

```
ret_t str_append_int (str_t* str, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | int32\_t | 要追加的整数。 |
#### str\_append\_int64 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_int64">追加一个int64整数。

* 函数原型：

```
ret_t str_append_int64 (str_t* str, int64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | int64\_t | 要追加的整数。 |
#### str\_append\_json\_bool\_pair 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_json_bool_pair">追加bool格式的json键值对。

* 函数原型：

```
ret_t str_append_json_bool_pair (str_t* str, const char* key, bool_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| key | const char* | 键。 |
| value | bool\_t | 值。 |
#### str\_append\_json\_double\_pair 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_json_double_pair">追加doube格式的json键值对。

* 函数原型：

```
ret_t str_append_json_double_pair (str_t* str, const char* key, double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| key | const char* | 键。 |
| value | double | 值。 |
#### str\_append\_json\_int\_pair 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_json_int_pair">追加int格式的json键值对。

* 函数原型：

```
ret_t str_append_json_int_pair (str_t* str, const char* key, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| key | const char* | 键。 |
| value | int32\_t | 值。 |
#### str\_append\_json\_str 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_json_str">追加一个字符串，字符串前后加英文双引号，并按JSON规则转义特殊字符。

* 函数原型：

```
ret_t str_append_json_str (str_t* str, const char* json_str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| json\_str | const char* | 待追加的字符串。 |
#### str\_append\_json\_str\_pair 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_json_str_pair">追加字符串格式的json键值对。

* 函数原型：

```
ret_t str_append_json_str_pair (str_t* str, const char* key, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| key | const char* | 键。 |
| value | const char* | 值。 |
#### str\_append\_more 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_more">追加多个字符串。以NULL结束。

示例：

```c
str_t s;
str_init(&s, 0);

str_append_more(&s, "abc", "123", NULL);
log_debug("%s\n", s.str);

str_reset(&s);
```

* 函数原型：

```
ret_t str_append_more (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要追加的字符串。 |
#### str\_append\_n\_chars 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_n_chars">同一个字符追加n次。

* 函数原型：

```
ret_t str_append_n_chars (str_t* str, char c, uint32_t n);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| c | char | 要追加的字符。 |
| n | uint32\_t | 字符的个数。 |
#### str\_append\_uint64 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_uint64">追加一个uint64整数。

* 函数原型：

```
ret_t str_append_uint64 (str_t* str, uint64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | uint64\_t | 要追加的整数。 |
#### str\_append\_unescape 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_unescape">对字符串s进行反转义，并追加到str对象。

* 函数原型：

```
ret_t str_append_unescape (str_t* str, const char* s, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| s | const char* | 字符串。 |
| size | uint32\_t | 字符串s的长度。 |
#### str\_append\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_with_len">追加字符串。

* 函数原型：

```
ret_t str_append_with_len (str_t* str, const char* text, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要追加的字符串。 |
| len | uint32\_t | 字符串长度。 |
#### str\_attach 函数
-----------------------

* 函数功能：

> <p id="str_t_str_attach">通过附加到一个buff来初始化str。
>可以避免str动态分配内存，同时也不会自动扩展内存，使用完成后无需调用str_reset。
```c
str_t s;
char buff[32];
str_attach(&s, buff, ARRAY_SIZE(buff));
str_set(&s, "abc");
str_append(&s, "123");
```

* 函数原型：

```
str_t* str_attach (str_t* str, char* buff, uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | str\_t* | str对象本身。 |
| str | str\_t* | str对象。 |
| buff | char* | 缓冲区。 |
| capacity | uint32\_t | 初始容量。 |
#### str\_attach\_with\_size 函数
-----------------------

* 函数功能：

> <p id="str_t_str_attach_with_size">通过附加到一个buff来初始化str。
>可以避免str动态分配内存，同时也不会自动扩展内存，使用完成后无需调用str_reset。
```c
str_t s;
char buff[32];
strcpy(buff, "a");
str_attach_with_size(&s, buff, 1, ARRAY_SIZE(buff));
str_set(&s, "abc");
str_append(&s, "123");
```

* 函数原型：

```
str_t* str_attach_with_size (str_t* str, char* buff, uint32_t size, uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | str\_t* | str对象本身。 |
| str | str\_t* | str对象。 |
| buff | char* | 缓冲区。 |
| size | uint32\_t | 初始长度。 |
| capacity | uint32\_t | 初始容量。 |
#### str\_clear 函数
-----------------------

* 函数功能：

> <p id="str_t_str_clear">清除字符串内容。

* 函数原型：

```
ret_t str_clear (str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
#### str\_common\_prefix 函数
-----------------------

* 函数功能：

> <p id="str_t_str_common_prefix">计算str和other的共同前缀，并设置到str中。

* 函数原型：

```
ret_t str_common_prefix (str_t* str, const char* other);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| other | const char* | 另外一个字符串。 |
#### str\_count 函数
-----------------------

* 函数功能：

> <p id="str_t_str_count">统计字串出现的次数。

* 函数原型：

```
uint32_t str_count (str_t* str, const char* substr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回字符串出现的次数。 |
| str | str\_t* | str对象。 |
| substr | const char* | 字串。 |
#### str\_create 函数
-----------------------

* 函数功能：

> <p id="str_t_str_create">创建str对象。

* 函数原型：

```
str_t* str_create (uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | str\_t* | str对象。 |
| capacity | uint32\_t | 初始容量。 |
#### str\_decode\_hex 函数
-----------------------

* 函数功能：

> <p id="str_t_str_decode_hex">把16进制格式的字符串解码成字符串。

* 函数原型：

```
ret_t str_decode_hex (str_t* str, uint8_t* data, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| data | uint8\_t* | 数据缓存区(返回)。 |
| size | uint32\_t | 数据最大长度。 |
#### str\_decode\_xml\_entity 函数
-----------------------

* 函数功能：

> <p id="str_t_str_decode_xml_entity">对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。

* 函数原型：

```
ret_t str_decode_xml_entity (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要解码的XML文本。 |
#### str\_decode\_xml\_entity\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_decode_xml_entity_with_len">对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。

* 函数原型：

```
ret_t str_decode_xml_entity_with_len (str_t* str, const char* text, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要解码的XML文本。 |
| len | uint32\_t | 字符串长度。 |
#### str\_destroy 函数
-----------------------

* 函数功能：

> <p id="str_t_str_destroy">销毁str对象

* 函数原型：

```
ret_t str_destroy (str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
#### str\_encode\_hex 函数
-----------------------

* 函数功能：

> <p id="str_t_str_encode_hex">把二进制的数据编码成16进制格式的字符串。

* 函数原型：

```
ret_t str_encode_hex (str_t* str, const uint8_t* data, uint32_t size, const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| data | const uint8\_t* | 数据。 |
| size | uint32\_t | 数据长度。 |
| format | const char* | 格式(如:"%02x" 表示生成小写) |
#### str\_encode\_xml\_entity 函数
-----------------------

* 函数功能：

> <p id="str_t_str_encode_xml_entity">对XML基本的entity进行编码，目前仅支持&lt;&gt;&quota;&amp;。

* 函数原型：

```
ret_t str_encode_xml_entity (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要编码的XML文本。 |
#### str\_encode\_xml\_entity\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_encode_xml_entity_with_len">对XML基本的entity进行编码，目前仅支持&lt;&gt;&quota;&amp;。

* 函数原型：

```
ret_t str_encode_xml_entity_with_len (str_t* str, const char* text, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要编码的XML文本。 |
| len | uint32\_t | 字符串长度。 |
#### str\_end\_with 函数
-----------------------

* 函数功能：

> <p id="str_t_str_end_with">判断字符串是否以指定的子串结尾。

* 函数原型：

```
bool_t str_end_with (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回是否以指定的子串结尾。 |
| str | str\_t* | str对象。 |
| text | const char* | 子字符串。 |
#### str\_eq 函数
-----------------------

* 函数功能：

> <p id="str_t_str_eq">判断两个字符串是否相等。

* 函数原型：

```
bool_t str_eq (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回是否相等。 |
| str | str\_t* | str对象。 |
| text | const char* | 待比较的字符串。 |
#### str\_expand\_vars 函数
-----------------------

* 函数功能：

> <p id="str_t_str_expand_vars">将字符串中的变量展开为obj中对应的属性值。

变量的格式为${xxx}：

* xxx为变量名时，${xxx}被展开为obj的属性xxx的值。
* xxx为表达式时，${xxx}被展开为表达式的值，表达式中可以用变量，$为变量的前缀，如${$x+$y}。
* xxx为变量名时，而不存在obj的属性时，${xxx}被移出。

* 函数原型：

```
ret_t str_expand_vars (str_t* str, const char* src, const tk_object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| src | const char* | 字符串。 |
| obj | const tk\_object\_t* | obj对象。 |
#### str\_extend 函数
-----------------------

* 函数功能：

> <p id="str_t_str_extend">扩展字符串到指定的容量。

* 函数原型：

```
ret_t str_extend (str_t* str, uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| capacity | uint32\_t | 初始容量。 |
#### str\_format 函数
-----------------------

* 函数功能：

> <p id="str_t_str_format">通过格式设置字符串。

* 函数原型：

```
ret_t str_format (str_t* str, uint32_t size, const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| size | uint32\_t | format生成的字符串的最大长度(用于预先分配内存)。 |
| format | const char* | 格式。 |
#### str\_from\_float 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_float">用浮点数初始化字符串。

* 函数原型：

```
ret_t str_from_float (str_t* str, double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | double | 浮点数。 |
#### str\_from\_int 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_int">用整数初始化字符串。

* 函数原型：

```
ret_t str_from_int (str_t* str, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | int32\_t | 整数。 |
#### str\_from\_int64 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_int64">用整数初始化字符串。

* 函数原型：

```
ret_t str_from_int64 (str_t* str, int64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | int64\_t | 整数。 |
#### str\_from\_uint32 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_uint32">用整数初始化字符串。

* 函数原型：

```
ret_t str_from_uint32 (str_t* str, uint32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | uint32\_t | 整数。 |
#### str\_from\_uint64 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_uint64">用整数初始化字符串。

* 函数原型：

```
ret_t str_from_uint64 (str_t* str, uint64_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | uint64\_t | 整数。 |
#### str\_from\_value 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_value">用value初始化字符串。

* 函数原型：

```
ret_t str_from_value (str_t* str, const value_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | const value\_t* | value。 |
#### str\_from\_wstr 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_wstr">用value初始化字符串。

* 函数原型：

```
ret_t str_from_wstr (str_t* str, const wchar_t* wstr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| wstr | const wchar\_t* | Unicode字符串。 |
#### str\_from\_wstr\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_wstr_with_len">用value初始化字符串。

* 函数原型：

```
ret_t str_from_wstr_with_len (str_t* str, const wchar_t* wstr, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| wstr | const wchar\_t* | Unicode字符串 |
| len | uint32\_t | Unicode字符串的长度。 |
#### str\_init 函数
-----------------------

* 函数功能：

> <p id="str_t_str_init">初始化字符串对象。

* 函数原型：

```
str_t* str_init (str_t* str, uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | str\_t* | str对象本身。 |
| str | str\_t* | str对象。 |
| capacity | uint32\_t | 初始容量。 |
#### str\_insert 函数
-----------------------

* 函数功能：

> <p id="str_t_str_insert">插入子字符串。

* 函数原型：

```
ret_t str_insert (str_t* str, uint32_t offset, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| offset | uint32\_t | 偏移量。 |
| text | const char* | 要插入的字符串。 |
#### str\_insert\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_insert_with_len">插入子字符串。

* 函数原型：

```
ret_t str_insert_with_len (str_t* str, uint32_t offset, const char* text, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| offset | uint32\_t | 偏移量。 |
| text | const char* | 要插入的字符串。 |
| len | uint32\_t | 字符串长度。 |
#### str\_pop 函数
-----------------------

* 函数功能：

> <p id="str_t_str_pop">删除最后一个字符。

* 函数原型：

```
ret_t str_pop (str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
#### str\_remove 函数
-----------------------

* 函数功能：

> <p id="str_t_str_remove">删除子字符串。

* 函数原型：

```
ret_t str_remove (str_t* str, uint32_t offset, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| offset | uint32\_t | 偏移量。 |
| len | uint32\_t | 长度。 |
#### str\_replace 函数
-----------------------

* 函数功能：

> <p id="str_t_str_replace">字符串替换。

* 函数原型：

```
ret_t str_replace (str_t* str, const char* text, const char* new_text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 待替换的子串。 |
| new\_text | const char* | 将替换成的子串。 |
#### str\_reset 函数
-----------------------

* 函数功能：

> <p id="str_t_str_reset">重置字符串为空。

* 函数原型：

```
ret_t str_reset (str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
#### str\_reverse 函数
-----------------------

* 函数功能：

> <p id="str_t_str_reverse">前后颠倒字符串。

* 函数原型：

```
ret_t str_reverse (str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
#### str\_set 函数
-----------------------

* 函数功能：

> <p id="str_t_str_set">设置字符串。

* 函数原型：

```
ret_t str_set (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要设置的字符串。 |
#### str\_set\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_set_with_len">设置字符串。

* 函数原型：

```
ret_t str_set_with_len (str_t* str, const char* text, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要设置的字符串。 |
| len | uint32\_t | 字符串长度。 |
#### str\_start\_with 函数
-----------------------

* 函数功能：

> <p id="str_t_str_start_with">判断字符串是否以指定的子串开头。

* 函数原型：

```
bool_t str_start_with (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回是否以指定的子串开头。 |
| str | str\_t* | str对象。 |
| text | const char* | 子字符串。 |
#### str\_to\_float 函数
-----------------------

* 函数功能：

> <p id="str_t_str_to_float">将字符串转成浮点数。

* 函数原型：

```
ret_t str_to_float (str_t* str, double* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | double* | 用于返回浮点数。 |
#### str\_to\_int 函数
-----------------------

* 函数功能：

> <p id="str_t_str_to_int">将字符串转成整数。

* 函数原型：

```
ret_t str_to_int (str_t* str, int32_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| value | int32\_t* | 用于返回整数。 |
#### str\_to\_lower 函数
-----------------------

* 函数功能：

> <p id="str_t_str_to_lower">将字符串转成小写。

* 函数原型：

```
ret_t str_to_lower (str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
#### str\_to\_upper 函数
-----------------------

* 函数功能：

> <p id="str_t_str_to_upper">将字符串转成大写。

* 函数原型：

```
ret_t str_to_upper (str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
#### str\_trim 函数
-----------------------

* 函数功能：

> <p id="str_t_str_trim">去除首尾指定的字符。

* 函数原型：

```
ret_t str_trim (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要去除的字符集合。 |
#### str\_trim\_left 函数
-----------------------

* 函数功能：

> <p id="str_t_str_trim_left">去除首部指定的字符。

* 函数原型：

```
ret_t str_trim_left (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要去除的字符集合。 |
#### str\_trim\_right 函数
-----------------------

* 函数功能：

> <p id="str_t_str_trim_right">去除尾部指定的字符。

* 函数原型：

```
ret_t str_trim_right (str_t* str, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | const char* | 要去除的字符集合。 |
#### str\_unescape 函数
-----------------------

* 函数功能：

> <p id="str_t_str_unescape">对字符串进行反转义。如：把"\n"转换成'\n'。

* 函数原型：

```
ret_t str_unescape (str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
#### capacity 属性
-----------------------
> <p id="str_t_capacity">容量。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="str_t_size">长度。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### str 属性
-----------------------
> <p id="str_t_str">字符串。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
