/*
 * Copyright (C) 2026 Muhammad Tayyab Akram
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

#include <API/SBBase.h>

#if SB_TEXT_API_SUPPORTED

#include <API/SBAttributeInfo.h>
#include <Core/List.h>

#include "SBAttributeList.h"

#define ALIGN_VALUE_SIZE(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

SB_INTERNAL void SBAttributeListInitialize(SBAttributeListRef list, SBUInteger valueSize)
{
    const SBUInteger idSize = sizeof(SBAttributeID);
    const SBUInteger itemSize = idSize + ALIGN_VALUE_SIZE(valueSize, idSize);

    ListInitialize(&list->_list, itemSize);
}

SB_INTERNAL SBUInteger SBAttributeListBinarySearchIndex(SBAttributeListRef list,
    SBAttributeID attributeID, SBBoolean *itemFound)
{
    const List *innerList = &list->_list;

    *itemFound = SBFalse;

    if (innerList->data) {
        SBUInteger low = 0;
        SBUInteger high = innerList->count;

        while (low < high) {
            SBUInteger mid = low + (high - low) / 2;
            SBAttributeItem *item = ListGetPtr(innerList, mid);
            SBAttributeID id = item->attributeID;

            if (id == attributeID) {
                *itemFound = SBTrue;
                return mid;
            }

            if (id < attributeID) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }

        return low;
    }

    return 0;
}

const SBAttributeItem *SBAttributeListGetItem(SBAttributeListRef list, SBUInteger index)
{
    return ListGetPtr(&list->_list, index);
}

SBUInteger SBAttributeListGetCount(SBAttributeListRef list)
{
    return list->_list.count;
}

#endif
