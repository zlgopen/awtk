/*
 * Copyright (C) 2015-2025 Muhammad Tayyab Akram
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
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <SheenBidi/SheenBidi.h>

#include <Parser/BidiCharacterTest.h>
#include <Parser/BidiMirroring.h>
#include <Parser/BidiTest.h>

#include "Utilities/Convert.h"

#include "Configuration.h"
#include "AlgorithmTests.h"

using namespace std;
using namespace SheenBidi;
using namespace SheenBidi::Parser;
using namespace SheenBidi::Utilities;

constexpr uint8_t LEVEL_X = UINT8_MAX;

struct TestInput {
    const SBCodepoint *codePoints;
    SBUInteger count;
    SBLevel baseLevel;
    SBMirrorLocatorRef mirrorLocator;
};

struct TestOutput {
    uint8_t paragraphLevel;
    const vector<uint8_t> &levels;
    const vector<size_t> &order;
    const uint32_t *mirrors;
    size_t foundMirrors;
};

static bool checkLevels(const TestInput &input, const TestOutput &output, SBLineRef line, SBLevel paragraphLevel);
static bool checkVisualOrder(const TestInput &input, const TestOutput &output, SBLineRef line);
static bool checkMirrors(const TestInput &input, const TestOutput &output, SBLineRef line);

static void printTestCase(size_t testNumber, const BidiTest::TestCase &testCase);
static void printTestCase(size_t testNumber, const BidiCharacterTest::TestCase &testCase);

static SBUInteger fillCodePoints(const vector<uint32_t> &source, SBCodepoint *codePoints) {
    for (auto &cp : source) {
        *(codePoints++) = cp;
    }

    return source.size();
}

static SBUInteger fillCodePoints(const vector<string> &bidiTypes, SBCodepoint *codePoints) {
    for (auto &t : bidiTypes) {
        *(codePoints++) = Convert::toCodePoint(t);
    }

    return bidiTypes.size();
}

static size_t fillMirrors(const BidiMirroring &bidiMirroring,
    const SBCodepoint *codePoints, SBUInteger count,
    const vector<uint8_t> &levels, uint32_t *mirrors)
{
    size_t foundMirrors = 0;

    for (size_t i = 0; i < count; i++) {
        auto level = levels.at(i);
        if (level & 1) {
            mirrors[i] = bidiMirroring.mirrorOf(codePoints[i]);
            if (mirrors[i]) {
                foundMirrors += 1;
            }
        } else {
            mirrors[i] = 0;
        }
    }

    return foundMirrors;
}

static bool runBidiTest(const TestInput &input, const TestOutput &output) {
    bool passed = true;

    SBCodepointSequence sequence;
    sequence.stringEncoding = SBStringEncodingUTF32;
    sequence.stringBuffer = input.codePoints;
    sequence.stringLength = input.count;

    auto algorithm = SBAlgorithmCreate(&sequence);
    auto paragraph = SBAlgorithmCreateParagraph(algorithm, 0, input.count, input.baseLevel);
    auto paragraphlevel = SBParagraphGetBaseLevel(paragraph);

    if (paragraphlevel == output.paragraphLevel || output.paragraphLevel == LEVEL_X) {
        auto line = SBParagraphCreateLine(paragraph, 0, input.count);

        passed &= checkLevels(input, output, line, paragraphlevel);
        passed &= checkVisualOrder(input, output, line);
        passed &= checkMirrors(input, output, line);

        SBLineRelease(line);
    } else {
        if (Configuration::DISPLAY_ERROR_DETAILS) {
            cout << "Test failed due to paragraph level mismatch." << endl;
            cout << "  Discovered Paragraph Level: " << (int)paragraphlevel << endl;
            cout << "  Expected Paragraph Level: " << (int)output.paragraphLevel << endl;
        }
        passed = false;
    }

    SBParagraphRelease(paragraph);
    SBAlgorithmRelease(algorithm);

    return passed;
}

static bool checkLevels(const TestInput &input, const TestOutput &output, SBLineRef line, SBLevel paragraphLevel) {
    auto runArray = SBLineGetRunsPtr(line);
    auto runCount = SBLineGetRunCount(line);

    for (size_t i = 0; i < runCount; i++) {
        auto runPtr = &runArray[i];
        auto start = runPtr->offset;
        auto end = start + runPtr->length - 1;
        auto level = runPtr->level;

        if (end >= input.count) {
            if (Configuration::DISPLAY_ERROR_DETAILS) {
                cout << "Test failed due to invalid run indexes." << endl;
                cout << "  Text Length: " << input.count << endl;
                cout << "  Run Start Index: " << start << endl;
                cout << "  Run End Index: " << end << endl;
            }
            return false;
        }

        auto oldLevel = paragraphLevel;
        for (size_t j = start; j--;) {
            if (output.levels.at(j) != LEVEL_X) {
                oldLevel = output.levels.at(j);
                break;
            }
        }

        for (size_t j = start; j <= end; j++) {
            if (output.levels.at(j) == LEVEL_X) {
                if (level != oldLevel) {
                    cout << "Warning: Level X should be equal to previous level." << endl;
                }
                continue;
            }

            if (output.levels.at(j) != level) {
                if (Configuration::DISPLAY_ERROR_DETAILS) {
                    cout << "Test failed due to level mismatch." << endl;
                    cout << "  Text Index: " << j << endl;
                    cout << "  Discovered Level: " << (int)level << endl;
                    cout << "  Expected Level: " << (int)output.levels.at(j) << endl;
                }
                return false;
            }

            oldLevel = output.levels.at(j);
        }
    }

    return true;
}

static bool checkVisualOrder(const TestInput &input, const TestOutput &output, SBLineRef line) {
    bool passed = true;

    auto runArray = SBLineGetRunsPtr(line);
    auto runCount = SBLineGetRunCount(line);

    size_t lgcIndex = 0;    // Logical Index (incremented from zero to char count)
    size_t ordIndex = 0;    // Order Index (not incremented when level x is found)
    size_t dcvIndex = 0;    // Discovered Visual Index
    size_t expIndex = 0;    // Expected Visual Index

    for (size_t i = 0; i < runCount; i++) {
        auto runPtr = &runArray[i];
        auto start = runPtr->offset;
        auto end = start + runPtr->length - 1;
        auto level = runPtr->level;

        if (level & 1) {
            dcvIndex = end;

            for (size_t j = start; j <= end; j++, dcvIndex--) {
                lgcIndex++;

                if (output.levels.at(dcvIndex) == LEVEL_X) {
                    continue;
                }

                expIndex = output.order.at(ordIndex++);
                if (expIndex != dcvIndex) {
                    passed = false;
                    break;
                }
            }
        } else {
            for (dcvIndex = start; dcvIndex <= end; dcvIndex++) {
                lgcIndex++;

                if (output.levels.at(dcvIndex) == LEVEL_X) {
                    continue;
                }

                expIndex = output.order.at(ordIndex++);
                if (expIndex != dcvIndex) {
                    passed = false;
                    break;
                }
            }
        }
    }

    if (!passed) {
        if (Configuration::DISPLAY_ERROR_DETAILS) {
            cout << "Test failed due to invalid visual order." << endl;
            cout << "  Logical Index: " << lgcIndex << endl;
            cout << "  Discovered Visual Index: " << dcvIndex << endl;
            cout << "  Expected Visual Index: " << expIndex << endl;
        }
    }

    return passed;
}

static bool checkMirrors(const TestInput &input, const TestOutput &output, SBLineRef line) {
    SBMirrorLocatorLoadLine(input.mirrorLocator, line, input.codePoints);

    auto agent = SBMirrorLocatorGetAgent(input.mirrorLocator);
    SBMirrorLocatorReset(input.mirrorLocator);

    size_t locatedMirrors = 0;

    while (SBMirrorLocatorMoveNext(input.mirrorLocator)) {
        if (agent->index < input.count) {
            if (agent->mirror == output.mirrors[agent->index]) {
                locatedMirrors += 1;
            } else {
                if (Configuration::DISPLAY_ERROR_DETAILS) {
                    cout << "Test failed due to mirror mismatch." << endl;
                    cout << "  Text Index: " << agent->index << endl;
                    cout << "  Discovered Mirror: "
                         << uppercase << hex << setfill('0')
                         << setw(4) << agent->mirror << endl
                         << nouppercase << dec << setfill('\0');
                    cout << "  Expected Mirror: "
                         << uppercase << hex << setfill('0')
                         << setw(4) << output.mirrors[agent->index] << endl
                         << nouppercase << dec << setfill('\0');
                }

                return false;
            }
        } else {
            if (Configuration::DISPLAY_ERROR_DETAILS) {
                cout << "Test failed due to invalid mirror index." << endl;
                cout << "  Text Length: " << input.count;
                cout << "  Located Index: " << agent->index << endl;
            }

            return false;
        }
    }

    if (locatedMirrors != output.foundMirrors) {
        if (Configuration::DISPLAY_ERROR_DETAILS) {
            cout << "Test failed due to mismatch in mirror count." << endl;
            cout << "  Discovered Mirrors: " << locatedMirrors << endl;
            cout << "  Expected Mirrors: " << output.foundMirrors << endl;
        }

        return false;
    }

    return true;
}

static size_t runBidiTestCases(BidiTest &bidiTest, const BidiMirroring &bidiMirroring) {
    auto mirrorLocator = SBMirrorLocatorCreate();

    SBCodepoint codePoints[256] = { 0 };
    SBCodepoint mirrors[256] = { 0 };

    size_t testNumber = 0;
    size_t failures = 0;

    auto &testCase = bidiTest.testCase();
    bidiTest.reset();

    for (; bidiTest.fetchNext(); testNumber++) {
        bool passed = true;

        printTestCase(testNumber, testCase);

        auto count = fillCodePoints(testCase.types, codePoints);
        auto foundMirrors = fillMirrors(bidiMirroring, codePoints, count,
            testCase.levels, mirrors);

        if (testCase.directions & BidiTest::ParagraphDirection::Auto) {
            passed &= runBidiTest(
                {codePoints, count, SBLevelDefaultLTR, mirrorLocator},
                {LEVEL_X, testCase.levels, testCase.order, mirrors, foundMirrors});
        }
        if (testCase.directions & BidiTest::ParagraphDirection::LTR) {
            passed &= runBidiTest(
                {codePoints, count, 0, mirrorLocator},
                {LEVEL_X, testCase.levels, testCase.order, mirrors, foundMirrors});
        }
        if (testCase.directions & BidiTest::ParagraphDirection::RTL) {
            passed &= runBidiTest(
                {codePoints, count, 1, mirrorLocator},
                {LEVEL_X, testCase.levels, testCase.order, mirrors, foundMirrors});
        }

        if (!passed) {
            failures += 1;
        }
    }

    SBMirrorLocatorRelease(mirrorLocator);

    return failures;
}

static void printTestCase(size_t testNumber, const BidiTest::TestCase &testCase) {
    if (Configuration::DISPLAY_TEST_CASE) {
        cout << "Bidi Test #" << testNumber << ':' << endl;

        cout << "Paragraph Directions: " << endl << '\t';
        if (testCase.directions & BidiTest::ParagraphDirection::Auto) {
            cout << "Auto ";
        }
        if (testCase.directions & BidiTest::ParagraphDirection::LTR) {
            cout << "LTR ";
        }
        if (testCase.directions & BidiTest::ParagraphDirection::RTL) {
            cout << "RTL ";
        }
        cout << endl;

        cout << "Character Types: " << endl << '\t';
        for (auto &type : testCase.types) {
            cout << type << '\t';
        }
        cout << endl;

        cout << "Levels: " << endl << '\t';
        for (auto level : testCase.levels) {
            if (level == LEVEL_X) {
                cout << 'x';
            } else {
                cout << (int)level;
            }
            cout << '\t';
        }
        cout << endl;

        cout << "Reordered Indexes: " << endl << '\t';
        if (testCase.order.empty()) {
            cout << "Empty";
        } else {
            for (auto index : testCase.order) {
                cout << index << '\t';
            }
        }
        cout << endl;
    }
}

static size_t runBidiCharacterTestCases(BidiCharacterTest &bidiCharacterTest, const BidiMirroring &bidiMirroring) {
    auto mirrorLocator = SBMirrorLocatorCreate();

    SBCodepoint codePoints[256] = { 0 };
    SBCodepoint mirrors[256] = { 0 };

    size_t testNumber = 0;
    size_t failures = 0;

    auto &testCase = bidiCharacterTest.testCase();
    bidiCharacterTest.reset();

    for (; bidiCharacterTest.fetchNext(); testNumber++) {
        printTestCase(testNumber, testCase);

        auto count = fillCodePoints(testCase.text, codePoints);
        auto foundMirrors = fillMirrors(bidiMirroring, codePoints, count,
            testCase.levels, mirrors);
        SBLevel inputLevel;

        switch (testCase.paragraphDirection) {
        case BidiCharacterTest::ParagraphDirection::LTR:
            inputLevel = 0;
            break;
        case BidiCharacterTest::ParagraphDirection::RTL:
            inputLevel = 1;
            break;
        default:
            inputLevel = SBLevelDefaultLTR;
            break;
        }

        if (!runBidiTest(
                {codePoints, count, inputLevel, mirrorLocator},
                {testCase.paragraphLevel, testCase.levels, testCase.order,
                    mirrors, foundMirrors})) {
            failures += 1;
        }
    }

    SBMirrorLocatorRelease(mirrorLocator);

    return failures;
}

static void printTestCase(size_t testNumber, const BidiCharacterTest::TestCase &testCase) {
    if (Configuration::DISPLAY_TEST_CASE) {
        cout << "Bidi Character Test #" << testNumber << endl;

        cout << "Paragraph Direction: " << endl << '\t';
        switch (testCase.paragraphDirection) {
        case BidiCharacterTest::ParagraphDirection::LTR:
            cout << "LTR" << endl;
            break;

        case BidiCharacterTest::ParagraphDirection::RTL:
            cout << "RTL" << endl;
            break;

        default:
            cout << "Auto" << endl;
            break;
        }

        cout << "Characters: " << endl << '\t';
        cout << uppercase << hex << setfill('0');
        for (auto codePoint : testCase.text) {
            cout << setw(4) << codePoint << '\t';
        }
        cout << nouppercase << dec << setfill('\0');
        cout << endl;

        cout << "Paragraph Level: " << endl << '\t';
        cout << (int)testCase.paragraphLevel << '\t';
        cout << endl;

        cout << "Levels: " << endl << '\t';
        for (auto level : testCase.levels) {
            if (level == LEVEL_X) {
                cout << 'x';
            } else {
                cout << (int)level;
            }
            cout << '\t';
        }
        cout << endl;

        cout << "Reordered Indexes: " << endl << '\t';
        if (testCase.order.empty()) {
            cout << "Empty";
        } else {
            for (auto index : testCase.order) {
                cout << index << '\t';
            }
        }
        cout << endl;
    }
}

AlgorithmTests::AlgorithmTests(BidiTest &bidiTest, BidiCharacterTest &bidiCharacterTest,
    const BidiMirroring &bidiMirroring) :
    m_bidiTest(bidiTest),
    m_bidiCharacterTest(bidiCharacterTest),
    m_bidiMirroring(bidiMirroring)
{
}

void AlgorithmTests::run()
{
    testBidiTypes();
    testParagraphBoundary();
    testParagraphCreation();
    testBidiAlgorithm();
}

void AlgorithmTests::testBidiAlgorithm() {
    cout << "Running bidi algorithm tests." << endl;

    size_t failures = 0;
    failures += runBidiTestCases(m_bidiTest, m_bidiMirroring);
    failures += runBidiCharacterTestCases(m_bidiCharacterTest, m_bidiMirroring);

    cout << failures << " error/s." << endl;
    cout << endl;

    assert(failures == 0);
}

void AlgorithmTests::testBidiTypes() {
    cout << "Running bidi types tests." << endl;

    struct Entry {
        char32_t codePoint;
        SBBidiType bidiType;
    };

    // List of representative characters, one per Bidi_Class
    const vector<Entry> entries = {
        {U'\u0041', SBBidiTypeL},    // L   (Latin Capital Letter A)
        {U'\u05D0', SBBidiTypeR},    // R   (Hebrew Letter Alef)
        {U'\u0627', SBBidiTypeAL},   // AL  (Arabic Letter Alef)
        {U'\u202D', SBBidiTypeLRO},  // LRO (Left-to-Right Override)
        {U'\u202E', SBBidiTypeRLO},  // RLO (Right-to-Left Override)
        {U'\u202A', SBBidiTypeLRE},  // LRE (Left-to-Right Embedding)
        {U'\u202B', SBBidiTypeRLE},  // RLE (Right-to-Left Embedding)
        {U'\u202C', SBBidiTypePDF},  // PDF (Pop Directional Format)
        {U'\u2066', SBBidiTypeLRI},  // LRI (Left-to-Right Isolate)
        {U'\u2067', SBBidiTypeRLI},  // RLI (Right-to-Left Isolate)
        {U'\u2068', SBBidiTypeFSI},  // FSI (First Strong Isolate)
        {U'\u2069', SBBidiTypePDI},  // PDI (Pop Directional Isolate)
        {U'\u0030', SBBidiTypeEN},   // EN  (European Number)
        {U'\u002B', SBBidiTypeES},   // ES  (Plus Sign - European Separator)
        {U'\u0025', SBBidiTypeET},   // ET  (Percent Sign - European Terminator)
        {U'\u0660', SBBidiTypeAN},   // AN  (Arabic-Indic Digit Zero)
        {U'\u002C', SBBidiTypeCS},   // CS  (Comma - Common Separator)
        {U'\u0020', SBBidiTypeWS},   // WS  (Space - Whitespace)
        {U'\u0009', SBBidiTypeS},    // S   (Tab - Segment Separator)
        {U'\u000A', SBBidiTypeB},    // B   (Line Feed - Paragraph Separator)
        {U'\u2060', SBBidiTypeBN},   // BN  (Word Joiner - Boundary Neutral)
        {U'\u0021', SBBidiTypeON}    // ON  (Exclamation Mark - Other Neutrals)
    };

    u32string sample;
    for (auto &e : entries) {
        sample += e.codePoint;
    }

    SBCodepointSequence sequence;
    sequence.stringEncoding = SBStringEncodingUTF32;
    sequence.stringBuffer = sample.data();
    sequence.stringLength = sample.length();

    auto algorithm = SBAlgorithmCreate(&sequence);
    auto bidiTypes = SBAlgorithmGetBidiTypesPtr(algorithm);

    for (size_t i = 0; i < sample.length(); i++) {
        assert(bidiTypes[i] == entries[i].bidiType);
    }

    SBAlgorithmRelease(algorithm);

    cout << "Passed." << endl;
    cout << endl;
}

void AlgorithmTests::testParagraphBoundary() {
    cout << "Running paragraph boundary tests." << endl;

    u16string text = u"abcdابجد\r\n";

    SBCodepointSequence sequence;
    sequence.stringEncoding = SBStringEncodingUTF16;
    sequence.stringBuffer = text.data();
    sequence.stringLength = text.length();

    auto algorithm = SBAlgorithmCreate(&sequence);
    SBAlgorithmGetParagraphBoundary(algorithm, 0, 10, nullptr, nullptr);

    struct Input {
        SBUInteger paragraphOffset;
        SBUInteger suggestedLength;
    };
    struct Output {
        SBUInteger actualLength;
        SBUInteger separatorLength;
    };

    auto test = [&](const Input &input, const Output &output) {
        SBUInteger actualLength = UINTPTR_MAX;
        SBUInteger separatorLength = UINTPTR_MAX;

        SBAlgorithmGetParagraphBoundary(algorithm, input.paragraphOffset, input.suggestedLength,
            &actualLength, &separatorLength);

        assert(actualLength == output.actualLength);
        assert(separatorLength == output.separatorLength);
    };

    test({0, 0}, {0, 0});
    test({0, 4}, {4, 0});
    test({0, 8}, {8, 0});
    test({0, 9}, {10, 2});
    test({0, 10}, {10, 2});
    test({0, UINTPTR_MAX}, {10, 2});
    test({4, 0}, {0, 0});
    test({4, 4}, {4, 0});
    test({4, 5}, {6, 2});
    test({4, 6}, {6, 2});
    test({4, UINTPTR_MAX}, {6, 2});
    test({8, 0}, {0, 0});
    test({8, 1}, {2, 2});
    test({8, 2}, {2, 2});
    test({10, 0}, {0, 0});
    test({10, UINTPTR_MAX}, {0, 0});
    test({UINTPTR_MAX, 0}, {0, 0});
    test({UINTPTR_MAX, UINTPTR_MAX}, {0, 0});

    SBAlgorithmRelease(algorithm);

    cout << "Passed." << endl;
    cout << endl;
}

void AlgorithmTests::testParagraphCreation() {
    cout << "Running paragraph creation tests." << endl;

    string text = "Line\r\n.";

    SBCodepointSequence sequence;
    sequence.stringEncoding = SBStringEncodingUTF8;
    sequence.stringBuffer = text.data();
    sequence.stringLength = text.length();

    auto algorithm = SBAlgorithmCreate(&sequence);

    struct Input {
        SBUInteger paragraphOffset;
        SBUInteger suggestedLength;
    };
    struct Output {
        SBUInteger paragraphLength;
    };

    auto test = [&](const Input &input, const Output &output) {
        auto paragraph = SBAlgorithmCreateParagraph(algorithm, input.paragraphOffset, input.suggestedLength, 0);

        if (output.paragraphLength == 0) {
            assert(paragraph == nullptr);
        } else {
            assert(paragraph != nullptr);

            auto actualOffset = SBParagraphGetOffset(paragraph);
            auto actualLength = SBParagraphGetLength(paragraph);
            auto levels = SBParagraphGetLevelsPtr(paragraph);

            assert(actualOffset == input.paragraphOffset);
            assert(actualLength == output.paragraphLength);

            for (size_t i = 0; i < actualLength; i++) {
                assert(levels[i] == 0);
            }

            SBParagraphRelease(paragraph);
        }
    };

    test({0, 0}, {0});
    test({0, 4}, {4});
    test({0, 5}, {6});
    test({0, 6}, {6});
    test({0, 7}, {6});
    test({0, UINTPTR_MAX}, {6});
    test({4, 0}, {0});
    test({4, 1}, {2});
    test({4, 2}, {2});
    test({4, 3}, {2});
    test({4, UINTPTR_MAX}, {2});
    test({6, 0}, {0});
    test({6, 1}, {1});
    test({6, UINTPTR_MAX}, {1});
    test({7, 0}, {0});
    test({7, UINTPTR_MAX}, {0});
    test({UINTPTR_MAX, 0}, {0});
    test({UINTPTR_MAX, UINTPTR_MAX}, {0});

    SBAlgorithmRelease(algorithm);

    cout << "Passed." << endl;
    cout << endl;
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    const char *dir = argv[1];

    BidiMirroring bidiMirroring(dir);
    BidiTest bidiTest(dir);
    BidiCharacterTest bidiCharacterTest(dir);

    AlgorithmTests algorithmTests(bidiTest, bidiCharacterTest, bidiMirroring);
    algorithmTests.run();

    return 0;
}

#endif
