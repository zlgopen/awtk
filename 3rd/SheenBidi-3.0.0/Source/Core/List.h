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

#ifndef _SB_INTERNAL_LIST_H
#define _SB_INTERNAL_LIST_H

#include <API/SBAssert.h>
#include <API/SBBase.h>

typedef struct {
    SBUInt8 *data;
    SBUInteger count;
    SBUInteger capacity;
    SBUInteger itemSize;
} List, *ListRef;

#define LIST(type)          \
struct {                    \
    type *items;            \
    SBUInteger count;       \
    SBUInteger capacity;    \
    SBUInteger _itemSize;   \
}

typedef int (*SBComparison)(const void *item1, const void *item2);

#define SB_PRIVATE  SB_INTERNAL

SB_PRIVATE void InitializeList(ListRef list, SBUInteger itemSize);
SB_PRIVATE void FinalizeItemsBuffer(ListRef list);
SB_PRIVATE void ExtractItemsBuffer(ListRef list, void **outArray, SBUInteger *outCount);

SB_PRIVATE SBBoolean SetItemCapacity(ListRef list, SBUInteger capacity);
SB_PRIVATE SBBoolean ReserveItemRange(ListRef list, SBUInteger index, SBUInteger count);
SB_PRIVATE SBBoolean InsertItemAtIndex(ListRef list, SBUInteger index, const void *itemPtr);

SB_PRIVATE void RemoveItemRange(ListRef list, SBUInteger index, SBUInteger count);
SB_PRIVATE void RemoveAllItems(ListRef list);
SB_PRIVATE SBBoolean TrimExcessCapacity(ListRef list);

SB_PRIVATE SBUInteger SearchItemInRange(ListRef list, const void *itemPtr, SBUInteger index, SBUInteger count);
SB_PRIVATE void SortItemRange(ListRef list, SBUInteger index, SBUInteger count, SBComparison comparison);

#define CheckItemIndex(list_, index_)                   \
(                                                       \
    SBAssert(index_ < (list_)->count)                   \
)

#define GetItemPointer(list_, index_)                   \
(                                                       \
    CheckItemIndex(list_, index_),                      \
    (void *)((list_)->data                              \
                + ((index_) * (list_)->itemSize))       \
)

#define GetItemReference(list_, index_)                 \
(                                                       \
    CheckItemIndex(list_, index_),                      \
    &(list_)->items[index_]                             \
)

#define GetItemValue(list_, index_)                     \
(                                                       \
    CheckItemIndex(list_, index_),                      \
    (list_)->items[index_]                              \
)

#define SetItemValue(list_, index_, value_)             \
do {                                                    \
    CheckItemIndex(list_, index_),                      \
    (list_)->items[index_] = value_;                    \
} while (0)

#define InsertItemAtEnd(list_, item_)                   \
    InsertItemAtIndex(list_, (list_)->count, item_)


#define ListInitialize(list, itemSize)              InitializeList((ListRef)(list), itemSize)
#define ListFinalize(list)                          FinalizeItemsBuffer((ListRef)(list))
#define ListFinalizeKeepingArray(list, outArray, outCount) \
    ExtractItemsBuffer((ListRef)(list), (void **)outArray, outCount)

#define ListSetCapacity(list, capacity)             SetItemCapacity((ListRef)(list), capacity)
#define ListReserveRange(list, index, count)        ReserveItemRange((ListRef)(list), index, count)
#define ListRemoveRange(list, index, count)         RemoveItemRange((ListRef)(list), index, count)

#define ListClear(list)                             RemoveAllItems((ListRef)(list))
#define ListTrimExcess(list)                        TrimExcessCapacity((ListRef)(list))

#define ListGetPtr(list, index)                     GetItemPointer((ListRef)(list), index)
#define ListGetRef(list, index)                     GetItemReference(list, index)
#define ListGetVal(list, index)                     GetItemValue(list, index)
#define ListSetVal(list, index, item)               SetItemValue(list, index, item)

#define ListAdd(list, item)                         InsertItemAtEnd((ListRef)(list), item)
#define ListInsert(list, index, item)               InsertItemAtIndex((ListRef)(list), index, item)
#define ListRemoveAt(list, index)                   RemoveItemRange((ListRef)(list), index, 1)
#define ListRemoveAll(list)                         RemoveAllItems((ListRef)(list))

#define ListIndexOfItem(list, item, index, count)   SearchItemInRange((ListRef)(list), item, index, count)
#define ListContainsItem(list, item) \
    (SearchItemInRange((ListRef)(list), item, 0, (list)->count) != SBInvalidIndex)

#define ListSort(list, index, count, comparison)    SortItemRange((ListRef)(list), index, count, comparison);

#endif
