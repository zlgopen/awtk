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

#ifndef _SB_INTERNAL_ATTRIBUTE_REGISTRY_H
#define _SB_INTERNAL_ATTRIBUTE_REGISTRY_H

#include <API/SBBase.h>

#if SB_TEXT_API_SUPPORTED

#include <SheenBidi/SBAttributeInfo.h>
#include <SheenBidi/SBAttributeRegistry.h>

#include <Core/Object.h>

typedef struct _SBAttributeRegistry {
    ObjectBase _base;
    SBAttributeInfo *attributeInfos;
    SBUInteger count;
    SBUInt8 valueSize;
    SBAttributeValueCallbacks _valueCallbacks;
} SBAttributeRegistry;

SB_INTERNAL const void *SBAttributeRegistryRetainAttribute(SBAttributeRegistryRef registry,
    const void *attributeValue);

SB_INTERNAL void SBAttributeRegistryReleaseAttribute(SBAttributeRegistryRef registry,
    const void *attributeValue);

SB_INTERNAL SBBoolean SBAttributeRegistryIsEqualAttribute(SBAttributeRegistryRef registry,
    SBAttributeID attributeID, const void *first, const void *second);

SB_INTERNAL const SBAttributeInfo *SBAttributeRegistryGetInfoReference(
    SBAttributeRegistryRef registry, SBAttributeID attributeID);

#endif

#endif
