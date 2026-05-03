/*
 * Copyright (C) 2016-2025 Muhammad Tayyab Akram
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

#ifndef _SB_INTERNAL_BASE_H
#define _SB_INTERNAL_BASE_H

#include <SheenBidi/SBBase.h>
#include <SheenBidi/SBBidiType.h>
#include <SheenBidi/SBConfig.h>
#include <SheenBidi/SBGeneralCategory.h>
#include <SheenBidi/SBScript.h>

#ifdef SB_CONFIG_UNITY
#define SB_INTERNAL static
#else
#define SB_INTERNAL
#endif

/* ---------- Compiler Version Detection ---------- */

/* Detect GCC and its version */
#if defined(__GNUC__) && !defined(__clang__)
#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#else
#define GCC_VERSION     0
#endif

/* Detect Clang and its version */
#if defined(__clang__)
#define CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)
#else
#define CLANG_VERSION   0
#endif

/* Detect MSVC and its version */
#if defined(_MSC_VER)
#define MSVC_VERSION    _MSC_VER
#else
#define MSVC_VERSION    0
#endif

/* ---------- Atomic Support Feature Detection ---------- */

/* Prefer C11 atomics if available */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
#define USE_C11_ATOMICS

/* Fallback to compiler intrinsics for GCC/Clang */
#elif defined(__GNUC__) || defined(__clang__)
/* Modern __atomic builtins (GCC 4.7+, Clang 3.1+) */
#if GCC_VERSION >= 407 || CLANG_VERSION >= 301
#define USE_ATOMIC_BUILTINS
/* Legacy __sync builtins (GCC 4.1+, Clang 1.0+) */
#elif GCC_VERSION >= 401 || CLANG_VERSION >= 100
#define USE_SYNC_BUILTINS
#endif

/* Windows-specific intrinsics */
#elif defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__)

#if MSVC_VERSION >= 1700    /* VS 2012+ */
#define USE_WIN_INTRINSICS
#else
#define USE_WIN_INTERLOCKED
#endif

#endif


/**
 * A value that indicates an invalid unsigned index.
 */
#define SBInvalidIndex  (SBUInteger)(-1)

SB_INTERNAL void SBUIntegerNormalizeRange(SBUInteger actualLength,
    SBUInteger *rangeOffset, SBUInteger *rangeLength);

SB_INTERNAL SBBoolean SBUIntegerVerifyRange(SBUInteger actualLength,
    SBUInteger rangeOffset, SBUInteger rangeLength);


#define SBNumberGetMax(first, second)           \
(                                               \
   (first) > (second)                           \
 ? (first)                                      \
 : (second)                                     \
)

#define SBNumberLimitIncrement(number, limit)   \
(                                               \
   (number) < (limit)                           \
 ? (number) + (1)                               \
 : (limit)                                      \
)

#define SBNumberLimitDecrement(number, limit)   \
(                                               \
   (number) > (limit)                           \
 ? (number) - (1)                               \
 : (limit)                                      \
)

#define SBNumberRingAdd(number, count, capacity) \
    (((number) + (count)) % (capacity))

#define SBNumberRingIncrement(number, capacity) \
    SBNumberRingAdd(number, 1, capacity)

#define SBNumberRingSubtract(number, count, capacity) \
    (((number) + (capacity) - (count)) % (capacity))

#define SBNumberRingDecrement(number, capacity) \
    SBNumberRingSubtract(number, 1, capacity)


#define SBLevelAsNormalBidiType(level)      \
(                                           \
   ((level) & 1)                            \
 ? SBBidiTypeR                              \
 : SBBidiTypeL                              \
)

#define SBLevelAsOppositeBidiType(level)    \
(                                           \
   ((level) & 1)                            \
 ? SBBidiTypeL                              \
 : SBBidiTypeR                              \
)


#define SBBidiTypeIsEqual(t1, t2)           ((t1) == (t2))

#define SBBidiTypeIsNumber(t)               SBUInt8InRange(t, SBBidiTypeAN, SBBidiTypeEN)
#define SBBidiTypeIsIsolate(t)              SBUInt8InRange(t, SBBidiTypeLRI, SBBidiTypePDI)

#define SBBidiTypeIsStrongOrNumber(t)       (SBBidiTypeIsStrong(t) || SBBidiTypeIsNumber(t))
#define SBBidiTypeIsNumberSeparator(t)      SBUInt8InRange(t, SBBidiTypeES, SBBidiTypeCS)
#define SBBidiTypeIsIsolateInitiator(t)     SBUInt8InRange(t, SBBidiTypeLRI, SBBidiTypeFSI)
#define SBBidiTypeIsIsolateTerminator(t)    SBBidiTypeIsEqual(t, SBBidiTypePDI)
#define SBBidiTypeIsNeutralOrIsolate(t)     SBUInt8InRange(t, SBBidiTypeWS, SBBidiTypePDI)

#define SBScriptIsCommonOrInherited(s)      ((s) <= SBScriptZYYY)

#endif
