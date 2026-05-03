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

#ifndef _SB_PUBLIC_TEXT_ITERATORS_H
#define _SB_PUBLIC_TEXT_ITERATORS_H

#include <SheenBidi/SBAttributeList.h>
#include <SheenBidi/SBBase.h>
#include <SheenBidi/SBTextType.h>

#if SB_TEXT_API_SUPPORTED

SB_EXTERN_C_BEGIN

/* ----------------------------------
 * Paragraph Iterator
 * ---------------------------------- */

/**
 * Opaque reference to a paragraph iterator.
 *
 * The iterator retains its parent `SBText` for its lifetime and must be released with
 * SBParagraphIteratorRelease() when no longer needed.
 *
 * @warning
 *      The parent text must not be modified during iteration. If the text is modified, the iterator
 *      behavior becomes undefined and should be reset before further use.
 */
typedef struct _SBParagraphIterator *SBParagraphIteratorRef;

/**
 * Metadata describing a paragraph.
 */
typedef struct _SBParagraphInfo {
    SBUInteger index;  /**< Start index of the paragraph in code units. */
    SBUInteger length; /**< Length of the paragraph in code units. */
    SBLevel baseLevel; /**< Base level of the paragraph. */
} SBParagraphInfo;

/**
 * Returns the parent text retained by the iterator.
 * 
 * @param iterator
 *      Paragraph iterator.
 * @return
 *      Text associated with the iterator (borrowed).
 */
SB_PUBLIC SBTextRef SBParagraphIteratorGetText(SBParagraphIteratorRef iterator);

/**
 * Resets iteration to the specified code-unit range.
 * 
 * The range is automatically normalized to fit within the text bounds. If the specified range
 * extends beyond the text length, it is clamped to the valid range.
 * 
 * @param iterator
 *      Paragraph iterator.
 * @param index
 *      Start index of the iteration window (in code units).
 * @param length
 *      Length of the iteration window (in code units).
 */
SB_PUBLIC void SBParagraphIteratorReset(SBParagraphIteratorRef iterator, SBUInteger index,
    SBUInteger length);

/**
 * Returns a pointer to the current paragraph information owned by the iterator.
 * 
 * @param iterator
 *      Paragraph iterator.
 * @return
 *      Pointer to `SBParagraphInfo` owned by the iterator.
 *
 * @note
 *      This function always returns the same pointer address for a given iterator instance. Only
 *      the content of the structure is updated with each call to MoveNext.
 * @warning
 *      The client should never modify the returned structure. The client can call this function
 *      once and keep the reference, reading from it after each MoveNext call.
 */
SB_PUBLIC const SBParagraphInfo *SBParagraphIteratorGetCurrent(SBParagraphIteratorRef iterator);

/**
 * Advances to the next paragraph intersecting the window.
 *
 * When the end of the iteration range is reached, subsequent calls return `SBFals`e and the current
 * element becomes invalid.
 *
 * @param iterator
 *      Paragraph iterator.
 * @return
 *      `SBTrue` if advanced to a valid element; `SBFalse` if end reached.
 *
 * @warning
 *      The parent text must not be modified during iteration. If modification occurs, reset the
 *      iterator before continuing.
 */
SB_PUBLIC SBBoolean SBParagraphIteratorMoveNext(SBParagraphIteratorRef iterator);

/**
 * Increases the reference count of the iterator. Each call to retain must be balanced with a call
 * to release.
 *
 * @param iterator
 *      The paragraph iterator to retain.
 * @return
 *      The same iterator object after retention.
 */
SB_PUBLIC SBParagraphIteratorRef SBParagraphIteratorRetain(SBParagraphIteratorRef iterator);

/**
 * Decreases the reference count of the iterator. When the reference count reaches zero, the
 * iterator frees its internal storage and releases the retained text.
 *
 * @param iterator
 *      The iterator to release.
 */
SB_PUBLIC void SBParagraphIteratorRelease(SBParagraphIteratorRef iterator);

/* ----------------------------------
 * Logical Run Iterator
 * ---------------------------------- */

/**
 * Opaque reference to a level run iterator (logical order).
 *
 * Iterates over runs of uniform bidirectional embedding level in logical order. The iterator
 * retains its parent `SBText` and must be released when no longer needed.
 *
 * @warning
 *      The parent text must not be modified during iteration. If the text is modified, the iterator
 *      behavior becomes undefined and should be reset before further use. Multiple instances of
 *      this iterator type can be used concurrently on the same text as long as the text is not
 *      being modified.
 */
typedef struct _SBLogicalRunIterator *SBLogicalRunIteratorRef;

/**
 * A run of uniform resolved bidi level in logical order.
 */
typedef struct _SBLogicalRun {
    SBUInteger index;  /**< Start index of the run in code units. */
    SBUInteger length; /**< Length of the run in code units. */
    SBLevel level;     /**< Resolved bidi level of the run. */
} SBLogicalRun;

/**
 * Returns the parent text retained by the iterator.
 * 
 * @param iterator
 *      Logical run iterator.
 * @return
 *      Text associated with the iterator (borrowed).
 */
SB_PUBLIC SBTextRef SBLogicalRunIteratorGetText(SBLogicalRunIteratorRef iterator);

/**
 * Resets iteration to the specified code-unit range.
 *
 * The range is automatically normalized to fit within the text bounds. If the specified range
 * extends beyond the text length, it is clamped to the valid range.
 *
 * @param iterator
 *      Logical run iterator.
 * @param index
 *      Start index of the iteration window (in code units).
 * @param length
 *      Length of the iteration window (in code units).
 */
SB_PUBLIC void SBLogicalRunIteratorReset(SBLogicalRunIteratorRef iterator, SBUInteger index,
    SBUInteger length);

/**
 * Returns a pointer to the current logical run information owned by the iterator.
 * 
 * @param iterator
 *      Logical run iterator.
 * @return
 *      Pointer to `SBLogicalRun` owned by the iterator.
 *
 * @note
 *      This function always returns the same pointer address for a given iterator instance. Only
 *      the content of the structure is updated with each call to MoveNext.
 * @warning
 *      The client should never modify the returned structure. The client can call this function
 *      once and keep the reference, reading from it after each MoveNext call.
 */
SB_PUBLIC const SBLogicalRun *SBLogicalRunIteratorGetCurrent(SBLogicalRunIteratorRef iterator);

/**
 * Advances to the next logical run.
 *
 * When the end of the iteration range is reached, subsequent calls return `SBFalse` and the current
 * element becomes invalid.
 * 
 * @param iterator
 *      Logical run iterator.
 * @return
 *      `SBTrue` if advanced to a valid element; `SBFalse` if end reached.
 *
 * @warning
 *      The parent text must not be modified during iteration. If modification occurs, reset the
 *      iterator before continuing.
 */
SB_PUBLIC SBBoolean SBLogicalRunIteratorMoveNext(SBLogicalRunIteratorRef iterator);

/**
 * Increases the reference count of the iterator. Each call to retain must be balanced with a call
 * to release.
 *
 * @param iterator
 *      The logical run iterator to retain.
 * @return
 *      The same iterator object after retention.
 */
SB_PUBLIC SBLogicalRunIteratorRef SBLogicalRunIteratorRetain(SBLogicalRunIteratorRef iterator);

/**
 * Decreases the reference count of the iterator. When the reference count reaches zero, the
 * iterator frees its internal storage and releases the retained text.
 *
 * @param iterator
 *      The logical run iterator to release.
 */
SB_PUBLIC void SBLogicalRunIteratorRelease(SBLogicalRunIteratorRef iterator);

/* ----------------------------------
 * Script Run Iterator
 * ---------------------------------- */

/**
 * Opaque reference to a script run iterator.
 *
 * Iterates over runs of uniform Unicode script property. The iterator retains its parent `SBText`
 * and must be released when no longer needed.
 *
 * @warning
 *      The parent text must not be modified during iteration. If the text is modified, the iterator
 *      behavior becomes undefined and should be reset before further use. Multiple instances of
 *      this iterator type can be used concurrently on the same text as long as the text is not
 *      being modified.
 */
typedef struct _SBScriptRunIterator *SBScriptRunIteratorRef;

/**
 * A run of uniform Unicode script property.
 */
typedef struct _SBScriptRun {
    SBUInteger index;  /**< Start index of the run in code units. */
    SBUInteger length; /**< Length of the run in code units. */
    SBScript script;   /**< Script property value for the run. */
} SBScriptRun;

/**
 * Returns the parent text retained by the iterator.
 * 
 * @param iterator
 *      Script run iterator.
 * @return
 *      Text associated with the iterator (borrowed).
 */
SB_PUBLIC SBTextRef SBScriptRunIteratorGetText(SBScriptRunIteratorRef iterator);

/**
 * Resets iteration to the specified code-unit range.
 *
 * The range is automatically normalized to fit within the text bounds. If the specified range
 * extends beyond the text length, it is clamped to the valid range.
 * 
 * @param iterator
 *      Script run iterator.
 * @param index
 *      Start index of the iteration window (in code units).
 * @param length
 *      Length of the iteration window (in code units).
 */
SB_PUBLIC void SBScriptRunIteratorReset(SBScriptRunIteratorRef iterator, SBUInteger index,
    SBUInteger length);

/**
 * Returns a pointer to the current script run information owned by the iterator. The pointer
 * remains valid until the next call to MoveNext or Reset.
 * 
 * @param iterator
 *      Script run iterator.
 * @return
 *      Pointer to `SBScriptRun` owned by the iterator.
 *
 * @note
 *      This function always returns the same pointer address for a given iterator instance. Only
 *      the content of the structure is updated with each call to MoveNext.
 * @warning
 *      The client should never modify the returned structure. The client can call this function
 *      once and keep the reference, reading from it after each MoveNext call.
 */
SB_PUBLIC const SBScriptRun *SBScriptRunIteratorGetCurrent(SBScriptRunIteratorRef iterator);

/**
 * Advances to the next script run.
 *
 * When the end of the iteration range is reached, subsequent calls return `SBFalse` and the current
 * element becomes invalid.
 *
 * @param iterator
 *      Script run iterator.
 * @return
 *      `SBTrue` if advanced to a valid element; `SBFalse` if end reached.
 *
 * @warning
 *      The parent text must not be modified during iteration. If modification occurs, reset the
 *      iterator before continuing.
 */
SB_PUBLIC SBBoolean SBScriptRunIteratorMoveNext(SBScriptRunIteratorRef iterator);

/**
 * Increases the reference count of the iterator. Each call to retain must be balanced with a call
 * to release.
 *
 * @param iterator
 *      The script run iterator to retain.
 * @return
 *      The same iterator object after retention.
 */
SB_PUBLIC SBScriptRunIteratorRef SBScriptRunIteratorRetain(SBScriptRunIteratorRef iterator);

/**
 * Decreases the reference count of the iterator. When the reference count reaches zero, the
 * iterator frees its internal storage and releases the retained text.
 *
 * @param iterator
 *      The iterator to release.
 */
SB_PUBLIC void SBScriptRunIteratorRelease(SBScriptRunIteratorRef iterator);

/* ----------------------------------
 * Attribute Run Iterator
 * ---------------------------------- */

/**
 * Opaque reference to an attribute run iterator.
 *
 * Iterates over runs of text with consistent attribute properties. Supports filtering by attribute
 * ID or attribute group/scope. The iterator retains its parent `SBText` and must be released when
 * no longer needed.
 *
 * @warning
 *      The parent text must not be modified during iteration. If the text is modified, the iterator
 *      behavior becomes undefined and should be reset before further use. Multiple instances of
 *      this iterator type can be used concurrently on the same text as long as the text is not
 *      being modified.
 */
typedef struct _SBAttributeRunIterator *SBAttributeRunIteratorRef;

 /**
 * A run representing a list of attribute items over a range.
 */
typedef struct _SBAttributeRun {
    SBUInteger index;              /**< Start index of the run in code units. */
    SBUInteger length;             /**< Length of the run in code units. */
    SBAttributeListRef attributes; /**< Attributes present on the run. */
} SBAttributeRun;

/**
 * Returns the parent text retained by the iterator.
 * 
 * @param iterator
 *      Attribute run iterator.
 * @return
 *      Text associated with the iterator (borrowed).
 */
SB_PUBLIC SBTextRef SBAttributeRunIteratorGetText(SBAttributeRunIteratorRef iterator);

/**
 * Configures the iterator to only return runs that contain the specified attribute ID. Empty runs
 * (runs with no matching attributes) are automatically skipped during iteration. If no runs match
 * the filter criteria, MoveNext will return `SBFalse`.
 *
 * @param iterator
 *      Attribute run iterator.
 * @param attributeID
 *      The attribute ID by which to filter the runs.
 */
SB_PUBLIC void SBAttributeRunIteratorSetupAttributeID(SBAttributeRunIteratorRef iterator,
    SBAttributeID attributeID);

/**
 * Configures the iterator to only return runs that contain attributes matching the specified group
 * and scope. Empty runs (runs with no matching attributes) are automatically skipped during
 * iteration. If no runs match the filter criteria, MoveNext will return `SBFalse`.
 *
 * @param iterator
 *      Attribute run iterator.
 * @param attributeGroup
 *      The attribute group by which to filter the runs.
 * @param attributeScope
 *      The attribute scope by which to filter the runs.
 */
SB_PUBLIC void SBAttributeRunIteratorSetupAttributeCollection(SBAttributeRunIteratorRef iterator,
    SBAttributeGroup attributeGroup, SBAttributeScope attributeScope);

/**
 * Resets iteration to the specified code-unit range.
 *
 * The range is automatically normalized to fit within the text bounds. If the specified range
 * extends beyond the text length, it is clamped to the valid range.
 *
 * @param iterator
 *      Attribute run iterator.
 * @param index
 *      Start index of the iteration window (in code units).
 * @param length
 *      Length of the iteration window (in code units).
 */
SB_PUBLIC void SBAttributeRunIteratorReset(SBAttributeRunIteratorRef iterator, SBUInteger index,
    SBUInteger length);

/**
 * Returns a pointer to the current attribute run information owned by the iterator. The pointer
 * remains valid until the next call to MoveNext or Reset.
 * 
 * @param iterator
 *      Attribute run iterator.
 * @return
 *      Pointer to `SBAttributeRun` owned by the iterator.
 *
 * @note
 *      This function always returns the same pointer address for a given iterator instance. Only
 *      the content of the structure is updated with each call to MoveNext.
 * @warning
 *      The client should never modify the returned structure. The client can call this function
 *      once and keep the reference, reading from it after each MoveNext call. The `attributes` list
 *      within the structure is also owned by the iterator and should not be modified.
 */
SB_PUBLIC const SBAttributeRun *SBAttributeRunIteratorGetCurrent(SBAttributeRunIteratorRef iterator);

/**
 * Advances to the next attribute run.
 *
 * When the end of the iteration range is reached, subsequent calls return `SBFalse` and the current
 * element becomes invalid. Runs with no attributes matching the current filter are automatically
 * skipped.
 *
 * @param iterator
 *      Attribute run iterator.
 * @return
 *      `SBTrue` if advanced to a valid element; `SBFalse` if end reached.
 *
 * @warning
 *      The parent text must not be modified during iteration. If modification occurs, reset the
 *      iterator before continuing.
 */
SB_PUBLIC SBBoolean SBAttributeRunIteratorMoveNext(SBAttributeRunIteratorRef iterator);

/**
 * Increases the reference count of the iterator. Each call to retain must be balanced with a call
 * to release.
 *
 * @param iterator
 *      The attribute run iterator to retain.
 * @return
 *      The same iterator object after retention.
 */
SB_PUBLIC SBAttributeRunIteratorRef SBAttributeRunIteratorRetain(SBAttributeRunIteratorRef iterator);

/**
 * Decreases the reference count of the iterator. When the reference count reaches zero, the
 * iterator frees its internal storage and releases the retained text.
 *
 * @param iterator
 *      The iterator to release.
 */
SB_PUBLIC void SBAttributeRunIteratorRelease(SBAttributeRunIteratorRef iterator);

/* ----------------------------------
 * Visual Run Iterator
 * ---------------------------------- */

/**
 * Opaque reference to a visual-order run iterator.
 *
 * Iterates over runs of uniform resolved bidi level in visual order (display order). The iterator
 * retains its parent `SBText` and must be released when no longer needed.
 *
 * @warning
 *      The parent text must not be modified during iteration. If the text is modified, the iterator
 *      behavior becomes undefined and should be reset before further use. Multiple instances of
 *      this iterator type can be used concurrently on the same text as long as the text is not
 *      being modified.
 */
typedef struct _SBVisualRunIterator *SBVisualRunIteratorRef;

/**
 * A run of uniform resolved bidi level in visual order.
 */
typedef struct _SBVisualRun {
    SBUInteger index;  /**< Start index of the run in code units. */
    SBUInteger length; /**< Length of the run in code units. */
    SBLevel level;     /**< Resolved bidi level of the run. */
} SBVisualRun;

/**
 * Returns the parent text retained by the iterator.
 * 
 * @param iterator
 *      Visual run iterator.
 * @return
 *      Text associated with the iterator (borrowed).
 */
SB_PUBLIC SBTextRef SBVisualRunIteratorGetText(SBVisualRunIteratorRef iterator);

/**
 * Resets iteration to the specified code-unit range.
 *
 * The range is automatically normalized to fit within the text bounds. If the specified range
 * extends beyond the text length, it is clamped to the valid range.
 *
 * @param iterator
 *      Visual run iterator.
 * @param index
 *      Start index of the iteration window (in code units).
 * @param length
 *      Length of the iteration window (in code units).
 */
SB_PUBLIC void SBVisualRunIteratorReset(SBVisualRunIteratorRef iterator, SBUInteger index,
    SBUInteger length);

/**
 * Returns a pointer to the current visual run information owned by the iterator. The pointer
 * remains valid until the next call to MoveNext or Reset.
 * 
 * @param iterator
 *      Visual run iterator.
 * @return
 *      Pointer to `SBVisualRun` owned by the iterator.
 *
 * @note
 *      This function always returns the same pointer address for a given iterator instance. Only
 *      the content of the structure is updated with each call to MoveNext.
 * @warning
 *      The client should never modify the returned structure. The client can call this function
 *      once and keep the reference, reading from it after each MoveNext call.
 */
SB_PUBLIC const SBVisualRun *SBVisualRunIteratorGetCurrent(SBVisualRunIteratorRef iterator);

/**
 * Advances to the next visual run.
 *
 * When the end of the iteration range is reached, subsequent calls return `SBFalse` and the current
 * element becomes invalid.
 *
 * @param iterator
 *      Visual run iterator.
 * @return
 *      `SBTrue` if advanced to a valid element; `SBFalse` if end reached.
 *
 * @warning
 *      The parent text must not be modified during iteration. If modification occurs, reset the
 *      iterator before continuing.
 */
SB_PUBLIC SBBoolean SBVisualRunIteratorMoveNext(SBVisualRunIteratorRef iterator);

/**
 * Increases the reference count of the iterator. Each call to retain must be balanced with a call
 * to release.
 *
 * @param iterator
 *      The visual run iterator to retain.
 * @return
 *      The same iterator object after retention.
 */
SB_PUBLIC SBVisualRunIteratorRef SBVisualRunIteratorRetain(SBVisualRunIteratorRef iterator);

/**
 * Decreases the reference count of the iterator. When the reference count reaches zero, the
 * iterator frees its internal storage and releases the retained text.
 *
 * @param iterator
 *      The iterator to release.
 */
SB_PUBLIC void SBVisualRunIteratorRelease(SBVisualRunIteratorRef iterator);

SB_EXTERN_C_END

#endif

#endif
