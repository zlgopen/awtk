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

#include <stddef.h>

#include <API/SBBase.h>
#include <API/SBCodepointSequence.h>
#include <API/SBLog.h>
#include <API/SBParagraph.h>
#include <Core/Object.h>

#include "SBAlgorithm.h"

typedef SBAlgorithm *SBMutableAlgorithmRef;

#define ALGORITHM  0
#define BIDI_TYPES 1
#define COUNT      2

static SBMutableAlgorithmRef AllocateAlgorithm(SBUInteger stringLength)
{
    void *pointers[COUNT] = { NULL };
    SBUInteger sizes[COUNT] = { 0 };
    SBMutableAlgorithmRef algorithm;

    sizes[ALGORITHM]  = sizeof(SBAlgorithm);
    sizes[BIDI_TYPES] = sizeof(SBBidiType) * stringLength;

    algorithm = ObjectCreate(sizes, COUNT, pointers, NULL);

    if (algorithm) {
        algorithm->fixedTypes = pointers[BIDI_TYPES];
    }

    return algorithm;
}

#undef ALGORITHM
#undef BIDI_TYPES
#undef COUNT

static SBAlgorithmRef CreateAlgorithm(const SBCodepointSequence *codepointSequence)
{
    SBUInteger stringLength = codepointSequence->stringLength;
    SBMutableAlgorithmRef algorithm;

    SB_LOG_BLOCK_OPENER("Algorithm Input");
    SB_LOG_STATEMENT("Codepoints", 1, SB_LOG_CODEPOINT_SEQUENCE(codepointSequence));
    SB_LOG_BLOCK_CLOSER();

    algorithm = AllocateAlgorithm(stringLength);

    if (algorithm) {
        algorithm->codepointSequence = *codepointSequence;

        SBCodepointSequenceDetermineBidiTypes(codepointSequence, algorithm->fixedTypes);

        SB_LOG_BLOCK_OPENER("Determined Types");
        SB_LOG_STATEMENT("Types",  1, SB_LOG_BIDI_TYPES_ARRAY(algorithm->fixedTypes, stringLength));
        SB_LOG_BLOCK_CLOSER();

        SB_LOG_BREAKER();
    }

    return algorithm;
}

SBAlgorithmRef SBAlgorithmCreate(const SBCodepointSequence *codepointSequence)
{
    SBAlgorithmRef algorithm = NULL;

    if (SBCodepointSequenceIsValid(codepointSequence)) {
        algorithm = CreateAlgorithm(codepointSequence);
    }

    return algorithm;
}

const SBBidiType *SBAlgorithmGetBidiTypesPtr(SBAlgorithmRef algorithm)
{
    return algorithm->fixedTypes;
}

void SBAlgorithmGetParagraphBoundary(SBAlgorithmRef algorithm,
    SBUInteger paragraphOffset, SBUInteger suggestedLength,
    SBUInteger *actualLength, SBUInteger *separatorLength)
{
    const SBCodepointSequence *codepointSequence = &algorithm->codepointSequence;
    SBBidiType *bidiTypes = algorithm->fixedTypes;

    SBUIntegerNormalizeRange(codepointSequence->stringLength, &paragraphOffset, &suggestedLength);

    SBCodepointSequenceGetParagraphBoundary(
        codepointSequence, bidiTypes, paragraphOffset, suggestedLength,
        actualLength, separatorLength
    );
}

SBParagraphRef SBAlgorithmCreateParagraph(SBAlgorithmRef algorithm,
    SBUInteger paragraphOffset, SBUInteger suggestedLength, SBLevel baseLevel)
{
    const SBCodepointSequence *codepointSequence = &algorithm->codepointSequence;
    SBUInteger stringLength = codepointSequence->stringLength;
    SBParagraphRef paragraph = NULL;

    SBUIntegerNormalizeRange(stringLength, &paragraphOffset, &suggestedLength);

    if (suggestedLength > 0) {
        paragraph = SBParagraphCreateWithAlgorithm(
            algorithm, paragraphOffset, suggestedLength, baseLevel
        );
    }

    return paragraph;
}

SBAlgorithmRef SBAlgorithmRetain(SBAlgorithmRef algorithm)
{
    return ObjectRetain((ObjectRef)algorithm);
}

void SBAlgorithmRelease(SBAlgorithmRef algorithm)
{
    ObjectRelease((ObjectRef)algorithm);
}
