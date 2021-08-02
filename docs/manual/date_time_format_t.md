## date\_time\_format\_t
### 概述
格式化日期时间。

格式规则：
* Y 代表年(完整显示)
* M 代表月(1-12)
* D 代表日(1-31)
* h 代表时(0-23)
* m 代表分(0-59)
* s 代表秒(0-59)
* w 代表星期(0-6)
* W 代表星期的英文缩写(支持翻译)
* YY 代表年(只显示末两位)
* MM 代表月(01-12)
* DD 代表日(01-31)
* hh 代表时(00-23)
* mm 代表分(00-59)
* ss 代表秒(00-59)
* MMM 代表月的英文缩写(支持翻译)

如 日期时间为：2018/11/12 9:10:20
* "Y/M/D"显示为"2018/11/12"
* "Y-M-D"显示为"2018-11-12"
* "Y-M-D h:m:s"显示为"2018-11-12 9:10:20"
* "Y-M-D hh:mm:ss"显示为"2018-11-12 09:10:20"
----------------------------------
### 函数
<p id="date_time_format_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#date_time_format_t_wstr_format_date_time">wstr\_format\_date\_time</a> | 格式化date_time。 |
| <a href="#date_time_format_t_wstr_format_time">wstr\_format\_time</a> | 格式化time。 |
#### wstr\_format\_date\_time 函数
-----------------------

* 函数功能：

> <p id="date_time_format_t_wstr_format_date_time">格式化date_time。

* 函数原型：

```
ret_t wstr_format_date_time (wstr_t* str, const char* format, date_time_t* dt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | 输出字符串。 |
| format | const char* | 格式。 |
| dt | date\_time\_t* | 时间。 |
#### wstr\_format\_time 函数
-----------------------

* 函数功能：

> <p id="date_time_format_t_wstr_format_time">格式化time。

* 函数原型：

```
ret_t wstr_format_time (wstr_t* str, const char* format, uint64_t time);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| str | wstr\_t* | 输出字符串。 |
| format | const char* | 格式。 |
| time | uint64\_t | 时间。 |
