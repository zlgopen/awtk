## app\_conf\_t
### 概述
应用程序的配置信息。

底层实现可以是任何格式，比如INI，XML，JSON和UBJSON。

对于树状的文档，key可以是多级的，用.分隔。如network.ip。
----------------------------------
### 函数
<p id="app_conf_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#app_conf_t_app_conf_deinit">app\_conf\_deinit</a> | 释放conf对象。 |
| <a href="#app_conf_t_app_conf_exist">app\_conf\_exist</a> | 检查配置项是否存在。 |
| <a href="#app_conf_t_app_conf_get">app\_conf\_get</a> | 获取配置项的值。 |
| <a href="#app_conf_t_app_conf_get_bool">app\_conf\_get\_bool</a> | 获取bool类型配置项的值。 |
| <a href="#app_conf_t_app_conf_get_double">app\_conf\_get\_double</a> | 获取单精度浮点数类型配置项的值。 |
| <a href="#app_conf_t_app_conf_get_instance">app\_conf\_get\_instance</a> | 获取app_conf实例。 |
| <a href="#app_conf_t_app_conf_get_int">app\_conf\_get\_int</a> | 获取整数类型配置项的值。 |
| <a href="#app_conf_t_app_conf_get_int64">app\_conf\_get\_int64</a> | 获取64位整数类型配置项的值。 |
| <a href="#app_conf_t_app_conf_get_str">app\_conf\_get\_str</a> | 获取字符串类型配置项的值。 |
| <a href="#app_conf_t_app_conf_get_wstr">app\_conf\_get\_wstr</a> | 获取宽字符串类型配置项的值。 |
| <a href="#app_conf_t_app_conf_off_changed">app\_conf\_off\_changed</a> | 注销配置变化事件。 |
| <a href="#app_conf_t_app_conf_off_changed_by_ctx">app\_conf\_off\_changed\_by\_ctx</a> | 注销配置变化事件。 |
| <a href="#app_conf_t_app_conf_on_changed">app\_conf\_on\_changed</a> | 注册配置变化事件。 |
| <a href="#app_conf_t_app_conf_reload">app\_conf\_reload</a> | 重新加载配置(内存中的配置丢失)。 |
| <a href="#app_conf_t_app_conf_remove">app\_conf\_remove</a> | 删除配置项。 |
| <a href="#app_conf_t_app_conf_save">app\_conf\_save</a> | 持久保存配置。 |
| <a href="#app_conf_t_app_conf_set">app\_conf\_set</a> | 设置配置项的值。 |
| <a href="#app_conf_t_app_conf_set_bool">app\_conf\_set\_bool</a> | 设置bool类型配置项的值。 |
| <a href="#app_conf_t_app_conf_set_double">app\_conf\_set\_double</a> | 设置双精度类型配置项的值。 |
| <a href="#app_conf_t_app_conf_set_instance">app\_conf\_set\_instance</a> | 设置app_conf实例。 |
| <a href="#app_conf_t_app_conf_set_int">app\_conf\_set\_int</a> | 设置整数类型配置项的值。 |
| <a href="#app_conf_t_app_conf_set_int64">app\_conf\_set\_int64</a> | 设置64位整数类型配置项的值。 |
| <a href="#app_conf_t_app_conf_set_str">app\_conf\_set\_str</a> | 设置字符串类型配置项的值。 |
| <a href="#app_conf_t_app_conf_set_wstr">app\_conf\_set\_wstr</a> | 设置宽字符串类型配置项的值。 |
#### app\_conf\_deinit 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_deinit">释放conf对象。

* 函数原型：

```
ret_t app_conf_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### app\_conf\_exist 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_exist">检查配置项是否存在。

* 函数原型：

```
bool_t app_conf_exist (const char* key);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，FALSE表示不存在。 |
| key | const char* | 配置项的名称。 |
#### app\_conf\_get 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_get">获取配置项的值。

* 函数原型：

```
ret_t app_conf_get (const char* key, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| key | const char* | 配置项的名称。 |
| v | value\_t* | 配置项的值(用于返回)。 |
#### app\_conf\_get\_bool 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_get_bool">获取bool类型配置项的值。

* 函数原型：

```
bool_t app_conf_get_bool (const char* key, bool_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回配置项的值（如果不存在返回缺省值）。 |
| key | const char* | 配置项的名称。 |
| defval | bool\_t | 缺省值。 |
#### app\_conf\_get\_double 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_get_double">获取单精度浮点数类型配置项的值。

* 函数原型：

```
double app_conf_get_double (const char* key, double defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | double | 返回配置项的值（如果不存在返回缺省值）。 |
| key | const char* | 配置项的名称。 |
| defval | double | 缺省值。 |
#### app\_conf\_get\_instance 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_get_instance">获取app_conf实例。

* 函数原型：

```
object_t* app_conf_get_instance ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回app\_conf实例。 |
#### app\_conf\_get\_int 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_get_int">获取整数类型配置项的值。

* 函数原型：

```
int32_t app_conf_get_int (const char* key, int32_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回配置项的值（如果不存在返回缺省值）。 |
| key | const char* | 配置项的名称。 |
| defval | int32\_t | 缺省值。 |
#### app\_conf\_get\_int64 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_get_int64">获取64位整数类型配置项的值。

* 函数原型：

```
int64_t app_conf_get_int64 (const char* key, int64_t defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int64\_t | 返回配置项的值（如果不存在返回缺省值）。 |
| key | const char* | 配置项的名称。 |
| defval | int64\_t | 缺省值。 |
#### app\_conf\_get\_str 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_get_str">获取字符串类型配置项的值。

* 函数原型：

```
const char* app_conf_get_str (const char* key, const char* defval);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回配置项的值（如果不存在返回缺省值）。 |
| key | const char* | 配置项的名称。 |
| defval | const char* | 缺省值。 |
#### app\_conf\_get\_wstr 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_get_wstr">获取宽字符串类型配置项的值。

* 函数原型：

```
ret_t app_conf_get_wstr (const char* key, const wchar_t* str, uint32_t max_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| key | const char* | 配置项的名称。 |
| str | const wchar\_t* | 返回值。 |
| max\_size | uint32\_t | 最大长度(字符数)。 |
#### app\_conf\_off\_changed 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_off_changed">注销配置变化事件。

* 函数原型：

```
ret_t app_conf_off_changed (uint32_t id);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| id | uint32\_t | app\_conf\_on\_changed返回的ID。 |
#### app\_conf\_off\_changed\_by\_ctx 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_off_changed_by_ctx">注销配置变化事件。

* 函数原型：

```
ret_t app_conf_off_changed_by_ctx (void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | void* | 事件处理函数上下文。 |
#### app\_conf\_on\_changed 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_on_changed">注册配置变化事件。

* 函数原型：

```
uint32_t app_conf_on_changed (event_func_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回id，用于app\_conf\_off\_changed。 |
| on\_event | event\_func\_t | 事件处理函数。 |
| ctx | void* | 事件处理函数上下文。 |
#### app\_conf\_reload 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_reload">重新加载配置(内存中的配置丢失)。

* 函数原型：

```
ret_t app_conf_reload ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### app\_conf\_remove 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_remove">删除配置项。

* 函数原型：

```
ret_t app_conf_remove (const char* key);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| key | const char* | 配置项的名称。 |
#### app\_conf\_save 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_save">持久保存配置。

* 函数原型：

```
ret_t app_conf_save ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### app\_conf\_set 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_set">设置配置项的值。

* 函数原型：

```
ret_t app_conf_set (const char* key, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| key | const char* | 配置项的名称。 |
| v | const value\_t* | 配置项的值。 |
#### app\_conf\_set\_bool 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_set_bool">设置bool类型配置项的值。

* 函数原型：

```
ret_t app_conf_set_bool (const char* key, bool_t v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| key | const char* | 配置项的名称。 |
| v | bool\_t | 配置项的值。 |
#### app\_conf\_set\_double 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_set_double">设置双精度类型配置项的值。

* 函数原型：

```
ret_t app_conf_set_double (const char* key, double v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| key | const char* | 配置项的名称。 |
| v | double | 配置项的值。 |
#### app\_conf\_set\_instance 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_set_instance">设置app_conf实例。

* 函数原型：

```
ret_t app_conf_set_instance (object_t* );
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
|  | object\_t* | 。 |
#### app\_conf\_set\_int 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_set_int">设置整数类型配置项的值。

* 函数原型：

```
ret_t app_conf_set_int (const char* key, int32_t v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| key | const char* | 配置项的名称。 |
| v | int32\_t | 配置项的值。 |
#### app\_conf\_set\_int64 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_set_int64">设置64位整数类型配置项的值。

* 函数原型：

```
ret_t app_conf_set_int64 (const char* key, int64_t v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| key | const char* | 配置项的名称。 |
| v | int64\_t | 配置项的值。 |
#### app\_conf\_set\_str 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_set_str">设置字符串类型配置项的值。

* 函数原型：

```
ret_t app_conf_set_str (const char* key, const char* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| key | const char* | 配置项的名称。 |
| v | const char* | 配置项的值。 |
#### app\_conf\_set\_wstr 函数
-----------------------

* 函数功能：

> <p id="app_conf_t_app_conf_set_wstr">设置宽字符串类型配置项的值。

* 函数原型：

```
ret_t app_conf_set_wstr (const char* key, const wchar_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| key | const char* | 配置项的名称。 |
| v | const wchar\_t* | 配置项的值。 |
