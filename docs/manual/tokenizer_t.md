## tokenizer\_t
### 概述
从字符串中解析出一个一个的token。

```c
tokenizer_t tokenizer;
tokenizer_t* t = tokenizer_init(&tokenizer, "20,111.2,22.3,333.3,44,555.5", 0xffffff, ",");

while(tokenizer_has_more(t)) {
double v = tokenizer_next_float(t, 0);
log_debug("%lf\n", v);
}

tokenizer_deinit(t);
```
----------------------------------
### 函数
<p id="tokenizer_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tokenizer_t_tokenizer_deinit">tokenizer\_deinit</a> | 重置tokenizer。 |
| <a href="#tokenizer_t_tokenizer_has_more">tokenizer\_has\_more</a> | 是否还有下一个token。 |
| <a href="#tokenizer_t_tokenizer_init">tokenizer\_init</a> | 初始化tokenizer对象。 |
| <a href="#tokenizer_t_tokenizer_init_ex">tokenizer\_init\_ex</a> | 初始化tokenizer对象。 |
| <a href="#tokenizer_t_tokenizer_next">tokenizer\_next</a> | 获取下一个token。 |
| <a href="#tokenizer_t_tokenizer_next_expr_until">tokenizer\_next\_expr\_until</a> | 获取下一个expr，该expr直到遇到指定的char。 |
| <a href="#tokenizer_t_tokenizer_next_float">tokenizer\_next\_float</a> | 获取下一个token，并转换成float。 |
| <a href="#tokenizer_t_tokenizer_next_int">tokenizer\_next\_int</a> | 获取下一个token，并转换成int。 |
| <a href="#tokenizer_t_tokenizer_next_until">tokenizer\_next\_until</a> | 获取下一个token，该token直到遇到指定的char。 |
### 属性
<p id="tokenizer_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#tokenizer_t_cursor">cursor</a> | uint32\_t | 当前位置。 |
| <a href="#tokenizer_t_separtor">separtor</a> | char* | 分隔字符串。 |
| <a href="#tokenizer_t_single_char_token">single\_char\_token</a> | char* | 单字符的token。 |
| <a href="#tokenizer_t_size">size</a> | uint32\_t | 字符串的长度。 |
| <a href="#tokenizer_t_str">str</a> | char* | 字符串。 |
#### tokenizer\_deinit 函数
-----------------------

* 函数功能：

> <p id="tokenizer_t_tokenizer_deinit">重置tokenizer。

* 函数原型：

```
ret_t tokenizer_deinit (tokenizer_t* tokenizer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| tokenizer | tokenizer\_t* | tokenizer对象。 |
#### tokenizer\_has\_more 函数
-----------------------

* 函数功能：

> <p id="tokenizer_t_tokenizer_has_more">是否还有下一个token。

* 函数原型：

```
bool_t tokenizer_has_more (tokenizer_t* tokenizer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 还有下一个token返回TRUE，否则返回FALSE。 |
| tokenizer | tokenizer\_t* | tokenizer对象。 |
#### tokenizer\_init 函数
-----------------------

* 函数功能：

> <p id="tokenizer_t_tokenizer_init">初始化tokenizer对象。

* 函数原型：

```
tokenizer_t* tokenizer_init (tokenizer_t* tokenizer, char* str, uint32_t size, char* separtor);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tokenizer\_t* | tokenizer对象本身。 |
| tokenizer | tokenizer\_t* | tokenizer对象。 |
| str | char* | 要解析的字符串。 |
| size | uint32\_t | 字符串长度。 |
| separtor | char* | 分隔字符。 |
#### tokenizer\_init\_ex 函数
-----------------------

* 函数功能：

> <p id="tokenizer_t_tokenizer_init_ex">初始化tokenizer对象。

* 函数原型：

```
tokenizer_t* tokenizer_init_ex (tokenizer_t* tokenizer, char* str, uint32_t size, char* separtor, char* single_char_token);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tokenizer\_t* | tokenizer对象本身。 |
| tokenizer | tokenizer\_t* | tokenizer对象。 |
| str | char* | 要解析的字符串。 |
| size | uint32\_t | 字符串长度。 |
| separtor | char* | 分隔字符。 |
| single\_char\_token | char* | 单字符token。 |
#### tokenizer\_next 函数
-----------------------

* 函数功能：

> <p id="tokenizer_t_tokenizer_next">获取下一个token。

* 函数原型：

```
char* tokenizer_next (tokenizer_t* tokenizer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 成功返回token，失败返回NULL。 |
| tokenizer | tokenizer\_t* | tokenizer对象。 |
#### tokenizer\_next\_expr\_until 函数
-----------------------

* 函数功能：

> <p id="tokenizer_t_tokenizer_next_expr_until">获取下一个expr，该expr直到遇到指定的char。

* 函数原型：

```
char* tokenizer_next_expr_until (tokenizer_t* tokenizer, const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 成功返回token，失败返回NULL。 |
| tokenizer | tokenizer\_t* | tokenizer对象。 |
| str | const char* | 字符集。 |
#### tokenizer\_next\_float 函数
-----------------------

* 函数功能：

> <p id="tokenizer_t_tokenizer_next_float">获取下一个token，并转换成float。

* 函数原型：

```
char* tokenizer_next_float (tokenizer_t* tokenizer, float defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 成功返回token的float值，失败返回缺省值。 |
| tokenizer | tokenizer\_t* | tokenizer对象。 |
| defval | float | 缺省值。 |
#### tokenizer\_next\_int 函数
-----------------------

* 函数功能：

> <p id="tokenizer_t_tokenizer_next_int">获取下一个token，并转换成int。

* 函数原型：

```
char* tokenizer_next_int (tokenizer_t* tokenizer, int defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 成功返回token的int值，失败返回缺省值。 |
| tokenizer | tokenizer\_t* | tokenizer对象。 |
| defval | int | 缺省值。 |
#### tokenizer\_next\_until 函数
-----------------------

* 函数功能：

> <p id="tokenizer_t_tokenizer_next_until">获取下一个token，该token直到遇到指定的char。

* 函数原型：

```
char* tokenizer_next_until (tokenizer_t* tokenizer, const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 成功返回token，失败返回NULL。 |
| tokenizer | tokenizer\_t* | tokenizer对象。 |
| str | const char* | 字符集。 |
#### cursor 属性
-----------------------
> <p id="tokenizer_t_cursor">当前位置。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### separtor 属性
-----------------------
> <p id="tokenizer_t_separtor">分隔字符串。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### single\_char\_token 属性
-----------------------
> <p id="tokenizer_t_single_char_token">单字符的token。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="tokenizer_t_size">字符串的长度。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### str 属性
-----------------------
> <p id="tokenizer_t_str">字符串。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
