/**
 * File:   mem_allocator_fixed_block.h
 * Author: AWTK Develop Team
 * Brief:  只能分配固定大小内存块的内存分配器。
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-04-12 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_MEM_ALLOCATOR_FIXED_BLOCK_H
#define TK_MEM_ALLOCATOR_FIXED_BLOCK_H

#include "tkc/mem_allocator.h"

BEGIN_C_DECLS

/**
 * @class mem_allocator_fixed_block_t
 * @parent mem_allocator_t
 * @annotation ["fake"]
 * 只能分配固定大小内存块的内存分配器。
 *
 * 用mem_allocator\_fixed\_block\_create创建时，用mem\_allocator\_destroy销毁。如：
 *
 * ```c
 * mem_allocator_t* allocator = mem_allocator_fixed_block_create(sizeof(int), 32);
 * ...
 * mem_allocator_destroy(allocator);
 * ```
 */

/**
 * @method mem_allocator_fixed_block_create
 * @annotation ["constructor"]
 * 创建固定大小内存块分配器。
 *
 * @param {uint32_t} size 每个内存块的大小。
 * @param {uint32_t} num 内存块的数量。
 *
 * @return {allocator_t*} 返回内存块分配器对象。
 */
mem_allocator_t* mem_allocator_fixed_block_create(uint32_t size, uint32_t num);

/**
 * @method mem_allocator_fixed_block_size
 * 获取分配器的每个内存块的大小。
 * 
 * @param {mem_allocator_t*} allocator 内存块分配器对象。
 *
 * @return {uint32_t} 返回内存块的大小。
 */
uint32_t mem_allocator_fixed_block_size(mem_allocator_t* allocator);

END_C_DECLS

#endif /*TK_MEM_ALLOCATOR_FIXED_BLOCK_H*/
