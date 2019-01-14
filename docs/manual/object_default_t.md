## object\_default\_t
### 概述
![image](images/object_default_t_0.png)


 对象接口的缺省实现。

 内部使用有序数组保存所有属性，可以快速查找指定名称的属性。


### 函数
<p id="object_default_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#object_default_t_object_default_create">object\_default\_create</a> |  |
### 属性
<p id="object_default_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#object_default_t_props">props</a> | named\_value\_t | 属性数组。 |
| <a href="#object_default_t_props_capacity">props\_capacity</a> | uint32\_t | 属性数组的容量。 |
| <a href="#object_default_t_props_size">props\_size</a> | uint32\_t | 属性个数。 |
#### object\_default\_create 函数
-----------------------

* 函数功能：

> <p id="object_default_t_object_default_create">
 创建对象。






* 函数原型：

```
object_t* object_default_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回object对象。 |
#### props 属性
-----------------------
> <p id="object_default_t_props"> 属性数组。




* 类型：named\_value\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### props\_capacity 属性
-----------------------
> <p id="object_default_t_props_capacity"> 属性数组的容量。




* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### props\_size 属性
-----------------------
> <p id="object_default_t_props_size"> 属性个数。




* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
