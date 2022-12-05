## assets\_managers\_t
### 概述
在某些情况下，需要多个资源管理器。比如在手表系统里，每个小应用或表盘，可能放在独立的资源包中，
此时优先加载应用自己的资源，如果没有就加载系统的资源。
> 通常AWTK是单进程应用程序，为了避免概念混淆，我们把这些独立可安装的小应用成为"applet"。
----------------------------------
### 函数
<p id="assets_managers_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#assets_managers_t_assets_manager_get_theme_name">assets\_manager\_get\_theme\_name</a> | 获取当前的主题名称。 |
| <a href="#assets_managers_t_assets_managers_is_applet_assets_supported">assets\_managers\_is\_applet\_assets\_supported</a> | 是否支持小应用程序(applet)拥有独立资源目录。 |
| <a href="#assets_managers_t_assets_managers_ref">assets\_managers\_ref</a> | 获取指定小应用程序(applet)的资源管理器。 |
| <a href="#assets_managers_t_assets_managers_set_applet_res_root">assets\_managers\_set\_applet\_res\_root</a> | 设置小应用程序(applet)的资源根目录。 |
| <a href="#assets_managers_t_assets_managers_set_theme">assets\_managers\_set\_theme</a> | 设置当前的主题。 |
| <a href="#assets_managers_t_assets_managers_unref">assets\_managers\_unref</a> | 释放指定小应用程序(applet)的资源管理器。 |
#### assets\_manager\_get\_theme\_name 函数
-----------------------

* 函数功能：

> <p id="assets_managers_t_assets_manager_get_theme_name">获取当前的主题名称。

* 函数原型：

```
const char* assets_manager_get_theme_name (assets_manager_t* am);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 返回主题名称。 |
| am | assets\_manager\_t* | 资源管理器对象。 |
#### assets\_managers\_is\_applet\_assets\_supported 函数
-----------------------

* 函数功能：

> <p id="assets_managers_t_assets_managers_is_applet_assets_supported">是否支持小应用程序(applet)拥有独立资源目录。

* 函数原型：

```
bool_t assets_managers_is_applet_assets_supported ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示支持，否则表示不支持。 |
#### assets\_managers\_ref 函数
-----------------------

* 函数功能：

> <p id="assets_managers_t_assets_managers_ref">获取指定小应用程序(applet)的资源管理器。

* 函数原型：

```
assets_manager_t* assets_managers_ref (const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | assets\_manager\_t* | 返回asset manager对象。 |
| name | const char* | 小应用程序(applet)的名称。 |
#### assets\_managers\_set\_applet\_res\_root 函数
-----------------------

* 函数功能：

> <p id="assets_managers_t_assets_managers_set_applet_res_root">设置小应用程序(applet)的资源根目录。

* 函数原型：

```
ret_t assets_managers_set_applet_res_root (const char* res_root);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| res\_root | const char* | 资源根目录。 |
#### assets\_managers\_set\_theme 函数
-----------------------

* 函数功能：

> <p id="assets_managers_t_assets_managers_set_theme">设置当前的主题。

* 函数原型：

```
ret_t assets_managers_set_theme (const char* theme);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| theme | const char* | 主题名称。 |
#### assets\_managers\_unref 函数
-----------------------

* 函数功能：

> <p id="assets_managers_t_assets_managers_unref">释放指定小应用程序(applet)的资源管理器。

* 函数原型：

```
ret_t assets_managers_unref (assets_manager_t* am);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | assets\_manager\_t* | 资源管理器对象。 |
