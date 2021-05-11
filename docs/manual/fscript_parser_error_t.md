## fscript\_parser\_error\_t
### 概述
解析错误信息。
----------------------------------
### 函数
<p id="fscript_parser_error_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#fscript_parser_error_t_fscript_parser_error_deinit">fscript\_parser\_error\_deinit</a> | 释放error对象中的资源。 |
### 属性
<p id="fscript_parser_error_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#fscript_parser_error_t_col">col</a> | int | 出现错误的代码列。 |
| <a href="#fscript_parser_error_t_message">message</a> | char* | 错误信息。 |
| <a href="#fscript_parser_error_t_offset">offset</a> | int | 出现错误的代码偏移。 |
| <a href="#fscript_parser_error_t_row">row</a> | int | 出现错误的代码行。 |
| <a href="#fscript_parser_error_t_token">token</a> | char* | 当前的token。 |
#### fscript\_parser\_error\_deinit 函数
-----------------------

* 函数功能：

> <p id="fscript_parser_error_t_fscript_parser_error_deinit">释放error对象中的资源。

* 函数原型：

```
ret_t fscript_parser_error_deinit (fscript_parser_error_t* error);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| error | fscript\_parser\_error\_t* | 解析错误信息。 |
#### col 属性
-----------------------
> <p id="fscript_parser_error_t_col">出现错误的代码列。

* 类型：int

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### message 属性
-----------------------
> <p id="fscript_parser_error_t_message">错误信息。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### offset 属性
-----------------------
> <p id="fscript_parser_error_t_offset">出现错误的代码偏移。

* 类型：int

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### row 属性
-----------------------
> <p id="fscript_parser_error_t_row">出现错误的代码行。

* 类型：int

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### token 属性
-----------------------
> <p id="fscript_parser_error_t_token">当前的token。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
