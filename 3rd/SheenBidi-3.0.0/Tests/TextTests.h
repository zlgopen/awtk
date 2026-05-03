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

#ifndef _SHEENBIDI__TEXT_TESTS_H
#define _SHEENBIDI__TEXT_TESTS_H

namespace SheenBidi {

class TextTests {
public:
    TextTests() = default;

    void run();

private:
    void testCreateImmutableText();
    void testCreateEmptyMutableText();
    void testCreateCopyWithDifferentEncodings();
    void testCreateImmutableCopy();
    void testCreateMutableCopy();
    void testGetCodeUnits();
    void testAppendCodeUnits();
    void testInsertCodeUnits();
    void testDeleteCodeUnits();
    void testReplaceCodeUnits();
    void testSetCodeUnits();
    void testGetBidiTypes();
    void testGetScripts();
    void testGetResolvedLevels();
    void testGetCodeUnitParagraphInfo();
    void testIterators();
    void testEditingSession();
    void testEdgeCases();
    void testInvalidOperations();
    void testReferenceCounting();
    void testDifferentBaseLevels();
    void testComplexBidirectionalText();
    void testParagraphScenarios();
    void testSetAttribute();
    void testRemoveAttribute();
    void testAttributeEdgeCases();
    void testAttributeComplexScenarios();
};

}

#endif
