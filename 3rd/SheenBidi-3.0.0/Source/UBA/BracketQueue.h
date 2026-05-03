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

#ifndef _SB_INTERNAL_BRACKET_QUEUE_H
#define _SB_INTERNAL_BRACKET_QUEUE_H

#include <SheenBidi/SBCodepoint.h>

#include <API/SBBase.h>
#include <Core/Memory.h>
#include <UBA/BidiChain.h>

#define BracketQueueEmbeddedElementCount    8
#define BracketQueueMaxOpenPairs            63

typedef struct _BracketQueueElement {
    SBCodepoint bracket;
    BidiLink priorStrongLink;
    BidiLink openingLink;
    BidiLink closingLink;
    SBBidiType innerStrongType;
} BracketQueueElement;

typedef struct _BracketQueueList {
    BracketQueueElement *elements;
    SBUInteger capacity;

    struct _BracketQueueList *previous;
    struct _BracketQueueList *next;
} BracketQueueList, *BracketQueueListRef;

typedef struct _BracketQueuePosition {
    BracketQueueListRef list;
    SBUInteger index;
} BracketQueuePosition;

typedef struct _BracketQueue {
    MemoryRef _memory;
    BracketQueueElement _elements[BracketQueueEmbeddedElementCount];
    BracketQueueList _firstList;
    BracketQueueListRef _rearList;
    SBUInteger _rearTop;
    BracketQueuePosition _firstOpenPair;
    BracketQueuePosition _front;
    SBUInteger _actualCount;
    SBUInteger pairCount;
    SBBidiType _direction;
    SBBoolean _isPopulated;
} BracketQueue, *BracketQueueRef;

SB_INTERNAL void BracketQueueInitialize(BracketQueueRef queue, MemoryRef memory);
SB_INTERNAL void BracketQueueReset(BracketQueueRef queue, SBBidiType direction);
SB_INTERNAL void BracketQueueMarkPopulated(BracketQueueRef queue);

SB_INTERNAL SBBoolean BracketQueueEnqueue(BracketQueueRef queue,
    BidiLink priorStrongLink, BidiLink openingLink, SBCodepoint bracket);
SB_INTERNAL void BracketQueueDequeue(BracketQueueRef queue);

SB_INTERNAL SBUInteger BracketQueueGetOpenPairCount(BracketQueueRef queue);
SB_INTERNAL void BracketQueueAssignInnerStrongType(BracketQueueRef queue, SBBidiType strongType);
SB_INTERNAL void BracketQueueClosePair(BracketQueueRef queue,
    BidiLink closingLink, SBCodepoint bracket);

SB_INTERNAL BidiLink BracketQueueGetPriorStrongLink(BracketQueueRef queue);
SB_INTERNAL BidiLink BracketQueueGetOpeningLink(BracketQueueRef queue);
SB_INTERNAL BidiLink BracketQueueGetClosingLink(BracketQueueRef queue);
SB_INTERNAL SBBidiType BracketQueueGetInnerStrongType(BracketQueueRef queue);

#endif
