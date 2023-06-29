## debugger\_fscript\_t
### 概述
fscript调试器
----------------------------------
### 函数
<p id="debugger_fscript_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#debugger_fscript_t_debugger_fscript_cast">debugger\_fscript\_cast</a> | 类型转换。 |
| <a href="#debugger_fscript_t_debugger_fscript_create">debugger\_fscript\_create</a> | 创建调试器对象。 |
| <a href="#debugger_fscript_t_debugger_fscript_get_start_line">debugger\_fscript\_get\_start\_line</a> | 获取第一行被执行的代码的行号。 |
| <a href="#debugger_fscript_t_debugger_fscript_set_code">debugger\_fscript\_set\_code</a> | 设置代码。 |
| <a href="#debugger_fscript_t_debugger_fscript_set_fscript">debugger\_fscript\_set\_fscript</a> | 设置fscript对象。 |
#### debugger\_fscript\_cast 函数
-----------------------

* 函数功能：

> <p id="debugger_fscript_t_debugger_fscript_cast">类型转换。

* 函数原型：

```
debugger_fscript_t* debugger_fscript_cast (debugger_t* debugger);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_fscript\_t* | 返回debugger对象。 |
| debugger | debugger\_t* | debugger对象。 |
#### debugger\_fscript\_create 函数
-----------------------

* 函数功能：

> <p id="debugger_fscript_t_debugger_fscript_create">创建调试器对象。

* 函数原型：

```
debugger_t* debugger_fscript_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_t* | 返回debugger对象。 |
#### debugger\_fscript\_get\_start\_line 函数
-----------------------

* 函数功能：

> <p id="debugger_fscript_t_debugger_fscript_get_start_line">获取第一行被执行的代码的行号。

* 函数原型：

```
int32_t debugger_fscript_get_start_line (fscript_t* fscript);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回行号。 |
| fscript | fscript\_t* | fscript对象。 |
#### debugger\_fscript\_set\_code 函数
-----------------------

* 函数功能：

> <p id="debugger_fscript_t_debugger_fscript_set_code">设置代码。

* 函数原型：

```
ret_t debugger_fscript_set_code (debugger_t* debugger, const binary_data_t* code, bool_t changed);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| code | const binary\_data\_t* | 代码。 |
| changed | bool\_t | 是否重新加载。 |
#### debugger\_fscript\_set\_fscript 函数
-----------------------

* 函数功能：

> <p id="debugger_fscript_t_debugger_fscript_set_fscript">设置fscript对象。

* 函数原型：

```
ret_t debugger_fscript_set_fscript (debugger_t* debugger, fscript_t* fscript);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| debugger | debugger\_t* | debugger对象。 |
| fscript | fscript\_t* | 脚本对象。 |
