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

#ifndef _SB_INTERNAL_THREAD_FENCE_H
#define _SB_INTERNAL_THREAD_FENCE_H

#include <API/SBBase.h>

#if defined(USE_C11_ATOMICS)

#include <stdatomic.h>
#define HAS_THREAD_FENCE_SUPPORT
#define ThreadFence()               atomic_thread_fence(memory_order_seq_cst)

#elif defined(USE_ATOMIC_BUILTINS)

#define HAS_THREAD_FENCE_SUPPORT
#define ThreadFence()               __atomic_thread_fence(__ATOMIC_SEQ_CST)

#elif defined(USE_SYNC_BUILTINS)

#define HAS_THREAD_FENCE_SUPPORT
#define ThreadFence()               __sync_synchronize()

#elif MSVC_VERSION >= 1400

#include <windows.h>
#define HAS_THREAD_FENCE_SUPPORT
#define ThreadFence()               MemoryBarrier()

#else /* Unsafe fallback — no barrier */

#define ThreadFence()

#endif

#endif
