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

#ifndef _SB_INTERNAL_ATTRIBUTE_MANAGER_H
#define _SB_INTERNAL_ATTRIBUTE_MANAGER_H

#include <API/SBBase.h>

#if SB_TEXT_API_SUPPORTED

#include <SheenBidi/SBAttributeInfo.h>
#include <SheenBidi/SBAttributeRegistry.h>
#include <SheenBidi/SBText.h>

#include <Core/List.h>
#include <Text/AttributeDictionary.h>

typedef struct _AttributeDictionaryCache {
    LIST(AttributeDictionaryRef) _attributeDicts;
    SBUInt8 _valueSize;
} AttributeDictionaryCache, *AttributeDictionaryCacheRef;

typedef struct _AttributeEntry {
    SBUInteger index;
    AttributeDictionaryRef attributes;
} AttributeEntry;

typedef struct _AttributeManager {
    SBTextRef parent;
    SBAttributeRegistryRef _registry;
    SBUInteger _stringLength;
    AttributeDictionaryCache _cache;
    AttributeDictionary _tempDict;
    LIST(AttributeEntry) _entries;
} AttributeManager, *AttributeManagerRef;

/**
 * Initializes an attribute manager.
 *
 * Prepares the attribute manager for use by initializing its internal data structures. When a
 * registry is provided, the manager allocates a dictionary cache, a temporary dictionary for
 * operations, an entries list, and inserts the first entry at index 0 with an empty attribute
 * dictionary. If registry is NULL, the manager remains uninitialized and all subsequent operations
 * become no-ops.
 *
 * @param manager
 *      The attribute manager to initialize.
 * @param parent
 *      The parent text object that owns this manager.
 * @param registry
 *      The attribute registry for managing attribute retention and release. If NULL, the manager
 *      will not allocate resources and all operations will be skipped.
 */
SB_INTERNAL void AttributeManagerInitialize(AttributeManagerRef manager,
    SBTextRef parent, SBAttributeRegistryRef registry);

/**
 * Finalizes an attribute manager and releases all resources.
 *
 * Releases all attribute dictionaries managed by the manager, clears the dictionary cache, and
 * finalizes all internal data structures. After calling this function, the manager must not be used
 * until re-initialized.
 *
 * @param manager
 *      The attribute manager to finalize. If initialized with NULL registry, this function safely
 *      returns without performing any operations.
 */
SB_INTERNAL void AttributeManagerFinalize(AttributeManagerRef manager);

/**
 * Copies all attributes from a source attribute manager to this manager.
 *
 * Creates deep copies of all attribute dictionaries from the source manager, preserving both the
 * entry indices and the attribute values. The process first removes and caches all existing entries
 * in the destination manager, reserves space for entries from the source, then iterates through
 * each source entry, copying its index and deep-copying its attribute dictionary (which retains all
 * attribute values through the registry).
 *
 * @param manager
 *      The attribute manager to copy attributes into.
 * @param source
 *      The source attribute manager to copy from. Must have the same registry.
 *
 * @note
 *      If the destination manager has no registry, this function returns immediately without
 *      performing any operations.
 * @note
 *      All existing entries and attributes in the destination manager are replaced, not appended
 *      to. The code unit count is also synchronized from the source.
 */
SB_INTERNAL void AttributeManagerCopyAttributes(AttributeManagerRef manager,
    const AttributeManager *source);

/**
 * Finds the attribute entry containing a specific code unit index.
 *
 * Performs a binary search through the entries list to locate the entry that covers the given code
 * unit index. Each entry covers a range from its index to the start of the next entry (or to the
 * total code unit count for the last entry). Returns the entry reference and optionally sets the
 * entry index.
 *
 * @param manager
 *      The attribute manager to search.
 * @param stringIndex
 *      The code unit index to find.
 * @param[out] entryIndex
 *      Optional pointer to receive the index of the found entry in the entries list.
 *      If NULL, the entry index is not returned.
 * @return
 *      Pointer to the attribute entry containing the code unit, or NULL if not found.
 *      In practice, should always return a valid entry if the index is within valid bounds.
 */
SB_INTERNAL AttributeEntry *AttributeManagerFindEntry(AttributeManagerRef manager,
    SBUInteger stringIndex, SBUInteger *entryIndex, SBUInteger *entryEnd);

/**
 * Replaces a range of text and adjusts attribute entries accordingly.
 *
 * @param manager
 *      The attribute manager to modify
 * @param replaceStart
 *      Starting index of the range to replace
 * @param oldLength
 *      Number of code units being replaced
 * @param newLength
 *      Number of code units in the replacement text
 *
 * @note
 *      - For insertions (oldLength == 0), uses attributes from previous code unit
 *      - For replacements, uses attributes from first replaced code unit
 *      - Handles paragraph merging when text is deleted
 */
SB_INTERNAL void AttributeManagerReplaceRange(AttributeManagerRef manager,
    SBUInteger replaceStart, SBUInteger oldLength, SBUInteger newLength);

#define AttributeManagerReserveRange(manager, index, length) \
    AttributeManagerReplaceRange(manager, index, 0, length)

#define AttributeManagerRemoveRange(manager, index, length) \
    AttributeManagerReplaceRange(manager, index, length, 0)

/**
 * Sets an attribute over a range of code units with automatic scope expansion.
 *
 * Applies an attribute to all code units in the specified range. For paragraph-scoped attributes,
 * automatically expands the range to include complete paragraphs at the boundaries, ensuring that
 * paragraph attributes apply uniformly across entire paragraphs rather than partial ones. The
 * attribute value is retained through the registry.
 * The implementation prepares a single-item dictionary containing the attribute and applies it over
 * the (possibly expanded) range using the standard apply operation.
 *
 * @param manager
 *      The attribute manager to modify. If manager has no registry, this function returns
 *      immediately without performing any operations.
 * @param index
 *      The starting code unit index of the range.
 * @param length
 *      The number of code units in the range. Must be greater than 0. May be expanded for
 *      paragraph-scoped attributes.
 * @param attributeID
 *      The ID of the attribute to set.
 * @param attributeValue
 *      The value of the attribute. Will be retained via the registry and applied to the specified
 *      attribute ID.
 */
SB_INTERNAL void AttributeManagerSetAttribute(AttributeManagerRef manager,
    SBUInteger index, SBUInteger length, SBAttributeID attributeID, const void *attributeValue);

/**
 * Removes an attribute from a range of code units with automatic scope shrinking.
 *
 * Removes a specific attribute from all code units in the specified range. For paragraph-scoped
 * attributes, automatically shrinks the range to exclude any partially covered paragraphs at the
 * boundaries, ensuring that paragraph attributes are only removed from fully covered paragraphs. If
 * the range shrinks to zero length, no operation is performed. If the manager has no registry, this
 * function returns immediately without performing any operations.
 *
 * @param manager
 *      The attribute manager to modify.
 * @param index
 *      The starting code unit index of the range.
 * @param length
 *      The number of code units in the range. Must be greater than 0. May be shrunk for
 *      paragraph-scoped attributes.
 * @param attributeID
 *      The ID of the attribute to remove from the range.
 */
SB_INTERNAL void AttributeManagerRemoveAttribute(AttributeManagerRef manager,
    SBUInteger index, SBUInteger length, SBAttributeID attributeID);

/**
 * Retrieves the next contiguous run with uniform value for a specific attribute.
 *
 * Finds the next "run" starting from runStart, where a run is a maximal contiguous range of code
 * units that either all have the same value for the specified attribute, or all lack the attribute
 * entirely.
 * The function first clears the output dictionary, then checks if the starting position is beyond
 * rangeEnd. It locates the first entry, extracts and examines the attribute, then extends the run
 * as far as possible while the attribute remains either present with equal value (using registry
 * equality comparison) or consistently absent. Updates runStart to point to the end of the found
 * run (exclusive) and populates the output dictionary with the attribute item if present in the
 * run, or leaves it empty if the run represents absent attributes.
 *
 * @param manager
 *      The attribute manager to query.
 * @param[in,out] runStart
 *      Pointer to the starting code unit index. On return, updated to the index immediately
 *      following the end of the found run.
 * @param rangeEnd
 *      The exclusive upper bound for the search. The run does not include this index.
 * @param attributeID
 *      The ID of the attribute to filter by.
 * @param[out] output
 *      Dictionary to populate with the attribute item if present in the run; will be empty if the
 *      run represents absent attributes. Always cleared at the start.
 * @return
 *      SBTrue if runStart < rangeEnd (a run was processed), SBFalse if runStart >= rangeEnd on
 *      entry (no more runs available).
 */
SB_INTERNAL SBBoolean AttributeManagerGetOnwardRunByFilteringID(AttributeManagerRef manager,
    SBUInteger *runStart, SBUInteger rangeEnd,
    SBAttributeID attributeID, AttributeDictionaryRef output);

/**
 * Retrieves the next contiguous run with uniform filtered attribute collection.
 *
 * Finds the next "run" starting from runStart, where a run is a maximal contiguous range of code
 * units that all have the same collection of attributes matching the specified scope and group
 * filters (or all have no matching attributes).
 * The function first clears the output dictionary, then checks if the starting position is beyond
 * rangeEnd. It locates the first entry and filters its attributes according to the scope and group.
 * If no matching attributes are found in the first entry, the run extends while no matching
 * attributes appear in subsequent entries. If matching attributes are found, the run extends while
 * all subsequent entries have exactly the same matching attributes (same IDs and equal values).
 * Updates runStart to point to the end of the found run (exclusive) and populates the output
 * dictionary with all matching attribute items from the run, or leaves it empty if the run
 * represents absence of matching attributes.
 *
 * @param manager
 *      The attribute manager to query.
 * @param[in,out] runStart
 *      Pointer to the starting code unit index. On return, updated to the index immediately
 *      following the end of the found run.
 * @param rangeEnd
 *      The exclusive upper bound for the search. The run does not include this index.
 * @param filterScope
 *      The attribute scope to filter by (e.g., character or paragraph). Only attributes matching
 *      this scope are included in runs.
 * @param filterGroup
 *      The attribute group to filter by, or SBAttributeGroupNone to match all groups within the
 *      specified scope.
 * @param[out] output
 *      Dictionary to populate with all matching attribute items from the run; will be empty if the
 *      run represents absence of matching attributes. Always cleared at the start.
 * @return
 *      SBTrue if runStart < rangeEnd (a run was processed), SBFalse if runStart >= rangeEnd on
 *      entry (no more runs available).
 */
SB_INTERNAL SBBoolean AttributeManagerGetOnwardRunByFilteringCollection(AttributeManagerRef manager,
    SBUInteger *runStart, SBUInteger rangeEnd,
    SBAttributeScope filterScope, SBAttributeGroup filterGroup, AttributeDictionaryRef output);

#endif

#endif
