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

#ifndef _SB_INTERNAL_ALLOCATOR_H
#define _SB_INTERNAL_ALLOCATOR_H

#include <SheenBidi/SBAllocator.h>

#include <API/SBBase.h>
#include <Core/Object.h>

typedef struct _SBAllocator {
    ObjectBase _base;
    void *_info;
    SBAllocatorProtocol _protocol;
} SBAllocator;

#define SBAllocatorMake(allocateBlock, reallocateBlock, deallocateBlock, \
    allocateScratch, resetScratch, finalize)                             \
{                                                                        \
    ObjectBaseMake(),                                                    \
    NULL,                                                                \
    {                                                                    \
        allocateBlock, reallocateBlock, deallocateBlock,                 \
        allocateScratch, resetScratch, finalize                          \
    }                                                                    \
}

SB_INTERNAL void *SBAllocatorAllocateBlock(SBAllocatorRef allocator, SBUInteger size);
SB_INTERNAL void *SBAllocatorReallocateBlock(SBAllocatorRef allocator, void *pointer, SBUInteger newSize);
SB_INTERNAL void SBAllocatorDeallocateBlock(SBAllocatorRef allocator, void *pointer);

SB_INTERNAL void *SBAllocatorAllocateScratch(SBAllocatorRef allocator, SBUInteger size);
SB_INTERNAL void SBAllocatorResetScratch(SBAllocatorRef allocator);

#endif
