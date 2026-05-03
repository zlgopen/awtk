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

#ifndef _SB_INTERNAL_ATOMIC_FLAG_H
#define _SB_INTERNAL_ATOMIC_FLAG_H

#include <API/SBBase.h>

/* Define data type for atomic flag. */
#ifdef USE_C11_ATOMICS

#include <stdatomic.h>
#define HAS_ATOMIC_FLAG_SUPPORT
typedef atomic_flag AtomicFlag;

#elif defined(USE_ATOMIC_BUILTINS) || defined(USE_SYNC_BUILTINS)

#define HAS_ATOMIC_FLAG_SUPPORT
typedef SBBoolean AtomicFlag;

#elif defined(USE_WIN_INTRINSICS)

#include <intrin.h>
#define HAS_ATOMIC_FLAG_SUPPORT
#pragma intrinsic(_InterlockedExchange8)
typedef volatile char AtomicFlag;

#elif defined(USE_WIN_INTERLOCKED)

#include <windows.h>
#define HAS_ATOMIC_FLAG_SUPPORT
typedef volatile LONG AtomicFlag;

#else

typedef SBBoolean AtomicFlag;

#endif

typedef AtomicFlag *AtomicFlagRef;

/* Define functions for atomic flag. */
#if defined(USE_C11_ATOMICS)

#define AtomicFlagMake()            ATOMIC_FLAG_INIT
#define AtomicFlagTestAndSet(flag)  ((SBBoolean)atomic_flag_test_and_set(flag))
#define AtomicFlagClear(flag)       atomic_flag_clear(flag)

#elif defined(USE_ATOMIC_BUILTINS)

#define AtomicFlagMake()            SBFalse
#define AtomicFlagTestAndSet(flag)  __atomic_test_and_set(flag, __ATOMIC_SEQ_CST)
#define AtomicFlagClear(flag)       __atomic_clear(flag, __ATOMIC_SEQ_CST)

#elif defined(USE_SYNC_BUILTINS)

#define AtomicFlagMake()            SBFalse
#define AtomicFlagTestAndSet(flag)  __sync_lock_test_and_set(flag, SBTrue)
#define AtomicFlagClear(flag)       __sync_lock_release(flag)

#elif defined(USE_WIN_INTRINSICS)

#define AtomicFlagMake()            0
#define AtomicFlagTestAndSet(flag)  (_InterlockedExchange8(flag, 1) == 1)
#define AtomicFlagClear(flag)       _InterlockedExchange8(flag, 0)

#elif defined(USE_WIN_INTERLOCKED)

#define AtomicFlagMake()            0
#define AtomicFlagTestAndSet(flag)  (InterlockedExchange(flag, 1) == 1)
#define AtomicFlagClear(flag)       InterlockedExchange(flag, 0)

#else /* Non-atomic fallback */

#define AtomicFlagMake()            SBFalse
#define AtomicFlagTestAndSet(flag)  (*(flag) ? SBTrue : (*(flag) = SBTrue, SBFalse))
#define AtomicFlagClear(flag)       (*(flag) = SBFalse)

#endif

#endif
