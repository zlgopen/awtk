/*
 * Copyright (C) 2025-2026 Muhammad Tayyab Akram
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
#include <string.h>

#include <API/SBAllocator.h>
#include <API/SBAssert.h>
#include <API/SBBase.h>

#include "List.h"

#define DEFAULT_LIST_CAPACITY 4

SB_PRIVATE void InitializeList(ListRef list, SBUInteger itemSize)
{
    /* Item size MUST be greater than 0. */
    SBAssert(itemSize > 0);

    list->data = NULL;
    list->count = 0;
    list->capacity = 0;
    list->itemSize = itemSize;
}

SB_PRIVATE void FinalizeItemsBuffer(ListRef list)
{
    if (list->data) {
        SBAllocatorDeallocateBlock(NULL, list->data);
    }
}

SB_PRIVATE void ExtractItemsBuffer(ListRef list, void **outArray, SBUInteger *outCount)
{
    if (list->count > 0) {
        SetItemCapacity(list, list->count);

        *outArray = list->data;
        *outCount = list->count;
    } else {
        FinalizeItemsBuffer(list);

        *outArray = NULL;
        *outCount = 0;
    }
}

SB_PRIVATE SBBoolean SetItemCapacity(ListRef list, SBUInteger capacity)
{
    /* The new capacity must be larger than total number of elements in the list. */
    SBAssert(capacity >= list->count);

    if (capacity != list->capacity) {
        SBBoolean isAllocated = SBFalse;
        void *block;

        if (list->data) {
            block = SBAllocatorReallocateBlock(NULL, list->data, list->itemSize * capacity);
        } else {
            block = SBAllocatorAllocateBlock(NULL, list->itemSize * capacity);
        }

        if (block) {
            list->data = block;
            list->capacity = capacity;

            isAllocated = SBTrue;
        }

        return isAllocated;
    }

    return SBTrue;
}

static SBBoolean EnsureItemCapacity(ListRef list, SBUInteger capacity)
{
    SBBoolean isEnsured = SBTrue;

    if (list->capacity < capacity) {
        SBUInteger newCapacity = (list->capacity ? list->count * 2 : DEFAULT_LIST_CAPACITY);
        if (newCapacity < capacity) {
            newCapacity = capacity;
        }

        isEnsured = SetItemCapacity(list, newCapacity);
    }

    return isEnsured;
}

static void MoveItemRange(ListRef list, SBUInteger srcIndex,
    SBUInteger dstIndex, SBUInteger itemCount)
{
    /* The capacity must be available to move the block. */
    SBAssert((srcIndex + itemCount) <= list->capacity && (dstIndex + itemCount) <= list->capacity);

    if (itemCount) {
        memmove(list->data + (dstIndex * list->itemSize),
            list->data + (srcIndex * list->itemSize), list->itemSize * itemCount);
    }
}

SB_PRIVATE SBBoolean ReserveItemRange(ListRef list, SBUInteger index, SBUInteger count)
{
    SBBoolean isReserved = SBFalse;

    /* The index must be valid and there should be no integer overflow. */
    SBAssert(index <= list->count && index <= (index + count));

    if (EnsureItemCapacity(list, list->count + count)) {
        MoveItemRange(list, index, index + count, list->count - index);
        list->count += count;

        isReserved = SBTrue;
    }

    return isReserved;
}

SB_PRIVATE SBBoolean InsertItemAtIndex(ListRef list, SBUInteger index, const void *itemPtr)
{
    SBBoolean isInserted = SBFalse;

    /* Index MUST be valid. */
    SBAssert(index <= list->count);

    if (ReserveItemRange(list, index, 1)) {
        void *destination = list->data + (index * list->itemSize);
        memcpy(destination, itemPtr, list->itemSize);

        isInserted = SBTrue;
    }

    return isInserted;
}

SB_PRIVATE void RemoveItemRange(ListRef list, SBUInteger index, SBUInteger count)
{
    SBUInteger nextIndex = index + count;

    /* The specified item indexes must be valid and there should be no integer overflow. */
    SBAssert(nextIndex <= list->count && index <= nextIndex);

    MoveItemRange(list, nextIndex, index, list->count - nextIndex);
    list->count -= count;
}

SB_PRIVATE void RemoveAllItems(ListRef list)
{
    list->count = 0;
}

SB_PRIVATE SBBoolean TrimExcessCapacity(ListRef list)
{
    return SetItemCapacity(list, list->count);
}

SB_PRIVATE SBUInteger SearchItemInRange(ListRef list, const void *itemPtr,
    SBUInteger index, SBUInteger count)
{
    SBUInteger max = index + count;

    /* The range must be valid and there should be no integer overflow. */
    SBAssert((list->count > 0 ? max <= list->count : max == 0) && index <= max);

    for (; index < max; index++) {
        void *currentPtr = GetItemPointer(list, index);
        if (memcmp(currentPtr, itemPtr, list->itemSize) == 0) {
            return index;
        }
    }

    return SBInvalidIndex;
}

SB_PRIVATE void SortItemRange(ListRef list, SBUInteger index, SBUInteger count,
    SBComparison comparison)
{
    /* The range must be valid and there should be no integer overflow. */
    SBAssert(SBUIntegerVerifyRange(list->count, index, count));

    qsort(list->data, list->count, list->itemSize, comparison);
}
