## mem\_allocator\_t
### 概述
内存分配接口。
----------------------------------
### 函数
<p id="mem_allocator_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#mem_allocator_t_mem_allocator_alloc">mem\_allocator\_alloc</a> | 分配指定大小的内存。 |
| <a href="#mem_allocator_t_mem_allocator_destroy">mem\_allocator\_destroy</a> | 销毁内存分配器。 |
| <a href="#mem_allocator_t_mem_allocator_dump">mem\_allocator\_dump</a> | 显示内存信息，用于调试。 |
| <a href="#mem_allocator_t_mem_allocator_free">mem\_allocator\_free</a> | 释放内存。 |
| <a href="#mem_allocator_t_mem_allocator_realloc">mem\_allocator\_realloc</a> | 重新分配指定大小的内存。 |
#### mem\_allocator\_alloc 函数
-----------------------

* 函数功能：

> <p id="mem_allocator_t_mem_allocator_alloc">分配指定大小的内存。

* 函数原型：

```
void* mem_allocator_alloc (mem_allocator_t* allocator, uint32_t size, const char* , uint32_t line);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回内存块的地址，失败返回NULL。 |
| allocator | mem\_allocator\_t* | allocator对象。 |
| size | uint32\_t | 内存的大小。 |
|  | const char* | 。 |
| line | uint32\_t | 分配内存的行数(用于调试)。 |
#### mem\_allocator\_destroy 函数
-----------------------

* 函数功能：

> <p id="mem_allocator_t_mem_allocator_destroy">销毁内存分配器。

* 函数原型：

```
ret_t mem_allocator_destroy (mem_allocator_t* allocator);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| allocator | mem\_allocator\_t* | allocator对象。 |
#### mem\_allocator\_dump 函数
-----------------------

* 函数功能：

> <p id="mem_allocator_t_mem_allocator_dump">显示内存信息，用于调试。

* 函数原型：

```
ret_t mem_allocator_dump (mem_allocator_t* allocator);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| allocator | mem\_allocator\_t* | allocator对象。 |
#### mem\_allocator\_free 函数
-----------------------

* 函数功能：

> <p id="mem_allocator_t_mem_allocator_free">释放内存。

* 函数原型：

```
void* mem_allocator_free (mem_allocator_t* allocator, void* ptr);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回内存块的地址，失败返回NULL。 |
| allocator | mem\_allocator\_t* | allocator对象。 |
| ptr | void* | 内存的地址。 |
#### mem\_allocator\_realloc 函数
-----------------------

* 函数功能：

> <p id="mem_allocator_t_mem_allocator_realloc">重新分配指定大小的内存。

* 函数原型：

```
void* mem_allocator_realloc (mem_allocator_t* allocator, void* ptr, uint32_t size, const char* , uint32_t line);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回内存块的地址，失败返回NULL。 |
| allocator | mem\_allocator\_t* | allocator对象。 |
| ptr | void* | 原来内存的地址。 |
| size | uint32\_t | 内存的大小。 |
|  | const char* | 。 |
| line | uint32\_t | 分配内存的行数(用于调试)。 |
