## plugin\_manager\_t
### 概述
插件管理器。
----------------------------------
### 函数
<p id="plugin_manager_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#plugin_manager_t_plugin_manager_create">plugin\_manager\_create</a> | 创建插件管理器。 |
| <a href="#plugin_manager_t_plugin_manager_destroy">plugin\_manager\_destroy</a> | 卸载全部插件，并销毁插件管理器对象。 |
| <a href="#plugin_manager_t_plugin_manager_dump">plugin\_manager\_dump</a> | 显示所有的插件。 |
| <a href="#plugin_manager_t_plugin_manager_exist">plugin\_manager\_exist</a> | 检查指定的插件是否存在。 |
| <a href="#plugin_manager_t_plugin_manager_refresh">plugin\_manager\_refresh</a> | 重新扫描目录中的插件，加载没有加载的插件。 |
| <a href="#plugin_manager_t_plugin_manager_reload">plugin\_manager\_reload</a> | 卸载所有插件并重新加载。 |
#### plugin\_manager\_create 函数
-----------------------

* 函数功能：

> <p id="plugin_manager_t_plugin_manager_create">创建插件管理器。

* 函数原型：

```
plugin_manager_t* plugin_manager_create (const char* path, plugin_get_init_func_name_t get_init, plugin_get_deinit_func_name_t get_deinit);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | plugin\_manager\_t* | 动态链接库对象。 |
| path | const char* | 插件所在的目录。 |
| get\_init | plugin\_get\_init\_func\_name\_t | 获取初始化函数的函数名。为NULL时使用"init"。 |
| get\_deinit | plugin\_get\_deinit\_func\_name\_t | 获取~初始化函数的函数名。为NULL时使用"deinit"。 |
#### plugin\_manager\_destroy 函数
-----------------------

* 函数功能：

> <p id="plugin_manager_t_plugin_manager_destroy">卸载全部插件，并销毁插件管理器对象。

* 函数原型：

```
ret_t plugin_manager_destroy (plugin_manager_t* plugin_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| plugin\_manager | plugin\_manager\_t* | plugin\_manager对象。 |
#### plugin\_manager\_dump 函数
-----------------------

* 函数功能：

> <p id="plugin_manager_t_plugin_manager_dump">显示所有的插件。

* 函数原型：

```
ret_t plugin_manager_dump (plugin_manager_t* plugin_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| plugin\_manager | plugin\_manager\_t* | plugin\_manager对象。 |
#### plugin\_manager\_exist 函数
-----------------------

* 函数功能：

> <p id="plugin_manager_t_plugin_manager_exist">检查指定的插件是否存在。

* 函数原型：

```
bool_t plugin_manager_exist (plugin_manager_t* plugin_manager, const char* lib_name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则表示不存在。 |
| plugin\_manager | plugin\_manager\_t* | plugin\_manager对象。 |
| lib\_name | const char* | 动态库的名称。 |
#### plugin\_manager\_refresh 函数
-----------------------

* 函数功能：

> <p id="plugin_manager_t_plugin_manager_refresh">重新扫描目录中的插件，加载没有加载的插件。

* 函数原型：

```
ret_t plugin_manager_refresh (plugin_manager_t* plugin_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| plugin\_manager | plugin\_manager\_t* | plugin\_manager对象。 |
#### plugin\_manager\_reload 函数
-----------------------

* 函数功能：

> <p id="plugin_manager_t_plugin_manager_reload">卸载所有插件并重新加载。

* 函数原型：

```
ret_t plugin_manager_reload (plugin_manager_t* plugin_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| plugin\_manager | plugin\_manager\_t* | plugin\_manager对象。 |
