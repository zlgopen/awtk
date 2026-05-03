/*
 * Copyright (C) 2016-2025 Muhammad Tayyab Akram
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
#include <API/SBCodepoint.h>
#include <Data/BidiTypeLookup.h>

#include "SBCodepointSequence.h"

SB_INTERNAL SBBoolean SBCodepointSequenceIsValid(const SBCodepointSequence *sequence)
{
    if (sequence) {
        SBBoolean encodingValid = SBFalse;

        switch (sequence->stringEncoding) {
        case SBStringEncodingUTF8:
        case SBStringEncodingUTF16:
        case SBStringEncodingUTF32:
            encodingValid = SBTrue;
            break;
        }

        return (encodingValid && sequence->stringBuffer && sequence->stringLength > 0);
    }

    return SBFalse;
}

SB_INTERNAL SBUInteger SBCodepointSequenceGetSeparatorLength(
    const SBCodepointSequence *sequence, SBUInteger separatorIndex)
{
    SBUInteger stringIndex = separatorIndex;
    SBCodepoint codepoint;
    SBUInteger separatorLength;

    codepoint = SBCodepointSequenceGetCodepointAt(sequence, &stringIndex);
    separatorLength = stringIndex - separatorIndex;

    if (codepoint == '\r') {
        /* Don't break in between 'CR' and 'LF'. */
        if (stringIndex < sequence->stringLength) {
            codepoint = SBCodepointSequenceGetCodepointAt(sequence, &stringIndex);

            if (codepoint == '\n') {
                separatorLength = stringIndex - separatorIndex;
            }
        }
    }

    return separatorLength;
}

SB_INTERNAL void SBCodepointSequenceDetermineBidiTypes(
    const SBCodepointSequence *sequence, SBBidiType *bidiTypes)
{
    SBUInteger stringIndex = 0;
    SBUInteger firstIndex = 0;
    SBCodepoint codepoint;

    while ((codepoint = SBCodepointSequenceGetCodepointAt(sequence, &stringIndex)) != SBCodepointInvalid) {
        bidiTypes[firstIndex] = LookupBidiType(codepoint);

        /* Subsequent code units get 'BN' type. */
        while (++firstIndex < stringIndex) {
            bidiTypes[firstIndex] = SBBidiTypeBN;
        }
    }
}

SB_INTERNAL void SBCodepointSequenceGetParagraphBoundary(
    const SBCodepointSequence *sequence, const SBBidiType *bidiTypes,
    SBUInteger paragraphOffset, SBUInteger suggestedLength,
    SBUInteger *actualLength, SBUInteger *separatorLength)
{
    SBUInteger limit = paragraphOffset + suggestedLength;
    SBUInteger index;

    if (separatorLength) {
        *separatorLength = 0;
    }

    for (index = paragraphOffset; index < limit; index++) {
        SBBidiType currentType = bidiTypes[index];

        if (currentType == SBBidiTypeB) {
            SBUInteger codeUnitCount = SBCodepointSequenceGetSeparatorLength(sequence, index);

            index += codeUnitCount;

            if (separatorLength) {
                *separatorLength = codeUnitCount;
            }
            break;
        }
    }

    if (actualLength) {
        *actualLength = index - paragraphOffset;
    }
}

SBCodepoint SBCodepointSequenceGetCodepointBefore(const SBCodepointSequence *codepointSequence, SBUInteger *stringIndex)
{
    SBCodepoint codepoint = SBCodepointInvalid;

    switch (codepointSequence->stringEncoding) {
    case SBStringEncodingUTF8:
        codepoint = SBCodepointDecodePreviousFromUTF8(codepointSequence->stringBuffer, codepointSequence->stringLength, stringIndex);
        break;

    case SBStringEncodingUTF16:
        codepoint = SBCodepointDecodePreviousFromUTF16(codepointSequence->stringBuffer, codepointSequence->stringLength, stringIndex);
        break;

    case SBStringEncodingUTF32:
        if ((*stringIndex - 1) < codepointSequence->stringLength) {
            const SBUInt32 *buffer = codepointSequence->stringBuffer;

            *stringIndex -= 1;
            codepoint = buffer[*stringIndex];

            if (!SBCodepointIsValid(codepoint)) {
                codepoint = SBCodepointFaulty;
            }
        }
        break;
    }

    return codepoint;
}

SBCodepoint SBCodepointSequenceGetCodepointAt(const SBCodepointSequence *codepointSequence, SBUInteger *stringIndex)
{
    SBCodepoint codepoint = SBCodepointInvalid;

    switch (codepointSequence->stringEncoding) {
    case SBStringEncodingUTF8:
        codepoint = SBCodepointDecodeNextFromUTF8(codepointSequence->stringBuffer, codepointSequence->stringLength, stringIndex);
        break;

    case SBStringEncodingUTF16:
        codepoint = SBCodepointDecodeNextFromUTF16(codepointSequence->stringBuffer, codepointSequence->stringLength, stringIndex);
        break;

    case SBStringEncodingUTF32:
        if (*stringIndex < codepointSequence->stringLength) {
            const SBUInt32 *buffer = codepointSequence->stringBuffer;

            codepoint = buffer[*stringIndex];
            *stringIndex += 1;

            if (!SBCodepointIsValid(codepoint)) {
                codepoint = SBCodepointFaulty;
            }
        }
        break;
    }

    return codepoint;
}
