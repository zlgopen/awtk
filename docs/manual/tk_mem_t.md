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
| <a href="#tk_mem_t_tk_mem_dump">tk\_mem\_dump</a> | 显示内存信息。 |
| <a href="#tk_mem_t_tk_mem_init">tk\_mem\_init</a> | 初始化内存。 |
| <a href="#tk_mem_t_tk_mem_init_stage2">tk\_mem\_init\_stage2</a> | 初始化互斥和oom。 |
| <a href="#tk_mem_t_tk_mem_is_valid_addr">tk\_mem\_is\_valid\_addr</a> | 检查给定的地址是否是一个有效的heap地址。 |
| <a href="#tk_mem_t_TKMEM_ALLOC">TKMEM\_ALLOC</a> | 分配一块内存。 |
| <a href="#tk_mem_t_TKMEM_CALLOC">TKMEM\_CALLOC</a> | 分配一块内存，并将内容清零。 |
| <a href="#tk_mem_t_TKMEM_FREE">TKMEM\_FREE</a> | 释放内存。 |
| <a href="#tk_mem_t_TKMEM_REALLOC">TKMEM\_REALLOC</a> | 重新分配一块内存，如果原来的内存块大于等于需要的空间，直接返回原来的内存块。 |
#### tk\_mem\_dump 函数
-----------------------

* 函数功能：

> <p id="tk_mem_t_tk_mem_dump">显示内存信息。

* 函数原型：

```
void tk_mem_dump ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | void | 返回void。 |
#### tk\_mem\_init 函数
-----------------------

* 函数功能：

> <p id="tk_mem_t_tk_mem_init">初始化内存。

* 函数原型：

```
ret_t tk_mem_init (void* buffer, uint32_t size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| buffer | void* | 内存地址。 |
| size | uint32\_t | 内存长度。 |
#### tk\_mem\_init\_stage2 函数
-----------------------

* 函数功能：

> <p id="tk_mem_t_tk_mem_init_stage2">初始化互斥和oom。

* 函数原型：

```
ret_t tk_mem_init_stage2 ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### tk\_mem\_is\_valid\_addr 函数
-----------------------

* 函数功能：

> <p id="tk_mem_t_tk_mem_is_valid_addr">检查给定的地址是否是一个有效的heap地址。

> 用于辅助发现内存问题。

* 函数原型：

```
bool_t tk_mem_is_valid_addr ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回FALSE一定是无效地址，返回TRUE在PC则不太确定。 |
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
