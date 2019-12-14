## tk\_mem\_t
### 概述
内存管理相关的宏和函数。

示例：

```c
char* str = (char*)TKMEM_ALLOC(100);
...
TKMEM_FREE(str);
```

```c
char* str = (char*)TKMEM_ALLOC(100);
...
str = (char*)TKMEM_REALLOC(str, 128);
...
TKMEM_FREE(str);
```
----------------------------------
### 函数
<p id="tk_mem_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#tk_mem_t_TKMEM_ALLOC">TKMEM\_ALLOC</a> | 分配一块内存。 |
| <a href="#tk_mem_t_TKMEM_CALLOC">TKMEM\_CALLOC</a> | 分配一块内存，并将内容清零。 |
| <a href="#tk_mem_t_TKMEM_FREE">TKMEM\_FREE</a> | 释放内存。 |
| <a href="#tk_mem_t_TKMEM_REALLOC">TKMEM\_REALLOC</a> | 重新分配一块内存，如果原来的内存块大于等于需要的空间，直接返回原来的内存块。 |
#### TKMEM\_ALLOC 宏
-----------------------

* 函数功能：

> <p id="tk_mem_t_TKMEM_ALLOC">分配一块内存。

* 函数原型：

```
void* TKMEM_ALLOC (uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回内存块首地址，失败返回NULL。 |
| size | uint32\_t | 内存大小。 |
#### TKMEM\_CALLOC 宏
-----------------------

* 函数功能：

> <p id="tk_mem_t_TKMEM_CALLOC">分配一块内存，并将内容清零。

* 函数原型：

```
void* TKMEM_CALLOC (uint32_t nmemb, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回内存块首地址，失败返回NULL。 |
| nmemb | uint32\_t | 内存块数量。 |
| size | uint32\_t | 每一块的大小。 |
#### TKMEM\_FREE 宏
-----------------------

* 函数功能：

> <p id="tk_mem_t_TKMEM_FREE">释放内存。

* 函数原型：

```
void TKMEM_FREE (void* p);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 无。 |
| p | void* | 内存地址。 |
#### TKMEM\_REALLOC 宏
-----------------------

* 函数功能：

> <p id="tk_mem_t_TKMEM_REALLOC">重新分配一块内存，如果原来的内存块大于等于需要的空间，直接返回原来的内存块。

* 函数原型：

```
void* TKMEM_REALLOC (void* p, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void* | 成功返回内存块首地址，失败返回NULL。 |
| p | void* | 原来的内存地址。 |
| size | uint32\_t | 每一块的大小。 |
