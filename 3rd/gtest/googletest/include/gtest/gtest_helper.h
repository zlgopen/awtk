#include "tkc/str.h"
#include "gtest/gtest.h"

#ifndef GTEST_HELPER_H
#define GTEST_HELPER_H

static inline void ASSERT_STREQ_UNIX(const char* str1, const char* str2) {
  str_t s1;
  str_t s2;
  str_init(&s1, 0);
  str_init(&s2, 0);
  str_set(&s1, str1);
  str_replace(&s1, "\r\n", "\n");
  str_replace(&s1, "\r", "\n");
  str_set(&s2, str2);
  str_replace(&s2, "\r\n", "\n");
  str_replace(&s2, "\r", "\n");

  ASSERT_STREQ(s1.str, s2.str);

  str_reset(&s1);
  str_reset(&s2);
}

#endif/*GTEST_HELPER_H*/
