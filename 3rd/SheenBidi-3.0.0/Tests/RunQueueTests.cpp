/*
 * Copyright (C) 2025 Muhammad Tayyab Akram
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

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include <SheenBidi/SBConfig.h>

extern "C" {
#include <API/SBBase.h>
#include <Core/Memory.h>
#include <UBA/BidiChain.h>
#include <UBA/LevelRun.h>
#include <UBA/RunQueue.h>
}

#include "RunQueueTests.h"

using namespace std;
using namespace SheenBidi;

static void initialize(LevelRun *levelRun, BidiLink link) {
    levelRun->next = levelRun;
    levelRun->firstLink = link;
    levelRun->lastLink = link;
    levelRun->subsequentLink = link;
    levelRun->extrema = 0;
    levelRun->kind = 0;
    levelRun->level = SBLevelInvalid;
}

static bool areEqual(const LevelRun *firstRun, const LevelRun *secondRun) {
    if (firstRun == secondRun) {
        return true;
    }

    return firstRun->next == secondRun->next
        && firstRun->firstLink == secondRun->firstLink
        && firstRun->lastLink == secondRun->lastLink
        && firstRun->subsequentLink == secondRun->subsequentLink
        && firstRun->extrema == secondRun->extrema
        && firstRun->kind == secondRun->kind
        && firstRun->level == secondRun->level;
}

void RunQueueTests::run() {
    testInitialize();
    testBulkInsertion();
}

void RunQueueTests::testInitialize() {
    Memory memory;
    RunQueue queue;

    MemoryInitialize(&memory);
    RunQueueInitialize(&queue, &memory);

    assert(queue._memory == &memory);

    assert(queue._firstList.previous == nullptr);
    assert(queue._firstList.next == nullptr);

    assert(queue._listPool == nullptr);
    assert(queue._rearList == &queue._firstList);
    assert(queue._rearTop == SBInvalidIndex);
    assert(queue._front.list == nullptr);
    assert(queue._front.index == SBInvalidIndex);
    assert(queue._lastPartialRun.list == nullptr);
    assert(queue._lastPartialRun.index == SBInvalidIndex);
    assert(queue.count == 0);

    MemoryFinalize(&memory);
}

void RunQueueTests::testBulkInsertion() {
    Memory memory;
    RunQueue queue;

    MemoryInitialize(&memory);
    RunQueueInitialize(&queue, &memory);

    array<LevelRun, 32> levelRuns{};
    uint32_t runCount = 0;

    for (uint32_t index = 0; index < levelRuns.size(); index++) {
        auto run = &levelRuns[index];
        initialize(run, index);

        RunQueueEnqueue(&queue, run);
        runCount += 1;

        assert(queue.count == runCount);
        assert(areEqual(&queue._rearList->elements[queue._rearTop], run));
    }

    for (auto &run : levelRuns) {
        auto front = RunQueueGetFront(&queue);

        assert(queue.count == runCount);
        assert(front == &queue._front.list->elements[queue._front.index]);
        assert(areEqual(front, &run));

        RunQueueDequeue(&queue);
        runCount -= 1;
    }

    MemoryFinalize(&memory);
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    RunQueueTests runQueueTests;
    runQueueTests.run();

    return 0;
}

#endif
