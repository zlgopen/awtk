## wstr\_t
### 概述
可变长度的宽字符字符串。

示例：

```c
wstr_t s;
wstr_init(&s, 0);

wstr_append(&s, L"abc");
wstr_append(&s, L"123");

wstr_reset(&s);
```
> 先调wstr\_init进行初始化，最后调用wstr\_reset释放内存。
----------------------------------
### 函数
<p id="wstr_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#wstr_t_wstr_add_float">wstr\_add\_float</a> | 将字符串转成浮点数，加上delta，再转换回来。 |
| <a href="#wstr_t_wstr_append">wstr\_append</a> | 追加字符串。 |
| <a href="#wstr_t_wstr_append_with_len">wstr\_append\_with\_len</a> | 追加字符串。 |
| <a href="#wstr_t_wstr_clear">wstr\_clear</a> | 清除字符串内容。 |
| <a href="#wstr_t_wstr_equal">wstr\_equal</a> | 判断两个字符是否相同。 |
| <a href="#wstr_t_wstr_from_float">wstr\_from\_float</a> | 用浮点数初始化字符串。 |
| <a href="#wstr_t_wstr_from_int">wstr\_from\_int</a> | 用整数初始化字符串。 |
| <a href="#wstr_t_wstr_from_value">wstr\_from\_value</a> | 用value初始化字符串。 |
| <a href="#wstr_t_wstr_get_utf8">wstr\_get\_utf8</a> | 获取UTF8字符串。 |
| <a href="#wstr_t_wstr_init">wstr\_init</a> | 初始化字符串对象。 |
| <a href="#wstr_t_wstr_insert">wstr\_insert</a> | 在指定位置插入字符串。 |
| <a href="#wstr_t_wstr_normalize_newline">wstr\_normalize\_newline</a> | 规范化换行符。 |
| <a href="#wstr_t_wstr_pop">wstr\_pop</a> | 删除尾部字符。 |
| <a href="#wstr_t_wstr_push">wstr\_push</a> | 追加一个字符。 |
| <a href="#wstr_t_wstr_push_int">wstr\_push\_int</a> | 追加一个整数。 |
| <a href="#wstr_t_wstr_remove">wstr\_remove</a> | 删除指定范围的字符。 |
| <a href="#wstr_t_wstr_reset">wstr\_reset</a> | 重置字符串为空。 |
| <a href="#wstr_t_wstr_set">wstr\_set</a> | 设置字符串。 |
| <a href="#wstr_t_wstr_set_utf8">wstr\_set\_utf8</a> | 设置UTF8字符串。 |
| <a href="#wstr_t_wstr_to_float">wstr\_to\_float</a> | 将字符串转成浮点数。 |
| <a href="#wstr_t_wstr_to_int">wstr\_to\_int</a> | 将字符串转成整数。 |
| <a href="#wstr_t_wstr_trim_float_zero">wstr\_trim\_float\_zero</a> | 去掉浮点数小数点尾部的零。 |
### 属性
<p id="wstr_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#wstr_t_capacity">capacity</a> | uint32\_t | 容量。 |
| <a href="#wstr_t_size">size</a> | uint32\_t | 长度。 |
| <a href="#wstr_t_str">str</a> | wchar\_t* | 字符串。 |
#### wstr\_add\_float 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_add_float">将字符串转成浮点数，加上delta，再转换回来。

* 函数原型：

```
ret_t wstr_add_float (wstr_t* str, double delta);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| delta | double | 要加上的值。 |
#### wstr\_append 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_append">追加字符串。

* 函数原型：

```
ret_t wstr_append (wstr_t* str, wchar_t* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| text | wchar\_t* | 要追加的字符串。 |
#### wstr\_append\_with\_len 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_append_with_len">追加字符串。

* 函数原型：

```
ret_t wstr_append_with_len (wstr_t* str, wchar_t* text, uint32_t len);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| text | wchar\_t* | 要追加的字符串。 |
| len | uint32\_t | 字符串长度。 |
#### wstr\_clear 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_clear">清除字符串内容。

* 函数原型：

```
ret_t wstr_clear (wstr_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
#### wstr\_equal 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_equal">判断两个字符是否相同。

* 函数原型：

```
bool_t wstr_equal (wstr_t* str, wstr_t* other);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示相同，否则表示不同。 |
| str | wstr\_t* | str对象。 |
| other | wstr\_t* | str对象。 |
#### wstr\_from\_float 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_from_float">用浮点数初始化字符串。

* 函数原型：

```
ret_t wstr_from_float (wstr_t* str, double v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| v | double | 浮点数。 |
#### wstr\_from\_int 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_from_int">用整数初始化字符串。

* 函数原型：

```
ret_t wstr_from_int (wstr_t* str, int32_t v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| v | int32\_t | 整数。 |
#### wstr\_from\_value 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_from_value">用value初始化字符串。

* 函数原型：

```
ret_t wstr_from_value (wstr_t* str, value_t v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| v | value\_t | value。 |
#### wstr\_get\_utf8 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_get_utf8">获取UTF8字符串。

* 函数原型：

```
ret_t wstr_get_utf8 (wstr_t* str, char* text, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| text | char* | 返回的字符串。 |
| size | uint32\_t | text最大长度。 |
#### wstr\_init 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_init">初始化字符串对象。

* 函数原型：

```
wstr_t* wstr_init (wstr_t* str, uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | wstr\_t* | str对象本身。 |
| str | wstr\_t* | str对象。 |
| capacity | uint32\_t | 初始容量。 |
#### wstr\_insert 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_insert">在指定位置插入字符串。

* 函数原型：

```
ret_t wstr_insert (wstr_t* str, uint32_t offset, wchar_t* text, uint32_t nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| offset | uint32\_t | 指定的位置。 |
| text | wchar\_t* | 待插入的文本。 |
| nr | uint32\_t | 要插入的字符数。 |
#### wstr\_normalize\_newline 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_normalize_newline">规范化换行符。

* 函数原型：

```
ret_t wstr_normalize_newline (wstr_t* str, wchar_t newline);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| newline | wchar\_t | 换行符。 |
#### wstr\_pop 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_pop">删除尾部字符。

* 函数原型：

```
ret_t wstr_pop (wstr_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
#### wstr\_push 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_push">追加一个字符。

* 函数原型：

```
ret_t wstr_push (wstr_t* str, wchar_t c);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| c | wchar\_t | 字符。 |
#### wstr\_push\_int 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_push_int">追加一个整数。

* 函数原型：

```
ret_t wstr_push_int (wstr_t* str, const char* format, int32_t value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| format | const char* | 格式(用于snprintf格式化数值) |
| value | int32\_t | 数值。 |
#### wstr\_remove 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_remove">删除指定范围的字符。

* 函数原型：

```
ret_t wstr_remove (wstr_t* str, uint32_t offset, uint32_t nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| offset | uint32\_t | 指定的位置。 |
| nr | uint32\_t | 要删除的字符数。 |
#### wstr\_reset 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_reset">重置字符串为空。

* 函数原型：

```
ret_t wstr_reset (wstr_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
#### wstr\_set 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_set">设置字符串。

* 函数原型：

```
ret_t wstr_set (wstr_t* str, wchar_t* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| text | wchar\_t* | 要设置的字符串。 |
#### wstr\_set\_utf8 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_set_utf8">设置UTF8字符串。

* 函数原型：

```
ret_t wstr_set_utf8 (wstr_t* str, char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| text | char* | 要设置的字符串。 |
#### wstr\_to\_float 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_to_float">将字符串转成浮点数。

* 函数原型：

```
ret_t wstr_to_float (wstr_t* str, double* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| v | double* | 用于返回浮点数。 |
#### wstr\_to\_int 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_to_int">将字符串转成整数。

* 函数原型：

```
ret_t wstr_to_int (wstr_t* str, int32_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
| v | int32\_t* | 用于返回整数。 |
#### wstr\_trim\_float\_zero 函数
-----------------------

* 函数功能：

> <p id="wstr_t_wstr_trim_float_zero">去掉浮点数小数点尾部的零。

* 函数原型：

```
ret_t wstr_trim_float_zero (wstr_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | str对象。 |
#### capacity 属性
-----------------------
> <p id="wstr_t_capacity">容量。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="wstr_t_size">长度。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### str 属性
-----------------------
> <p id="wstr_t_str">字符串。

* 类型：wchar\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
