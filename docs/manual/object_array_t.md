## object\_array\_t
### 概述
![image](images/object_array_t_0.png)

简单的动态数组，内部存放value对象。

访问时属性名称为：

* "size"/"length" 用于获取数组的长度。
* index 用于访问属性，-1可以用来追加新元素。
----------------------------------
### 函数
<p id="object_array_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#object_array_t_object_array_clear_props">object\_array\_clear\_props</a> | 清除全部属性。 |
| <a href="#object_array_t_object_array_clone">object\_array\_clone</a> | 克隆对象。 |
| <a href="#object_array_t_object_array_create">object\_array\_create</a> | 创建对象。 |
| <a href="#object_array_t_object_array_get">object\_array\_get</a> | 获取指定序数的元素。 |
| <a href="#object_array_t_object_array_insert">object\_array\_insert</a> | 在指定位置插入一个元素。 |
| <a href="#object_array_t_object_array_pop">object\_array\_pop</a> | 弹出一个元素。 |
| <a href="#object_array_t_object_array_push">object\_array\_push</a> | 追加一个元素。 |
| <a href="#object_array_t_object_array_remove">object\_array\_remove</a> | 在指定位置删除一个元素。 |
| <a href="#object_array_t_object_array_set">object\_array\_set</a> | 设置指定序数的元素。 |
| <a href="#object_array_t_object_array_unref">object\_array\_unref</a> | for script gc |
### 属性
<p id="object_array_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#object_array_t_capacity">capacity</a> | uint32\_t | 属性数组的容量。 |
| <a href="#object_array_t_props">props</a> | value\_t | 属性数组。 |
| <a href="#object_array_t_size">size</a> | uint32\_t | 属性个数。 |
#### object\_array\_clear\_props 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_clear_props">清除全部属性。

* 函数原型：

```
ret_t object_array_clear_props (object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
#### object\_array\_clone 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_clone">克隆对象。

* 函数原型：

```
object_t* object_array_clone (object_array_t* o);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回object对象。 |
| o | object\_array\_t* | 被克隆的对象。 |
#### object\_array\_create 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_create">创建对象。

* 函数原型：

```
object_t* object_array_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t* | 返回object对象。 |
#### object\_array\_get 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_get">获取指定序数的元素。

* 函数原型：

```
ret_t object_array_get (object_t* obj, uint32_t i, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| i | uint32\_t | 序数。 |
| v | value\_t* | 返回的元素。 |
#### object\_array\_insert 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_insert">在指定位置插入一个元素。

* 函数原型：

```
ret_t object_array_insert (object_t* obj, uint32_t index, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
| index | uint32\_t | 位置。 |
| v | const value\_t* | 值。 |
#### object\_array\_pop 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_pop">弹出一个元素。

* 函数原型：

```
ret_t object_array_pop (object_t* obj, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
| v | value\_t* | 返回值。 |
#### object\_array\_push 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_push">追加一个元素。

* 函数原型：

```
ret_t object_array_push (object_t* obj, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
| v | const value\_t* | 值。 |
#### object\_array\_remove 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_remove">在指定位置删除一个元素。

* 函数原型：

```
ret_t object_array_remove (object_t* obj, uint32_t index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
| index | uint32\_t | 位置。 |
#### object\_array\_set 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_set">设置指定序数的元素。

* 函数原型：

```
ret_t object_array_set (object_t* obj, uint32_t i, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 数组对象。 |
| i | uint32\_t | 序数。 |
| v | const value\_t* | 元素。 |
#### object\_array\_unref 函数
-----------------------

* 函数功能：

> <p id="object_array_t_object_array_unref">for script gc

* 函数原型：

```
ret_t object_array_unref (object_t* obj);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| obj | object\_t* | 对象。 |
#### capacity 属性
-----------------------
> <p id="object_array_t_capacity">属性数组的容量。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### props 属性
-----------------------
> <p id="object_array_t_props">属性数组。

* 类型：value\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="object_array_t_size">属性个数。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
| 可脚本化   | 是 |
