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

#ifndef _SB_PUBLIC_ATTRIBUTE_LIST_H
#define _SB_PUBLIC_ATTRIBUTE_LIST_H

#include <SheenBidi/SBAttributeInfo.h>
#include <SheenBidi/SBBase.h>

#if SB_TEXT_API_SUPPORTED

SB_EXTERN_C_BEGIN

/**
 * Opaque reference to an attribute list.
 */
typedef const struct _SBAttributeList *SBAttributeListRef;

/**
 * Returns a pointer to the attribute item located at the given index in the list.
 *
 * @param list
 *      The attribute list to retrieve from.
 * @param index
 *      The zero-based index of the item to retrieve. Must be less than the count returned by
 *      SBAttributeListGetCount().
 * @return
 *      A pointer to the attribute item at the specified index.
 */
SB_PUBLIC const SBAttributeItem *SBAttributeListGetItem(SBAttributeListRef list, SBUInteger index);

/**
 * Returns the number of attributes in the list.
 *
 * @param list
 *      The attribute list to query.
 * @return
 *      The total count of attributes currently stored in the list.
 */
SB_PUBLIC SBUInteger SBAttributeListGetCount(SBAttributeListRef list);

SB_EXTERN_C_END

#endif

#endif
