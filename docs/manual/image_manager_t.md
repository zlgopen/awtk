## image\_manager\_t
### 概述
图片管理器。负责加载，解码和缓存图片。
----------------------------------
### 函数
<p id="image_manager_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#image_manager_t_image_manager">image\_manager</a> | 获取缺省的图片管理器。 |
| <a href="#image_manager_t_image_manager_create">image\_manager\_create</a> | 创建图片管理器。 |
| <a href="#image_manager_t_image_manager_deinit">image\_manager\_deinit</a> | 析构图片管理器。 |
| <a href="#image_manager_t_image_manager_destroy">image\_manager\_destroy</a> | 析构并释放图片管理器。 |
| <a href="#image_manager_t_image_manager_get_bitmap">image\_manager\_get\_bitmap</a> | 获取指定的图片。 |
| <a href="#image_manager_t_image_manager_init">image\_manager\_init</a> | 初始化图片管理器。 |
| <a href="#image_manager_t_image_manager_preload">image\_manager\_preload</a> | 预加载指定的图片。 |
| <a href="#image_manager_t_image_manager_set">image\_manager\_set</a> | 设置缺省的图片管理器。 |
| <a href="#image_manager_t_image_manager_set_assets_manager">image\_manager\_set\_assets\_manager</a> | 设置资源管理器对象。 |
| <a href="#image_manager_t_image_manager_unload_all">image\_manager\_unload\_all</a> | 从图片管理器中卸载全部图片。 |
| <a href="#image_manager_t_image_manager_unload_bitmap">image\_manager\_unload\_bitmap</a> | 从图片管理器中卸载指定的图片。 |
| <a href="#image_manager_t_image_manager_unload_unused">image\_manager\_unload\_unused</a> | 从图片管理器中卸载指定时间内没有使用的图片。 |
| <a href="#image_manager_t_image_manager_update_specific">image\_manager\_update\_specific</a> | 更新缓存中图片的specific信息。 |
### 属性
<p id="image_manager_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
#### image\_manager 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager">获取缺省的图片管理器。

* 函数原型：

```
image_manager_t* image_manager ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | image\_manager\_t* | 返回图片管理器对象。 |
#### image\_manager\_create 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_create">创建图片管理器。

* 函数原型：

```
image_manager_t* image_manager_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | image\_manager\_t* | 返回图片管理器对象。 |
#### image\_manager\_deinit 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_deinit">析构图片管理器。

* 函数原型：

```
ret_t image_manager_deinit (image_manager_t* imm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
#### image\_manager\_destroy 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_destroy">析构并释放图片管理器。

* 函数原型：

```
ret_t image_manager_destroy (image_manager_t* imm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
#### image\_manager\_get\_bitmap 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_get_bitmap">获取指定的图片。
先从缓存查找，如果没找到，再加载并缓存。

* 函数原型：

```
ret_t image_manager_get_bitmap (image_manager_t* imm, char* name, bitmap_t* image);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| name | char* | 图片名称。 |
| image | bitmap\_t* | 用于返回图片。 |
#### image\_manager\_init 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_init">初始化图片管理器。

* 函数原型：

```
image_manager_t* image_manager_init (image_manager_t* imm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | image\_manager\_t* | 返回图片管理器对象。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
#### image\_manager\_preload 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_preload">预加载指定的图片。

* 函数原型：

```
ret_t image_manager_preload (image_manager_t* imm, char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| name | char* | 图片名称。 |
#### image\_manager\_set 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_set">设置缺省的图片管理器。

* 函数原型：

```
ret_t image_manager_set (image_manager_t* imm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
#### image\_manager\_set\_assets\_manager 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_set_assets_manager">设置资源管理器对象。

之所以需要设置资源管理器对象，而不是使用缺省的资源管理器对象，是因为在designer中有两个图片管理器：

* 一个用于designer本身加载图片。

* 一个用于被设计的窗口加载图片。

这两个图片管理器需要从不同的路径加载资源。

* 函数原型：

```
ret_t image_manager_set_assets_manager (image_manager_t* imm, assets_manager_t* assets_manager);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| assets\_manager | assets\_manager\_t* | 资源管理器。 |
#### image\_manager\_unload\_all 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_unload_all">从图片管理器中卸载全部图片。

* 函数原型：

```
ret_t image_manager_unload_all (image_manager_t* imm);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
#### image\_manager\_unload\_bitmap 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_unload_bitmap">从图片管理器中卸载指定的图片。

* 函数原型：

```
ret_t image_manager_unload_bitmap (image_manager_t* imm, bitmap_t* image);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| image | bitmap\_t* | 图片。 |
#### image\_manager\_unload\_unused 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_unload_unused">从图片管理器中卸载指定时间内没有使用的图片。

* 函数原型：

```
ret_t image_manager_unload_unused (image_manager_t* imm, uint32_t time_delta_s);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| time\_delta\_s | uint32\_t | 时间范围，单位为秒。 |
#### image\_manager\_update\_specific 函数
-----------------------

* 函数功能：

> <p id="image_manager_t_image_manager_update_specific">更新缓存中图片的specific信息。

* 函数原型：

```
ret_t image_manager_update_specific (image_manager_t* imm, bitmap_t* image);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| image | bitmap\_t* | 返回图片信息。 |
