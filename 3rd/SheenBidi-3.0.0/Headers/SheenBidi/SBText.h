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

#ifndef _SB_PUBLIC_TEXT_H
#define _SB_PUBLIC_TEXT_H

#include <SheenBidi/SBAttributeRegistry.h>
#include <SheenBidi/SBBase.h>
#include <SheenBidi/SBCodepointSequence.h>
#include <SheenBidi/SBTextConfig.h>
#include <SheenBidi/SBTextIterators.h>
#include <SheenBidi/SBTextType.h>

#if SB_TEXT_API_SUPPORTED

SB_EXTERN_C_BEGIN

/**
 * Creates an immutable text object from raw code units and configuration.
 *
 * The text object will analyze the content for bidirectional properties, script identification, and
 * paragraph boundaries upon creation.
 *
 * @param string
 *      Pointer to code units in the specified encoding.
 * @param length
 *      Number of code units in string.
 * @param encoding
 *      String encoding (UTF-8, UTF-16, or UTF-32).
 * @param config
 *      Non-NULL configuration that supplies the attribute registry and defaults.
 * @return
 *      A reference to a text object if the call was successful, `NULL` otherwise.
 */
SB_PUBLIC SBTextRef SBTextCreate(const void *string, SBUInteger length, SBStringEncoding encoding,
    SBTextConfigRef config);

/**
 * Creates a new immutable text object that is an exact copy of the source text.
 *
 * The copy contains all code units, bidirectional properties, script information, paragraph
 * analysis, and attributes from the source.
 *
 * @param text
 *      Source text object to copy.
 * @return
 *      New immutable copy, or `NULL` on failure.
 */
SB_PUBLIC SBTextRef SBTextCreateCopy(SBTextRef text);

/**
 * Creates a new mutable text object that is an exact copy of the source text.
 *
 * The copy can be modified using the mutable text interface while the original remains unchanged.
 *
 * @param text
 *      Source text object to copy (can be immutable or mutable).
 * @return
 *      New mutable text object with the same content and properties as the source.
 */
SB_PUBLIC SBMutableTextRef SBTextCreateMutableCopy(SBTextRef text);

/**
 * Returns the encoding specified at text object creation.
 *
 * This encoding remains constant for the lifetime of the text object.
 *
 * @param text
 *      Text object.
 * @return
 *      String encoding used by this text object.
 */
SB_PUBLIC SBStringEncoding SBTextGetEncoding(SBTextRef text);

/**
 * Returns a borrowed reference to the attribute registry associated with the text object.
 *
 * The registry defines the valid attribute IDs and their value management callbacks.
 * 
 * @param text
 *      Text object.
 * @return
 *      Attribute registry (borrowed; do not release).
 */
SB_PUBLIC SBAttributeRegistryRef SBTextGetAttributeRegistry(SBTextRef text);

/**
 * Returns the number of code units in the text object.
 * 
 * @param text
 *      Text object.
 * @return
 *      Number of code units in the text.
 */
SB_PUBLIC SBUInteger SBTextGetLength(SBTextRef text);

/**
 * Copies code units from the text into a caller-provided buffer.
 *
 * The buffer must be large enough to hold the requested number of code units in the text's encoding
 * format.
 *
 * @param text
 *      Text object.
 * @param index
 *      Start index (in code units).
 * @param length
 *      Number of code units to copy.
 * @param buffer
 *      Output buffer (must be large enough for `length` code units).
 *
 * @warning
 *      Behavior is undefined if buffer is too small or range is invalid.
 */
SB_PUBLIC void SBTextGetCodeUnits(SBTextRef text, SBUInteger index, SBUInteger length,
    void *buffer);

/**
 * Copies bidirectional types for a code-unit range.
 *
 * @param text
 *      Text object.
 * @param index
 *      Start index (in code units).
 * @param length
 *      Number of code units to copy bidi types for.
 * @param buffer
 *      Output array of `SBBidiType` with `length` entries.
 *
 * @warning
 *      Behavior is undefined if buffer is too small, range is invalid, or text is currently being
 *      edited.
 */
SB_PUBLIC void SBTextGetBidiTypes(SBTextRef text, SBUInteger index, SBUInteger length,
    SBBidiType *buffer);

/**
 * Copies script identifications for a code-unit range.
 * 
 * @param text
 *      Text object.
 * @param index
 *      Start index (in code units).
 * @param length
 *      Number of code units to copy the scripts for.
 * @param buffer
 *      Output array of `SBScript` with `length` entries.
 *
 * @warning
 *      Behavior is undefined if buffer is too small, range is invalid, or text is currently being
 *      edited.
 */
SB_PUBLIC void SBTextGetScripts(SBTextRef text, SBUInteger index, SBUInteger length,
    SBScript *buffer);

/**
 * Copies resolved bidirectional levels for a code-unit range.
 * 
 * @param text
 *      Text object.
 * @param index
 *      Start index (in code units).
 * @param length
 *      Number of code units to copy the bidirectional levels for.
 * @param buffer
 *      Output array of SBLevel with `length` entries.
 *
 * @warning
 *      Behavior is undefined if buffer is too small, range is invalid, or text is currently being
 *      edited.
 */
SB_PUBLIC void SBTextGetResolvedLevels(SBTextRef text, SBUInteger index, SBUInteger length,
    SBLevel *buffer);

/**
 * Retrieves information for the paragraph containing a specific code unit.
 * 
 * @param text
 *      Text object.
 * @param index
 *      Code-unit index whose paragraph is queried.
 * @param paragraphInfo
 *      Output pointer to `SBParagraphInfo` to receive data.
 *
 * @warning
 *      Index must be valid and text must not be undergoing editing.
 */
SB_PUBLIC void SBTextGetCodeUnitParagraphInfo(SBTextRef text, SBUInteger index,
    SBParagraphInfo *paragraphInfo);

/**
 * Creates a new paragraph iterator that can traverse all paragraphs in the text object.
 *
 * The iterator starts before the first paragraph and must be advanced with MoveNext().
 * 
 * @param text
 *      Text object.
 * @return
 *      Paragraph iterator on success, `NULL` on failure.
 */
SB_PUBLIC SBParagraphIteratorRef SBTextCreateParagraphIterator(SBTextRef text);

/**
 * Creates a new logical run iterator that can traverse all bidirectional level runs in logical
 * order.
 *
 * Logical runs represent contiguous sequences of text with the same resolved bidirectional level.
 * 
 * @param text
 *      Text object.
 * @return
 *      Logical run iterator on success, `NULL` on failure.
 */
SB_PUBLIC SBLogicalRunIteratorRef SBTextCreateLogicalRunIterator(SBTextRef text);

/**
 * Creates a new script run iterator that can traverse all script runs in the text.
 *
 * Script runs represent contiguous sequences of text with the same Unicode script property.
 * 
 * @param text
 *      Text object.
 * @return
 *      Script run iterator on success, `NULL` on failure.
 */
SB_PUBLIC SBScriptRunIteratorRef SBTextCreateScriptRunIterator(SBTextRef text);

/**
 * Creates a new attribute run iterator that can traverse all attribute runs in the text.
 *
 * Initially, the iterator has no filter applied and will return all attribute runs. Use the Setup
 * functions to apply filtering by attribute ID or group/scope.
 * 
 * @param text
 *      Text object.
 * @return
 *      Attribute iterator on success, `NULL` on failure.
 */
SB_PUBLIC SBAttributeRunIteratorRef SBTextCreateAttributeRunIterator(SBTextRef text);

/**
 * Creates a new visual run iterator that can traverse bidirectional runs in visual order for a
 * specific line range.
 *
 * Visual runs represent text as it should appear on screen after bidirectional reordering.
 * 
 * @param text
 *      Text object.
 * @param index 
 *      Start index of the line (in code units).
 * @param length
 *      Length of the line (in code units).
 * @return
 *      Visual run iterator on success, `NULL` on failure.
 */
SB_PUBLIC SBVisualRunIteratorRef SBTextCreateVisualRunIterator(SBTextRef text, SBUInteger index,
    SBUInteger length);

/**
 * Increments the reference count of a text object.
 * 
 * @param text
 *      The text object whose reference count will be incremented.
 * @return
 *      The same text object passed in as the parameter.
 */
SB_PUBLIC SBTextRef SBTextRetain(SBTextRef text);

/**
 * Decrements the reference count of a text object. The object will be deallocated when its
 * reference count reaches zero.
 *
 * @param text
 *      The text object whose reference count will be decremented.
 */
SB_PUBLIC void SBTextRelease(SBTextRef text);


/* ----------------------------------
 * Mutable Text
 * ---------------------------------- */

/**
 * Creates a new mutable text object that starts empty but can be modified through the mutable text
 * interface. The encoding and attribute registry are fixed at creation.
 * 
 * @param encoding
 *      Target encoding for the new text (UTF-8/16/32).
 * @param config
 *      Non-NULL configuration with registry and defaults.
 * @return
 *      A reference to a mutable text object if the call was successful, `NULL` otherwise.
 */
SB_PUBLIC SBMutableTextRef SBTextCreateMutable(SBStringEncoding encoding, SBTextConfigRef config);

/**
 * Signals the start of a batch of editing operations. While in editing mode, text analysis
 * (bidirectional processing, script identification) is deferred until SBTextEndEditing() is called.
 * This improves performance when making multiple sequential modifications.
 * 
 * @param text
 *      Mutable text object.
 */
SB_PUBLIC void SBTextBeginEditing(SBMutableTextRef text);

/**
 * Signals the end of a batch editing session and triggers text analysis for all modified
 * paragraphs. This includes bidirectional analysis, script identification, and paragraph boundary
 * detection.
 * 
 * @param text
 *      Mutable text object.
 */
SB_PUBLIC void SBTextEndEditing(SBMutableTextRef text);

/**
 * Adds new code units to the end of the text object. If the text is not in editing mode, analysis
 * is performed immediately on the new content.
 * 
 * @param text
 *      Mutable text object.
 * @param codeUnitBuffer
 *      Pointer to code units in the text's encoding.
 * @param codeUnitCount
 *      Number of code units to append.
 */
SB_PUBLIC void SBTextAppendCodeUnits(SBMutableTextRef text, const void *codeUnitBuffer,
    SBUInteger codeUnitCount);

/**
 * Inserts new code units at the specified position in the text. Existing content at and after the
 * insertion point is shifted right. If the text is not in editing mode, analysis is performed
 * immediately.
 * 
 * @param text
 *      Mutable text object.
 * @param index
 *      Insertion index (in code units).
 * @param codeUnitBuffer
 *      Pointer to code units in the text's encoding.
 * @param codeUnitCount
 *      Number of code units to insert.
 *
 * @warning
 *      The index must be within the range [0, current length].
 *      The buffer must contain valid code units in the text's encoding format.
 */
SB_PUBLIC void SBTextInsertCodeUnits(SBMutableTextRef text, SBUInteger index,
    const void *codeUnitBuffer, SBUInteger codeUnitCount);

/**
 * Removes a contiguous range of code units from the text. Content after the deletion range is
 * shifted left. If the text is not in editing mode, analysis is performed immediately.
 *
 * @param text
 *      Mutable text object.
 * @param index
 *      Start index of the range to delete (in code units).
 * @param length
 *      Number of code units to delete.
 *
 * @warning
 *      The deletion range [index, index+length) must be within the current text bounds.
 */
SB_PUBLIC void SBTextDeleteCodeUnits(SBMutableTextRef text, SBUInteger index, SBUInteger length);

/**
 * Completely replaces the current text content with the new code units. This is equivalent to
 * deleting all existing content and then inserting the new content at position 0.
 * 
 * @param text
 *      Mutable text object.
 * @param codeUnitBuffer
 *      Pointer to code units in the text's encoding.
 * @param codeUnitCount
 *      Number of code units in codeUnitBuffer.
 *
 * @warning
 *      All existing content and attributes are removed.
 *      The buffer must contain valid code units in the text's encoding format.
 */
SB_PUBLIC void SBTextSetCodeUnits(SBMutableTextRef text, const void *codeUnitBuffer,
    SBUInteger codeUnitCount);

/**
 * Replaces a contiguous range of existing code units with new content. If the text is not in
 * editing mode, analysis is performed immediately.
 * 
 * @param text
 *      Mutable text object.
 * @param index
 *      Start index of the range to replace (in code units).
 * @param length
 *      Length of the range to replace (in code units).
 * @param codeUnitBuffer
 *      Pointer to replacement code units in the text's encoding.
 * @param codeUnitCount
 *      Number of replacement code units.
 *
 * @warning
 *      The replacement range [index, index+length) must be within the current text bounds.
 *      The buffer must contain valid code units in the text's encoding format.
 */
SB_PUBLIC void SBTextReplaceCodeUnits(SBMutableTextRef text, SBUInteger index, SBUInteger length,
    const void *codeUnitBuffer, SBUInteger codeUnitCount);

/**
 * Applies the specified attribute with the given value to the range of code units. If the attribute
 * already exists in parts of the range, those values are replaced. The attribute value is managed
 * according to the callbacks defined in the attribute registry.
 * 
 * @param text
 *      Mutable text object.
 * @param index
 *      Start index of the range (in code units).
 * @param length
 *      Length of the range (in code units).
 * @param attributeID
 *      ID of the attribute to set.
 * @param attributeValue
 *      Value pointer to associate; retained via registry callbacks if applicable.
 * @return
 *      `SBTrue` on success, `SBFalse` on failure.
 *
 * @warning
 *      The range must be within the current text bounds.
 *      The attribute ID must be registered in the text's attribute registry.
 *      The attribute value must be compatible with the attribute's value type as defined in the
 *      registry.
 */
SB_PUBLIC void SBTextSetAttribute(SBMutableTextRef text, SBUInteger index, SBUInteger length,
    SBAttributeID attributeID, const void *attributeValue);

/**
 * Removes the specified attribute from the given range of code units. If the attribute is not
 * present in the range, this operation has no effect.
 *
 * @param text
 *      Mutable text object.
 * @param index
 *      Start index of the range (in code units).
 * @param length
 *      Length of the range (in code units).
 * @param attributeID
 *      ID of the attribute to remove.
 *
 * @warning
 *      The range must be within the current text bounds.
 *      The attribute ID must be registered in the text's attribute registry.
 */
SB_PUBLIC void SBTextRemoveAttribute(SBMutableTextRef text, SBUInteger index, SBUInteger length,
    SBAttributeID attributeID);

SB_EXTERN_C_END

#endif

#endif
