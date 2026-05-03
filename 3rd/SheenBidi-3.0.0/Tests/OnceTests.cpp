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
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <thread>
#include <vector>

extern "C" {
#include <Core/Once.h>
}

#include "OnceTests.h"

using namespace std;
using namespace SheenBidi;

constexpr size_t NumThreads = 16;
constexpr size_t Iterations = 10000;

void OnceTests::run() {
    void testBasicFunctionality();
    void testThreadSafety();
    void testNestedOnce();
    void testMultipleOnceObjects();
    void testNestedOnceCalls();
}

void OnceTests::testBasicFunctionality() {
    Once once = OnceMake();
    size_t counter = 0;

    auto initFunc = [](void *info) {
        auto counter = static_cast<size_t *>(info);
        *counter += 1;
    };

    // First call should execute
    assert(OnceTryExecute(&once, initFunc, &counter) == true);
    assert(counter == 1);

    // Subsequent calls should not execute
    assert(OnceTryExecute(&once, initFunc, &counter) == false);
    assert(counter == 1);
}

void OnceTests::testThreadSafety() {
    Once once = OnceMake();

    atomic<size_t> executionCount{0};
    atomic<size_t> successCount{0};
    vector<thread> threads;

    auto initFunc = [](void *info) {
        auto executionCount = static_cast<atomic<size_t> *>(info);
        *executionCount += 1;
        // Simulate some work
        this_thread::sleep_for(chrono::milliseconds(10));
    };

    auto worker = [&]() {
        for (size_t i = 0; i < Iterations; i++) {
            if (OnceTryExecute(&once, initFunc, &executionCount)) {
                successCount += 1;
            }
        }
    };

    for (size_t i = 0; i < NumThreads; i++) {
        threads.emplace_back(worker);
    }

    for (auto &t : threads) {
        t.join();
    }

    // Only one execution should have occurred
    assert(executionCount == 1);
    assert(successCount == 1);
}

void OnceTests::testMultipleOnceObjects() {
    constexpr size_t NumObjects = 10;

    vector<Once> onceObjects(NumObjects, OnceMake());
    array<atomic<size_t>, NumObjects> counters;
    vector<thread> threads;

    auto initFunc = [](void *info) {
        auto counter = static_cast<atomic<size_t> *>(info);
        *counter += 1;
    };

    auto worker = [&](size_t id) {
        OnceTryExecute(&onceObjects[id], initFunc, &counters[id]);
    };

    for (size_t i = 0; i < NumObjects; i++) {
        threads.emplace_back(worker, i);
    }

    for (auto &t : threads) {
        t.join();
    }

    // Each Once object should have executed exactly once
    for (size_t i = 0; i < NumObjects; i++) {
        assert(counters[i] == 1);
    }
}

void OnceTests::testNestedOnceCalls() {
    struct Data {
        Once outerOnce = OnceMake();
        Once innerOnce = OnceMake();
        size_t outerCounter = 0;
        size_t innerCounter = 0;
    } data;

    static auto innerFunc = [](void *info) {
        auto data = static_cast<Data *>(info);
        data->innerCounter += 1;
    };

    auto outerFunc = [](void *info) {
        auto data = static_cast<Data *>(info);
        data->outerCounter += 1;
        OnceTryExecute(&data->innerOnce, innerFunc, &data);
    };

    // Execute outer once which executes inner once
    OnceTryExecute(&data.outerOnce, outerFunc, &data);
    
    assert(data.outerCounter == 1);
    assert(data.innerCounter == 1);

    // Try executing both again
    OnceTryExecute(&data.outerOnce, outerFunc, &data);
    OnceTryExecute(&data.innerOnce, innerFunc, &data);

    // Counters should not have changed
    assert(data.outerCounter == 1);
    assert(data.innerCounter == 1);
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    OnceTests onceTests;
    onceTests.run();

    return 0;
}

#endif
