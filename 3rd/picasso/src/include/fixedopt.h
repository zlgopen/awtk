/* Fixed mathematics 
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _FIXED_MATH_H_
#define _FIXED_MATH_H_

//32 bit system value define
typedef int int32_t;
#if defined(__GNUC__)
typedef long long int64_t;
#elif defined(_MSC_VER)
typedef __int64 int64_t;
#elif defined(__WATCOMC__)
typedef __int64 int64_t;
#else
#error no define 64 bit integer.
#endif

namespace fxmath {

// Fixed point number using 16.16 notation.
typedef int32_t fixed_type;
typedef int64_t fixed_type64;


// Fixed point specialed 

// Fixed point bits
#define FIXED_BITS    (sizeof(fixed_type) * 8)

// Fixed point non-integer part bits
#define FIXED_Q        (FIXED_BITS - (FIXED_BITS / 2))

// The number 1 expressed as fixed
#define FIXED_1        (((fixed_type)1) << FIXED_Q)

// The number 0.5 expressed as fixed
#define FIXED_0_5   (((fixed_type)1) << (FIXED_Q - 1))

// The biggest number representable
#define FIXED_MAX     ((fixed_type)(((fixed_type)1) << (FIXED_BITS - 1) ^ (~0)))

// The smallest number representable
#define FIXED_MIN    ((fixed_type)((fixed_type)1) << (FIXED_BITS - 1))

// The biggest float number 
#define FLOAT_F_MAX    (((fixed_type)FIXED_MAX) / (double)FIXED_1)

// The smallest float number
#define FLOAT_F_MIN    (((fixed_type)FIXED_MIN) / (double)FIXED_1)

// Retrieves the fractionary part of x.
#define FIXED_FRACTION(x)  ((x) & ((1 << FIXED_Q) - 1))

// Set fixed point to valid value
#define FIXED_VALID(s , st , v)  ((s < (st)FLOAT_F_MIN ? FIXED_MIN : (s > (st)FLOAT_F_MAX ? FIXED_MAX : (v))))

class fixed
{
public:
    //type converion
    explicit fixed(double v) : m_data(FIXED_VALID(v, double, (fixed_type)(v * (double)FIXED_1))) { }
    explicit fixed(float v) : m_data(FIXED_VALID(v, float, (fixed_type)(v * (float)FIXED_1))) { }
    explicit fixed(int v) : m_data(FIXED_VALID(v, int, ((fixed_type)v) << FIXED_Q)) { }

    operator double () const { return ((double)(m_data / (double)FIXED_1)); }
    operator float () const { return ((float)(m_data / (float)FIXED_1)); }
    operator int () const { return ((int)(m_data >> FIXED_Q)); }

public:
    // operators
    fixed() : m_data(0) {}
    fixed(fixed_type v, int) : m_data(v) {}
    fixed_type data(void) const { return m_data; }

    friend bool operator < (const fixed &a, const fixed &b);
    friend bool operator > (const fixed &a, const fixed &b);
    friend bool operator >= (const fixed &a, const fixed &b);
    friend bool operator <= (const fixed &a, const fixed &b);
    friend bool operator == (const fixed &a, const fixed &b);
    friend bool operator != (const fixed &a, const fixed &b);
    friend fixed operator + (const fixed &a, const fixed &b);
    friend fixed operator - (const fixed &a, const fixed &b);
    friend fixed operator * (const fixed &a, const fixed &b);
    friend fixed operator / (const fixed &a, const fixed &b);
public:
    //special values
    // pi = 3.14159265358979323846
    static fixed fxPI(void);
    // pi * 2
    static fixed fxPI2(void);
    // pi / 2
    static fixed fxPIdiv2(void);
    // 0
    static fixed fixed_0(void);
public:
    // math functions
    friend int floor(fixed x);
    friend int ceil(fixed x);
    friend int round(fixed x);
private:
    fixed_type m_data;
};

// a < b
inline bool operator < (const fixed& a, const fixed& b)
{
    return (a.m_data < b.m_data) ? true : false;
}

// a > b 
inline bool operator > (const fixed& a, const fixed& b)
{
    return (a.m_data > b.m_data) ? true : false;
}

// a >= b 
inline bool operator >= (const fixed& a, const fixed& b)
{
    return (a.m_data >= b.m_data) ? true : false;
}

// a <= b 
inline bool operator <= (const fixed& a, const fixed& b)
{
    return (a.m_data <= b.m_data) ? true : false;
}

// a == b 
inline bool operator == (const fixed& a, const fixed& b)
{
    return (a.m_data == b.m_data) ? true : false;
}

// a != b 
inline bool operator != (const fixed& a, const fixed& b)
{
    return (a.m_data != b.m_data) ? true : false;
}

// r = a + b;
inline fixed operator + (const fixed& a, const fixed& b)
{
    register fixed_type r = a.m_data + b.m_data;
    return fixed(r, 0);
}

// r = a - b;
inline fixed operator - (const fixed& a, const fixed& b)
{
    register fixed_type r = a.m_data - b.m_data;
    return fixed(r, 0);
}

#if defined(__GNUC__) && defined(__arm__) && (__SIZEOF_INT__ == 4) 
//integer 12% speed up
inline fixed_type arm_fixmul (fixed_type a, fixed_type b)
{
    int res_low, res_hi;

    __asm__ ("smull %0, %1, %2, %3     \n"
            "mov   %0, %0,     lsr %4 \n"
            "add   %1, %0, %1, lsl %5 \n"
            : "=r"(res_hi), "=r"(res_low) \
            : "r"(a), "r"(b), "i"(FIXED_Q), "i"(32 - FIXED_Q));

    return (fixed_type) res_low;
}

inline fixed operator * (const fixed& a, const fixed& b)
{
    return fixed(arm_fixmul(a.data(), b.data()), 0);
}
#else
// r = a * b;
inline fixed operator * (const fixed& a, const fixed& b)
{
    fixed_type64 r = (fixed_type64)a.m_data * (fixed_type64)b.m_data;
    return fixed((fixed_type)(r>>FIXED_Q), 0);
}
#endif

// r = a / b;
inline fixed operator / (const fixed& a, const fixed& b)
{
    fixed_type64 r = ((((fixed_type64)a.m_data) << FIXED_Q) / b.m_data) ;
    return fixed((fixed_type)r, 0);
}


// Std C math library fixed point version

// double floor(double x)
inline int floor(fixed x)
{
    return (int)((x.m_data >= 0) ? (x.m_data >> FIXED_Q) : ~((~(x.m_data)) >> FIXED_Q));
}

// double ceil(double x)
inline int ceil(fixed x)
{
    x.m_data += ((((fixed_type)1) << FIXED_Q)-1);
    return (int)((x.m_data >= 0) ? (x.m_data >> FIXED_Q) : ~((~(x.m_data)) >> FIXED_Q));
}

// double round(double x)
inline int round(fixed x)
{
    return floor(fixed(x.data()+FIXED_0_5, 0));
}

inline fixed_type flt_to_fixed(float f)
{
    return (fixed_type)(f * (float)FIXED_1);
}

// double fabs (double x)
fixed fabs(fixed x);

// double sin(double x)
fixed sin(fixed radian);

// double asin(double x)
fixed asin(fixed x);

// double cos(double x)
fixed cos(fixed radian);

// double acos(double x)
fixed acos(fixed x);

// double tan(double x)
fixed tan(fixed radian);

// double atan(double x)
fixed atan(fixed value);

// double atan2(double y, double x)
fixed atan2(fixed y, fixed x);

// double sqrt(double x)
fixed sqrt(fixed x);

// double fmod(double x, double y)
fixed fmod(fixed x, fixed y);

}
#endif /*_FIXED_MATH_H_*/
