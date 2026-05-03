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

#include <API/SBAssert.h>
#include <API/SBBase.h>
#include <Core/Memory.h>

#include "StatusStack.h"

#define STACK_LIST      0
#define ELEMENTS        1
#define COUNT           2

static StatusStackElementRef InsertStatusStackElement(StatusStackRef stack)
{
    StatusStackListRef peekList = stack->_peekList;
    StatusStackElementRef element = NULL;

    if ((stack->_peekTop + 1) < peekList->capacity) {
        stack->_peekTop += 1;
        stack->count += 1;

        element = &peekList->elements[stack->_peekTop];
    } else {
        StatusStackListRef previousList = peekList;

        peekList = previousList->next;

        if (!peekList) {
            const SBUInteger capacity = stack->count;
            void *pointers[COUNT] = { NULL };
            SBUInteger sizes[COUNT];

            sizes[STACK_LIST] = sizeof(StatusStackList);
            sizes[ELEMENTS]   = sizeof(StatusStackElement) * capacity;

            if (MemoryAllocateChunks(stack->_memory, MemoryTypeScratch, sizes, COUNT, pointers)) {
                peekList = pointers[STACK_LIST];
                peekList->elements = pointers[ELEMENTS];
                peekList->capacity = capacity;
                peekList->previous = previousList;
                peekList->next = NULL;

                previousList->next = peekList;
            }
        }

        if (peekList) {
            stack->_peekList = peekList;
            stack->_peekTop = 0;
            stack->count += 1;

            element = &peekList->elements[0];
        }
    }

    return element;
}

#undef STACK_LIST
#undef ELEMENTS
#undef COUNT

SB_INTERNAL void StatusStackInitialize(StatusStackRef stack, MemoryRef memory)
{
    stack->_memory = memory;

    stack->_firstList.elements = stack->_elements;
    stack->_firstList.capacity = StatusStackEmbeddedElementCount;
    stack->_firstList.previous = NULL;
    stack->_firstList.next = NULL;

    StatusStackSetEmpty(stack);
}

SB_INTERNAL SBBoolean StatusStackPush(StatusStackRef stack,
    SBLevel embeddingLevel, SBBidiType overrideStatus, SBBoolean isolateStatus)
{
    SBBoolean isPushed = SBFalse;
    StatusStackElementRef element;

    /* The stack can hold upto 127 elements. */
    SBAssert(stack->count <= 127);

    element = InsertStatusStackElement(stack);

    if (element) {
        element->embeddingLevel = embeddingLevel;
        element->overrideStatus = overrideStatus;
        element->isolateStatus = isolateStatus;

        isPushed = SBTrue;
    }

    return isPushed;
}

SB_INTERNAL void StatusStackPop(StatusStackRef stack)
{
    /* The stack should not be empty. */
    SBAssert(stack->count > 0);

    if (stack->_peekTop != 0) {
        stack->_peekTop -= 1;
    } else {
        stack->_peekList = stack->_peekList->previous;
        stack->_peekTop = stack->_peekList->capacity - 1;
    }

    stack->count -= 1;
}

SB_INTERNAL void StatusStackSetEmpty(StatusStackRef stack)
{
    stack->_peekList = &stack->_firstList;
    stack->_peekTop = SBInvalidIndex;
    stack->count = 0;
}

SB_INTERNAL SBLevel StatusStackGetEmbeddingLevel(StatusStackRef stack)
{
    return stack->_peekList->elements[stack->_peekTop].embeddingLevel;
}

SB_INTERNAL SBBidiType StatusStackGetOverrideStatus(StatusStackRef stack)
{
    return stack->_peekList->elements[stack->_peekTop].overrideStatus;
}

SB_INTERNAL SBBoolean StatusStackGetIsolateStatus(StatusStackRef stack)
{
    return stack->_peekList->elements[stack->_peekTop].isolateStatus;
}
