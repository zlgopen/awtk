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

#ifndef _SB_PUBLIC_ALLOCATOR_H
#define _SB_PUBLIC_ALLOCATOR_H

#include <SheenBidi/SBBase.h>

SB_EXTERN_C_BEGIN

typedef const struct _SBAllocator *SBAllocatorRef;

/**
 * Function type for allocating a single memory block.
 *
 * @param size
 *      Size of the memory block to allocate, in bytes.
 * @param info
 *      User-defined context pointer provided during allocator creation.
 * @return
 *      Pointer to the allocated memory block, or `NULL` on failure.
 */
typedef void *(*SBAllocatorAllocateBlockFunc)(SBUInteger size, void *info);

/**
 * Function type for resizing a previously allocated memory block.
 *
 * @param pointer
 *      Pointer to the memory block to be resized.
 * @param newSize
 *      The new size in bytes for the memory block.
 * @param info
 *      User-defined context pointer provided during allocator creation.
 * @return
 *      Pointer to the resized memory block, or `NULL` if the operation failed.
 */
typedef void *(*SBAllocatorReallocateBlockFunc)(void *pointer, SBUInteger newSize, void *info);

/**
 * Function type for deallocating a previously allocated memory block.
 *
 * @param pointer
 *      Pointer to the memory block to deallocate.
 * @param info
 *      User-defined context pointer provided during allocator creation.
 */
typedef void (*SBAllocatorDeallocateBlockFunc)(void *pointer, void *info);

/**
 * Function type for allocating short-lived scratch memory.
 *
 * Scratch allocations are intended for temporary use within the scope of a public function call.
 * All memory returned by this function is automatically invalidated after a corresponding call to
 * `resetScratch`, which is typically called at the end of the same public function.
 *
 * The implementation is responsible for managing thread safety (e.g. using thread-local buffers,
 * locking, or fallback mechanisms).
 *
 * @param size
 *      The requested size of scratch memory, in bytes.
 * @param info
 *      User-defined context pointer provided during allocator creation.
 * @return
 *      Pointer to the allocated memory block, or `NULL` on failure.
 */
typedef void *(*SBAllocatorAllocateScratchFunc)(SBUInteger size, void *info);

/**
 * Function type for resetting the scratch memory space.
 *
 * This invalidates all previous scratch allocations for the current execution context (e.g.,
 * thread). It is typically called once per public function to reuse memory without freeing
 * individual chunks.
 *
 * @param info
 *      User-defined context pointer provided during allocator creation.
 */
typedef void (*SBAllocatorResetScratchFunc)(void *info);

/**
 * Function signature for finalizing an allocator object.
 * This is called once the reference count reaches zero.
 *
 * @param info
 *      User-defined context pointer provided during allocator creation.
 */
typedef void (*SBAllocatorFinalizeFunc)(void *info);

/**
 * Set of function pointers defining a memory allocator protocol.
 *
 * All functions are optional except `allocateBlock` and `deallocateBlock`. Optional functions will
 * fall back to standard mechanisms if not provided.
 */
typedef struct _SBAllocatorProtocol {
    /**
     * Allocates a memory block of the given size.
     */
    SBAllocatorAllocateBlockFunc allocateBlock;
    /**
     * Reallocates a previously allocated block to a new size.
     */
    SBAllocatorReallocateBlockFunc reallocateBlock;
    /**
     * Deallocates a memory block.
     */
    SBAllocatorDeallocateBlockFunc deallocateBlock;
    /**
     * Allocates short-lived scratch memory.
     * If `NULL`, the allocator will return `NULL` prompting the caller to use `allocateBlock`
     * instead.
     */
    SBAllocatorAllocateScratchFunc allocateScratch;
    /**
     * Resets the scratch memory space, invalidating previous allocations.
     * If `NULL`, the allocator will do nothing.
     */
    SBAllocatorResetScratchFunc resetScratch;
    /**
     * Optional function to finalize the allocator when its reference count drops to zero.
     */
    SBAllocatorFinalizeFunc finalize;
} SBAllocatorProtocol;

/**
 * Returns the global default allocator object.
 */
SB_PUBLIC SBAllocatorRef SBAllocatorGetDefault(void);

/**
 * Sets the global default allocator object.
 * 
 * @param allocator
 *      The allocator object to set as the default. If `NULL`, the default allocator will be reset
 *      to a standard implementation.
 */
SB_PUBLIC void SBAllocatorSetDefault(SBAllocatorRef allocator);

/**
 * Creates a custom allocator using the provided allocation protocol and optional context.
 *
 * @param protocol
 *      The set of function pointers defining custom allocation behavior.
 * @param info
 *      An optional pointer to context data passed to each allocator function.
 * @return
 *      A reference to the allocator instance, or `NULL` on failure.
 */
SB_PUBLIC SBAllocatorRef SBAllocatorCreate(const SBAllocatorProtocol *protocol, void *info);

/**
 * Increments the reference count of an allocator object.
 * 
 * @param allocator
 *      The allocator object whose reference count will be incremented.
 * @return
 *      The same allocator object passed in as the parameter.
 */
SB_PUBLIC SBAllocatorRef SBAllocatorRetain(SBAllocatorRef allocator);

/**
 * Decrements the reference count of an allocator object. The object will be deallocated when its
 * reference count reaches zero.
 * 
 * @param allocator
 *      The allocator object whose reference count will be decremented.
 */
SB_PUBLIC void SBAllocatorRelease(SBAllocatorRef allocator);

SB_EXTERN_C_END

#endif
