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

#include <API/SBBase.h>

#if SB_TEXT_API_SUPPORTED

#include <stddef.h>

#include <API/SBAttributeRegistry.h>
#include <Core/Object.h>

#include "SBTextConfig.h"

static void FinalizeTextConfig(ObjectRef object)
{
    SBTextConfigRef config = object;

    if (config->attributeRegistry) {
        SBAttributeRegistryRelease(config->attributeRegistry);
    }
}

SBTextConfigRef SBTextConfigCreate(void)
{
    const SBUInteger size = sizeof(SBTextConfig);
    void *pointer = NULL;
    SBTextConfigRef config;

    config = ObjectCreate(&size, 1, &pointer, &FinalizeTextConfig);

    if (config) {
        config->attributeRegistry = NULL;
        config->baseLevel = SBLevelDefaultLTR;
    }

    return config;
}

void SBTextConfigSetAttributeRegistry(SBTextConfigRef config,
    SBAttributeRegistryRef attributeRegistry)
{
    if (config->attributeRegistry) {
        SBAttributeRegistryRelease(config->attributeRegistry);
        config->attributeRegistry = NULL;
    }

    if (attributeRegistry) {
        config->attributeRegistry = SBAttributeRegistryRetain(attributeRegistry);
    }
}

void SBTextConfigSetBaseLevel(SBTextConfigRef config, SBLevel baseLevel)
{
    config->baseLevel = baseLevel;
}

SBTextConfigRef SBTextConfigRetain(SBTextConfigRef config)
{
    return ObjectRetain((ObjectRef)config);
}

void SBTextConfigRelease(SBTextConfigRef config)
{
    ObjectRelease((ObjectRef)config);
}

#endif
