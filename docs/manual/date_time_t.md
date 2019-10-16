## date\_time\_t
### 概述
 日期时间。

> 在嵌入式平台中，在系统初始时，需要调用date\_time\_set\_impl设置实际获取系统时间的函数。


----------------------------------
### 函数
<p id="date_time_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#date_time_t_date_time_create">date\_time\_create</a> | 创建date_time对象，并初始为当前日期和时间(一般供脚本语言中使用)。 |
| <a href="#date_time_t_date_time_destroy">date\_time\_destroy</a> | 销毁date_time对象(一般供脚本语言中使用)。 |
| <a href="#date_time_t_date_time_init">date\_time\_init</a> | 初始为当前日期和时间。 |
| <a href="#date_time_t_date_time_set_impl">date\_time\_set\_impl</a> | 设置获取当前日期和时间的函数。 |
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

> <p id="date_time_t_date_time_create"> 创建date_time对象，并初始为当前日期和时间(一般供脚本语言中使用)。



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

> <p id="date_time_t_date_time_destroy"> 销毁date_time对象(一般供脚本语言中使用)。




* 函数原型：

```
ret_t date_time_destroy (date_time_t* dt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dt | date\_time\_t* | date\_time对象。 |
#### date\_time\_init 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_init"> 初始为当前日期和时间。



* 函数原型：

```
date_time_t* date_time_init (date_time_t* dt);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | date\_time\_t* | 返回date\_time对象。 |
| dt | date\_time\_t* | date\_time对象。 |
#### date\_time\_set\_impl 函数
-----------------------

* 函数功能：

> <p id="date_time_t_date_time_set_impl"> 设置获取当前日期和时间的函数。

 > 嵌入式平台需要提供并设置获取当前日期和时间的函数，否则相关的功能(如时钟控件)将无法正常工作。




* 函数原型：

```
ret_t date_time_set_impl (date_time_get_now_t date_time_get_now);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| date\_time\_get\_now | date\_time\_get\_now\_t | 获取当前日期和时间的函数。 |
#### day 属性
-----------------------
> <p id="date_time_t_day"> 日(1-31)。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### hour 属性
-----------------------
> <p id="date_time_t_hour"> 时(0 - 23)。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### minute 属性
-----------------------
> <p id="date_time_t_minute"> 分(0 - 59)。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### month 属性
-----------------------
> <p id="date_time_t_month"> 月(1-12)。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### second 属性
-----------------------
> <p id="date_time_t_second"> 秒(0 - 59)。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### wday 属性
-----------------------
> <p id="date_time_t_wday"> 星期几(0-6, Sunday = 0)。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### year 属性
-----------------------
> <p id="date_time_t_year"> 年。


* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
