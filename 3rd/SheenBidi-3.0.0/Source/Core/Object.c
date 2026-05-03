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
#include <API/SBAssert.h>
#include <Core/AtomicUInt.h>
#include <Core/Memory.h>

#include "Object.h"

SB_INTERNAL void ObjectBaseInitialize(ObjectBaseRef objectBase)
{
    MemoryInitialize(&objectBase->memory);
    objectBase->finalize = NULL;
    objectBase->retainCount = 0;
}

SB_INTERNAL ObjectRef ObjectCreate(const SBUInteger *chunkSizes, SBUInteger chunkCount,
    void **outPointers, FinalizeFunc finalizer)
{
    ObjectBaseRef base = NULL;
    Memory memory;

    /* Number of chunks MUST be greater than or equal to one. */
    SBAssert(chunkCount >= 1);
    /* Size of first chunk MUST be greater than the size of ObjectBase structure. */
    SBAssert(chunkSizes[0] > sizeof(ObjectBase));

    MemoryInitialize(&memory);

    if (MemoryAllocateChunks(&memory, MemoryTypePermanent, chunkSizes, chunkCount, outPointers)) {
        base = outPointers[0];
        base->memory = memory;
        base->finalize = finalizer;

        AtomicUIntInitialize(&base->retainCount, 1);
    }

    return base;
}

SB_INTERNAL SBUInteger ObjectGetRetainCount(ObjectRef object)
{
    ObjectBaseRef base = (ObjectBaseRef)object;

    return AtomicUIntLoad(&base->retainCount);
}

SB_INTERNAL ObjectRef ObjectRetain(ObjectRef object)
{
    ObjectBaseRef base = (ObjectBaseRef)object;

    AtomicUIntIncrement(&base->retainCount);

    return object;
}

SB_INTERNAL void ObjectRelease(ObjectRef object)
{
    ObjectBaseRef base = (ObjectBaseRef)object;

    if (AtomicUIntDecrement(&base->retainCount) == 0) {
        if (base->finalize) {
            base->finalize(object);
        }

        MemoryFinalize(&base->memory);
    }
}
