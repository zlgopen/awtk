## slist\_t
### 概述
单向链表

用slist\_init初始化时，用slist\_deinit释放。如：

```c
slist_t slist;
slist_init(&slist, destroy, compare);
...
slist_deinit(&slist);
```

用slist\_create创建时，用slist\_destroy销毁。如：

```c
slist_t* slist = slist_create(destroy, compare);
...
slist_destroy(slist);
```
----------------------------------
### 函数
<p id="slist_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#slist_t_slist_append">slist\_append</a> | 在尾巴追加一个元素。 |
| <a href="#slist_t_slist_count">slist\_count</a> | 返回满足条件元素的个数。 |
| <a href="#slist_t_slist_create">slist\_create</a> | 创建slist对象 |
| <a href="#slist_t_slist_deinit">slist\_deinit</a> | 清除单向链表中的元素。 |
| <a href="#slist_t_slist_destroy">slist\_destroy</a> | 清除单向链表中的元素，并释放单向链表对象。 |
| <a href="#slist_t_slist_find">slist\_find</a> | 查找第一个满足条件的元素。 |
| <a href="#slist_t_slist_foreach">slist\_foreach</a> | 遍历元素。 |
| <a href="#slist_t_slist_head_pop">slist\_head\_pop</a> | 弹出第一个元素。 |
| <a href="#slist_t_slist_init">slist\_init</a> | 初始化slist对象 |
| <a href="#slist_t_slist_prepend">slist\_prepend</a> | 在头部追加一个元素。 |
| <a href="#slist_t_slist_remove">slist\_remove</a> | 删除第一个满足条件的元素。 |
| <a href="#slist_t_slist_remove_all">slist\_remove\_all</a> | 删除全部元素。 |
| <a href="#slist_t_slist_size">slist\_size</a> | 返回元素个数。 |
| <a href="#slist_t_slist_tail_pop">slist\_tail\_pop</a> | 弹出最后一个元素。 |
### 属性
<p id="slist_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#slist_t_compare">compare</a> | tk\_compare\_t | 元素比较函数。 |
| <a href="#slist_t_destroy">destroy</a> | tk\_destroy\_t | 元素销毁函数。 |
| <a href="#slist_t_first">first</a> | slist\_node\_t* | 首节点。 |
#### slist\_append 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_append">在尾巴追加一个元素。

* 函数原型：

```
ret_t slist_append (slist_t* slist, void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| slist | slist\_t* | 单向链表对象。 |
| data | void* | 待追加的元素。 |
#### slist\_count 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_count">返回满足条件元素的个数。

* 函数原型：

```
int32_t slist_count (slist_t* slist, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回元素个数。 |
| slist | slist\_t* | 单向链表对象。 |
| ctx | void* | 比较函数的上下文。 |
#### slist\_create 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_create">创建slist对象

* 函数原型：

```
slist_t* slist_create (tk_destroy_t destroy, tk_compare_t compare);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | slist\_t* | 单向链表对象。 |
| destroy | tk\_destroy\_t | 元素销毁函数。 |
| compare | tk\_compare\_t | 元素比较函数。 |
#### slist\_deinit 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_deinit">清除单向链表中的元素。

* 函数原型：

```
ret_t slist_deinit (slist_t* slist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| slist | slist\_t* | 单向链表对象。 |
#### slist\_destroy 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_destroy">清除单向链表中的元素，并释放单向链表对象。

* 函数原型：

```
ret_t slist_destroy (slist_t* slist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| slist | slist\_t* | 单向链表对象。 |
#### slist\_find 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_find">查找第一个满足条件的元素。

* 函数原型：

```
void* slist_find (slist_t* slist, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 如果找到，返回满足条件的对象，否则返回NULL。 |
| slist | slist\_t* | 单向链表对象。 |
| ctx | void* | 比较函数的上下文。 |
#### slist\_foreach 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_foreach">遍历元素。

* 函数原型：

```
ret_t slist_foreach (slist_t* slist, tk_visit_t visit, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| slist | slist\_t* | 单向链表对象。 |
| visit | tk\_visit\_t | 遍历函数。 |
| ctx | void* | 遍历函数的上下文。 |
#### slist\_head\_pop 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_head_pop">弹出第一个元素。

* 函数原型：

```
ret_t slist_head_pop (slist_t* slist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 成功返回最后一个元素，失败返回NULL。 |
| slist | slist\_t* | 单向链表对象。 |
#### slist\_init 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_init">初始化slist对象

* 函数原型：

```
slist_t* slist_init (slist_t* slist, tk_destroy_t destroy, tk_compare_t compare);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | slist\_t* | 单向链表对象。 |
| slist | slist\_t* | 单向链表对象。 |
| destroy | tk\_destroy\_t | 元素销毁函数。 |
| compare | tk\_compare\_t | 元素比较函数。 |
#### slist\_prepend 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_prepend">在头部追加一个元素。

* 函数原型：

```
ret_t slist_prepend (slist_t* slist, void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| slist | slist\_t* | 单向链表对象。 |
| data | void* | 待追加的元素。 |
#### slist\_remove 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_remove">删除第一个满足条件的元素。

* 函数原型：

```
ret_t slist_remove (slist_t* slist, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| slist | slist\_t* | 单向链表对象。 |
| ctx | void* | 比较函数的上下文。 |
#### slist\_remove\_all 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_remove_all">删除全部元素。

* 函数原型：

```
ret_t slist_remove_all (slist_t* slist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| slist | slist\_t* | 单向链表对象。 |
#### slist\_size 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_size">返回元素个数。

* 函数原型：

```
int32_t slist_size (slist_t* slist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int32\_t | 返回元素个数。 |
| slist | slist\_t* | 单向链表对象。 |
#### slist\_tail\_pop 函数
-----------------------

* 函数功能：

> <p id="slist_t_slist_tail_pop">弹出最后一个元素。

* 函数原型：

```
ret_t slist_tail_pop (slist_t* slist);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 成功返回最后一个元素，失败返回NULL。 |
| slist | slist\_t* | 单向链表对象。 |
#### compare 属性
-----------------------
> <p id="slist_t_compare">元素比较函数。

* 类型：tk\_compare\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### destroy 属性
-----------------------
> <p id="slist_t_destroy">元素销毁函数。

* 类型：tk\_destroy\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### first 属性
-----------------------
> <p id="slist_t_first">首节点。

* 类型：slist\_node\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
