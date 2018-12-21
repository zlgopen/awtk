## image\_manager\_t
### 概述
 图片管理器。负责加载，解码和缓存图片。

### 函数
<p id="image_manager_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#image_manager_t_image_manager">image\_manager</a> | 获取缺省的图片管理器。 |
| <a href="#image_manager_t_image_manager_add">image\_manager\_add</a> | 向缓存中加入一张图片 |
| <a href="#image_manager_t_image_manager_create">image\_manager\_create</a> | 创建图片管理器。 |
| <a href="#image_manager_t_image_manager_deinit">image\_manager\_deinit</a> | 析构图片管理器。 |
| <a href="#image_manager_t_image_manager_destroy">image\_manager\_destroy</a> | 析构并释放图片管理器。 |
| <a href="#image_manager_t_image_manager_init">image\_manager\_init</a> | 初始化图片管理器。 |
| <a href="#image_manager_t_image_manager_load">image\_manager\_load</a> | 加载指定的图片。 |
| <a href="#image_manager_t_image_manager_lookup">image\_manager\_lookup</a> | 从缓存中查找图片 |
| <a href="#image_manager_t_image_manager_set">image\_manager\_set</a> | 设置缺省的图片管理器。 |
| <a href="#image_manager_t_image_manager_set_assets_manager">image\_manager\_set\_assets\_manager</a> | 设置资源管理器对象 |
| <a href="#image_manager_t_image_manager_unload_unused">image\_manager\_unload\_unused</a> | 从图片管理器中卸载指定时间内没有使用的图片。 |
| <a href="#image_manager_t_image_manager_update_specific">image\_manager\_update\_specific</a> | 更新缓存中图片的specific信息。 |
### 属性
<p id="image_manager_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
### 事件
<p id="image_manager_t_events">

| 事件名称 | 类型  | 说明 | 
| -------- | ----- | ------- | 
#### image\_manager 函数
* 函数原型：

```
image_manager_t* image_manager ();
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | image\_manager\_t* | 返回图片管理器对象。 |
* 函数功能：

> <p id="image_manager_t_image_manager"> 获取缺省的图片管理器。



#### image\_manager\_add 函数
* 函数原型：

```
ret_t image_manager_add (image_manager_t* imm, char* name, bitmap_t* image);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| name | char* | 图片名。 |
| image | bitmap\_t* | 图片信息。 |
* 函数功能：

> <p id="image_manager_t_image_manager_add"> 向缓存中加入一张图片




#### image\_manager\_create 函数
* 函数原型：

```
image_manager_t* image_manager_create (image_loader_t* loader);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | image\_manager\_t* | 返回图片管理器对象。 |
| loader | image\_loader\_t* | 图片加载器。 |
* 函数功能：

> <p id="image_manager_t_image_manager_create"> 创建图片管理器。




#### image\_manager\_deinit 函数
* 函数原型：

```
ret_t image_manager_deinit (image_manager_t* imm);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
* 函数功能：

> <p id="image_manager_t_image_manager_deinit"> 析构图片管理器。




#### image\_manager\_destroy 函数
* 函数原型：

```
ret_t image_manager_destroy (image_manager_t* imm);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
* 函数功能：

> <p id="image_manager_t_image_manager_destroy"> 析构并释放图片管理器。




#### image\_manager\_init 函数
* 函数原型：

```
image_manager_t* image_manager_init (image_manager_t* imm, image_loader_t* loader);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | image\_manager\_t* | 返回图片管理器对象。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| loader | image\_loader\_t* | 图片加载器。 |
* 函数功能：

> <p id="image_manager_t_image_manager_init"> 初始化图片管理器。




#### image\_manager\_load 函数
* 函数原型：

```
ret_t image_manager_load (image_manager_t* imm, char* name, bitmap_t* image);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| name | char* | 图片名称。 |
| image | bitmap\_t* | 用于返回图片。 |
* 函数功能：

> <p id="image_manager_t_image_manager_load"> 加载指定的图片。




#### image\_manager\_lookup 函数
* 函数原型：

```
ret_t image_manager_lookup (image_manager_t* imm, char* name, bitmap_t* image);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| name | char* | 图片名。 |
| image | bitmap\_t* | 返回图片信息。 |
* 函数功能：

> <p id="image_manager_t_image_manager_lookup"> 从缓存中查找图片




#### image\_manager\_set 函数
* 函数原型：

```
ret_t image_manager_set (image_manager_t* imm);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
* 函数功能：

> <p id="image_manager_t_image_manager_set"> 设置缺省的图片管理器。




#### image\_manager\_set\_assets\_manager 函数
* 函数原型：

```
ret_t image_manager_set_assets_manager (image_manager_t* imm, assets_manager_t* assets_manager);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| assets\_manager | assets\_manager\_t* | 资源管理器。 |
* 函数功能：

> <p id="image_manager_t_image_manager_set_assets_manager"> 设置资源管理器对象




#### image\_manager\_unload\_unused 函数
* 函数原型：

```
ret_t image_manager_unload_unused (image_manager_t* imm, uint32_t time_delta_s);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| time\_delta\_s | uint32\_t | 时间范围，单位为秒。 |
* 函数功能：

> <p id="image_manager_t_image_manager_unload_unused"> 从图片管理器中卸载指定时间内没有使用的图片。




#### image\_manager\_update\_specific 函数
* 函数原型：

```
ret_t image_manager_update_specific (image_manager_t* imm, bitmap_t* image);
```

* 参数说明：

-----------------------

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| imm | image\_manager\_t* | 图片管理器对象。 |
| image | bitmap\_t* | 返回图片信息。 |
* 函数功能：

> <p id="image_manager_t_image_manager_update_specific"> 更新缓存中图片的specific信息。




