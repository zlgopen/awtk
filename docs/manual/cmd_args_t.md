## cmd\_args\_t
### 概述
命令行处理。
----------------------------------
### 函数
<p id="cmd_args_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#cmd_args_t_cmd_args_init">cmd\_args\_init</a> | 初始化。 |
| <a href="#cmd_args_t_cmd_args_process">cmd\_args\_process</a> | 处理。 |
| <a href="#cmd_args_t_cmd_args_process_str">cmd\_args\_process\_str</a> | 处理。 |
| <a href="#cmd_args_t_cmd_args_show_help">cmd\_args\_show\_help</a> | 显示帮助信息。 |
#### cmd\_args\_init 函数
-----------------------

* 函数功能：

> <p id="cmd_args_t_cmd_args_init">初始化。

* 函数原型：

```
cmd_args_t* cmd_args_init (cmd_args_t* args, const char* usage, const cmd_arg_desc_t* desc, uint32_t desc_nr, cmd_arg_on_arg_t on_arg, void* on_arg_ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | cmd\_args\_t* | 返回CMD\_ARGS对象。 |
| args | cmd\_args\_t* | args对象。 |
| usage | const char* | 帮助信息。 |
| desc | const cmd\_arg\_desc\_t* | 参数描述。 |
| desc\_nr | uint32\_t | 参数描述个数。 |
| on\_arg | cmd\_arg\_on\_arg\_t | 参数处理回调函数。 |
| on\_arg\_ctx | void* | 参数处理回调函数的上下文。 |
#### cmd\_args\_process 函数
-----------------------

* 函数功能：

> <p id="cmd_args_t_cmd_args_process">处理。

* 函数原型：

```
ret_t cmd_args_process (cmd_args_t* args, int argc, char** argv);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| args | cmd\_args\_t* | args对象。 |
| argc | int | 参数个数。 |
| argv | char** | 参数数组。 |
#### cmd\_args\_process\_str 函数
-----------------------

* 函数功能：

> <p id="cmd_args_t_cmd_args_process_str">处理。

* 函数原型：

```
ret_t cmd_args_process_str (cmd_args_t* args, const char* cmd_line);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| args | cmd\_args\_t* | args对象。 |
| cmd\_line | const char* | 字符串格式的参数(适用于Windows)。 |
#### cmd\_args\_show\_help 函数
-----------------------

* 函数功能：

> <p id="cmd_args_t_cmd_args_show_help">显示帮助信息。

* 函数原型：

```
ret_t cmd_args_show_help (cmd_args_t* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| args | cmd\_args\_t* | args对象。 |
