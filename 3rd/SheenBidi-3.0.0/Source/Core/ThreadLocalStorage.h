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

#ifndef _SB_INTERNAL_THREAD_LOCAL_STORAGE_H
#define _SB_INTERNAL_THREAD_LOCAL_STORAGE_H

#include <stddef.h>

#include <API/SBBase.h>

/* Detect C11 support */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
#define USE_C11_THREAD_LOCAL

/* Detect GCC thread-local support (available since GCC 3.3) */
#elif GCC_VERSION >= 303
#define USE_GNUC_THREAD_LOCAL

/* Detect Clang thread-local support (available since Clang 1.0) */
#elif CLANG_VERSION >= 100
#define USE_CLANG_THREAD_LOCAL

/* Detect MSVC thread-local support (available since MSVC 2005) */
#elif MSVC_VERSION >= 1400
#define USE_MSVC_THREAD_LOCAL

/* Check for pthread availability */
#elif defined(_POSIX_THREADS) || defined(__unix__) || defined(__unix) || defined(unix) \
        || (defined(__APPLE__) && defined(__MACH__)) || defined(__linux__)
#define USE_PTHREAD_KEY

/* Check for Windows availability */
#elif defined(_WIN32)
#define USE_WIN_TLS

#endif

/* Define data type for thread local storage. */
#ifdef USE_C11_THREAD_LOCAL

#define HAS_TLS_SUPPORT
#define ThreadLocalStorage _Thread_local void *

#elif defined(USE_GNUC_THREAD_LOCAL) || defined(USE_CLANG_THREAD_LOCAL)

#define HAS_TLS_SUPPORT
#define ThreadLocalStorage __thread void *

#elif defined(USE_MSVC_THREAD_LOCAL)

#define HAS_TLS_SUPPORT
#define ThreadLocalStorage __declspec(thread) void *

#elif defined(USE_PTHREAD_KEY)

#include <pthread.h>
#define HAS_TLS_SUPPORT
typedef pthread_key_t ThreadLocalStorage;

#elif defined(USE_WIN_TLS)

#include <windows.h>
#define HAS_TLS_SUPPORT
typedef DWORD ThreadLocalStorage;

#else

typedef void *ThreadLocalStorage;

#endif

/* Define functions for thread local storage. */
#ifdef USE_PTHREAD_KEY

#define ThreadLocalStorageInitialize(tls)   pthread_key_create(&(tls), NULL)
#define ThreadLocalStorageGet(tls)          pthread_getspecific(tls)
#define ThreadLocalStorageSet(tls, value)   pthread_setspecific(tls, value)
#define ThreadLocalStorageFinalize(tls)     pthread_key_delete(tls)

#elif defined(USE_WIN_TLS)

#define ThreadLocalStorageInitialize(tls)   ((tls) = TlsAlloc())
#define ThreadLocalStorageGet(tls)          TlsGetValue(tls)
#define ThreadLocalStorageSet(tls, value)   TlsSetValue(tls, value)
#define ThreadLocalStorageFinalize(tls)     TlsFree(tls)

#else

#define ThreadLocalStorageInitialize(tls)   ((tls) = NULL)
#define ThreadLocalStorageGet(tls)          (tls)
#define ThreadLocalStorageSet(tls, value)   ((tls) = (value))
#define ThreadLocalStorageFinalize(tls)

#endif

#endif
