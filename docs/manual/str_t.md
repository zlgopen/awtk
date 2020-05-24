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
| <a href="#str_t_str_append_char">str\_append\_char</a> | 追加一个字符。 |
| <a href="#str_t_str_append_double">str\_append\_double</a> | 追加一个浮点数。 |
| <a href="#str_t_str_append_int">str\_append\_int</a> | 追加一个整数。 |
| <a href="#str_t_str_append_json_bool_pair">str\_append\_json\_bool\_pair</a> | 追加bool格式的json键值对。 |
| <a href="#str_t_str_append_json_double_pair">str\_append\_json\_double\_pair</a> | 追加doube格式的json键值对。 |
| <a href="#str_t_str_append_json_int_pair">str\_append\_json\_int\_pair</a> | 追加int格式的json键值对。 |
| <a href="#str_t_str_append_json_str">str\_append\_json\_str</a> | 追加一个字符串，字符串前后加英文双引号，字符串本身的双引号被转义为\"。 |
| <a href="#str_t_str_append_json_str_pair">str\_append\_json\_str\_pair</a> | 追加字符串格式的json键值对。 |
| <a href="#str_t_str_append_more">str\_append\_more</a> | 追加多个字符串。以NULL结束。 |
| <a href="#str_t_str_append_with_len">str\_append\_with\_len</a> | 追加字符串。 |
| <a href="#str_t_str_clear">str\_clear</a> | 清除字符串内容。 |
| <a href="#str_t_str_decode_xml_entity">str\_decode\_xml\_entity</a> | 对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。 |
| <a href="#str_t_str_decode_xml_entity_with_len">str\_decode\_xml\_entity\_with\_len</a> | 对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。 |
| <a href="#str_t_str_end_with">str\_end\_with</a> | 判断字符串是否以指定的子串结尾。 |
| <a href="#str_t_str_eq">str\_eq</a> | 判断两个字符串是否相等。 |
| <a href="#str_t_str_expand_vars">str\_expand\_vars</a> | 将字符串中的变量展开为obj中对应的属性值。 |
| <a href="#str_t_str_extend">str\_extend</a> | 扩展字符串到指定的容量。 |
| <a href="#str_t_str_from_float">str\_from\_float</a> | 用浮点数初始化字符串。 |
| <a href="#str_t_str_from_int">str\_from\_int</a> | 用整数初始化字符串。 |
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
ret_t str_append (str_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 要追加的字符串。 |
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

> <p id="str_t_str_append_json_str">追加一个字符串，字符串前后加英文双引号，字符串本身的双引号被转义为\"。

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
ret_t str_append_more (str_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 要追加的字符串。 |
#### str\_append\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_append_with_len">追加字符串。

* 函数原型：

```
ret_t str_append_with_len (str_t* str, char* text, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 要追加的字符串。 |
| len | uint32\_t | 字符串长度。 |
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
#### str\_decode\_xml\_entity 函数
-----------------------

* 函数功能：

> <p id="str_t_str_decode_xml_entity">对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。

* 函数原型：

```
ret_t str_decode_xml_entity (str_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 要解码的XML文本。 |
#### str\_decode\_xml\_entity\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_decode_xml_entity_with_len">对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。

* 函数原型：

```
ret_t str_decode_xml_entity_with_len (str_t* str, char* text, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 要解码的XML文本。 |
| len | uint32\_t | 字符串长度。 |
#### str\_end\_with 函数
-----------------------

* 函数功能：

> <p id="str_t_str_end_with">判断字符串是否以指定的子串结尾。

* 函数原型：

```
bool_t str_end_with (str_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回是否以指定的子串结尾。 |
| str | str\_t* | str对象。 |
| text | char* | 子字符串。 |
#### str\_eq 函数
-----------------------

* 函数功能：

> <p id="str_t_str_eq">判断两个字符串是否相等。

* 函数原型：

```
bool_t str_eq (str_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回是否相等。 |
| str | str\_t* | str对象。 |
| text | char* | 待比较的字符串。 |
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
ret_t str_expand_vars (str_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
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
#### str\_from\_float 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_float">用浮点数初始化字符串。

* 函数原型：

```
ret_t str_from_float (str_t* str, double v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| v | double | 浮点数。 |
#### str\_from\_int 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_int">用整数初始化字符串。

* 函数原型：

```
ret_t str_from_int (str_t* str, int32_t v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| v | int32\_t | 整数。 |
#### str\_from\_value 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_value">用value初始化字符串。

* 函数原型：

```
ret_t str_from_value (str_t* str, value_t v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| v | value\_t | value。 |
#### str\_from\_wstr 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_wstr">用value初始化字符串。

* 函数原型：

```
ret_t str_from_wstr (str_t* str, wchar_t* wstr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| wstr | wchar\_t* | Unicode字符串。 |
#### str\_from\_wstr\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_from_wstr_with_len">用value初始化字符串。

* 函数原型：

```
ret_t str_from_wstr_with_len (str_t* str, wchar_t* wstr, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| wstr | wchar\_t* | Unicode字符串 |
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
ret_t str_insert (str_t* str, uint32_t offset, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| offset | uint32\_t | 偏移量。 |
| text | char* | 要插入的字符串。 |
#### str\_insert\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_insert_with_len">插入子字符串。

* 函数原型：

```
ret_t str_insert_with_len (str_t* str, uint32_t offset, char* text, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| offset | uint32\_t | 偏移量。 |
| text | char* | 要插入的字符串。 |
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
ret_t str_replace (str_t* str, char* text, char* new_text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 待替换的子串。 |
| new\_text | char* | 将替换成的子串。 |
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
#### str\_set 函数
-----------------------

* 函数功能：

> <p id="str_t_str_set">设置字符串。

* 函数原型：

```
ret_t str_set (str_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 要设置的字符串。 |
#### str\_set\_with\_len 函数
-----------------------

* 函数功能：

> <p id="str_t_str_set_with_len">设置字符串。

* 函数原型：

```
ret_t str_set_with_len (str_t* str, char* text, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 要设置的字符串。 |
| len | uint32\_t | 字符串长度。 |
#### str\_start\_with 函数
-----------------------

* 函数功能：

> <p id="str_t_str_start_with">判断字符串是否以指定的子串开头。

* 函数原型：

```
bool_t str_start_with (str_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回是否以指定的子串开头。 |
| str | str\_t* | str对象。 |
| text | char* | 子字符串。 |
#### str\_to\_float 函数
-----------------------

* 函数功能：

> <p id="str_t_str_to_float">将字符串转成浮点数。

* 函数原型：

```
ret_t str_to_float (str_t* str, double* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| v | double* | 用于返回浮点数。 |
#### str\_to\_int 函数
-----------------------

* 函数功能：

> <p id="str_t_str_to_int">将字符串转成整数。

* 函数原型：

```
ret_t str_to_int (str_t* str, int32_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| v | int32\_t* | 用于返回整数。 |
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
ret_t str_trim (str_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 要去除的字符集合。 |
#### str\_trim\_left 函数
-----------------------

* 函数功能：

> <p id="str_t_str_trim_left">去除首部指定的字符。

* 函数原型：

```
ret_t str_trim_left (str_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 要去除的字符集合。 |
#### str\_trim\_right 函数
-----------------------

* 函数功能：

> <p id="str_t_str_trim_right">去除尾部指定的字符。

* 函数原型：

```
ret_t str_trim_right (str_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | str\_t* | str对象。 |
| text | char* | 要去除的字符集合。 |
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
