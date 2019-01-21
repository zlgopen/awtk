## array\_t
### 概述
 动态数组，根据元素个数动态调整数组的容量。

 > 本类已经过时，逐步替换成darray。


----------------------------------
### 函数
<p id="array_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#array_t_array_create">array\_create</a> | 创建array对象 |
| <a href="#array_t_array_deinit">array\_deinit</a> | 清除数组中的元素。 |
| <a href="#array_t_array_destroy">array\_destroy</a> | 清除数组中的元素，并释放数组对象。 |
| <a href="#array_t_array_find">array\_find</a> | 查找第一个满足条件的元素。 |
| <a href="#array_t_array_find_index">array\_find\_index</a> | 查找第一个满足条件的元素，并返回位置。 |
| <a href="#array_t_array_init">array\_init</a> | 初始化array对象 |
| <a href="#array_t_array_pop">array\_pop</a> | 弹出最后一个元素。 |
| <a href="#array_t_array_push">array\_push</a> | 在尾巴追加一个元素。 |
| <a href="#array_t_array_remove">array\_remove</a> | 删除第一个满足条件的元素。 |
| <a href="#array_t_array_remove_all">array\_remove\_all</a> | 删除全部满足条件的元素。 |
### 属性
<p id="array_t_properties">

| 名属性称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#array_t_capacity">capacity</a> | uint16\_t | 数组的容量大小。 |
| <a href="#array_t_elms">elms</a> | void** | 数组中的元素。 |
| <a href="#array_t_size">size</a> | uint16\_t | 数组中元素的个数。 |
#### array\_create 函数
-----------------------

* 函数功能：

> <p id="array_t_array_create"> 创建array对象




* 函数原型：

```
array_t* array_create (uint16_t* capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | array\_t* | 数组对象。 |
| capacity | uint16\_t* | 数组的初始容量。 |
#### array\_deinit 函数
-----------------------

* 函数功能：

> <p id="array_t_array_deinit"> 清除数组中的元素。




* 函数原型：

```
void array_deinit (array_t* array);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 无。 |
| array | array\_t* | 数组对象。 |
#### array\_destroy 函数
-----------------------

* 函数功能：

> <p id="array_t_array_destroy"> 清除数组中的元素，并释放数组对象。




* 函数原型：

```
void array_destroy (array_t* array);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 无。 |
| array | array\_t* | 数组对象。 |
#### array\_find 函数
-----------------------

* 函数功能：

> <p id="array_t_array_find"> 查找第一个满足条件的元素。




* 函数原型：

```
void* array_find (array_t* array, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 如果找到，返回满足条件的对象，否则返回NULL。 |
| array | array\_t* | 数组对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL时直接比较指针。 |
| ctx | void* | 比较函数的上下文。 |
#### array\_find\_index 函数
-----------------------

* 函数功能：

> <p id="array_t_array_find_index"> 查找第一个满足条件的元素，并返回位置。




* 函数原型：

```
int array_find_index (array_t* array, tk_compare_t cmp, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | int | 如果找到，返回满足条件的对象的位置，否则返回-1。 |
| array | array\_t* | 数组对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL时直接比较指针。 |
| ctx | void* | 比较函数的上下文。 |
#### array\_init 函数
-----------------------

* 函数功能：

> <p id="array_t_array_init"> 初始化array对象




* 函数原型：

```
array_t* array_init (array_t* array, uint16_t* capacity);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | array\_t* | 数组对象。 |
| array | array\_t* | 数组对象。 |
| capacity | uint16\_t* | 数组的初始容量。 |
#### array\_pop 函数
-----------------------

* 函数功能：

> <p id="array_t_array_pop"> 弹出最后一个元素。




* 函数原型：

```
void* array_pop (array_t* array);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回最后一个元素，失败返回NULL。 |
| array | array\_t* | 数组对象。 |
#### array\_push 函数
-----------------------

* 函数功能：

> <p id="array_t_array_push"> 在尾巴追加一个元素。




* 函数原型：

```
ret_t array_push (array_t* array, void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| array | array\_t* | 数组对象。 |
| data | void* | 待追加的元素。 |
#### array\_remove 函数
-----------------------

* 函数功能：

> <p id="array_t_array_remove"> 删除第一个满足条件的元素。




* 函数原型：

```
ret_t array_remove (array_t* array, tk_compare_t cmp, void* ctx, tk_destroy_t destroy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| array | array\_t* | 数组对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL时直接比较指针。 |
| ctx | void* | 比较函数的上下文。 |
| destroy | tk\_destroy\_t | 销毁元素的回调函数。 |
#### array\_remove\_all 函数
-----------------------

* 函数功能：

> <p id="array_t_array_remove_all"> 删除全部满足条件的元素。




* 函数原型：

```
ret_t array_remove_all (array_t* array, tk_compare_t cmp, void* ctx, tk_destroy_t destroy);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| array | array\_t* | 数组对象。 |
| cmp | tk\_compare\_t | 比较函数，为NULL时直接比较指针。 |
| ctx | void* | 比较函数的上下文。 |
| destroy | tk\_destroy\_t | 销毁元素的回调函数。 |
#### capacity 属性
-----------------------
> <p id="array_t_capacity"> 数组的容量大小。



* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### elms 属性
-----------------------
> <p id="array_t_elms"> 数组中的元素。



* 类型：void**

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### size 属性
-----------------------
> <p id="array_t_size"> 数组中元素的个数。



* 类型：uint16\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
