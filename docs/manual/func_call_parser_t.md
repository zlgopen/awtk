## func\_call\_parser\_t
### 概述
从字符串中解析出函数调用需要的参数。

```
func_call => func_name '(' params ')'
params => param ',' params
param => name '=' value
func_name => ID
name = ID
value = int | float | ID
```

如:move(x=10, y=20)
如:rename(old_name=aa, new_name=bb)
----------------------------------
### 函数
<p id="func_call_parser_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#func_call_parser_t_func_call_parse">func\_call\_parse</a> | 解析参数，并将结果生成一个object对象。 |
| <a href="#func_call_parser_t_func_call_parser_deinit">func\_call\_parser\_deinit</a> | 重置parser。 |
| <a href="#func_call_parser_t_func_call_parser_init">func\_call\_parser\_init</a> | 初始化parser对象。 |
| <a href="#func_call_parser_t_func_call_parser_parse">func\_call\_parser\_parse</a> | 开始解析。 |
#### func\_call\_parse 函数
-----------------------

* 函数功能：

> <p id="func_call_parser_t_func_call_parse">解析参数，并将结果生成一个object对象。

> 调用者负责释放返回的object对象。

* 函数原型：

```
object_t* func_call_parse (const char* str, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回object对象。 |
| str | const char* | 要解析的字符串。 |
| size | uint32\_t | 字符串长度。 |
#### func\_call\_parser\_deinit 函数
-----------------------

* 函数功能：

> <p id="func_call_parser_t_func_call_parser_deinit">重置parser。

* 函数原型：

```
ret_t func_call_parser_deinit (func_call_parser_t* parser);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| parser | func\_call\_parser\_t* | parser对象。 |
#### func\_call\_parser\_init 函数
-----------------------

* 函数功能：

> <p id="func_call_parser_t_func_call_parser_init">初始化parser对象。

* 函数原型：

```
func_call_parser_t* func_call_parser_init (func_call_parser_t* parser, const char* str, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | func\_call\_parser\_t* | parser对象本身。 |
| parser | func\_call\_parser\_t* | parser对象。 |
| str | const char* | 要解析的字符串。 |
| size | uint32\_t | 字符串长度。 |
#### func\_call\_parser\_parse 函数
-----------------------

* 函数功能：

> <p id="func_call_parser_t_func_call_parser_parse">开始解析。

* 函数原型：

```
ret_t func_call_parser_parse (func_call_parser_t* parser);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| parser | func\_call\_parser\_t* | parser对象。 |
