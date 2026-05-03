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
#include <cassert>
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <SheenBidi/SBBidiType.h>
#include <SheenBidi/SBConfig.h>

extern "C" {
#include <Core/Memory.h>
#include <Data/PairingLookup.h>
#include <UBA/BidiChain.h>
#include <UBA/BracketQueue.h>
}

#include "BracketQueueTests.h"

using namespace std;
using namespace SheenBidi;

static constexpr size_t MaxDepth = 63;

static bool isOpeningBracket(char32_t c) {
    static const unordered_set<char32_t> OpeningBrackets = {
        U'(', U'[', U'{'
    };

    return OpeningBrackets.find(c) != OpeningBrackets.end();
}

static bool isClosingBracket(char32_t c) {
    static const unordered_set<char32_t> ClosingBrackets = {
        U')', U']', U'}'
    };

    return ClosingBrackets.find(c) != ClosingBrackets.end();
}

static char32_t getPairedBracket(char32_t c) {
    static const unordered_map<char32_t, char32_t> BracketPairs = {
        {U'(', U')'}, {U')', U'('}, {U'[', U']'}, {U']', U'['}, 
        {U'{', U'}'}, {U'}', U'{'}
    };

    auto it = BracketPairs.find(c);
    if (it == BracketPairs.end()) {
        return U'\0';
    }

    return it->second;
}

static vector<pair<size_t, size_t>> findReferenceBracketPairs(const u32string &text) {
    struct StackElement {
        char32_t bracket;
        size_t index;
    };

    vector<StackElement> stack;
    vector<pair<size_t, size_t>> pairs;

    for (size_t i = 0; i < text.size(); ++i) {
        auto ch = text[i];

        if (isOpeningBracket(ch)) {
            if (stack.size() >= MaxDepth) {
                return {};  // Stack overflow, abort processing
            }

            auto pairedBracket = getPairedBracket(ch);
            stack.push_back({pairedBracket, i});
        } else if (isClosingBracket(ch)) {
            if (stack.empty()) {
                continue;  // No opening bracket to match with
            }

            auto top = stack.size();
            while (top-- > 0) {
                const auto &element = stack[top];
                if (element.bracket == ch) {
                    pairs.emplace_back(element.index, i);
                    stack.resize(top);  // Pop through this element inclusively
                    break;
                }
            }
        }
    }

    sort(pairs.begin(), pairs.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

    return pairs;
}

static vector<pair<size_t, size_t>> findActualBracketPairs(const u32string &text) {
    Memory memory;
    BracketQueue queue;

    MemoryInitialize(&memory);
    BracketQueueInitialize(&queue, &memory);
    BracketQueueReset(&queue, SBBidiTypeL);

    vector<pair<size_t, size_t>> pairs;

    for (size_t i = 0; i < text.size(); i++) {
        auto ch = text[i];

        BracketType type;
        auto pair = LookupBracketPair(ch, &type);

        if (type == BracketTypeOpen) {
            auto priorStrongLink = BidiLinkNone;
            auto openingLink = static_cast<BidiLink>(i);

            if (BracketQueueGetOpenPairCount(&queue) >= MaxDepth) {
                break;
            }

            BracketQueueEnqueue(&queue, priorStrongLink, openingLink, pair);
        } else if (type == BracketTypeClose) {
            auto closingLink = static_cast<BidiLink>(i);
            BracketQueueClosePair(&queue, closingLink, ch);
        }
    }

    BracketQueueMarkPopulated(&queue);

    while (queue.pairCount > 0) {
        auto openingLink = BracketQueueGetOpeningLink(&queue);
        auto closingLink = BracketQueueGetClosingLink(&queue);

        pairs.emplace_back(openingLink, closingLink);

        BracketQueueDequeue(&queue);
    }

    MemoryFinalize(&memory);

    return pairs;
}

static void testBracketPairs(const u32string &text) {
    auto referencePairs = findReferenceBracketPairs(text);
    auto actualPairs = findActualBracketPairs(text);

    assert(referencePairs.size() == actualPairs.size());

    for (size_t i = 0; i < referencePairs.size(); i++) {
        assert(referencePairs[i].first == actualPairs[i].first);
        assert(referencePairs[i].second == actualPairs[i].second);
    }
}

static void testBracketPairs(const function<u32string()> &text) {
    testBracketPairs(text());
}

void BracketQueueTests::run() {
    // No brackets
    testBracketPairs(U"abc");
    // Single pair
    testBracketPairs(U"a(b)c");
    // Mismatched brackets
    testBracketPairs(U"a(b]c");
    // Nested pairs
    testBracketPairs(U"a(b(c)d)e");
    // Overlapping brackets
    testBracketPairs(U"a(b[c)d]");
    // Depth limit with opening brackets
    testBracketPairs([]() -> u32string {
        u32string sample;
        for (size_t i = 0; i <= MaxDepth; i++) {
            sample += U'(';
        }
        return sample;
    });
    // Complex nesting
    testBracketPairs(U"(a[b{c}d]e)");
    // Unmatched closing bracket
    testBracketPairs(U"a)b(c");
    // Only opening brackets
    testBracketPairs(U"(((");
    // Only closing brackets
    testBracketPairs(U")))");
    // Mixed brackets with no pairs
    testBracketPairs(U"(]}{[)");
    // Deeply nested brackets
    testBracketPairs([]() -> u32string {
        u32string text;
        for (size_t i = 0; i < MaxDepth; i++) {
            text += U'(';
        }
        text += U'x';
        for (size_t i = 0; i < MaxDepth; i++) {
            text += U')';
        }
        return text;
    });
    // Depth limit with nested brackets
    testBracketPairs([]() -> u32string {
        u32string text;
        for (size_t i = 0; i <= MaxDepth; i++) {
            text += U'(';
        }
        text += U'x';
        for (size_t i = 0; i <= MaxDepth; i++) {
            text += U')';
        }
        return text;
    });
    // Multiple different type separate pairs
    testBracketPairs(U"(a)[b]{c}");
    // Multiple same type pairs
    testBracketPairs(U"(a)(b)(c)");
    // Multiple pairs with text between
    testBracketPairs(U"(a) text [b] more {c}");
    // Multiple separate nested pairs
    testBracketPairs(U"(a[b]c) [d{e}f]");
    // Multiple pairs with mixed nesting
    testBracketPairs(U"(a[b{c}d]e) [f{g(h)i}j]");
    // Multiple pairs with unmatched in between
    testBracketPairs(U"(a) ] [ (b)");
    // Multiple pairs at stack limit
    testBracketPairs([]() -> u32string {
        u32string text;
        for (size_t i = 0; i < MaxDepth; i++) {
            text += U"(x)";
        }
        return text;
    });
    // Multiple pairs four times the stack limit
    testBracketPairs([]() -> u32string {
        auto pairCount = MaxDepth * 4;
        u32string text;
        for (size_t i = 0; i < pairCount; i++) {
            text += U"(x)";
        }
        return text;
    });
    // Multiple pairs with different depths
    testBracketPairs(U"(a[b(c)d]e) {f[g{h}i]j}");
    // Multiple pairs with weird nesting
    testBracketPairs(U"(a[b]c{d}e[f]g)");
}

#ifdef STANDALONE_TESTING

int main(int argc, const char *argv[]) {
    BracketQueueTests bracketQueueTests;
    bracketQueueTests.run();

    return 0;
}

#endif
