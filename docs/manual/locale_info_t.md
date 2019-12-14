## locale\_info\_t
### 概述
本地化信息。提供字符串翻译数据管理，当前语言改变的事件通知等等。
----------------------------------
### 函数
<p id="locale_info_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#locale_info_t_locale_info">locale\_info</a> | 获取缺省locale_info。 |
| <a href="#locale_info_t_locale_info_change">locale\_info\_change</a> | 设置当前的国家和语言。 |
| <a href="#locale_info_t_locale_info_create">locale\_info\_create</a> | 创建locale_info。 |
| <a href="#locale_info_t_locale_info_destroy">locale\_info\_destroy</a> | 释放全部资源并销毁locale_info对象。 |
| <a href="#locale_info_t_locale_info_off">locale\_info\_off</a> | 注销指定事件的处理函数。 |
| <a href="#locale_info_t_locale_info_on">locale\_info\_on</a> | 注册指定事件的处理函数。 |
| <a href="#locale_info_t_locale_info_reload">locale\_info\_reload</a> | 重新加载字符串资源。 |
| <a href="#locale_info_t_locale_info_set">locale\_info\_set</a> | 设置缺省locale_info。 |
| <a href="#locale_info_t_locale_info_set_assets_manager">locale\_info\_set\_assets\_manager</a> | 设置资源管理器对象。 |
| <a href="#locale_info_t_locale_info_tr">locale\_info\_tr</a> | 翻译字符串。 |
### 属性
<p id="locale_info_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#locale_info_t_country;">country;</a> | char* | 国家或地区。如：CN |
| <a href="#locale_info_t_language">language</a> | char* | 语言。如：zh |
#### locale\_info 函数
-----------------------

* 函数功能：

> <p id="locale_info_t_locale_info">获取缺省locale_info。

* 函数原型：

```
locale_info_t* locale_info ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | locale\_info\_t* | 返回locale\_info对象。 |
#### locale\_info\_change 函数
-----------------------

* 函数功能：

> <p id="locale_info_t_locale_info_change">设置当前的国家和语言。

* 函数原型：

```
ret_t locale_info_change (locale_info_t* locale_info, char* language, char* country);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| locale\_info | locale\_info\_t* | locale\_info对象。 |
| language | char* | 语言。 |
| country | char* | 国家或地区。 |
#### locale\_info\_create 函数
-----------------------

* 函数功能：

> <p id="locale_info_t_locale_info_create">创建locale_info。

* 函数原型：

```
locale_info_t* locale_info_create (char* language, char* country);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | locale\_info\_t* | 返回locale\_info对象。 |
| language | char* | 语言。 |
| country | char* | 国家或地区。 |
#### locale\_info\_destroy 函数
-----------------------

* 函数功能：

> <p id="locale_info_t_locale_info_destroy">释放全部资源并销毁locale_info对象。

* 函数原型：

```
ret_t locale_info_destroy (locale_info_t* locale_info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| locale\_info | locale\_info\_t* | locale\_info对象。 |
#### locale\_info\_off 函数
-----------------------

* 函数功能：

> <p id="locale_info_t_locale_info_off">注销指定事件的处理函数。

* 函数原型：

```
ret_t locale_info_off (locale_info_t* locale_info, uint32_t id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| locale\_info | locale\_info\_t* | locale\_info对象。 |
| id | uint32\_t | locale\_info\_on返回的ID。 |
#### locale\_info\_on 函数
-----------------------

* 函数功能：

> <p id="locale_info_t_locale_info_on">注册指定事件的处理函数。

* 函数原型：

```
uint32_t locale_info_on (locale_info_t* locale_info, event_type_t type, event_func_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回id，用于locale\_info\_off。 |
| locale\_info | locale\_info\_t* | locale\_info对象。 |
| type | event\_type\_t | 事件类型，目前固定为EVT\_LOCALE\_CHANGED。 |
| on\_event | event\_func\_t | 事件处理函数。 |
| ctx | void* | 事件处理函数上下文。 |
#### locale\_info\_reload 函数
-----------------------

* 函数功能：

> <p id="locale_info_t_locale_info_reload">重新加载字符串资源。

* 函数原型：

```
ret_t locale_info_reload (locale_info_t* locale_info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| locale\_info | locale\_info\_t* | locale\_info对象。 |
#### locale\_info\_set 函数
-----------------------

* 函数功能：

> <p id="locale_info_t_locale_info_set">设置缺省locale_info。

* 函数原型：

```
ret_t locale_info_set (locale_info_t* locale_info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| locale\_info | locale\_info\_t* | locale\_info对象。 |
#### locale\_info\_set\_assets\_manager 函数
-----------------------

* 函数功能：

> <p id="locale_info_t_locale_info_set_assets_manager">设置资源管理器对象。

* 函数原型：

```
ret_t locale_info_set_assets_manager (locale_info_t* locale_info, assets_manager_t* assets_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| locale\_info | locale\_info\_t* | locale\_info对象。 |
| assets\_manager | assets\_manager\_t* | 资源管理器。 |
#### locale\_info\_tr 函数
-----------------------

* 函数功能：

> <p id="locale_info_t_locale_info_tr">翻译字符串。

* 函数原型：

```
const char* locale_info_tr (locale_info_t* locale_info, const char* text);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回翻译之后的字符串。 |
| locale\_info | locale\_info\_t* | locale\_info对象。 |
| text | const char* | 待翻译的文本。 |
#### country; 属性
-----------------------
> <p id="locale_info_t_country;">国家或地区。如：CN

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### language 属性
-----------------------
> <p id="locale_info_t_language">语言。如：zh

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
