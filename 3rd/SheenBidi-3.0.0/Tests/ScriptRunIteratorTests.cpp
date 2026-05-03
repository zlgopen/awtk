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
#include <vector>

#include <SheenBidi/SBScript.h>
#include <SheenBidi/SBText.h>
#include <SheenBidi/SBTextConfig.h>

extern "C" {
#include <API/SBBase.h>
#include <API/SBTextIterators.h>
}

#include "ScriptRunIteratorTests.h"

using namespace std;
using namespace SheenBidi;

static SBTextRef SBTextCreateWithString(const u32string &str) {
    auto config = SBTextConfigCreate();

    auto text = SBTextCreateMutable(SBStringEncodingUTF32, config);
    SBTextAppendCodeUnits(text, str.data(), str.length());

    SBTextConfigRelease(config);

    return text;
}

void ScriptRunIteratorTests::run() {
    testInitialization();
    testBasicIteration();
    testScriptBoundaries();
    testMixedScripts();
    testCommonScripts();
    testInheritedScript();
    testPartialRange();
    testRetainRelease();
    testEdgeCases();
    testComplexScenarios();
}

void ScriptRunIteratorTests::testInitialization() {
    // Test 1: Create iterator with valid text but zero-length
    {
        auto text = SBTextCreateWithString(U"");
        auto iterator = SBScriptRunIteratorCreate(text);
        assert(iterator != nullptr);

        auto run = SBScriptRunIteratorGetCurrent(iterator);
        assert(run != nullptr);
        assert(run->index == SBInvalidIndex);
        assert(run->length == 0);
        assert(run->script == SBScriptNil);

        SBScriptRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 2: Create iterator with valid text and non-zero length
    {
        auto text = SBTextCreateWithString(U"طیب");
        auto iterator = SBScriptRunIteratorCreate(text);
        assert(iterator != nullptr);

        auto run = SBScriptRunIteratorGetCurrent(iterator);
        assert(run != nullptr);
        assert(run->index == SBInvalidIndex);
        assert(run->length == 0);
        assert(run->script == SBScriptNil);

        SBScriptRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void ScriptRunIteratorTests::testBasicIteration() {
    // Test basic Latin script
    {
        auto text = SBTextCreateWithString(U"Hello");
        auto iterator = SBScriptRunIteratorCreate(text);
        auto run = SBScriptRunIteratorGetCurrent(iterator);

        assert(SBScriptRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 5);
        assert(run->script == SBScriptLATN);

        assert(!SBScriptRunIteratorMoveNext(iterator));

        SBScriptRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test basic Arabic script
    {
        auto text = SBTextCreateWithString(U"عدد");
        auto iterator = SBScriptRunIteratorCreate(text);
        auto run = SBScriptRunIteratorGetCurrent(iterator);

        assert(SBScriptRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 3);
        assert(run->script == SBScriptARAB);

        assert(!SBScriptRunIteratorMoveNext(iterator));

        SBScriptRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void ScriptRunIteratorTests::testScriptBoundaries() {
    // Test text with mixed scripts
    {
        const char32_t codeUnits[] = {
            'A', 'B',           // Latin
            0x0627, 0x0628,     // Arabic
            0x0915, 0x0916      // Devanagari
        };
        auto text = SBTextCreateWithString(u32string(codeUnits, 6));
        auto iterator = SBScriptRunIteratorCreate(text);
        auto run = SBScriptRunIteratorGetCurrent(iterator);

        // First run (Latin)
        assert(SBScriptRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 2);
        assert(run->script == SBScriptLATN);

        // Second run (Arabic)
        assert(SBScriptRunIteratorMoveNext(iterator));
        assert(run->index == 2);
        assert(run->length == 2);
        assert(run->script == SBScriptARAB);

        // Third run (Devanagari)
        assert(SBScriptRunIteratorMoveNext(iterator));
        assert(run->index == 4);
        assert(run->length == 2);
        assert(run->script == SBScriptDEVA);

        assert(!SBScriptRunIteratorMoveNext(iterator));

        SBScriptRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void ScriptRunIteratorTests::testMixedScripts() {
    const char32_t codeUnits[] = {
        'A',                // Latin
        '1',                // Common (digit)
        0x0627,             // Arabic
        ',',                // Common (punctuation)
        0x0915              // Devanagari
    };
    auto text = SBTextCreateWithString(u32string(codeUnits, 5));
    auto iterator = SBScriptRunIteratorCreate(text);
    auto run = SBScriptRunIteratorGetCurrent(iterator);

    // Verify script runs are created correctly
    int runCount = 0;
    while (SBScriptRunIteratorMoveNext(iterator)) {
        assert((run->index + run->length) <= 5);
        assert(run->script != SBScriptNil);
        runCount++;
    }

    assert(runCount > 0);

    SBScriptRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void ScriptRunIteratorTests::testCommonScripts() {
    const char32_t codeUnits[] = {
        'A',                // Latin
        '1', '2', '3',      // Common (digits)
        0x0627,             // Arabic
        '.', ',', '!',      // Common (punctuation)
        0x0915              // Devanagari
    };
    auto text = SBTextCreateWithString(u32string(codeUnits, 9));
    auto iterator = SBScriptRunIteratorCreate(text);
    auto run = SBScriptRunIteratorGetCurrent(iterator);

    while (SBScriptRunIteratorMoveNext(iterator)) {
        SBScript script = run->script;
        assert(script != SBScriptZYYY); // Common script should be merged with adjacent scripts
    }

    SBScriptRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void ScriptRunIteratorTests::testInheritedScript() {
    const char32_t codeUnits[] = {
        'A',                // Latin
        0x0301,             // Inherited (combining acute accent)
        0x0627,             // Arabic
        0x0302,             // Inherited (combining circumflex accent)
        0x0915              // Devanagari
    };
    auto text = SBTextCreateWithString(u32string(codeUnits, 5));
    auto iterator = SBScriptRunIteratorCreate(text);
    auto run = SBScriptRunIteratorGetCurrent(iterator);

    while (SBScriptRunIteratorMoveNext(iterator)) {
        SBScript script = run->script;
        assert(script != SBScriptZINH); // Inherited script should be merged with base characters
    }

    SBScriptRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void ScriptRunIteratorTests::testPartialRange() {
    const char32_t codeUnits[] = {
        'A', 'B', '\n',     // Latin
        '1', '2', '\n',     // Common (digits)
        0x0627, 0x0628,     // Arabic
        0x0301,  '\n'       // Inherited (combining mark)
    };
    auto text = SBTextCreateWithString(u32string(codeUnits, 10));
    auto iterator = SBScriptRunIteratorCreate(text);

    /* Start within Latin paragraph and end within Common paragraph. */
    {
        SBScriptRunIteratorReset(iterator, 1, 3);
        auto run = SBScriptRunIteratorGetCurrent(iterator);

        const vector<SBScriptRun> result = {
            {1, 2, SBScriptLATN},
            {3, 1, SBScriptZYYY}
        };

        // Verify the complex scenario is handled correctly
        size_t index = 0;
        while (SBScriptRunIteratorMoveNext(iterator)) {
            assert(index < result.size());
            assert(run->index == result[index].index);
            assert(run->length == result[index].length);
            assert(run->script == result[index].script);

            index += 1;
        }

        // Verify all text was covered
        assert(index == result.size());
    }

    /* Start within Common paragraph and cover full Arabic paragraph. */
    {
        SBScriptRunIteratorReset(iterator, 4, 6);
        auto run = SBScriptRunIteratorGetCurrent(iterator);

        const vector<SBScriptRun> result = {
            {4, 2, SBScriptZYYY},
            {6, 4, SBScriptARAB}
        };

        // Verify the complex scenario is handled correctly
        size_t index = 0;
        while (SBScriptRunIteratorMoveNext(iterator)) {
            assert(index < result.size());
            assert(run->index == result[index].index);
            assert(run->length == result[index].length);
            assert(run->script == result[index].script);

            index += 1;
        }

        // Verify all text was covered
        assert(index == result.size());
    }

    SBScriptRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void ScriptRunIteratorTests::testRetainRelease() {
    auto text = SBTextCreateWithString(U"SheenBidi");
    auto iterator = SBScriptRunIteratorCreate(text);

    // Test retain/release
    auto retained = SBScriptRunIteratorRetain(iterator);
    assert(retained == iterator);

    SBScriptRunIteratorRelease(retained);
    SBScriptRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void ScriptRunIteratorTests::testEdgeCases() {
    // Test 1: Single character text
    {
        auto text = SBTextCreateWithString(U"A");
        auto iterator = SBScriptRunIteratorCreate(text);
        auto run = SBScriptRunIteratorGetCurrent(iterator);

        assert(SBScriptRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 1);
        assert(run->script == SBScriptLATN);

        assert(!SBScriptRunIteratorMoveNext(iterator));

        SBScriptRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 2: Text with only Common script characters
    {
        auto text = SBTextCreateWithString(U"123");
        auto iterator = SBScriptRunIteratorCreate(text);
        auto run = SBScriptRunIteratorGetCurrent(iterator);

        assert(SBScriptRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 3);
        assert(run->script == SBScriptZYYY);

        assert(!SBScriptRunIteratorMoveNext(iterator));

        SBScriptRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 3: Text with only Inherited script characters
    {
        const char32_t codeUnits[] = {0x0301, 0x0302, 0x0303}; // Combining marks
        auto text = SBTextCreateWithString(u32string(codeUnits, 3));
        auto iterator = SBScriptRunIteratorCreate(text);
        auto run = SBScriptRunIteratorGetCurrent(iterator);

        assert(SBScriptRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 3);
        assert(run->script == SBScriptZYYY);

        assert(!SBScriptRunIteratorMoveNext(iterator));

        SBScriptRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void ScriptRunIteratorTests::testComplexScenarios() {
    const char32_t codeUnits[] = {
        'A', 'B',           // Latin
        '1', '2',           // Common (digits)
        0x0627, 0x0628,     // Arabic
        ',', ' ',           // Common (punctuation, space)
        0x0915, 0x0916,     // Devanagari
        0x0301,             // Inherited (combining mark)
        'C', 'D'            // Latin
    };
    auto text = SBTextCreateWithString(u32string(codeUnits, 13));
    auto iterator = SBScriptRunIteratorCreate(text);
    auto run = SBScriptRunIteratorGetCurrent(iterator);

    const vector<SBScriptRun> result = {
        {0, 4, SBScriptLATN},
        {4, 4, SBScriptARAB},
        {8, 3, SBScriptDEVA},
        {11, 2, SBScriptLATN}
    };

    // Verify the complex scenario is handled correctly
    size_t index = 0;
    while (SBScriptRunIteratorMoveNext(iterator)) {
        assert(index < result.size());
        assert(run->index == result[index].index);
        assert(run->length == result[index].length);
        assert(run->script == result[index].script);

        index += 1;
    }

    // Verify all text was covered
    assert(index == result.size());

    SBScriptRunIteratorRelease(iterator);
    SBTextRelease(text);
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    ScriptRunIteratorTests scriptRunIteratorTests;
    scriptRunIteratorTests.run();

    return 0;
}

#endif
