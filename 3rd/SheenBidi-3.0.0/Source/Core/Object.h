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

#ifndef _SB_INTERNAL_OBJECT_H
#define _SB_INTERNAL_OBJECT_H

#include <stddef.h>

#include <API/SBBase.h>
#include <Core/AtomicUInt.h>
#include <Core/Memory.h>

/**
 * Represents a generic reference-counted object backed by a memory arena.
 */
typedef void *ObjectRef;

/**
 * Function signature for finalizing a reference-counted object.
 * This is called once the reference count reaches zero.
 *
 * @param object
 *      The object being finalized.
 */
typedef void (*FinalizeFunc)(ObjectRef object);

/**
 * Internal structure representing the base of an object.
 */
typedef struct ObjectBase {
    Memory memory;
    FinalizeFunc finalize;
    AtomicUInt retainCount;
} ObjectBase, *ObjectBaseRef;

#ifndef SB_CONFIG_ALLOW_NON_ATOMIC_FALLBACK

#if !defined(HAS_ATOMIC_UINT_SUPPORT)
#error "No atomic operations available. For thread-unsafe reference counting, manually define \
`SB_CONFIG_ALLOW_NON_ATOMIC_FALLBACK`."
#endif

#endif

#define ObjectBaseMake() { MemoryMake(), NULL, 0 }

SB_INTERNAL void ObjectBaseInitialize(ObjectBaseRef objectBase);

/**
 * Creates a reference-counted object and allocates memory chunks for it.
 *
 * This function allocates a single contiguous memory block and divides it into chunks according to
 * the provided `chunkSizes`. The addresses of these chunks are returned in the `outPointers` array.
 *
 * The first chunk (i.e., `outPointers[0]`) MUST correspond to a user-defined struct that has
 * `ObjectBase` as its **first field**. This is critical because all Object-related functionalities
 * such as reference counting, memory management, and finalization rely on being able to treat the
 * object as an `ObjectBaseRef`.
 *
 * Example:
 * ```
 * typedef struct MyObject {
 *     ObjectBase base; // MUST be first
 *     int field1;
 *     float field2;
 * } MyObject;
 *
 * SBUInteger sizes[] = { sizeof(MyObject), otherSize1, otherSize2 };
 * void *pointers[3];
 * MyObject *obj = ObjectCreate(sizes, 3, pointers, FinalizeMyObject);
 * ```
 *
 * @param chunkSizes
 *      An array of sizes (in bytes) for each memory chunk.
 * @param chunkCount
 *      Number of chunks (must be >= 1).
 * @param outPointers
 *      Output array to receive the addresses of the allocated chunks.
 * @param finalizer
 *      Optional function to finalize the object when its reference count drops to zero.
 * @return
 *      A reference to the created object (same as `outPointers[0]`), or NULL if allocation failed.
 */
SB_INTERNAL ObjectRef ObjectCreate(const SBUInteger *chunkSizes, SBUInteger chunkCount,
    void **outPointers, FinalizeFunc finalizer);

/**
 * Retrieves the current reference count of an object.
 * 
 * @param object
 *      The object whose reference count is queried.
 * @return
 *      The current reference count.
 */
SB_INTERNAL SBUInteger ObjectGetRetainCount(ObjectRef object);

/**
 * Retains the object by incrementing its reference count.
 *
 * @param object
 *      The object to retain.
 * @return
 *      The same object.
 */
SB_INTERNAL ObjectRef ObjectRetain(ObjectRef object);

/**
 * Releases the object by decrementing its reference count. When the count reaches zero, the
 * finalizer (if any) is called, and all associated memory is freed.
 *
 * @param object
 *      The object to release.
 */
SB_INTERNAL void ObjectRelease(ObjectRef object);

#endif
