/*
 * Copyright (C) 2025 Muhammad Tayyab Akram
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _SB_INTERNAL_MEMORY_H
#define _SB_INTERNAL_MEMORY_H

#include <stddef.h>

#include <API/SBBase.h>

/**
 * Represents a single dynamically allocated memory block.
 * Each block stores a pointer to the next block, forming a singly linked list. The usable memory
 * begins immediately after the `MemoryBlock` structure.
 */
typedef struct _MemoryBlock {
    struct _MemoryBlock *next;
} MemoryBlock, *MemoryBlockRef;

/**
 * Represents a linked list of memory blocks used for managing dynamic allocations.
 * The list always includes a sentinel `first` block, and a pointer to the last block to allow
 * efficient appending.
 */
typedef struct _MemoryList {
    MemoryBlock first;
    MemoryBlockRef last;
} MemoryList, *MemoryListRef;

/**
 * Base structure for managing internal memory allocations.
 * Intended to be embedded in other structs (e.g., Object).
 */
typedef struct Memory {
    MemoryListRef _list;
} Memory, *MemoryRef;

enum {
    MemoryTypePermanent = 0,
    MemoryTypeScratch = 1
};
typedef SBUInt8 MemoryType;

#define MemoryMake() { NULL }

/**
 * Initializes a Memory structure to prepare it for allocations.
 *
 * @param memory
 *      The Memory instance to initialize.
 */
SB_INTERNAL void MemoryInitialize(MemoryRef memory);

/**
 * Allocates a single contiguous memory block of the given size. The block is tracked internally
 * and released by `MemoryFinalize()`.
 *
 * @param memory
 *      The Memory to allocate from.
 * @param size
 *      Size of the memory block to allocate.
 * @return
 *      Pointer to the allocated memory block, or NULL on failure.
 */
SB_INTERNAL void *MemoryAllocateBlock(MemoryRef memory, MemoryType type, SBUInteger size);

/**
 * Allocates a single contiguous memory block and splits it into multiple chunks. The entire memory
 * block is tracked internally and released by `MemoryFinalize()`.
 *
 * @param memory
 *      The Memory to allocate from.
 * @param chunkSizes
 *      Array of chunk sizes.
 * @param chunkCount
 *      Number of chunks.
 * @param outPointers
 *      Output array to receive chunk pointers.
 * @return
 *      `SBTrue` if successful, `SBFalse` otherwise.
 */
SB_INTERNAL SBBoolean MemoryAllocateChunks(MemoryRef memory, MemoryType type,
    const SBUInteger *chunkSizes, SBUInteger chunkCount, void **outPointers);

/**
 * Frees all memory blocks tracked by the given Memory.
 *
 * @param memory
 *      The Memory to finalize.
 */
SB_INTERNAL void MemoryFinalize(MemoryRef memory);

#endif
