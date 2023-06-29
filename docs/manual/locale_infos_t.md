## locale\_infos\_t
### 概述
在某些情况下，需要多个资源管理器。比如在手表系统里，每个应用或表盘，可能放在独立的资源包中，
此时优先加载应用自己的资源，如果没有就加载系统的资源。
----------------------------------
### 函数
<p id="locale_infos_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#locale_infos_t_locale_infos_change">locale\_infos\_change</a> | 设置全部locale_info的当前国家和语言。 |
| <a href="#locale_infos_t_locale_infos_off">locale\_infos\_off</a> | 注销指定事件的处理函数。 |
| <a href="#locale_infos_t_locale_infos_on">locale\_infos\_on</a> | 注册指定事件的处理函数。 |
| <a href="#locale_infos_t_locale_infos_ref">locale\_infos\_ref</a> | 获取指定小应用程序(applet)的locale_info。 |
| <a href="#locale_infos_t_locale_infos_reload_all">locale\_infos\_reload\_all</a> | 重新加载全部字符串资源。 |
| <a href="#locale_infos_t_locale_infos_unref">locale\_infos\_unref</a> | 释放指定小应用程序(applet)的locale_info。 |
#### locale\_infos\_change 函数
-----------------------

* 函数功能：

> <p id="locale_infos_t_locale_infos_change">设置全部locale_info的当前国家和语言。

* 函数原型：

```
ret_t locale_infos_change (const char* language, const char* country);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| language | const char* | 语言。 |
| country | const char* | 国家或地区。 |
#### locale\_infos\_off 函数
-----------------------

* 函数功能：

> <p id="locale_infos_t_locale_infos_off">注销指定事件的处理函数。

* 函数原型：

```
ret_t locale_infos_off (uint32_t id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| id | uint32\_t | locale\_infos\_on返回的ID。 |
#### locale\_infos\_on 函数
-----------------------

* 函数功能：

> <p id="locale_infos_t_locale_infos_on">注册指定事件的处理函数。

* 函数原型：

```
uint32_t locale_infos_on (event_type_t type, event_func_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回id，用于locale\_infos\_off。 |
| type | event\_type\_t | 事件类型，目前有EVT\_LOCALE\_INFOS\_LOAD\_INFO、EVT\_LOCALE\_INFOS\_UNLOAD\_INFO。 |
| on\_event | event\_func\_t | 事件处理函数。 |
| ctx | void* | 事件处理函数上下文。 |
#### locale\_infos\_ref 函数
-----------------------

* 函数功能：

> <p id="locale_infos_t_locale_infos_ref">获取指定小应用程序(applet)的locale_info。

* 函数原型：

```
locale_info_t* locale_infos_ref (const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | locale\_info\_t* | 返回locale\_info对象。 |
| name | const char* | 小应用程序(applet)的名称。 |
#### locale\_infos\_reload\_all 函数
-----------------------

* 函数功能：

> <p id="locale_infos_t_locale_infos_reload_all">重新加载全部字符串资源。

* 函数原型：

```
ret_t locale_infos_reload_all ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### locale\_infos\_unref 函数
-----------------------

* 函数功能：

> <p id="locale_infos_t_locale_infos_unref">释放指定小应用程序(applet)的locale_info。

* 函数原型：

```
ret_t locale_infos_unref (locale_info_t* locale_info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| locale\_info | locale\_info\_t* | locale\_info对象。 |
