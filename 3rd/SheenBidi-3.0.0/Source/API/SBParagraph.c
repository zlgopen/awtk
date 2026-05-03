/*
 * Copyright (C) 2014-2025 Muhammad Tayyab Akram
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

#include <API/SBAlgorithm.h>
#include <API/SBAllocator.h>
#include <API/SBAssert.h>
#include <API/SBBase.h>
#include <API/SBCodepointSequence.h>
#include <API/SBLine.h>
#include <API/SBLog.h>
#include <Core/Memory.h>
#include <Core/Object.h>
#include <UBA/BidiChain.h>
#include <UBA/IsolatingRun.h>
#include <UBA/LevelRun.h>
#include <UBA/RunQueue.h>
#include <UBA/StatusStack.h>

#include "SBParagraph.h"

typedef SBParagraph *SBMutableParagraphRef;

typedef struct _ParagraphContext {
    BidiChain bidiChain;
    StatusStack statusStack;
    RunQueue runQueue;
    IsolatingRun isolatingRun;
} ParagraphContext, *ParagraphContextRef;

static void PopulateBidiChain(BidiChainRef chain, const SBBidiType *types, SBUInteger length);
static SBBoolean ProcessRun(ParagraphContextRef context, const LevelRun *levelRun, SBBoolean resolveIsolatingRuns);
static void FinalizeParagraph(ObjectRef object);

#define BIDI_LINKS        0
#define BIDI_TYPES        1
#define BIDI_FLAGS        2
#define COUNT             3

static SBBoolean InitializeParagraphContext(ParagraphContextRef context, MemoryRef memory,
    const SBBidiType *types, SBLevel *levels, SBUInteger length)
{
    SBBoolean isInitialized = SBFalse;
    void *pointers[COUNT] = { NULL };
    SBUInteger sizes[COUNT];

    sizes[BIDI_LINKS] = sizeof(BidiLink) * (length + 2);
    sizes[BIDI_TYPES] = sizeof(SBBidiType) * (length + 2);
    sizes[BIDI_FLAGS] = sizeof(BidiFlag) * (length + 2);

    if (MemoryAllocateChunks(memory, MemoryTypeScratch, sizes, COUNT, pointers)) {
        BidiLink *fixedLinks = pointers[BIDI_LINKS];
        SBBidiType *fixedTypes = pointers[BIDI_TYPES];
        BidiFlag *fixedFlags = pointers[BIDI_FLAGS];

        BidiChainInitialize(&context->bidiChain, fixedTypes, levels, fixedFlags, fixedLinks);
        StatusStackInitialize(&context->statusStack, memory);
        RunQueueInitialize(&context->runQueue, memory);
        IsolatingRunInitialize(&context->isolatingRun, memory);

        PopulateBidiChain(&context->bidiChain, types, length);

        isInitialized = SBTrue;
    }

    return isInitialized;
}

#undef BIDI_LINKS
#undef BIDI_TYPES
#undef BIDI_FLAGS
#undef COUNT

#define PARAGRAPH 0
#define LEVELS    1
#define COUNT     2

static SBMutableParagraphRef AllocateParagraph(SBUInteger length)
{
    void *pointers[COUNT] = { NULL };
    SBUInteger sizes[COUNT] = { 0 };
    SBMutableParagraphRef paragraph;

    sizes[PARAGRAPH] = sizeof(SBParagraph);
    sizes[LEVELS]    = sizeof(SBLevel) * (length + 2);

    paragraph = ObjectCreate(sizes, COUNT, pointers, FinalizeParagraph);

    if (paragraph) {
        paragraph->fixedLevels = pointers[LEVELS];
    }

    return paragraph;
}

#undef PARAGRAPH
#undef LEVELS
#undef COUNT

static void FinalizeParagraph(ObjectRef object)
{
    SBParagraphRef paragraph = object;
    SBAlgorithmRef algorithm;

    algorithm = paragraph->_algorithm;

    if (algorithm) {
        SBAlgorithmRelease(algorithm);
    }
}

static SBUInteger DetermineBoundary(const SBCodepointSequence *codepointSequence,
    const SBBidiType *bidiTypes, SBUInteger paragraphOffset, SBUInteger suggestedLength)
{
    SBUInteger suggestedLimit = paragraphOffset + suggestedLength;
    SBUInteger stringIndex;

    for (stringIndex = paragraphOffset; stringIndex < suggestedLimit; stringIndex++) {
        if (bidiTypes[stringIndex] == SBBidiTypeB) {
            stringIndex += SBCodepointSequenceGetSeparatorLength(codepointSequence, stringIndex);
            goto Return;
        }
    }

Return:
    return (stringIndex - paragraphOffset);
}

static void PopulateBidiChain(BidiChainRef chain, const SBBidiType *types, SBUInteger length)
{
    SBBidiType type = SBBidiTypeNil;
    SBUInteger priorIndex = SBInvalidIndex;
    SBUInteger index;

    for (index = 0; index < length; index++) {
        SBBidiType priorType = type;
        type = types[index];

        switch (type) {
        case SBBidiTypeB:
        case SBBidiTypeON:
        case SBBidiTypeLRE:
        case SBBidiTypeRLE:
        case SBBidiTypeLRO:
        case SBBidiTypeRLO:
        case SBBidiTypePDF:
        case SBBidiTypeLRI:
        case SBBidiTypeRLI:
        case SBBidiTypeFSI:
        case SBBidiTypePDI:
            BidiChainAdd(chain, type, index - priorIndex);
            priorIndex = index;

            if (type == SBBidiTypeB) {
                index = length;
                goto AddLast;
            }
            break;

        default:
            if (type != priorType) {
                BidiChainAdd(chain, type, index - priorIndex);
                priorIndex = index;
            }
            break;
        }
    }

AddLast:
    BidiChainAdd(chain, SBBidiTypeNil, index - priorIndex);
}

static BidiLink SkipIsolatingRun(BidiChainRef chain, BidiLink skipLink, BidiLink breakLink)
{
    BidiLink link = skipLink;
    SBUInteger depth = 1;

    while ((link = BidiChainGetNext(chain, link)) != breakLink) {
        SBBidiType type = BidiChainGetType(chain, link);

        switch (type) {
        case SBBidiTypeLRI:
        case SBBidiTypeRLI:
        case SBBidiTypeFSI:
            depth += 1;
            break;

        case SBBidiTypePDI:
            if (--depth == 0) {
                return link;
            }
            break;
        }
    }

    return BidiLinkNone;
}

static SBLevel DetermineBaseLevel(BidiChainRef chain, BidiLink skipLink, BidiLink breakLink, SBLevel defaultLevel, SBBoolean isIsolate)
{
    BidiLink link = skipLink;

    /* Rules P2, P3 */
    while ((link = BidiChainGetNext(chain, link)) != breakLink) {
        SBBidiType type = BidiChainGetType(chain, link);

        switch (type) {
        case SBBidiTypeL:
            return 0;

        case SBBidiTypeAL:
        case SBBidiTypeR:
            return 1;

        case SBBidiTypeLRI:
        case SBBidiTypeRLI:
        case SBBidiTypeFSI:
            link = SkipIsolatingRun(chain, link, breakLink);
            if (link == BidiLinkNone) {
                goto Default;
            }
            break;

        case SBBidiTypePDI:
            if (isIsolate) {
                /*
                 * In case of isolating run, the PDI will be the last code point.
                 * NOTE:
                 *      The inner isolating runs will be skipped by the case above this one.
                 */
                goto Default;
            }
            break;
        }
    }

Default:
    return defaultLevel;
}

static SBLevel DetermineParagraphLevel(BidiChainRef chain, SBLevel baseLevel)
{
    if (baseLevel >= SBLevelMax) {
        return DetermineBaseLevel(chain, chain->roller, chain->roller,
                                  (baseLevel != SBLevelDefaultRTL ? 0 : 1),
                                  SBFalse);
    }

    return baseLevel;
}

static SBBoolean DetermineLevels(ParagraphContextRef context, SBLevel baseLevel)
{
    BidiChainRef chain = &context->bidiChain;
    StatusStackRef stack = &context->statusStack;
    BidiLink roller = chain->roller;
    BidiLink link;

    BidiLink priorLink;
    BidiLink firstLink;
    BidiLink lastLink;

    SBLevel priorLevel;
    SBBidiType sor;
    SBBidiType eor;

    SBUInteger overIsolate;
    SBUInteger overEmbedding;
    SBUInteger validIsolate;

    priorLink = chain->roller;
    firstLink = BidiLinkNone;
    lastLink = BidiLinkNone;

    priorLevel = baseLevel;
    sor = SBBidiTypeNil;

    /* Rule X1 */
    overIsolate = 0;
    overEmbedding = 0;
    validIsolate = 0;

    StatusStackPush(stack, baseLevel, SBBidiTypeON, SBFalse);

    BidiChainForEach(chain, roller, link) {
        SBBoolean forceFinish = SBFalse;
        SBBoolean bnEquivalent = SBFalse;
        SBBidiType type;

        type = BidiChainGetType(chain, link);

#define LeastGreaterOddLevel()                                              \
(                                                                           \
        (StatusStackGetEmbeddingLevel(stack) + 1) | 1                       \
)

#define LeastGreaterEvenLevel()                                             \
(                                                                           \
        (StatusStackGetEmbeddingLevel(stack) + 2) & ~1                      \
)

#define MergeLinkIfNeeded()                                                 \
{                                                                           \
        if (BidiChainMergeNext(chain, priorLink)) {                         \
            continue;                                                       \
        }                                                                   \
}

#define PushEmbedding(l, o)                                                 \
{                                                                           \
        SBLevel newLevel = l;                                               \
                                                                            \
        bnEquivalent = SBTrue;                                              \
                                                                            \
        if (newLevel <= SBLevelMax && !overIsolate && !overEmbedding) {     \
            if (!StatusStackPush(stack, newLevel, o, SBFalse)) {            \
                return SBFalse;                                             \
            }                                                               \
        } else {                                                            \
            if (!overIsolate) {                                             \
                overEmbedding += 1;                                         \
            }                                                               \
        }                                                                   \
}

#define PushIsolate(l, o)                                                   \
{                                                                           \
        SBBidiType priorStatus = StatusStackGetOverrideStatus(stack);       \
        SBLevel newLevel = l;                                               \
                                                                            \
        BidiChainSetLevel(chain, link,                                      \
                          StatusStackGetEmbeddingLevel(stack));             \
                                                                            \
        if (newLevel <= SBLevelMax && !overIsolate && !overEmbedding) {     \
            validIsolate += 1;                                              \
                                                                            \
            if (!StatusStackPush(stack, newLevel, o, SBTrue)) {             \
                return SBFalse;                                             \
            }                                                               \
        } else {                                                            \
            overIsolate += 1;                                               \
        }                                                                   \
                                                                            \
        if (priorStatus != SBBidiTypeON) {                                  \
            BidiChainSetType(chain, link, priorStatus);                     \
            MergeLinkIfNeeded();                                            \
        }                                                                   \
}

        switch (type) {
        /* Rule X2 */
        case SBBidiTypeRLE:
            PushEmbedding(LeastGreaterOddLevel(), SBBidiTypeON);
            break;

        /* Rule X3 */
        case SBBidiTypeLRE:
            PushEmbedding(LeastGreaterEvenLevel(), SBBidiTypeON);
            break;

        /* Rule X4 */
        case SBBidiTypeRLO:
            PushEmbedding(LeastGreaterOddLevel(), SBBidiTypeR);
            break;

        /* Rule X5 */
        case SBBidiTypeLRO:
            PushEmbedding(LeastGreaterEvenLevel(), SBBidiTypeL);
            break;

        /* Rule X5a */
        case SBBidiTypeRLI:
            PushIsolate(LeastGreaterOddLevel(), SBBidiTypeON);
            break;

        /* Rule X5b */
        case SBBidiTypeLRI:
            PushIsolate(LeastGreaterEvenLevel(), SBBidiTypeON);
            break;

        /* Rule X5c */
        case SBBidiTypeFSI:
        {
            SBBoolean isRTL = (DetermineBaseLevel(chain, link, roller, 0, SBTrue) == 1);
            PushIsolate(isRTL ? LeastGreaterOddLevel() : LeastGreaterEvenLevel(), SBBidiTypeON);
            break;
        }

        /* Rule X6 */
        default:
            BidiChainSetLevel(chain, link, StatusStackGetEmbeddingLevel(stack));

            if (StatusStackGetOverrideStatus(stack) != SBBidiTypeON) {
                BidiChainSetType(chain, link, StatusStackGetOverrideStatus(stack));
                MergeLinkIfNeeded();
            }
            break;

        /* Rule X6a */
        case SBBidiTypePDI:
        {
            SBBidiType overrideStatus;

            if (overIsolate != 0) {
                overIsolate -= 1;
            } else if (validIsolate == 0) {
                /* Do nothing */
            } else {
                overEmbedding = 0;

                while (!StatusStackGetIsolateStatus(stack)) {
                    StatusStackPop(stack);
                }
                StatusStackPop(stack);

                validIsolate -= 1;
            }

            BidiChainSetLevel(chain, link, StatusStackGetEmbeddingLevel(stack));
            overrideStatus = StatusStackGetOverrideStatus(stack);

            if (overrideStatus != SBBidiTypeON) {
                BidiChainSetType(chain, link, overrideStatus);
                MergeLinkIfNeeded();
            }
            break;
        }

        /* Rule X7 */
        case SBBidiTypePDF:
            bnEquivalent = SBTrue;

            if (overIsolate != 0) {
                /* Do nothing */
            } else if (overEmbedding != 0) {
                overEmbedding -= 1;
            } else if (!StatusStackGetIsolateStatus(stack) && stack->count >= 2) {
                StatusStackPop(stack);
            }
            break;

        /* Rule X8 */
        case SBBidiTypeB:
            /*
             * These values are reset for clarity, in this implementation B can only occur as the
             * last code in the array.
             */
            StatusStackSetEmpty(stack);
            StatusStackPush(stack, baseLevel, SBBidiTypeON, SBFalse);

            overIsolate = 0;
            overEmbedding = 0;
            validIsolate = 0;

            BidiChainSetLevel(chain, link, baseLevel);
            break;

        case SBBidiTypeBN:
            bnEquivalent = SBTrue;
            break;

        case SBBidiTypeNil:
            forceFinish = SBTrue;
            BidiChainSetLevel(chain, link, baseLevel);
            break;
        }

        /* Rule X9 */
        if (bnEquivalent) {
            /* The type of this link is BN equivalent, so abandon it and continue the loop. */
            BidiChainAbandonNext(chain, priorLink);
            continue;
        }

        if (sor == SBBidiTypeNil) {
            sor = SBLevelAsNormalBidiType(SBNumberGetMax(baseLevel, BidiChainGetLevel(chain, link)));
            firstLink = link;
            priorLevel = BidiChainGetLevel(chain, link);
        } else if (priorLevel != BidiChainGetLevel(chain, link) || forceFinish) {
            LevelRun levelRun;
            SBLevel currentLevel;

            /* Since the level has changed at this link, therefore the run must end at prior link. */
            lastLink = priorLink;

            /* Save the current level i.e. level of the next run. */
            currentLevel = BidiChainGetLevel(chain, link);
            /*
             * Now we have both the prior level and the current level i.e. unchanged levels of both
             * the current run and the next run. So, identify eor of the current run.
             * NOTE:
             *      sor of the run has been already determined at this stage.
             */
            eor = SBLevelAsNormalBidiType(SBNumberGetMax(priorLevel, currentLevel));

            LevelRunInitialize(&levelRun, chain, firstLink, lastLink, sor, eor);

            if (!ProcessRun(context, &levelRun, forceFinish)) {
                return SBFalse;
            }

            /* The sor of next run (if any) should be technically equal to eor of this run. */
            sor = eor;
            /* The next run (if any) will start from this index. */
            firstLink = link;

            priorLevel = currentLevel;
        }

        priorLink = link;
    }

    return SBTrue;
}

static SBBoolean ProcessRun(ParagraphContextRef context, const LevelRun *levelRun, SBBoolean resolveIsolatingRuns)
{
    RunQueueRef queue = &context->runQueue;

    if (!RunQueueEnqueue(queue, levelRun)) {
        return SBFalse;
    }

    if (resolveIsolatingRuns) {
        IsolatingRunRef isolatingRun = &context->isolatingRun;

        /* Rule X10 */
        for (; queue->count > 0; RunQueueDequeue(queue)) {
            const LevelRun *front = RunQueueGetFront(queue);

            if (RunKindIsAttachedTerminating(front->kind)) {
                continue;
            }

            isolatingRun->baseLevelRun = front;

            if (!IsolatingRunResolve(isolatingRun)) {
                return SBFalse;
            }
        }
    }

    return SBTrue;
}

static void SaveLevels(BidiChainRef chain, SBLevel *levels, SBLevel baseLevel)
{
    BidiLink roller = chain->roller;
    BidiLink link;

    SBUInteger index = 0;
    SBLevel level = baseLevel;

    BidiChainForEach(chain, roller, link) {
        SBUInteger offset = BidiChainGetOffset(chain, link);

        for (; index < offset; index++) {
            levels[index] = level;
        }

        level = BidiChainGetLevel(chain, link);
    }
}

static SBBoolean ResolveParagraph(SBMutableParagraphRef paragraph, MemoryRef memory,
    const SBCodepointSequence *codepointSequence, const SBBidiType *refBidiTypes,
    SBUInteger offset, SBUInteger length, SBLevel baseLevel)
{
    const SBBidiType *bidiTypes = &refBidiTypes[offset];
    SBBoolean isSucceeded = SBFalse;
    ParagraphContext context;

    if (InitializeParagraphContext(&context, memory, bidiTypes, paragraph->fixedLevels, length)) {
        SBLevel resolvedLevel = DetermineParagraphLevel(&context.bidiChain, baseLevel);

        SB_LOG_BLOCK_OPENER("Determined Paragraph Level");
        SB_LOG_STATEMENT("Base Level", 1, SB_LOG_LEVEL(resolvedLevel));
        SB_LOG_BLOCK_CLOSER();

        context.isolatingRun.codepointSequence = codepointSequence;
        context.isolatingRun.bidiTypes = bidiTypes;
        context.isolatingRun.bidiChain = &context.bidiChain;
        context.isolatingRun.paragraphOffset = offset;
        context.isolatingRun.paragraphLevel = resolvedLevel;

        if (DetermineLevels(&context, resolvedLevel)) {
            SaveLevels(&context.bidiChain, paragraph->fixedLevels, resolvedLevel);

            SB_LOG_BLOCK_OPENER("Determined Embedding Levels");
            SB_LOG_STATEMENT("Levels", 1, SB_LOG_LEVELS_ARRAY(paragraph->fixedLevels, length));
            SB_LOG_BLOCK_CLOSER();

            paragraph->codepointSequence = *codepointSequence;
            paragraph->refTypes = bidiTypes;
            paragraph->offset = offset;
            paragraph->length = length;
            paragraph->baseLevel = resolvedLevel;

            isSucceeded = SBTrue;
        }
    }

    return isSucceeded;
}

static SBParagraphRef CreateParagraph(SBAlgorithmRef algorithm,
    const SBCodepointSequence *codepointSequence, const SBBidiType *refBidiTypes,
    SBUInteger paragraphOffset, SBUInteger suggestedLength, SBLevel baseLevel)
{
    SBUInteger actualLength;
    SBMutableParagraphRef paragraph;

    if (algorithm) {
        codepointSequence = &algorithm->codepointSequence;
        refBidiTypes = algorithm->fixedTypes;
    }

    SB_LOG_BLOCK_OPENER("Paragraph Input");
    SB_LOG_STATEMENT("Paragraph Offset", 1, SB_LOG_NUMBER(paragraphOffset));
    SB_LOG_STATEMENT("Suggested Length", 1, SB_LOG_NUMBER(suggestedLength));
    SB_LOG_STATEMENT("Base Direction",   1, SB_LOG_BASE_LEVEL(baseLevel));
    SB_LOG_BLOCK_CLOSER();

    actualLength = DetermineBoundary(codepointSequence, refBidiTypes, paragraphOffset, suggestedLength);

    SB_LOG_BLOCK_OPENER("Determined Paragraph Boundary");
    SB_LOG_STATEMENT("Actual Length", 1, SB_LOG_NUMBER(actualLength));
    SB_LOG_BLOCK_CLOSER();

    paragraph = AllocateParagraph(actualLength);

    if (paragraph) {
        SBBoolean isResolved;
        Memory memory;

        MemoryInitialize(&memory);
        isResolved = ResolveParagraph(
            paragraph, &memory, codepointSequence, refBidiTypes,
            paragraphOffset, actualLength, baseLevel
        );

        if (isResolved) {
            paragraph->_algorithm = (algorithm ? SBAlgorithmRetain(algorithm) : NULL);
        } else {
            ObjectRelease(paragraph);
            paragraph = NULL;
        }

        MemoryFinalize(&memory);
        SBAllocatorResetScratch(NULL);
    }

    SB_LOG_BREAKER();

    return paragraph;
}

SB_INTERNAL SBParagraphRef SBParagraphCreateWithAlgorithm(SBAlgorithmRef algorithm,
    SBUInteger paragraphOffset, SBUInteger suggestedLength, SBLevel baseLevel)
{
    SBUInteger stringLength = algorithm->codepointSequence.stringLength;

    /* The specified range MUST be valid */
    SBAssert(SBUIntegerVerifyRange(stringLength, paragraphOffset, suggestedLength) && suggestedLength > 0);

    return CreateParagraph(algorithm, NULL, NULL, paragraphOffset, suggestedLength, baseLevel);
}

SB_INTERNAL SBParagraphRef SBParagraphCreateWithCodepointSequence(
    const SBCodepointSequence *codepointSequence, const SBBidiType *refBidiTypes,
    SBUInteger paragraphOffset, SBUInteger suggestedLength, SBLevel baseLevel)
{
    SBUInteger stringLength = codepointSequence->stringLength;

    /* The specified range MUST be valid */
    SBAssert(SBUIntegerVerifyRange(stringLength, paragraphOffset, suggestedLength) && suggestedLength > 0);

    return CreateParagraph(NULL, codepointSequence, refBidiTypes, paragraphOffset, suggestedLength, baseLevel);
}

SBUInteger SBParagraphGetOffset(SBParagraphRef paragraph)
{
    return paragraph->offset;
}

SBUInteger SBParagraphGetLength(SBParagraphRef paragraph)
{
    return paragraph->length;
}

SBLevel SBParagraphGetBaseLevel(SBParagraphRef paragraph)
{
    return paragraph->baseLevel;
}

const SBLevel *SBParagraphGetLevelsPtr(SBParagraphRef paragraph)
{
    return paragraph->fixedLevels;
}

SBLineRef SBParagraphCreateLine(SBParagraphRef paragraph, SBUInteger lineOffset, SBUInteger lineLength)
{
    SBUInteger paragraphOffset = paragraph->offset;
    SBUInteger paragraphLength = paragraph->length;
    SBUInteger paragraphLimit = paragraphOffset + paragraphLength;
    SBUInteger lineLimit = lineOffset + lineLength;

    if (lineOffset < lineLimit && lineOffset >= paragraphOffset && lineLimit <= paragraphLimit) {
        return SBLineCreate(paragraph, lineOffset, lineLength);
    }

    return NULL;
}

SBParagraphRef SBParagraphRetain(SBParagraphRef paragraph)
{
    return ObjectRetain((ObjectRef)paragraph);
}

void SBParagraphRelease(SBParagraphRef paragraph)
{
    ObjectRelease((ObjectRef)paragraph);
}
