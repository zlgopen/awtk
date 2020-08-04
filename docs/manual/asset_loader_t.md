## asset\_loader\_t
### 概述
资源加载器接口。
----------------------------------
### 函数
<p id="asset_loader_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#asset_loader_t_asset_loader_create">asset\_loader\_create</a> | 创建缺省的资源加载器。 |
| <a href="#asset_loader_t_asset_loader_destroy">asset\_loader\_destroy</a> | 销毁loader对象。 |
| <a href="#asset_loader_t_asset_loader_exist">asset\_loader\_exist</a> | 判断指定资源是否存在。 |
| <a href="#asset_loader_t_asset_loader_load">asset\_loader\_load</a> | 加载指定的资源。 |
#### asset\_loader\_create 函数
-----------------------

* 函数功能：

> <p id="asset_loader_t_asset_loader_create">创建缺省的资源加载器。

* 函数原型：

```
asset_loader_t* asset_loader_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_loader\_t* | 返回loader对象。 |
#### asset\_loader\_destroy 函数
-----------------------

* 函数功能：

> <p id="asset_loader_t_asset_loader_destroy">销毁loader对象。

* 函数原型：

```
ret_t asset_loader_destroy (asset_loader_t* loader);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| loader | asset\_loader\_t* | loader对象。 |
#### asset\_loader\_exist 函数
-----------------------

* 函数功能：

> <p id="asset_loader_t_asset_loader_exist">判断指定资源是否存在。
。

* 函数原型：

```
bool_t asset_loader_exist (asset_loader_t* loader, const char* path);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则不存在。 |
| loader | asset\_loader\_t* | loader对象。 |
| path | const char* | 资源是路径。 |
#### asset\_loader\_load 函数
-----------------------

* 函数功能：

> <p id="asset_loader_t_asset_loader_load">加载指定的资源。

* 函数原型：

```
asset_info_t* asset_loader_load (asset_loader_t* loader, uint16_t type, uint16_t subtype, const char* path, const char* name);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_info\_t* | 返回资源对象(由调用者销毁)。 |
| loader | asset\_loader\_t* | loader对象。 |
| type | uint16\_t | 资源类型。 |
| subtype | uint16\_t | 资源子类型。 |
| path | const char* | 路径。 |
| name | const char* | 名称。 |
