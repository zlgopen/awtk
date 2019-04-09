## assets\_manager\_t
### 概述
 资源管理器。
 这里的资源管理器并非Windows下的文件浏览器，而是负责对各种资源，比如字体、主题、图片、界面数据、字符串和其它数据的进行集中管理的组件。引入资源管理器的目的有以下几个：

 *
让上层不需要了解存储的方式。在没有文件系统时或者内存紧缺时，把资源转成常量数组直接编译到代码中。在有文件系统而且内存充足时，资源放在文件系统中。在有网络时，资源也可以存放在服务器上(暂未实现)。资源管理器为上层提供统一的接口，让上层而不用关心底层的存储方式。

 *
让上层不需要了解资源的具体格式。比如一个名为earth的图片，没有文件系统或内存紧缺，图片直接用位图数据格式存在ROM中，而有文件系统时，则用PNG格式存放在文件系统中。资源管理器让上层不需要关心图片的格式，访问时指定图片的名称即可(不用指定扩展名)。

 *
让上层不需要了解屏幕的密度。不同的屏幕密度下需要加载不同的图片，比如MacPro的Retina屏就需要用双倍解析度的图片，否则就出现界面模糊。AWTK以后会支持PC软件和手机软件的开发，所以资源管理器需要为此提供支持，让上层不需关心屏幕的密度。

 *
对资源进行内存缓存。不同类型的资源使用方式是不一样的，比如字体和主题加载之后会一直使用，UI文件在生成界面之后就暂时不需要了，PNG文件解码之后就只需要保留解码的位图数据即可。资源管理器配合图片管理器等其它组件实现资源的自动缓存。

当从文件系统加载资源时，目录结构要求如下：

 ```
 assets/raw/
  fonts   字体
  images  图片
    x1   普通密度屏幕的图片。
    x2   2倍密度屏幕的图片。
    x3   3倍密度屏幕的图片。
  strings 需要翻译的字符串。
  styles  主题数据。
  ui      UI描述数据。
 ```



----------------------------------
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
| <a href="#assets_manager_t_assets_manager_load">assets\_manager\_load</a> | 从文件系统中加载指定的资源，并缓存到内存中。在定义了宏WITH\_FS\_RES时才生效。 |
| <a href="#assets_manager_t_assets_manager_preload">assets\_manager\_preload</a> | 从文件系统中加载指定的资源，并缓存到内存中。在定义了宏WITH\_FS\_RES时才生效。 |
| <a href="#assets_manager_t_assets_manager_ref">assets\_manager\_ref</a> | 在资源管理器的缓存中查找指定的资源并引用它，如果缓存中不存在，尝试加载该资源。 |
| <a href="#assets_manager_t_assets_manager_set">assets\_manager\_set</a> | 设置缺省资源管理器。 |
| <a href="#assets_manager_t_assets_manager_set_res_root">assets\_manager\_set\_res\_root</a> | 设置资源所在的目录(其下目录结构请参考demos)。 |
| <a href="#assets_manager_t_assets_manager_unref">assets\_manager\_unref</a> | 释放指定的资源。 |
#### assets\_manager 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager"> 获取缺省资源管理器。




* 函数原型：

```
assets_manager_t* assets_manager ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | assets\_manager\_t* | 返回asset manager对象。 |
#### assets\_manager\_add 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_add"> 向资源管理器中增加一个资源。




* 函数原型：

```
ret_t assets_manager_add (assets_manager_t* am, asset_info_t info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | assets\_manager\_t* | asset manager对象。 |
| info | asset\_info\_t | 待增加的资源。 |
#### assets\_manager\_clear\_cache 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_clear_cache"> 清除指定类型的缓存。




* 函数原型：

```
ret_t assets_manager_clear_cache (assets_manager_t* am, asset_type_t type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | assets\_manager\_t* | asset manager对象。 |
| type | asset\_type\_t | 资源的类型。 |
#### assets\_manager\_create 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_create"> 创建资源管理器。




* 函数原型：

```
assets_manager_t* assets_manager_create (uint32_t init_nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | assets\_manager\_t* | 返回asset manager对象。 |
| init\_nr | uint32\_t | 预先分配资源的个数。 |
#### assets\_manager\_deinit 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_deinit"> 释放全部资源。




* 函数原型：

```
ret_t assets_manager_deinit (assets_manager_t* am);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | assets\_manager\_t* | asset manager对象。 |
#### assets\_manager\_destroy 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_destroy"> 释放全部资源并销毁asset manager对象。




* 函数原型：

```
ret_t assets_manager_destroy (assets_manager_t* am);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | assets\_manager\_t* | asset manager对象。 |
#### assets\_manager\_find\_in\_cache 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_find_in_cache"> 在资源管理器的缓存中查找指定的资源(不引用)。




* 函数原型：

```
asset_info_t* assets_manager_find_in_cache (assets_manager_t* am, asset_type_t type, char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_info\_t* | 返回资源。 |
| am | assets\_manager\_t* | asset manager对象。 |
| type | asset\_type\_t | 资源的类型。 |
| name | char* | 资源的名称。 |
#### assets\_manager\_init 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_init"> 初始化资源管理器。




* 函数原型：

```
assets_manager_t* assets_manager_init (assets_manager_t* am, uint32_t init_nr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | assets\_manager\_t* | 返回asset manager对象。 |
| am | assets\_manager\_t* | asset manager对象。 |
| init\_nr | uint32\_t | 预先分配资源的个数。 |
#### assets\_manager\_load 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_load"> 从文件系统中加载指定的资源，并缓存到内存中。在定义了宏WITH\_FS\_RES时才生效。




* 函数原型：

```
asset_info_t* assets_manager_load (assets_manager_t* am, asset_type_t type, char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_info\_t* | 返回资源。 |
| am | assets\_manager\_t* | asset manager对象。 |
| type | asset\_type\_t | 资源的类型。 |
| name | char* | 资源的名称。 |
#### assets\_manager\_preload 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_preload"> 从文件系统中加载指定的资源，并缓存到内存中。在定义了宏WITH\_FS\_RES时才生效。




* 函数原型：

```
ret_t assets_manager_preload (assets_manager_t* am, asset_type_t type, char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | assets\_manager\_t* | asset manager对象。 |
| type | asset\_type\_t | 资源的类型。 |
| name | char* | 资源的名称。 |
#### assets\_manager\_ref 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_ref"> 在资源管理器的缓存中查找指定的资源并引用它，如果缓存中不存在，尝试加载该资源。




* 函数原型：

```
asset_info_t* assets_manager_ref (assets_manager_t* am, asset_type_t type, char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_info\_t* | 返回资源。 |
| am | assets\_manager\_t* | asset manager对象。 |
| type | asset\_type\_t | 资源的类型。 |
| name | char* | 资源的名称。 |
#### assets\_manager\_set 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_set"> 设置缺省资源管理器。




* 函数原型：

```
ret_t assets_manager_set (assets_manager_t* am);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | assets\_manager\_t* | asset manager对象。 |
#### assets\_manager\_set\_res\_root 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_set_res_root"> 设置资源所在的目录(其下目录结构请参考demos)。




* 函数原型：

```
ret_t assets_manager_set_res_root (assets_manager_t* am, const char* res_root);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | assets\_manager\_t* | asset manager对象。 |
| res\_root | const char* | 资源所在的目录。 |
#### assets\_manager\_unref 函数
-----------------------

* 函数功能：

> <p id="assets_manager_t_assets_manager_unref"> 释放指定的资源。




* 函数原型：

```
ret_t assets_manager_unref (assets_manager_t* am, asset_info_t* info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| am | assets\_manager\_t* | asset manager对象。 |
| info | asset\_info\_t* | 资源。 |
