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

#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include <SheenBidi/SBBase.h>
#include <SheenBidi/SBConfig.h>

extern "C" {
#include <API/SBAllocator.h>
}

#include "AllocatorTests.h"

using namespace std;
using namespace SheenBidi;

void AllocatorTests::run() {
#ifdef SB_CONFIG_UNITY
    cout << "Most of the allocator test cases will be skipped in unity mode." << endl;
#elif defined(SB_CONFIG_DISABLE_SCRATCH_MEMORY)
    cout << "Scratch memory is disabled due to which related test cases will be skipped." << endl;
#endif

    testBasicBlockAllocation();
    testLargeAllocation();
    testBasicReallocation();
    testReallocationFromNull();
    testBasicScratchAllocation();
    testScratchMemoryReuse();
    testScratchMemoryAlignment();
    testThreadSafeBlockAllocation();
    testThreadLocalScratchMemory();
    testScratchMemoryExhaustion();
    testCustomAllocatorProtocol();
    testDefaultAllocatorChanges();
    testThreadSafeDefaultAllocatorSwitch();
}

void AllocatorTests::testBasicBlockAllocation() {
#if !defined(SB_CONFIG_UNITY)
    void *pointer = SBAllocatorAllocateBlock(nullptr, 100);
    assert(pointer != nullptr);

    SBAllocatorDeallocateBlock(nullptr, pointer);
#endif
}

void AllocatorTests::testLargeAllocation() {
#if !defined(SB_CONFIG_UNITY)
    const size_t blockSize = 1 << 25;  // 32MB
    void *pointer = SBAllocatorAllocateBlock(nullptr, blockSize);

    assert(pointer != nullptr);
    memset(pointer, 0xAA, blockSize);  // Test we can actually use the memory

    SBAllocatorDeallocateBlock(nullptr, pointer);
#endif
}

void AllocatorTests::testBasicReallocation() {
#if !defined(SB_CONFIG_UNITY)
    const size_t initialSize = 100;
    const size_t newSize = 200;

    void *pointer = SBAllocatorAllocateBlock(nullptr, initialSize);
    assert(pointer != nullptr);

    // Fill with pattern
    memset(pointer, 0xBB, initialSize);

    void *newPointer = SBAllocatorReallocateBlock(nullptr, pointer, newSize);
    assert(pointer != nullptr);

    // Verify content was preserved
    for (size_t i = 0; i < initialSize; i++) {
        assert(((uint8_t *)newPointer)[i] == 0xBB);
    }

    SBAllocatorDeallocateBlock(nullptr, newPointer);
#endif
}

void AllocatorTests::testReallocationFromNull() {
#if !defined(SB_CONFIG_UNITY)
    void *pointer = SBAllocatorReallocateBlock(nullptr, nullptr, 100);
    assert(pointer != nullptr);  // Should behave like malloc

    SBAllocatorDeallocateBlock(nullptr, pointer);
#endif
}

void AllocatorTests::testBasicScratchAllocation() {
#if !defined(SB_CONFIG_UNITY) && !defined(SB_CONFIG_DISABLE_SCRATCH_MEMORY)
    void *pointer = SBAllocatorAllocateScratch(nullptr, 100);
    assert(pointer != nullptr);

    // Should be able to use the memory
    memset(pointer, 0xCC, 100);

    SBAllocatorResetScratch(nullptr);
#endif
}

void AllocatorTests::testScratchMemoryReuse() {
#if !defined(SB_CONFIG_UNITY) && !defined(SB_CONFIG_DISABLE_SCRATCH_MEMORY)
    void *pointer1 = SBAllocatorAllocateScratch(nullptr, 100);
    assert(pointer1 != nullptr);

    SBAllocatorResetScratch(nullptr);

    void *pointer2 = SBAllocatorAllocateScratch(nullptr, 100);
    assert(pointer2 != nullptr);
    
    // Should reuse the same memory after reset
    assert(pointer1 == pointer2);

    SBAllocatorResetScratch(nullptr);
#endif
}

void AllocatorTests::testScratchMemoryAlignment() {
#if !defined(SB_CONFIG_UNITY) && !defined(SB_CONFIG_DISABLE_SCRATCH_MEMORY)
    const size_t sizes[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 29, 31, 37, 61 };
    const size_t align = sizeof(uintptr_t);

    vector<void *> allocations;

    for (auto s : sizes) {
        void *pointer = SBAllocatorAllocateScratch(nullptr, s);
        if (!pointer) {
            break;  // No more scratch memory available
        }
        allocations.push_back(pointer);

        // Verify the pointer is aligned
        assert((reinterpret_cast<uintptr_t>(pointer) % align) == 0);
    }

    // Verify all allocations are unique
    for (size_t i = 0; i < allocations.size(); i++) {
        for (size_t j = i + 1; j < allocations.size(); ++j) {
            assert(allocations[i] != allocations[j]);
        }
    }

    SBAllocatorResetScratch(nullptr);
#endif
}

void AllocatorTests::testThreadSafeBlockAllocation() {
#if !defined(SB_CONFIG_UNITY)
    constexpr int NumThreads = 8;
    constexpr int AllocationsPerThread = 1000;

    vector<thread> threads;
    vector<void *> allocations(NumThreads * AllocationsPerThread, nullptr);

    auto worker = [&](size_t id) {
        for (size_t i = 0; i < AllocationsPerThread; i++) {
            size_t index = (id * AllocationsPerThread) + i;
            allocations[index] = SBAllocatorAllocateBlock(nullptr, 100);
            assert(allocations[index] != nullptr);
            memset(allocations[index], id + i, 100);  // Use the memory
        }
    };

    for (size_t i = 0; i < NumThreads; i++) {
        threads.emplace_back(worker, i);
    }

    for (auto &t : threads) {
        t.join();
    }

    // Verify all allocations are unique and valid
    for (size_t i = 0; i < allocations.size(); i++) {
        for (size_t j = i + 1; j < allocations.size(); ++j) {
            assert(allocations[i] != allocations[j]);
        }

        SBAllocatorDeallocateBlock(nullptr, allocations[i]);
    }
#endif
}

void AllocatorTests::testThreadLocalScratchMemory() {
#if !defined(SB_CONFIG_UNITY) && !defined(SB_CONFIG_DISABLE_SCRATCH_MEMORY)
    constexpr int NumThreads = 16;

    vector<thread> threads;
    vector<void *> threadPointers(NumThreads, nullptr);
    atomic<size_t> threadAllocations{0};
    size_t totalAllocations = 0;

    auto worker = [&](size_t id) {
        void *pointer = SBAllocatorAllocateScratch(nullptr, 100);
        threadPointers[id] = pointer;
        threadAllocations += 1;

        if (pointer) {
            // Verify this thread has exclusive access to this memory
            memset(pointer, id, 100);
            for (size_t j = 0; j < 100; j++) {
                assert(((uint8_t *)pointer)[j] == (uint8_t)id);
            }
        }

        // Wait until all threads have allocated their scratch memory
        while (threadAllocations != NumThreads) {
           this_thread::sleep_for(chrono::milliseconds(10));
        }

        SBAllocatorResetScratch(nullptr);
    };

    for (size_t i = 0; i < NumThreads; i++) {
        threads.emplace_back(worker, i);
    }

    for (auto &t : threads) {
        t.join();
    }

    // Verify all threads got different scratch memory areas
    for (size_t i = 0; i < threadPointers.size(); i++) {
        if (!threadPointers[i]) {
            continue;  // Skip if this thread couldn't allocate scratch memory
        }
        totalAllocations += 1;

        for (size_t j = i + 1; j < threadPointers.size(); j++) {
            assert(threadPointers[i] != threadPointers[j]);
        }
    }

    // Ensure total allocations do not exceed the configured scratch pool size
    assert(totalAllocations == min(SB_CONFIG_SCRATCH_POOL_SIZE, NumThreads));
#endif
}

void AllocatorTests::testScratchMemoryExhaustion() {
#if !defined(SB_CONFIG_UNITY) && !defined(SB_CONFIG_DISABLE_SCRATCH_MEMORY)
    vector<void *> allocations;
    while (void *pointer = SBAllocatorAllocateScratch(nullptr, SB_CONFIG_SCRATCH_BUFFER_SIZE / 4)) {
        allocations.push_back(pointer);
    }

    // Should eventually return null when exhausted
    assert(SBAllocatorAllocateScratch(nullptr, 1) == nullptr);

    // Reset should allow new allocations
    SBAllocatorResetScratch(nullptr);
    void *pointer = SBAllocatorAllocateScratch(nullptr, SB_CONFIG_SCRATCH_BUFFER_SIZE / 2);
    assert(pointer != nullptr);

    SBAllocatorResetScratch(nullptr);
#endif
}

void AllocatorTests::testCustomAllocatorProtocol() {
    struct Data {
        uint8_t allocateBuffer[1] = { 0xAA };
        uint8_t reallocateBuffer[1] = { 0xBB };
        uint8_t scratchBuffer[1] = { 0xCC };
        size_t allocateCount = 0;
        size_t reallocateCount = 0;
        size_t deallocateCount = 0;
        size_t allocateScratchCount = 0;
        size_t resetScratchCount = 0;
        size_t finalizeCount = 0;
    } data;

    SBAllocatorProtocol protocol = {
        [](SBUInteger size, void *info) -> void * {
            auto data = static_cast<Data *>(info);
            data->allocateCount += 1;
            return data->allocateBuffer;
        },
        [](void *pointer, SBUInteger newSize, void *info) -> void * {
            auto data = static_cast<Data *>(info);
            data->reallocateCount += 1;
            return data->reallocateBuffer;
        },
        [](void *pointer, void *info) {
            auto data = static_cast<Data *>(info);
            data->deallocateCount += 1;
        },
        [](SBUInteger size, void *info) -> void * {
            auto data = static_cast<Data *>(info);
            data->allocateScratchCount += 1;
            return data->scratchBuffer;
        },
        [](void *info) {
            auto data = static_cast<Data *>(info);
            data->resetScratchCount += 1;
        },
        [](void *info) {
            auto data = static_cast<Data *>(info);
            data->finalizeCount += 1;
        }
    };

    auto customAllocator = SBAllocatorCreate(&protocol, &data);

    void *pointer = SBAllocatorAllocateBlock(customAllocator, 100);
    assert(pointer == data.allocateBuffer);
    assert(data.allocateCount == 1);

    void *newPointer = SBAllocatorReallocateBlock(customAllocator, pointer, 200);
    assert(newPointer == data.reallocateBuffer);
    assert(data.reallocateCount == 1);

    SBAllocatorDeallocateBlock(customAllocator, newPointer);
    assert(data.deallocateCount == 1);

    void *scratchPointer = SBAllocatorAllocateScratch(customAllocator, 100);
    assert(scratchPointer == data.scratchBuffer);
    assert(data.allocateScratchCount == 1);

    SBAllocatorResetScratch(customAllocator);
    assert(data.resetScratchCount == 1);

    SBAllocatorRetain(customAllocator);
    assert(data.finalizeCount == 0);

    SBAllocatorRelease(customAllocator);
    assert(data.finalizeCount == 0);

    SBAllocatorRelease(customAllocator);
    assert(data.finalizeCount == 1);
}

void AllocatorTests::testDefaultAllocatorChanges() {
    assert(SBAllocatorGetDefault() == nullptr);

    struct Data {
        uint8_t allocateBuffer[1] = { 0xAA };
        uint8_t reallocateBuffer[1] = { 0xBB };
        uint8_t scratchBuffer[1] = { 0xCC };
        size_t allocateCount = 0;
        size_t reallocateCount = 0;
        size_t deallocateCount = 0;
        size_t allocateScratchCount = 0;
        size_t resetScratchCount = 0;
    } data;

    SBAllocatorProtocol protocol = {
        [](SBUInteger size, void *info) -> void * {
            auto data = static_cast<Data *>(info);
            data->allocateCount += 1;
            return data->allocateBuffer;
        },
        [](void *pointer, SBUInteger newSize, void *info) -> void * {
            auto data = static_cast<Data *>(info);
            data->reallocateCount += 1;
            return data->reallocateBuffer;
        },
        [](void *pointer, void *info) {
            auto data = static_cast<Data *>(info);
            data->deallocateCount += 1;
        },
        [](SBUInteger size, void *info) -> void * {
            auto data = static_cast<Data *>(info);
            data->allocateScratchCount += 1;
            return data->scratchBuffer;
        },
        [](void *info) {
            auto data = static_cast<Data *>(info);
            data->resetScratchCount += 1;
        },
        nullptr
    };
    SBAllocatorRef customAllocator = SBAllocatorCreate(&protocol, &data);

    SBAllocatorSetDefault(customAllocator);
    assert(SBAllocatorGetDefault() == customAllocator);

    void *pointer = SBAllocatorAllocateBlock(nullptr, 100);
    assert(pointer == data.allocateBuffer);
    assert(data.allocateCount == 1);

    void *newPointer = SBAllocatorReallocateBlock(nullptr, pointer, 200);
    assert(newPointer == data.reallocateBuffer);
    assert(data.reallocateCount == 1);

    SBAllocatorDeallocateBlock(nullptr, newPointer);
    assert(data.deallocateCount == 1);

    void *scratchPointer = SBAllocatorAllocateScratch(nullptr, 100);
    assert(scratchPointer == data.scratchBuffer);
    assert(data.allocateScratchCount == 1);

    SBAllocatorResetScratch(nullptr);
    assert(data.resetScratchCount == 1);

    SBAllocatorSetDefault(nullptr);
    assert(SBAllocatorGetDefault() == nullptr);

    SBAllocatorRelease(customAllocator);
}

void AllocatorTests::testThreadSafeDefaultAllocatorSwitch() {
    constexpr size_t NumThreads = 8;
    constexpr size_t Iterations = 1000;

    vector<thread> threads;
    atomic<size_t> allocationCounts[2] = { {0}, {0} };

    SBAllocatorProtocol protocol = {
        [](SBUInteger size, void *info) -> void * {
            auto counter = static_cast<atomic<size_t> *>(info);
            *counter += 1;
            return malloc(size);
        },
        [](void *pointer, SBUInteger newSize, void *info) -> void * {
            return realloc(pointer, newSize);
        },
        [](void *pointer, void *info) {
            free(pointer);
        },
        nullptr,
        nullptr,
        nullptr
    };
    // Create two custom allocators
    SBAllocatorRef allocators[2] = {
        SBAllocatorCreate(&protocol, &allocationCounts[0]),
        SBAllocatorCreate(&protocol, &allocationCounts[1])
    };

    auto worker = [&]() {
        for (size_t i = 0; i < Iterations; i++) {
            // Randomly switch between allocators
            auto which = rand() % 2;
            SBAllocatorSetDefault(allocators[which]);

            // Do some allocations
            void *pointer = SBAllocatorAllocateBlock(nullptr, 16);
            if (pointer) {
                SBAllocatorDeallocateBlock(nullptr, pointer);
            }
        }
    };

    // Threads will randomly switch the default allocator
    for (size_t i = 0; i < NumThreads; i++) {
        threads.emplace_back(worker);
    }

    for (auto &t : threads) {
        t.join();
    }

    // Verify both allocators were used
    assert(allocationCounts[0] > 0);
    assert(allocationCounts[1] > 0);
    assert((allocationCounts[0] + allocationCounts[1]) == (NumThreads * Iterations));

    // Reset to original
    SBAllocatorSetDefault(nullptr);

    // Release the custom allocators
    SBAllocatorRelease(allocators[0]);
    SBAllocatorRelease(allocators[1]);
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    AllocatorTests allocatorTests;
    allocatorTests.run();

    return 0;
}

#endif
