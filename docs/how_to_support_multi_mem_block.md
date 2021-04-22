# AWTK 内存分配器支持多块不连续的内存。

在嵌入式系统中，可能有多块不连续的内存。AWTK 最新版本支持管理多个不连续的内存块。使用方法如下：

* 定义内存块数目

> 在 awtk\_config.h 中定义：

```c
#define TK_MAX_MEM_BLOCK_NR 3
```

* 初始化内存

> 需要使用 tk\_mem\_init\_ex 代替 tk\_mem\_init 初始化内存。一般将大块放到前面，小块放到后面。也可以将速度快的内存放到前面，将速度慢的放到后面。

```c
/**
 * @method tk_mem_init_ex
 * @export none
 * 初始化内存，支持多块不连续的内存。
 * >最后一个参数必须为NULL。
 *
 * 示例：
 * ```c
 * tk_mem_init_ex(mem1, sizeof(mem1), mem2, sizeof(mem2), mem3, sizeof(mem3), NULL);
 * ```
 *
 * @param {void*} buffer 内存地址。
 * @param {uint32_t} size 内存长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mem_init_ex(void* buffer, uint32_t size, ...);

```

示例：
```c
 tk_mem_init_ex(mem1, sizeof(mem1), mem2, sizeof(mem2), mem3, sizeof(mem3), NULL);
```
