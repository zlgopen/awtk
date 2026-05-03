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

#ifndef _SB_INTERNAL_RUN_QUEUE_H
#define _SB_INTERNAL_RUN_QUEUE_H

#include <API/SBBase.h>
#include <Core/Memory.h>
#include <UBA/LevelRun.h>

#define RunQueueListElementCount    8

typedef struct _RunQueueList {
    LevelRun elements[RunQueueListElementCount];

    struct _RunQueueList *previous;     /**< Reference to the previous list of queue elements */
    struct _RunQueueList *next;         /**< Reference to the next list of queue elements */
} RunQueueList, *RunQueueListRef;

typedef struct _RunQueuePosition {
    RunQueueListRef list;               /**< Reference to the list containing the element */
    SBUInteger index;                   /**< Index of the element in the list */
} RunQueuePosition;

typedef struct _RunQueue {
    MemoryRef _memory;
    RunQueueList _firstList;            /**< First list of elements, which is part of the queue */
    RunQueueListRef _listPool;          /**< Pool of reusable lists */
    RunQueueListRef _rearList;          /**< The list containing rear element of the queue */
    SBUInteger _rearTop;                /**< Index of rear element in rear list */
    RunQueuePosition _front;            /**< Position of the front element */
    RunQueuePosition _lastPartialRun;   /**< Position of the last partial run */
    SBUInteger count;                   /**< Number of elements the queue contains */
} RunQueue, *RunQueueRef;

SB_INTERNAL void RunQueueInitialize(RunQueueRef queue, MemoryRef memory);

SB_INTERNAL SBBoolean RunQueueEnqueue(RunQueueRef queue, const LevelRun *levelRun);
SB_INTERNAL void RunQueueDequeue(RunQueueRef queue);

SB_INTERNAL const LevelRun *RunQueueGetFront(RunQueueRef queue);

#endif
