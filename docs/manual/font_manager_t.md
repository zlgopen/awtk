## font\_manager\_t
### 概述
字体管理器，负责字体的加载和缓存管理。
(如果使用nanovg，字体由nanovg内部管理)
----------------------------------
### 函数
<p id="font_manager_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#font_manager_t_font_manager">font\_manager</a> | 获取缺省的字体管理器。 |
| <a href="#font_manager_t_font_manager_add_font">font\_manager\_add\_font</a> | 向缓存中加入字体。 |
| <a href="#font_manager_t_font_manager_create">font\_manager\_create</a> | 创建字体管理器。 |
| <a href="#font_manager_t_font_manager_deinit">font\_manager\_deinit</a> | 析构字体管理器。 |
| <a href="#font_manager_t_font_manager_destroy">font\_manager\_destroy</a> | 析构并释放字体管理器。 |
| <a href="#font_manager_t_font_manager_get_font">font\_manager\_get\_font</a> | 从缓存中查找字体，如果没找到，再加载字体，并缓存。 |
| <a href="#font_manager_t_font_manager_init">font\_manager\_init</a> | 初始化字体管理器。 |
| <a href="#font_manager_t_font_manager_set">font\_manager\_set</a> | 设置缺省的字体管理器。 |
| <a href="#font_manager_t_font_manager_set_assets_manager">font\_manager\_set\_assets\_manager</a> | 设置资源管理器对象 |
| <a href="#font_manager_t_font_manager_shrink_cache">font\_manager\_shrink\_cache</a> | 清除最久没有被使用的缓冲字模。 |
| <a href="#font_manager_t_font_manager_unload_all">font\_manager\_unload\_all</a> | 卸载全部字体。 |
| <a href="#font_manager_t_font_manager_unload_font">font\_manager\_unload\_font</a> | 卸载指定的字体。 |
### 属性
<p id="font_manager_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
#### font\_manager 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager">获取缺省的字体管理器。

* 函数原型：

```
font_manager_t* font_manager ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | font\_manager\_t* | 返回字体管理器对象。 |
#### font\_manager\_add\_font 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_add_font">向缓存中加入字体。

* 函数原型：

```
ret_t font_manager_add_font (font_manager_t* fm, char* name, font_t* font);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fm | font\_manager\_t* | 字体管理器对象。 |
| name | char* | 字体名。 |
| font | font\_t* | 字体。 |
#### font\_manager\_create 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_create">创建字体管理器。

* 函数原型：

```
font_manager_t* font_manager_create (font_loader_t* loader);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | font\_manager\_t* | 返回字体管理器对象。 |
| loader | font\_loader\_t* | 字体加载器。 |
#### font\_manager\_deinit 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_deinit">析构字体管理器。

* 函数原型：

```
ret_t font_manager_deinit (font_manager_t* fm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fm | font\_manager\_t* | 字体管理器对象。 |
#### font\_manager\_destroy 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_destroy">析构并释放字体管理器。

* 函数原型：

```
ret_t font_manager_destroy (font_manager_t* fm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fm | font\_manager\_t* | 字体管理器对象。 |
#### font\_manager\_get\_font 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_get_font">从缓存中查找字体，如果没找到，再加载字体，并缓存。

* 函数原型：

```
font_t* font_manager_get_font (font_manager_t* fm, char* name, font_size_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | font\_t* | 返回字体对象。 |
| fm | font\_manager\_t* | 字体管理器对象。 |
| name | char* | 字体名，为NULL时使用缺省字体。 |
| size | font\_size\_t | 字体的大小。 |
#### font\_manager\_init 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_init">初始化字体管理器。

* 函数原型：

```
font_manager_t* font_manager_init (font_manager_t* fm, font_loader_t* loader);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | font\_manager\_t* | 返回字体管理器对象。 |
| fm | font\_manager\_t* | 字体管理器对象。 |
| loader | font\_loader\_t* | 字体加载器。 |
#### font\_manager\_set 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_set">设置缺省的字体管理器。

* 函数原型：

```
ret_t font_manager_set (font_manager_t* fm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fm | font\_manager\_t* | 字体管理器对象。 |
#### font\_manager\_set\_assets\_manager 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_set_assets_manager">设置资源管理器对象

之所以需要设置资源管理器对象，而不是使用缺省的资源管理器对象，是因为在designer中有两个字体管理器：

* 一个用于designer本身加载字体。

* 一个用于被设计的窗口加载字体。

这两个字体管理器需要从不同的路径加载资源。

* 函数原型：

```
ret_t font_manager_set_assets_manager (font_manager_t* imm, assets_manager_t* assets_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | font\_manager\_t* | 字体管理器对象。 |
| assets\_manager | assets\_manager\_t* | 资源管理器。 |
#### font\_manager\_shrink\_cache 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_shrink_cache">清除最久没有被使用的缓冲字模。

* 函数原型：

```
ret_t font_manager_shrink_cache (font_manager_t* fm, uint32_t cache_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fm | font\_manager\_t* | 字体管理器对象。 |
| cache\_size | uint32\_t | 每种字体保留缓存字模的个数。 |
#### font\_manager\_unload\_all 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_unload_all">卸载全部字体。

* 函数原型：

```
ret_t font_manager_unload_all (font_manager_t* fm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fm | font\_manager\_t* | 字体管理器对象。 |
#### font\_manager\_unload\_font 函数
-----------------------

* 函数功能：

> <p id="font_manager_t_font_manager_unload_font">卸载指定的字体。

* 函数原型：

```
ret_t font_manager_unload_font (font_manager_t* fm, char* name, font_size_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| fm | font\_manager\_t* | 字体管理器对象。 |
| name | char* | 字体名，为NULL时使用缺省字体。 |
| size | font\_size\_t | 字体的大小(矢量字体指定为0即可)。 |
