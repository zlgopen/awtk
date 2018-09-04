/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Whether we have alarm() */
//#define HAVE_ALARM 1

/* Whether the compiler supports __builtin_clz */
//#define HAVE_BUILTIN_CLZ /**/

/* Define to 1 if you have the <dlfcn.h> header file. */
//#define HAVE_DLFCN_H 1

/* Whether we have FE_DIVBYZERO */
//#define HAVE_FEDIVBYZERO 1

/* Whether we have feenableexcept() */
//#define HAVE_FEENABLEEXCEPT 1

/* Define to 1 if we have <fenv.h> */
//#define HAVE_FENV_H 1

/* Whether the tool chain supports __float128 */
//#define HAVE_FLOAT128 /**/

/* Whether the compiler supports GCC vector extensions */
//#define HAVE_GCC_VECTOR_EXTENSIONS /**/

/* Define to 1 if you have the `getisax' function. */
/* #undef HAVE_GETISAX */

/* Whether we have getpagesize() */
//#define HAVE_GETPAGESIZE 1

/* Whether we have gettimeofday() */
//#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <inttypes.h> header file. */
//#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `pixman-1' library (-lpixman-1). */
/* #undef HAVE_LIBPIXMAN_1 */

/* Whether we have libpng */
/* #undef HAVE_LIBPNG */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Whether we have mmap() */
//#define HAVE_MMAP 1

/* Whether we have mprotect() */
//#define HAVE_MPROTECT 1

/* Whether we have posix_memalign() */
//#define HAVE_POSIX_MEMALIGN 1

/* Whether pthreads is supported */
//#define HAVE_PTHREADS /**/

/* Whether we have sigaction() */
//#define HAVE_SIGACTION 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
//#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if we have <sys/mman.h> */
//#define HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
//#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
//#define HAVE_UNISTD_H 1

/* enable TIMER_BEGIN/TIMER_END macros */
//#define PIXMAN_TIMERS 1

#ifdef WITH_64BIT_CPU
/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8
#else
#define SIZEOF_LONG 4
#endif/*WITH_64BIT_CPU*/

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* The compiler supported TLS storage class */
#define TLS 

/* Whether the tool chain supports __attribute__((constructor)) */
//#define TOOLCHAIN_SUPPORTS_ATTRIBUTE_CONSTRUCTOR /**/

/* use ARM IWMMXT compiler intrinsics */
/* #undef USE_ARM_IWMMXT */

/* use ARM NEON assembly optimizations */
/* #undef USE_ARM_NEON */

/* use ARM SIMD assembly optimizations */
/* #undef USE_ARM_SIMD */

/* use GNU-style inline assembler */
/* #undef USE_GCC_INLINE_ASM */

/* use Loongson Multimedia Instructions */
/* #undef USE_LOONGSON_MMI */

/* use MIPS DSPr2 assembly optimizations */
/* #undef USE_MIPS_DSPR2 */

/* use OpenMP in the test suite */
/* #undef USE_OPENMP */

/* use SSE2 compiler intrinsics */
/* #undef USE_SSE2 */

/* use SSSE3 compiler intrinsics */
/* #undef USE_SSSE3 */

/* use VMX compiler intrinsics */
/* #undef USE_VMX */

/* use x86 MMX compiler intrinsics */
/* #undef USE_X86_MMX */

/* Version number of package */
#define VERSION "0.34.0"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

#ifndef COMPILE_TIME_ASSERT
#define COMPILE_TIME_ASSERT(p) 
#endif

#ifdef HAS_STD_MALLOC
#include <stdlib.h>
#define px_malloc  malloc
#define px_realloc realloc
#define px_calloc  calloc
#define px_free    free
#else
#include "base/mem.h"
#define px_malloc  tk_alloc
#define px_realloc tk_realloc
#define px_calloc  tk_calloc
#define px_free    tk_free
#endif/*HAS_STD_MALLOC*/
/* Define to sqrt if you do not have the `sqrtf' function. */
/* #undef sqrtf */
