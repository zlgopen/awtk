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
#include <API/SBParagraph.h>
#include <API/SBTextIterators.h>
}

#include "LogicalRunIteratorTests.h"

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

void LogicalRunIteratorTests::run() {
    testInitialization();
    testBasicIteration();
    testLevelBoundaries();
    testMultipleParagraphs();
    testMixedDirectionality();
    testRetainRelease();
    testEdgeCases();
    testComplexScenarios();
}

void LogicalRunIteratorTests::testInitialization() {
    // Test 1: Create iterator with valid text but zero-length
    {
        auto text = SBTextCreateTest(u"");
        auto iterator = SBLogicalRunIteratorCreate(text);
        assert(iterator != nullptr);

        auto run = SBLogicalRunIteratorGetCurrent(iterator);
        assert(run != nullptr);
        assert(run->index == SBInvalidIndex);
        assert(run->length == 0);
        assert(run->level == 0);

        assert(!SBLogicalRunIteratorMoveNext(iterator));

        SBLogicalRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 2: Create iterator with valid text and non-zero length
    {
        auto text = SBTextCreateTest(u"abc");
        auto iterator = SBLogicalRunIteratorCreate(text);
        assert(iterator != nullptr);

        auto run = SBLogicalRunIteratorGetCurrent(iterator);
        assert(run != nullptr);
        assert(run->index == SBInvalidIndex);
        assert(run->length == 0);
        assert(run->level == 0);

        SBLogicalRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void LogicalRunIteratorTests::testBasicIteration() {
    // Test basic LTR text
    {
        auto text = SBTextCreateTest(u"Hello");
        auto iterator = SBLogicalRunIteratorCreate(text);
        auto run = SBLogicalRunIteratorGetCurrent(iterator);

        assert(SBLogicalRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 5);
        assert(run->level == 0);

        assert(!SBLogicalRunIteratorMoveNext(iterator));

        SBLogicalRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test basic RTL text
    {
        auto text = SBTextCreateTest(u"طیب"); // Arabic letters
        auto iterator = SBLogicalRunIteratorCreate(text);
        auto run = SBLogicalRunIteratorGetCurrent(iterator);

        assert(SBLogicalRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 3);
        assert(run->level == 1);

        assert(!SBLogicalRunIteratorMoveNext(iterator));

        SBLogicalRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void LogicalRunIteratorTests::testLevelBoundaries() {
    auto text = SBTextCreateTest(u"ABابCD");
    auto iterator = SBLogicalRunIteratorCreate(text);
    auto run = SBLogicalRunIteratorGetCurrent(iterator);

    // First run (LTR)
    assert(SBLogicalRunIteratorMoveNext(iterator));
    assert(run->index == 0);
    assert(run->length == 2);
    assert(run->level == 0);

    // Second run (RTL)
    assert(SBLogicalRunIteratorMoveNext(iterator));
    assert(run->index == 2);
    assert(run->length == 2);
    assert(run->level == 1);

    // Third run (LTR)
    assert(SBLogicalRunIteratorMoveNext(iterator));
    assert(run->index == 4);
    assert(run->length == 2);
    assert(run->level == 0);

    assert(!SBLogicalRunIteratorMoveNext(iterator));

    SBLogicalRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void LogicalRunIteratorTests::testMultipleParagraphs() {
    auto text = SBTextCreateTest(u"AB\nاب\nCD");
    auto iterator = SBLogicalRunIteratorCreate(text);
    auto run = SBLogicalRunIteratorGetCurrent(iterator);

    // First paragraph
    assert(SBLogicalRunIteratorMoveNext(iterator));
    assert(run->index == 0);
    assert(run->length == 3);
    assert(run->level == 0);

    // Second paragraph
    assert(SBLogicalRunIteratorMoveNext(iterator));
    assert(run->index == 3);
    assert(run->length == 3);
    assert(run->level == 1);

    // Third paragraph
    assert(SBLogicalRunIteratorMoveNext(iterator));
    assert(run->index == 6);
    assert(run->length == 2);
    assert(run->level == 0);

    assert(!SBLogicalRunIteratorMoveNext(iterator));

    SBLogicalRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void LogicalRunIteratorTests::testMixedDirectionality() {
    const char16_t codeUnits[] = {
        u'A',       // LTR
        u'ا',       // RTL
        u'1',       // Neutral
        u'ب',       // RTL
        u'B'        // LTR
    };
    auto string = u16string(codeUnits, 5);
    auto text = SBTextCreateTest(string);
    auto iterator = SBLogicalRunIteratorCreate(text);
    auto run = SBLogicalRunIteratorGetCurrent(iterator);

    const vector<SBLogicalRun> result = {
        {0, 1, 0},
        {1, 1, 1},
        {2, 1, 2},
        {3, 1, 1},
        {4, 1, 0},
    };

    // Verify runs are created with correct directionality
    size_t index = 0;
    while (SBLogicalRunIteratorMoveNext(iterator)) {
        assert(index < result.size());
        assert(run->index == result[index].index);
        assert(run->length == result[index].length);
        assert(run->level == result[index].level);

        index += 1;
    }

    assert(index == result.size());

    SBLogicalRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void LogicalRunIteratorTests::testRetainRelease() {
    auto text = SBTextCreateTest(u"ABC");
    auto iterator = SBLogicalRunIteratorCreate(text);

    // Test retain/release
    auto retained = SBLogicalRunIteratorRetain(iterator);
    assert(retained == iterator);

    SBLogicalRunIteratorRelease(retained);
    SBLogicalRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void LogicalRunIteratorTests::testEdgeCases() {
    // Test 1: Single character text
    {
        auto text = SBTextCreateTest(u"A");
        auto iterator = SBLogicalRunIteratorCreate(text);
        auto run = SBLogicalRunIteratorGetCurrent(iterator);

        assert(SBLogicalRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 1);
        assert(run->level == 0);

        assert(!SBLogicalRunIteratorMoveNext(iterator));

        SBLogicalRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 2: Text with only neutral characters
    {
        auto text = SBTextCreateTest(u"123");
        auto iterator = SBLogicalRunIteratorCreate(text);
        auto run = SBLogicalRunIteratorGetCurrent(iterator);

        assert(SBLogicalRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 3);
        assert(run->level == 0);

        assert(!SBLogicalRunIteratorMoveNext(iterator));

        SBLogicalRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 3: Text with only whitespace
    {
        auto text = SBTextCreateTest(u" \t\n");
        auto iterator = SBLogicalRunIteratorCreate(text);
        auto run = SBLogicalRunIteratorGetCurrent(iterator);

        assert(SBLogicalRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 3);
        assert(run->level == 0);

        assert(!SBLogicalRunIteratorMoveNext(iterator));

        SBLogicalRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void LogicalRunIteratorTests::testComplexScenarios() {
    const char16_t codeUnits[] = {
        'A', 'B',           // English (LTR)
        0x05D0, 0x05D1,     // Hebrew (RTL)
        '1', '2',           // Numbers (Neutral)
        0x0627, 0x0628,     // Arabic (RTL)
        'C', 'D'            // English (LTR)
    };
    auto string = u16string(codeUnits, 10);
    auto text = SBTextCreateTest(string);
    auto iterator = SBLogicalRunIteratorCreate(text);
    auto run = SBLogicalRunIteratorGetCurrent(iterator);

    const vector<SBLogicalRun> result = {
        {0, 2, 0},
        {2, 2, 1},
        {4, 2, 2},
        {6, 2, 1},
        {8, 2, 0},
    };

    // Verify the complex scenario is handled correctly
    size_t index = 0;
    while (SBLogicalRunIteratorMoveNext(iterator)) {
        assert(index < result.size());
        assert(run->index == result[index].index);
        assert(run->length == result[index].length);
        assert(run->level == result[index].level);

        index += 1;
    }

    // Verify all text was covered
    assert(index == result.size());

    SBLogicalRunIteratorRelease(iterator);
    SBTextRelease(text);
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    LogicalRunIteratorTests logicalRunIteratorTests;
    logicalRunIteratorTests.run();

    return 0;
}

#endif
