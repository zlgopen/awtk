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

#ifndef _SB_PUBLIC_TEXT_CONFIG_H
#define _SB_PUBLIC_TEXT_CONFIG_H

#include <SheenBidi/SBAttributeRegistry.h>
#include <SheenBidi/SBBase.h>

#if SB_TEXT_API_SUPPORTED

SB_EXTERN_C_BEGIN

/**
 * Opaque reference to a configuration object required for text creation.
 */
typedef struct _SBTextConfig *SBTextConfigRef;

/**
 * Creates an empty text config instance.
 * 
 * @return
 *      A reference to the text config instance, or `NULL` on failure.
 * @note
 *      Must be populated with an attribute registry before text creation.
 */
SB_PUBLIC SBTextConfigRef SBTextConfigCreate(void);

/**
 * Sets the attribute registry used by any text created with this config.
 * 
 * @param config
 *      The text config object.
 * @param attributeRegistry
 *      Attribute registry reference; the text config retains it.
 */
SB_PUBLIC void SBTextConfigSetAttributeRegistry(SBTextConfigRef config,
    SBAttributeRegistryRef attributeRegistry);

/**
 * Sets the base paragraph level policy for newly created texts.
 * 
 * @param config
 *      The text config object.
 * @param baseLevel
 *      `SBLevelDefaultLTR`, `SBLevelDefaultRTL`, or a concrete level.
 */
SB_PUBLIC void SBTextConfigSetBaseLevel(SBTextConfigRef config, SBLevel baseLevel);

SB_PUBLIC SBTextConfigRef SBTextConfigRetain(SBTextConfigRef config);

SB_PUBLIC void SBTextConfigRelease(SBTextConfigRef config);

SB_EXTERN_C_END

#endif

#endif
