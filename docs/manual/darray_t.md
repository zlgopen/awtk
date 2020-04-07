## darray\_t
### 概述
动态数组，根据元素个数动态调整数组的容量。

用darray\_init初始化时，用darray\_deinit释放。如：

```c
darray_t darray;
darray_init(&darray, 10, destroy, compare);
...
darray_deinit(&darray);
```

用darray\_create创建时，用darray\_destroy销毁。如：

```c
darray_t* darray = darray_create(10, destroy, compare);
...
darray_destroy(darray);
```
----------------------------------
### 函数
<p id="darray_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#darray_t_darray_bsearch">darray\_bsearch</a> | 二分查找(确保数组有序)。 |
| <a href="#darray_t_darray_bsearch_index">darray\_bsearch\_index</a> | 二分查找(确保数组有序)。 |
| <a href="#darray_t_darray_clear">darray\_clear</a> | 清除全部元素。 |
| <a href="#darray_t_darray_count">darray\_count</a> | 返回满足条件元素的个数。 |
| <a href="#darray_t_darray_create">darray\_create</a> | 创建darray对象。 |
| <a href="#darray_t_darray_deinit">darray\_deinit</a> | 清除全部元素，并释放elms。 |
| <a href="#darray_t_darray_destroy">darray\_destroy</a> | 销毁darray对象。 |
| <a href="#darray_t_darray_find">darray\_find</a> | 查找第一个满足条件的元素。 |
| <a href="#darray_t_darray_find_all">darray\_find\_all</a> | 查找全部满足条件的元素。 |
| <a href="#darray_t_darray_find_index">darray\_find\_index</a> | 查找第一个满足条件的元素，并返回位置。 |
| <a href="#darray_t_darray_foreach">darray\_foreach</a> | 遍历元素。 |
| <a href="#darray_t_darray_get">darray\_get</a> | 获取指定序数的元素。 |
| <a href="#darray_t_darray_head">darray\_head</a> | 返回第一个元素。 |
| <a href="#darray_t_darray_init">darray\_init</a> | 初始化darray对象。 |
| <a href="#darray_t_darray_pop">darray\_pop</a> | 弹出最后一个元素。 |
| <a href="#darray_t_darray_push">darray\_push</a> | 在尾巴追加一个元素。 |
| <a href="#darray_t_darray_remove">darray\_remove</a> | 删除第一个满足条件的元素。 |
| <a href="#darray_t_darray_remove_all">darray\_remove\_all</a> | 删除全部满足条件的元素。 |
| <a href="#darray_t_darray_remove_index">darray\_remove\_index</a> | 删除指定位置的元素。 |
| <a href="#darray_t_darray_sort">darray\_sort</a> | 排序。 |
| <a href="#darray_t_darray_tail">darray\_tail</a> | 返回最后一个元素。 |
### 属性
<p id="darray_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#darray_t_capacity">capacity</a> | uint32\_t | 数组的容量大小。 |
| <a href="#darray_t_compare">compare</a> | tk\_compare\_t | 元素比较函数。 |
| <a href="#darray_t_destroy">destroy</a> | tk\_destroy\_t | 元素销毁函数。 |
| <a href="#darray_t_elms">elms</a> | void** | 数组中的元素。 |
| <a href="#darray_t_size">size</a> | uint32\_t | 数组中元素的个数。 |
#### darray\_bsearch 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_bsearch">二分查找(确保数组有序)。

* 函数原型：

```
void* darray_bsearch (darray_t* darray, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 如果找到，返回满足条件的对象，否则返回NULL。 |
| darray | darray\_t* | 数组对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL则使用内置的比较函数。 |
| ctx | void* | 比较函数的上下文。 |
#### darray\_bsearch\_index 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_bsearch_index">二分查找(确保数组有序)。

* 函数原型：

```
int darray_bsearch_index (darray_t* darray, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 如果找到，返回满足条件的对象的位置，否则返回-1。 |
| darray | darray\_t* | 数组对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL则使用内置的比较函数。 |
| ctx | void* | 比较函数的上下文。 |
#### darray\_clear 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_clear">清除全部元素。

* 函数原型：

```
ret_t darray_clear (darray_t* darray);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| darray | darray\_t* | 数组对象。 |
#### darray\_count 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_count">返回满足条件元素的个数。

* 函数原型：

```
int32_t darray_count (darray_t* darray, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回元素个数。 |
| darray | darray\_t* | 单向链表对象。 |
| ctx | void* | 比较函数的上下文。 |
#### darray\_create 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_create">创建darray对象。

* 函数原型：

```
darray_t* darray_create (uint32_t capacity, tk_destroy_t destroy, tk_compare_t compare);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | darray\_t* | 数组对象。 |
| capacity | uint32\_t | 数组的初始容量。 |
| destroy | tk\_destroy\_t | 元素销毁函数。 |
| compare | tk\_compare\_t | 元素比较函数。 |
#### darray\_deinit 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_deinit">清除全部元素，并释放elms。

* 函数原型：

```
ret_t darray_deinit (darray_t* darray);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| darray | darray\_t* | 数组对象。 |
#### darray\_destroy 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_destroy">销毁darray对象。

* 函数原型：

```
ret_t darray_destroy (darray_t* darray);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| darray | darray\_t* | 数组对象。 |
#### darray\_find 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_find">查找第一个满足条件的元素。

* 函数原型：

```
void* darray_find (darray_t* darray, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 如果找到，返回满足条件的对象，否则返回NULL。 |
| darray | darray\_t* | 数组对象。 |
| ctx | void* | 比较函数的上下文。 |
#### darray\_find\_all 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_find_all">查找全部满足条件的元素。

```
darray_t matched;
darray_init(&matched, 0, NULL, NULL);
darray_find_all(darray, mycmp, myctx, &matched);
...
darray_deinit(&matched);

```

* 函数原型：

```
ret_t darray_find_all (darray_t* darray, tk_compare_t cmp, void* ctx, darray_t* matched);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| darray | darray\_t* | 数组对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL则使用内置的比较函数。 |
| ctx | void* | 比较函数的上下文。 |
| matched | darray\_t* | 返回满足条件的元素。 |
#### darray\_find\_index 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_find_index">查找第一个满足条件的元素，并返回位置。

* 函数原型：

```
int darray_find_index (darray_t* darray, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 如果找到，返回满足条件的对象的位置，否则返回-1。 |
| darray | darray\_t* | 数组对象。 |
| ctx | void* | 比较函数的上下文。 |
#### darray\_foreach 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_foreach">遍历元素。

* 函数原型：

```
ret_t darray_foreach (darray_t* darray, tk_visit_t visit, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| darray | darray\_t* | 数组对象。 |
| visit | tk\_visit\_t | 遍历函数。 |
| ctx | void* | 遍历函数的上下文。 |
#### darray\_get 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_get">获取指定序数的元素。

* 函数原型：

```
void* darray_get (darray_t* darray, uint32_t index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 返回满足条件的对象，否则返回NULL。 |
| darray | darray\_t* | 数组对象。 |
| index | uint32\_t | 序数。 |
#### darray\_head 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_head">返回第一个元素。

* 函数原型：

```
void* darray_head (darray_t* darray);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回最后一个元素，失败返回NULL。 |
| darray | darray\_t* | 数组对象。 |
#### darray\_init 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_init">初始化darray对象。

* 函数原型：

```
darray_t* darray_init (darray_t* darray, uint32_t* capacity, tk_destroy_t destroy, tk_compare_t compare);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | darray\_t* | 数组对象。 |
| darray | darray\_t* | 数组对象。 |
| capacity | uint32\_t* | 数组的初始容量。 |
| destroy | tk\_destroy\_t | 元素销毁函数。 |
| compare | tk\_compare\_t | 元素比较函数。 |
#### darray\_pop 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_pop">弹出最后一个元素。

* 函数原型：

```
void* darray_pop (darray_t* darray);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回最后一个元素，失败返回NULL。 |
| darray | darray\_t* | 数组对象。 |
#### darray\_push 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_push">在尾巴追加一个元素。

* 函数原型：

```
ret_t darray_push (darray_t* darray, void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| darray | darray\_t* | 数组对象。 |
| data | void* | 待追加的元素。 |
#### darray\_remove 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_remove">删除第一个满足条件的元素。

* 函数原型：

```
ret_t darray_remove (darray_t* darray, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| darray | darray\_t* | 数组对象。 |
| ctx | void* | 比较函数的上下文。 |
#### darray\_remove\_all 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_remove_all">删除全部满足条件的元素。

* 函数原型：

```
ret_t darray_remove_all (darray_t* darray, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| darray | darray\_t* | 数组对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL则使用内置的比较函数。 |
| ctx | void* | 比较函数的上下文。 |
#### darray\_remove\_index 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_remove_index">删除指定位置的元素。

* 函数原型：

```
ret_t darray_remove_index (darray_t* darray, uint32_t index);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| darray | darray\_t* | 数组对象。 |
| index | uint32\_t | 位置序数。 |
#### darray\_sort 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_sort">排序。

* 函数原型：

```
ret_t darray_sort (darray_t* darray, tk_compare_t cmp);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| darray | darray\_t* | 数组对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL则使用内置的比较函数。 |
#### darray\_tail 函数
-----------------------

* 函数功能：

> <p id="darray_t_darray_tail">返回最后一个元素。

* 函数原型：

```
void* darray_tail (darray_t* darray);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回最后一个元素，失败返回NULL。 |
| darray | darray\_t* | 数组对象。 |
#### capacity 属性
-----------------------
> <p id="darray_t_capacity">数组的容量大小。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### compare 属性
-----------------------
> <p id="darray_t_compare">元素比较函数。

* 类型：tk\_compare\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### destroy 属性
-----------------------
> <p id="darray_t_destroy">元素销毁函数。

* 类型：tk\_destroy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### elms 属性
-----------------------
> <p id="darray_t_elms">数组中的元素。

* 类型：void**

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="darray_t_size">数组中元素的个数。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
