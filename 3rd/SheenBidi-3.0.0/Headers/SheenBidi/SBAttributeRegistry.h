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

#ifndef _SB_PUBLIC_ATTRIBUTE_REGISTRY_H
#define _SB_PUBLIC_ATTRIBUTE_REGISTRY_H

#include <SheenBidi/SBAttributeInfo.h>
#include <SheenBidi/SBBase.h>

#if SB_TEXT_API_SUPPORTED

SB_EXTERN_C_BEGIN

/**
 * Opaque reference to an attribute registry.
 */
typedef const struct _SBAttributeRegistry *SBAttributeRegistryRef;

/**
 * Creates an attribute registry from an array of attribute infos.
 * 
 * @param attributeInfos
 *      Pointer to an array of `SBAttributeInfo` entries. The array content is copied internally;
 *      the caller retains ownership.
 * @param count
 *      Number of entries in the `attributeInfos` array.
 * @param valueSize
 *      The size in bytes of the value field for each attribute.
 * @param valueCallbacks
 *      Optional pointer to an `SBAttributeValueCallbacks` structure defining custom value lifecycle
 *      and comparison behavior.
 * @return
 *      A reference to the attribute registry instance, or `NULL` on failure.
 */
SB_PUBLIC SBAttributeRegistryRef SBAttributeRegistryCreate(const SBAttributeInfo *attributeInfos,
    SBUInteger count, SBUInt8 valueSize, const SBAttributeValueCallbacks *valueCallbacks);

/**
 * Copies the info for a given attribute ID into the `attributeInfo` parameter.
 * 
 * @param registry
 *      The attribute registry object.
 * @param attributeID
 *      Attribute ID to query.
 * @param attributeInfo
 *      Output pointer to receive a copy of the attribute info; must not be `NULL`.
 * @return
 *      `SBTrue` if the attribute ID exists in the registry, `SBFalse` otherwise.
 */
SB_PUBLIC SBBoolean SBAttributeRegistryGetAttributeInfo(SBAttributeRegistryRef registry,
    SBAttributeID attributeID, SBAttributeInfo *attributeInfo);

/**
 * Looks up an attribute ID by name.
 * 
 * @param registry
 *      The attribute registry object.
 * @param name
 *      Null-terminated unique name of the attribute to find.
 * @return
 *      The attribute ID for `name`, or `SBAttributeIDNone` if not found.
 */
SB_PUBLIC SBAttributeID SBAttributeRegistryGetAttributeID(SBAttributeRegistryRef registry,
    const char *name);

/**
 * Increments the reference count of an attribute registry object.
 * 
 * @param registry
 *      The attribute registry object whose reference count will be incremented.
 * @return
 *      The same attribute registry object passed in as the parameter.
 */
SB_PUBLIC SBAttributeRegistryRef SBAttributeRegistryRetain(SBAttributeRegistryRef registry);

/**
 * Decrements the reference count of an attribute registry object. The object will be deallocated
 * when its reference count reaches zero.
 *
 * @param registry
 *      The attribute registry object whose reference count will be decremented.
 */
SB_PUBLIC void SBAttributeRegistryRelease(SBAttributeRegistryRef registry);

SB_EXTERN_C_END

#endif

#endif
