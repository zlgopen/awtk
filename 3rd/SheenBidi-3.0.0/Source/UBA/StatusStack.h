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

#ifndef _SB_INTERNAL_STATUS_STACK_H
#define _SB_INTERNAL_STATUS_STACK_H

#include <API/SBBase.h>
#include <Core/Memory.h>

#define StatusStackEmbeddedElementCount     16
#define StatusStackMaxCapacity              127

typedef struct _StatusStackElement {
    SBBoolean isolateStatus;
    SBBidiType overrideStatus;
    SBLevel embeddingLevel;
} StatusStackElement, *StatusStackElementRef;

typedef struct _StatusStackList {
    StatusStackElement *elements;
    SBUInteger capacity;

    struct _StatusStackList *previous;
    struct _StatusStackList *next;
} StatusStackList, *StatusStackListRef;

typedef struct _StatusStack {
    MemoryRef _memory;
    StatusStackElement _elements[StatusStackEmbeddedElementCount];
    StatusStackList _firstList;
    StatusStackListRef _peekList;
    SBUInteger _peekTop;
    SBUInteger count;
} StatusStack, *StatusStackRef;

SB_INTERNAL void StatusStackInitialize(StatusStackRef stack, MemoryRef memory);

SB_INTERNAL SBBoolean StatusStackPush(StatusStackRef stack,
    SBLevel embeddingLevel, SBBidiType overrideStatus, SBBoolean isolateStatus);
SB_INTERNAL void StatusStackPop(StatusStackRef stack);
SB_INTERNAL void StatusStackSetEmpty(StatusStackRef stack);

SB_INTERNAL SBLevel StatusStackGetEmbeddingLevel(StatusStackRef stack);
SB_INTERNAL SBBidiType StatusStackGetOverrideStatus(StatusStackRef stack);
SB_INTERNAL SBBoolean StatusStackGetIsolateStatus(StatusStackRef stack);

#endif
