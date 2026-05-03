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

#ifndef _SB_INTERNAL_TEXT_ITERATORS_H
#define _SB_INTERNAL_TEXT_ITERATORS_H

#include <API/SBBase.h>

#if SB_TEXT_API_SUPPORTED

#include <SheenBidi/SBLine.h>
#include <SheenBidi/SBTextIterators.h>

#include <API/SBText.h>
#include <Core/Object.h>
#include <Text/AttributeDictionary.h>

typedef struct _TextIterator {
    SBTextRef text;
    SBBoolean visualDirectionMode;
    SBBoolean forwardMode;
    SBUInteger startIndex;
    SBUInteger endIndex;
    SBUInteger paragraphIndex;
    TextParagraphRef currentParagraph;
    SBUInteger paragraphStart;
    SBUInteger paragraphEnd;
} TextIterator, *TextIteratorRef;

typedef struct _SBParagraphIterator {
    ObjectBase _base;
    TextIterator parent;
    SBParagraphInfo currentParagraph;
} SBParagraphIterator;

typedef struct _SBLogicalRunIterator {
    ObjectBase _base;
    TextIterator parent;
    SBUInteger levelIndex;
    SBLogicalRun currentRun;
} SBLogicalRunIterator;

typedef struct _SBScriptRunIterator {
    ObjectBase _base;
    TextIterator parent;
    SBUInteger scriptIndex;
    SBScriptRun currentRun;
} SBScriptRunIterator;

typedef struct _SBAttributeRunIterator {
    ObjectBase _base;
    SBTextRef text;
    AttributeDictionary items;
    SBUInteger startIndex;
    SBUInteger endIndex;
    SBUInteger currentIndex;
    SBAttributeRun currentRun;
    SBAttributeID filterAttributeID;
    SBAttributeGroup filterGroup;
    SBAttributeScope filterScope;
} SBAttributeRunIterator;

typedef struct _SBVisualRunIterator {
    ObjectBase _base;
    TextIterator parent;
    SBLineRef bidiLine;
    SBUInteger runIndex;
    SBVisualRun currentRun;
} SBVisualRunIterator;

/**
 * Creates and initializes an iterator that can traverse through paragraphs in the given text. Each
 * paragraph represents a sequence of text with consistent bidirectional properties.
 *
 * @param text
 *      The text to iterate through.
 * @return
 *      A new paragraph iterator object, or NULL if creation fails.
 */
SB_INTERNAL SBParagraphIteratorRef SBParagraphIteratorCreate(SBTextRef text);

/**
 * Creates and initializes an iterator that can traverse through runs of text with consistent
 * bidirectional embedding levels. Logical runs represent text segments that have the same
 * bidirectional properties in their logical order.
 *
 * @param text
 *      The text to iterate through.
 * @return
 *      A new logical run iterator object, or NULL if creation fails.
 */
SB_INTERNAL SBLogicalRunIteratorRef SBLogicalRunIteratorCreate(SBTextRef text);

/**
 * Creates and initializes an iterator that can traverse through runs of text with consistent
 * writing scripts. Script runs represent text segments that use the same writing system (e.g.,
 * Latin, Arabic, Devanagari).
 *
 * @param text
 *      The text to iterate through.
 * @return
 *      A new script run iterator object, or NULL if creation fails.
 */
SB_INTERNAL SBScriptRunIteratorRef SBScriptRunIteratorCreate(SBTextRef text);

/**
 * Creates and initializes an iterator that can traverse through runs of text with consistent
 * attribute properties. Attribute runs represent text segments that share common formatting or
 * metadata attributes.
 *
 * @param text
 *      The text to iterate through.
 * @return
 *      A new attribute run iterator object, or NULL if creation fails.
 */
SB_INTERNAL SBAttributeRunIteratorRef SBAttributeRunIteratorCreate(SBTextRef text);

/**
 * Creates and initializes an iterator that can traverse through runs of text in visual order
 * (display order) according to the Unicode Bidirectional Algorithm. Visual runs represent text
 * segments as they should appear on screen.
 *
 * @param text
 *      The text to iterate through.
 * @return
 *      A new visual run iterator object, or NULL if creation fails.
 */
SB_INTERNAL SBVisualRunIteratorRef SBVisualRunIteratorCreate(SBTextRef text);

#endif

#endif
