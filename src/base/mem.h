/**
 * File:   mem.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  memory manager functions.
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

typedef struct _mem_stat_t {
  uint32_t used;
  uint32_t free;
  uint32_t total;
  uint32_t free_block_nr;
  uint32_t used_block_nr;
} mem_stat_t;

ret_t mem_init(void* buffer, uint32_t length);

mem_stat_t mem_stat(void);

void mem_info_dump(void);

#ifdef HAS_STD_MALLOC
#define TKMEM_ALLOC(size) malloc(size)
#define TKMEM_ZALLOC(type) (type*)calloc(1, sizeof(type))
#define TKMEM_ZALLOCN(type, n) (type*)calloc(n, sizeof(type))
#define TKMEM_REALLOC(type, p, n) (type*)realloc(p, (n) * sizeof(type))
#define TKMEM_FREE(p) free(p)
#else
void* tk_calloc(uint32_t nmemb, uint32_t size);
void* tk_realloc(void* ptr, uint32_t size);
void tk_free(void* ptr);
void* tk_alloc(uint32_t size);

#define TKMEM_ALLOC(size) tk_alloc(size)
#define TKMEM_ZALLOC(type) (type*)tk_calloc(1, sizeof(type))
#define TKMEM_ZALLOCN(type, n) (type*)tk_calloc(n, sizeof(type))
#define TKMEM_REALLOC(type, p, n) (type*)tk_realloc(p, (n) * sizeof(type))
#define TKMEM_FREE(p) tk_free(p)
#endif

END_C_DECLS
#endif /*TK_TKMEM_MANAGER_H*/
