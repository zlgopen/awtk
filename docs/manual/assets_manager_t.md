## assets\_manager\_t
### 概述
 资源管理器。

### 函数
<p id="assets_manager_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#assets_manager_t_assets_manager">assets\_manager</a> | 获取缺省资源管理器。 |
| <a href="#assets_manager_t_assets_manager_add">assets\_manager\_add</a> | 向资源管理器中增加一个资源。 |
| <a href="#assets_manager_t_assets_manager_clear_cache">assets\_manager\_clear\_cache</a> | 清除指定类型的缓存。 |
| <a href="#assets_manager_t_assets_manager_create">assets\_manager\_create</a> | 创建资源管理器。 |
| <a href="#assets_manager_t_assets_manager_deinit">assets\_manager\_deinit</a> | 释放全部资源。 |
| <a href="#assets_manager_t_assets_manager_destroy">assets\_manager\_destroy</a> | 释放全部资源并销毁asset manager对象。 |
| <a href="#assets_manager_t_assets_manager_find_in_cache">assets\_manager\_find\_in\_cache</a> | 在资源管理器的缓存中查找指定的资源(不引用)。 |
| <a href="#assets_manager_t_assets_manager_init">assets\_manager\_init</a> | 初始化资源管理器。 |
| <a href="#assets_manager_t_assets_manager_load">assets\_manager\_load</a> | 从文件系统中加载指定的资源，并缓存到内存中。在定义了宏WITH_FS_RES时才生效。 |
| <a href="#assets_manager_t_assets_manager_ref">assets\_manager\_ref</a> | 在资源管理器的缓存中查找指定的资源并引用它，如果缓存中不存在，尝试加载该资源。 |
| <a href="#assets_manager_t_assets_manager_set">assets\_manager\_set</a> | 设置缺省资源管理器。 |
| <a href="#assets_manager_t_assets_manager_set_res_root">assets\_manager\_set\_res\_root</a> | 设置资源所在的目录(其下目录结构请参考demos)。 |
| <a href="#assets_manager_t_assets_manager_unref">assets\_manager\_unref</a> | 释放指定的资源。 |
### 属性
<p id="assets_manager_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="assets_manager_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### assets\_manager 函数
* 函数原型：

```
assets_manager_t* assets_manager ();
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | assets\_manager\_t* | 返回asset manager对象。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager"> 获取缺省资源管理器。




#### assets\_manager\_add 函数
* 函数原型：

```
ret_t assets_manager_add (assets_manager_t* rm, asset_info_t info);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rm | assets\_manager\_t* | asset manager对象。 |
| info | asset\_info\_t | 待增加的资源。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_add"> 向资源管理器中增加一个资源。




#### assets\_manager\_clear\_cache 函数
* 函数原型：

```
ret_t assets_manager_clear_cache (assets_manager_t* rm, asset_type_t type);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rm | assets\_manager\_t* | asset manager对象。 |
| type | asset\_type\_t | 资源的类型。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_clear_cache"> 清除指定类型的缓存。




#### assets\_manager\_create 函数
* 函数原型：

```
assets_manager_t* assets_manager_create (uint32_t init_nr);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | assets\_manager\_t* | 返回asset manager对象。 |
| init\_nr | uint32\_t | 预先分配资源的个数。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_create"> 创建资源管理器。




#### assets\_manager\_deinit 函数
* 函数原型：

```
ret_t assets_manager_deinit (assets_manager_t* rm);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rm | assets\_manager\_t* | asset manager对象。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_deinit"> 释放全部资源。




#### assets\_manager\_destroy 函数
* 函数原型：

```
ret_t assets_manager_destroy (assets_manager_t* rm);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rm | assets\_manager\_t* | asset manager对象。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_destroy"> 释放全部资源并销毁asset manager对象。




#### assets\_manager\_find\_in\_cache 函数
* 函数原型：

```
asset_info_t* assets_manager_find_in_cache (assets_manager_t* rm, asset_type_t type, char* name);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_info\_t* | 返回资源。 |
| rm | assets\_manager\_t* | asset manager对象。 |
| type | asset\_type\_t | 资源的类型。 |
| name | char* | 资源的名称。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_find_in_cache"> 在资源管理器的缓存中查找指定的资源(不引用)。




#### assets\_manager\_init 函数
* 函数原型：

```
assets_manager_t* assets_manager_init (assets_manager_t* rm, uint32_t init_nr);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | assets\_manager\_t* | 返回asset manager对象。 |
| rm | assets\_manager\_t* | asset manager对象。 |
| init\_nr | uint32\_t | 预先分配资源的个数。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_init"> 初始化资源管理器。




#### assets\_manager\_load 函数
* 函数原型：

```
asset_info_t* assets_manager_load (assets_manager_t* rm, asset_type_t type, char* name);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_info\_t* | 返回资源。 |
| rm | assets\_manager\_t* | asset manager对象。 |
| type | asset\_type\_t | 资源的类型。 |
| name | char* | 资源的名称。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_load"> 从文件系统中加载指定的资源，并缓存到内存中。在定义了宏WITH_FS_RES时才生效。




#### assets\_manager\_ref 函数
* 函数原型：

```
asset_info_t* assets_manager_ref (assets_manager_t* rm, asset_type_t type, char* name);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_info\_t* | 返回资源。 |
| rm | assets\_manager\_t* | asset manager对象。 |
| type | asset\_type\_t | 资源的类型。 |
| name | char* | 资源的名称。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_ref"> 在资源管理器的缓存中查找指定的资源并引用它，如果缓存中不存在，尝试加载该资源。




#### assets\_manager\_set 函数
* 函数原型：

```
ret_t assets_manager_set (assets_manager_t* rm);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rm | assets\_manager\_t* | asset manager对象。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_set"> 设置缺省资源管理器。




#### assets\_manager\_set\_res\_root 函数
* 函数原型：

```
ret_t assets_manager_set_res_root (assets_manager_t* rm, const char* res_root);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rm | assets\_manager\_t* | asset manager对象。 |
| res\_root | const char* | 资源所在的目录。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_set_res_root"> 设置资源所在的目录(其下目录结构请参考demos)。




#### assets\_manager\_unref 函数
* 函数原型：

```
ret_t assets_manager_unref (assets_manager_t* rm, asset_info_t* info);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| rm | assets\_manager\_t* | asset manager对象。 |
| info | asset\_info\_t* | 资源。 |
* 函数功能：

> <p id="assets_manager_t_assets_manager_unref"> 释放指定的资源。




