#ifndef WASM_HELPER_H
#define WASM_HELPER_H

#include <math.h>
#include <stdio.h>

BEGIN_C_DECLS

#define WITH_SOCKET 1

#ifndef PRIu64
#if UINTPTR_MAX == 0xffffffff
#define __PRI64_PREFIX "ll"
#else
#define __PRI64_PREFIX "l"
#endif
#define PRIu64 __PRI64_PREFIX "u"
#define PRId64 __PRI64_PREFIX "d"
#endif /*PRIu64*/

#ifndef PRIu32
#define PRIu32 "u"
#endif /*PRIu32*/

#ifndef PRIx64
#define PRIx64 "llx"
#endif/*PRIx64*/

#ifdef WITH_EASTL
#include <wchar.h>
#else
#ifndef WITH_WCSXXX
#define WITH_WCSXXX 1
#endif /*WITH_WCSXXX*/
#endif /*WITH_EASTL*/

#define WITHOUT_FSCRIPT

int awplc_log(const char* format, ...);
#define log_impl awplc_log
#define assert(__pp) wasm_assert(__pp, #__pp)

#define ret_t_init(r) *r = RET_OK;

double sin(double x);
double cos(double x);
double tan(double x);
double asin(double x);
double acos(double x);
double atan(double x);
double atan2(double y, double x);
double sqrt(double x);
double log10(double x);
double log(double x);
double exp(double x);

#if defined(WIN32) && defined(_MSC_VER)
_Check_return_ _ACRTIMP double __cdecl round(_In_ double _X);
_Check_return_ _ACRTIMP double __cdecl floor(_In_ double _X);
_Check_return_ _ACRTIMP double __cdecl ceil(_In_ double _X);
#else
double round(double x);
double floor(double x);
double ceil(double x);
#endif /*WIN32*/

#ifndef __cplusplus
typedef int wchar_t;
typedef int wint_t;
#endif /*_cplusplus*/

int iswupper(wchar_t ch);
int iswlower(wchar_t ch);
int iswdigit(wchar_t ch);
int iswxdigit(wchar_t ch);
int iswalpha(wchar_t ch);
int iswalnum(wchar_t ch);
int iswspace(wchar_t ch);
int islower(int c);

#if defined(__GNUC__) && !defined(__cplusplus)
typedef _Bool bool_t;
#else
typedef uint8_t bool_t;
#endif

double atof(const char* str);
char* strrchr(const char* s, int c);
int wasm_assert(bool_t p, const char* text);
int strcasecmp(const char* s1, const char* s2);
long strtol(const char* str, char** endptr, int base);
long long strtoll(const char* str, char** endptr, int base);
unsigned long strtoul(const char* str, char** endptr, int base);
unsigned long long strtoull(const char* str, char** endptr, int base);

long random(void);
void srand(unsigned seed);

void qsort(void*, size_t, size_t, int (*)(const void*, const void*));

#define towlower(c) tolower(c)
#define towupper(c) toupper(c)

#define HAS_NO_VSSCANF 1

void perror(const char* s);

struct in_addr {
  unsigned long s_addr;
};

struct sockaddr {
  unsigned short sa_family;
  char sa_data[14];
};

struct sockaddr_in {
  short sin_family;
  unsigned short sin_port;
  struct in_addr sin_addr;
  char sin_zero[8];
};

END_C_DECLS

#endif /*WASM_HELPER_H*/
