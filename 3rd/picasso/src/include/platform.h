/* Picasso - a vector graphics library
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

/* ENABLE() - turn on a specific feature */
#define ENABLE(FEATURE) (defined ENABLE_##FEATURE && ENABLE_##FEATURE)

/* COMPILER() - the compiler being used to build the project */
#define COMPILER(FEATURE) (defined COMPILER_##FEATURE  && COMPILER_##FEATURE)

/* CPU() - the target CPU architecture */
#define CPU(FEATURE) (defined CPU_##FEATURE  && CPU_##FEATURE)


/* COMPILER(MSVC) - Microsoft Visual C++ */
#if defined(_MSC_VER)
#define COMPILER_MSVC 1
#endif

/* COMPILER(GCC) - GNU Compiler Collection */
#if defined(__GNUC__)
#define COMPILER_GCC 1
#endif

/* COMPILER(INTEL) - Intel C++ Compiler */
#if defined(__INTEL_COMPILER)
#define COMPILER_INTEL 1
#endif

/* COMPILER(WATCOM) - Watcom C/C++ Compiler */
#if defined(__WATCOMC__)
#define COMPILER_WATCOM 1
#endif

/* COMPILER(CLANG) - llvm/clang C/C++ Compiler */
#if defined(__clang__)
#define COMPILER_CLANG 1
#endif


/* CPU(X86) - i386 / x86 32-bit */
#if   defined(__i386__) \
    || defined(i386)     \
    || defined(_M_IX86)  \
    || defined(_X86_)    \
    || defined(__THW_INTEL)
#define CPU_X86 1
#endif

/* CPU(X86_64) - AMD64 / Intel64 / x86_64 64-bit */
#if   defined(__x86_64__) \
    || defined(_M_X64)
#define CPU_X86_64 1
#endif

/* CPU(ARM) - ARM any version */
#if   defined(arm) \
     || defined(__arm__) \
     || defined(ARM) \
     || defined(_ARM_)
#define CPU_ARM 1
#endif

#if defined(__ARM_NEON__)
#define CPU_ARM_NEON 1
#endif

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
#define CPU_ARM_VFP 1
#endif

#if defined(WINCE) || defined(__WATCOMC__)
/* math.h */
#define sinf(x) ((float)sin(x))
#define cosf(x) ((float)cos(x))
#define acosf(x) ((float)acos(x))
#define tanf(x) ((float)tan(x))
#define asinf(x) ((float)asin(x))
#define atan2f(x, y) ((float)atan2((x), (y)))
#define powf(x, y) ((float)pow((x), (y)))
#if defined(__WATCOMC__)
#define sqrtf(x) ((float)sqrt((x)))
#define fabsf(x) ((float)fabs((x)))
#define fmodf(x, y) ((float)fmod((x), (y)))
#define floorf(x) ((float)floor((x)))
#define ceilf(x) ((float)ceil((x)))
#endif
#endif

#if COMPILER(GCC)
#if __GNUC__ == 2 && __GNUC_MINOR__ < 96
#define __builtin_expect(x, expected_value) (x)
#endif
#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
#else
#define likely(X)  (X)
#define unlikely(X)  (X)
#endif

#endif /*_PLATFORM_H_*/
