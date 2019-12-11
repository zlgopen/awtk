## platform\_t
### 概述
平台接口，包括：获取时间、休眠等函数。
----------------------------------
### 函数
<p id="platform_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#platform_t_get_time_ms64">get\_time\_ms64</a> | 获取当前时间。 |
| <a href="#platform_t_platform_prepare">platform\_prepare</a> | 平台准备函数。 |
| <a href="#platform_t_sleep_ms">sleep\_ms</a> | 睡眠指定时间。 |
#### get\_time\_ms64 函数
-----------------------

* 函数功能：

> <p id="platform_t_get_time_ms64">获取当前时间。

* 函数原型：

```
uint64_t get_time_ms64 ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint64\_t | 成功返回当前时间。 |
#### platform\_prepare 函数
-----------------------

* 函数功能：

> <p id="platform_t_platform_prepare">平台准备函数。

* 函数原型：

```
ret_t platform_prepare ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### sleep\_ms 函数
-----------------------

* 函数功能：

> <p id="platform_t_sleep_ms">睡眠指定时间。

* 函数原型：

```
void sleep_ms (uint32_t ms);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 无。 |
| ms | uint32\_t | 睡眠时间(毫秒)。 |
