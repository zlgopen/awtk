/* Picasso - a vector graphics library
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _MATH_TYPES_H_
#define _MATH_TYPES_H_

#include "common.h"

#include <math.h>
typedef float scalar;
#define FLT_TO_SCALAR(v)  ((scalar)(v))
#define INT_TO_SCALAR(v)  ((scalar)(v))
#define SCALAR_TO_FLT(v)  ((float)(v))
#define SCALAR_TO_INT(v)  ((int)(v))
//math PI value
#define PI      3.14159265358979323846f
#define _2PI    6.28318530717958647692f
#define _PIdiv2 1.57079632679489661923f
#define _1divPI 0.31830988618379069f
#define _1div2PI 0.1591549432737563f

#define Fabs(x) fabsf(x)
#define Fmod(x, y) fmodf((x), (y))
#define Sqrt(x) sqrtf(x)
#define Tan(x) tanf(x)
#define Atan2(x, y) atan2f((x), (y))
#define Sin(x) sinf(x)
#define Cos(x) cosf(x)
#define Acos(x) acosf(x)
#define Asin(x) asinf(x)
#define Floor(x) floorf(x)
#define Ceil(x) ceilf(x)
#define Exp(x) expf(x)
#define Pow(x, y) powf((x), (y))

// max min
template <typename T> inline T Min(T a, T b) { return (a < b) ? a : b; }
template <typename T> inline T Max(T a, T b) { return (a > b) ? a : b; }

// float type
inline unsigned _uround(float v)
{
    return unsigned(v + 0.5f);
}

inline int _iround(float v)
{
    return int((v < 0.0f) ? v - 0.5f : v + 0.5f);
}

#endif /*_MATH_TYPES_H_*/
