/**
 * File:   wchar32.c
 * Author: AWTK Develop Team
 * Brief:  32bits wide char
 *
 * Copyright (c) 2024 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2024-12-19 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "tkc/mem.h"
#include "tkc/wchar32.h"

wchar32_t* wcs32_init_with_wcs(wchar32_t* s1, const wchar_t* s2) {
  wchar32_t* start = s1;
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while ((*s1++ = *s2++) != 0) {
  }

  return start;
}

wchar32_t* wcp32cpy(wchar32_t* s1, wchar32_t* s2) {
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while ((*s1++ = *s2++) != 0) {
  }

  return s1 - 1;
}

wchar32_t* wcp32ncpy(wchar32_t* s1, wchar32_t* s2, size_t n) {
  wchar32_t* start = s1;
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while (n-- > 0) {
    if ((*s1++ = *s2++) == 0) {
      break;
    }
  }

  return start;
}

int wcs32casecmp(const wchar32_t* s1, const wchar32_t* s2) {
  return_value_if_fail(s1 != NULL && s2 != NULL, 0);

  while (towlower(*s1) == towlower(*s2)) {
    if (*s1 == 0) {
      return 0;
    }
    s1++;
    s2++;
  }

  return tolower(*s1) - tolower(*s2);
}

wchar32_t* wcs32cat(wchar32_t* s1, const wchar32_t* s2) {
  wchar32_t* start = s1;
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while (*s1 != 0) {
    s1++;
  }

  while ((*s1++ = *s2++) != 0) {
  }

  return start;
}

wchar32_t* wcs32chr(const wchar32_t* s, wchar32_t c) {
  return_value_if_fail(s != NULL, NULL);

  while (*s != 0) {
    if (*s == c) {
      return (wchar32_t*)s;
    }
    s++;
  }

  return NULL;
}

int wcs32cmp(const wchar32_t* s1, const wchar32_t* s2) {
  return_value_if_fail(s1 != NULL && s2 != NULL, 0);

  while (*s1 == *s2) {
    if (*s1 == 0) {
      return 0;
    }
    s1++;
    s2++;
  }

  return *s1 - *s2;
}

wchar32_t* wcs32cpy(wchar32_t* s1, const wchar32_t* s2) {
  wchar32_t* start = s1;
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while ((*s1++ = *s2++) != 0) {
  }

  return start;
}

size_t wcs32cspn(const wchar32_t* s1, const wchar32_t* s2) {
  size_t i = 0;
  return_value_if_fail(s1 != NULL && s2 != NULL, 0);

  while (s1[i] != 0) {
    if (wcs32chr(s2, s1[i]) != NULL) {
      break;
    }
    i++;
  }

  return i;
}

wchar32_t* wcs32dup(const wchar32_t* s) {
  wchar32_t* ret = NULL;
  uint32_t size = 0;
  return_value_if_fail(s != NULL, NULL);
  size = (wcs32len(s) + 1) * sizeof(wchar32_t);
  ret = (wchar32_t*)TKMEM_ALLOC(size);
  return_value_if_fail(ret != NULL, NULL);

  return wcs32cpy(ret, s);
}

size_t wcs32lcat(wchar32_t* s1, const wchar32_t* s2, size_t n) {
  size_t i = 0;
  return_value_if_fail(s1 != NULL && s2 != NULL, 0);

  while (*s1 != 0) {
    s1++;
  }

  while (n-- > 0) {
    if ((*s1++ = *s2++) == 0) {
      break;
    }
    i++;
  }

  return i;
}

size_t wcs32lcpy(wchar32_t* s1, const wchar32_t* s2, size_t n) {
  size_t i = 0;
  return_value_if_fail(s1 != NULL && s2 != NULL, 0);

  while (n-- > 0) {
    if ((*s1++ = *s2++) == 0) {
      break;
    }
    i++;
  }

  return i;
}

size_t wcs32len(const wchar32_t* s) {
  size_t i = 0;
  return_value_if_fail(s != NULL, 0);

  while (s[i] != 0) {
    i++;
  }

  return i;
}

int wcs32ncasecmp(const wchar32_t* s1, const wchar32_t* s2, size_t n) {
  return_value_if_fail(s1 != NULL && s2 != NULL, 0);

  while (n-- > 0) {
    if (towlower(*s1) != towlower(*s2)) {
      return tolower(*s1) - tolower(*s2);
    }
    if (*s1 == 0) {
      return 0;
    }
    s1++;
    s2++;
  }

  return 0;
}

wchar32_t* wcs32ncat(wchar32_t* s1, const wchar32_t* s2, size_t n) {
  wchar32_t* start = s1;
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while (*s1 != 0) {
    s1++;
  }

  while (n-- > 0) {
    if ((*s1++ = *s2++) == 0) {
      break;
    }
  }

  return start;
}

int wcs32ncmp(const wchar32_t* s1, const wchar32_t* s2, size_t n) {
  return_value_if_fail(s1 != NULL && s2 != NULL, 0);

  while (n-- > 0) {
    if (*s1 != *s2) {
      return *s1 - *s2;
    }
    if (*s1 == 0) {
      return 0;
    }
    s1++;
    s2++;
  }

  return 0;
}

wchar32_t* wcs32ncpy(wchar32_t* s1, const wchar32_t* s2, size_t n) {
  wchar32_t* start = s1;
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while (n-- > 0) {
    if ((*s1++ = *s2++) == 0) {
      break;
    }
  }

  return start;
}

size_t wcs32nlen(const wchar32_t* s, size_t maxlen) {
  size_t i = 0;
  return_value_if_fail(s != NULL, 0);

  while (maxlen-- > 0) {
    if (s[i] == 0) {
      break;
    }
    i++;
  }

  return i;
}

wchar32_t* wcs32pbrk(const wchar32_t* s1, const wchar32_t* s2) {
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while (*s1 != 0) {
    if (wcs32chr(s2, *s1) != NULL) {
      return (wchar32_t*)s1;
    }
    s1++;
  }

  return NULL;
}

wchar32_t* wcs32rchr(const wchar32_t* s, wchar32_t c) {
  const wchar32_t* start = s;
  return_value_if_fail(s != NULL, NULL);

  while (*s != 0) {
    s++;
  }

  while (s >= start) {
    if (*s == c) {
      return (wchar32_t*)s;
    }
    s--;
  }

  return NULL;
}

size_t wcs32spn(const wchar32_t* s1, const wchar32_t* s2) {
  size_t i = 0;
  return_value_if_fail(s1 != NULL && s2 != NULL, 0);

  while (s1[i] != 0) {
    if (wcs32chr(s2, s1[i]) == NULL) {
      break;
    }
    i++;
  }

  return i;
}

wchar32_t* wcs32str(const wchar32_t* s1, const wchar32_t* s2) {
  size_t len = wcs32len(s2);
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while (*s1 != 0) {
    if (wcs32ncmp(s1, s2, len) == 0) {
      return (wchar32_t*)s1;
    }
    s1++;
  }

  return NULL;
}

wchar32_t* w32memchr(const wchar32_t* s, wchar32_t c, size_t n) {
  return_value_if_fail(s != NULL, NULL);

  while (n-- > 0) {
    if (*s == c) {
      return (wchar32_t*)s;
    }
    s++;
  }

  return NULL;
}

int w32memcmp(const wchar32_t* s1, const wchar32_t* s2, size_t n) {
  return_value_if_fail(s1 != NULL && s2 != NULL, 0);

  while (n-- > 0) {
    if (*s1 != *s2) {
      return *s1 - *s2;
    }
    s1++;
    s2++;
  }

  return 0;
}

wchar32_t* w32memcpy(wchar32_t* s1, const wchar32_t* s2, size_t n) {
  wchar32_t* start = s1;
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while (n-- > 0) {
    *s1++ = *s2++;
  }

  return start;
}

wchar32_t* w32memmove(wchar32_t* s1, const wchar32_t* s2, size_t n) {
  wchar32_t* start = s1;
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  if (s1 < s2) {
    while (n-- > 0) {
      *s1++ = *s2++;
    }
  } else {
    s1 += n;
    s2 += n;
    while (n-- > 0) {
      *--s1 = *--s2;
    }
  }

  return start;
}

wchar32_t* w32memset(wchar32_t* s, wchar32_t c, size_t n) {
  wchar32_t* start = s;
  return_value_if_fail(s != NULL, NULL);

  while (n-- > 0) {
    *s++ = c;
  }

  return start;
}

#ifdef WITH_WCSXXX
wchar_t* wcpcpy(wchar_t* s1, wchar_t* s2) {
  return wcp32cpy((wchar32_t*)s1, (wchar32_t*)s2);
}

wchar_t* wcpncpy(wchar_t* s1, wchar_t* s2, size_t n) {
  return wcp32ncpy((wchar32_t*)s1, (wchar32_t*)s2, n);
}

int wcscasecmp(const wchar_t* s1, const wchar_t* s2) {
  return wcs32casecmp((const wchar32_t*)s1, (const wchar32_t*)s2);
}

wchar_t* wcscat(wchar_t* s1, const wchar_t* s2) {
  return (wchar_t*)wcs32cat((wchar32_t*)s1, (const wchar32_t*)s2);
}

wchar_t* wcschr(const wchar_t* s, wchar_t c) {
  return (wchar_t*)wcs32chr((const wchar32_t*)s, c);
}

int wcscmp(const wchar_t* s1, const wchar_t* s2) {
  return wcs32cmp((const wchar32_t*)s1, (const wchar32_t*)s2);
}

size_t wcscspn(const wchar_t* s1, const wchar_t* s2) {
  return wcs32cspn((const wchar32_t*)s1, (const wchar32_t*)s2);
}

wchar_t* wcsdup(const wchar_t* s) {
  return (wchar_t*)wcs32dup((const wchar32_t*)s);
}

size_t wcslcat(wchar_t* s1, const wchar_t* s2, size_t n) {
  return wcs32lcat((wchar32_t*)s1, (const wchar32_t*)s2, n);
}

size_t wcslcpy(wchar_t* s1, const wchar_t* s2, size_t n) {
  return wcs32lcpy((wchar32_t*)s1, (const wchar32_t*)s2, n);
}

size_t wcslen(const wchar_t* s) {
  return wcs32len((const wchar32_t*)s);
}

int wcsncasecmp(const wchar_t* s1, const wchar_t* s2, size_t n) {
  return wcs32ncasecmp((const wchar32_t*)s1, (const wchar32_t*)s2, n);
}

wchar_t* wcscpy(wchar_t* s1, const wchar_t* s2) {
  return (wchar_t*)wcs32cpy((wchar32_t*)s1, (const wchar32_t*)s2);
}

wchar_t* wcsncat(wchar_t* s1, const wchar_t* s2, size_t n) {
  return (wchar_t*)wcs32ncat((wchar32_t*)s1, (const wchar32_t*)s2, n);
}

int wcsncmp(const wchar_t* s1, const wchar_t* s2, size_t n) {
  return wcs32ncmp((const wchar32_t*)s1, (const wchar32_t*)s2, n);
}

wchar_t* wcsncpy(wchar_t* s1, const wchar_t* s2, size_t n) {
  return (wchar_t*)wcs32ncpy((wchar32_t*)s1, (const wchar32_t*)s2, n);
}

size_t wcsnlen(const wchar_t* s, size_t maxlen) {
  return wcs32nlen((const wchar32_t*)s, maxlen);
}

wchar_t* wcspbrk(const wchar_t* s1, const wchar_t* s2) {
  return (wchar_t*)wcs32pbrk((const wchar32_t*)s1, (const wchar32_t*)s2);
}

wchar_t* wcsrchr(const wchar_t* s, wchar_t c) {
  return (wchar_t*)wcs32rchr((const wchar32_t*)s, c);
}

size_t wcsspn(const wchar_t* s1, const wchar_t* s2) {
  return wcs32spn((const wchar32_t*)s1, (const wchar32_t*)s2);
}

wchar_t* wcsstr(const wchar_t* s1, const wchar_t* s2) {
  return (wchar_t*)wcs32str((const wchar32_t*)s1, (const wchar32_t*)s2);
}

wchar_t* wmemchr(const wchar_t* s, wchar_t c, size_t n) {
  return (wchar_t*)w32memchr((const wchar32_t*)s, c, n);
}

int wmemcmp(const wchar_t* s1, const wchar_t* s2, size_t n) {
  return w32memcmp((const wchar32_t*)s1, (const wchar32_t*)s2, n);
}

wchar_t* wmemcpy(wchar_t* s1, const wchar_t* s2, size_t n) {
  return (wchar_t*)w32memcpy((wchar32_t*)s1, (const wchar32_t*)s2, n);
}

wchar_t* wmemmove(wchar_t* s1, const wchar_t* s2, size_t n) {
  return (wchar_t*)w32memmove((wchar32_t*)s1, (const wchar32_t*)s2, n);
}

wchar_t* wmemset(wchar_t* s, wchar_t c, size_t n) {
  return (wchar_t*)w32memset((wchar32_t*)s, c, n);
}

int iswspace(wint_t ch) {
  return ch == ' ';
}

int towlower(wint_t ch) {
  return tolower(ch);
}
#endif /*WITH_WCSXXX*/
