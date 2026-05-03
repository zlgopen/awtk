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
#include <stdlib.h>
#include <string.h>

#include <API/SBAssert.h>
#include <API/SBAttributeRegistry.h>
#include <API/SBCodepoint.h>
#include <API/SBCodepointSequence.h>
#include <API/SBParagraph.h>
#include <API/SBScriptLocator.h>
#include <API/SBTextConfig.h>
#include <API/SBTextIterators.h>
#include <Core/List.h>
#include <Core/Object.h>
#include <Text/AttributeManager.h>

#include "SBText.h"

/* =========================================================================
 * Text Paragraph Implementation
 * ========================================================================= */

 /**
 * Initializes a TextParagraph structure with default values.
 */
static void InitializeTextParagraph(TextParagraphRef paragraph)
{
    paragraph->index = SBInvalidIndex;
    paragraph->length = 0;
    paragraph->needsReanalysis = SBTrue;
    paragraph->bidiParagraph = NULL;

    ListInitialize(&paragraph->scripts, sizeof(SBScriptRun));
}

/**
 * Releases resources associated with a TextParagraph structure.
 */
static void FinalizeTextParagraph(TextParagraphRef paragraph)
{
    SBParagraphRef bidiParagraph = paragraph->bidiParagraph;

    if (bidiParagraph) {
        SBParagraphRelease(bidiParagraph);
    }

    ListFinalize(&paragraph->scripts);
}

/* =========================================================================
 * Text Implementation
 * ========================================================================= */

/**
 * Returns the size in bytes of a single code unit for the given encoding.
 * 
 * @param encoding
 *      The string encoding.
 * @return
 *      Size in bytes of a code unit, or 0 if encoding is invalid.
 */
static SBUInteger GetCodeUnitSize(SBStringEncoding encoding)
{
    switch (encoding) {
    case SBStringEncodingUTF8:
        return sizeof(SBUInt8);

    case SBStringEncodingUTF16:
        return sizeof(SBUInt16);

    case SBStringEncodingUTF32:
        return sizeof(SBUInt32);

    default:
        return 0;
    }
}

/**
 * Returns the maximum number of code units needed to represent a single code point in the given
 * encoding.
 * 
 * @param text
 *      The text object.
 * @return
 *      Maximum code units per code point, or 0 if encoding is invalid.
 */
static SBUInteger GetMaxCodeUnitsPerCodepoint(SBTextRef text)
{
    switch (text->encoding) {
    case SBStringEncodingUTF8:
        return 4;

    case SBStringEncodingUTF16:
        return 2;

    case SBStringEncodingUTF32:
        return 1;

    default:
        return 0;
    }
}

/**
 * Finalizes all paragraphs in the text object by releasing their resources.
 */
static void FinalizeAllParagraphs(SBTextRef text)
{
    SBUInteger paragraphIndex;

    for (paragraphIndex = 0; paragraphIndex < text->paragraphs.count; paragraphIndex++) {
        FinalizeTextParagraph(ListGetRef(&text->paragraphs, paragraphIndex));
    }
}

/**
 * Comparison function for binary search to locate a paragraph containing a specific code unit. Used
 * by bsearch() to find the paragraph that contains a given code unit index.
 */
static int ParagraphIndexComparison(const void *key, const void *element) {
    const SBUInteger *codeUnitIndex = key;
    const TextParagraph *paragraph = element;
    SBUInteger paragraphStart;
    SBUInteger paragraphEnd;

    paragraphStart = paragraph->index;
    paragraphEnd = paragraphStart + paragraph->length;

    if (*codeUnitIndex < paragraphStart) {
        return -1;
    }
    if (*codeUnitIndex >= paragraphEnd) {
        return 1;
    }

    return 0;
}

SB_INTERNAL SBUInteger SBTextGetCodeUnitParagraphIndex(SBTextRef text, SBUInteger codeUnitIndex)
{
    TextParagraph *array = text->paragraphs.items;
    SBUInteger count = text->paragraphs.count;
    void *item = NULL;

    if (array) {
        item = bsearch(&codeUnitIndex, array, count, sizeof(TextParagraph), ParagraphIndexComparison);
    }

    if (item) {
        return (SBUInteger)((TextParagraph *)item - array);
    }

    return SBInvalidIndex;
}

SB_INTERNAL void SBTextGetBoundaryParagraphs(SBTextRef text,
    SBUInteger rangeStart, SBUInteger rangeEnd,
    TextParagraphRef *firstParagraph, TextParagraphRef *lastParagraph)
{
    SBUInteger codeUnitCount = text->codeUnits.count;

    SBAssert(firstParagraph && lastParagraph);

    *firstParagraph = NULL;
    *lastParagraph = NULL;

    /* Find the first paragraph intersecting the range */
    if (rangeStart < codeUnitCount) {
        SBUInteger paragraphIndex;
        SBUInteger paragraphStart;
        SBUInteger paragraphEnd;

        paragraphIndex = SBTextGetCodeUnitParagraphIndex(text, rangeStart);
        *firstParagraph = ListGetRef(&text->paragraphs, paragraphIndex);

        paragraphStart = (*firstParagraph)->index;
        paragraphEnd = paragraphStart + (*firstParagraph)->length;

        /* If the range doesn't extend beyond the first paragraph, they're the same */
        if (paragraphEnd >= rangeEnd) {
            *lastParagraph = *firstParagraph;
            return;
        }
    }

    /* Find the last paragraph if it's different from the first */
    if (rangeEnd <= codeUnitCount) {
        SBUInteger paragraphIndex;

        paragraphIndex = SBTextGetCodeUnitParagraphIndex(text, rangeEnd - 1);
        *lastParagraph = ListGetRef(&text->paragraphs, paragraphIndex);
    }
}

SBTextRef SBTextCreate(const void *string, SBUInteger length, SBStringEncoding encoding,
    SBTextConfigRef config)
{
    SBMutableTextRef text = SBTextCreateMutable(encoding, config);

    if (text) {
        SBTextAppendCodeUnits(text, string, length);
        text->isMutable = SBFalse;
    }

    return text;
}

SBTextRef SBTextCreateCopy(SBTextRef text)
{
    SBMutableTextRef copy = SBTextCreateMutableCopy(text);

    if (copy) {
        copy->isMutable = SBFalse;
    }

    return copy;
}

SBStringEncoding SBTextGetEncoding(SBTextRef text)
{
    return text->encoding;
}

SBAttributeRegistryRef SBTextGetAttributeRegistry(SBTextRef text)
{
    return text->attributeRegistry;
}

SBUInteger SBTextGetLength(SBTextRef text)
{
    return text->codeUnits.count;
}

void SBTextGetCodeUnits(SBTextRef text, SBUInteger index, SBUInteger length, void *buffer)
{
    SBBoolean isRangeValid = SBUIntegerVerifyRange(text->codeUnits.count, index, length);
    SBUInteger byteCount;
    const void *source;

    SBAssert(isRangeValid);

    byteCount = length * text->codeUnits.itemSize;
    source = ListGetPtr(&text->codeUnits, index);

    memcpy(buffer, source, byteCount);
}

void SBTextGetBidiTypes(SBTextRef text, SBUInteger index, SBUInteger length, SBBidiType *buffer)
{
    SBBoolean isRangeValid = SBUIntegerVerifyRange(text->codeUnits.count, index, length);
    const SBBidiType *bidiTypes;
    SBUInteger byteCount;

    SBAssert(isRangeValid);

    bidiTypes = &text->bidiTypes.items[index];
    byteCount = length * sizeof(SBBidiType);

    memcpy(buffer, bidiTypes, byteCount);
}

void SBTextGetScripts(SBTextRef text, SBUInteger index, SBUInteger length, SBScript *buffer)
{
    SBBoolean isRangeValid = SBUIntegerVerifyRange(text->codeUnits.count, index, length);
    SBUInteger rangeStart;
    SBUInteger rangeEnd;
    SBUInteger paragraphIndex;

    SBAssert(isRangeValid && !text->isEditing);

    rangeStart = index;
    rangeEnd = index + length;
    paragraphIndex = SBTextGetCodeUnitParagraphIndex(text, rangeStart);

    while (rangeStart < rangeEnd) {
        const TextParagraph *textParagraph = ListGetRef(&text->paragraphs, paragraphIndex);
        SBUInteger copyStart = textParagraph->index;
        SBUInteger copyEnd = copyStart + textParagraph->length;
        const SBScript *scriptArray;
        SBUInteger scriptCount;
        SBUInteger byteCount;

        /* Clamp copy range to requested range */
        if (copyStart < rangeStart) {
            copyStart = rangeStart;
        }
        if (copyEnd > rangeEnd) {
            copyEnd = rangeEnd;
        }

        scriptArray = ListGetRef(&textParagraph->scripts, copyStart - textParagraph->index);
        scriptCount = copyEnd - copyStart;
        byteCount = scriptCount * sizeof(SBScript);

        memcpy(buffer, scriptArray, byteCount);

        buffer += scriptCount;
        rangeStart = copyEnd;
        paragraphIndex += 1;
    }
}

void SBTextGetResolvedLevels(SBTextRef text, SBUInteger index, SBUInteger length, SBLevel *buffer)
{
    SBBoolean isRangeValid = SBUIntegerVerifyRange(text->codeUnits.count, index, length);
    SBUInteger rangeStart;
    SBUInteger rangeEnd;
    SBUInteger paragraphIndex;

    SBAssert(isRangeValid && !text->isEditing);

    rangeStart = index;
    rangeEnd = index + length;
    paragraphIndex = SBTextGetCodeUnitParagraphIndex(text, index);

    while (rangeStart < rangeEnd) {
        const TextParagraph *textParagraph = ListGetRef(&text->paragraphs, paragraphIndex);
        SBUInteger copyStart = textParagraph->index;
        SBUInteger copyEnd = copyStart + textParagraph->length;
        SBParagraphRef bidiParagraph;
        const SBLevel *levelArray;
        SBUInteger levelCount;
        SBUInteger byteCount;

        /* Clamp copy range to requested range */
        if (copyStart < rangeStart) {
            copyStart = rangeStart;
        }
        if (copyEnd > rangeEnd) {
            copyEnd = rangeEnd;
        }

        bidiParagraph = textParagraph->bidiParagraph;
        levelArray = &bidiParagraph->fixedLevels[copyStart - bidiParagraph->offset];
        levelCount = copyEnd - copyStart;
        byteCount = levelCount * sizeof(SBLevel);

        memcpy(buffer, levelArray, byteCount);

        buffer += levelCount;
        rangeStart = copyEnd;
        paragraphIndex += 1;
    }
}

void SBTextGetCodeUnitParagraphInfo(SBTextRef text, SBUInteger index,
    SBParagraphInfo *paragraphInfo)
{
    SBBoolean isValidIndex = index < text->codeUnits.count;
    SBUInteger paragraphIndex;
    const TextParagraph *textParagraph;
    SBParagraphRef bidiParagraph;

    SBAssert(isValidIndex && !text->isEditing);

    paragraphIndex = SBTextGetCodeUnitParagraphIndex(text, index);
    textParagraph = ListGetRef(&text->paragraphs, paragraphIndex);
    bidiParagraph = textParagraph->bidiParagraph;

    paragraphInfo->index = textParagraph->index;
    paragraphInfo->length = textParagraph->length;
    paragraphInfo->baseLevel = bidiParagraph->baseLevel;
}

SBParagraphIteratorRef SBTextCreateParagraphIterator(SBTextRef text)
{
    return SBParagraphIteratorCreate(text);
}

SBLogicalRunIteratorRef SBTextCreateLogicalRunIterator(SBTextRef text)
{
    return SBLogicalRunIteratorCreate(text);
}

SBScriptRunIteratorRef SBTextCreateScriptRunIterator(SBTextRef text)
{
    return SBScriptRunIteratorCreate(text);
}

SBAttributeRunIteratorRef SBTextCreateAttributeRunIterator(SBTextRef text)
{
    return SBAttributeRunIteratorCreate(text);
}

SBVisualRunIteratorRef SBTextCreateVisualRunIterator(SBTextRef text,
    SBUInteger index, SBUInteger length)
{
    SBVisualRunIteratorRef iterator = SBVisualRunIteratorCreate(text);

    if (iterator) {
        SBVisualRunIteratorReset(iterator, index, length);
    }

    return iterator;
}

SBTextRef SBTextRetain(SBTextRef text)
{
    return ObjectRetain((ObjectRef)text);
}

void SBTextRelease(SBTextRef text)
{
    ObjectRelease((ObjectRef)text);
}

/* =========================================================================
 * Mutable Text Implementation
 * ========================================================================= */

static void DetermineChunkBidiTypes(SBMutableTextRef text, SBUInteger index, SBUInteger length)
{
    SBUInteger codeUnitCount = text->codeUnits.count;

    if (codeUnitCount > 0) {
        SBUInteger startIndex = index;
        SBUInteger endIndex = startIndex + length;
        SBStringEncoding encoding = text->encoding;
        const void *buffer = text->codeUnits.data;
        SBUInteger surround;
        SBCodepointSequence sequence;

        surround = GetMaxCodeUnitsPerCodepoint(text);

        startIndex = (startIndex >= surround ? startIndex - surround : 0);
        endIndex = ((endIndex + surround) <= codeUnitCount ? endIndex + surround : codeUnitCount);
        endIndex -= 1;

        /* Align to code point boundaries */
        SBCodepointSkipToStart(buffer, codeUnitCount, encoding, &startIndex);
        SBCodepointSkipToEnd(buffer, codeUnitCount, encoding, &endIndex);

        sequence.stringEncoding = encoding;
        sequence.stringBuffer = SBCodepointGetBufferOffset(buffer, encoding, startIndex);
        sequence.stringLength = endIndex - startIndex;

        SBCodepointSequenceDetermineBidiTypes(&sequence, &text->bidiTypes.items[startIndex]);
    }
}

static void ReplaceBidiTypes(SBMutableTextRef text,
    SBUInteger rangeStart, SBUInteger oldLength, SBUInteger newLength)
{
    if (newLength > oldLength) {
        ListReserveRange(&text->bidiTypes, rangeStart, newLength - oldLength);
    } else {
        ListRemoveRange(&text->bidiTypes, rangeStart, oldLength - newLength);
    }

    DetermineChunkBidiTypes(text, rangeStart, newLength);
}

static TextParagraphRef InsertEmptyParagraph(SBMutableTextRef text, SBUInteger listIndex)
{
    SBBoolean succeeded;
    TextParagraph paragraph;

    InitializeTextParagraph(&paragraph);
    succeeded = ListInsert(&text->paragraphs, listIndex, &paragraph);

    return (succeeded ? ListGetRef(&text->paragraphs, listIndex) : NULL);
}

static void RemoveParagraphRange(SBMutableTextRef text, SBUInteger index, SBUInteger length)
{
    SBUInteger endIndex = index + length;
    SBUInteger paragraphIndex;

    /* Finalize each paragraph's resources */
    for (paragraphIndex = index; paragraphIndex < endIndex; paragraphIndex++) {
        TextParagraphRef paragraph = ListGetRef(&text->paragraphs, paragraphIndex);
        FinalizeTextParagraph(paragraph);
    }

    ListRemoveRange(&text->paragraphs, index, length);
}

/**
 * Adjusts the start index of all paragraphs from a given position onward by a delta.
 * Used when text is inserted or deleted to shift paragraph boundaries.
 * 
 * @param text
 *      Mutable text object.
 * @param listIndex
 *      Starting list position (inclusive).
 * @param indexDelta
 *      Amount to add to each paragraph's index (can be negative).
 */
static void ShiftParagraphRanges(SBMutableTextRef text, SBUInteger listIndex, SBInteger indexDelta) {
    while (listIndex < text->paragraphs.count) {
        TextParagraphRef paragraph = ListGetRef(&text->paragraphs, listIndex);
        paragraph->index += indexDelta;
        listIndex += 1;
    }
}

static void UpdateParagraphsForTextReplacement(SBMutableTextRef text,
    SBUInteger replaceStart, SBUInteger oldLength, SBUInteger newLength)
{
    SBUInteger oldEnd = replaceStart + oldLength;
    SBUInteger newEnd = replaceStart + newLength;
    SBInteger lengthDelta = (SBInteger)(newLength - oldLength);
    SBUInteger paragraphIndex;
    SBUInteger removalEnd;
    SBCodepointSequence sequence;
    TextParagraphRef paragraph;
    SBUInteger scanIndex;

    /* Find the first affected paragraph */
    paragraphIndex = SBTextGetCodeUnitParagraphIndex(text, replaceStart > 0 ? replaceStart - 1 : 0);
    if (paragraphIndex == SBInvalidIndex) {
        paragraphIndex = text->paragraphs.count;
    }

    /* Determine starting point for scanning */
    if (paragraphIndex < text->paragraphs.count) {
        paragraph = ListGetRef(&text->paragraphs, paragraphIndex);
        scanIndex = paragraph->index;
    } else {
        scanIndex = replaceStart;
    }

    /* Setup for scanning */
    sequence.stringEncoding = text->encoding;
    sequence.stringBuffer = text->codeUnits.data;
    sequence.stringLength = text->codeUnits.count;

    while (scanIndex < sequence.stringLength) {
        SBUInteger separatorLength;
        SBUInteger paraLength;

        SBCodepointSequenceGetParagraphBoundary(&sequence, text->bidiTypes.items,
            scanIndex, sequence.stringLength - scanIndex, &paraLength, &separatorLength);

        /* Get or create paragraph slot */
        if (paragraphIndex < text->paragraphs.count) {
            paragraph = ListGetRef(&text->paragraphs, paragraphIndex);

            /* Check if this slot is within reusable range */
            if (paragraph->index > oldEnd) {
                /* Slot is after affected region, insert new one */
                paragraph = InsertEmptyParagraph(text, paragraphIndex);
            } else {
                SBUInteger paragraphEnd = paragraph->index + paragraph->length;

                /* Check for splitting */
                if (paragraphEnd > oldEnd && separatorLength > 0) {
                    newEnd = paragraphEnd + lengthDelta;
                }
            }
        } else {
            /* Need new slot */
            paragraph = InsertEmptyParagraph(text, paragraphIndex);
        }

        /* Update paragraph */
        paragraph->index = scanIndex;
        paragraph->length = paraLength;
        paragraph->needsReanalysis = SBTrue;

        scanIndex += paraLength;
        paragraphIndex += 1;

        if (scanIndex > replaceStart && scanIndex >= newEnd) {
            break;
        }
    }

    /* Remove any leftover slots that weren't reused */
    removalEnd = paragraphIndex;
    while (removalEnd < text->paragraphs.count) {
        paragraph = ListGetRef(&text->paragraphs, removalEnd);
        if (paragraph->index > oldEnd) {
            break;
        }

        removalEnd += 1;
    }

    RemoveParagraphRange(text, paragraphIndex, removalEnd - paragraphIndex);

    /* Shift paragraphs after the affected region */
    if (lengthDelta != 0 && paragraphIndex < text->paragraphs.count) {
        ShiftParagraphRanges(text, paragraphIndex, lengthDelta);
    }
}

#define UpdateParagraphsForTextInsertion(text, index, length) \
    UpdateParagraphsForTextReplacement(text, index, 0, length)

#define UpdateParagraphsForTextRemoval(text, index, length) \
    UpdateParagraphsForTextReplacement(text, index, length, 0)

static void GenerateBidiParagraph(SBMutableTextRef text, TextParagraphRef paragraph)
{
    SBCodepointSequence codepointSequence;
    const SBBidiType *bidiTypes;

    codepointSequence.stringEncoding = text->encoding;
    codepointSequence.stringBuffer = text->codeUnits.data;
    codepointSequence.stringLength = text->codeUnits.count;

    bidiTypes = text->bidiTypes.items;

    if (paragraph->bidiParagraph) {
        /* Release old bidi paragraph */
        SBParagraphRelease(paragraph->bidiParagraph);
        paragraph->bidiParagraph = NULL;
    }

    paragraph->bidiParagraph = SBParagraphCreateWithCodepointSequence(
        &codepointSequence, bidiTypes, paragraph->index, paragraph->length, text->baseLevel);
}

static void PopulateParagraphScripts(SBMutableTextRef text, TextParagraphRef paragraph)
{
    SBScriptLocatorRef scriptLocator;
    SBCodepointSequence codepointSequence;
    const SBScriptAgent *scriptAgent;

    scriptLocator = text->scriptLocator;

    codepointSequence.stringEncoding = text->encoding;
    codepointSequence.stringBuffer = ListGetPtr(&text->codeUnits, paragraph->index);
    codepointSequence.stringLength = paragraph->length;

    ListRemoveAll(&paragraph->scripts);
    ListReserveRange(&paragraph->scripts, 0, paragraph->length);

    scriptAgent = &scriptLocator->agent;
    SBScriptLocatorLoadCodepoints(scriptLocator, &codepointSequence);

    while (SBScriptLocatorMoveNext(scriptLocator)) {
        SBUInteger runStart = scriptAgent->offset;
        SBUInteger runEnd = runStart + scriptAgent->length;
        SBScript runScript = scriptAgent->script;

        while (runStart < runEnd) {
            ListSetVal(&paragraph->scripts, runStart, runScript);
            runStart += 1;
        }
    }
}

/**
 * Analyzes all paragraphs marked as needing reanalysis.
 * Generates bidirectional properties and script information.
 */
static void AnalyzeDirtyParagraphs(SBMutableTextRef text)
{
    SBUInteger paragraphCount = text->paragraphs.count;
    SBUInteger paragraphIndex;

    for (paragraphIndex = 0; paragraphIndex < paragraphCount; paragraphIndex++) {
        TextParagraphRef paragraph = ListGetRef(&text->paragraphs, paragraphIndex);

        if (paragraph->needsReanalysis) {
            GenerateBidiParagraph(text, paragraph);
            PopulateParagraphScripts(text, paragraph);

            paragraph->needsReanalysis = SBFalse;
        }
    }
}

/**
 * Cleanup callback for mutable text objects; releases all owned resources.
 */
static void FinalizeMutableText(ObjectRef object)
{
    SBMutableTextRef text = object;

    AttributeManagerFinalize(&text->attributeManager);
    FinalizeAllParagraphs(text);

    ListFinalize(&text->codeUnits);
    ListFinalize(&text->bidiTypes);
    ListFinalize(&text->paragraphs);

    if (text->scriptLocator) {
        SBScriptLocatorRelease(text->scriptLocator);
    }
    if (text->attributeRegistry) {
        SBAttributeRegistryRelease(text->attributeRegistry);
    }
}

SB_INTERNAL SBMutableTextRef SBTextCreateMutableWithParameters(SBStringEncoding encoding,
    SBAttributeRegistryRef attributeRegistry, SBLevel baseLevel)
{
    const SBUInteger size = sizeof(SBText);
    void *pointer = NULL;
    SBMutableTextRef text;

    text = ObjectCreate(&size, 1, &pointer, FinalizeMutableText);

    if (text) {
        if (attributeRegistry) {
            attributeRegistry = SBAttributeRegistryRetain(attributeRegistry);
        }

        text->encoding = encoding;
        text->isMutable = SBTrue;
        text->baseLevel = baseLevel;
        text->isEditing = SBFalse;
        text->scriptLocator = SBScriptLocatorCreate();
        text->attributeRegistry = attributeRegistry;

        AttributeManagerInitialize(&text->attributeManager, text, attributeRegistry);
        ListInitialize(&text->codeUnits, GetCodeUnitSize(encoding));
        ListInitialize(&text->bidiTypes, sizeof(SBBidiType));
        ListInitialize(&text->paragraphs, sizeof(TextParagraph));
    }

    return text;
}

SBMutableTextRef SBTextCreateMutable(SBStringEncoding encoding, SBTextConfigRef config)
{
    SBMutableTextRef text = SBTextCreateMutableWithParameters(encoding,
        config->attributeRegistry, config->baseLevel);

    if (text) {
        /* TODO: Apply default attributes */
    }

    return text;
}

SBMutableTextRef SBTextCreateMutableCopy(SBTextRef text)
{
    SBMutableTextRef copy = SBTextCreateMutableWithParameters(text->encoding,
        text->attributeRegistry, text->baseLevel);

    if (copy) {
        SBUInteger byteCount;
        SBUInteger paragraphCount;
        SBUInteger paragraphIndex;

        /* Copy code units */
        ListReserveRange(&copy->codeUnits, 0, text->codeUnits.count);
        byteCount = text->codeUnits.count * text->codeUnits.itemSize;
        memcpy(copy->codeUnits.data, text->codeUnits.data, byteCount);

        /* Copy bidi types */
        ListReserveRange(&copy->bidiTypes, 0, text->bidiTypes.count);
        byteCount = text->bidiTypes.count * sizeof(SBBidiType);
        memcpy(copy->bidiTypes.items, text->bidiTypes.items, byteCount);

        /* Copy paragraphs */
        paragraphCount = text->paragraphs.count;
        ListReserveRange(&copy->paragraphs, 0, paragraphCount);

        for (paragraphIndex = 0; paragraphIndex < paragraphCount; paragraphIndex++) {
            TextParagraphRef source = ListGetRef(&text->paragraphs, paragraphIndex);
            TextParagraphRef destination = ListGetRef(&copy->paragraphs, paragraphIndex);

            destination->index = source->index;
            destination->length = source->length;
            ListInitialize(&destination->scripts, sizeof(SBScript));

            if (source->needsReanalysis) {
                destination->needsReanalysis = SBTrue;
                destination->bidiParagraph = NULL;
            } else {
                SBUInteger scriptCount = source->scripts.count;

                destination->needsReanalysis = SBFalse;
                destination->bidiParagraph = SBParagraphRetain(source->bidiParagraph);

                ListReserveRange(&destination->scripts, 0, scriptCount);
                byteCount = scriptCount * sizeof(SBScript);
                memcpy(destination->scripts.items, source->scripts.items, byteCount);
            }
        }

        AnalyzeDirtyParagraphs(copy);

        /* Copy attributes */
        AttributeManagerCopyAttributes(&copy->attributeManager, &text->attributeManager);
    }

    return copy;
}

void SBTextBeginEditing(SBMutableTextRef text)
{
    SBAssert(text->isMutable);

    text->isEditing = SBTrue;
}

void SBTextEndEditing(SBMutableTextRef text)
{
    SBAssert(text->isMutable);

    AnalyzeDirtyParagraphs(text);
    text->isEditing = SBFalse;
}

void SBTextAppendCodeUnits(SBMutableTextRef text,
    const void *codeUnitBuffer, SBUInteger codeUnitCount)
{
    SBAssert(text->isMutable);

    SBTextInsertCodeUnits(text, text->codeUnits.count, codeUnitBuffer, codeUnitCount);
}

void SBTextInsertCodeUnits(SBMutableTextRef text, SBUInteger index,
    const void *codeUnitBuffer, SBUInteger codeUnitCount)
{
    SBAssert(text->isMutable && index <= text->codeUnits.count);

    if (codeUnitCount > 0) {
        SBUInteger byteCount;
        void *destination;

        /* Reserve space in code units */
        ListReserveRange(&text->codeUnits, index, codeUnitCount);

        byteCount = codeUnitCount * text->codeUnits.itemSize;
        destination = ListGetPtr(&text->codeUnits, index);
        memcpy(destination, codeUnitBuffer, byteCount);

        /* Insert bidi types */
        ReplaceBidiTypes(text, index, 0, codeUnitCount);

        /* Update paragraph structures */
        UpdateParagraphsForTextInsertion(text, index, codeUnitCount);

        /* Reserve attribute manager space */
        AttributeManagerReserveRange(&text->attributeManager, index, codeUnitCount);

        /* Perform immediate analysis if not in batch editing mode */
        if (!text->isEditing) {
            AnalyzeDirtyParagraphs(text);
        }
    }
}

void SBTextDeleteCodeUnits(SBMutableTextRef text, SBUInteger index, SBUInteger length)
{
    SBUInteger rangeEnd = index + length;
    SBBoolean isRangeValid = (rangeEnd <= text->codeUnits.count && index <= rangeEnd);

    SBAssert(text->isMutable && isRangeValid);

    if (length > 0) {
        /* Remove code units */
        ListRemoveRange(&text->codeUnits, index, length);

        /* Remove bidi types */
        ReplaceBidiTypes(text, index, length, 0);

        /* Update paragraph structures */
        UpdateParagraphsForTextRemoval(text, index, length);

        /* Remove from attribute manager */
        AttributeManagerRemoveRange(&text->attributeManager, index, length);

        if (!text->isEditing) {
            /* Perform immediate analysis if not in batch editing mode */
            AnalyzeDirtyParagraphs(text);
        }
    }
}

void SBTextSetCodeUnits(SBMutableTextRef text,
    const void *codeUnitBuffer, SBUInteger codeUnitCount)
{
    SBAssert(text->isMutable);

    SBTextReplaceCodeUnits(text, 0, text->codeUnits.count, codeUnitBuffer, codeUnitCount);
}

void SBTextReplaceCodeUnits(SBMutableTextRef text, SBUInteger index, SBUInteger length,
    const void *codeUnitBuffer, SBUInteger codeUnitCount)
{
    SBUInteger rangeEnd = index + length;
    SBBoolean isRangeValid = (rangeEnd <= text->codeUnits.count && index <= rangeEnd);

    SBAssert(text->isMutable && isRangeValid);

    if (length > 0 || codeUnitCount > 0) {
        if (codeUnitCount > length) {
            ListReserveRange(&text->codeUnits, index, codeUnitCount - length);
        } else {
            ListRemoveRange(&text->codeUnits, index, length - codeUnitCount);
        }

        if (codeUnitCount > 0) {
            SBUInteger byteCount = codeUnitCount * text->codeUnits.itemSize;
            void *destination = ListGetPtr(&text->codeUnits, index);

            memcpy(destination, codeUnitBuffer, byteCount);
        }

        /* Remove bidi types */
        ReplaceBidiTypes(text, index, length, codeUnitCount);

        /* Update paragraph structures */
        UpdateParagraphsForTextReplacement(text, index, length, codeUnitCount);

        AttributeManagerReplaceRange(&text->attributeManager, index, length, codeUnitCount);

        if (!text->isEditing) {
            /* Perform immediate analysis if not in batch editing mode */
            AnalyzeDirtyParagraphs(text);
        }
    }
}

void SBTextSetAttribute(SBMutableTextRef text, SBUInteger index, SBUInteger length,
    SBAttributeID attributeID, const void *attributeValue)
{
    SBUInteger rangeEnd = index + length;
    SBBoolean isRangeValid = (rangeEnd <= text->codeUnits.count && index <= rangeEnd);

    SBAssert(text->isMutable && isRangeValid);

    if (length > 0) {
        AttributeManagerSetAttribute(&text->attributeManager,
            index, length, attributeID, attributeValue);
    }
}

void SBTextRemoveAttribute(SBMutableTextRef text, SBUInteger index, SBUInteger length,
    SBAttributeID attributeID)
{
    SBUInteger rangeEnd = index + length;
    SBBoolean isRangeValid = (rangeEnd <= text->codeUnits.count && index <= rangeEnd);

    SBAssert(text->isMutable && isRangeValid);

    if (length > 0) {
        AttributeManagerRemoveAttribute(&text->attributeManager, index, length, attributeID);
    }
}

#endif
