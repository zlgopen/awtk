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
#include <cstdint>
#include <string>
#include <vector>

#include <SheenBidi/SBAttributeInfo.h>
#include <SheenBidi/SBAttributeList.h>
#include <SheenBidi/SBAttributeRegistry.h>
#include <SheenBidi/SBText.h>
#include <SheenBidi/SBTextConfig.h>

extern "C" {
#include <API/SBBase.h>
#include <API/SBParagraph.h>
#include <API/SBTextIterators.h>
}

#include "AttributeRunIteratorTests.h"

using namespace std;
using namespace SheenBidi;

using AttributeValue = uint32_t;

struct Typeface {
    static const AttributeValue Serif, SansSerif, Monospace;
};

const AttributeValue Typeface::Serif = 1;
const AttributeValue Typeface::SansSerif = 2;
const AttributeValue Typeface::Monospace = 3;

struct Alignment {
    static const AttributeValue Leading, Center, Trailing;
};

const AttributeValue Alignment::Leading = 1;
const AttributeValue Alignment::Center = 2;
const AttributeValue Alignment::Trailing = 3;

struct AttributeName {
    static constexpr auto Typeface = "typeface";
    static constexpr auto Alignment = "alignment";
};

struct AttributeItem {
    SBAttributeID attributeID;
    AttributeValue attributeValue;

    bool operator==(const AttributeItem &other) const {
        return attributeID == other.attributeID
            && attributeValue == other.attributeValue;
    }
};

static const vector<SBAttributeInfo> TestAttributes = {
    {AttributeName::Typeface, 1, SBAttributeScopeCharacter},
    {AttributeName::Alignment, 2, SBAttributeScopeParagraph}
};

static SBMutableTextRef SBTextCreateTest(const string &str, SBLevel baseLevel = SBLevelDefaultLTR) {
    auto registry = SBAttributeRegistryCreate(TestAttributes.data(), TestAttributes.size(),
        sizeof(AttributeValue), nullptr);

    auto config = SBTextConfigCreate();
    SBTextConfigSetAttributeRegistry(config, registry);
    SBTextConfigSetBaseLevel(config, baseLevel);

    auto text = SBTextCreateMutable(SBStringEncodingUTF8, config);
    SBTextAppendCodeUnits(text, str.data(), str.length());

    SBTextConfigRelease(config);
    SBAttributeRegistryRelease(registry);

    return text;
}

static bool verifyAttributes(SBAttributeListRef attributes, const vector<AttributeItem> &items) {
    if (attributes == nullptr) {
        return false;
    }

    auto count = SBAttributeListGetCount(attributes);
    if (count != items.size()) {
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        auto attribute = reinterpret_cast<const AttributeItem *>(SBAttributeListGetItem(attributes, i));
        auto item = items[i];

        if (!(*attribute == item)) {
            return false;
        }
    }

    return true;
}

void AttributeRunIteratorTests::run() {
    testInitialization();
    testBasicIteration();
    testAttributeBoundaries();
    testMultipleAttributes();
    testOverlappingRuns();
    testAttributeMerging();
    testRetainRelease();
    testEdgeCases();
    testComplexScenarios();
}

void AttributeRunIteratorTests::testInitialization() {
    // Test 1: Create iterator with valid text but zero-length
    {
        auto text = SBTextCreateTest("");
        auto iterator = SBAttributeRunIteratorCreate(text);
        assert(iterator != nullptr);

        auto run = SBAttributeRunIteratorGetCurrent(iterator);
        assert(run != nullptr);
        assert(run->index == SBInvalidIndex);
        assert(run->length == 0);
        assert(run->attributes == nullptr);

        assert(!SBAttributeRunIteratorMoveNext(iterator));

        SBAttributeRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 2: Create iterator with valid text and non-zero length
    {
        auto text = SBTextCreateTest("ABC");
        auto iterator = SBAttributeRunIteratorCreate(text);
        assert(iterator != nullptr);

        auto run = SBAttributeRunIteratorGetCurrent(iterator);
        assert(run != nullptr);
        assert(run->index == SBInvalidIndex);
        assert(run->length == 0);
        assert(run->attributes == nullptr);

        SBAttributeRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 3: Create iterator with invalid attribute type
    {
        auto text = SBTextCreateTest("ABC");
        SBTextSetAttribute(text, 0, 3, 100, &Typeface::Monospace);

        auto iterator = SBAttributeRunIteratorCreate(text);
        assert(iterator != nullptr);

        auto run = SBAttributeRunIteratorGetCurrent(iterator);
        assert(run != nullptr);
        assert(run->index == SBInvalidIndex);
        assert(run->length == 0);
        assert(run->attributes == nullptr);

        SBAttributeRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void AttributeRunIteratorTests::testBasicIteration() {
    auto text = SBTextCreateTest("Hello");
    auto registry = SBTextGetAttributeRegistry(text);

    // Set a single attribute for the entire text
    auto typeface = SBAttributeRegistryGetAttributeID(registry, AttributeName::Alignment);
    SBTextSetAttribute(text, 2, 1, typeface, &Alignment::Center);

    auto iterator = SBAttributeRunIteratorCreate(text);
    auto run = SBAttributeRunIteratorGetCurrent(iterator);

    SBAttributeRunIteratorSetupAttributeID(iterator, typeface);

    assert(SBAttributeRunIteratorMoveNext(iterator));
    assert(run->index == 0);
    assert(run->length == 5);
    assert(verifyAttributes(run->attributes, {{typeface, Alignment::Center}}));

    assert(!SBAttributeRunIteratorMoveNext(iterator));

    SBAttributeRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void AttributeRunIteratorTests::testAttributeBoundaries() {
    auto text = SBTextCreateTest("ABCDEF");
    auto registry = SBTextGetAttributeRegistry(text);

    // Set different attributes for different ranges
    auto typeface = SBAttributeRegistryGetAttributeID(registry, AttributeName::Typeface);
    SBTextSetAttribute(text, 0, 2, typeface, &Typeface::Serif);
    SBTextSetAttribute(text, 2, 2, typeface, &Typeface::SansSerif);
    SBTextSetAttribute(text, 4, 2, typeface, &Typeface::Monospace);

    auto iterator = SBAttributeRunIteratorCreate(text);
    auto run = SBAttributeRunIteratorGetCurrent(iterator);

    // First run
    assert(SBAttributeRunIteratorMoveNext(iterator));
    assert(run->index == 0);
    assert(run->length == 2);
    assert(verifyAttributes(run->attributes, {{typeface, Typeface::Serif}}));

    // Second run
    assert(SBAttributeRunIteratorMoveNext(iterator));
    assert(run->index == 2);
    assert(run->length == 2);
    assert(verifyAttributes(run->attributes, {{typeface, Typeface::SansSerif}}));

    // Third run
    assert(SBAttributeRunIteratorMoveNext(iterator));
    assert(run->index == 4);
    assert(run->length == 2);
    assert(verifyAttributes(run->attributes, {{typeface, Typeface::Monospace}}));

    assert(!SBAttributeRunIteratorMoveNext(iterator));

    SBAttributeRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void AttributeRunIteratorTests::testMultipleAttributes() {
    auto text = SBTextCreateTest("Test");
    auto registry = SBTextGetAttributeRegistry(text);

    // Set attributes of different types
    auto typeface = SBAttributeRegistryGetAttributeID(registry, AttributeName::Typeface);
    auto alignment = SBAttributeRegistryGetAttributeID(registry, AttributeName::Alignment);

    SBTextSetAttribute(text, 0, 4, typeface, &Typeface::SansSerif);
    SBTextSetAttribute(text, 0, 1, alignment, &Alignment::Leading);

    auto iterator = SBAttributeRunIteratorCreate(text);
    auto run = SBAttributeRunIteratorGetCurrent(iterator);

    SBAttributeRunIteratorSetupAttributeID(iterator, typeface);

    assert(SBAttributeRunIteratorMoveNext(iterator));
    assert(run->index == 0);
    assert(run->length == 4);
    assert(verifyAttributes(run->attributes, {{typeface, Typeface::SansSerif}}));

    SBAttributeRunIteratorSetupAttributeID(iterator, alignment);

    assert(SBAttributeRunIteratorMoveNext(iterator));
    assert(run->index == 0);
    assert(run->length == 4);
    assert(verifyAttributes(run->attributes, {{alignment, Alignment::Leading}}));
    assert(!SBAttributeRunIteratorMoveNext(iterator));

    SBAttributeRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void AttributeRunIteratorTests::testOverlappingRuns() {
    auto text = SBTextCreateTest("Sheen");
    auto registry = SBTextGetAttributeRegistry(text);

    // Create overlapping runs
    auto typeface = SBAttributeRegistryGetAttributeID(registry, AttributeName::Typeface);
    SBTextSetAttribute(text, 0, 3, typeface, &Typeface::SansSerif);
    SBTextSetAttribute(text, 2, 3, typeface, &Typeface::Monospace);

    auto iterator = SBAttributeRunIteratorCreate(text);
    auto run = SBAttributeRunIteratorGetCurrent(iterator);

    // Verify the resulting runs
    assert(SBAttributeRunIteratorMoveNext(iterator));
    assert(run->index == 0);
    assert(run->length == 2);
    assert(verifyAttributes(run->attributes, {{typeface, Typeface::SansSerif}}));

    assert(SBAttributeRunIteratorMoveNext(iterator));
    assert(run->index == 2);
    assert(run->length == 3);
    assert(verifyAttributes(run->attributes, {{typeface, Typeface::Monospace}}));

    assert(!SBAttributeRunIteratorMoveNext(iterator));

    SBAttributeRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void AttributeRunIteratorTests::testAttributeMerging() {
    auto text = SBTextCreateTest("World");
    auto registry = SBTextGetAttributeRegistry(text);

    // Set same attribute value in adjacent ranges
    auto typeface = SBAttributeRegistryGetAttributeID(registry, AttributeName::Typeface);
    SBTextSetAttribute(text, 0, 2, typeface, &Typeface::SansSerif);
    SBTextSetAttribute(text, 2, 3, typeface, &Typeface::SansSerif);

    auto iterator = SBAttributeRunIteratorCreate(text);
    auto run = SBAttributeRunIteratorGetCurrent(iterator);

    // Verify runs are merged
    assert(SBAttributeRunIteratorMoveNext(iterator));
    assert(run->index == 0);
    assert(run->length == 5);
    assert(verifyAttributes(run->attributes, {{typeface, Typeface::SansSerif}}));

    SBAttributeRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void AttributeRunIteratorTests::testRetainRelease() {
    auto text = SBTextCreateTest("ABC");
    auto iterator = SBAttributeRunIteratorCreate(text);

    // Test retain/release
    auto retained = SBAttributeRunIteratorRetain(iterator);
    assert(retained == iterator);

    SBAttributeRunIteratorRelease(retained);
    SBAttributeRunIteratorRelease(iterator);
    SBTextRelease(text);
}

void AttributeRunIteratorTests::testEdgeCases() {
    // Test 1: Single character text with attribute
    {
        auto text = SBTextCreateTest("A");
        auto registry = SBTextGetAttributeRegistry(text);

        // Set same attribute value in adjacent ranges
        auto typeface = SBAttributeRegistryGetAttributeID(registry, AttributeName::Typeface);
        SBTextSetAttribute(text, 0, 1, typeface, &Typeface::Monospace);

        auto iterator = SBAttributeRunIteratorCreate(text);
        auto run = SBAttributeRunIteratorGetCurrent(iterator);

        assert(SBAttributeRunIteratorMoveNext(iterator));
        assert(run->index == 0);
        assert(run->length == 1);
        assert(verifyAttributes(run->attributes, {{typeface, Typeface::Monospace}}));

        assert(!SBAttributeRunIteratorMoveNext(iterator));

        SBAttributeRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 2: Text with no attributes set
    {
        auto text = SBTextCreateTest("ABC");
        auto iterator = SBAttributeRunIteratorCreate(text);
        auto run = SBAttributeRunIteratorGetCurrent(iterator);

        assert(!SBAttributeRunIteratorMoveNext(iterator));
        assert(run->index == SBInvalidIndex);
        assert(run->length == 0);
        assert(run->attributes == nullptr);

        SBAttributeRunIteratorRelease(iterator);
        SBTextRelease(text);
    }

    // Test 3: Text with partial attribute coverage
    {
        auto text = SBTextCreateTest("ABC");
        auto registry = SBTextGetAttributeRegistry(text);

        auto typeface = SBAttributeRegistryGetAttributeID(registry, AttributeName::Typeface);
        SBTextSetAttribute(text, 1, 1, typeface, &Typeface::Serif);

        auto iterator = SBAttributeRunIteratorCreate(text);
        auto run = SBAttributeRunIteratorGetCurrent(iterator);

        assert(SBAttributeRunIteratorMoveNext(iterator));
        assert(run->index == 1);
        assert(run->length == 1);
        assert(verifyAttributes(run->attributes, {{typeface, Typeface::Serif}}));

        assert(!SBAttributeRunIteratorMoveNext(iterator));

        SBAttributeRunIteratorRelease(iterator);
        SBTextRelease(text);
    }
}

void AttributeRunIteratorTests::testComplexScenarios() {
    auto text = SBTextCreateTest("ABCDEFGH");
    auto registry = SBTextGetAttributeRegistry(text);

    // Overlapping and adjacent runs
    auto typeface = SBAttributeRegistryGetAttributeID(registry, AttributeName::Typeface);
    SBTextSetAttribute(text, 0, 3, typeface, &Typeface::Serif);
    SBTextSetAttribute(text, 2, 3, typeface, &Typeface::SansSerif);
    SBTextSetAttribute(text, 4, 4, typeface, &Typeface::Monospace);

    auto iterator = SBAttributeRunIteratorCreate(text);
    auto run = SBAttributeRunIteratorGetCurrent(iterator);

    // Verify the resulting runs
    SBUInteger lastOffset = 0;
    while (SBAttributeRunIteratorMoveNext(iterator)) {
        SBUInteger offset = run->index;
        SBUInteger length = run->length;

        assert(offset >= lastOffset);
        assert(offset + length <= 8);

        lastOffset = offset + length;
    }

    // Verify all attributed text was covered
    assert(lastOffset <= 8);

    SBAttributeRunIteratorRelease(iterator);
    SBTextRelease(text);
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    AttributeRunIteratorTests attributeRunIteratorTests;
    attributeRunIteratorTests.run();

    return 0;
}

#endif
