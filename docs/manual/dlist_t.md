## dlist\_t
### 概述
双向链表

用dlist\_init初始化时，用dlist\_deinit释放。如：

```c
dlist_t dlist;
dlist_init(&dlist, destroy, compare);
...
dlist_deinit(&dlist);
```

用dlist\_create创建时，用dlist\_destroy销毁。如：

```c
dlist_t* dlist = dlist_create(destroy, compare);
...
dlist_destroy(dlist);
```
----------------------------------
### 函数
<p id="dlist_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#dlist_t_dlist_append">dlist\_append</a> | 在尾巴追加一个元素。 |
| <a href="#dlist_t_dlist_count">dlist\_count</a> | 返回满足条件元素的个数。 |
| <a href="#dlist_t_dlist_create">dlist\_create</a> | 创建dlist对象 |
| <a href="#dlist_t_dlist_deinit">dlist\_deinit</a> | 清除双向链表中的元素。 |
| <a href="#dlist_t_dlist_destroy">dlist\_destroy</a> | 清除双向链表中的元素，并释放双向链表对象。 |
| <a href="#dlist_t_dlist_find">dlist\_find</a> | 查找第一个满足条件的元素。 |
| <a href="#dlist_t_dlist_find_ex">dlist\_find\_ex</a> | 查找第一个满足条件的元素。 |
| <a href="#dlist_t_dlist_find_last">dlist\_find\_last</a> | 查找最后一个满足条件的元素。 |
| <a href="#dlist_t_dlist_foreach">dlist\_foreach</a> | 遍历元素。 |
| <a href="#dlist_t_dlist_foreach_reverse">dlist\_foreach\_reverse</a> | 反向遍历元素。 |
| <a href="#dlist_t_dlist_head">dlist\_head</a> | 返回第一个元素。 |
| <a href="#dlist_t_dlist_head_pop">dlist\_head\_pop</a> | 弹出第一个元素。 |
| <a href="#dlist_t_dlist_init">dlist\_init</a> | 初始化dlist对象 |
| <a href="#dlist_t_dlist_insert">dlist\_insert</a> | 插入一个元素。 |
| <a href="#dlist_t_dlist_is_empty">dlist\_is\_empty</a> | 列表是否为空。 |
| <a href="#dlist_t_dlist_prepend">dlist\_prepend</a> | 在头部追加一个元素。 |
| <a href="#dlist_t_dlist_remove">dlist\_remove</a> | 删除第一个满足条件的元素。 |
| <a href="#dlist_t_dlist_remove_all">dlist\_remove\_all</a> | 删除全部元素。 |
| <a href="#dlist_t_dlist_remove_ex">dlist\_remove\_ex</a> | 删除满足条件的元素。 |
| <a href="#dlist_t_dlist_remove_last">dlist\_remove\_last</a> | 删除最后一个满足条件的元素。 |
| <a href="#dlist_t_dlist_reverse">dlist\_reverse</a> | 反转链表。 |
| <a href="#dlist_t_dlist_size">dlist\_size</a> | 返回元素个数。 |
| <a href="#dlist_t_dlist_tail">dlist\_tail</a> | 返回最后一个元素。 |
| <a href="#dlist_t_dlist_tail_pop">dlist\_tail\_pop</a> | 弹出最后一个元素。 |
### 属性
<p id="dlist_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#dlist_t_compare">compare</a> | tk\_compare\_t | 元素比较函数。 |
| <a href="#dlist_t_destroy">destroy</a> | tk\_destroy\_t | 元素销毁函数。 |
| <a href="#dlist_t_first">first</a> | dlist\_node\_t* | 首节点。 |
| <a href="#dlist_t_last">last</a> | dlist\_node\_t* | 尾节点。 |
| <a href="#dlist_t_size">size</a> | int32\_t | 元素个数。 |
#### dlist\_append 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_append">在尾巴追加一个元素。

* 函数原型：

```
ret_t dlist_append (dlist_t* dlist, void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
| data | void* | 待追加的元素。 |
#### dlist\_count 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_count">返回满足条件元素的个数。

* 函数原型：

```
int32_t dlist_count (dlist_t* dlist, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回元素个数。 |
| dlist | dlist\_t* | 双向链表对象。 |
| ctx | void* | 比较函数的上下文。 |
#### dlist\_create 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_create">创建dlist对象

* 函数原型：

```
dlist_t* dlist_create (tk_destroy_t destroy, tk_compare_t compare);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | dlist\_t* | 双向链表对象。 |
| destroy | tk\_destroy\_t | 元素销毁函数。 |
| compare | tk\_compare\_t | 元素比较函数。 |
#### dlist\_deinit 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_deinit">清除双向链表中的元素。

* 函数原型：

```
ret_t dlist_deinit (dlist_t* dlist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
#### dlist\_destroy 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_destroy">清除双向链表中的元素，并释放双向链表对象。

* 函数原型：

```
ret_t dlist_destroy (dlist_t* dlist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
#### dlist\_find 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_find">查找第一个满足条件的元素。

* 函数原型：

```
void* dlist_find (dlist_t* dlist, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 如果找到，返回满足条件的对象，否则返回NULL。 |
| dlist | dlist\_t* | 双向链表对象。 |
| ctx | void* | 比较函数的上下文。 |
#### dlist\_find\_ex 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_find_ex">查找第一个满足条件的元素。

* 函数原型：

```
void* dlist_find_ex (dlist_t* dlist, tk_compare_t compare, void* ctx, bool_t reverse);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 返回节点。 |
| dlist | dlist\_t* | 双向链表对象。 |
| compare | tk\_compare\_t | 元素比较函数。 |
| ctx | void* | 比较函数的上下文。 |
| reverse | bool\_t | 逆序查找。 |
#### dlist\_find\_last 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_find_last">查找最后一个满足条件的元素。

* 函数原型：

```
void* dlist_find_last (dlist_t* dlist, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 如果找到，返回满足条件的对象，否则返回NULL。 |
| dlist | dlist\_t* | 双向链表对象。 |
| ctx | void* | 比较函数的上下文。 |
#### dlist\_foreach 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_foreach">遍历元素。

* 函数原型：

```
ret_t dlist_foreach (dlist_t* dlist, tk_visit_t visit, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
| visit | tk\_visit\_t | 遍历函数。 |
| ctx | void* | 遍历函数的上下文。 |
#### dlist\_foreach\_reverse 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_foreach_reverse">反向遍历元素。

* 函数原型：

```
ret_t dlist_foreach_reverse (dlist_t* dlist, tk_visit_t visit, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
| visit | tk\_visit\_t | 遍历函数。 |
| ctx | void* | 遍历函数的上下文。 |
#### dlist\_head 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_head">返回第一个元素。

* 函数原型：

```
void* dlist_head (dlist_t* dlist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回最后一个元素，失败返回NULL。 |
| dlist | dlist\_t* | 双向链表对象。 |
#### dlist\_head\_pop 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_head_pop">弹出第一个元素。

* 函数原型：

```
void* dlist_head_pop (dlist_t* dlist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回最后一个元素，失败返回NULL。 |
| dlist | dlist\_t* | 双向链表对象。 |
#### dlist\_init 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_init">初始化dlist对象

* 函数原型：

```
dlist_t* dlist_init (dlist_t* dlist, tk_destroy_t destroy, tk_compare_t compare);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | dlist\_t* | 双向链表对象。 |
| dlist | dlist\_t* | 双向链表对象。 |
| destroy | tk\_destroy\_t | 元素销毁函数。 |
| compare | tk\_compare\_t | 元素比较函数。 |
#### dlist\_insert 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_insert">插入一个元素。

* 函数原型：

```
ret_t dlist_insert (dlist_t* dlist, uint32_t index, void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
| index | uint32\_t | 位置序数。 |
| data | void* | 待追加的元素。 |
#### dlist\_is\_empty 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_is_empty">列表是否为空。

* 函数原型：

```
bool_t dlist_is_empty (dlist_t* dlist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回 TRUE 表示空列表，返回 FALSE 表示列表有数据。 |
| dlist | dlist\_t* | 双向链表对象。 |
#### dlist\_prepend 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_prepend">在头部追加一个元素。

* 函数原型：

```
ret_t dlist_prepend (dlist_t* dlist, void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
| data | void* | 待追加的元素。 |
#### dlist\_remove 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_remove">删除第一个满足条件的元素。

* 函数原型：

```
ret_t dlist_remove (dlist_t* dlist, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
| ctx | void* | 比较函数的上下文。 |
#### dlist\_remove\_all 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_remove_all">删除全部元素。

* 函数原型：

```
ret_t dlist_remove_all (dlist_t* dlist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
#### dlist\_remove\_ex 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_remove_ex">删除满足条件的元素。
备注：
如果队列中符合条件的元素不足 remove_size，移除最后一个符合条件的元素后返回 RET_OK。
如果队列中符合条件的元素大于 remove_size，在队列中移除 remove_size 个元素后返回 RET_OK。
remove_size 为负数则会移除所有符合条件的元素。

* 函数原型：

```
ret_t dlist_remove_ex (dlist_t* dlist, tk_compare_t compare, void* ctx, int32_t remove_size, bool_t reverse);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
| compare | tk\_compare\_t | 元素比较函数。 |
| ctx | void* | 比较函数的上下文。 |
| remove\_size | int32\_t | 删除个数。 |
| reverse | bool\_t | 逆序删除。 |
#### dlist\_remove\_last 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_remove_last">删除最后一个满足条件的元素。

* 函数原型：

```
ret_t dlist_remove_last (dlist_t* dlist, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
| ctx | void* | 比较函数的上下文。 |
#### dlist\_reverse 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_reverse">反转链表。

* 函数原型：

```
ret_t dlist_reverse (dlist_t* dlist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| dlist | dlist\_t* | 双向链表对象。 |
#### dlist\_size 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_size">返回元素个数。

* 函数原型：

```
int32_t dlist_size (dlist_t* dlist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回元素个数。 |
| dlist | dlist\_t* | 双向链表对象。 |
#### dlist\_tail 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_tail">返回最后一个元素。

* 函数原型：

```
void* dlist_tail (dlist_t* dlist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回最后一个元素，失败返回NULL。 |
| dlist | dlist\_t* | 双向链表对象。 |
#### dlist\_tail\_pop 函数
-----------------------

* 函数功能：

> <p id="dlist_t_dlist_tail_pop">弹出最后一个元素。

* 函数原型：

```
void* dlist_tail_pop (dlist_t* dlist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回最后一个元素，失败返回NULL。 |
| dlist | dlist\_t* | 双向链表对象。 |
#### compare 属性
-----------------------
> <p id="dlist_t_compare">元素比较函数。

* 类型：tk\_compare\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### destroy 属性
-----------------------
> <p id="dlist_t_destroy">元素销毁函数。

* 类型：tk\_destroy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### first 属性
-----------------------
> <p id="dlist_t_first">首节点。

* 类型：dlist\_node\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### last 属性
-----------------------
> <p id="dlist_t_last">尾节点。

* 类型：dlist\_node\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="dlist_t_size">元素个数。

* 类型：int32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
