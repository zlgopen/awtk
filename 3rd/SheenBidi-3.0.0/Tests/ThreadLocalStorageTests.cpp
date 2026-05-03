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

#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <mutex>
#include <thread>
#include <vector>

extern "C" {
#include <Core/ThreadLocalStorage.h>
}

#include "ThreadLocalStorageTests.h"

using namespace std;
using namespace SheenBidi;

constexpr size_t NumThreads = 8;

static ThreadLocalStorage TLS;

void ThreadLocalStorageTests::run() {
    testBasicOperations();
    testThreadLocalness();
    testMultipleThreads();
}

void ThreadLocalStorageTests::testBasicOperations() {
    ThreadLocalStorageInitialize(TLS);

    uint32_t number = 0x1234;
    uint32_t *value = &number;

    ThreadLocalStorageSet(TLS, value);
    assert(ThreadLocalStorageGet(TLS) == value);

    ThreadLocalStorageFinalize(TLS);
}

void ThreadLocalStorageTests::testThreadLocalness() {
    ThreadLocalStorageInitialize(TLS);

    uint32_t mainNumber = 0x1234;
    uint32_t *mainValue = &mainNumber;
    ThreadLocalStorageSet(TLS, mainValue);

    thread t([&]() {
        uint32_t threadNumber = 0x5678;
        uint32_t *threadValue = &threadNumber;
        ThreadLocalStorageSet(TLS, threadValue);

        assert(ThreadLocalStorageGet(TLS) == threadValue);
    });

    t.join();

    // Main thread's value should be unaffected
    assert(ThreadLocalStorageGet(TLS) == mainValue);

    ThreadLocalStorageFinalize(TLS);
}

void ThreadLocalStorageTests::testMultipleThreads() {
    ThreadLocalStorageInitialize(TLS);

    vector<bool> results(NumThreads);
    vector<thread> threads;
    mutex m;

    auto worker = [&](size_t id) {
        uint32_t number = id * 11;
        uint32_t *value = &number;
        ThreadLocalStorageSet(TLS, value);

        this_thread::sleep_for(chrono::milliseconds(rand() % 5));
        lock_guard<mutex> lock(m);
        results[id] = (ThreadLocalStorageGet(TLS) == value);
    };

    for (size_t i = 0; i < NumThreads; i++) {
        threads.emplace_back(worker, i);
    }

    for (auto &t : threads) {
        t.join();
    }

    for (size_t i = 0; i < NumThreads; i++) {
        assert(results[i] == true);
    }

    ThreadLocalStorageFinalize(TLS);
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    ThreadLocalStorageTests threadLocalStorageTests;
    threadLocalStorageTests.run();

    return 0;
}

#endif
