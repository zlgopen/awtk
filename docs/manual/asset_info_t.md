## asset\_info\_t
### 概述
单个资源的描述信息。
----------------------------------
### 函数
<p id="asset_info_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#asset_info_t_asset_info_create">asset\_info\_create</a> | 创建asset_info对象。 |
| <a href="#asset_info_t_asset_info_destroy">asset\_info\_destroy</a> | 销毁asset_info对象。 |
| <a href="#asset_info_t_asset_info_ref">asset\_info\_ref</a> | 增加asset_info对象的引用计数。 |
| <a href="#asset_info_t_asset_info_unref">asset\_info\_unref</a> | 减少asset_info对象的引用计数。 |
### 属性
<p id="asset_info_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#asset_info_t_is_in_rom">is\_in\_rom</a> | uint8\_t | 资源是否在ROM中。 |
| <a href="#asset_info_t_name">name</a> | char* | 名称。 |
| <a href="#asset_info_t_refcount">refcount</a> | uint32\_t | 引用计数。 |
| <a href="#asset_info_t_size">size</a> | uint32\_t | 大小。 |
| <a href="#asset_info_t_subtype">subtype</a> | uint8\_t | 子类型。 |
| <a href="#asset_info_t_type">type</a> | uint16\_t | 类型。 |
#### asset\_info\_create 函数
-----------------------

* 函数功能：

> <p id="asset_info_t_asset_info_create">创建asset_info对象。

> 主要供脚本语言使用。

* 函数原型：

```
asset_info_t* asset_info_create (uint16_t type, uint16_t subtype, const char* name, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | asset\_info\_t* | asset\_info对象。 |
| type | uint16\_t | 资源的类型。 |
| subtype | uint16\_t | 资源的子类型。 |
| name | const char* | 资源的名称。 |
| size | uint32\_t | 资源的数据长度(用于分配空间)。 |
#### asset\_info\_destroy 函数
-----------------------

* 函数功能：

> <p id="asset_info_t_asset_info_destroy">销毁asset_info对象。

* 函数原型：

```
ret_t asset_info_destroy (asset_info_t* info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| info | asset\_info\_t* | asset\_info对象。 |
#### asset\_info\_ref 函数
-----------------------

* 函数功能：

> <p id="asset_info_t_asset_info_ref">增加asset_info对象的引用计数。

* 函数原型：

```
ret_t asset_info_ref (asset_info_t* info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| info | asset\_info\_t* | asset\_info对象。 |
#### asset\_info\_unref 函数
-----------------------

* 函数功能：

> <p id="asset_info_t_asset_info_unref">减少asset_info对象的引用计数。

* 函数原型：

```
ret_t asset_info_unref (asset_info_t* info);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| info | asset\_info\_t* | asset\_info对象。 |
#### is\_in\_rom 属性
-----------------------
> <p id="asset_info_t_is_in_rom">资源是否在ROM中。

* 类型：uint8\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### name 属性
-----------------------
> <p id="asset_info_t_name">名称。

* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### refcount 属性
-----------------------
> <p id="asset_info_t_refcount">引用计数。
is\_in\_rom == FALSE时才有效。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### size 属性
-----------------------
> <p id="asset_info_t_size">大小。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### subtype 属性
-----------------------
> <p id="asset_info_t_subtype">子类型。

* 类型：uint8\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### type 属性
-----------------------
> <p id="asset_info_t_type">类型。

* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
