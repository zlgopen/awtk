## typed\_array\_t
### 概述
带类型的动态数组，根据元素个数动态调整数组的容量。

示例：

```c
value_t v;
typed_array_t* typed_array = typed_array_create(VALUE_TYPE_INT, 10);
typed_array_push(a, value_set_int(&v, 123));
typed_array_push(a, value_set_int(&v, 234));
...
typed_array_destroy(typed_array);
```
----------------------------------
### 函数
<p id="typed_array_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#typed_array_t_typed_array_clear">typed\_array\_clear</a> | 清除全部元素。 |
| <a href="#typed_array_t_typed_array_create">typed\_array\_create</a> | 创建typed_array对象。 |
| <a href="#typed_array_t_typed_array_destroy">typed\_array\_destroy</a> | 销毁typed_array对象。 |
| <a href="#typed_array_t_typed_array_extend">typed\_array\_extend</a> | 扩展typed_array到指定容量。 |
| <a href="#typed_array_t_typed_array_get">typed\_array\_get</a> | 获取指定序数的元素。 |
| <a href="#typed_array_t_typed_array_insert">typed\_array\_insert</a> | 在指定序数插入元素。 |
| <a href="#typed_array_t_typed_array_pop">typed\_array\_pop</a> | 弹出最后一个元素。 |
| <a href="#typed_array_t_typed_array_push">typed\_array\_push</a> | 在尾巴追加一个元素。 |
| <a href="#typed_array_t_typed_array_remove">typed\_array\_remove</a> | 删除指定位置的元素。 |
| <a href="#typed_array_t_typed_array_set">typed\_array\_set</a> | 设置指定序数的元素。 |
| <a href="#typed_array_t_typed_array_tail">typed\_array\_tail</a> | 返回最后一个元素。 |
### 属性
<p id="typed_array_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#typed_array_t_capacity">capacity</a> | uint32\_t | 数组的容量大小。 |
| <a href="#typed_array_t_data">data</a> | uint8\_t* | 数组中的元素。 |
| <a href="#typed_array_t_element_size">element\_size</a> | uint32\_t | 单个元素的大小。 |
| <a href="#typed_array_t_size">size</a> | uint32\_t | 数组中元素的个数。 |
| <a href="#typed_array_t_type">type</a> | value\_type\_t | 元素的类型。 |
#### typed\_array\_clear 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_clear">清除全部元素。

* 函数原型：

```
ret_t typed_array_clear (typed_array_t* typed_array);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| typed\_array | typed\_array\_t* | 数组对象。 |
#### typed\_array\_create 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_create">创建typed_array对象。

* 函数原型：

```
typed_array_t* typed_array_create (value_type_t type, uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | typed\_array\_t* | 数组对象。 |
| type | value\_type\_t | 元素的类型。 |
| capacity | uint32\_t | 数组的初始容量(元素个数)。 |
#### typed\_array\_destroy 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_destroy">销毁typed_array对象。

* 函数原型：

```
ret_t typed_array_destroy (typed_array_t* typed_array);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| typed\_array | typed\_array\_t* | 数组对象。 |
#### typed\_array\_extend 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_extend">扩展typed_array到指定容量。

* 函数原型：

```
ret_t typed_array_extend (value_type_t type, uint32_t capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| type | value\_type\_t | 元素的类型。 |
| capacity | uint32\_t | 数组的容量(元素个数)。 |
#### typed\_array\_get 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_get">获取指定序数的元素。

* 函数原型：

```
ret_t typed_array_get (typed_array_t* typed_array, uint32_t index, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| typed\_array | typed\_array\_t* | 数组对象。 |
| index | uint32\_t | 序数。 |
| v | value\_t* | 返回的元素。 |
#### typed\_array\_insert 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_insert">在指定序数插入元素。

* 函数原型：

```
ret_t typed_array_insert (typed_array_t* typed_array, uint32_t index, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| typed\_array | typed\_array\_t* | 数组对象。 |
| index | uint32\_t | 序数。 |
| v | const value\_t* | 元素。 |
#### typed\_array\_pop 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_pop">弹出最后一个元素。

* 函数原型：

```
void* typed_array_pop (typed_array_t* typed_array);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回最后一个元素，失败返回NULL。 |
| typed\_array | typed\_array\_t* | 数组对象。 |
#### typed\_array\_push 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_push">在尾巴追加一个元素。

* 函数原型：

```
ret_t typed_array_push (typed_array_t* typed_array, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| typed\_array | typed\_array\_t* | 数组对象。 |
| v | const value\_t* | 待追加的元素。 |
#### typed\_array\_remove 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_remove">删除指定位置的元素。

* 函数原型：

```
ret_t typed_array_remove (typed_array_t* typed_array, uint32_t index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| typed\_array | typed\_array\_t* | 数组对象。 |
| index | uint32\_t | 位置序数。 |
#### typed\_array\_set 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_set">设置指定序数的元素。

* 函数原型：

```
ret_t typed_array_set (typed_array_t* typed_array, uint32_t index, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| typed\_array | typed\_array\_t* | 数组对象。 |
| index | uint32\_t | 序数。 |
| v | const value\_t* | 元素。 |
#### typed\_array\_tail 函数
-----------------------

* 函数功能：

> <p id="typed_array_t_typed_array_tail">返回最后一个元素。

* 函数原型：

```
ret_t typed_array_tail (typed_array_t* typed_array, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| typed\_array | typed\_array\_t* | 数组对象。 |
| v | value\_t* | 返回的元素。 |
#### capacity 属性
-----------------------
> <p id="typed_array_t_capacity">数组的容量大小。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### data 属性
-----------------------
> <p id="typed_array_t_data">数组中的元素。

* 类型：uint8\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### element\_size 属性
-----------------------
> <p id="typed_array_t_element_size">单个元素的大小。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="typed_array_t_size">数组中元素的个数。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### type 属性
-----------------------
> <p id="typed_array_t_type">元素的类型。

* 类型：value\_type\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
