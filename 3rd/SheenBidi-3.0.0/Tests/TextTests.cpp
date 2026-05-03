/*
 * Copyright (C) 2025-2026 Muhammad Tayyab Akram
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
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <SheenBidi/SBAttributeList.h>
#include <SheenBidi/SBAttributeRegistry.h>
#include <SheenBidi/SBCodepointSequence.h>
#include <SheenBidi/SBText.h>
#include <SheenBidi/SBTextConfig.h>

extern "C" {
#include <API/SBText.h>
#include <Core/List.h>
}

#include "TextTests.h"

using namespace std;
using namespace SheenBidi;

class AttributeRegistryHolder {
public:
    explicit AttributeRegistryHolder(SBAttributeRegistryRef registry)
        : m_ptr(registry) { }

    ~AttributeRegistryHolder() {
        SBAttributeRegistryRelease(m_ptr);
    }

    operator SBAttributeRegistryRef() const { return m_ptr; }

private:
    const SBAttributeRegistryRef m_ptr;
};

class TextConfigHolder {
public:
    explicit TextConfigHolder(SBTextConfigRef config)
        : m_ptr(config) { }

    ~TextConfigHolder() {
        SBTextConfigRelease(m_ptr);
    }

    operator SBTextConfigRef() const { return m_ptr; }

private:
    const SBTextConfigRef m_ptr;
};

struct AttributeName {
    static constexpr auto Color = "color";
    static constexpr auto Alignment = "alignment";
};

struct AttributeID {
    static const SBAttributeID Color;
    static const SBAttributeID Alignment;
};

using AttributeValue = const char *;

struct AttributeItem {
    SBAttributeID attributeID;
    AttributeValue attributeValue;
};

static const auto DefaultAttributeRegistry = [] {
    const vector<SBAttributeInfo> DefaultAttributes = {
        {AttributeName::Color, 1, SBAttributeScopeCharacter},
        {AttributeName::Alignment, 2, SBAttributeScopeParagraph}
    };
    auto instance = SBAttributeRegistryCreate(DefaultAttributes.data(), DefaultAttributes.size(),
        sizeof(AttributeValue), nullptr);

    return AttributeRegistryHolder(instance);
}();

static const auto DefaultTextConfig = [] {
    auto config = SBTextConfigCreate();
    SBTextConfigSetAttributeRegistry(config, DefaultAttributeRegistry);
    return TextConfigHolder(config);
}();

const SBAttributeID AttributeID::Color = SBAttributeRegistryGetAttributeID(
    DefaultAttributeRegistry, AttributeName::Color);
const SBAttributeID AttributeID::Alignment = SBAttributeRegistryGetAttributeID(
    DefaultAttributeRegistry, AttributeName::Alignment);

static void verifyParagraphRanges(SBTextRef text, const vector<pair<size_t, size_t>> &ranges) {
    assert(text->paragraphs.count == ranges.size());

    for (size_t i = 0; i < text->paragraphs.count; ++i) {
        auto paragraph = ListGetRef(&text->paragraphs, i);
        auto &range = ranges.at(i);

        assert(paragraph->index == range.first);
        assert(paragraph->length == range.second);
    }
}

struct AttributeRun {
    SBUInteger index;
    SBUInteger length;
    map<SBAttributeID, AttributeValue> attributes;
};

static void verifyAttributeRuns(SBTextRef text, SBAttributeScope scope,
    const vector<AttributeRun> &runs) {
    auto iterator = SBTextCreateAttributeRunIterator(text);
    auto current = SBAttributeRunIteratorGetCurrent(iterator);

    SBAttributeRunIteratorSetupAttributeCollection(iterator, SBAttributeGroupNone, scope);

    size_t runIndex = 0;
    while (SBAttributeRunIteratorMoveNext(iterator)) {
        auto &run = runs.at(runIndex);

        map<SBAttributeID, AttributeValue> attributes;
        auto itemCount = SBAttributeListGetCount(current->attributes);

        for (SBUInteger attrIndex = 0; attrIndex < itemCount; attrIndex++) {
            auto item = reinterpret_cast<const AttributeItem *>(
                SBAttributeListGetItem(current->attributes, attrIndex));
            attributes[item->attributeID] = item->attributeValue;
        }

        assert(current->index == run.index);
        assert(current->length == run.length);
        assert(attributes == run.attributes);

        runIndex += 1;
    }

    assert(runIndex == runs.size());

    SBAttributeRunIteratorRelease(iterator);
}

void TextTests::run() {
    testCreateImmutableText();
    testCreateEmptyMutableText();
    testCreateCopyWithDifferentEncodings();
    testCreateImmutableCopy();
    testCreateMutableCopy();
    testGetCodeUnits();
    testAppendCodeUnits();
    testInsertCodeUnits();
    testDeleteCodeUnits();
    testReplaceCodeUnits();
    testSetCodeUnits();
    testGetBidiTypes();
    testGetScripts();
    testGetResolvedLevels();
    testGetCodeUnitParagraphInfo();
    testIterators();
    testEditingSession();
    testEdgeCases();
    testInvalidOperations();
    testReferenceCounting();
    testDifferentBaseLevels();
    testComplexBidirectionalText();
    testParagraphScenarios();
    testSetAttribute();
    testRemoveAttribute();
    testAttributeEdgeCases();
    testAttributeComplexScenarios();
}

void TextTests::testCreateImmutableText() {
    auto content = "Hello World";
    auto text = SBTextCreate(content, 11, SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    assert(SBTextGetEncoding(text) == SBStringEncodingUTF8);
    assert(SBTextGetLength(text) == 11);
    assert(SBTextGetAttributeRegistry(text) == DefaultAttributeRegistry);

    SBTextRelease(text);
}

void TextTests::testCreateEmptyMutableText() {
    SBMutableTextRef text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    assert(SBTextGetEncoding(text) == SBStringEncodingUTF8);
    assert(SBTextGetLength(text) == 0);
    assert(SBTextGetAttributeRegistry(text) == DefaultAttributeRegistry);

    SBTextRelease(text);
}

void TextTests::testCreateCopyWithDifferentEncodings() {
    // Test UTF-8
    auto text8 = SBTextCreate("Test UTF-8", 10, SBStringEncodingUTF8, DefaultTextConfig);
    assert(text8 != nullptr);

    // Test UTF-16
    auto text16 = SBTextCreate(u"Hello", 5, SBStringEncodingUTF16, DefaultTextConfig);
    assert(text16 != nullptr);

    // Test UTF-32
    auto text32 = SBTextCreate(U"Hello", 5, SBStringEncodingUTF32, DefaultTextConfig);
    assert(text32 != nullptr);

    assert(SBTextGetEncoding(text8) == SBStringEncodingUTF8);
    assert(SBTextGetEncoding(text16) == SBStringEncodingUTF16);
    assert(SBTextGetEncoding(text32) == SBStringEncodingUTF32);

    SBTextRelease(text8);
    SBTextRelease(text16);
    SBTextRelease(text32);
}

void TextTests::testCreateImmutableCopy() {
    auto content = "Original Text";
    auto original = SBTextCreate(content, 13, SBStringEncodingUTF8, DefaultTextConfig);
    assert(original != nullptr);

    auto copy = SBTextCreateCopy(original);
    assert(copy != nullptr);

    assert(SBTextGetLength(copy) == SBTextGetLength(original));
    assert(SBTextGetEncoding(copy) == SBTextGetEncoding(original));

    // Verify content is the same
    char buffer1[14], buffer2[14];
    SBTextGetCodeUnits(original, 0, 13, buffer1);
    SBTextGetCodeUnits(copy, 0, 13, buffer2);
    assert(memcmp(buffer1, buffer2, 13) == 0);

    SBTextRelease(original);
    SBTextRelease(copy);
}

void TextTests::testCreateMutableCopy() {
    auto content = "Original Text";
    auto original = SBTextCreate(content, 13, SBStringEncodingUTF8, DefaultTextConfig);
    assert(original != nullptr);

    auto mutableCopy = SBTextCreateMutableCopy(original);
    assert(mutableCopy != nullptr);

    assert(SBTextGetLength(mutableCopy) == SBTextGetLength(original));

    // Test that mutable copy can be modified
    auto appendText = " Appended";
    SBTextAppendCodeUnits(mutableCopy, appendText, 9);
    assert(SBTextGetLength(mutableCopy) == 22);

    SBTextRelease(original);
    SBTextRelease(mutableCopy);
}

void TextTests::testGetCodeUnits() {
    const auto content = "Hello World";
    auto text = SBTextCreate(content, 11, SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    char buffer[12];
    SBTextGetCodeUnits(text, 0, 11, buffer);
    buffer[11] = '\0';
    assert(strcmp(buffer, "Hello World") == 0);

    // Test partial range
    char partial[6];
    SBTextGetCodeUnits(text, 6, 5, partial);
    partial[5] = '\0';
    assert(strcmp(partial, "World") == 0);

    SBTextRelease(text);
}

void TextTests::testAppendCodeUnits() {
    auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    auto content1 = "Hello";
    auto content2 = " World";

    SBTextAppendCodeUnits(text, content1, 5);
    assert(SBTextGetLength(text) == 5);

    SBTextAppendCodeUnits(text, content2, 6);
    assert(SBTextGetLength(text) == 11);

    char buffer[12];
    SBTextGetCodeUnits(text, 0, 11, buffer);
    buffer[11] = '\0';
    assert(strcmp(buffer, "Hello World") == 0);

    SBTextRelease(text);
}

void TextTests::testInsertCodeUnits() {
    auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    auto initial = "Hello";
    SBTextAppendCodeUnits(text, initial, 5);

    // Insert in middle
    auto insert = " W";
    SBTextInsertCodeUnits(text, 1, insert, 2);

    char buffer[8];
    SBTextGetCodeUnits(text, 0, 7, buffer);
    buffer[7] = '\0';
    assert(strcmp(buffer, "H Wello") == 0);

    // Insert at beginning
    auto prefix = "Pre";
    SBTextInsertCodeUnits(text, 0, prefix, 3);

    // Insert at end
    auto suffix = " suffix";
    SBTextInsertCodeUnits(text, SBTextGetLength(text), suffix, 7);

    SBTextRelease(text);
}

void TextTests::testDeleteCodeUnits() {
    SBMutableTextRef text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    auto initial = "Hello World";
    SBTextAppendCodeUnits(text, initial, 11);

    // Delete from middle
    SBTextDeleteCodeUnits(text, 5, 6); // Delete " World"
    assert(SBTextGetLength(text) == 5);

    char buffer[6];
    SBTextGetCodeUnits(text, 0, 5, buffer);
    buffer[5] = '\0';
    assert(strcmp(buffer, "Hello") == 0);

    // Delete from beginning
    SBTextDeleteCodeUnits(text, 0, 1); // Delete "H"
    assert(SBTextGetLength(text) == 4);

    // Delete from end
    SBTextDeleteCodeUnits(text, 3, 1); // Delete "o"
    assert(SBTextGetLength(text) == 3);

    SBTextRelease(text);
}

void TextTests::testReplaceCodeUnits() {
    auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    auto initial = "Hello World";
    SBTextAppendCodeUnits(text, initial, 11);

    // Replace middle section
    auto replacement = "There";
    SBTextReplaceCodeUnits(text, 6, 5, replacement, 5);

    char buffer[12];
    SBTextGetCodeUnits(text, 0, 11, buffer);
    buffer[11] = '\0';
    assert(strcmp(buffer, "Hello There") == 0);

    // Replace entire content
    auto newContent = "New Text";
    SBTextReplaceCodeUnits(text, 0, 11, newContent, 8);
    assert(SBTextGetLength(text) == 8);

    SBTextRelease(text);
}

void TextTests::testSetCodeUnits() {
    auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    auto initial = "Initial Text";
    SBTextAppendCodeUnits(text, initial, 12);

    auto newText = "New Text";
    SBTextSetCodeUnits(text, newText, 8);

    assert(SBTextGetLength(text) == 8);

    char buffer[9];
    SBTextGetCodeUnits(text, 0, 8, buffer);
    buffer[8] = '\0';
    assert(strcmp(buffer, "New Text") == 0);

    SBTextRelease(text);
}

void TextTests::testGetBidiTypes() {
    // Test with mixed-direction text
    auto mixedText = "Hello العالم";
    auto text = SBTextCreate(mixedText, 18, SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    SBBidiType types[18];
    SBTextGetBidiTypes(text, 0, 18, types);

    SBBidiType result[] = {
        SBBidiTypeL, SBBidiTypeL, SBBidiTypeL, SBBidiTypeL, SBBidiTypeL, SBBidiTypeWS,
        SBBidiTypeAL, SBBidiTypeBN, SBBidiTypeAL, SBBidiTypeBN, SBBidiTypeAL, SBBidiTypeBN,
        SBBidiTypeAL, SBBidiTypeBN, SBBidiTypeAL, SBBidiTypeBN, SBBidiTypeAL, SBBidiTypeBN
    };
    assert(memcmp(types, result, sizeof(types)) == 0);

    SBTextRelease(text);
}

void TextTests::testGetScripts() {
    auto multiScriptText = "Hello 世界";
    auto text = SBTextCreate(multiScriptText, 12, SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    SBScript scripts[12];
    SBTextGetScripts(text, 0, 12, scripts);

    SBScript result[] = {
        SBScriptLATN, SBScriptLATN, SBScriptLATN, SBScriptLATN, SBScriptLATN, SBScriptLATN,
        SBScriptHANI, SBScriptHANI, SBScriptHANI, SBScriptHANI, SBScriptHANI, SBScriptHANI
    };
    assert(memcmp(scripts, result, sizeof(scripts)) == 0);

    SBTextRelease(text);
}

void TextTests::testGetResolvedLevels() {
    auto rtlText = "مرحبا";
    auto text = SBTextCreate(rtlText, 10, SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    SBLevel levels[10];
    SBTextGetResolvedLevels(text, 0, 10, levels);

    SBLevel result[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    assert(memcmp(levels, result, sizeof(levels)) == 0);

    SBTextRelease(text);
}

void TextTests::testGetCodeUnitParagraphInfo() {
    auto content = "First paragraph.\nSecond paragraph.";
    auto text = SBTextCreate(content, 34, SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    SBParagraphInfo info;

    // Test the first paragraph
    SBTextGetCodeUnitParagraphInfo(text, 0, &info);
    assert(info.index == 0);
    assert(info.length == 17);
    assert(info.baseLevel == 0);

    // Test the second paragraph
    SBTextGetCodeUnitParagraphInfo(text, 17, &info);
    assert(info.index == 17);
    assert(info.length == 17);
    assert(info.baseLevel == 0);

    SBTextRelease(text);
}

void TextTests::testIterators() {
    auto testText = "Test text for iterators";
    auto text = SBTextCreate(testText, 23, SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    // Test paragraph iterator
    auto paragraphIterator = SBTextCreateParagraphIterator(text);
    assert(paragraphIterator != nullptr);
    SBParagraphIteratorRelease(paragraphIterator);

    // Test logical run iterator
    auto logicalRunIterator = SBTextCreateLogicalRunIterator(text);
    assert(logicalRunIterator != nullptr);
    SBLogicalRunIteratorRelease(logicalRunIterator);

    // Test script run iterator
    auto scriptRunIterator = SBTextCreateScriptRunIterator(text);
    assert(scriptRunIterator != nullptr);
    SBScriptRunIteratorRelease(scriptRunIterator);

    // Test attribute run iterator
    auto attributeRunIterator = SBTextCreateAttributeRunIterator(text);
    assert(attributeRunIterator != nullptr);
    SBAttributeRunIteratorRelease(attributeRunIterator);

    SBTextRelease(text);
}

void TextTests::testEditingSession() {
    auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    SBTextBeginEditing(text);

    // Perform multiple operations without analysis
    auto text1 = "First";
    auto text2 = "Second";
    auto text3 = "Third";

    SBTextAppendCodeUnits(text, text1, 5);
    SBTextAppendCodeUnits(text, text2, 6);
    SBTextInsertCodeUnits(text, 5, text3, 5);

    // End editing - analysis should happen now
    SBTextEndEditing(text);

    assert(SBTextGetLength(text) == 16);

    SBTextRelease(text);
}

void TextTests::testEdgeCases() {
    // Test empty text
    auto emptyText = SBTextCreate("", 0, SBStringEncodingUTF8, DefaultTextConfig);
    assert(emptyText != nullptr);
    assert(SBTextGetLength(emptyText) == 0);
    SBTextRelease(emptyText);

    // Test very long text
    string content(1000, 'A');
    auto longText = SBTextCreate(content.c_str(), 1000, SBStringEncodingUTF8, DefaultTextConfig);
    assert(longText != nullptr);
    assert(SBTextGetLength(longText) == 1000);
    SBTextRelease(longText);
}

void TextTests::testInvalidOperations() {
    auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    // Test operations on empty text
    SBTextAppendCodeUnits(text, nullptr, 0);
    SBTextInsertCodeUnits(text, 0, nullptr, 0);
    SBTextDeleteCodeUnits(text, 0, 0);
    SBTextSetCodeUnits(text, nullptr, 0);
    SBTextReplaceCodeUnits(text, 0, 0, nullptr, 0);

    SBTextRelease(text);
}

void TextTests::testReferenceCounting() {
    auto testText = "Reference counting test";
    auto text = SBTextCreate(testText, 23, SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    // Test retain/release
    auto retained = SBTextRetain(text);
    assert(retained == text);

    SBTextRelease(retained);
    SBTextRelease(text);
}

void TextTests::testDifferentBaseLevels() {
    auto rtlConfig = SBTextConfigCreate();
    SBTextConfigSetBaseLevel(rtlConfig, 1);

    auto testText = "Test text";
    auto text = SBTextCreate(testText, 9, SBStringEncodingUTF8, rtlConfig);
    assert(text != nullptr);

    // Verify text was created successfully regardless of base level
    assert(SBTextGetLength(text) == 9);

    SBTextRelease(text);
    SBTextConfigRelease(rtlConfig);
}

void TextTests::testComplexBidirectionalText() {
    // Mixed LTR/RTL text
    auto complexText = "Hello (שלום) World";
    auto text = SBTextCreate(complexText, 22, SBStringEncodingUTF8, DefaultTextConfig);
    assert(text != nullptr);

    // Test all getter functions on complex text
    char codeUnits[22];
    SBTextGetCodeUnits(text, 0, 22, codeUnits);

    SBBidiType bidiTypes[22];
    SBTextGetBidiTypes(text, 0, 22, bidiTypes);

    SBScript scripts[22];
    SBTextGetScripts(text, 0, 22, scripts);

    SBLevel levels[22];
    SBTextGetResolvedLevels(text, 0, 22, levels);

    // Verify we can process complex text without crashes
    assert(SBTextGetLength(text) == 22);

    SBTextRelease(text);
}

void TextTests::testParagraphScenarios() {
    // Basic single paragraph
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Single paragraph text";
        SBTextAppendCodeUnits(text, content, 21);

        verifyParagraphRanges(text, {{0, 21}});

        SBTextRelease(text);
    }

    // Multiple paragraphs with \n separator
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First paragraph\nSecond paragraph\nThird paragraph";
        SBTextAppendCodeUnits(text, content, 48);

        verifyParagraphRanges(text, {{0, 16}, {16, 17}, {33, 15}});

        SBTextRelease(text);
    }

    // Multiple paragraphs with \r\n separator (Windows-style)
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First paragraph\r\nSecond paragraph\r\nThird paragraph";
        SBTextAppendCodeUnits(text, content, 50);

        verifyParagraphRanges(text, {{0, 17}, {17, 18}, {35, 15}});

        SBTextRelease(text);
    }

    // Mixed line endings: \n, \r\n, and \r
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Unix line\nWindows line\r\nOld Mac line\rEnd";
        SBTextAppendCodeUnits(text, content, 40);

        verifyParagraphRanges(text, {{0, 10}, {10, 14}, {24, 13}, {37, 3}});

        SBTextRelease(text);
    }

    // Empty paragraphs (consecutive line breaks)
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First\n\nThird\n\n\nSixth";
        SBTextAppendCodeUnits(text, content, 20);

        verifyParagraphRanges(text, {{0, 6}, {6, 1}, {7, 6}, {13, 1}, {14, 1}, {15, 5}});

        SBTextRelease(text);
    }

    // Paragraph starting with line break
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "\nStarts with newline";
        SBTextAppendCodeUnits(text, content, 20);

        verifyParagraphRanges(text, {{0, 1}, {1, 19}});

        SBTextRelease(text);
    }

    // Paragraph ending with line break
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Ends with newline\n";
        SBTextAppendCodeUnits(text, content, 18);

        verifyParagraphRanges(text, {{0, 18}});

        SBTextRelease(text);
    }

    // Only line breaks
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "\n\n\n";
        SBTextAppendCodeUnits(text, content, 3);

        verifyParagraphRanges(text, {{0, 1}, {1, 1}, {2, 1}});

        SBTextRelease(text);
    }

    // Insert at start
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Paragraph1\nParagraph2";
        SBTextAppendCodeUnits(text, content, 21);

        SBTextInsertCodeUnits(text, 0, "Start ", 6);

        verifyParagraphRanges(text, {{0, 17}, {17, 10}});

        SBTextRelease(text);
    }

    // Insert within paragraph
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Hello World";
        SBTextAppendCodeUnits(text, content, 11);

        SBTextInsertCodeUnits(text, 5, " My", 3);

        verifyParagraphRanges(text, {{0, 14}});

        SBTextRelease(text);
    }

    // Insert new line splits paragraph
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "HelloWorld";
        SBTextAppendCodeUnits(text, content, 10);

        SBTextInsertCodeUnits(text, 5, "\n", 1);

        verifyParagraphRanges(text, {{0, 6}, {6, 5}});

        SBTextRelease(text);
    }

    // Insert \r\n splits paragraph
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "HelloWorld";
        SBTextAppendCodeUnits(text, content, 10);

        SBTextInsertCodeUnits(text, 5, "\r\n", 2);

        verifyParagraphRanges(text, {{0, 7}, {7, 5}});

        SBTextRelease(text);
    }

    // Insert at paragraph boundary
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First\nSecond";
        SBTextAppendCodeUnits(text, content, 12);

        SBTextInsertCodeUnits(text, 6, "Inserted ", 9);

        verifyParagraphRanges(text, {{0, 6}, {6, 15}});

        SBTextRelease(text);
    }

    // Remove within single paragraph
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "HelloWorld";
        SBTextAppendCodeUnits(text, content, 10);

        SBTextDeleteCodeUnits(text, 2, 3);

        verifyParagraphRanges(text, {{0, 7}});

        SBTextRelease(text);
    }

    // Remove paragraph boundary (\n)
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Paragraph1\nParagraph2\nParagraph3";
        SBTextAppendCodeUnits(text, content, 32);

        SBTextDeleteCodeUnits(text, 10, 1); // Remove \n

        verifyParagraphRanges(text, {{0, 21}, {21, 10}});

        SBTextRelease(text);
    }

    // Remove paragraph boundary (\r\n)
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First\r\nSecond\r\nThird";
        SBTextAppendCodeUnits(text, content, 20);

        SBTextDeleteCodeUnits(text, 5, 2); // Remove \r\n

        verifyParagraphRanges(text, {{0, 13}, {13, 5}});

        SBTextRelease(text);
    }

    // Remove only part of \r\n (should still split paragraphs)
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First\r\nSecond";
        SBTextAppendCodeUnits(text, content, 13);

        SBTextDeleteCodeUnits(text, 5, 1); // Remove \r only

        verifyParagraphRanges(text, {{0, 6}, {6, 6}}); // \n still causes split

        SBTextRelease(text);
    }

    // Remove spanning multiple paragraphs
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "P1\nP2\nP3\nP4";
        SBTextAppendCodeUnits(text, content, 11);

        SBTextDeleteCodeUnits(text, 2, 5); // Remove "\nP2\n"

        verifyParagraphRanges(text, {{0, 4}, {4, 2}}); // "P1\n" + "P3\nP4"

        SBTextRelease(text);
    }

    // Remove entire paragraph including boundary
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First\nSecond\nThird";
        SBTextAppendCodeUnits(text, content, 18);

        SBTextDeleteCodeUnits(text, 0, 7); // Remove "First\n"

        verifyParagraphRanges(text, {{0, 6}, {6, 5}}); // "Second\nThird"

        SBTextRelease(text);
    }

    // Remove all text
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Line1\nLine2";
        SBTextAppendCodeUnits(text, content, 11);

        SBTextDeleteCodeUnits(text, 0, 11);

        verifyParagraphRanges(text, {});

        SBTextRelease(text);
    }

    // Insert into empty text
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        SBTextInsertCodeUnits(text, 0, "New", 3);

        verifyParagraphRanges(text, {{0, 3}});

        SBTextRelease(text);
    }

    // Replace across boundary
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "ABC\nDEF";
        SBTextAppendCodeUnits(text, content, 7);

        SBTextReplaceCodeUnits(text, 2, 3, "XY", 2); // Replace "C\nD" with "XY"

        verifyParagraphRanges(text, {{0, 6}}); // "ABXYEF" as single paragraph

        SBTextRelease(text);
    }

    // Replace paragraph boundary with text (merge paragraphs)
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First\nSecond";
        SBTextAppendCodeUnits(text, content, 12);

        SBTextReplaceCodeUnits(text, 5, 1, " and ", 5); // Replace \n with " and "

        verifyParagraphRanges(text, {{0, 16}}); // "First and Second" as single paragraph

        SBTextRelease(text);
    }

    // Replace text with paragraph boundary (split paragraph)
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Hello World";
        SBTextAppendCodeUnits(text, content, 11);

        SBTextReplaceCodeUnits(text, 5, 1, "\n", 1); // Replace space with \n

        verifyParagraphRanges(text, {{0, 6}, {6, 5}}); // "Hello\nWorld"

        SBTextRelease(text);
    }

    // Complex replacement spanning multiple paragraphs
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "P1\nP2\nP3\nP4";
        SBTextAppendCodeUnits(text, content, 11);

        SBTextReplaceCodeUnits(text, 2, 5, "Middle\r\nText", 12); // Replace "\nP2\n" with "Middle\r\nText"

        verifyParagraphRanges(text, {{0, 10}, {10, 6}, {16, 2}}); // "P1Middle\r\nTextP3\nP4"

        SBTextRelease(text);
    }

    // Append to text with existing paragraphs
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First\nSecond";
        SBTextAppendCodeUnits(text, content, 12);

        SBTextAppendCodeUnits(text, "\nThird", 6);

        verifyParagraphRanges(text, {{0, 6}, {6, 7}, {13, 5}});

        SBTextRelease(text);
    }

    // Append empty string
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Existing";
        SBTextAppendCodeUnits(text, content, 8);

        SBTextAppendCodeUnits(text, "", 0);

        verifyParagraphRanges(text, {{0, 8}});

        SBTextRelease(text);
    }

    // Set code units on existing text (complete replacement)
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Old text\nwith paragraphs";
        SBTextAppendCodeUnits(text, content, 24);

        auto newContent = "New single paragraph";
        SBTextSetCodeUnits(text, newContent, 20);

        verifyParagraphRanges(text, {{0, 20}});

        SBTextRelease(text);
    }

    // Unicode paragraph separators
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        // U+2029 is PARAGRAPH SEPARATOR
        auto content = "First\xE2\x80\xA9Second\xE2\x80\xA9Third";
        SBTextAppendCodeUnits(text, content, 22);

        verifyParagraphRanges(text, {{0, 8}, {8, 9}, {17, 5}});

        SBTextRelease(text);
    }

    // Mixed Unicode and ASCII paragraph separators
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        // Mix of \n, \r\n, and U+2029
        auto content = "Line1\nLine2\r\nLine3\xE2\x80\xA9Line4";
        SBTextAppendCodeUnits(text, content, 26);

        verifyParagraphRanges(text, {{0, 6}, {6, 7}, {13, 8}, {21, 5}});

        SBTextRelease(text);
    }

    // Very long paragraph
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        string longParagraph(1000, 'A'); // 1000 'A's
        SBTextAppendCodeUnits(text, longParagraph.c_str(), 1000);

        verifyParagraphRanges(text, {{0, 1000}});

        SBTextRelease(text);
    }

    // Many small paragraphs
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        string manyParagraphs;
        for (int i = 0; i < 50; ++i) {
            manyParagraphs += "P" + to_string(i) + "\n";
        }
        SBTextAppendCodeUnits(text, manyParagraphs.c_str(), manyParagraphs.length());

        // Should have 50 paragraphs
        assert(text->paragraphs.count == 50);

        SBTextRelease(text);
    }

    // Editing session with paragraph modifications
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        SBTextBeginEditing(text);

        auto content = "Initial\nText";
        SBTextAppendCodeUnits(text, content, 12);

        SBTextInsertCodeUnits(text, 7, "\nInserted", 9); // After "Initial"
        SBTextDeleteCodeUnits(text, 0, 8); // Remove "Initial\n"
        SBTextReplaceCodeUnits(text, 8, 5, "Modified", 8); // Replace "Text"

        SBTextEndEditing(text);

        verifyParagraphRanges(text, {{0, 16}}); // "InsertedModified"

        SBTextRelease(text);
    }

    // RTL text paragraphs
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Hello (مرحبا)\nWorld (عالم)";
        SBTextAppendCodeUnits(text, content, 35);

        verifyParagraphRanges(text, {{0, 19}, {19, 16}});

        SBTextRelease(text);
    }

    // Empty text then add content
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        verifyParagraphRanges(text, {});

        SBTextAppendCodeUnits(text, "First\nSecond", 12);

        verifyParagraphRanges(text, {{0, 6}, {6, 6}});

        SBTextRelease(text);
    }

    // Multiple operations in sequence
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        SBTextAppendCodeUnits(text, "Start", 5);
        verifyParagraphRanges(text, {{0, 5}});

        SBTextInsertCodeUnits(text, 2, "\n", 1);
        verifyParagraphRanges(text, {{0, 3}, {3, 3}});

        SBTextAppendCodeUnits(text, "\nEnd", 4);
        verifyParagraphRanges(text, {{0, 3}, {3, 4}, {7, 3}});

        SBTextDeleteCodeUnits(text, 3, 2);
        verifyParagraphRanges(text, {{0, 3}, {3, 2}, {5, 3}});

        SBTextRelease(text);
    }
}

void TextTests::testSetAttribute() {
    // Test setting character-scoped attribute
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Hello World";
        SBTextAppendCodeUnits(text, content, 11);

        // Set color attribute on entire text
        AttributeValue redColor("red");
        SBTextSetAttribute(text, 0, 11, AttributeID::Color, &redColor);

        // Set color attribute on partial range
        AttributeValue blueColor("blue");
        SBTextSetAttribute(text, 0, 5, AttributeID::Color, &blueColor);

        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {0, 5, {{AttributeID::Color, blueColor}}},
            {5, 6, {{AttributeID::Color, redColor}}}
        });

        SBTextRelease(text);
    }

    // Test setting paragraph-scoped attribute
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First paragraph.\nSecond paragraph.";
        SBTextAppendCodeUnits(text, content, 34);

        // Set alignment attribute on first paragraph
        AttributeValue centerAlign("center");
        SBTextSetAttribute(text, 0, 17, AttributeID::Alignment, &centerAlign);

        // Set alignment attribute on second paragraph
        AttributeValue rightAlign("right");
        SBTextSetAttribute(text, 17, 17, AttributeID::Alignment, &rightAlign);

        verifyAttributeRuns(text, SBAttributeScopeParagraph, {
            {0, 17, {{AttributeID::Alignment, centerAlign}}},
            {17, 17, {{AttributeID::Alignment, rightAlign}}}
        });

        SBTextRelease(text);
    }

    // Test setting attribute on empty text
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        // Should handle zero-length range gracefully
        AttributeValue colorValue("test");
        SBTextSetAttribute(text, 0, 0, AttributeID::Color, &colorValue);

        verifyAttributeRuns(text, SBAttributeScopeParagraph, {});

        SBTextRelease(text);
    }

    // Test setting multiple attributes on the same range
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Test content";
        SBTextAppendCodeUnits(text, content, 12);

        AttributeValue colorValue("green");
        AttributeValue alignValue("left");

        // Set multiple attributes on same range
        SBTextSetAttribute(text, 0, 12, AttributeID::Color, &colorValue);
        SBTextSetAttribute(text, 0, 12, AttributeID::Alignment, &alignValue);

        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {0, 12, {{AttributeID::Color, colorValue}}}
        });
        verifyAttributeRuns(text, SBAttributeScopeParagraph, {
            {0, 12, {{AttributeID::Alignment, alignValue}}}
        });

        SBTextRelease(text);
    }

    // Test setting attribute with editing session
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        SBTextBeginEditing(text);

        auto content = "Editing session test";
        SBTextAppendCodeUnits(text, content, 20);

        // Set attribute during editing session
        AttributeValue colorValue("yellow");
        SBTextSetAttribute(text, 0, 20, AttributeID::Color, &colorValue);

        SBTextEndEditing(text);

        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {0, 20, {{AttributeID::Color, colorValue}}}
        });

        SBTextRelease(text);
    }
}

void TextTests::testRemoveAttribute() {
    // Test removing character-scoped attribute
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Hello World";
        SBTextAppendCodeUnits(text, content, 11);

        // Set color attribute first
        AttributeValue redColor("red");
        SBTextSetAttribute(text, 0, 11, AttributeID::Color, &redColor);

        // Remove color attribute from entire text
        SBTextRemoveAttribute(text, 0, 11, AttributeID::Color);

        // Remove again (should be idempotent)
        SBTextRemoveAttribute(text, 0, 11, AttributeID::Color);

        verifyAttributeRuns(text, SBAttributeScopeCharacter, {});

        SBTextRelease(text);
    }

    // Test removing paragraph-scoped attribute
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "First paragraph.\nSecond paragraph.";
        SBTextAppendCodeUnits(text, content, 34);

        // Set alignment attribute on both paragraphs
        AttributeValue centerAlign("center");
        SBTextSetAttribute(text, 0, 34, AttributeID::Alignment, &centerAlign);

        // Remove alignment from the first paragraph only
        SBTextRemoveAttribute(text, 0, 17, AttributeID::Alignment);

        // Alignment should still exist on the second paragraph
        verifyAttributeRuns(text, SBAttributeScopeParagraph, {
            {17, 17, {{AttributeID::Alignment, centerAlign}}}
        });

        SBTextRelease(text);
    }

    // Test removing attribute from partial range
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Hello World";
        SBTextAppendCodeUnits(text, content, 11);

        // Set color attribute on entire text
        AttributeValue redColor("red");
        SBTextSetAttribute(text, 0, 11, AttributeID::Color, &redColor);

        // Remove color from first half only
        SBTextRemoveAttribute(text, 0, 5, AttributeID::Color);

        // Color should still exist on second half
        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {5, 6, {{AttributeID::Color, redColor}}}
        });

        SBTextRelease(text);
    }

    // Test removing non-existent attribute
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Test content";
        SBTextAppendCodeUnits(text, content, 12);

        // Remove attribute that was never set (should succeed)
        SBTextRemoveAttribute(text, 0, 12, AttributeID::Color);

        verifyAttributeRuns(text, SBAttributeScopeCharacter, {});

        SBTextRelease(text);
    }

    // Test removing attribute from empty text
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        // Should handle zero-length range gracefully
        SBTextRemoveAttribute(text, 0, 0, AttributeID::Color);

        verifyAttributeRuns(text, SBAttributeScopeCharacter, {});

        SBTextRelease(text);
    }

    // Test removing attribute with editing session
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Editing session test";
        SBTextAppendCodeUnits(text, content, 20);

        // Set attribute first
        AttributeValue colorValue("yellow");
        SBTextSetAttribute(text, 0, 20, AttributeID::Color, &colorValue);

        SBTextBeginEditing(text);

        // Remove attribute during editing session
        SBTextRemoveAttribute(text, 0, 20, AttributeID::Color);

        SBTextEndEditing(text);

        verifyAttributeRuns(text, SBAttributeScopeCharacter, {});

        SBTextRelease(text);
    }
}

void TextTests::testAttributeEdgeCases() {
    // Test attribute operations on boundary conditions
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Short";
        SBTextAppendCodeUnits(text, content, 5);

        // Set attribute at very end
        AttributeValue colorValue("red");
        SBTextSetAttribute(text, 4, 1, AttributeID::Color, &colorValue);

        // Remove from single character
        SBTextRemoveAttribute(text, 4, 1, AttributeID::Color);

        verifyAttributeRuns(text, SBAttributeScopeCharacter, {});

        SBTextRelease(text);
    }

    // Test attribute operations after text modifications
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Original text";
        SBTextAppendCodeUnits(text, content, 13);

        // Set attribute
        AttributeValue colorValue("blue");
        SBTextSetAttribute(text, 0, 13, AttributeID::Color, &colorValue);

        // Insert text in middle
        auto insertText = " modified";
        SBTextInsertCodeUnits(text, 8, insertText, 9);

        // Attribute should still be present
        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {0, 22, {{AttributeID::Color, colorValue}}}
        });

        // Remove part of the text
        SBTextDeleteCodeUnits(text, 0, 8);

        // Attribute should still be present on remaining text
        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {0, 14, {{AttributeID::Color, colorValue}}}
        });

        SBTextRelease(text);
    }

    // Test multiple attribute types on overlapping ranges
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Test content for multiple attributes";
        SBTextAppendCodeUnits(text, content, 37);

        // Set color on the first half
        AttributeValue redColor("red");
        SBTextSetAttribute(text, 0, 18, AttributeID::Color, &redColor);

        // Set alignment on overlapping range
        AttributeValue centerAlign("center");
        SBTextSetAttribute(text, 10, 20, AttributeID::Alignment, &centerAlign);

        // Remove color from the middle section
        SBTextRemoveAttribute(text, 5, 10, AttributeID::Color);

        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {0, 5, {{AttributeID::Color, redColor}}},
            {15, 3, {{AttributeID::Color, redColor}}},
        });
        verifyAttributeRuns(text, SBAttributeScopeParagraph, {
            {0, 37, {{AttributeID::Alignment, centerAlign}}}
        });

        SBTextRelease(text);
    }
}

void TextTests::testAttributeComplexScenarios() {
    // Test attributes with complex text modifications
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Start Middle End";
        SBTextAppendCodeUnits(text, content, 16);

        // Set attributes on various ranges
        AttributeValue redColor("red");
        AttributeValue blueColor("blue");
        AttributeValue greenColor("green");

        SBTextSetAttribute(text, 0, 5, AttributeID::Color, &redColor);    // "Start" - red
        SBTextSetAttribute(text, 6, 6, AttributeID::Color, &blueColor);   // "Middle" - blue
        SBTextSetAttribute(text, 13, 3, AttributeID::Color, &greenColor); // "End" - green

        // Insert text that splits an attributed range
        auto insertText = "New";
        SBTextInsertCodeUnits(text, 6, insertText, 3); // Insert before "Middle"

        // "Middle" should still be blue, but now at different position
        // "New" should have no color attribute
        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {0, 5, {{AttributeID::Color, redColor}}},
            {9, 6, {{AttributeID::Color, blueColor}}},
            {16, 3, {{AttributeID::Color, greenColor}}}
        });

        // Delete text that contains attributes
        SBTextDeleteCodeUnits(text, 0, 4); // Delete "Star" from "Start"

        // Remaining "t" should still be red
        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {0, 1, {{AttributeID::Color, redColor}}},
            {5, 6, {{AttributeID::Color, blueColor}}},
            {12, 3, {{AttributeID::Color, greenColor}}}
        });

        // Replace text with attributes
        auto replaceText = "Replacement";
        SBTextReplaceCodeUnits(text, 5, 6, replaceText, 11); // Replace "Middle"

        // Replacement text should inherit blue color
        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {0, 1, {{AttributeID::Color, redColor}}},
            {5, 11, {{AttributeID::Color, blueColor}}},
            {17, 3, {{AttributeID::Color, greenColor}}}
        });

        SBTextRelease(text);
    }

    // Test attributes with bidirectional text
    {
        auto text = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(text != nullptr);

        auto content = "Hello (العربية) World";
        SBTextAppendCodeUnits(text, content, 25);

        // Set attributes on mixed-direction text
        AttributeValue colorValue("purple");
        SBTextSetAttribute(text, 7, 8, AttributeID::Color, &colorValue); // Arabic text

        // Verify we can iterate attributes on bidirectional text
        verifyAttributeRuns(text, SBAttributeScopeCharacter, {
            {7, 8, {{AttributeID::Color, colorValue}}}
        });

        SBTextRelease(text);
    }

    // Test attribute persistence across copy operations
    {
        auto original = SBTextCreateMutable(SBStringEncodingUTF8, DefaultTextConfig);
        assert(original != nullptr);

        auto content = "Text with attributes";
        SBTextAppendCodeUnits(original, content, 20);

        // Set attributes
        AttributeValue colorValue("orange");
        AttributeValue alignValue("justify");

        SBTextSetAttribute(original, 0, 20, AttributeID::Color, &colorValue);
        SBTextSetAttribute(original, 0, 20, AttributeID::Alignment, &alignValue);

        // Create immutable copy
        auto immutableCopy = SBTextCreateCopy(original);
        assert(immutableCopy != nullptr);

        // Create mutable copy
        auto mutableCopy = SBTextCreateMutableCopy(original);
        assert(mutableCopy != nullptr);

        // Verify copies have same content and attributes
        assert(SBTextGetLength(immutableCopy) == SBTextGetLength(original));
        verifyAttributeRuns(immutableCopy, SBAttributeScopeCharacter, {
            {0, 20, {{AttributeID::Color, colorValue}}}
        });
        verifyAttributeRuns(immutableCopy, SBAttributeScopeParagraph, {
            {0, 20, {{AttributeID::Alignment, alignValue}}}
        });

        assert(SBTextGetLength(mutableCopy) == SBTextGetLength(original));
        verifyAttributeRuns(mutableCopy, SBAttributeScopeCharacter, {
            {0, 20, {{AttributeID::Color, colorValue}}}
        });
        verifyAttributeRuns(mutableCopy, SBAttributeScopeParagraph, {
            {0, 20, {{AttributeID::Alignment, alignValue}}}
        });

        SBTextRelease(original);
        SBTextRelease(immutableCopy);
        SBTextRelease(mutableCopy);
    }
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    TextTests textTests;
    textTests.run();

    return 0;
}

#endif
