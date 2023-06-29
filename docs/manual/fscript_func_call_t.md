## fscript\_func\_call\_t
### 概述
函数描述。
----------------------------------
### 函数
<p id="fscript_func_call_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#fscript_func_call_t_fscript_ensure_locals">fscript\_ensure\_locals</a> | export for debugger |
| <a href="#fscript_func_call_t_fscript_exec_func_default">fscript\_exec\_func\_default</a> | 执行函数的默认实现。 |
| <a href="#fscript_func_call_t_fscript_find_func">fscript\_find\_func</a> | 查找函数。 |
| <a href="#fscript_func_call_t_fscript_get_code_id">fscript\_get\_code\_id</a> | 获取code_id。 |
| <a href="#fscript_func_call_t_fscript_set_hooks">fscript\_set\_hooks</a> | 设置回调函数。 |
| <a href="#fscript_func_call_t_fscript_set_self_hooks">fscript\_set\_self\_hooks</a> | 设置 fscript 对象的回调函数。 |
| <a href="#fscript_func_call_t_fscript_set_var_default">fscript\_set\_var\_default</a> | 设置变量的默认实现。 |
### 属性
<p id="fscript_func_call_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#fscript_func_call_t_col">col</a> | uint16\_t | 对应源代码列号。 |
| <a href="#fscript_func_call_t_ctx">ctx</a> | void* | 函数需要的上下文。 |
| <a href="#fscript_func_call_t_func">func</a> | fscript\_func\_t | 函数指针。 |
| <a href="#fscript_func_call_t_row">row</a> | uint16\_t | 对应源代码行号。 |
#### fscript\_ensure\_locals 函数
-----------------------

* 函数功能：

> <p id="fscript_func_call_t_fscript_ensure_locals">export for debugger

* 函数原型：

```
ret_t fscript_ensure_locals (fscript_t* fscript);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
#### fscript\_exec\_func\_default 函数
-----------------------

* 函数功能：

> <p id="fscript_func_call_t_fscript_exec_func_default">执行函数的默认实现。

* 函数原型：

```
ret_t fscript_exec_func_default (fscript_t* fscript, fscript_func_call_t* iter, value_t* result);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
| iter | fscript\_func\_call\_t* | 当前函数。 |
| result | value\_t* | 返回结果。 |
#### fscript\_find\_func 函数
-----------------------

* 函数功能：

> <p id="fscript_func_call_t_fscript_find_func">查找函数。

* 函数原型：

```
fscript_func_t fscript_find_func (fscript_t* fscript, const char* name, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | fscript\_func\_t | 返回函数指针，NULL表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
| name | const char* | 函数名。 |
| size | uint32\_t | 函数名长度。 |
#### fscript\_get\_code\_id 函数
-----------------------

* 函数功能：

> <p id="fscript_func_call_t_fscript_get_code_id">获取code_id。

* 函数原型：

```
char* fscript_get_code_id (const char* str);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | char* | 返回code\_id，有调用者释放返回的字符串。 |
| str | const char* | 代码。 |
#### fscript\_set\_hooks 函数
-----------------------

* 函数功能：

> <p id="fscript_func_call_t_fscript_set_hooks">设置回调函数。

* 函数原型：

```
ret_t fscript_set_hooks (const fscript_hooks_t* hooks);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| hooks | const fscript\_hooks\_t* | 回调函数。 |
#### fscript\_set\_self\_hooks 函数
-----------------------

* 函数功能：

> <p id="fscript_func_call_t_fscript_set_self_hooks">设置 fscript 对象的回调函数。

* 函数原型：

```
ret_t fscript_set_self_hooks (fscript_t* fscript, const fscript_hooks_t* hooks);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | fscript 对象。 |
| hooks | const fscript\_hooks\_t* | 回调函数。 |
#### fscript\_set\_var\_default 函数
-----------------------

* 函数功能：

> <p id="fscript_func_call_t_fscript_set_var_default">设置变量的默认实现。

* 函数原型：

```
ret_t fscript_set_var_default (fscript_t* fscript, const char* name, const value_t* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fscript | fscript\_t* | 脚本引擎对象。 |
| name | const char* | 变量名。 |
| value | const value\_t* | 值。 |
#### col 属性
-----------------------
> <p id="fscript_func_call_t_col">对应源代码列号。

* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### ctx 属性
-----------------------
> <p id="fscript_func_call_t_ctx">函数需要的上下文。
>目前主要保持自定义函数的实现。

* 类型：void*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### func 属性
-----------------------
> <p id="fscript_func_call_t_func">函数指针。

* 类型：fscript\_func\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### row 属性
-----------------------
> <p id="fscript_func_call_t_row">对应源代码行号。

* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
