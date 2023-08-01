#ifndef WASM_HELPER_H
#define WASM_HELPER_H

#include <stdio.h>

BEGIN_C_DECLS

#ifndef PRIu64
#if UINTPTR_MAX == 0xffffffff
#define __PRI64_PREFIX "ll"
#else
#define __PRI64_PREFIX "l"
#endif
#define PRIu64 __PRI64_PREFIX "u"
#define PRId64 __PRI64_PREFIX "d"
#endif /*PRIu64*/

#define INFINITY 3.40282347E+38f
#define assert(__pp) wasm_assert(__pp, #__pp)

#define WITH_WCSXXX 1
#define WITHOUT_FSCRIPT

#define log_impl printf
#define abs(a) ((a) > 0 ? (a) : -(a))
#define fabs(a) ((a) > 0 ? (a) : -(a))

#ifndef __cplusplus
typedef int wchar_t;
#endif /*_cplusplus*/

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
_Check_return_ _ACRTIMP double    __cdecl floor(_In_ double _X);
_Check_return_ _ACRTIMP double    __cdecl ceil(_In_ double _X);
#else
double round(double x);
double floor(double x);
double ceil(double x);
#endif/*WIN32*/

int iswspace(wchar_t ch);
size_t wcslen(const wchar_t* s);
int wcscmp(const wchar_t* s1, const wchar_t* s2);
int wcscasecmp(const wchar_t* s1, const wchar_t* s2);
int wcsncmp(const wchar_t* s1, const wchar_t* s2, size_t n);

wchar_t* wcsdup(const wchar_t* s);
wchar_t* wcschr(const wchar_t* s, wchar_t c);
wchar_t* wcscpy(wchar_t* s1, const wchar_t* s2);
wchar_t* wcsncpy(wchar_t* s1, const wchar_t* s2, uint32_t n);
wchar_t* wcsstr(const wchar_t* s1, const wchar_t* s2);

double atof(const char* str);
char* strrchr(const char* s, int c);
void wasm_assert(int p, const char* text);
int strcasecmp(const char* s1, const char* s2);
long strtol(const char* str, char** endptr, int base);
long long strtoll(const char* str, char** endptr, int base);
unsigned long strtoul(const char* str, char** endptr, int base);
unsigned long long strtoull(const char* str, char** endptr, int base);

void qsort (void *, size_t, size_t, int (*)(const void *, const void *));

END_C_DECLS

#endif /*WASM_HELPER_H*/
