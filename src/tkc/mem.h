/**
 * File:   mem.h
 * Author: AWTK Develop Team
 * Brief:  memory manager functions.
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TKMEM_MANAGER_H
#define TK_TKMEM_MANAGER_H

#include "tkc/types_def.h"
#include "tkc/platform.h"

BEGIN_C_DECLS

typedef ret_t (*tk_mem_on_out_of_memory_t)(void* ctx, uint32_t tried_times, uint32_t need_size);
ret_t tk_mem_set_on_out_of_memory(tk_mem_on_out_of_memory_t on_out_of_memory, void* ctx);

void* tk_calloc(uint32_t nmemb, uint32_t size, const char* func, uint32_t line);
void* tk_realloc(void* ptr, uint32_t size, const char* func, uint32_t line);
void* tk_alloc(uint32_t size, const char* func, uint32_t line);
void tk_free(void* ptr);

/**
 * @class tk_mem_t
 * @annotation ["fake"]
 * 内存管理相关的宏和函数。
 *
 * 示例：
 *
 * ```c
 * char* str = (char*)TKMEM_ALLOC(100);
 * ...
 * TKMEM_FREE(str);
 * ```
 *
 * ```c
 * char* str = (char*)TKMEM_ALLOC(100);
 * ...
 * str = (char*)TKMEM_REALLOC(str, 128);
 * ...
 * TKMEM_FREE(str);
 * ```
 */

/**
 * @method TKMEM_ALLOC
 * @export tk_alloc
 * 分配一块内存。
 *
 * @annotation ["macro"]
 * @param {uint32_t} size 内存大小。
 *
 * @return {void*} 成功返回内存块首地址，失败返回NULL。
 */
#define TKMEM_ALLOC(size) tk_alloc(size, __FUNCTION__, __LINE__)

/**
 * @method TKMEM_CALLOC
 * @export tk_calloc
 * 分配一块内存，并将内容清零。
 *
 * @annotation ["macro"]
 * @param {uint32_t} nmemb 内存块数量。
 * @param {uint32_t} size 每一块的大小。
 *
 * @return {void*} 成功返回内存块首地址，失败返回NULL。
 */
#define TKMEM_CALLOC(nmemb, size) tk_calloc(nmemb, size, __FUNCTION__, __LINE__)

/**
 * @method TKMEM_REALLOC
 * @export tk_realloc
 * 重新分配一块内存，如果原来的内存块大于等于需要的空间，直接返回原来的内存块。
 *
 * @annotation ["macro"]
 * @param {void*} p 原来的内存地址。
 * @param {uint32_t} size 每一块的大小。
 *
 * @return {void*} 成功返回内存块首地址，失败返回NULL。
 */
#define TKMEM_REALLOC(p, size) tk_realloc(p, size, __FUNCTION__, __LINE__)

/**
 * @method TKMEM_FREE
 * @export tk_free
 * 释放内存。
 *
 * @annotation ["macro"]
 * @param {void*} p 内存地址。
 *
 * @return {void} 无。
 */
#ifdef WITH_CPPCHECK
#define TKMEM_FREE tk_free
#else
#define TKMEM_FREE(p)  \
  {                    \
    tk_free((void*)p); \
    p = NULL;          \
  }
#endif /*WITH_CPPCHECK*/

/*helpler*/
#define TKMEM_ZALLOC(type) (type*)tk_calloc(1, sizeof(type), __FUNCTION__, __LINE__)
#define TKMEM_ZALLOCN(type, n) (type*)tk_calloc(n, sizeof(type), __FUNCTION__, __LINE__)
#define TKMEM_REALLOCT(type, p, n) (type*)tk_realloc(p, (n) * sizeof(type), __FUNCTION__, __LINE__)

/**
 * @method tk_mem_dump
 * 显示内存信息。
 *
 * @return {void} 返回void。
 */
void tk_mem_dump(void);

/**
 * @method tk_mem_init
 * @export none
 * 初始化内存。
 *
 * @param {void*} buffer 内存地址。
 * @param {uint32_t} size 内存长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mem_init(void* buffer, uint32_t size);

/**
 * @method tk_mem_init_stage2
 * @export none
 * 初始化互斥和oom。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_mem_init_stage2(void);

/**
 * @method tk_mem_is_valid_addr
 * 检查给定的地址是否是一个有效的heap地址。
 * 
 * > 用于辅助发现内存问题。
 *
 * @return {bool_t} 返回FALSE一定是无效地址，返回TRUE在PC则不太确定。
 */
bool_t tk_mem_is_valid_addr(void* addr);

#define TK_IS_VALID_ADDR(addr) tk_mem_is_valid_addr(addr)

END_C_DECLS

#endif /*TK_TKMEM_MANAGER_H*/
