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
#include <cstddef>
#include <string>
#include <vector>

#include <SheenBidi/SBText.h>
#include <SheenBidi/SBTextConfig.h>

extern "C" {
#include <API/SBBase.h>
#include <API/SBTextIterators.h>
}

#include "VisualRunIteratorTests.h"

using namespace std;
using namespace SheenBidi;

static SBTextRef SBTextCreateTest(const u16string &str, SBLevel baseLevel = SBLevelDefaultLTR) {
    auto config = SBTextConfigCreate();
    SBTextConfigSetBaseLevel(config, baseLevel);

    auto text = SBTextCreateMutable(SBStringEncodingUTF16, config);
    SBTextAppendCodeUnits(text, str.data(), str.length());

    SBTextConfigRelease(config);

    return text;
}

void VisualRunIteratorTests::run() {
    testInitialization();
    testBasicIteration();
    testDirectionRuns();
    testMixedDirectionality();
    testVisualization();
    testPartialRange();
    testRetainRelease();
    testEdgeCases();
}

void VisualRunIteratorTests::testInitialization() {
    // Test 1: Create iterator with valid text but zero-length
    {
        auto text = SBTextCreateTest(u"");
        auto iterator = SBVisualRunIteratorCreate(text);
        assert(iterator != nullptr);

        auto run = SBVisualRunIteratorGetCurrent(iterator);
        assert(run != nullptr);
        assert(run->index == SBInvalidIndex);
        assert(run->length == 0);
        assert(run->level == 0);

        assert(!SBVisualRunIteratorMoveNext(iterator));

        SBVisualRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 2: Create iterator with valid text and non-zero length
    {
        auto text = SBTextCreateTest(u"ABC");
        auto iterator = SBVisualRunIteratorCreate(text);
        assert(iterator != nullptr);

        auto run = SBVisualRunIteratorGetCurrent(iterator);
        assert(run != nullptr);
        assert(run->index == SBInvalidIndex);
        assert(run->length == 0);
        assert(run->level == 0);

        SBVisualRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void VisualRunIteratorTests::testBasicIteration() {
    // Test basic LTR text
    {
        auto text = SBTextCreateTest(u"Hello");
        auto iterator = SBVisualRunIteratorCreate(text);
        auto run = SBVisualRunIteratorGetCurrent(iterator);

        assert(SBVisualRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 5);
        assert(run->level == 0);
        
        assert(!SBVisualRunIteratorMoveNext(iterator));

        SBVisualRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test basic RTL text
    {
        auto text = SBTextCreateTest(u"عدل");
        auto iterator = SBVisualRunIteratorCreate(text);
        auto run = SBVisualRunIteratorGetCurrent(iterator);

        assert(SBVisualRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 3);
        assert(run->level == 1);

        assert(!SBVisualRunIteratorMoveNext(iterator));

        SBVisualRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void VisualRunIteratorTests::testDirectionRuns() {
    auto text = SBTextCreateTest(u"ABابCD");
    auto iterator = SBVisualRunIteratorCreate(text);
    auto run = SBVisualRunIteratorGetCurrent(iterator);

    // First run (LTR)
    assert(SBVisualRunIteratorMoveNext(iterator));
    assert(run->index == 0);
    assert(run->length == 2);
    assert(run->level == 0);

    // Second run (RTL)
    assert(SBVisualRunIteratorMoveNext(iterator));
    assert(run->index == 2);
    assert(run->length == 2);
    assert(run->level == 1);

    // Third run (LTR)
    assert(SBVisualRunIteratorMoveNext(iterator));
    assert(run->index == 4);
    assert(run->length == 2);
    assert(run->level == 0);

    assert(!SBVisualRunIteratorMoveNext(iterator));

    SBVisualRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void VisualRunIteratorTests::testMixedDirectionality() {
    const char16_t codeUnits[] = {
        u'A',       // LTR
        u'ا',       // RTL
        u'1',       // Neutral
        u'ب',       // RTL
        u'B'        // LTR
    };
    auto string = u16string(codeUnits, 5);
    auto text = SBTextCreateTest(string);
    auto iterator = SBVisualRunIteratorCreate(text);
    auto run = SBVisualRunIteratorGetCurrent(iterator);

    const vector<SBVisualRun> result = {
        {0, 1, 0},
        {3, 1, 1},
        {2, 1, 2},
        {1, 1, 1},
        {4, 1, 0},
    };

    // Verify visual ordering
    size_t index = 0;
    while (SBVisualRunIteratorMoveNext(iterator)) {
        assert(index < result.size());
        assert(run->index == result[index].index);
        assert(run->length == result[index].length);
        assert(run->level == result[index].level);

        index += 1;
    }

    assert(index == result.size());

    SBVisualRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void VisualRunIteratorTests::testVisualization() {
    const char16_t codeUnits[] = {
        'A', 'B',           // English
        0x05D0, 0x05D1,     // Hebrew
        '1', '2',           // Numbers
        0x0627, 0x0628      // Arabic
    };
    auto string = u16string(codeUnits, 8);
    auto text = SBTextCreateTest(string);
    auto iterator = SBVisualRunIteratorCreate(text);
    auto run = SBVisualRunIteratorGetCurrent(iterator);

    const vector<SBVisualRun> result = {
        {0, 2, 0},
        {6, 2, 1},
        {4, 2, 2},
        {2, 2, 1},
    };

    // Verify the complex scenario is handled correctly
    size_t index = 0;
    while (SBVisualRunIteratorMoveNext(iterator)) {
        assert(index < result.size());
        assert(run->index == result[index].index);
        assert(run->length == result[index].length);
        assert(run->level == result[index].level);

        index += 1;
    }

    // Verify all text was covered
    assert(index == result.size());

    SBVisualRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void VisualRunIteratorTests::testPartialRange() {
    const char16_t codeUnits[] = {
        'A', 'B', '\n',         // English
        0x05D0, 0x05D1, '\n',   // Hebrew
        '1', '2', '\n',         // Numbers
        0x0627, 0x0628, '\n'    // Arabic
    };
    auto string = u16string(codeUnits, 12);
    auto text = SBTextCreateTest(string);
    auto iterator = SBVisualRunIteratorCreate(text);

    /* Start within LTR paragraph and end within Neutral (LTR) paragraph. */
    {
        SBVisualRunIteratorReset(iterator, 1, 6);
        auto run = SBVisualRunIteratorGetCurrent(iterator);

        const vector<SBVisualRun> result = {
            {1, 2, 0},
            {3, 3, 1},
            {6, 1, 0}
        };

        // Verify the complex scenario is handled correctly
        size_t index = 0;
        while (SBVisualRunIteratorMoveNext(iterator)) {
            assert(index < result.size());
            assert(run->index == result[index].index);
            assert(run->length == result[index].length);
            assert(run->level == result[index].level);

            index += 1;
        }

        // Verify all text was covered
        assert(index == result.size());
    }

    /* Start at RTL paragraph and cover full Neutral (LTR) paragraph. */
    {
        SBVisualRunIteratorReset(iterator, 3, 6);
        auto run = SBVisualRunIteratorGetCurrent(iterator);

        const vector<SBVisualRun> result = {
            {6, 3, 0},
            {3, 3, 1}
        };

        // Verify the complex scenario is handled correctly
        size_t index = 0;
        while (SBVisualRunIteratorMoveNext(iterator)) {
            assert(index < result.size());
            assert(run->index == result[index].index);
            assert(run->length == result[index].length);
            assert(run->level == result[index].level);

            index += 1;
        }

        // Verify all text was covered
        assert(index == result.size());
    }

    /* Start within RTL paragraph and end within RTL paragraph. */
    {
        SBVisualRunIteratorReset(iterator, 4, 6);
        auto run = SBVisualRunIteratorGetCurrent(iterator);

        const vector<SBVisualRun> result = {
            {9, 1, 1},
            {6, 3, 0},
            {4, 2, 1}
        };

        // Verify the complex scenario is handled correctly
        size_t index = 0;
        while (SBVisualRunIteratorMoveNext(iterator)) {
            assert(index < result.size());
            assert(run->index == result[index].index);
            assert(run->length == result[index].length);
            assert(run->level == result[index].level);

            index += 1;
        }

        // Verify all text was covered
        assert(index == result.size());
    }

    SBVisualRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void VisualRunIteratorTests::testRetainRelease() {
    auto text = SBTextCreateTest(u"SheenBidi");
    auto iterator = SBVisualRunIteratorCreate(text);

    // Test retain/release
    SBVisualRunIteratorRef retained = SBVisualRunIteratorRetain(iterator);
    assert(retained == iterator);

    SBVisualRunIteratorRelease(retained);
    SBVisualRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void VisualRunIteratorTests::testEdgeCases() {
    // Test 1: Single character text
    {
        auto text = SBTextCreateTest(u"A");
        auto iterator = SBVisualRunIteratorCreate(text);
        auto run = SBVisualRunIteratorGetCurrent(iterator);

        assert(SBVisualRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 1);

        assert(!SBVisualRunIteratorMoveNext(iterator));

        SBVisualRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 2: Text with only neutral characters
    {
        auto text = SBTextCreateTest(u"123");
        auto iterator = SBVisualRunIteratorCreate(text);
        auto run = SBVisualRunIteratorGetCurrent(iterator);

        assert(SBVisualRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 3);
        assert(run->level == 0);

        assert(!SBVisualRunIteratorMoveNext(iterator));

        SBVisualRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    VisualRunIteratorTests visualRunIteratorTests;
    visualRunIteratorTests.run();

    return 0;
}

#endif
