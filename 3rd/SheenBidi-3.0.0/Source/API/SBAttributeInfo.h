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

#ifndef _SB_INTERNAL_ATTRIBUTE_INFO_H
#define _SB_INTERNAL_ATTRIBUTE_INFO_H

#include <API/SBBase.h>

#if SB_TEXT_API_SUPPORTED

#include <string.h>
#include <SheenBidi/SBAttributeInfo.h>

#define SBAttributeIDMake(index, size)                  \
    ((((SBUInt32)((index) + 1)) << 8) | ((SBUInt32)(size) & 0xFF))

#define SBAttributeIDGetSize(id)                        \
    ((SBUInt8)((id) & 0xFF))

#define SBAttributeIDGetIndex(id)                       \
    ((SBUInt32)(((id) >> 8) - 1))

#define SBAttributeIDIsValid(id)                        \
    ((id) != SBAttributeIDNone)

#define SBAttributeItemGetValuePtr(item)                \
    ((void *)(&(item)->attributeID + 1))

#define SBAttributeItemSetValue(item, valuePtr)         \
    memcpy(SBAttributeItemGetValuePtr(item), valuePtr, SBAttributeIDGetSize((item)->attributeID))

#define SBAttributeItemSet(item, id, valuePtr)          \
do {                                                    \
    (item)->attributeID = (id);                         \
    SBAttributeItemSetValue(item, valuePtr);            \
} while (0)

#define SBAttributeItemIsEqualValue(id, first, second)  \
    (memcmp(first, second, SBAttributeIDGetSize(id)) == 0)

#endif

#endif
