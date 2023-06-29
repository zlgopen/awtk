## fscript\_t
### 概述
一个简易的函数式脚本引擎。
用法请参考：https://github.com/zlgopen/awtk/blob/master/docs/fscript.md
----------------------------------
### 函数
<p id="fscript_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#fscript_t_fscript_clean">fscript\_clean</a> | 使用lua或其它脚本来运行fscript时，本函数用于清理不必要的数据结构。 |
| <a href="#fscript_t_fscript_create">fscript\_create</a> | 创建引擎对象，并解析代码。 |
| <a href="#fscript_t_fscript_create_ex">fscript\_create\_ex</a> | 创建引擎对象，并解析代码。 |
| <a href="#fscript_t_fscript_deinit">fscript\_deinit</a> | 清除引擎对象的数据。 |
| <a href="#fscript_t_fscript_destroy">fscript\_destroy</a> | 销毁引擎对象。 |
| <a href="#fscript_t_fscript_eval">fscript\_eval</a> | 执行一段脚本。 |
| <a href="#fscript_t_fscript_exec">fscript\_exec</a> | 执行解析后的代码。 |
| <a href="#fscript_t_fscript_get_global_object">fscript\_get\_global\_object</a> | 获取fscript的全局对象。 |
| <a href="#fscript_t_fscript_global_deinit">fscript\_global\_deinit</a> | 全局释放。 |
| <a href="#fscript_t_fscript_global_init">fscript\_global\_init</a> | 全局初始化。 |
| <a href="#fscript_t_fscript_init">fscript\_init</a> | 初始化引擎对象，并解析代码。 |
| <a href="#fscript_t_fscript_register_const_double">fscript\_register\_const\_double</a> | 注册浮点数常量。 |
| <a href="#fscript_t_fscript_register_const_int">fscript\_register\_const\_int</a> | 注册整数常量。 |
| <a href="#fscript_t_fscript_register_const_value">fscript\_register\_const\_value</a> | 注册常量。 |
| <a href="#fscript_t_fscript_register_func">fscript\_register\_func</a> | 注册全局自定义函数。 |
| <a href="#fscript_t_fscript_register_funcs">fscript\_register\_funcs</a> | 注册全局自定义函数。 |
| <a href="#fscript_t_fscript_reload">fscript\_reload</a> | 重新加载代码。 |
| <a href="#fscript_t_fscript_set_error">fscript\_set\_error</a> | 用于扩展函数设置遇到的错误。 |
| <a href="#fscript_t_fscript_set_global_object">fscript\_set\_global\_object</a> | 用于替换默认的全局对象。 |
| <a href="#fscript_t_fscript_set_on_error">fscript\_set\_on\_error</a> | 设置错误处理函数。 |
| <a href="#fscript_t_fscript_set_print_func">fscript\_set\_print\_func</a> | 设置打印日志的函数。 |
| <a href="#fscript_t_fscript_syntax_check">fscript\_syntax\_check</a> | 解析代码，分析是否有语法错误。 |
| <a href="#fscript_t_tk_expr_eval">tk\_expr\_eval</a> | 对fscript的简单包装。 |
### 属性
<p id="fscript_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#fscript_t_error_code">error\_code</a> | ret\_t | 运行时错误码。 |
| <a href="#fscript_t_error_col">error\_col</a> | int32\_t | 运行时错误的列号。 |
| <a href="#fscript_t_error_message">error\_message</a> | char* | 运行时错误信息。 |
| <a href="#fscript_t_error_row">error\_row</a> | int32\_t | 运行时错误的行号。 |
| <a href="#fscript_t_lines">lines</a> | uint16\_t | 代码总行数。 |
| <a href="#fscript_t_obj">obj</a> | tk\_object\_t* | 脚本执行上下文。 |
| <a href="#fscript_t_str">str</a> | str\_t | C语言实现函数可以使用这个变量，可以有效避免内存分配。 |
#### fscript\_clean 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_clean">使用lua或其它脚本来运行fscript时，本函数用于清理不必要的数据结构。

* 函数原型：

```
ret_t fscript_clean (fscript_t* fscript);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
#### fscript\_create 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_create">创建引擎对象，并解析代码。

* 函数原型：

```
fscript_t* fscript_create (tk_object_t* obj, const char* script);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | fscript\_t* | 返回fscript对象。 |
| obj | tk\_object\_t* | 脚本执行上下文。 |
| script | const char* | 脚本代码。 |
#### fscript\_create\_ex 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_create_ex">创建引擎对象，并解析代码。

* 函数原型：

```
fscript_t* fscript_create_ex (tk_object_t* obj, const char* script, bool_t keep_func_name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | fscript\_t* | 返回fscript对象。 |
| obj | tk\_object\_t* | 脚本执行上下文。 |
| script | const char* | 脚本代码。 |
| keep\_func\_name | bool\_t | 是否在func\_call结构后保存函数名。 |
#### fscript\_deinit 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_deinit">清除引擎对象的数据。

* 函数原型：

```
ret_t fscript_deinit (fscript_t* fscript);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
#### fscript\_destroy 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_destroy">销毁引擎对象。

* 函数原型：

```
ret_t fscript_destroy (fscript_t* fscript);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
#### fscript\_eval 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_eval">执行一段脚本。

* 函数原型：

```
ret_t fscript_eval (tk_object_t* obj, const char* script, value_t* result);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | 脚本执行上下文。 |
| script | const char* | 脚本代码。 |
| result | value\_t* | 执行结果(调用者需要用value\_reset函数清除result)。 |
#### fscript\_exec 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_exec">执行解析后的代码。

* 函数原型：

```
ret_t fscript_exec (fscript_t* fscript, value_t* result);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
| result | value\_t* | 执行结果(调用者需要用value\_reset函数清除result)。 |
#### fscript\_get\_global\_object 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_get_global_object">获取fscript的全局对象。

* 函数原型：

```
tk_object_t* fscript_get_global_object ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_object\_t* | 返回fscript的全局对象。 |
#### fscript\_global\_deinit 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_global_deinit">全局释放。

* 函数原型：

```
ret_t fscript_global_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_global\_init 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_global_init">全局初始化。

* 函数原型：

```
ret_t fscript_global_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### fscript\_init 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_init">初始化引擎对象，并解析代码。

* 函数原型：

```
fscript_t* fscript_init (fscript_t* fscript, tk_object_t* obj, const char* script, const char* first_call_name, bool_t keep_func_name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | fscript\_t* | 返回fscript对象。 |
| fscript | fscript\_t* | 初始化 fscript 对象。 |
| obj | tk\_object\_t* | 脚本执行上下文。 |
| script | const char* | 脚本代码。 |
| first\_call\_name | const char* | 第一个函数的名字。 |
| keep\_func\_name | bool\_t | 是否在func\_call结构后保存函数名。 |
#### fscript\_register\_const\_double 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_register_const_double">注册浮点数常量。

* 函数原型：

```
ret_t fscript_register_const_double (const char* name, double value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| name | const char* | 常量名。 |
| value | double | 数据。 |
#### fscript\_register\_const\_int 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_register_const_int">注册整数常量。

* 函数原型：

```
ret_t fscript_register_const_int (const char* name, int value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| name | const char* | 常量名。 |
| value | int | 数据。 |
#### fscript\_register\_const\_value 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_register_const_value">注册常量。

* 函数原型：

```
ret_t fscript_register_const_value (const char* name, const value_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| name | const char* | 常量名。 |
| value | const value\_t* | 数据。 |
#### fscript\_register\_func 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_register_func">注册全局自定义函数。

* 函数原型：

```
ret_t fscript_register_func (const char* name, fscript_func_t func);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| name | const char* | 函数名(无需加函数前缀)。 |
| func | fscript\_func\_t | 函数指针。 |
#### fscript\_register\_funcs 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_register_funcs">注册全局自定义函数。

* 函数原型：

```
ret_t fscript_register_funcs (const general_factory_table_t* table);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| table | const general\_factory\_table\_t* | 函数表。 |
#### fscript\_reload 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_reload">重新加载代码。

* 函数原型：

```
ret_t fscript_reload (fscript_t* fscript, const char* script);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
| script | const char* | 脚本代码。 |
#### fscript\_set\_error 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_set_error">用于扩展函数设置遇到的错误。

* 函数原型：

```
ret_t fscript_set_error (fscript_t* fscript, ret_t code, const char* func, const char* message);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
| code | ret\_t | 错误码。 |
| func | const char* | 函数名。 |
| message | const char* | 错误消息。 |
#### fscript\_set\_global\_object 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_set_global_object">用于替换默认的全局对象。
>仅限于在系统初始化时调用。

* 函数原型：

```
ret_t fscript_set_global_object (tk_object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | 全局对象。 |
#### fscript\_set\_on\_error 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_set_on_error">设置错误处理函数。

* 函数原型：

```
ret_t fscript_set_on_error (fscript_t* fscript, fscript_on_error_t on_error, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
| on\_error | fscript\_on\_error\_t | 错误处理函数。 |
| ctx | void* | 错误处理函数的上下文。 |
#### fscript\_set\_print\_func 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_set_print_func">设置打印日志的函数。

* 函数原型：

```
ret_t fscript_set_print_func (fscript_t* fscript, fscript_func_t print);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
| print | fscript\_func\_t | 打印日志的函数。 |
#### fscript\_syntax\_check 函数
-----------------------

* 函数功能：

> <p id="fscript_t_fscript_syntax_check">解析代码，分析是否有语法错误。

示例：
```c
fscript_parser_error_t error;
fscript_syntax_check(obj, "1+1", &error);
fscript_parser_error_deinit(&error);
```

* 函数原型：

```
ret_t fscript_syntax_check (tk_object_t* obj, const char* script, fscript_parser_error_t* error);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | tk\_object\_t* | 脚本执行上下文。 |
| script | const char* | 脚本代码。 |
| error | fscript\_parser\_error\_t* | 用于返回错误信息。 |
#### tk\_expr\_eval 函数
-----------------------

* 函数功能：

> <p id="fscript_t_tk_expr_eval">对fscript的简单包装。

* 函数原型：

```
double tk_expr_eval (const char* expr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回表达式的值。 |
| expr | const char* | 表达式。 |
#### error\_code 属性
-----------------------
> <p id="fscript_t_error_code">运行时错误码。

* 类型：ret\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### error\_col 属性
-----------------------
> <p id="fscript_t_error_col">运行时错误的列号。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### error\_message 属性
-----------------------
> <p id="fscript_t_error_message">运行时错误信息。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### error\_row 属性
-----------------------
> <p id="fscript_t_error_row">运行时错误的行号。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### lines 属性
-----------------------
> <p id="fscript_t_lines">代码总行数。

* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### obj 属性
-----------------------
> <p id="fscript_t_obj">脚本执行上下文。

* 类型：tk\_object\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### str 属性
-----------------------
> <p id="fscript_t_str">C语言实现函数可以使用这个变量，可以有效避免内存分配。

* 类型：str\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
