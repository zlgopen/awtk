## log\_t
### 概述
log。
----------------------------------
### 函数
<p id="log_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#log_t_log_debug">log\_debug</a> | 输出调试信息。 |
| <a href="#log_t_log_error">log\_error</a> | 输出错误信息。 |
| <a href="#log_t_log_get_log_level">log\_get\_log\_level</a> | 获取log的级别。 |
| <a href="#log_t_log_info">log\_info</a> | 输出参考信息。 |
| <a href="#log_t_log_set_log_level">log\_set\_log\_level</a> | 设置log的级别。 |
| <a href="#log_t_log_warn">log\_warn</a> | 输出警告信息。 |
#### log\_debug 函数
-----------------------

* 函数功能：

> <p id="log_t_log_debug">输出调试信息。
> 变参函数。

```
log_debug("debug message\n");
```

* 函数原型：

```
void log_debug (const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 无返回值。 |
| format | const char* | 格式或信息。 |
#### log\_error 函数
-----------------------

* 函数功能：

> <p id="log_t_log_error">输出错误信息。
> 变参函数。

```
log_error("error message\n");
```

* 函数原型：

```
void log_error (const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 无返回值。 |
| format | const char* | 格式或信息。 |
#### log\_get\_log\_level 函数
-----------------------

* 函数功能：

> <p id="log_t_log_get_log_level">获取log的级别。

* 函数原型：

```
log_level_t log_get_log_level ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | log\_level\_t | 返回log的级别。 |
#### log\_info 函数
-----------------------

* 函数功能：

> <p id="log_t_log_info">输出参考信息。
> 变参函数。

```
log_info("info message\n");
```

* 函数原型：

```
void log_info (const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 无返回值。 |
| format | const char* | 格式或信息。 |
#### log\_set\_log\_level 函数
-----------------------

* 函数功能：

> <p id="log_t_log_set_log_level">设置log的级别。

* 函数原型：

```
ret_t log_set_log_level (log_level_t log_level);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| log\_level | log\_level\_t | log的级别。 |
#### log\_warn 函数
-----------------------

* 函数功能：

> <p id="log_t_log_warn">输出警告信息。
> 变参函数。

```
log_warn("warn message\n");
```

* 函数原型：

```
void log_warn (const char* format);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 无返回值。 |
| format | const char* | 格式或信息。 |
