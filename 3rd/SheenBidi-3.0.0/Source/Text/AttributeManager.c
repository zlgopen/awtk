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

#include <API/SBAttributeRegistry.h>
#include <API/SBText.h>
#include <Core/List.h>
#include <Text/AttributeDictionary.h>

#include "AttributeManager.h"

/* =========================================================================
 * Attribute Dictionary Cache Implementation
 * ========================================================================= */

/**
 * Initializes an attribute dictionary cache.
 *
 * The cache stores reusable attribute dictionaries to reduce memory allocations
 * when attributes are frequently added and removed.
 */
static void InitializeAttributeDictionaryCache(AttributeDictionaryCacheRef cache, SBUInt8 valueSize)
{
    ListInitialize(&cache->_attributeDicts, sizeof(AttributeDictionaryRef));
    cache->_valueSize = valueSize;
}

/**
 * Finalizes an attribute dictionary cache and destroys all cached dictionaries.
 */
static void FinalizeAttributeDictionaryCache(AttributeDictionaryCacheRef cache,
    SBAttributeRegistryRef registry)
{
    SBUInteger dictCount = cache->_attributeDicts.count;
    SBUInteger dictIndex;

    /* Destroy all cached attribute dictionaries */
    for (dictIndex = 0; dictIndex < dictCount; dictIndex++) {
        AttributeDictionaryRef dictionary = ListGetVal(&cache->_attributeDicts, dictIndex);
        AttributeDictionaryDestroy(dictionary, registry);
    }

    /* Finalize the cache list */
    ListFinalize(&cache->_attributeDicts);
}

/**
 * Retrieves a cached attribute dictionary or creates a new one if the cache is empty.
 */
static AttributeDictionaryRef AcquireAttributeDictionaryFromCache(AttributeDictionaryCacheRef cache)
{
    AttributeDictionaryRef dictionary = NULL;
    SBUInteger dictCount = cache->_attributeDicts.count;

    if (dictCount == 0) {
        /* Create a new dictionary if the cache is empty */
        dictionary = AttributeDictionaryCreate(cache->_valueSize);
    } else {
        /* Reuse the last cached dictionary */
        dictionary = ListGetVal(&cache->_attributeDicts, dictCount - 1);
        ListRemoveAt(&cache->_attributeDicts, dictCount - 1);
    }

    return dictionary;
}

/**
 * Caches an attribute dictionary for later reuse. The dictionary is cleared before caching.
 */
static void StoreAttributeDictionaryInCache(AttributeDictionaryCacheRef cache,
    AttributeDictionaryRef dictionary, SBAttributeRegistryRef registry)
{
    AttributeDictionaryClear(dictionary, registry);
    ListAdd(&cache->_attributeDicts, &dictionary);
}

/* =========================================================================
 * Attribute Manager Implementation
 * ========================================================================= */

/**
 * Operation types for attribute modifications.
 */
enum {
    AttributeOperationApply = 1,    /**< Apply/merge new attributes. */
    AttributeOperationRemove = 2    /**< Remove specific attribute. */
};
typedef SBUInt8 AttributeOperationType;

/**
 * Determines which side of the split receives the modified attributes.
 */
enum {
    SplitUpdateTargetRight = 0,     /**< Right segment gets modified attributes. */
    SplitUpdateTargetLeft = 1       /**< Left segment gets modified attributes. */
};
typedef SBUInt8 SplitUpdateTarget;

/**
 * Parameters for attribute operations.
 *
 * Uses a union to store operation-specific parameters without memory overhead.
 */
typedef union _AttributeOperationParams {
    struct {
        AttributeDictionaryRef attributes;  /**< Attributes to merge in. */
    } apply;
    struct {
        SBAttributeID attributeID;          /**< Attribute ID to remove. */
    } remove;
} AttributeOperationParams;

/**
 * Expands the range start to include the beginning of the first paragraph.
 */
static void ExpandRangeToIncludeFirstParagraph(AttributeManagerRef manager, SBUInteger *rangeStart)
{
    SBTextRef text = manager->parent;
    SBUInteger paragraphIndex;
    TextParagraphRef paragraph;

    paragraphIndex = SBTextGetCodeUnitParagraphIndex(text, *rangeStart);
    paragraph = ListGetRef(&text->paragraphs, paragraphIndex);

    /* Expand to paragraph start */
    *rangeStart = paragraph->index;
}

/**
 * Expands the range end to include the end of the last paragraph.
 */
static void ExpandRangeToIncludeLastParagraph(AttributeManagerRef manager, SBUInteger *rangeEnd)
{
    SBTextRef text = manager->parent;
    SBUInteger paragraphIndex;
    TextParagraphRef paragraph;

    paragraphIndex = SBTextGetCodeUnitParagraphIndex(text, *rangeEnd - 1);
    paragraph = ListGetRef(&text->paragraphs, paragraphIndex);

    /* Expand to paragraph end */
    *rangeEnd = paragraph->index + paragraph->length;
}

/**
 * Expands both range boundaries to fully include any partially covered paragraphs.
 */
static void ExpandRangeToIncludeBoundaryParagraphs(AttributeManagerRef manager,
    SBUInteger *rangeStart, SBUInteger *rangeEnd)
{
    ExpandRangeToIncludeFirstParagraph(manager, rangeStart);
    ExpandRangeToIncludeLastParagraph(manager, rangeEnd);
}

/**
 * Shrinks the range start to exclude the first paragraph if it's not fully covered.
 */
static void ShrinkRangeToExcludeFirstParagraph(AttributeManagerRef manager,
    SBUInteger *rangeStart, SBUInteger rangeEnd)
{
    SBTextRef text = manager->parent;
    SBUInteger paragraphIndex;
    TextParagraphRef textParagraph;
    SBUInteger paragraphStart;
    SBUInteger paragraphEnd;

    paragraphIndex = SBTextGetCodeUnitParagraphIndex(text, *rangeStart);
    textParagraph = ListGetRef(&text->paragraphs, paragraphIndex);

    paragraphStart = textParagraph->index;
    paragraphEnd = paragraphStart + textParagraph->length;

    /* Exclude the paragraph if not fully covered */
    if (*rangeStart > paragraphStart) {
        *rangeStart = paragraphEnd;

        /* Clamp to rangeEnd */
        if (*rangeStart > rangeEnd) {
            *rangeStart = rangeEnd;
        }
    }
}

/**
 * Shrinks the range end to exclude the last paragraph if it's not fully covered.
 */
static void ShrinkRangeToExcludeLastParagraph(AttributeManagerRef manager,
    SBUInteger rangeStart, SBUInteger *rangeEnd)
{
    SBTextRef text = manager->parent;
    SBUInteger paragraphIndex;
    TextParagraphRef textParagraph;
    SBUInteger paragraphStart;
    SBUInteger paragraphEnd;

    paragraphIndex = SBTextGetCodeUnitParagraphIndex(text, *rangeEnd - 1);
    textParagraph = ListGetRef(&text->paragraphs, paragraphIndex);

    paragraphStart = textParagraph->index;
    paragraphEnd = paragraphStart + textParagraph->length;

    /* Exclude paragraph if not fully covered */
    if (paragraphEnd < *rangeEnd) {
        *rangeEnd = paragraphStart;

        /* Clamp to rangeStart */
        if (*rangeEnd < rangeStart) {
            *rangeEnd = rangeStart;
        }
    }
}

/**
 * Shrinks a range to exclude boundary paragraphs that are not fully covered.
 */
static void ShrinkRangeToExcludeBoundaryParagraphs(AttributeManagerRef manager,
    SBUInteger *rangeStart, SBUInteger *rangeEnd)
{
    ShrinkRangeToExcludeFirstParagraph(manager, rangeStart, *rangeEnd);

    if (*rangeEnd > *rangeStart) {
        ShrinkRangeToExcludeLastParagraph(manager, *rangeStart, rangeEnd);
    }
}

/**
 * Calculates the end index (exclusive) of an attribute entry.
 *
 * For all entries except the last, the end is the start of the next entry.
 * For the last entry, the end is the total code unit count.
 */
static SBUInteger GetAttributeEntryEndIndex(AttributeManagerRef manager, SBUInteger entryIndex)
{
    if (entryIndex < manager->_entries.count - 1) {
        const AttributeEntry *nextEntry = ListGetRef(&manager->_entries, entryIndex + 1);
        return nextEntry->index;
    }

    return manager->_stringLength;
}

/**
 * Shifts all entry indices starting from a given position by a delta amount.
 */
static void ShiftAttributeEntryRanges(AttributeManagerRef manager,
    SBUInteger entryIndex, SBInteger indexDelta) {
    while (entryIndex < manager->_entries.count) {
        AttributeEntry *currentEntry = ListGetRef(&manager->_entries, entryIndex);
        currentEntry->index += indexDelta;
        entryIndex += 1;
    }
}

/**
 * Applies an attribute operation to a dictionary.
 *
 * @param attributes
 *      The attribute dictionary to modify.
 * @param operation
 *      Type of operation to perform.
 * @param params
 *      Operation-specific parameters.
 * @param unchanged
 *      Optional output: set to SBTrue if no changes were made.
 */
static void ApplyOperationToAttributes(AttributeDictionaryRef attributes,
    AttributeOperationType operation, AttributeOperationParams params,
    SBAttributeRegistryRef registry, SBBoolean *unchanged)
{
    switch (operation) {
    case AttributeOperationApply:
        AttributeDictionaryMerge(attributes, params.apply.attributes, registry, unchanged);
        break;
    case AttributeOperationRemove:
        AttributeDictionaryRemove(attributes, params.remove.attributeID, registry, unchanged);
        break;
    }
}

/**
 * Splits an attribute entry at specified index and applies operation to one side.
 *
 * @param manager
 *      The attribute manager containing the entries.
 * @param entryIndex
 *      Index of the entry to split.
 * @param splitIndex
 *      Code unit index where to split the entry.
 * @param operation
 *      Operation to apply after split.
 * @param params
 *      Operation parameters.
 * @param updateTarget
 *      Which side of split receives the modified attributes.
 * @return
 *      SBTrue if split was performed, SBFalse if no split needed.
 */
static SBBoolean SplitAttributesEntry(AttributeManagerRef manager,
    SBUInteger entryIndex, SBUInteger splitIndex, AttributeOperationType operation,
    AttributeOperationParams params, SplitUpdateTarget updateTarget)
{
    SBAttributeRegistryRef registry = manager->_registry;
    AttributeEntry firstEntry;
    SBUInteger entryEnd;

    firstEntry = ListGetVal(&manager->_entries, entryIndex);
    entryEnd = GetAttributeEntryEndIndex(manager, entryIndex);

    /* Only split if split index is strictly between entry boundaries */
    if (splitIndex > firstEntry.index && splitIndex < entryEnd) {
        SBBoolean unchanged = SBTrue;
        AttributeDictionaryRef modifiedAttributes;

        /* Create working copy of attributes for modification */
        modifiedAttributes = AcquireAttributeDictionaryFromCache(&manager->_cache);
        AttributeDictionarySet(modifiedAttributes, firstEntry.attributes, registry);

        /* Apply operation to determine if changes would occur */
        ApplyOperationToAttributes(modifiedAttributes, operation, params, registry, &unchanged);

        if (unchanged) {
            /* Operation produces no change - no split needed, cache the copy */
            StoreAttributeDictionaryInCache(&manager->_cache, modifiedAttributes, registry);
        } else {
            AttributeEntry newEntry;
            newEntry.index = splitIndex;

            if (updateTarget == SplitUpdateTargetRight) {
                /* Right side gets modified attributes, left side keeps original */
                newEntry.attributes = modifiedAttributes;
            } else {
                AttributeEntry *original = ListGetRef(&manager->_entries, entryIndex);

                /* Left side gets modified attributes, right side keeps original */
                newEntry.attributes = original->attributes;
                original->attributes = modifiedAttributes;
            }

            ListInsert(&manager->_entries, entryIndex + 1, &newEntry);

            return SBTrue;
        }
    }

    return SBFalse;
}

/**
 * Core implementation for applying attribute operations over a code unit range.
 *
 * Handles three cases:
 * 1. Operation covers entire entry - apply directly
 * 2. Operation is entirely within one entry - split into 3 entries
 * 3. Operation spans multiple entries - split at boundaries and apply to all
 * 
 * @param manager
 *      The attribute manager to modify.
 * @param index
 *      Starting code unit index of operation range.
 * @param length
 *      Number of code units in operation range.
 * @param operation
 *      Type of operation to perform.
 * @param params
 *      Operation-specific parameters.
 */
static void ApplyOperationOverRange(AttributeManagerRef manager,
    SBUInteger index, SBUInteger length,
    AttributeOperationType operation, AttributeOperationParams params)
{
    SBAttributeRegistryRef registry = manager->_registry;
    SBUInteger rangeStart = index;
    SBUInteger rangeEnd = index + length;
    SBUInteger entryIndex;
    AttributeEntry *entry;
    SBUInteger entryEnd;

    /* Locate the entry containing the range start */
    entry = AttributeManagerFindEntry(manager, rangeStart, &entryIndex, &entryEnd);

    if (rangeStart == entry->index && rangeEnd == entryEnd) {
        /* CASE 1: Operation covers entire entry exactly */
        ApplyOperationToAttributes(entry->attributes, operation, params, registry, NULL);
    } else if (rangeStart > entry->index && rangeEnd < entryEnd) {
        /* CASE 2: Operation is entirely within one entry - split into 3 */
        AttributeDictionaryRef modifiedAttributes;
        SBBoolean unchanged;

        modifiedAttributes = AcquireAttributeDictionaryFromCache(&manager->_cache);
        AttributeDictionarySet(modifiedAttributes, entry->attributes, registry);
        ApplyOperationToAttributes(modifiedAttributes, operation, params, registry, &unchanged);

        if (unchanged) {
            /* No changes needed - don't split */
            StoreAttributeDictionaryInCache(&manager->_cache, modifiedAttributes, registry);
        } else {
            AttributeDictionaryRef cloneAttributes;

            cloneAttributes = AcquireAttributeDictionaryFromCache(&manager->_cache);
            AttributeDictionarySet(cloneAttributes, entry->attributes, registry);

            /* Reserve space for 2 new entries */
            ListReserveRange(&manager->_entries, entryIndex + 1, 2);

            /* Middle entry: gets modified attributes */
            entry = ListGetRef(&manager->_entries, entryIndex + 1);
            entry->index = rangeStart;
            entry->attributes = modifiedAttributes;

            /* Right entry: keeps original attributes */
            entry = ListGetRef(&manager->_entries, entryIndex + 2);
            entry->index = rangeEnd;
            entry->attributes = cloneAttributes;
        }
    } else {
        /* CASE 3: Operation spans multiple entries - handle boundaries and interior */
        SBBoolean entryProcessed = SBFalse;

        /* Split at range start if needed */
        if (SplitAttributesEntry(manager, entryIndex, rangeStart,
                operation, params, SplitUpdateTargetRight)) {
            entryProcessed = SBTrue;    /* Split already applied operation */
            entryIndex += 1;            /* Move to new right-side entry */
        }

        /* Process all entries intersecting with the operation range */
        while (rangeStart < rangeEnd) {
            entry = ListGetRef(&manager->_entries, entryIndex);
            entryEnd = GetAttributeEntryEndIndex(manager, entryIndex);

            /* Split at range end if current entry extends beyond operation range */
            if (entryEnd > rangeEnd) {
                /*
                 * Split at range end - modified attributes go to left side (range interior) so the
                 * portion inside range gets the operation applied
                 */
                SplitAttributesEntry(manager, entryIndex, rangeEnd,
                    operation, params, SplitUpdateTargetLeft);
                /* Remaining portion beyond range is unmodified */
                break;
            }

            /* Apply operation to current entry if not already processed by split */
            if (!entryProcessed) {
                ApplyOperationToAttributes(entry->attributes, operation, params, registry, NULL);
            }

            /* Advance to next entry */
            rangeStart = entryEnd;
            entryIndex += 1;
            entryProcessed = SBFalse;
        }
    }
}

/**
 * Applies a set of attributes over a range by merging them into existing attributes.
 */
static void ApplyAttributesOverRange(AttributeManagerRef manager,
    SBUInteger index, SBUInteger length, AttributeDictionaryRef attributes)
{
    AttributeOperationParams params;
    params.apply.attributes = attributes;

    ApplyOperationOverRange(manager, index, length, AttributeOperationApply, params);
}

/**
 * Removes a specific attribute from a range of code units.
 */
static void RemoveAttributeFromRange(AttributeManagerRef manager,
    SBUInteger index, SBUInteger length, SBAttributeID attributeID)
{
    AttributeOperationParams params;
    params.remove.attributeID = attributeID;

    ApplyOperationOverRange(manager, index, length, AttributeOperationRemove, params);
}

/**
 * Adjusts paragraph-scoped attributes after text has been removed.
 *
 * When text is removed, separate paragraphs may merge into one. This function ensures
 * paragraph-scoped attributes are consistent across the merged boundary by:
 * - Copying paragraph attributes from before the merge point to the second half
 * - Copying paragraph attributes from after the merge point to the first half
 */
static void AdjustParagraphAttributesAfterMerge(AttributeManagerRef manager,
    SBUInteger index, SBUInteger newLength)
{
    SBUInteger rangeEnd = index + newLength;
    SBUInteger mergePointIndex = rangeEnd;

    if (mergePointIndex > 0 && mergePointIndex < manager->_stringLength) {
        SBUInteger precedingIndex = mergePointIndex - 1;
        SBBoolean paragraphsMerged;
        TextParagraphRef precedingParagraph;
        TextParagraphRef followingParagraph;

        SBTextGetBoundaryParagraphs(manager->parent, precedingIndex, mergePointIndex,
            &precedingParagraph, &followingParagraph);
        paragraphsMerged = (precedingParagraph && followingParagraph
            && precedingParagraph == followingParagraph);

        if (paragraphsMerged) {
            SBAttributeRegistryRef registry = manager->_registry;
            SBUInteger paragraphStart = precedingParagraph->index;
            SBUInteger paragraphEnd = paragraphStart + precedingParagraph->length;
            AttributeDictionaryRef paragraphAttributes;
            AttributeEntry *entry;

            paragraphAttributes = &manager->_tempDict;

            /* Extract paragraph-scoped attributes from before the merge point */
            entry = AttributeManagerFindEntry(manager, precedingIndex, NULL, NULL);

            AttributeDictionaryFilter(entry->attributes,
                SBAttributeScopeParagraph, SBAttributeGroupNone, registry, paragraphAttributes);

            if (!AttributeDictionaryIsEmpty(paragraphAttributes)) {
                /* Apply to the second half of the merged paragraph */
                ApplyAttributesOverRange(manager,
                    mergePointIndex, paragraphEnd - mergePointIndex, paragraphAttributes);
            }

            /* Extract paragraph-scoped attributes from after the merge point */
            entry = AttributeManagerFindEntry(manager, mergePointIndex, NULL, NULL);

            AttributeDictionaryFilter(entry->attributes,
                SBAttributeScopeParagraph, SBAttributeGroupNone, registry, paragraphAttributes);

            if (!AttributeDictionaryIsEmpty(paragraphAttributes)) {
                /* Apply to the first half of the merged paragraph */
                ApplyAttributesOverRange(manager,
                    paragraphStart, mergePointIndex - paragraphStart, paragraphAttributes);
            }
        }
    }
}

/**
 * Inserts the first attribute entry at index 0 with an empty attribute dictionary.
 *
 * This is called during initialization to create a base entry covering the entire text.
 */
static void InsertFirstAttributeEntry(AttributeManagerRef manager)
{
    AttributeEntry entry;

    entry.index = 0;
    entry.attributes = AcquireAttributeDictionaryFromCache(&manager->_cache);

    ListAdd(&manager->_entries, &entry);
}

/**
 * Removes and caches a range of attribute entries.
 *
 * Attempts to cache each removed entry's dictionary for reuse, then removes all entries in the
 * specified range from the entries list.
 */
static void RemoveAttributeEntryRange(AttributeManagerRef manager,
    SBUInteger index, SBUInteger length)
{
    SBAttributeRegistryRef registry = manager->_registry;
    SBUInteger rangeEnd = index + length;
    SBUInteger entryIndex;

    for (entryIndex = index; entryIndex < rangeEnd; entryIndex++) {
        AttributeEntry *entry = ListGetRef(&manager->_entries, entryIndex);
        StoreAttributeDictionaryInCache(&manager->_cache, entry->attributes, registry);
    }

    ListRemoveRange(&manager->_entries, index, length);
}

SB_INTERNAL void AttributeManagerInitialize(AttributeManagerRef manager,
    SBTextRef parent, SBAttributeRegistryRef registry)
{
    manager->parent = parent;
    manager->_registry = registry;
    manager->_stringLength = 0;

    if (registry) {
        /* Initialize all structures only when a registry is provided */
        InitializeAttributeDictionaryCache(&manager->_cache, registry->valueSize);
        AttributeDictionaryInitialize(&manager->_tempDict, registry->valueSize);
        ListInitialize(&manager->_entries, sizeof(AttributeEntry));
        InsertFirstAttributeEntry(manager);
    }
}

SB_INTERNAL void AttributeManagerFinalize(AttributeManagerRef manager)
{
    SBAttributeRegistryRef registry = manager->_registry;

    if (registry) {
        SBUInteger entryCount = manager->_entries.count;
        SBUInteger entryIndex;

        AttributeDictionaryFinalize(&manager->_tempDict, NULL);
        FinalizeAttributeDictionaryCache(&manager->_cache, registry);

        /* Finalize all entries */
        for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
            AttributeEntry *entry = ListGetRef(&manager->_entries, entryIndex);
            AttributeDictionaryDestroy(entry->attributes, registry);
        }

        ListFinalize(&manager->_entries);
    }
}

SB_INTERNAL void AttributeManagerCopyAttributes(AttributeManagerRef manager,
    const AttributeManager *source)
{
    SBAttributeRegistryRef registry = manager->_registry;

    if (registry) {
        SBUInteger entryCount = source->_entries.count;
        SBUInteger entryIndex;

        /* Clear existing entries and cache their dictionaries */
        RemoveAttributeEntryRange(manager, 0, manager->_entries.count);
        /* Reserve space for source entries */
        ListReserveRange(&manager->_entries, 0, entryCount);

        /* Deep copy each entry and its attributes */
        for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
            const AttributeEntry *sourceEntry = ListGetRef(&source->_entries, entryIndex);
            AttributeEntry *newEntry = ListGetRef(&manager->_entries, entryIndex);
            AttributeDictionaryRef cloneAttributes;

            cloneAttributes = AcquireAttributeDictionaryFromCache(&manager->_cache);
            AttributeDictionarySet(cloneAttributes, sourceEntry->attributes, registry);

            newEntry->index = sourceEntry->index;
            newEntry->attributes = cloneAttributes;
        }

        manager->_stringLength = source->_stringLength;
    }
}

SB_INTERNAL AttributeEntry *AttributeManagerFindEntry(AttributeManagerRef manager,
    SBUInteger stringIndex, SBUInteger *entryIndex, SBUInteger *entryEnd)
{
    AttributeEntry *entries = manager->_entries.items;
    SBUInteger count = manager->_entries.count;
    SBUInteger stringLength = manager->_stringLength;
    SBUInteger low;
    SBUInteger high;

    SBAssert(entries && count > 0);

    low = 0;
    high = count - 1;

    while (low <= high) {
        SBUInteger mid = low + (high - low) / 2;
        AttributeEntry *entry;
        SBUInteger runStart;
        SBUInteger runEnd;

        entry = &entries[mid];
        runStart = entry->index;
        runEnd = (mid < count - 1) ? entries[mid + 1].index : stringLength;

        if (stringIndex < runStart) {
            /* Target is before the current entry */
            high = mid - 1;
        } else if (stringIndex >= runEnd) {
            /* Target is after the current entry */
            low = mid + 1;
        } else {
            /* Target is within the current entry */
            if (entryIndex) {
                *entryIndex = mid;
            }
            if (entryEnd) {
                *entryEnd = runEnd;
            }

            return entry;
        }
    }

    return NULL;
}

SB_INTERNAL void AttributeManagerReplaceRange(AttributeManagerRef manager,
    SBUInteger replaceStart, SBUInteger oldLength, SBUInteger newLength)
{
    SBAttributeRegistryRef registry = manager->_registry;

    if (registry) {
        SBUInteger replaceEnd = replaceStart + oldLength;
        SBInteger lengthDelta = (SBInteger)(newLength - oldLength);

        if (replaceStart == manager->_stringLength) {
            /* Appending at end - no entries to adjust */
            manager->_stringLength += lengthDelta;
        } else if (manager->_entries.count == 1) {
            /* Single entry covers all text - simple update */
            manager->_stringLength += lengthDelta;
        } else {
            SBUInteger stringIndex;
            SBUInteger firstIndex;
            SBUInteger entryEnd;
            SBUInteger shiftStart;

            /* Select which adjacent code unit's attributes to extend into the range */
            if (oldLength == 0 && replaceStart > 0) {
                /* Insertion: use attributes from the code unit before insertion point */
                stringIndex = replaceStart - 1;
            } else {
                /* Replacement: use attributes from the first replaced code unit */
                stringIndex = replaceStart;
            }

            /* Find the entry containing the reference code unit */
            AttributeManagerFindEntry(manager, stringIndex, &firstIndex, &entryEnd);
            shiftStart = firstIndex + 1;

            /* Remove entries that are completely covered by the replacement range */
            if (entryEnd < replaceEnd) {
                SBUInteger removalStart = firstIndex + 1; /* Keep first entry */
                SBUInteger removalCount = 0;
                SBUInteger scanIndex;

                /* Scan entries to find which ones are completely covered */
                for (scanIndex = removalStart; scanIndex < manager->_entries.count; scanIndex++) {
                    entryEnd = GetAttributeEntryEndIndex(manager, scanIndex);

                    if (entryEnd <= replaceEnd) {
                        /* This entry is completely within replacement range - mark for removal */
                        removalCount += 1;
                    } else {
                        /* Entry extends beyond replacement range - adjust its start position */
                        AttributeEntry *currentEntry = ListGetRef(&manager->_entries, scanIndex);
                        currentEntry->index = replaceEnd;
                        break;
                    }
                }

                /* Remove all marked entries */
                RemoveAttributeEntryRange(manager, removalStart, removalCount);
            }

            /* Special handling for pure deletions */
            if (newLength == 0) {
                AttributeEntry *entry = ListGetRef(&manager->_entries, firstIndex);
                entryEnd = GetAttributeEntryEndIndex(manager, firstIndex);

                /* Check if the first entry exactly matches the deleted range */
                if (entry->index == replaceStart && entryEnd == replaceEnd) {
                    if (firstIndex != 0 || manager->_entries.count > 1) {
                        /* Safe to delete - not the only entry */
                        RemoveAttributeEntryRange(manager, firstIndex, 1);
                        shiftStart -= 1;
                    } else {
                        /* This is the only entry - reset it to cover the empty text */
                        AttributeDictionaryClear(entry->attributes, registry);
                        entry->index = 0;
                    }
                }
            }

            /* Shift all entries after the modified region */
            ShiftAttributeEntryRanges(manager, shiftStart, lengthDelta);
            manager->_stringLength += lengthDelta;

            if (newLength == 0 || oldLength > 0) {
                /* Deletion or replacement can cause paragraph merges */
                AdjustParagraphAttributesAfterMerge(manager, replaceStart, newLength);
            }
        }
    }
}

SB_INTERNAL void AttributeManagerSetAttribute(AttributeManagerRef manager,
    SBUInteger index, SBUInteger length, SBAttributeID attributeID, const void *attributeValue)
{
    SBAttributeRegistryRef registry = manager->_registry;

    SBAssert(length > 0);

    if (registry) {
        const SBAttributeInfo *attributeInfo;

        attributeInfo = SBAttributeRegistryGetInfoReference(registry, attributeID);

        if (attributeInfo) {
            AttributeDictionaryRef attributes = &manager->_tempDict;

            /* Expand the range for paragraph-scoped attributes */
            if (attributeInfo->scope == SBAttributeScopeParagraph) {
                SBUInteger startIndex = index;
                SBUInteger endIndex = startIndex + length;

                ExpandRangeToIncludeBoundaryParagraphs(manager, &startIndex, &endIndex);

                index = startIndex;
                length = endIndex - startIndex;
            }

            /* Prepare single-item dictionary with the attribute */
            AttributeDictionaryClear(attributes, NULL);
            AttributeDictionaryPut(attributes, attributeID, attributeValue, NULL, NULL);

            /* Apply over the (possibly expanded) range */
            ApplyAttributesOverRange(manager, index, length, attributes);
        }
    }
}

SB_INTERNAL void AttributeManagerRemoveAttribute(AttributeManagerRef manager,
    SBUInteger index, SBUInteger length, SBAttributeID attributeID)
{
    SBAttributeRegistryRef registry = manager->_registry;

    SBAssert(length > 0);

    if (registry) {
        const SBAttributeInfo *attributeInfo;

        attributeInfo = SBAttributeRegistryGetInfoReference(registry, attributeID);

        /* Update the range in case of paragraph scope */
        if (attributeInfo->scope == SBAttributeScopeParagraph) {
            SBUInteger startIndex = index;
            SBUInteger endIndex = startIndex + length;

            ShrinkRangeToExcludeBoundaryParagraphs(manager, &startIndex, &endIndex);

            index = startIndex;
            length = endIndex - startIndex;
        }

        if (length > 0) {
            RemoveAttributeFromRange(manager, index, length, attributeID);
        }
    }
}

SB_INTERNAL SBBoolean AttributeManagerGetOnwardRunByFilteringID(AttributeManagerRef manager,
    SBUInteger *runStart, SBUInteger rangeEnd,
    SBAttributeID attributeID, AttributeDictionaryRef output)
{
    SBAttributeRegistryRef registry = manager->_registry;
    SBUInteger entryIndex;
    const AttributeEntry *entry;
    const void *initialValue;

    /* Clear output dictionary before populating */
    AttributeDictionaryClear(output, NULL);

    /* Check for the possibility of a next run first */
    if (*runStart >= rangeEnd) {
        return SBFalse;
    }

    /* Get the first entry and look for the attribute in it */
    entry = AttributeManagerFindEntry(manager, *runStart, &entryIndex, runStart);
    initialValue = AttributeDictionaryFindValue(entry->attributes, attributeID);

    entryIndex += 1;

    if (initialValue) {
        /* Put the initial item to the output dictionary */
        AttributeDictionaryPut(output, attributeID, initialValue, NULL, NULL);

        /* Iterate while the attribute value remains the same */
        while (*runStart < rangeEnd) {
            SBBoolean valuesMatched = SBFalse;
            const void *subsequentValue;

            entry = ListGetRef(&manager->_entries, entryIndex);
            subsequentValue = AttributeDictionaryFindValue(entry->attributes, attributeID);

            /* Check if the attribute value matches */
            if (subsequentValue) {
                valuesMatched = SBAttributeRegistryIsEqualAttribute(registry,
                    attributeID, initialValue, subsequentValue);
            }

            /* Stop if the value changes */
            if (!valuesMatched) {
                break;
            }

            *runStart = GetAttributeEntryEndIndex(manager, entryIndex);
            entryIndex += 1;
        }
    } else {
        /* Iterate while the attribute doesn't exist */
        while (*runStart < rangeEnd) {
            const void *subsequentItem;

            entry = ListGetRef(&manager->_entries, entryIndex);
            subsequentItem = AttributeDictionaryFindValue(entry->attributes, attributeID);

            /* Stop when the attribute appears */
            if (subsequentItem) {
                break;
            }

            *runStart = GetAttributeEntryEndIndex(manager, entryIndex);
            entryIndex += 1;
        }
    }

    if (*runStart > rangeEnd) {
        *runStart = rangeEnd;
    }

    return SBTrue;
}

SB_INTERNAL SBBoolean AttributeManagerGetOnwardRunByFilteringCollection(AttributeManagerRef manager,
    SBUInteger *runStart, SBUInteger rangeEnd,
    SBAttributeScope filterScope, SBAttributeGroup filterGroup, AttributeDictionaryRef output)
{
    SBAttributeRegistryRef registry = manager->_registry;
    SBUInteger entryIndex;
    const AttributeEntry *entry;

    /* Clear the output dictionary before populating */
    AttributeDictionaryClear(output, NULL);

    /* Check for the possibility of a next run first */
    if (*runStart >= rangeEnd) {
        return SBFalse;
    }

    /* Get the first entry and filter its attributes */
    entry = AttributeManagerFindEntry(manager, *runStart, &entryIndex, runStart);
    AttributeDictionaryFilter(entry->attributes, filterScope, filterGroup, registry, output);

    entryIndex += 1;

    if (AttributeDictionaryIsEmpty(output)) {
        /* Iterate while no matching attributes exist */
        while (*runStart < rangeEnd) {
            entry = ListGetRef(&manager->_entries, entryIndex);

            /* Stop when matching attributes appear */
            if (AttributeDictionaryMatchAny(entry->attributes, filterScope, filterGroup, registry)) {
                break;
            }

            *runStart = GetAttributeEntryEndIndex(manager, entryIndex);
            entryIndex += 1;
        }
    } else {
        /* Iterate while the filtered attributes remain the same */
        while (*runStart < rangeEnd) {
            entry = ListGetRef(&manager->_entries, entryIndex);

            /* Stop if filtered attributes change */
            if (!AttributeDictionaryMatchAll(entry->attributes, filterScope, filterGroup, registry, output)) {
                break;
            }

            *runStart = GetAttributeEntryEndIndex(manager, entryIndex);
            entryIndex += 1;
        }
    }

    if (*runStart > rangeEnd) {
        *runStart = rangeEnd;
    }

    return SBTrue;
}

#endif
