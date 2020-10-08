## line\_break\_check
### 概述
换行相关函数。
----------------------------------
### 函数
<p id="line_break_check_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#line_break_check_line_break">line\_break</a> | 解析字符串，对于每一行调用on_line函数。 |
| <a href="#line_break_check_line_break_check">line\_break\_check</a> | 检查两个字符之间是否允许换行。 |
| <a href="#line_break_check_line_break_count">line\_break\_count</a> | 统计行数。 |
| <a href="#line_break_check_word_break_check">word\_break\_check</a> | 检查两个字符之间是否允许单词分割。 |
#### line\_break 函数
-----------------------

* 函数功能：

> <p id="line_break_check_line_break">解析字符串，对于每一行调用on_line函数。

* 函数原型：

```
ret_t line_break (const wchar_t* str, line_break_on_line_t on_line, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | const wchar\_t* | 字符串。 |
| on\_line | line\_break\_on\_line\_t | 回调函数。 |
| ctx | void* | 回调函数的上下文。 |
#### line\_break\_check 函数
-----------------------

* 函数功能：

> <p id="line_break_check_line_break_check">检查两个字符之间是否允许换行。

* 函数原型：

```
line_break_type_t line_break_check (wchar_t c1, wchar_t c2);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | line\_break\_type\_t | 返回类型。 |
| c1 | wchar\_t | 前面的字符。 |
| c2 | wchar\_t | 后面的字符。 |
#### line\_break\_count 函数
-----------------------

* 函数功能：

> <p id="line_break_check_line_break_count">统计行数。

* 函数原型：

```
uint32_t line_break_count (const wchar_t* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回行数。 |
| str | const wchar\_t* | 字符串。 |
#### word\_break\_check 函数
-----------------------

* 函数功能：

> <p id="line_break_check_word_break_check">检查两个字符之间是否允许单词分割。

* 函数原型：

```
line_break_type_t word_break_check (wchar_t c1, wchar_t c2);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | line\_break\_type\_t | 返回类型。 |
| c1 | wchar\_t | 前面的字符。 |
| c2 | wchar\_t | 后面的字符。 |
