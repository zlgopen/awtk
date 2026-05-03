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

#include <stddef.h>

#include <API/SBBase.h>
#include <API/SBAllocator.h>
#include <API/SBAssert.h>

#include "Memory.h"

/**
 * Computes the total size required for a set of memory chunks.
 */
static SBUInteger CalculateTotalSize(const SBUInteger *chunkSizes, SBUInteger chunkCount)
{
    SBUInteger totalSize = 0;
    SBUInteger index;

    for (index = 0; index < chunkCount; index++) {
        totalSize += chunkSizes[index];
    }

    return totalSize;
}

/**
 * Splits a contiguous memory block into multiple logical chunks.
 */
static void SplitMemoryBlock(void *pointer,
    const SBUInteger *chunkSizes, SBUInteger chunkCount, void **outPointers)
{
    SBUInt8 *chunkPointer = pointer;
    SBUInteger offset = 0;
    SBUInteger index;

    for (index = 0; index < chunkCount; index++) {
        outPointers[index] = chunkPointer + offset;
        offset += chunkSizes[index];
    }
}

SB_INTERNAL void MemoryInitialize(MemoryRef memory)
{
    memory->_list = NULL;
}

SB_INTERNAL void *MemoryAllocateBlock(MemoryRef memory, MemoryType type, SBUInteger size)
{
    void *pointer = NULL;

    /* Size MUST be greater than zero. */
    SBAssert(size > 0);

    if (type == MemoryTypeScratch) {
        pointer = SBAllocatorAllocateScratch(NULL, size);
    }

    if (!pointer) {
        MemoryListRef memoryList = memory->_list;

        if (memoryList) {
            const SBUInteger headerSize = sizeof(MemoryBlock);

            pointer = SBAllocatorAllocateBlock(NULL, headerSize + size);

            if (pointer) {
                SBUInt8 *base = pointer;
                MemoryBlockRef block;

                block = pointer;
                block->next = NULL;

                memoryList->last->next = block;
                memoryList->last = block;

                pointer = base + headerSize;
            }
        } else {
            const SBUInteger headerSize = sizeof(MemoryList);

            pointer = SBAllocatorAllocateBlock(NULL, headerSize + size);

            if (pointer) {
                SBUInt8 *base = pointer;

                memoryList = pointer;
                memoryList->first.next = NULL;
                memoryList->last = &memoryList->first;

                memory->_list = memoryList;

                pointer = base + headerSize;
            }
        }
    }

    return pointer;
}

SB_INTERNAL SBBoolean MemoryAllocateChunks(MemoryRef memory, MemoryType type,
    const SBUInteger *chunkSizes, SBUInteger chunkCount, void **outPointers)
{
    SBUInteger totalSize = CalculateTotalSize(chunkSizes, chunkCount);
    SBBoolean succeeded = SBFalse;
    void *pointer;

    /* Total size MUST be greater than zero. */
    SBAssert(totalSize > 0);

    pointer = MemoryAllocateBlock(memory, type, totalSize);

    if (pointer) {
        SplitMemoryBlock(pointer, chunkSizes, chunkCount, outPointers);
        succeeded = SBTrue;
    }

    return succeeded;
}

SB_INTERNAL void MemoryFinalize(MemoryRef memory)
{
    MemoryListRef memoryList = memory->_list;

    if (memoryList) {
        MemoryBlockRef block = &memoryList->first;

        while (block) {
            MemoryBlockRef next = block->next;
            /* Deallocate the block along with its data as they were allocated together. */
            SBAllocatorDeallocateBlock(NULL, block);

            block = next;
        }
    }
}
