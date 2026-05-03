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
#include <UBA/LevelRun.h>
#include <UBA/RunKind.h>

#include "RunQueue.h"

#define MakeRunQueuePosition()                      \
    { NULL, SBInvalidIndex }
#define SetRunQueuePosition(rqi, _list, _index)     \
    ((rqi).list = (_list), (rqi).index = (_index))
#define IsInvalidRunQueuePosition(rqi)              \
    ((rqi).list == NULL)

static const RunQueuePosition InvalidRunQueuePosition = MakeRunQueuePosition();

static RunQueueListRef AllocateRunQueueListPool(RunQueueRef queue)
{
    const SBUInteger newCapacity = queue->count;
    const SBUInteger embeddedCount = RunQueueListElementCount;
    const SBUInteger listCount = (newCapacity + embeddedCount - 1) / embeddedCount;
    const SBUInteger blockSize = sizeof(RunQueueList) * listCount;
    RunQueueList *pool;

    pool = MemoryAllocateBlock(queue->_memory, MemoryTypeScratch, blockSize);

    if (pool) {
        RunQueueListRef previous;
        RunQueueListRef current;
        SBUInteger index;

        /* First element. */
        current = &pool[0];
        current->previous = NULL;
        previous = current;

        /* Middle elements. */
        for (index = 1; index < listCount - 1; index++) {
            current = &pool[index];
            current->previous = previous;
            previous->next = current;
            previous = current;
        }

        /* Last element. */
        current->previous = previous;
        current->next = NULL;
    }

    return pool;
}

static void AddRunQueueListToReusablePool(RunQueueRef queue, RunQueueListRef list)
{
    RunQueueListRef head = queue->_listPool;

    if (head) {
        head->previous = list;
    }

    list->previous = NULL;
    list->next = head;

    queue->_listPool = list;
}

static RunQueueListRef GetReusableRunQueueList(RunQueueRef queue)
{
    RunQueueListRef head = queue->_listPool;
    RunQueueListRef list = NULL;

    if (!head) {
        head = AllocateRunQueueListPool(queue);
    }

    if (head) {
        list = head;
        head = head->next;

        list->previous = NULL;
        list->next = NULL;

        if (head) {
            head->previous = NULL;
        }
        queue->_listPool = head;
    }

    return list;
}

static LevelRun *InsertRunQueueElement(RunQueueRef queue)
{
    RunQueueListRef rearList = queue->_rearList;
    LevelRun *element = NULL;

    if ((queue->_rearTop + 1) < RunQueueListElementCount) {
        queue->_rearTop += 1;
        queue->count += 1;

        element = &rearList->elements[queue->_rearTop];
    } else {
        RunQueueListRef previousList = rearList;

        rearList = GetReusableRunQueueList(queue);

        if (rearList) {
            rearList->previous = previousList;
            previousList->next = rearList;

            queue->_rearList = rearList;
            queue->_rearTop = 0;
            queue->count += 1;

            element = &rearList->elements[0];
        }
    }

    return element;
}

static void FindPreviousPartialRun(RunQueueRef queue)
{
    RunQueuePosition partial = queue->_lastPartialRun;

    if (!IsInvalidRunQueuePosition(partial)) {
        RunQueuePosition front = queue->_front;
        RunQueueListRef breakList = front.list->previous;
        RunQueuePosition current;

        SetRunQueuePosition(current, partial.list, partial.index + 1);

        do {
            SBUInteger start = (current.list == front.list ? front.index : 0);

            while (current.index-- > start) {
                const LevelRun *levelRun = &current.list->elements[current.index];

                if (RunKindIsPartialIsolate(levelRun->kind)) {
                    queue->_lastPartialRun = current;
                    return;
                }
            }

            current.list = current.list->previous;
            current.index = RunQueueListElementCount;
        } while (current.list != breakList);

        /* No more partial run. */
        queue->_lastPartialRun = InvalidRunQueuePosition;
    }
}

SB_INTERNAL void RunQueueInitialize(RunQueueRef queue, MemoryRef memory)
{
    queue->_memory = memory;

    /* Initialize first list. */
    queue->_firstList.previous = NULL;
    queue->_firstList.next = NULL;

    /* Initialize rest of the elements. */
    queue->_listPool = NULL;
    queue->_rearList = &queue->_firstList;
    queue->_rearTop = SBInvalidIndex;
    queue->_front = InvalidRunQueuePosition;
    queue->_lastPartialRun = InvalidRunQueuePosition;
    queue->count = 0;
}

SB_INTERNAL SBBoolean RunQueueEnqueue(RunQueueRef queue, const LevelRun *levelRun)
{
    SBBoolean isEnqueued = SBFalse;
    LevelRun *element;

    element = InsertRunQueueElement(queue);

    if (element) {
        /* Copy the level run into the current element. */
        *element = *levelRun;

        /* Complete the latest isolating run with this terminating run. */
        if (!IsInvalidRunQueuePosition(queue->_lastPartialRun)
                && RunKindIsTerminating(element->kind)) {
            LevelRun *incompleteRun = &queue->_lastPartialRun.list->elements[queue->_lastPartialRun.index];
            LevelRunAttach(incompleteRun, element);
            FindPreviousPartialRun(queue);
        }

        /* Save the location of the isolating run. */
        if (RunKindIsIsolate(element->kind)) {
            SetRunQueuePosition(queue->_lastPartialRun, queue->_rearList, queue->_rearTop);
        }

        if (IsInvalidRunQueuePosition(queue->_front)) {
            /* This is the first element in the queue. */
            SetRunQueuePosition(queue->_front, queue->_rearList, queue->_rearTop);
        }

        isEnqueued = SBTrue;
    }

    return isEnqueued;
}

SB_INTERNAL void RunQueueDequeue(RunQueueRef queue)
{
    /* The queue should not be empty. */
    SBAssert(queue->count > 0);

    if ((queue->_front.index + 1) < RunQueueListElementCount) {
        queue->_front.index += 1;
    } else {
        RunQueueListRef frontList = queue->_front.list;

        queue->_front.list = frontList->next;
        queue->_front.index = 0;

        if (queue->_front.list) {
            queue->_front.list->previous = NULL;
        }

        AddRunQueueListToReusablePool(queue, frontList);
    }

    queue->count -= 1;

    if (queue->count == 0) {
        queue->_front = InvalidRunQueuePosition;
    }
}

SB_INTERNAL const LevelRun *RunQueueGetFront(RunQueueRef queue)
{
    return &queue->_front.list->elements[queue->_front.index];
}
