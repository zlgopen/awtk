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

#include <API/SBBase.h>

#if SB_TEXT_API_SUPPORTED

#include <stddef.h>

#include <API/SBAllocator.h>
#include <API/SBAttributeInfo.h>
#include <API/SBAttributeList.h>
#include <API/SBAttributeRegistry.h>

#include "AttributeDictionary.h"

/**
 * Determines whether an attribute item matches the specified filter criteria.
 *
 * Checks if the attribute's scope matches the target scope. If a specific group filter is
 * provided (not SBAttributeGroupNone), also verifies that the attribute's group matches.
 * Both conditions must be satisfied for the item to be considered a match.
 *
 * @param item
 *      The attribute item to check.
 * @param registry
 *      The attribute registry used to retrieve attribute metadata.
 * @param filterScope
 *      The scope to match against (character or paragraph).
 * @param filterGroup
 *      The group to match against, or SBAttributeGroupNone to skip group filtering.
 * @return
 *      SBTrue if the item matches all filter criteria, SBFalse otherwise.
 */
static SBBoolean CheckAttributeMatchesFilter(SBAttributeItem *item,
    SBAttributeRegistryRef registry, SBAttributeScope filterScope, SBAttributeGroup filterGroup)
{
    const SBAttributeInfo *attributeInfo;
    SBBoolean matchesFilter;

    attributeInfo = SBAttributeRegistryGetInfoReference(registry, item->attributeID);

    /* Match the scope first */
    matchesFilter = (attributeInfo->scope == filterScope);

    /* Match the group if a specific group filter is provided */
    if (matchesFilter && filterGroup != SBAttributeGroupNone) {
        matchesFilter = (attributeInfo->group == filterGroup);
    }

    return matchesFilter;
}

static void ReleaseAllAttributeItems(AttributeDictionaryRef dictionary,
    SBAttributeRegistryRef registry)
{
    SBUInteger itemIndex;

    if (registry) {
        SBUInteger itemCount = SBAttributeListSize(&dictionary->_list);

        /* Release attribute values through the registry */
        for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
            SBAttributeItem *item = SBAttributeListGetAt(&dictionary->_list, itemIndex);
            const void *valuePtr = SBAttributeItemGetValuePtr(item);

            SBAttributeRegistryReleaseAttribute(registry, valuePtr);
        }
    }
}

SB_INTERNAL void AttributeDictionaryInitialize(AttributeDictionaryRef dictionary, SBUInt8 valueSize)
{
    SBUInteger itemSize = sizeof(SBAttributeItem) + valueSize;

    SBAttributeListInitialize(&dictionary->_list, itemSize);
}

SB_INTERNAL void AttributeDictionaryFinalize(AttributeDictionaryRef dictionary,
    SBAttributeRegistryRef registry)
{
    ReleaseAllAttributeItems(dictionary, registry);
    SBAttributeListFinalize(&dictionary->_list);
}

SB_INTERNAL AttributeDictionaryRef AttributeDictionaryCreate(SBUInt8 valueSize)
{
    AttributeDictionaryRef dictionary;

    dictionary = SBAllocatorAllocateBlock(NULL, sizeof(AttributeDictionary));

    if (dictionary) {
        AttributeDictionaryInitialize(dictionary, valueSize);
    }

    return dictionary;
}

SB_INTERNAL void AttributeDictionaryDestroy(AttributeDictionaryRef dictionary,
    SBAttributeRegistryRef registry)
{
    AttributeDictionaryFinalize(dictionary, registry);
    SBAllocatorDeallocateBlock(NULL, dictionary);
}

SB_INTERNAL SBBoolean AttributeDictionaryIsEmpty(AttributeDictionaryRef dictionary)
{
    return (SBAttributeListSize(&dictionary->_list) == 0);
}

SB_INTERNAL void AttributeDictionarySet(AttributeDictionaryRef dictionary,
    AttributeDictionaryRef other, SBAttributeRegistryRef registry)
{
    SBUInteger itemCount = SBAttributeListSize(&other->_list);
    SBUInteger itemIndex;

    AttributeDictionaryClear(dictionary, registry);
    SBAttributeListReserveRange(&dictionary->_list, 0, itemCount);

    /* Copy each attribute item, retaining values through the registry */
    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        const SBAttributeItem *source = SBAttributeListGetAt(&other->_list, itemIndex);
        SBAttributeItem *destination = SBAttributeListGetAt(&dictionary->_list, itemIndex);
        const void *oldValue;
        const void *newValue;

        oldValue = SBAttributeItemGetValuePtr(source);
        newValue = SBAttributeRegistryRetainAttribute(registry, oldValue);

        SBAttributeItemSet(destination, source->attributeID, newValue);
    }
}

SB_INTERNAL void AttributeDictionaryPut(AttributeDictionaryRef dictionary,
    SBAttributeID attributeID, const void *attributeValue, SBAttributeRegistryRef registry,
    SBBoolean *unchanged)
{
    SBBoolean itemFound;
    SBUInteger itemIndex;
    const void *newValue;

    /* Find the index where the item exists or should be inserted */
    itemIndex = SBAttributeListBinarySearchIndex(&dictionary->_list, attributeID, &itemFound);

    if (registry) {
        /* Retain the new attribute value upfront */
        newValue = SBAttributeRegistryRetainAttribute(registry, attributeValue);
    } else {
        newValue = attributeValue;
    }

    if (itemFound) {
        SBAttributeItem *item = SBAttributeListGetAt(&dictionary->_list, itemIndex);
        const void *previousValue = SBAttributeItemGetValuePtr(item);

        if (registry) {
            if (unchanged) {
                *unchanged = SBAttributeRegistryIsEqualAttribute(registry, attributeID, previousValue, newValue);
            }

            /* Release the old attribute value being replaced */
            SBAttributeRegistryReleaseAttribute(registry, previousValue);
        } else {
            if (unchanged) {
                *unchanged = SBAttributeItemIsEqualValue(item->attributeID, previousValue, newValue);
            }
        }

        /* Update with the new value */
        SBAttributeItemSetValue(item, newValue);
    } else {
        SBAttributeItem *newItem;

        /* Insert the new item at the correct position to maintain sorted order */
        SBAttributeListReserveRange(&dictionary->_list, itemIndex, 1);

        newItem = SBAttributeListGetAt(&dictionary->_list, itemIndex);
        SBAttributeItemSet(newItem, attributeID, newValue);

        if (unchanged) {
            *unchanged = SBFalse;
        }
    }
}

SB_INTERNAL void AttributeDictionaryMerge(AttributeDictionaryRef dictionary,
    AttributeDictionaryRef other, SBAttributeRegistryRef registry, SBBoolean *unchanged)
{
    SBUInteger itemCount = SBAttributeListSize(&other->_list);
    SBUInteger itemIndex;
    SBBoolean remainedUnchanged;
    SBBoolean *noImpact;

    if (unchanged) {
        *unchanged = SBTrue;
        noImpact = &remainedUnchanged;
    } else {
        noImpact = NULL;
    }

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        SBAttributeItem *currentItem = SBAttributeListGetAt(&other->_list, itemIndex);

        AttributeDictionaryPut(dictionary, currentItem->attributeID,
            SBAttributeItemGetValuePtr(currentItem), registry, noImpact);

        if (noImpact && !remainedUnchanged) {
            *unchanged = SBFalse;
        }
    }
}

SB_INTERNAL void AttributeDictionaryFilter(AttributeDictionaryRef dictionary,
    SBAttributeScope targetScope, SBAttributeGroup targetGroup,
    SBAttributeRegistryRef registry, AttributeDictionaryRef result)
{
    SBUInteger itemCount = SBAttributeListSize(&dictionary->_list);
    SBUInteger itemIndex;

    /* Clear the result dictionary before populating it */
    AttributeDictionaryClear(result, NULL);

    /* Iterate through all items in the dictionary */
    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        SBAttributeItem *currentItem = SBAttributeListGetAt(&dictionary->_list, itemIndex);
        SBBoolean matched;

        /* Check if the item matches the filter criteria */
        matched = CheckAttributeMatchesFilter(currentItem, registry, targetScope, targetGroup);

        if (matched) {
            const void *valuePtr = SBAttributeItemGetValuePtr(currentItem);
            SBAttributeItem *newItem;

            /* Add matching item to the result dictionary */
            SBAttributeListReserveEnd(&result->_list, 1);
            newItem = SBAttributeListGetLast(&result->_list);

            SBAttributeItemSet(newItem, currentItem->attributeID, valuePtr);
        }
    }
}

SB_INTERNAL const void *AttributeDictionaryFindValue(
    AttributeDictionaryRef dictionary, SBAttributeID attributeID)
{
    const void *value = NULL;
    SBUInteger itemIndex;
    SBBoolean itemFound;

    itemIndex = SBAttributeListBinarySearchIndex(&dictionary->_list, attributeID, &itemFound);

    if (itemFound) {
        SBAttributeItem *item = SBAttributeListGetAt(&dictionary->_list, itemIndex);
        value = SBAttributeItemGetValuePtr(item);
    }

    return value;
}

SB_INTERNAL SBBoolean AttributeDictionaryMatchAny(AttributeDictionaryRef dictionary,
    SBAttributeScope targetScope, SBAttributeGroup targetGroup, SBAttributeRegistryRef registry)
{
    SBUInteger itemCount = SBAttributeListSize(&dictionary->_list);
    SBUInteger itemIndex;

    /* Iterate through all items */
    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        SBAttributeItem *currentItem = SBAttributeListGetAt(&dictionary->_list, itemIndex);

        /* Return true if there is a match */
        if (CheckAttributeMatchesFilter(currentItem, registry, targetScope, targetGroup)) {
            return SBTrue;
        }
    }

    return SBFalse;
}

SB_INTERNAL SBBoolean AttributeDictionaryMatchAll(AttributeDictionaryRef dictionary,
    SBAttributeScope targetScope, SBAttributeGroup targetGroup,
    SBAttributeRegistryRef registry, AttributeDictionaryRef other)
{
    SBUInteger dictCount = SBAttributeListSize(&dictionary->_list);
    SBUInteger otherCount = SBAttributeListSize(&other->_list);
    SBUInteger dictIndex;
    SBUInteger otherIndex;

    dictIndex = 0;
    otherIndex = 0;

    /* Compare filtered elements from both dictionaries */
    while (dictIndex < dictCount && otherIndex < otherCount) {
        SBAttributeItem *dictItem;
        SBAttributeItem *otherItem;

        dictItem = SBAttributeListGetAt(&dictionary->_list, dictIndex);
        otherItem = SBAttributeListGetAt(&other->_list, otherIndex);

        /* Skip non-matching items in the primary dictionary */
        if (!CheckAttributeMatchesFilter(dictItem, registry, targetScope, targetGroup)) {
            dictIndex += 1;
            continue;
        }

        /* Skip non-matching items in the other dictionary */
        if (!CheckAttributeMatchesFilter(otherItem, registry, targetScope, targetGroup)) {
            otherIndex += 1;
            continue;
        }

        /* Both items match the filter; verify they have the same ID and equal values */
        if (dictItem->attributeID != otherItem->attributeID ||
            !SBAttributeRegistryIsEqualAttribute(registry, dictItem->attributeID,
                SBAttributeItemGetValuePtr(dictItem), SBAttributeItemGetValuePtr(otherItem))) {
            return SBFalse;
        }

        dictIndex += 1;
        otherIndex += 1;
    }

    /* Verify primary dictionary has no remaining filtered elements */
    while (dictIndex < dictCount) {
        SBAttributeItem *currentItem = SBAttributeListGetAt(&dictionary->_list, dictIndex);

        if (CheckAttributeMatchesFilter(currentItem, registry, targetScope, targetGroup)) {
            return SBFalse;
        }

        dictIndex += 1;
    }

    /* Verify other dictionary has no remaining filtered elements */
    while (otherIndex < otherCount) {
        SBAttributeItem *currentItem = SBAttributeListGetAt(&other->_list, otherIndex);

        if (CheckAttributeMatchesFilter(currentItem, registry, targetScope, targetGroup)) {
            return SBFalse;
        }

        otherIndex += 1;
    }

    return SBTrue;
}

SB_INTERNAL void AttributeDictionaryRemove(AttributeDictionaryRef dictionary,
    SBAttributeID attributeID, SBAttributeRegistryRef registry, SBBoolean *unchanged)
{
    SBUInteger itemIndex;
    SBBoolean itemFound;

    itemIndex = SBAttributeListBinarySearchIndex(&dictionary->_list, attributeID, &itemFound);

    if (itemFound) {
        SBAttributeItem *item = SBAttributeListGetAt(&dictionary->_list, itemIndex);

        if (registry) {
            const void *valuePtr = SBAttributeItemGetValuePtr(item);

            /* Release the attribute value through the registry before removal */
            SBAttributeRegistryReleaseAttribute(registry, valuePtr);
        }

        /* Remove the item from the list */
        SBAttributeListRemoveAt(&dictionary->_list, itemIndex);
    }

    if (unchanged) {
        *unchanged = !itemFound;
    }
}

SB_INTERNAL void AttributeDictionaryClear(AttributeDictionaryRef dictionary,
    SBAttributeRegistryRef registry)
{
    ReleaseAllAttributeItems(dictionary, registry);
    SBAttributeListRemoveAll(&dictionary->_list);
}

#endif
