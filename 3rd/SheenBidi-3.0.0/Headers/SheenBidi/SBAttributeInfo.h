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

#ifndef _SB_PUBLIC_ATTRIBUTE_INFO_H
#define _SB_PUBLIC_ATTRIBUTE_INFO_H

#include <SheenBidi/SBBase.h>

#if SB_TEXT_API_SUPPORTED

SB_EXTERN_C_BEGIN

enum {
    SBAttributeIDNone = 0           /**< No Attribute */
};
/**
 * Opaque numeric identifier assigned by the registry to an attribute.
 * 
 * Attribute IDs are used everywhere in the API to refer to known attributes; clients can fetch
 * metadata via the attribute registry.
 */
typedef SBUInteger SBAttributeID;

enum {
    SBAttributeGroupNone = 0        /**< No Special Grouping */
};
/**
 * Abstract grouping for attributes (e.g., shaping, paint, language).
 * 
 * Groups can be used for filtering and coalescing strategies.
 */
typedef SBUInt8 SBAttributeGroup;

enum {
    SBAttributeScopeCharacter = 0,  /**< Applies only to the exact character range. */
    SBAttributeScopeParagraph = 1   /**< Applies to entire paragraphs intersecting range. */
};
/**
 * Application scope for an attribute.
 */
typedef SBUInt8 SBAttributeScope;

/**
 * Callback function to compare two attribute values for equality.
 *
 * @param first
 *      First attribute value pointer.
 * @param second
 *      Second attribute value pointer.
 * @return
 *      `SBTrue` if equal, `SBFalse` otherwise.
 */
typedef SBBoolean (*SBAttributeValueEqualCallback)(const void *first, const void *second);

/**
 * Callback function to retain an attribute value.
 *
 * @param value
 *      Pointer of attribute value to retain.
 * @return
 *      Pointer of retained attribute value.
 */
typedef const void *(*SBAttributeValueRetainCallback)(const void *value);

/**
 * Callback function to release an attribute value.
 *
 * @param value
 *      Pointer of attribute value to release.
 */
typedef void (*SBAttributeValueReleaseCallback)(const void *value);

/**
 * Lifecycle and equality callbacks for attribute value management.
 */
typedef struct _SBAttributeValueCallbacks {
    /**
     * Equality function (optional). If `NULL`, bitwise equality (memcmp) is used to compare
     * attribute values.
     */
    SBAttributeValueEqualCallback equal;
    /**
     * Value retention function (optional). If `NULL`, values are stored as-is without modification.
     */
    SBAttributeValueRetainCallback retain;
    /**
     * Value release function (optional). If `NULL`, no cleanup is performed when values are
     * removed.
     */
    SBAttributeValueReleaseCallback release;
} SBAttributeValueCallbacks;

/**
 * Public description of a registered attribute.
 */
typedef struct _SBAttributeInfo {
    /**
     * Null-terminated unique attribute name.
     */
    const char *name;
    /**
     * Abstract group used for filtering and run coalescing policies.
     */
    SBAttributeGroup group;
    /**
     * Application scope (character/paragraph).
     */
    SBAttributeScope scope;
} SBAttributeInfo;

/**
 * A stored attribute item with its ID and associated value.
 */
typedef struct _SBAttributeItem {
    SBAttributeID attributeID;  /**< Attribute ID. */
    /* Additional value field follows immediately after attributeID in memory */
} SBAttributeItem;

SB_EXTERN_C_END

#endif

#endif
