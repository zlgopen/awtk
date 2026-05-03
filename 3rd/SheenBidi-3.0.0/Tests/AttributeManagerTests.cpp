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
#include <random>
#include <string>
#include <vector>

#include <SheenBidi/SBAttributeList.h>
#include <SheenBidi/SBBase.h>

extern "C" {
#include <API/SBAttributeRegistry.h>
#include <API/SBText.h>
#include <Text/AttributeDictionary.h>
#include <Text/AttributeManager.h>
}

#include "AttributeManagerTests.h"

using namespace std;
using namespace SheenBidi;

struct AttributeName {
    static constexpr auto Alignment = "alignment";
    static constexpr auto Color = "color";
    static constexpr auto Font = "font";
    static constexpr auto Language = "language";
};

enum class AttributeType {
    None,
    Alignment,
    Color,
    Font,
    Language
};

enum class Alignment {
    Leading,
    Center,
    Trailing
};

struct Color {
    const uint32_t rgb;

    static const Color Red, Green, Blue, Yellow, Black, Purple;

    explicit Color(uint32_t rgb) : rgb(rgb) { }

    bool operator==(const Color &other) const {
        return rgb == other.rgb;
    }
};

const Color Color::Red = Color(0xFF0000);
const Color Color::Green = Color(0x00FF00);
const Color Color::Blue = Color(0x0000FF);
const Color Color::Yellow = Color(0xFFFF00);
const Color Color::Black = Color(0x000000);
const Color Color::Purple = Color(0xFF00FF);

struct Font {
    const string name;

    static const Font Arial, Times;

private:
    explicit Font(const char *name) : name(name) { }

    bool operator==(const Font &other) const {
        return name == other.name;
    }
};

const Font Font::Arial = Font("Arial");
const Font Font::Times = Font("Times");

struct Language {
    const string locale;

    static const Language English, Urdu;

private:
    explicit Language(const char *locale): locale(locale) { }

    bool operator==(const Language &other) const {
        return locale == other.locale;
    }
};

const Language Language::English = Language("en");
const Language Language::Urdu = Language("ur");

struct AttributeValue {
    const AttributeType type;
    union {
        Alignment alignment;
        Color color;
        const Font *font;
        const Language *language;
    };

    static const AttributeValue None;

    static AttributeValue *create(const AttributeValue &source) {
        return new AttributeValue(source);
    }

    static AttributeValue *retain(AttributeValue *value) {
        value->retainCount += 1;
        return value;
    }

    static void release(AttributeValue *value) {
        if (--value->retainCount == 0) {
            delete value;
        }
    }

    AttributeValue() :
        type(AttributeType::None) { }

    AttributeValue(const Alignment &alignment) :
        type(AttributeType::Alignment), alignment(alignment) { }

    AttributeValue(const Color &color) :
        type(AttributeType::Color), color(color) { }

    AttributeValue(const Font &font) :
        type(AttributeType::Font), font(&font) { }

    AttributeValue(const Language &language)
        : type(AttributeType::Language), language(&language) { }

    bool operator==(const AttributeValue &other) const {
        if (type == other.type) {
            switch (type) {
            case AttributeType::None:
                return true;
            case AttributeType::Alignment:
                return alignment == other.alignment;
            case AttributeType::Color:
                return color == other.color;
            case AttributeType::Font:
                return font == other.font;
            case AttributeType::Language:
                return language == other.language;
            }
        }

        return false;
    }

private:
    size_t retainCount = 1;

    void *operator new(size_t size) {
        return ::operator new(size);
    }

    void operator delete(void *ptr) {
        ::operator delete(ptr);
    }
};

const AttributeValue AttributeValue::None = AttributeValue();

class AttributePool {
public:
    AttributePool() = default;
    AttributePool(const AttributePool &) = delete;
    AttributePool &operator=(const AttributePool &) = delete;

    ~AttributePool() {
        for (AttributeValue *object : m_objects) {
            AttributeValue::release(object);
        }
    }

    AttributeValue *add(const AttributeValue &source) {
        auto object = AttributeValue::create(source);
        m_objects.push_back(object);
        return object;
    }

private:
    vector<AttributeValue *> m_objects;
};

static const vector<SBAttributeInfo> TestAttributes = {
    {AttributeName::Color, 1, SBAttributeScopeCharacter},
    {AttributeName::Font, 1, SBAttributeScopeCharacter},
    {AttributeName::Language, 2, SBAttributeScopeParagraph},
    {AttributeName::Alignment, 3, SBAttributeScopeParagraph}
};

// Helper to generate random strings
static string generateString(size_t length) {
    static const string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static random_device rd;
    static mt19937 generator(rd());
    static uniform_int_distribution<> distribution(0, characters.size() - 1);

    string result;
    result.reserve(length);
    for (size_t i = 0; i < length; i++) {
        result += characters[distribution(generator)];
    }
    return result;
}

// Helper to verify attribute at position
static bool verifyAttribute(AttributeManagerRef manager, size_t index,
    SBAttributeID attributeID, const AttributeValue &expectedValue)
{
    auto entry = AttributeManagerFindEntry(manager, index, nullptr, nullptr);
    auto actualValue = AttributeDictionaryFindValue(entry->attributes, attributeID);
    if (!actualValue) {
        return expectedValue == AttributeValue::None;
    }

    return SBAttributeRegistryIsEqualAttribute(manager->_registry, attributeID,
        actualValue, &expectedValue);
}

static SBMutableTextRef SBTextCreateWithRegistry(SBAttributeRegistryRef registry) {
    auto config = SBTextConfigCreate();
    SBTextConfigSetAttributeRegistry(config, registry);

    auto text = SBTextCreateMutable(SBStringEncodingUTF8, config);

    SBTextConfigRelease(config);

    return text;
}

static SBMutableTextRef SBTextCreateWithDefaultRegistry() {
    SBAttributeValueCallbacks callbacks = {
        [](const void *first, const void *second) -> SBBoolean {
            auto value1 = static_cast<const AttributeValue *>(first);
            auto value2 = static_cast<const AttributeValue *>(second);
            return *value1 == *value2;
        },
        [](const void *value) -> const void * {
            auto attribute = static_cast<AttributeValue *>(const_cast<void *>(value));
            return AttributeValue::retain(attribute);
        },
        [](const void *value) {
            auto attribute = static_cast<AttributeValue *>(const_cast<void *>(value));
            AttributeValue::release(attribute);
        }
    };
    auto registry = SBAttributeRegistryCreate(TestAttributes.data(), TestAttributes.size(),
        sizeof(AttributeValue), &callbacks);
    auto text = SBTextCreateWithRegistry(registry);

    SBAttributeRegistryRelease(text->attributeRegistry);

    return text;
}

static void SBTextAppendRandomCodeUnits(SBMutableTextRef text, SBUInteger length) {
    const auto content = generateString(length);
    SBTextAppendCodeUnits(text, content.c_str(), length);
}

static void SBTextInsertRandomParagraph(SBMutableTextRef text,
    SBUInteger index, SBUInteger length) {
    const auto paragraph = generateString(length - 1) + '\n';
    SBTextInsertCodeUnits(text, index, paragraph.c_str(), length);
}

static void SBTextReplaceWithRandomCodeUnits(SBMutableTextRef text,
    SBUInteger index, SBUInteger length, SBUInteger newLength) {
    const auto content = generateString(newLength);
    SBTextReplaceCodeUnits(text, index, length, content.c_str(), newLength);
}

void AttributeManagerTests::run() {
    testInitializeWithNullRegistry();
    testInitializeWithValidRegistry();

    // Set/Remove attribute tests
    testSetCharacterAttribute();
    testSetParagraphAttributeExtension();
    testRemoveAttribute();
    testMultipleAttributesSameRange();
    testOverwriteAttributeValue();
    testComplexAttributeOverlapping();
    testBoundaryConditions();
    testLargeTextOperations();
    testMultipleParagraphAttributes();
    testAttributeRemovalEdgeCases();
    testOperationsWithNullRegistry();
    testZeroLengthOperations();

    // Replacement tests
    testReplaceRangePureInsertion();
    testReplaceRangePureDeletion();
    testReplaceRangeBasicReplacement();
    testReplaceRangeAtTextBoundaries();
    testReplaceRangeWithSingleEntry();
    testReplaceRangeSpanningMultipleEntries();
    testReplaceRangeRemovingEntireEntries();
    testReplaceRangeWithParagraphAttributes();
    testReplaceRangeParagraphMerging();
    testReplaceRangeComplexAttributePatterns();
    testReplaceRangeMultipleAttributes();
    testReplaceRangeAttributeInheritance();

    // Run iteration tests
    testGetRunByIDBasic();
    testGetRunByFilteredCollection();
    testEmptyRuns();
    testFilterByAttributeGroup();
    testRunBoundariesWithMixedAttributes();
    testComplexRunDetectionScenarios();
}

void AttributeManagerTests::testInitializeWithNullRegistry() {
    auto text = SBTextCreateWithRegistry(nullptr);
    auto manager = &text->attributeManager;

    assert(manager->_registry == nullptr);
    assert(manager->parent == text);

    SBTextRelease(text);
}

void AttributeManagerTests::testInitializeWithValidRegistry() {
    auto text = SBTextCreateWithDefaultRegistry();
    auto manager = &text->attributeManager;

    assert(manager->_registry != nullptr);
    assert(manager->_registry == text->attributeRegistry);
    assert(manager->parent == text);
    assert(manager->_entries.count == 1);

    SBTextAppendRandomCodeUnits(text, 5);
    assert(manager->_entries.count == 1);

    // All positions should have no attributes initially
    for (SBUInteger i = 0; i < 5; i++) {
        assert(verifyAttribute(manager, i, 0, AttributeValue::None));
    }

    SBTextRelease(text);
}

void AttributeManagerTests::testSetCharacterAttribute() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 5);
    SBTextAppendRandomCodeUnits(text, 5);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto blue = pool.add(Color::Blue);

    SBTextSetAttribute(text, 1, 3, color, blue);

    // Verify attributes in range
    for (size_t i = 1; i < 4; i++) {
        assert(verifyAttribute(manager, i, color, Color::Blue));
    }

    // Verify position 0 doesn't have the attribute
    assert(verifyAttribute(manager, 0, color, AttributeValue::None));

    SBTextRelease(text);
}

void AttributeManagerTests::testSetParagraphAttributeExtension() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextInsertRandomParagraph(text, 0, 10);
    SBTextInsertRandomParagraph(text, 10, 10);
    SBTextInsertRandomParagraph(text, 20, 10);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto langID = SBAttributeRegistryGetAttributeID(registry, AttributeName::Language);

    AttributePool pool;
    auto english = pool.add(Language::English);

    // Set the paragraph attribute in the middle of the first paragraph
    SBTextSetAttribute(text, 6, 5, langID, english);

    // Should extend to the entire first and second paragraphs (0-19)
    for (size_t i = 0; i < 20; i++) {
        assert(verifyAttribute(manager, i, langID, Language::English));
    }

    // The third paragraph should not have the attribute
    assert(verifyAttribute(manager, 20, langID, AttributeValue::None));

    SBTextRelease(text);
}

void AttributeManagerTests::testRemoveAttribute() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 5);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto green = pool.add(Color::Green);

    // Set and verify attribute
    SBTextSetAttribute(text, 1, 3, color, green);
    assert(verifyAttribute(manager, 2, color, Color::Green));

    // Remove attribute
    SBTextRemoveAttribute(text, 2, 1, color);
    assert(verifyAttribute(manager, 2, color, AttributeValue::None));

    SBTextRelease(text);
}

void AttributeManagerTests::testMultipleAttributesSameRange() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 5);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);
    auto font = SBAttributeRegistryGetAttributeID(registry, AttributeName::Font);

    AttributePool pool;
    auto purple = pool.add(Color::Purple);
    auto arial = pool.add(Font::Arial);

    // Set multiple attributes at the same position
    SBTextSetAttribute(text, 2, 1, color, purple);
    SBTextSetAttribute(text, 2, 1, font, arial);

    assert(verifyAttribute(manager, 2, color, Color::Purple));
    assert(verifyAttribute(manager, 2, font, Font::Arial));

    SBTextRelease(text);
}

void AttributeManagerTests::testOverwriteAttributeValue() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 3);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto black = pool.add(Color::Black);

    // Set initial value
    SBTextSetAttribute(text, 1, 1, color, red);
    assert(verifyAttribute(manager, 1, color, Color::Red));

    // Overwrite with new value
    SBTextSetAttribute(text, 1, 1, color, black);
    assert(verifyAttribute(manager, 1, color, Color::Black));

    SBTextRelease(text);
}

void AttributeManagerTests::testComplexAttributeOverlapping() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextInsertRandomParagraph(text, 0, 20);
    SBTextInsertRandomParagraph(text, 20, 20);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);
    auto font = SBAttributeRegistryGetAttributeID(registry, AttributeName::Font);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto blue = pool.add(Color::Blue);
    auto arial = pool.add(Font::Arial);

    // Create overlapping attribute regions
    SBTextSetAttribute(text, 0, 10, color, red);
    SBTextSetAttribute(text, 5, 10, font, arial);
    SBTextSetAttribute(text, 10, 10, color, blue);

    // Verify overlapping regions
    for (size_t i = 0; i < 5; i++) {
        assert(verifyAttribute(manager, i, color, Color::Red));
        assert(verifyAttribute(manager, i, font, AttributeValue::None));
    }

    for (size_t i = 5; i < 10; i++) {
        assert(verifyAttribute(manager, i, color, Color::Red));
        assert(verifyAttribute(manager, i, font, Font::Arial));
    }

    for (size_t i = 10; i < 15; i++) {
        assert(verifyAttribute(manager, i, color, Color::Blue));
        assert(verifyAttribute(manager, i, font, Font::Arial));
    }

    for (size_t i = 15; i < 20; i++) {
        assert(verifyAttribute(manager, i, color, Color::Blue));
        assert(verifyAttribute(manager, i, font, AttributeValue::None));
    }

    SBTextRelease(text);
}

void AttributeManagerTests::testBoundaryConditions() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextInsertRandomParagraph(text, 0, 10);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto blue = pool.add(Color::Blue);
    auto green = pool.add(Color::Green);
    auto yellow = pool.add(Color::Yellow);

    // Test at the start boundary
    SBTextSetAttribute(text, 0, 1, color, red);
    assert(verifyAttribute(manager, 0, color, Color::Red));

    // Test at the end boundary
    SBTextSetAttribute(text, 9, 1, color, blue);
    assert(verifyAttribute(manager, 9, color, Color::Blue));

    // Test a single character in the middle
    SBTextSetAttribute(text, 5, 1, color, green);
    assert(verifyAttribute(manager, 5, color, Color::Green));

    // Test zero-length range (should not crash)
    SBTextSetAttribute(text, 5, 0, color, yellow);

    SBTextRelease(text);
}

void AttributeManagerTests::testLargeTextOperations() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 1000);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto green = pool.add(Color::Green);
    auto blue = pool.add(Color::Blue);
    auto yellow = pool.add(Color::Yellow);
    
    // Set attributes in chunks
    SBTextSetAttribute(text, 0, 250, color, red);
    SBTextSetAttribute(text, 250, 250, color, green);
    SBTextSetAttribute(text, 500, 250, color, blue);
    SBTextSetAttribute(text, 750, 250, color, yellow);

    // Verify chunks
    assert(verifyAttribute(manager, 0, color, Color::Red));
    assert(verifyAttribute(manager, 249, color, Color::Red));
    assert(verifyAttribute(manager, 250, color, Color::Green));
    assert(verifyAttribute(manager, 499, color, Color::Green));
    assert(verifyAttribute(manager, 500, color, Color::Blue));
    assert(verifyAttribute(manager, 749, color, Color::Blue));
    assert(verifyAttribute(manager, 750, color, Color::Yellow));
    assert(verifyAttribute(manager, 999, color, Color::Yellow));

    SBTextRelease(text);
}

void AttributeManagerTests::testMultipleParagraphAttributes() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextInsertRandomParagraph(text, 0, 10);
    SBTextInsertRandomParagraph(text, 10, 10);
    SBTextInsertRandomParagraph(text, 20, 10);
    SBTextInsertRandomParagraph(text, 30, 10);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto language = SBAttributeRegistryGetAttributeID(registry, AttributeName::Language);
    auto alignment = SBAttributeRegistryGetAttributeID(registry, AttributeName::Alignment);

    AttributePool pool;
    auto english = pool.add(Language::English);
    auto urdu = pool.add(Language::Urdu);
    auto center = pool.add(Alignment::Center);

    // Set different paragraph attributes
    SBTextSetAttribute(text, 5, 1, language, english);
    SBTextSetAttribute(text, 15, 1, language, urdu);
    SBTextSetAttribute(text, 25, 1, alignment, center);

    // Verify paragraph-level extension
    for (size_t i = 0; i < 10; i++) {
        assert(verifyAttribute(manager, i, language, Language::English));
    }
    for (size_t i = 10; i < 20; i++) {
        assert(verifyAttribute(manager, i, language, Language::Urdu));
    }
    for (size_t i = 20; i < 30; i++) {
        assert(verifyAttribute(manager, i, alignment, Alignment::Center));
    }

    SBTextRelease(text);
}

void AttributeManagerTests::testAttributeRemovalEdgeCases() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextInsertRandomParagraph(text, 0, 10);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);

    // Set attribute
    SBTextSetAttribute(text, 0, 10, color, red);

    // Remove from middle
    SBTextRemoveAttribute(text, 3, 4, color);

    // Verify boundaries
    assert(verifyAttribute(manager, 0, color, Color::Red));
    assert(verifyAttribute(manager, 2, color, Color::Red));
    assert(verifyAttribute(manager, 3, color, AttributeValue::None));
    assert(verifyAttribute(manager, 6, color, AttributeValue::None));
    assert(verifyAttribute(manager, 7, color, Color::Red));
    assert(verifyAttribute(manager, 9, color, Color::Red));

    // Remove from start
    SBTextRemoveAttribute(text, 0, 3, color);
    assert(verifyAttribute(manager, 0, color, AttributeValue::None));
    assert(verifyAttribute(manager, 2, color, AttributeValue::None));

    // Remove from end
    SBTextRemoveAttribute(text, 7, 3, color);
    assert(verifyAttribute(manager, 7, color, AttributeValue::None));
    assert(verifyAttribute(manager, 9, color, AttributeValue::None));

    SBTextRelease(text);
}

void AttributeManagerTests::testOperationsWithNullRegistry() {
    auto text = SBTextCreateWithRegistry(nullptr);
    SBTextAppendRandomCodeUnits(text, 10);

    // These should handle null registry gracefully
    SBTextDeleteCodeUnits(text, 0, 5);
    SBTextRemoveAttribute(text, 0, 5, 1);

    // Setting attributes with null registry should not crash
    SBTextSetAttribute(text, 0, 1, 1, "value");

    SBTextRelease(text);
}

void AttributeManagerTests::testZeroLengthOperations() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 5);

    auto registry = text->attributeRegistry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);

    // Zero-length operations should be handled without crashing
    SBTextSetAttribute(text, 2, 0, color, red);
    SBTextRemoveAttribute(text, 2, 0, color);

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangePureInsertion() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 10);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto blue = pool.add(Color::Blue);

    // Set up initial attributes: [RRRRRBBBBB] where R=red, B=blue
    SBTextSetAttribute(text, 0, 5, color, red);
    SBTextSetAttribute(text, 5, 5, color, blue);

    // Pure insertion at position 3 (middle of red range)
    SBTextReplaceWithRandomCodeUnits(text, 3, 0, 3);

    // Expected: [RRR + new3 + RRBBBBB] where new3 inherits red
    assert(verifyAttribute(manager, 0, color, Color::Red));   // First red preserved
    assert(verifyAttribute(manager, 2, color, Color::Red));   // First red preserved
    assert(verifyAttribute(manager, 3, color, Color::Red));   // New text inherits red
    assert(verifyAttribute(manager, 5, color, Color::Red));   // New text inherits red
    assert(verifyAttribute(manager, 6, color, Color::Red));   // Second part of red
    assert(verifyAttribute(manager, 7, color, Color::Red));   // Second part of red
    assert(verifyAttribute(manager, 8, color, Color::Blue));  // Blue range shifted
    assert(verifyAttribute(manager, 12, color, Color::Blue)); // Blue range shifted

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangePureDeletion() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 10);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto green = pool.add(Color::Green);
    auto blue = pool.add(Color::Blue);

    // Set up initial attributes: [RRRGGGBBB] where R=red, G=green, B=blue
    SBTextSetAttribute(text, 0, 3, color, red);
    SBTextSetAttribute(text, 3, 3, color, green);
    SBTextSetAttribute(text, 6, 3, color, blue);

    // Pure deletion of range 2-7 (overlapping all three ranges)
    SBTextReplaceWithRandomCodeUnits(text, 2, 5, 0);

    // Expected: [RRBB] - deletion removes parts of each range
    assert(verifyAttribute(manager, 0, color, Color::Red));   // First part of red preserved
    assert(verifyAttribute(manager, 1, color, Color::Red));   // First part of red preserved
    assert(verifyAttribute(manager, 2, color, Color::Blue));  // Blue range shifted
    assert(verifyAttribute(manager, 3, color, Color::Blue));  // Blue range shifted

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeBasicReplacement() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 8);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto green = pool.add(Color::Green);

    // Set up initial attributes: [RRRRGGGG] where R=red, G=green
    SBTextSetAttribute(text, 0, 4, color, red);
    SBTextSetAttribute(text, 4, 4, color, green);

    // Replace range 2-6 with 5 new characters
    SBTextReplaceWithRandomCodeUnits(text, 2, 4, 5);

    // Expected: [RR + new5 + GG] where new5 inherits red then green
    assert(verifyAttribute(manager, 0, color, Color::Red));   // First red preserved
    assert(verifyAttribute(manager, 1, color, Color::Red));   // First red preserved
    assert(verifyAttribute(manager, 2, color, Color::Red));   // New text inherits red
    assert(verifyAttribute(manager, 6, color, Color::Red));   // New text inherits red
    assert(verifyAttribute(manager, 7, color, Color::Green)); // Switch to green
    assert(verifyAttribute(manager, 8, color, Color::Green)); // Remaining green

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeAtTextBoundaries() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 5);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto blue = pool.add(Color::Blue);

    // Set attributes: first 3 red, last 2 blue
    SBTextSetAttribute(text, 0, 3, color, red);
    SBTextSetAttribute(text, 3, 2, color, blue);

    // Test replacement at start boundary
    SBTextReplaceWithRandomCodeUnits(text, 0, 2, 4);
    assert(verifyAttribute(manager, 0, color, Color::Red));  // New text inherits red
    assert(verifyAttribute(manager, 3, color, Color::Red));  // New text inherits red
    assert(verifyAttribute(manager, 4, color, Color::Red));  // Old text keeps red
    assert(verifyAttribute(manager, 5, color, Color::Blue)); // Blue range shifted

    // Test replacement at end boundary (appending)
    SBTextReplaceWithRandomCodeUnits(text, 7, 0, 3);
    for (SBUInteger i = 5; i < 10; i++) {
        assert(verifyAttribute(manager, i, color, Color::Blue));
    }

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeWithSingleEntry() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 5);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);

    // Single entry covers everything (red)
    SBTextSetAttribute(text, 0, 5, color, red);

    // Various replacements should maintain single entry
    SBTextReplaceWithRandomCodeUnits(text, 2, 1, 2); // Replace middle
    assert(manager->_entries.count == 1);
    for (SBUInteger i = 0; i < 6; i++) {
        assert(verifyAttribute(manager, i, color, Color::Red));
    }

    SBTextReplaceWithRandomCodeUnits(text, 0, 1, 1); // Replace start
    assert(manager->_entries.count == 1);

    SBTextReplaceWithRandomCodeUnits(text, 5, 1, 1); // Replace end
    assert(manager->_entries.count == 1);

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeSpanningMultipleEntries() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 12);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto green = pool.add(Color::Green);
    auto blue = pool.add(Color::Blue);

    // Create multiple attribute entries: [RRR GGGG BBBBB]
    SBTextSetAttribute(text, 0, 3, color, red);
    SBTextSetAttribute(text, 3, 4, color, green);
    SBTextSetAttribute(text, 7, 5, color, blue);

    // Replace range spanning all entries (2-9)
    SBTextReplaceWithRandomCodeUnits(text, 2, 7, 6);

    // Verify resulting attributes preserve the pattern
    assert(verifyAttribute(manager, 0, color, Color::Red));   // First red preserved
    assert(verifyAttribute(manager, 1, color, Color::Red));   // First red preserved
    assert(verifyAttribute(manager, 2, color, Color::Red));   // New text inherits red
    assert(verifyAttribute(manager, 3, color, Color::Red));   // New text inherits red
    assert(verifyAttribute(manager, 7, color, Color::Red));   // New text inherits red
    assert(verifyAttribute(manager, 8, color, Color::Blue));  // Remaining blue
    assert(verifyAttribute(manager, 10, color, Color::Blue)); // Remaining blue

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeRemovingEntireEntries() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 9);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto green = pool.add(Color::Green);
    auto blue = pool.add(Color::Blue);

    // Create entries: [RRR GGG BBB]
    SBTextSetAttribute(text, 0, 3, color, red);
    SBTextSetAttribute(text, 3, 3, color, green);
    SBTextSetAttribute(text, 6, 3, color, blue);

    // Replace entire middle entry (positions 3-5)
    SBTextReplaceWithRandomCodeUnits(text, 3, 3, 2);

    // New text should inherit from first replaced character (green)
    assert(verifyAttribute(manager, 0, color, Color::Red));   // Red preserved
    assert(verifyAttribute(manager, 2, color, Color::Red));   // Red preserved
    assert(verifyAttribute(manager, 3, color, Color::Green)); // New text gets green
    assert(verifyAttribute(manager, 4, color, Color::Green)); // New text gets green
    assert(verifyAttribute(manager, 5, color, Color::Blue));  // Blue preserved
    assert(verifyAttribute(manager, 7, color, Color::Blue));  // Blue preserved

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeWithParagraphAttributes() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextInsertRandomParagraph(text, 0, 6);  // First paragraph
    SBTextInsertRandomParagraph(text, 6, 6);  // Second paragraph

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto language = SBAttributeRegistryGetAttributeID(registry, AttributeName::Language);

    AttributePool pool;
    auto english = pool.add(Language::English);
    auto urdu = pool.add(Language::Urdu);

    // Set different paragraph attributes
    SBTextSetAttribute(text, 0, 1, language, english); // First paragraph
    SBTextSetAttribute(text, 6, 1, language, urdu);    // Second paragraph

    // Replace text within first paragraph (should not affect paragraph scope)
    SBTextReplaceWithRandomCodeUnits(text, 2, 2, 3);

    // Paragraph attributes should still cover entire paragraphs
    for (SBUInteger i = 0; i < 7; i++) { // First paragraph extended due to insertion
        assert(verifyAttribute(manager, i, language, Language::English));
    }
    for (SBUInteger i = 7; i < 13; i++) { // Second paragraph
        assert(verifyAttribute(manager, i, language, Language::Urdu));
    }

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeParagraphMerging() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextInsertRandomParagraph(text, 0, 6);  // "12345\n"
    SBTextInsertRandomParagraph(text, 6, 6);  // "67890\n"

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto language = SBAttributeRegistryGetAttributeID(registry, AttributeName::Language);
    auto alignment = SBAttributeRegistryGetAttributeID(registry, AttributeName::Alignment);

    AttributePool pool;
    auto english = pool.add(Language::English);
    auto urdu = pool.add(Language::Urdu);
    auto leading = pool.add(Alignment::Leading);
    auto center = pool.add(Alignment::Center);

    // Set different paragraph attributes
    SBTextSetAttribute(text, 0, 1, language, english);
    SBTextSetAttribute(text, 0, 1, alignment, leading);
    SBTextSetAttribute(text, 6, 1, language, urdu);
    SBTextSetAttribute(text, 6, 1, alignment, center);

    // Delete the newline character that separates paragraphs
    SBTextReplaceWithRandomCodeUnits(text, 5, 2, 0); // Delete newline and first char

    // Paragraphs should merge with combined attributes
    for (SBUInteger i = 0; i < 10; i++) {
        // Both language and alignment attributes should be present
        assert(verifyAttribute(manager, i, language, Language::English));
        assert(verifyAttribute(manager, i, alignment, Alignment::Leading));
    }

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeComplexAttributePatterns() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 15);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);
    auto font = SBAttributeRegistryGetAttributeID(registry, AttributeName::Font);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto green = pool.add(Color::Green);
    auto blue = pool.add(Color::Blue);
    auto arial = pool.add(Font::Arial);

    // Complex pattern: [RR FF RR+FF GG BB]
    SBTextSetAttribute(text, 0, 2, color, red);
    SBTextSetAttribute(text, 2, 2, font, arial);
    SBTextSetAttribute(text, 4, 3, color, red);
    SBTextSetAttribute(text, 4, 3, font, arial);
    SBTextSetAttribute(text, 7, 3, color, green);
    SBTextSetAttribute(text, 10, 5, color, blue);

    // Complex replacement overlapping multiple regions
    SBTextReplaceWithRandomCodeUnits(text, 3, 8, 5);

    // Verify the complex result maintains attribute boundaries
    assert(verifyAttribute(manager, 0, color, Color::Red));
    assert(verifyAttribute(manager, 1, color, Color::Red));
    assert(verifyAttribute(manager, 2, font, Font::Arial));
    assert(verifyAttribute(manager, 7, font, Font::Arial));
    assert(verifyAttribute(manager, 8, color, Color::Blue));
    assert(verifyAttribute(manager, 11, color, Color::Blue));

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeMultipleAttributes() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 10);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);
    auto font = SBAttributeRegistryGetAttributeID(registry, AttributeName::Font);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto times = pool.add(Font::Times);

    // Set multiple attributes on same range
    SBTextSetAttribute(text, 0, 10, color, red);
    SBTextSetAttribute(text, 3, 4, font, times);

    // Replace range that overlaps font attribute
    SBTextReplaceWithRandomCodeUnits(text, 4, 2, 3);

    // Both attributes should be preserved
    for (size_t i = 0; i < 11; i++) {
        assert(verifyAttribute(manager, i, color, Color::Red));
    }
    for (size_t i = 3; i < 8; i++) {
        assert(verifyAttribute(manager, 3, font, Font::Times));
    }

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeAttributeInheritance() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 8);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto blue = pool.add(Color::Blue);

    // Test that inserted text inherits correct attributes
    SBTextSetAttribute(text, 0, 4, color, red);
    SBTextSetAttribute(text, 4, 4, color, blue);

    // Insert at boundary between red and blue
    SBTextReplaceWithRandomCodeUnits(text, 4, 0, 2);

    // New text should inherit from left side (red)
    for (size_t i = 0; i < 6; i++) {
        assert(verifyAttribute(manager, i, color, Color::Red));
    }

    // Insert at start of blue range with deletion
    SBTextReplaceWithRandomCodeUnits(text, 6, 2, 3);
    for (size_t i = 6; i < 11; i++) {
        assert(verifyAttribute(manager, i, color, Color::Blue));
    }

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeEdgeCases() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 5);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);

    SBTextSetAttribute(text, 0, 5, color, red);

    // Edge case: replace entire text
    SBTextReplaceWithRandomCodeUnits(text, 0, 5, 3);
    for (SBUInteger i = 0; i < 3; i++) {
        assert(verifyAttribute(manager, i, color, Color::Red));
    }

    // Edge case: replace with same length
    SBTextReplaceWithRandomCodeUnits(text, 1, 1, 1);
    assert(verifyAttribute(manager, 1, color, Color::Red));

    // Edge case: replace zero characters at end (should not crash)
    SBTextReplaceWithRandomCodeUnits(text, 3, 0, 0);

    SBTextRelease(text);
}

void AttributeManagerTests::testReplaceRangeZeroLengthOperations() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 5);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto red = pool.add(Color::Red);

    // Zero-length operations should be handled without crashing
    SBTextReplaceWithRandomCodeUnits(text, 2, 0, 0); // No-op
    SBTextReplaceWithRandomCodeUnits(text, 0, 0, 0); // No-op at start
    SBTextReplaceWithRandomCodeUnits(text, 5, 0, 0); // No-op at end

    // Setting attributes after zero-length ops should work
    SBTextSetAttribute(text, 2, 1, color, red);
    assert(verifyAttribute(manager, 2, color, Color::Red));

    SBTextRelease(text);
}

void AttributeManagerTests::testGetRunByIDBasic() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextInsertRandomParagraph(text, 0, 10);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributePool pool;
    auto purple = pool.add(Color::Purple);

    // Set attribute on first half
    SBTextSetAttribute(text, 0, 5, color, purple);

    AttributeDictionary output;
    AttributeDictionaryInitialize(&output, registry->valueSize);

    SBUInteger startIndex = 0;
    SBUInteger endIndex = 10;

    // First run (0-4 with color)
    assert(AttributeManagerGetOnwardRunByFilteringID(manager, &startIndex, endIndex, color, &output));
    assert(AttributeDictionaryGetCount(&output) == 1);
    assert(startIndex == 5);

    // Second run (5-9 without color)
    assert(AttributeManagerGetOnwardRunByFilteringID(manager, &startIndex, endIndex, color, &output));
    assert(AttributeDictionaryGetCount(&output) == 0);
    assert(startIndex == 10);

    AttributeDictionaryFinalize(&output, nullptr);
    SBTextRelease(text);
}

void AttributeManagerTests::testGetRunByFilteredCollection() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextInsertRandomParagraph(text, 0, 10);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);
    auto font = SBAttributeRegistryGetAttributeID(registry, AttributeName::Font);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto blue = pool.add(Color::Blue);
    auto arial = pool.add(Font::Arial);

    // Set different attributes in different ranges
    SBTextSetAttribute(text, 0, 3, color, red);
    SBTextSetAttribute(text, 0, 3, font, arial);
    SBTextSetAttribute(text, 3, 4, color, blue);

    AttributeDictionary output;
    AttributeDictionaryInitialize(&output, registry->valueSize);

    SBUInteger startIndex = 0;
    SBUInteger endIndex = 10;

    // Get runs filtered by character scope
    assert(AttributeManagerGetOnwardRunByFilteringCollection(manager,
        &startIndex, endIndex, SBAttributeScopeCharacter, SBAttributeGroupNone, &output));

    assert(AttributeDictionaryGetCount(&output) == 2); // color and font
    assert(startIndex == 3);

    AttributeDictionaryFinalize(&output, nullptr);
    SBTextRelease(text);
}

void AttributeManagerTests::testEmptyRuns() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 5);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);

    AttributeDictionary output;
    AttributeDictionaryInitialize(&output, registry->valueSize);

    SBUInteger startIndex = 0;
    SBUInteger endIndex = 5;

    // Should find a run with no attributes
    assert(AttributeManagerGetOnwardRunByFilteringID(manager, &startIndex, endIndex, color, &output));
    assert(AttributeDictionaryGetCount(&output) == 0);
    assert(startIndex == 5);

    AttributeDictionaryFinalize(&output, nullptr);
    SBTextRelease(text);
}

void AttributeManagerTests::testFilterByAttributeGroup() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 10);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);
    auto font = SBAttributeRegistryGetAttributeID(registry, AttributeName::Font);
    auto language = SBAttributeRegistryGetAttributeID(registry, AttributeName::Language);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto arial = pool.add(Font::Arial);
    auto english = pool.add(Language::English);

    // Set attributes from different groups
    SBTextSetAttribute(text, 0, 5, color, red);
    SBTextSetAttribute(text, 0, 5, font, arial);
    SBTextSetAttribute(text, 3, 5, language, english);

    AttributeDictionary output;
    AttributeDictionaryInitialize(&output, registry->valueSize);

    SBUInteger startIndex = 0;
    SBUInteger endIndex = 10;

    // Filter by character group (group 1)
    assert(AttributeManagerGetOnwardRunByFilteringCollection(manager,
        &startIndex, endIndex, SBAttributeScopeCharacter, 1, &output));

    // Should only get color and font (both group 1), not language (group 2)
    assert(startIndex == 5);
    assert(AttributeDictionaryGetCount(&output) == 2);

    AttributeDictionaryFinalize(&output, nullptr);
    SBTextRelease(text);
}

void AttributeManagerTests::testRunBoundariesWithMixedAttributes() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextInsertRandomParagraph(text, 0, 30);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);
    auto font = SBAttributeRegistryGetAttributeID(registry, AttributeName::Font);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto arial = pool.add(Font::Arial);

    // Create a pattern: color-only, both, font-only, none
    SBTextSetAttribute(text, 0, 5, color, red);
    SBTextSetAttribute(text, 5, 5, color, red);
    SBTextSetAttribute(text, 5, 5, font, arial);
    SBTextSetAttribute(text, 10, 5, font, arial);
    // 15-30: no attributes

    AttributeDictionary output;
    AttributeDictionaryInitialize(&output, registry->valueSize);

    SBUInteger startIndex = 0;
    SBUInteger endIndex = 30;

    // Get runs for the color
    assert(AttributeManagerGetOnwardRunByFilteringID(manager, &startIndex, endIndex, color, &output));
    assert(startIndex == 10); // color ends at 10

    // Continue to the next run (no color)
    assert(AttributeManagerGetOnwardRunByFilteringID(manager, &startIndex, endIndex, color, &output));
    assert(startIndex == 30); // no color from 10-30

    AttributeDictionaryFinalize(&output, nullptr);
    SBTextRelease(text);
}

void AttributeManagerTests::testComplexRunDetectionScenarios() {
    auto text = SBTextCreateWithDefaultRegistry();
    SBTextAppendRandomCodeUnits(text, 20);

    auto manager = &text->attributeManager;
    auto registry = manager->_registry;
    auto color = SBAttributeRegistryGetAttributeID(registry, AttributeName::Color);
    auto font = SBAttributeRegistryGetAttributeID(registry, AttributeName::Font);

    AttributePool pool;
    auto red = pool.add(Color::Red);
    auto blue = pool.add(Color::Blue);
    auto arial = pool.add(Font::Arial);
    auto times = pool.add(Font::Times);

    // Create a complex pattern: A-B-A-B where A=color, B=font, AB=both
    SBTextSetAttribute(text, 0, 5, color, red);   // 0-4: color
    SBTextSetAttribute(text, 5, 5, font, arial);  // 5-9: font
    SBTextSetAttribute(text, 10, 5, color, blue); // 10-14: color
    SBTextSetAttribute(text, 10, 5, font, arial); // 10-14: both
    SBTextSetAttribute(text, 15, 5, font, times); // 15-19: font

    AttributeDictionary output;
    AttributeDictionaryInitialize(&output, text->attributeRegistry->valueSize);

    SBUInteger startIndex = 0;

    // Test run detection with mixed attributes
    AttributeManagerGetOnwardRunByFilteringCollection(manager,
        &startIndex, 20, SBAttributeScopeCharacter, SBAttributeGroupNone, &output);
    assert(startIndex == 5); // First run: positions 0-4 (color only)

    AttributeManagerGetOnwardRunByFilteringCollection(manager,
        &startIndex, 20, SBAttributeScopeCharacter, SBAttributeGroupNone, &output);
    assert(startIndex == 10); // Second run: positions 5-9 (font only)

    AttributeManagerGetOnwardRunByFilteringCollection(manager,
        &startIndex, 20, SBAttributeScopeCharacter, SBAttributeGroupNone, &output);
    assert(startIndex == 15); // Third run: positions 10-14 (both color and font)

    AttributeManagerGetOnwardRunByFilteringCollection(manager,
        &startIndex, 20, SBAttributeScopeCharacter, SBAttributeGroupNone, &output);
    assert(startIndex == 20); // Fourth run: positions 15-19 (font only)

    AttributeDictionaryFinalize(&output, nullptr);
    SBTextRelease(text);
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    AttributeManagerTests attributeStorageTests;
    attributeStorageTests.run();

    return 0;
}

#endif
