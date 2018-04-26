#ifndef _WIN32_STDINT_H
#define _WIN32_STDINT_H

#if defined(_MSC_VER) && (_MSC_VER < 1600)
#include <stddef.h>
#include <limits.h>

/* 7.18.1.1  Exact-width integer types */
#ifndef int8_t
typedef signed char int8_t;
#endif
#ifndef uint8_t
typedef unsigned char   uint8_t;
#endif
typedef short  int16_t;
typedef unsigned short  uint16_t;
typedef int  int32_t;
typedef unsigned   uint32_t;

#ifdef __GNUC__
typedef long long  int64_t;
typedef unsigned long long   uint64_t;
#else
typedef __int64  int64_t;
typedef unsigned __int64   uint64_t;
#endif

/* 7.18.1.2  Minimum-width integer types */
typedef signed char int_least8_t;
typedef unsigned char   uint_least8_t;
typedef short  int_least16_t;
typedef unsigned short  uint_least16_t;
typedef int  int_least32_t;
typedef unsigned   uint_least32_t;

#ifdef __GNUC__
typedef long long  int_least64_t;
typedef unsigned long long   uint_least64_t;
#else
typedef __int64  int_least64_t;
typedef unsigned __int64   uint_least64_t;
#endif

/*  7.18.1.3  Fastest minimum-width integer types 
 *  Not actually guaranteed to be fastest for all purposes
 *  Here we use the exact-width types for 8 and 16-bit ints. 
 */
typedef char int_fast8_t;
typedef unsigned char uint_fast8_t;
typedef short  int_fast16_t;
typedef unsigned short  uint_fast16_t;
typedef int  int_fast32_t;
typedef unsigned  int  uint_fast32_t;

#ifdef __GNUC__
typedef long long  int_fast64_t;
typedef unsigned long long   uint_fast64_t;
#else
typedef __int64  int_fast64_t;
typedef unsigned __int64   uint_fast64_t;
#endif

/* 7.18.1.4  Integer types capable of holding object pointers */
typedef int intptr_t;
typedef unsigned uintptr_t;

/* 7.18.1.5  Greatest-width integer types */
#ifdef __GNUC__
typedef long long  intmax_t;
typedef unsigned long long   uintmax_t;
#else
typedef __int64  intmax_t;
typedef unsigned __int64   uintmax_t;
#endif

/* 7.18.2  Limits of specified-width integer types */
#if !defined ( __cplusplus) || defined (__STDC_LIMIT_MACROS)

/* 7.18.2.1  Limits of exact-width integer types */
#define INT8_MIN (-128) 
#define INT16_MIN (-32768)
#define INT32_MIN (-2147483647 - 1)
#ifdef __GNUC__
#define INT64_MIN  (-9223372036854775807LL - 1)
#else
#define INT64_MIN  (-9223372036854775807i64 - 1)
#endif

#define INT8_MAX 127
#define INT16_MAX 32767
#define INT32_MAX 2147483647
#ifdef __GNUC__
#define INT64_MAX 9223372036854775807LL
#else
#define INT64_MAX 9223372036854775807i64
#endif

#define UINT8_MAX 0xff /* 255U */
#define UINT16_MAX 0xffff /* 65535U */
#define UINT32_MAX 0xffffffff  /* 4294967295U */
#ifdef __GNUC__
#define UINT64_MAX 0xffffffffffffffffULL /* 18446744073709551615ULL */
#else
#define UINT64_MAX 0xffffffffffffffffui64 /* 18446744073709551615ULL */
#endif

/* 7.18.2.2  Limits of minimum-width integer types */
#define INT_LEAST8_MIN INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define INT_LEAST8_MAX INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define UINT_LEAST8_MAX UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

/* 7.18.2.3  Limits of fastest minimum-width integer types */
#define INT_FAST8_MIN INT8_MIN
#define INT_FAST16_MIN INT16_MIN
#define INT_FAST32_MIN INT32_MIN
#define INT_FAST64_MIN INT64_MIN

#define INT_FAST8_MAX INT8_MAX
#define INT_FAST16_MAX INT16_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST64_MAX INT64_MAX

#define UINT_FAST8_MAX UINT8_MAX
#define UINT_FAST16_MAX UINT16_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT64_MAX

/* 7.18.2.4  Limits of integer types capable of holding
    object pointers */ 
#define INTPTR_MIN INT32_MIN
#define INTPTR_MAX INT32_MAX
#define UINTPTR_MAX UINT32_MAX

/* 7.18.2.5  Limits of greatest-width integer types */
#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX

/* 7.18.3  Limits of other integer types */
#define PTRDIFF_MIN INT32_MIN
#define PTRDIFF_MAX INT32_MAX

#define SIG_ATOMIC_MIN INT32_MIN
#define SIG_ATOMIC_MAX INT32_MAX

#ifndef SIZE_MAX
#define SIZE_MAX UINT32_MAX
#endif

#ifndef WCHAR_MIN  /* also in wchar.h */ 
#define WCHAR_MIN 0
#define WCHAR_MAX 0xffff /* UINT16_MAX */
#endif

/*
 * wint_t is unsigned short for compatibility with MS runtime
 */
#define WINT_MIN 0
#define WINT_MAX 0xffff /* UINT16_MAX */

#endif /* !defined ( __cplusplus) || defined __STDC_LIMIT_MACROS */

#endif /*_MSC_VER*/

#endif //STDINT_H
