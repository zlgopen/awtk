## asset\_info\_t
### 概述
 单个资源的描述信息。


----------------------------------
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
#### is\_in\_rom 属性
-----------------------
> <p id="asset_info_t_is_in_rom"> 资源是否在ROM中。



* 类型：uint8\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### name 属性
-----------------------
> <p id="asset_info_t_name"> 名称。



* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### refcount 属性
-----------------------
> <p id="asset_info_t_refcount"> 引用计数。
 is\_in\_rom == FALSE时才有效。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### size 属性
-----------------------
> <p id="asset_info_t_size"> 大小。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### subtype 属性
-----------------------
> <p id="asset_info_t_subtype"> 子类型。



* 类型：uint8\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
#### type 属性
-----------------------
> <p id="asset_info_t_type"> 类型。



* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
