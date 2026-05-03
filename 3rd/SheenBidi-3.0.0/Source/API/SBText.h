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

#ifndef _SB_INTERNAL_TEXT_H
#define _SB_INTERNAL_TEXT_H

#include <API/SBBase.h>

#if SB_TEXT_API_SUPPORTED

#include <SheenBidi/SBAttributeRegistry.h>
#include <SheenBidi/SBCodepointSequence.h>
#include <SheenBidi/SBParagraph.h>
#include <SheenBidi/SBScriptLocator.h>
#include <SheenBidi/SBText.h>

#include <Core/List.h>
#include <Core/Object.h>
#include <Text/AttributeManager.h>

typedef struct _TextParagraph {
    SBUInteger index;
    SBUInteger length;
    SBBoolean needsReanalysis;
    SBParagraphRef bidiParagraph;
    LIST(SBScript) scripts;
} TextParagraph, *TextParagraphRef;

typedef struct _SBText {
    ObjectBase _base;
    SBStringEncoding encoding;
    SBBoolean isMutable;
    SBLevel baseLevel;
    SBBoolean isEditing;
    SBScriptLocatorRef scriptLocator;
    SBAttributeRegistryRef attributeRegistry;
    AttributeManager attributeManager;
    List codeUnits;
    LIST(SBBidiType) bidiTypes;
    LIST(TextParagraph) paragraphs;
} SBText;

/**
 * Creates a mutable text object with explicit parameters.
 * 
 * @param encoding
 *      String encoding.
 * @param attributeRegistry
 *      Attribute registry (can be `NULL`).
 * @param baseLevel
 *      Base bidirectional level.
 * @return
 *      New mutable text object, or `NULL` on failure.
 */
SB_INTERNAL SBMutableTextRef SBTextCreateMutableWithParameters(SBStringEncoding encoding,
    SBAttributeRegistryRef attributeRegistry, SBLevel baseLevel);

/**
 * Finds the paragraph index containing the specified code unit index.
 * 
 * @param text
 *      The text object.
 * @param codeUnitIndex
 *      The code unit index to search for.
 * @return
 *      The index of the paragraph containing the code unit, or `SBInvalidIndex` if not found.
 */
SB_INTERNAL SBUInteger SBTextGetCodeUnitParagraphIndex(SBTextRef text, SBUInteger codeUnitIndex);

/**
 * Retrieves the first and last paragraphs that intersect with a specified code unit range. If the
 * range spans a single paragraph, both output parameters reference the same paragraph.
 *
 * @param text
 *      The text object.
 * @param rangeStart
 *      The starting code unit index (inclusive).
 * @param rangeEnd
 *      The ending code unit index (exclusive).
 * @param[out] firstParagraph
 *      Pointer to receive the first intersecting paragraph reference.
 * @param[out] lastParagraph
 *      Pointer to receive the last intersecting paragraph reference.
 */
SB_INTERNAL void SBTextGetBoundaryParagraphs(SBTextRef text,
    SBUInteger rangeStart, SBUInteger rangeEnd,
    TextParagraphRef *firstParagraph, TextParagraphRef *lastParagraph);

#endif

#endif
