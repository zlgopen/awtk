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
#include <stdlib.h>
#include <string.h>

#include <API/SBAttributeInfo.h>
#include <Core/Object.h>

#include "SBAttributeRegistry.h"

static int AttributeInfoComparison(const void *first, const void *second)
{
    const SBAttributeInfo *info1 = first;
    const SBAttributeInfo *info2 = second;

    return strcmp(info1->name, info2->name);
}

static int AttributeNameComparison(const void *key, const void *array)
{
    const char *name = key;
    const SBAttributeInfo *attributeInfo = array;

    return strcmp(name, attributeInfo->name);
}

static SBUInteger CountCombinedNamesLength(
    const SBAttributeInfo *attributeInfos, SBUInteger attributeCount)
{
    SBUInteger combinedLength = 0;
    SBUInteger index;

    for (index = 0; index < attributeCount; index++) {
        const SBAttributeInfo *currentInfo = &attributeInfos[index];
        SBUInteger nameLength = strlen(currentInfo->name) + 1;
        combinedLength += nameLength;
    }

    return combinedLength;
}

#define ATTRIBUTE_REGISTRY  0
#define ATTRIBUTE_INFOS     1
#define ATTRIBUTE_NAMES     2
#define COUNT               3

static SBAttributeRegistry *AllocateAttributeRegistry(
    SBUInteger attributeCount, SBUInteger namesLength, char **namesPointer)
{
    void *pointers[COUNT] = { NULL };
    SBUInteger sizes[COUNT] = { 0 };
    SBAttributeRegistry *attributeRegistry;

    sizes[ATTRIBUTE_REGISTRY] = sizeof(SBAttributeRegistry);
    sizes[ATTRIBUTE_INFOS]    = sizeof(SBAttributeInfo) * attributeCount;
    sizes[ATTRIBUTE_NAMES]    = namesLength;

    attributeRegistry = ObjectCreate(sizes, COUNT, pointers, NULL);

    if (attributeRegistry) {
        attributeRegistry->attributeInfos = pointers[ATTRIBUTE_INFOS];
        *namesPointer = pointers[ATTRIBUTE_NAMES];
    }

    return attributeRegistry;
}

#undef ATTRIBUTE_REGISTRY
#undef ATTRIBUTE_INFOS
#undef ATTRIBUTE_NAMES
#undef COUNT

static SBAttributeInfo *GetAttributeInfoForID(SBAttributeRegistryRef registry, SBAttributeID id)
{
    SBUInteger index = SBAttributeIDGetIndex(id);

    if (index < registry->count) {
        return &registry->attributeInfos[index];
    }

    return NULL;
}

SB_INTERNAL const void *SBAttributeRegistryRetainAttribute(SBAttributeRegistryRef registry,
    const void *attributeValue)
{
    SBAttributeValueRetainCallback retain = registry->_valueCallbacks.retain;

    if (retain) {
        return retain(attributeValue);
    }

    return attributeValue;
}

SB_INTERNAL void SBAttributeRegistryReleaseAttribute(SBAttributeRegistryRef registry,
    const void *attributeValue)
{
    SBAttributeValueReleaseCallback release = registry->_valueCallbacks.release;

    if (release) {
        release(attributeValue);
    }
}

SB_INTERNAL SBBoolean SBAttributeRegistryIsEqualAttribute(SBAttributeRegistryRef registry,
    SBAttributeID attributeID, const void *first, const void *second)
{
    SBAttributeValueEqualCallback equal = registry->_valueCallbacks.equal;

    if (equal) {
        return equal(first, second);
    }

    return SBAttributeItemIsEqualValue(attributeID, first, second);
}

SB_INTERNAL const SBAttributeInfo *SBAttributeRegistryGetInfoReference(
    SBAttributeRegistryRef registry, SBAttributeID attributeID)
{
    return GetAttributeInfoForID(registry, attributeID);
}

SBAttributeRegistryRef SBAttributeRegistryCreate(const SBAttributeInfo *attributeInfos,
    SBUInteger count, SBUInt8 valueSize, const SBAttributeValueCallbacks *valueCallbacks)
{
    SBUInteger namesLength = CountCombinedNamesLength(attributeInfos, count);
    char *namesPointer;
    SBAttributeRegistry *attributeRegistry;

    attributeRegistry = AllocateAttributeRegistry(count, namesLength, &namesPointer);

    if (attributeRegistry) {
        SBAttributeInfo *destination = attributeRegistry->attributeInfos;
        SBUInteger index;

        for (index = 0; index < count; index++) {
            destination[index].name = namesPointer;
            destination[index].group = attributeInfos[index].group;
            destination[index].scope = attributeInfos[index].scope;

            strcpy(namesPointer, attributeInfos[index].name);
            namesPointer += strlen(attributeInfos[index].name) + 1;
        }

        attributeRegistry->count = count;
        attributeRegistry->valueSize = valueSize;

        if (valueCallbacks) {
            attributeRegistry->_valueCallbacks = *valueCallbacks;
        } else {
            attributeRegistry->_valueCallbacks.equal = NULL;
            attributeRegistry->_valueCallbacks.release = NULL;
            attributeRegistry->_valueCallbacks.retain = NULL;
        }

        qsort(attributeRegistry->attributeInfos, count, sizeof(SBAttributeInfo), AttributeInfoComparison);
    }

    return attributeRegistry;
}

SBBoolean SBAttributeRegistryGetAttributeInfo(SBAttributeRegistryRef registry,
    SBAttributeID attributeID, SBAttributeInfo *attributeInfo)
{
    SBBoolean idExists = SBFalse;

    if (SBAttributeIDIsValid(attributeID)) {
        SBUInteger index = SBAttributeIDGetIndex(attributeID);

        *attributeInfo = registry->attributeInfos[index];
        idExists = SBTrue;
    }

    return idExists;
}

SBAttributeID SBAttributeRegistryGetAttributeID(
    SBAttributeRegistryRef registry, const char *name)
{
    SBAttributeInfo *attributeInfo = bsearch(name, registry->attributeInfos, registry->count,
        sizeof(SBAttributeInfo), AttributeNameComparison);

    if (attributeInfo) {
        SBUInteger index = attributeInfo - registry->attributeInfos;
        SBUInt8 size = registry->valueSize;

        return SBAttributeIDMake(index, size);
    }

    return SBAttributeIDNone;
}

SBAttributeRegistryRef SBAttributeRegistryRetain(SBAttributeRegistryRef registry)
{
    return ObjectRetain((ObjectRef)registry);
}

void SBAttributeRegistryRelease(SBAttributeRegistryRef registry)
{
    ObjectRelease((ObjectRef)registry);
}

#endif
