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

#ifndef _SB_INTERNAL_ATTRIBUTE_DICTIONARY_H
#define _SB_INTERNAL_ATTRIBUTE_DICTIONARY_H

#include <API/SBBase.h>

#if SB_TEXT_API_SUPPORTED

#include <SheenBidi/SBAttributeRegistry.h>
#include <API/SBAttributeList.h>

typedef struct _AttributeDictionary {
    SBAttributeList _list;
} AttributeDictionary, *AttributeDictionaryRef;

/**
 * Initializes an attribute dictionary.
 *
 * @param dictionary
 *      The attribute dictionary to initialize.
 * @param valueSize
 *      The size in bytes of attribute values.
 */
SB_INTERNAL void AttributeDictionaryInitialize(AttributeDictionaryRef dictionary, SBUInt8 valueSize);

/**
 * Finalizes an attribute dictionary and releases all resources.
 *
 * @param dictionary
 *      The attribute dictionary to finalize.
 * @param registry
 *      The attribute registry used to release attribute values.
 */
SB_INTERNAL void AttributeDictionaryFinalize(AttributeDictionaryRef dictionary,
    SBAttributeRegistryRef registry);

/**
 * Creates a new attribute dictionary.
 *
 * @param valueSize
 *      The size in bytes of attribute values.
 * @return
 *      A new attribute dictionary reference, or NULL if allocation fails.
 */
SB_INTERNAL AttributeDictionaryRef AttributeDictionaryCreate(SBUInt8 valueSize);

/**
 * Deallocates an attribute dictionary.
 *
 * @param dictionary
 *      The attribute dictionary to deallocate.
 * @param registry
 *      The attribute registry used to release attribute values.
 */
SB_INTERNAL void AttributeDictionaryDestroy(AttributeDictionaryRef dictionary,
    SBAttributeRegistryRef registry);

#define AttributeDictionaryGetCount(dictionary)     \
    SBAttributeListSize(&(dictionary)->_list)

/**
 * Checks whether an attribute dictionary is empty.
 *
 * @param dictionary
 *      The attribute dictionary to check.
 * @return
 *      `SBTrue` if empty, `SBFalse` otherwise.
 */
SB_INTERNAL SBBoolean AttributeDictionaryIsEmpty(AttributeDictionaryRef dictionary);

/**
 * Clears this dictionary and copies all attributes from the source, retaining their values.
 *
 * @param dictionary
 *      The attribute dictionary to modify.
 * @param other
 *      The source dictionary to copy from.
 * @param registry
 *      The attribute registry for value lifecycle management.
 */
SB_INTERNAL void AttributeDictionarySet(AttributeDictionaryRef dictionary,
    AttributeDictionaryRef other, SBAttributeRegistryRef registry);

/**
 * Adds or updates an attribute item in the dictionary.
 *
 * If the attribute exists, updates its value; otherwise inserts it in sorted order.
 *
 * @param dictionary
 *      The destination dictionary.
 * @param attributeID
 *      The attribute ID to add or update.
 * @param attributeValue
 *      The attribute value.
 * @param registry
 *      The attribute registry for value lifecycle management.
 * @param unchanged
 *      Optional output parameter. Set to SBTrue if the item already existed with an equal value,
 *      SBFalse if item was inserted or value was changed.
 */
SB_INTERNAL void AttributeDictionaryPut(AttributeDictionaryRef dictionary,
    SBAttributeID attributeID, const void *attributeValue, SBAttributeRegistryRef registry,
    SBBoolean *unchanged);

/**
 * Merges all attributes from another dictionary into this dictionary.
 *
 * @param dictionary
 *      The destination dictionary.
 * @param other
 *      The source dictionary to merge from.
 * @param registry
 *      The attribute registry for value lifecycle management.
 * @param unchanged
 *      Optional output parameter. Set to SBTrue if all items already existed with equal values,
 *      SBFalse if any item was inserted or modified.
 */
SB_INTERNAL void AttributeDictionaryMerge(AttributeDictionaryRef dictionary,
    AttributeDictionaryRef other, SBAttributeRegistryRef registry, SBBoolean *unchanged);

/**
 * Retrieves filtered attributes matching the specified scope and group.
 *
 * @param dictionary
 *      The attribute dictionary to query.
 * @param targetScope
 *      The attribute scope to filter by (e.g., character or paragraph).
 * @param targetGroup
 *      The attribute group to filter by. If SBAttributeGroupNone is specified, all groups within
 *      the scope are included.
 * @param registry
 *      The attribute registry used to retrieve attribute metadata for filtering.
 * @param result
 *      The dictionary where matching items will be added. Will be empty if no attributes match.
 *      Always cleared at the start.
 */
SB_INTERNAL void AttributeDictionaryFilter(AttributeDictionaryRef dictionary,
    SBAttributeScope targetScope, SBAttributeGroup targetGroup,
    SBAttributeRegistryRef registry, AttributeDictionaryRef result);

/**
 * Searches for an attribute value by ID.
 *
 * @param dictionary
 *      The attribute dictionary to search.
 * @param attributeID
 *      The attribute ID to search for.
 * @return
 *      A pointer to the matching attribute value, or NULL if not found.
 */
SB_INTERNAL const void *AttributeDictionaryFindValue(AttributeDictionaryRef dictionary,
    SBAttributeID attributeID);

/**
 * Checks if any attribute matching the specified scope and group exists in the dictionary.
 *
 * @param dictionary
 *      The attribute dictionary to check.
 * @param targetScope
 *      The attribute scope to filter by (e.g., character or paragraph).
 * @param targetGroup
 *      The attribute group to filter by (or SBAttributeGroupNone for no group filtering).
 * @param registry
 *      The attribute registry used to retrieve attribute metadata for filtering.
 * @return
 *      SBTrue if at least one matching attribute exists, SBFalse otherwise.
 */
SB_INTERNAL SBBoolean AttributeDictionaryMatchAny(AttributeDictionaryRef dictionary,
    SBAttributeScope targetScope, SBAttributeGroup targetGroup, SBAttributeRegistryRef registry);

/**
 * Checks if all filtered attributes have equal values between two dictionaries.
 *
 * Compares filtered attributes (by scope and group) for equality and verifies both dictionaries
 * have the same filtered content.
 *
 * @param dictionary
 *      The attribute dictionary to check.
 * @param targetScope
 *      The attribute scope to filter by (e.g., character or paragraph).
 * @param targetGroup
 *      The attribute group to filter by, or SBAttributeGroupNone to include all groups.
 * @param registry
 *      The attribute registry used to retrieve attribute metadata and compare equality.
 * @param other
 *      The other attribute dictionary to compare against.
 * @return
 *      SBTrue if all filtered attributes exist in both dictionaries with equal values and neither
 *      dictionary has extra filtered attributes, SBFalse otherwise.
 */
SB_INTERNAL SBBoolean AttributeDictionaryMatchAll(AttributeDictionaryRef dictionary,
    SBAttributeScope targetScope, SBAttributeGroup targetGroup,
    SBAttributeRegistryRef registry, AttributeDictionaryRef other);

/**
 * Removes an attribute with the specified ID.
 *
 * @param dictionary
 *      The attribute dictionary from which to remove the attribute.
 * @param attributeID
 *      The ID of the attribute to remove.
 * @param registry
 *      The attribute registry used to release the attribute value.
 * @param unchanged
 *      Optional output parameter. Set to SBTrue if the attribute was not found, SBFalse if it was
 *      removed.
 */
SB_INTERNAL void AttributeDictionaryRemove(AttributeDictionaryRef dictionary,
    SBAttributeID attributeID, SBAttributeRegistryRef registry, SBBoolean *unchanged);

/**
 * Removes all attributes from the dictionary.
 *
 * @param dictionary
 *      The attribute dictionary to clear.
 * @param registry
 *      The attribute registry used to release attribute values.
 */
SB_INTERNAL void AttributeDictionaryClear(AttributeDictionaryRef dictionary,
    SBAttributeRegistryRef registry);

#endif

#endif
