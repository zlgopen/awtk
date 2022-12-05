## debugger\_factory\_t
### 概述
@annotaion ["fake"]
调试器工厂
----------------------------------
### 函数
<p id="debugger_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#debugger_factory_t_debugger_factory_attach_debugger">debugger\_factory\_attach\_debugger</a> | 创建调试器。 |
| <a href="#debugger_factory_t_debugger_factory_deinit">debugger\_factory\_deinit</a> | 释放资源。 |
| <a href="#debugger_factory_t_debugger_factory_init">debugger\_factory\_init</a> | 初始化工厂。 |
| <a href="#debugger_factory_t_debugger_factory_launch_debugger">debugger\_factory\_launch\_debugger</a> | 创建调试器。 |
| <a href="#debugger_factory_t_debugger_factory_reg">debugger\_factory\_reg</a> | 注册创建函数。 |
#### debugger\_factory\_attach\_debugger 函数
-----------------------

* 函数功能：

> <p id="debugger_factory_t_debugger_factory_attach_debugger">创建调试器。

* 函数原型：

```
debugger_t* debugger_factory_attach_debugger (const char* lang, const char* code_id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_t* | 返回调试器对象。 |
| lang | const char* | 语言类型。 |
| code\_id | const char* | 代码的ID。 |
#### debugger\_factory\_deinit 函数
-----------------------

* 函数功能：

> <p id="debugger_factory_t_debugger_factory_deinit">释放资源。

* 函数原型：

```
ret_t debugger_factory_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### debugger\_factory\_init 函数
-----------------------

* 函数功能：

> <p id="debugger_factory_t_debugger_factory_init">初始化工厂。

* 函数原型：

```
ret_t debugger_factory_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### debugger\_factory\_launch\_debugger 函数
-----------------------

* 函数功能：

> <p id="debugger_factory_t_debugger_factory_launch_debugger">创建调试器。

* 函数原型：

```
debugger_t* debugger_factory_launch_debugger (const char* lang, const binary_data_t* code);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | debugger\_t* | 返回调试器对象。 |
| lang | const char* | 语言类型。 |
| code | const binary\_data\_t* | 代码。 |
#### debugger\_factory\_reg 函数
-----------------------

* 函数功能：

> <p id="debugger_factory_t_debugger_factory_reg">注册创建函数。

* 函数原型：

```
ret_t debugger_factory_reg (const char* lang, debugger_fscript_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| lang | const char* | 语言类型。 |
| create | debugger\_fscript\_create\_t | 创建函数。 |
