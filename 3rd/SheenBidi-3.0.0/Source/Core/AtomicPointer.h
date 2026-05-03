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

#ifndef _SB_INTERNAL_ATOMIC_POINTER_H
#define _SB_INTERNAL_ATOMIC_POINTER_H

#include <API/SBBase.h>

/* Define data type for atomic pointer. */
#ifdef USE_C11_ATOMICS

#include <stdatomic.h>
#define HAS_ATOMIC_POINTER_SUPPORT
typedef _Atomic(void *) AtomicPointer;
#define AtomicPointerType(type) _Atomic(type *)

#elif defined(USE_ATOMIC_BUILTINS) || defined(USE_SYNC_BUILTINS)

#define HAS_ATOMIC_POINTER_SUPPORT
typedef void *AtomicPointer;
#define AtomicPointerType(type) type *

#elif defined(USE_WIN_INTRINSICS)

#include <intrin.h>
#define HAS_ATOMIC_POINTER_SUPPORT
#pragma intrinsic(_InterlockedExchangePointer, _InterlockedCompareExchangePointer)
typedef void * volatile AtomicPointer;
#define AtomicPointerType(type) type * volatile

#elif defined(USE_WIN_INTERLOCKED)

#include <windows.h>
#define HAS_ATOMIC_POINTER_SUPPORT
typedef void * volatile AtomicPointer;
#define AtomicPointerType(type) type * volatile

#else

typedef void *AtomicPointer;
#define AtomicPointerType(type) type *

#endif

typedef AtomicPointer *AtomicPointerRef;

/* Define functions for atomic pointer. */
#if defined(USE_C11_ATOMICS)

#define AtomicPointerLoad(pointer)                              \
    atomic_load(pointer)
#define AtomicPointerStore(pointer, value)                      \
    atomic_store(pointer, value)
#define AtomicPointerCompareAndSet(pointer, expected, desired)  \
    atomic_compare_exchange_strong(pointer, expected, desired)

#elif defined(USE_ATOMIC_BUILTINS)

#define AtomicPointerLoad(pointer)                              \
    __atomic_load_n(pointer, __ATOMIC_SEQ_CST)
#define AtomicPointerStore(pointer, value)                      \
    __atomic_store_n(pointer, value, __ATOMIC_SEQ_CST)
#define AtomicPointerCompareAndSet(pointer, expected, desired)  \
    __atomic_compare_exchange_n(pointer, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)

#elif defined(USE_SYNC_BUILTINS)

#define AtomicPointerLoad(pointer)                              \
    __sync_fetch_and_add(pointer, 0)
#define AtomicPointerStore(pointer, value)                      \
    __sync_lock_test_and_set(pointer, value)
#define AtomicPointerCompareAndSet(pointer, expected, desired)  \
    __sync_bool_compare_and_swap(pointer, *(expected), desired)

#elif defined(USE_WIN_INTRINSICS)

#define AtomicPointerLoad(pointer)                              \
    _InterlockedCompareExchangePointer((AtomicPointerRef)(pointer), NULL, NULL)
#define AtomicPointerStore(pointer, value)                      \
    _InterlockedExchangePointer((AtomicPointerRef)(pointer), (void *)(value))
#define AtomicPointerCompareAndSet(pointer, expected, desired)  \
    (_InterlockedCompareExchangePointer((AtomicPointerRef)(pointer), (void *)(desired), (void *)(*(expected))) == *(expected))

#elif defined(USE_WIN_INTERLOCKED)

#ifdef _WIN64
#define AtomicPointerLoad(pointer)                              \
    ((void *)InterlockedCompareExchange64((LONG64 volatile *)(pointer), 0, 0))
#define AtomicPointerStore(pointer, value)                      \
    InterlockedExchange64((LONG64 volatile *)(pointer), (LONG64)(value))
#define AtomicPointerCompareAndSet(pointer, expected, desired)  \
    (((void *)InterlockedCompareExchange64((LONG64 volatile *)(pointer), (LONG64)(desired), (LONG64)(*(expected)))) == *(expected))
#else
#define AtomicPointerLoad(pointer)                              \
    ((void *)InterlockedCompareExchange((LONG volatile *)(pointer), 0, 0))
#define AtomicPointerStore(pointer, value)                      \
    InterlockedExchange((LONG volatile *)(pointer), (LONG)(value))
#define AtomicPointerCompareAndSet(pointer, expected, desired)  \
    (((void *)InterlockedCompareExchange((LONG volatile *)(pointer), (LONG)(desired), (LONG)(*(expected)))) == *(expected))
#endif

#else /* Non-atomic fallback */

#define AtomicPointerLoad(pointer)                              \
    (*(pointer))
#define AtomicPointerStore(pointer, value)                      \
    (*(pointer) = (value))
#define AtomicPointerCompareAndSet(pointer, expected, desired)  \
    ((*(pointer) == *(expected)) ? ((*(pointer) = (desired)), SBTrue) : SBFalse)

#endif

#endif
