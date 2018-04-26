/* Picasso - a vector graphics library
 *
 * Copyright (C) 2015 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _FAST_COPY_SSE2_H_
#define _FAST_COPY_SSE2_H_

#include <stdint.h>
#include <emmintrin.h>

// use sse2 intrinces for copy data.

inline void fastcopy_sse2_32(uint8_t* __restrict dest, const uint8_t* __restrict src, int n)
{
    for (; ((intptr_t)dest & 15) && (n > 0); n--) {
        *dest++ = *src++;
    }

    for (; n >= 32; n -= 32) {
        _mm_store_si128((__m128i *)dest, _mm_loadu_si128((__m128i *)src));
        _mm_store_si128((__m128i *)(dest + 16), _mm_loadu_si128((__m128i *)(src + 16)));
        src += 32;
        dest += 32;
    }

    if (n >= 16) {
        _mm_store_si128((__m128i *)dest, _mm_loadu_si128((__m128i *)src));
        src += 16;
        dest += 16;
        n -= 16;
    }

    for (; n > 0; n--) {
        *dest++ = *src++;
    }
}

inline void fastcopy_sse2_16(uint8_t* __restrict dest, const uint8_t* __restrict src, int n)
{
    for (; ((intptr_t)dest & 15) && (n > 0); n--) {
        *dest++ = *src++;
    }

    for (; n >= 16; n -= 16) {
        _mm_store_si128((__m128i *)dest, _mm_loadu_si128((__m128i *)src));
        src += 16;
        dest += 16;
    }

    for (; n > 0; n--) {
        *dest++ = *src++;
    }
}

#endif /*_FAST_COPY_SSE2_H_*/
