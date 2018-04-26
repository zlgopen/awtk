/* Picasso - a vector graphics library
 *
 * Copyright (C) 2015 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _FAST_COPY_H_
#define _FAST_COPY_H_

#if ENABLE(FAST_COPY) && !CPU(ARM)

#if (CPU(X86) || CPU(X86_64)) && defined(__SSE2__)

#include "fastcopy_sse.h"
#define fastcopy(d, s, n) fastcopy_sse2_32((uint8_t*)(d), (uint8_t*)(s), n)

#else
inline void fastcopy4(uint8_t* __restrict dest, const uint8_t* __restrict src, int n)
{
    register int i;
    for (i = 0; i < (n&3); i++){
        *dest++ = *src++;
    }

    n >>= 2;

    for (i = 0; i < n; i++){
        *(uint32_t *)dest = *(uint32_t *)src;
        dest += 4;
        src += 4;
    }
}

#define fastcopy(d, s, n) fastcopy4((uint8_t*)(d), (uint8_t*)(s), n)

#endif /*__SSE2__*/

#else

#define fastcopy(d, s, n) memcpy((d), (s), n)

#endif /*ENABLE(FAST_COPY)*/

#endif /*_FAST_COPY_H_*/
