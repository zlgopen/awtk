/*
 * Copyright (C) 2014-2026 Muhammad Tayyab Akram
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

#ifndef _SB_PUBLIC_CONFIG_H
#define _SB_PUBLIC_CONFIG_H

/* #define SB_CONFIG_DLL_EXPORT */
/* #define SB_CONFIG_DLL_IMPORT */
/* #define SB_CONFIG_LOG */
/* #define SB_CONFIG_UNITY */
/* #define SB_CONFIG_ALLOW_NON_ATOMIC_FALLBACK */

/**
 * Disable scratch memory functionality of default allocator.
 * When defined, all scratch memory operations will return NULL/do nothing.
 */
/* #define SB_CONFIG_DISABLE_SCRATCH_MEMORY */

/**
 * Enables the optional text editing and analysis API, including support for inserting, removing,
 * and modifying code units, applying attributes, and querying logical, script, attribute, and
 * visual runs. These features perform incremental paragraph analysis internally.
 *
 * The API is considered experimental and may change based on production feedback.
 */
/* #define SB_CONFIG_EXPERIMENTAL_TEXT_API */

/**
 * Define the size of each scratch buffer in bytes.
 * Default is 8192 bytes (8KB) if not specified.
 */
#ifndef SB_CONFIG_SCRATCH_BUFFER_SIZE
#define SB_CONFIG_SCRATCH_BUFFER_SIZE 8192
#endif

/**
 * Define the number of scratch buffers in the pool.
 * Default is 3 buffers if not specified.
 */
#ifndef SB_CONFIG_SCRATCH_POOL_SIZE
#define SB_CONFIG_SCRATCH_POOL_SIZE 3
#endif

#endif
