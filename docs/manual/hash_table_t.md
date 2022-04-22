## hash\_table\_t
### 概述
哈希表。

用hash_table\_init初始化时，用hash_table\_deinit释放。如：

```c
hash_table_t hash_table;
hash_table_init(&hash_table, 10, destroy, compare, hash);
...
hash_table_deinit(&hash_table);
```

用hash_table\_create创建时，用hash_table\_destroy销毁。如：

```c
hash_table_t* hash_table = hash_table_create(10, destroy, compare, hash);
...
hash_table_destroy(hash_table);
```
----------------------------------
### 函数
<p id="hash_table_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#hash_table_t_hash_table_add">hash\_table\_add</a> | 加入一个元素。 |
| <a href="#hash_table_t_hash_table_clear">hash\_table\_clear</a> | 清除全部元素。 |
| <a href="#hash_table_t_hash_table_count">hash\_table\_count</a> | 返回满足条件元素的个数。 |
| <a href="#hash_table_t_hash_table_create">hash\_table\_create</a> | 创建hash_table对象。 |
| <a href="#hash_table_t_hash_table_deinit">hash\_table\_deinit</a> | 清除全部元素，并释放elms。 |
| <a href="#hash_table_t_hash_table_destroy">hash\_table\_destroy</a> | 销毁hash_table对象。 |
| <a href="#hash_table_t_hash_table_find">hash\_table\_find</a> | 查找第一个满足条件的元素。 |
| <a href="#hash_table_t_hash_table_foreach">hash\_table\_foreach</a> | 遍历元素。 |
| <a href="#hash_table_t_hash_table_hash_int">hash\_table\_hash\_int</a> | 计算int的hash值。 |
| <a href="#hash_table_t_hash_table_hash_str">hash\_table\_hash\_str</a> | 计算字符串hash值。 |
| <a href="#hash_table_t_hash_table_init">hash\_table\_init</a> | 初始化hash_table对象。 |
| <a href="#hash_table_t_hash_table_remove">hash\_table\_remove</a> | 删除第一个满足条件的元素。 |
| <a href="#hash_table_t_hash_table_remove_all">hash\_table\_remove\_all</a> | 删除全部满足条件的元素。 |
### 属性
<p id="hash_table_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#hash_table_t_buckets">buckets</a> | darray\_t | buckets。 |
| <a href="#hash_table_t_compare">compare</a> | tk\_compare\_t | 元素比较函数。 |
| <a href="#hash_table_t_destroy">destroy</a> | tk\_destroy\_t | 元素销毁函数。 |
| <a href="#hash_table_t_hash">hash</a> | tk\_hash\_t | 元素哈希函数。 |
#### hash\_table\_add 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_add">加入一个元素。

* 函数原型：

```
ret_t hash_table_add (hash_table_t* hash_table, void* data, bool_t replace_if_exist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| hash\_table | hash\_table\_t* | 哈希表对象。 |
| data | void* | 数据。 |
| replace\_if\_exist | bool\_t | 如果存在是否替换。 |
#### hash\_table\_clear 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_clear">清除全部元素。

* 函数原型：

```
ret_t hash_table_clear (hash_table_t* hash_table);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| hash\_table | hash\_table\_t* | 哈希表对象。 |
#### hash\_table\_count 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_count">返回满足条件元素的个数。

* 函数原型：

```
int32_t hash_table_count (hash_table_t* hash_table, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回元素个数。 |
| hash\_table | hash\_table\_t* | 单向链表对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL则使用内置的比较函数。 |
| ctx | void* | 比较函数的上下文。 |
#### hash\_table\_create 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_create">创建hash_table对象。

* 函数原型：

```
hash_table_t* hash_table_create (uint32_t capacity, tk_destroy_t destroy, tk_compare_t compare, tk_hash_t hash);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | hash\_table\_t* | 哈希表对象。 |
| capacity | uint32\_t | 哈希表桶数。 |
| destroy | tk\_destroy\_t | 元素销毁函数。 |
| compare | tk\_compare\_t | 元素比较函数。 |
| hash | tk\_hash\_t | 元素哈希函数。 |
#### hash\_table\_deinit 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_deinit">清除全部元素，并释放elms。

* 函数原型：

```
ret_t hash_table_deinit (hash_table_t* hash_table);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| hash\_table | hash\_table\_t* | 哈希表对象。 |
#### hash\_table\_destroy 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_destroy">销毁hash_table对象。

* 函数原型：

```
ret_t hash_table_destroy (hash_table_t* hash_table);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| hash\_table | hash\_table\_t* | 哈希表对象。 |
#### hash\_table\_find 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_find">查找第一个满足条件的元素。

* 函数原型：

```
void* hash_table_find (hash_table_t* hash_table, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 如果找到，返回满足条件的对象，否则返回NULL。 |
| hash\_table | hash\_table\_t* | 哈希表对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL则使用内置的比较函数。 |
| ctx | void* | 比较函数的上下文。 |
#### hash\_table\_foreach 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_foreach">遍历元素。

* 函数原型：

```
ret_t hash_table_foreach (hash_table_t* hash_table, tk_visit_t visit, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| hash\_table | hash\_table\_t* | 哈希表对象。 |
| visit | tk\_visit\_t | 遍历函数。 |
| ctx | void* | 遍历函数的上下文。 |
#### hash\_table\_hash\_int 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_hash_int">计算int的hash值。

* 函数原型：

```
uint32_t hash_table_hash_int (const void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回hash值。 |
| data | const void* | 数据。 |
#### hash\_table\_hash\_str 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_hash_str">计算字符串hash值。

* 函数原型：

```
uint32_t hash_table_hash_str (const void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回hash值。 |
| data | const void* | 数据。 |
#### hash\_table\_init 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_init">初始化hash_table对象。

* 函数原型：

```
hash_table_t* hash_table_init (hash_table_t* hash_table, uint32_t capacity, tk_destroy_t destroy, tk_compare_t compare, tk_hash_t hash);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | hash\_table\_t* | 哈希表对象。 |
| hash\_table | hash\_table\_t* | 哈希表对象。 |
| capacity | uint32\_t | 哈希表桶数。 |
| destroy | tk\_destroy\_t | 元素销毁函数。 |
| compare | tk\_compare\_t | 元素比较函数。 |
| hash | tk\_hash\_t | 元素哈希函数。 |
#### hash\_table\_remove 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_remove">删除第一个满足条件的元素。

* 函数原型：

```
ret_t hash_table_remove (hash_table_t* hash_table, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| hash\_table | hash\_table\_t* | 哈希表对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL则使用内置的比较函数。 |
| ctx | void* | 比较函数的上下文。 |
#### hash\_table\_remove\_all 函数
-----------------------

* 函数功能：

> <p id="hash_table_t_hash_table_remove_all">删除全部满足条件的元素。

* 函数原型：

```
ret_t hash_table_remove_all (hash_table_t* hash_table, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| hash\_table | hash\_table\_t* | 哈希表对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL则使用内置的比较函数。 |
| ctx | void* | 比较函数的上下文。 |
#### buckets 属性
-----------------------
> <p id="hash_table_t_buckets">buckets。

* 类型：darray\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### compare 属性
-----------------------
> <p id="hash_table_t_compare">元素比较函数。

* 类型：tk\_compare\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### destroy 属性
-----------------------
> <p id="hash_table_t_destroy">元素销毁函数。

* 类型：tk\_destroy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### hash 属性
-----------------------
> <p id="hash_table_t_hash">元素哈希函数。

* 类型：tk\_hash\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
