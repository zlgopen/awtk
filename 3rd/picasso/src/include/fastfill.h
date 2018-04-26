/* Picasso - a vector graphics library
 *
 * Copyright (C) 2015 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _FAST_FILL_H_
#define _FAST_FILL_H_

#if CPU(ARM) && CPU(ARM_NEON)

extern "C" void fastfill16_neon(uint16_t *dst, uint16_t value, int n);
extern "C" void fastfill32_neon(uint32_t *dst, uint32_t value, int n);

#define fastfill16(d, v, n) fastfill16_neon((d), (v), (n))
#define fastfill32(d, v, n) fastfill32_neon((d), (v), (n))

#else
inline void fastfill16(uint16_t *dst, uint16_t value, int n)
{
    do {
        *dst++ = value;
    } while(--n);
}

inline void fastfill32(uint32_t *dst, uint32_t value, int n)
{
    do {
        *dst++ = value;
    } while(--n);
}

#endif /*ARM_NEON*/

#endif /*_FAST_FILL_H_*/
