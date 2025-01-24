## tk\_atomic\_t
### 概述
@export none
原子操作类。
----------------------------------
### 函数
<p id="tk_atomic_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_atomic_t_tk_atomic_compare_exchange_strong">tk\_atomic\_compare\_exchange\_strong</a> | 原子比较交换操作(Compare And Swap)。 |
| <a href="#tk_atomic_t_tk_atomic_compare_exchange_weak">tk\_atomic\_compare\_exchange\_weak</a> | 原子比较交换操作(Compare And Swap)。 |
| <a href="#tk_atomic_t_tk_atomic_create">tk\_atomic\_create</a> | 创建原子操作类对象。 |
| <a href="#tk_atomic_t_tk_atomic_deinit">tk\_atomic\_deinit</a> | 释放原子操作类对象。 |
| <a href="#tk_atomic_t_tk_atomic_destroy">tk\_atomic\_destroy</a> | 销毁原子操作类对象。 |
| <a href="#tk_atomic_t_tk_atomic_exchange">tk\_atomic\_exchange</a> | 原子交换操作。 |
| <a href="#tk_atomic_t_tk_atomic_fetch_add">tk\_atomic\_fetch\_add</a> | 原子加操作。 |
| <a href="#tk_atomic_t_tk_atomic_fetch_sub">tk\_atomic\_fetch\_sub</a> | 原子减操作。 |
| <a href="#tk_atomic_t_tk_atomic_init">tk\_atomic\_init</a> | 初始化原子操作类对象。 |
| <a href="#tk_atomic_t_tk_atomic_load">tk\_atomic\_load</a> | 原子读操作。 |
| <a href="#tk_atomic_t_tk_atomic_store">tk\_atomic\_store</a> | 原子写操作。 |
| <a href="#tk_atomic_t_tk_atomic_support_value_type">tk\_atomic\_support\_value\_type</a> | 判断原子操作是否支持该类型。 |
#### tk\_atomic\_compare\_exchange\_strong 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_compare_exchange_strong">原子比较交换操作(Compare And Swap)。
> 如果 atomic 等于 expect，则将 atomic 改为 desire, 否则将 expect 改为 atomic。

* 函数原型：

```
bool_t tk_atomic_compare_exchange_strong (tk_atomic_t* atomic, value_t* expect, value_t* desire);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示成功，否则表示失败。 |
| atomic | tk\_atomic\_t* | 原子操作类对象。 |
| expect | value\_t* | 期望值。 |
| desire | value\_t* | 设定值。 |
#### tk\_atomic\_compare\_exchange\_weak 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_compare_exchange_weak">原子比较交换操作(Compare And Swap)。
> 如果 atomic 等于 expect，则将 atomic 改为 desire, 否则将 expect 改为 atomic。

weak版本的CAS允许偶然出乎意料的返回（比如在字段值和期待值一样的时候却返回了FALSE），
不过在一些循环算法中，这是可以接受的。通常它比起strong有更高的性能。

* 函数原型：

```
bool_t tk_atomic_compare_exchange_weak (tk_atomic_t* atomic, value_t* expect, value_t* desire);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示成功，否则表示失败。 |
| atomic | tk\_atomic\_t* | 原子操作类对象。 |
| expect | value\_t* | 期望值。 |
| desire | value\_t* | 设定值。 |
#### tk\_atomic\_create 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_create">创建原子操作类对象。

* 函数原型：

```
tk_atomic_t* tk_atomic_create (const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | tk\_atomic\_t* | 返回原子操作类对象。 |
| v | const value\_t* | 值。 |
#### tk\_atomic\_deinit 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_deinit">释放原子操作类对象。

* 函数原型：

```
ret_t tk_atomic_deinit (tk_atomic_t* atomic);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| atomic | tk\_atomic\_t* | 原子操作类对象。 |
#### tk\_atomic\_destroy 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_destroy">销毁原子操作类对象。

* 函数原型：

```
ret_t tk_atomic_destroy (tk_atomic_t* atomic);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| atomic | tk\_atomic\_t* | 原子操作类对象。 |
#### tk\_atomic\_exchange 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_exchange">原子交换操作。

* 函数原型：

```
ret_t tk_atomic_exchange (tk_atomic_t* atomic, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| atomic | tk\_atomic\_t* | 原子操作类对象。 |
| v | value\_t* | 交换值。 |
#### tk\_atomic\_fetch\_add 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_fetch_add">原子加操作。

* 函数原型：

```
ret_t tk_atomic_fetch_add (tk_atomic_t* atomic, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| atomic | tk\_atomic\_t* | 原子操作类对象。 |
| v | value\_t* | 值。 |
#### tk\_atomic\_fetch\_sub 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_fetch_sub">原子减操作。

* 函数原型：

```
ret_t tk_atomic_fetch_sub (tk_atomic_t* atomic, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| atomic | tk\_atomic\_t* | 原子操作类对象。 |
| v | value\_t* | 值。 |
#### tk\_atomic\_init 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_init">初始化原子操作类对象。

* 函数原型：

```
ret_t tk_atomic_init (tk_atomic_t* atomic, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| atomic | tk\_atomic\_t* | 原子操作类对象。 |
| v | const value\_t* | 值。 |
#### tk\_atomic\_load 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_load">原子读操作。

* 函数原型：

```
ret_t tk_atomic_load (const tk_atomic_t* atomic, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| atomic | const tk\_atomic\_t* | 原子操作类对象。 |
| v | value\_t* | 用于返回读取值。 |
#### tk\_atomic\_store 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_store">原子写操作。

* 函数原型：

```
ret_t tk_atomic_store (tk_atomic_t* atomic, const value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| atomic | tk\_atomic\_t* | 原子操作类对象。 |
| v | const value\_t* | 写入值。 |
#### tk\_atomic\_support\_value\_type 函数
-----------------------

* 函数功能：

> <p id="tk_atomic_t_tk_atomic_support_value_type">判断原子操作是否支持该类型。

* 函数原型：

```
bool_t tk_atomic_support_value_type (value_type_t type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回 TRUE 表示支持，FALSE 表示不支持。 |
| type | value\_type\_t | 类型。 |
