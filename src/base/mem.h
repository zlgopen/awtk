/**
 * File:   mem.h
 * Author: AWTK Develop Team
 * Brief:  memory manager functions.
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/types_def.h"

BEGIN_C_DECLS

#ifdef HAS_STD_MALLOC
#define TKMEM_INIT(size)
#else
#define TKMEM_INIT(size)               \
  {                                    \
    static uint32_t mheap[size >> 2];  \
    tk_mem_init(mheap, sizeof(mheap)); \
  }
#endif /*HAS_STD_MALLOC*/

ret_t tk_mem_init(void* buffer, uint32_t length);
void* tk_calloc(uint32_t nmemb, uint32_t size, const char* func, uint32_t line);
void* tk_realloc(void* ptr, uint32_t size, const char* func, uint32_t line);
void* tk_alloc(uint32_t size, const char* func, uint32_t line);
void tk_free(void* ptr);

#define TKMEM_ALLOC(size) tk_alloc(size, __FUNCTION__, __LINE__)
#define TKMEM_CALLOC(nmemb, size) tk_calloc(nmemb, size, __FUNCTION__, __LINE__)
#define TKMEM_ZALLOC(type) (type*)tk_calloc(1, sizeof(type), __FUNCTION__, __LINE__)
#define TKMEM_ZALLOCN(type, n) (type*)tk_calloc(n, sizeof(type), __FUNCTION__, __LINE__)
#define TKMEM_REALLOC(type, p, n) (type*)tk_realloc(p, (n) * sizeof(type), __FUNCTION__, __LINE__)
#define TKMEM_FREE(p) \
  tk_free((void*)p);  \
  p = NULL;

/*for memory debug*/
typedef struct _mem_stat_t {
  uint32_t used_bytes;
  uint32_t used_block_nr;
} mem_stat_t;

void tk_mem_dump(void);
mem_stat_t tk_mem_stat(void);

END_C_DECLS
#endif /*TK_TKMEM_MANAGER_H*/
