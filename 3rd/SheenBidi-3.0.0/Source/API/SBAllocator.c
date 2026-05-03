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
#include <stdlib.h>

#include <API/SBBase.h>
#include <Core/AtomicPointer.h>
#include <Core/Object.h>
#include <Core/Once.h>
#include <Core/ThreadLocalStorage.h>

#include "SBAllocator.h"

typedef SBAllocator *SBMutableAllocatorRef;
typedef AtomicPointerType(SBAllocator) SBAtomicAllocatorRef;

static SBAtomicAllocatorRef DefaultAllocator = NULL;

#ifndef SB_CONFIG_DISABLE_SCRATCH_MEMORY

#if defined(HAS_ATOMIC_POINTER_SUPPORT) && defined(HAS_TLS_SUPPORT) && defined(HAS_ONCE_SUPPORT)
#define USE_SCRATCH_MEMORY
#else
#error "Scratch memory functionality requires atomic operations, thread-local, and once support. \
To proceed without scratch memory, manually define `SB_CONFIG_DISABLE_SCRATCH_MEMORY`."
#endif

#endif

#ifdef USE_SCRATCH_MEMORY

typedef struct _Buffer {
    struct _Buffer *next;
    SBUInteger offset;
    SBUInt8 data[SB_CONFIG_SCRATCH_BUFFER_SIZE];
} Buffer, *BufferRef;
typedef AtomicPointerType(Buffer) AtomicBufferRef;

static Buffer BufferPool[SB_CONFIG_SCRATCH_POOL_SIZE];
static AtomicBufferRef BufferStack = NULL;
static ThreadLocalStorage ScratchBuffer;

#define ALIGN_UP(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

static void InitializeBufferStack(void *info)
{
    SBUInteger index;

    for (index = 0; index < SB_CONFIG_SCRATCH_POOL_SIZE - 1; index++) {
        BufferPool[index].next = &BufferPool[index + 1];
        BufferPool[index].offset = 0;
    }

    BufferPool[SB_CONFIG_SCRATCH_POOL_SIZE - 1].next = NULL;
    BufferPool[SB_CONFIG_SCRATCH_POOL_SIZE - 1].offset = 0;
    AtomicPointerStore(&BufferStack, &BufferPool[0]);
}

static SBBoolean TryLazyInitializeBufferStack(void)
{
    static Once once = OnceMake();
    return OnceTryExecute(&once, InitializeBufferStack, NULL);
}

static BufferRef DetachBuffer(void)
{
    BufferRef buffer = NULL;

    if (TryLazyInitializeBufferStack()) {
        BufferRef expected = NULL;

        do {
            buffer = AtomicPointerLoad(&BufferStack);
            expected = buffer;

            if (!buffer) {
                break;
            }
        } while (!AtomicPointerCompareAndSet(&BufferStack, &expected, buffer->next));
    }

    return buffer;
}

static void RecycleBuffer(BufferRef buffer)
{
    BufferRef top;
    BufferRef expected;

    buffer->offset = 0;

    do {
        top = AtomicPointerLoad(&BufferStack);
        expected = top;
        buffer->next = top;
    } while (!AtomicPointerCompareAndSet(&BufferStack, &expected, buffer));
}

static void InitializeScratchBuffer(void *info)
{
    ThreadLocalStorageInitialize(ScratchBuffer);
}

static SBBoolean TryLazyInitializeScratchBuffer(void)
{
    static Once once = OnceMake();
    return OnceTryExecute(&once, InitializeScratchBuffer, NULL);
}

static void *NativeAllocateScratch(SBUInteger size, void *info)
{
    void *pointer = NULL;

    if (TryLazyInitializeScratchBuffer()) {
        BufferRef buffer = ThreadLocalStorageGet(ScratchBuffer);

        if (!buffer) {
            buffer = DetachBuffer();

            if (buffer) {
                ThreadLocalStorageSet(ScratchBuffer, buffer);
            }
        }

        if (buffer) {
            SBUInteger alignedSize = ALIGN_UP(size, sizeof(void *));
            SBUInteger limit = buffer->offset + alignedSize;

            if (limit <= SB_CONFIG_SCRATCH_BUFFER_SIZE) {
                pointer = buffer->data + buffer->offset;
                buffer->offset += alignedSize;
            }
        }
    }

    return pointer;
}

static void NativeResetScratch(void *info)
{
    if (TryLazyInitializeScratchBuffer()) {
        BufferRef buffer = ThreadLocalStorageGet(ScratchBuffer);

        if (buffer) {
            ThreadLocalStorageSet(ScratchBuffer, NULL);
            RecycleBuffer(buffer);
        }
    }
}

#else

#define NativeAllocateScratch   NULL
#define NativeResetScratch      NULL

#endif

static void *NativeAllocateBlock(SBUInteger size, void *info)
{
    return malloc(size);
}

static void *NativeReallocateBlock(void *pointer, SBUInteger size, void *info)
{
    return realloc(pointer, size);
}

static void NativeDeallocateBlock(void *pointer, void *info)
{
    free(pointer);
}

static SBAllocator NativeAllocator = SBAllocatorMake(
    NativeAllocateBlock, NativeReallocateBlock, NativeDeallocateBlock,
    NativeAllocateScratch, NativeResetScratch, NULL);

static void FinalizeAllocator(ObjectRef object)
{
    SBAllocatorRef allocator = object;

    if (allocator->_protocol.finalize) {
        allocator->_protocol.finalize(allocator->_info);
    }
}

static SBAllocatorRef SBAllocatorGetCurrent(void)
{
    SBAllocatorRef allocator = AtomicPointerLoad(&DefaultAllocator);

    if (!allocator) {
        allocator = &NativeAllocator;
    }

    return allocator;
}

SB_INTERNAL void *SBAllocatorAllocateBlock(SBAllocatorRef allocator, SBUInteger size)
{
    if (!allocator) {
        allocator = SBAllocatorGetCurrent();
    }

    return allocator->_protocol.allocateBlock(size, allocator->_info);
}

SB_INTERNAL void *SBAllocatorReallocateBlock(SBAllocatorRef allocator, void *pointer, SBUInteger newSize)
{
    if (!allocator) {
        allocator = SBAllocatorGetCurrent();
    }

    return allocator->_protocol.reallocateBlock(pointer, newSize, allocator->_info);
}

SB_INTERNAL void SBAllocatorDeallocateBlock(SBAllocatorRef allocator, void *pointer)
{
    if (!allocator) {
        allocator = SBAllocatorGetCurrent();
    }

    allocator->_protocol.deallocateBlock(pointer, allocator->_info);
}

SB_INTERNAL void *SBAllocatorAllocateScratch(SBAllocatorRef allocator, SBUInteger size)
{
    SBAllocatorAllocateScratchFunc func;
    void *pointer = NULL;

    if (!allocator) {
        allocator = SBAllocatorGetCurrent();
    }

    func = allocator->_protocol.allocateScratch;

    if (func) {
        pointer = func(size, allocator->_info);
    }

    return pointer;
}

SB_INTERNAL void SBAllocatorResetScratch(SBAllocatorRef allocator)
{
    SBAllocatorResetScratchFunc func;

    if (!allocator) {
        allocator = SBAllocatorGetCurrent();
    }

    func = allocator->_protocol.resetScratch;

    if (func) {
        func(allocator->_info);
    }
}

SBAllocatorRef SBAllocatorGetDefault(void)
{
    return AtomicPointerLoad(&DefaultAllocator);
}

void SBAllocatorSetDefault(SBAllocatorRef allocator)
{
    AtomicPointerStore(&DefaultAllocator, (SBAllocator *)allocator);
}

SBAllocatorRef SBAllocatorCreate(const SBAllocatorProtocol *protocol, void *info)
{
    const SBUInteger size = sizeof(SBAllocator);
    void *pointer = NULL;
    SBMutableAllocatorRef allocator = NULL;

    allocator = ObjectCreate(&size, 1, &pointer, FinalizeAllocator);

    if (allocator) {
        allocator->_info = info;
        allocator->_protocol = *protocol;
    }

    return allocator;
}

SBAllocatorRef SBAllocatorRetain(SBAllocatorRef allocator)
{
    return ObjectRetain((ObjectRef)allocator);
}

void SBAllocatorRelease(SBAllocatorRef allocator)
{
    ObjectRelease((ObjectRef)allocator);
}
