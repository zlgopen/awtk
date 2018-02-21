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

#ifndef LFTK_MEM_MANAGER_H
#define LFTK_MEM_MANAGER_H

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

void* mem_calloc(uint32_t nmemb, uint32_t size);
void* meme_realloc(void* ptr, uint32_t size);
void  mem_free(void* ptr);
void* mem_alloc(uint32_t size);

END_C_DECLS

#define MEM_ALLOC(size) mem_alloc(size)
#define MEM_ZALLOC(type) (type*)mem_calloc(1, sizeof(type))
#define MEM_ZALLOCN(type, n) (type*)mem_calloc(n, sizeof(type))
#define MEM_REALLOC(type, p, n) (type*)meme_realloc(p, (n) * sizeof(type))
#define MEM_FREE(p) mem_free(p)

#endif/*LFTK_MEM_MANAGER_H*/

