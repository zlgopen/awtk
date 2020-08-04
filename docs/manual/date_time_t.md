## date\_time\_t
### 概述
日期时间。

> 在嵌入式平台中，在系统初始时，需要调用date\_time\_global\_init设置实际获取/设置系统时间的函数。
----------------------------------
### 函数
<p id="date_time_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#date_time_t_date_time_create">date\_time\_create</a> | 创建date_time对象，并初始为当前日期和时间(一般供脚本语言中使用)。 |
| <a href="#date_time_t_date_time_destroy">date\_time\_destroy</a> | 销毁date_time对象(一般供脚本语言中使用)。 |
| <a href="#date_time_t_date_time_from_time">date\_time\_from\_time</a> | 从time转换而来。 |
| <a href="#date_time_t_date_time_get_days">date\_time\_get\_days</a> | 获取指定年份月份的天数。 |
| <a href="#date_time_t_date_time_get_month_name">date\_time\_get\_month\_name</a> | 获取指定月份的英文名称(简写)。 |
| <a href="#date_time_t_date_time_get_wday">date\_time\_get\_wday</a> | 获取指定日期是周几(0-6, Sunday = 0)。。 |
| <a href="#date_time_t_date_time_get_wday_name">date\_time\_get\_wday\_name</a> | 获取周几的英文名称(简写)。 |
| <a href="#date_time_t_date_time_global_init_ex">date\_time\_global\_init\_ex</a> | 时间日期全局初始化。 |
| <a href="#date_time_t_date_time_init">date\_time\_init</a> | 初始为当前日期和时间。 |
| <a href="#date_time_t_date_time_is_leap">date\_time\_is\_leap</a> | 是否是闰年。 |
| <a href="#date_time_t_date_time_set">date\_time\_set</a> | 设置当前时间。 |
### 属性
<p id="date_time_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#date_time_t_day">day</a> | int32\_t | 日(1-31)。 |
| <a href="#date_time_t_hour">hour</a> | int32\_t | 时(0 - 23)。 |
| <a href="#date_time_t_minute">minute</a> | int32\_t | 分(0 - 59)。 |
| <a href="#date_time_t_month">month</a> | int32\_t | 月(1-12)。 |
| <a href="#date_time_t_second">second</a> | int32\_t | 秒(0 - 59)。 |
| <a href="#date_time_t_wday">wday</a> | int32\_t | 星期几(0-6, Sunday = 0)。 |
| <a href="#date_time_t_year">year</a> | int32\_t | 年。 |
#### date\_time\_create 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_create">创建date_time对象，并初始为当前日期和时间(一般供脚本语言中使用)。

* 函数原型：

```
date_time_t* date_time_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | date\_time\_t* | 返回date\_time对象。 |
#### date\_time\_destroy 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_destroy">销毁date_time对象(一般供脚本语言中使用)。

* 函数原型：

```
ret_t date_time_destroy (date_time_t* dt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dt | date\_time\_t* | date\_time对象。 |
#### date\_time\_from\_time 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_from_time">从time转换而来。

* 函数原型：

```
ret_t date_time_from_time (date_time_t* dt, uint64_t time);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dt | date\_time\_t* | date\_time对象。 |
| time | uint64\_t | 时间。 |
#### date\_time\_get\_days 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_get_days">获取指定年份月份的天数。

* 函数原型：

```
int32_t date_time_get_days (uint32_t year, uint32_t montn);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回大于0表示天数，否则表示失败。 |
| year | uint32\_t | 年份。 |
| montn | uint32\_t | 月份(1-12)。 |
#### date\_time\_get\_month\_name 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_get_month_name">获取指定月份的英文名称(简写)。

* 函数原型：

```
const char* date_time_get_month_name (uint32_t montn);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回指定月份的英文名称(简写)。 |
| montn | uint32\_t | 月份(1-12)。 |
#### date\_time\_get\_wday 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_get_wday">获取指定日期是周几(0-6, Sunday = 0)。。

* 函数原型：

```
int32_t date_time_get_wday (uint32_t year, uint32_t montn, uint32_t day);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回大于等于0表示周几(0-6)，否则表示失败。 |
| year | uint32\_t | 年份。 |
| montn | uint32\_t | 月份(1-12)。 |
| day | uint32\_t | 日(1-31)。 |
#### date\_time\_get\_wday\_name 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_get_wday_name">获取周几的英文名称(简写)。

* 函数原型：

```
const char* date_time_get_wday_name (uint32_t wday);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回指定周几的英文名称(简写)。 |
| wday | uint32\_t | 星期几(0-6, Sunday = 0)。 |
#### date\_time\_global\_init\_ex 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_global_init_ex">时间日期全局初始化。

> 嵌入式平台需要提供并设置获取当前日期和时间的函数，否则相关的功能(如时钟控件)将无法正常工作。

* 函数原型：

```
ret_t date_time_global_init_ex (date_time_vtable_t vt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vt | date\_time\_vtable\_t | 日期和时间的相关函数的实现。 |
#### date\_time\_init 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_init">初始为当前日期和时间。

* 函数原型：

```
date_time_t* date_time_init (date_time_t* dt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | date\_time\_t* | 返回date\_time对象。 |
| dt | date\_time\_t* | date\_time对象。 |
#### date\_time\_is\_leap 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_is_leap">是否是闰年。

* 函数原型：

```
bool_t date_time_is_leap (uint32_t year);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示是，否则表示否。 |
| year | uint32\_t | 年份。 |
#### date\_time\_set 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_set">设置当前时间。

* 函数原型：

```
ret_t date_time_set (date_time_t* dt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dt | date\_time\_t* | date\_time对象。 |
#### day 属性
-----------------------
> <p id="date_time_t_day">日(1-31)。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### hour 属性
-----------------------
> <p id="date_time_t_hour">时(0 - 23)。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### minute 属性
-----------------------
> <p id="date_time_t_minute">分(0 - 59)。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### month 属性
-----------------------
> <p id="date_time_t_month">月(1-12)。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### second 属性
-----------------------
> <p id="date_time_t_second">秒(0 - 59)。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### wday 属性
-----------------------
> <p id="date_time_t_wday">星期几(0-6, Sunday = 0)。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### year 属性
-----------------------
> <p id="date_time_t_year">年。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
