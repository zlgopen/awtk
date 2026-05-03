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
#include <string>

#include <SheenBidi/SBText.h>
#include <SheenBidi/SBTextConfig.h>

extern "C" {
#include <API/SBBase.h>
#include <API/SBTextIterators.h>
}

#include "ParagraphIteratorTests.h"

using namespace std;
using namespace SheenBidi;

static SBTextRef SBTextCreateTest(const string &str, SBLevel baseLevel = 0) {
    auto config = SBTextConfigCreate();
    SBTextConfigSetBaseLevel(config, baseLevel);

    auto text = SBTextCreateMutable(SBStringEncodingUTF8, config);
    SBTextAppendCodeUnits(text, str.c_str(), str.length());

    SBTextConfigRelease(config);

    return text;
}

void ParagraphIteratorTests::run() {
    testInitialization();
    testBasicIteration();
    testParagraphInfo();
    testPartialRange();
    testBaseLevelHandling();
    testRetainRelease();
    testEdgeCases();
    testResetBehavior();
}

void ParagraphIteratorTests::testInitialization() {
    auto text = SBTextCreateTest("Hello\nWorld");
    auto iterator = SBParagraphIteratorCreate(text);
    
    assert(iterator != nullptr);
    assert(SBParagraphIteratorGetText(iterator) == text);
    
    auto info = SBParagraphIteratorGetCurrent(iterator);
    assert(info != nullptr);
    assert(info->index == SBInvalidIndex);
    assert(info->length == 0);
    assert(info->baseLevel == 0);
    
    SBParagraphIteratorRelease(iterator);
    SBTextRelease(text);
}

void ParagraphIteratorTests::testBasicIteration() {
    auto text = SBTextCreateTest("First\nSecond\nThird");
    auto iterator = SBParagraphIteratorCreate(text);
    auto info = SBParagraphIteratorGetCurrent(iterator);

    // First paragraph
    assert(SBParagraphIteratorMoveNext(iterator));
    assert(info->index == 0);
    assert(info->length == 6); // "First\n"

    // Second paragraph
    assert(SBParagraphIteratorMoveNext(iterator));
    assert(info->index == 6);
    assert(info->length == 7); // "Second\n"

    // Third paragraph
    assert(SBParagraphIteratorMoveNext(iterator));
    assert(info->index == 13);
    assert(info->length == 5); // "Third"

    // No more paragraphs
    assert(!SBParagraphIteratorMoveNext(iterator));

    SBParagraphIteratorRelease(iterator);
    SBTextRelease(text);
}

void ParagraphIteratorTests::testParagraphInfo() {
    auto text = SBTextCreateTest("RTL\nLTR", 1); // RTL base direction
    auto iterator = SBParagraphIteratorCreate(text);

    assert(SBParagraphIteratorMoveNext(iterator));

    auto info = SBParagraphIteratorGetCurrent(iterator);
    assert(info->baseLevel == 1); // Should inherit RTL base direction

    SBParagraphIteratorRelease(iterator);
    SBTextRelease(text);
}

void ParagraphIteratorTests::testPartialRange() {
    auto text = SBTextCreateTest("One\nTwo\nThree\nFour");
    auto iterator = SBParagraphIteratorCreate(text);
    
    // Test middle paragraphs only
    SBParagraphIteratorReset(iterator, 4, 9); // "Two\nThree"
    
    assert(SBParagraphIteratorMoveNext(iterator));
    const SBParagraphInfo* info = SBParagraphIteratorGetCurrent(iterator);
    assert(info->index == 4);
    assert(info->length == 4); // "Two\n"
    
    assert(SBParagraphIteratorMoveNext(iterator));
    info = SBParagraphIteratorGetCurrent(iterator);
    assert(info->index == 8);
    assert(info->length == 5); // "Three"
    
    SBParagraphIteratorRelease(iterator);
    SBTextRelease(text);
}

void ParagraphIteratorTests::testBaseLevelHandling() {
    auto text = SBTextCreateTest("A\nب\nC"); // Mixed LTR/RTL text
    auto iterator = SBParagraphIteratorCreate(text);
    auto info = SBParagraphIteratorGetCurrent(iterator);

    while (SBParagraphIteratorMoveNext(iterator)) {
        assert(info->baseLevel <= 1); // Should be either 0 (LTR) or 1 (RTL)
    }

    SBParagraphIteratorRelease(iterator);
    SBTextRelease(text);
}

void ParagraphIteratorTests::testRetainRelease() {
    auto text = SBTextCreateTest("Test");
    auto iterator = SBParagraphIteratorCreate(text);

    auto retained = SBParagraphIteratorRetain(iterator);
    assert(retained == iterator);

    // Both references should be valid
    assert(SBParagraphIteratorMoveNext(iterator));
    
    SBParagraphIteratorRelease(retained);
    SBParagraphIteratorRelease(iterator);
    SBTextRelease(text);
}

void ParagraphIteratorTests::testEdgeCases() {
    // Empty text
    auto emptyText = SBTextCreateTest("");
    auto iterator = SBParagraphIteratorCreate(emptyText);

    SBParagraphIteratorReset(iterator, 0, 0);
    assert(!SBParagraphIteratorMoveNext(iterator));

    SBParagraphIteratorRelease(iterator);
    SBTextRelease(emptyText);
}

void ParagraphIteratorTests::testResetBehavior() {
    auto text = SBTextCreateTest("A\nB\nC");
    auto iterator = SBParagraphIteratorCreate(text);
    
    // Initial range
    SBParagraphIteratorReset(iterator, 0, 3);
    assert(SBParagraphIteratorMoveNext(iterator));
    
    // Reset to different range
    SBParagraphIteratorReset(iterator, 2, 2);
    assert(SBParagraphIteratorMoveNext(iterator));
    const SBParagraphInfo* info = SBParagraphIteratorGetCurrent(iterator);
    assert(info->index == 2);
    
    // Reset with invalid range
    SBParagraphIteratorReset(iterator, 100, 1);
    assert(!SBParagraphIteratorMoveNext(iterator));
    
    SBParagraphIteratorRelease(iterator);
    SBTextRelease(text);
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    ParagraphIteratorTests paragraphIteratorTests;
    paragraphIteratorTests.run();

    return 0;
}

#endif
