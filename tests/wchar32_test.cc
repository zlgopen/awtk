#include "tkc/wchar32.h"
#include "tkc/mem.h"
#include "gtest/gtest.h"

TEST(Wchar32, wcp32cpy) {
  wchar32_t src[10] = {0};
  wchar32_t dst[10] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wchar32_t* ret = wcp32cpy(dst, src);
  EXPECT_EQ(wcs32len(dst), 5);
  EXPECT_EQ(wcs32cmp(dst, src), 0);
  EXPECT_EQ(ret, dst + 5);
}

TEST(Wchar32, wcp32ncpy) {
  wchar32_t src[10] = {0};
  wchar32_t dst[10] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wchar32_t* ret = wcp32ncpy(dst, src, 5);
  EXPECT_EQ(wcs32len(dst), 5);
  EXPECT_EQ(wcs32cmp(dst, src), 0);
  EXPECT_EQ(ret, dst);
}

TEST(Wchar32, wcs32casecmp) {
  wchar32_t src[10] = {0};
  wchar32_t dst[10] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hEllo");

  EXPECT_EQ(wcs32casecmp(dst, src), 0);
  EXPECT_EQ(wcs32casecmp(dst, dst), 0);
}

TEST(Wchar32, wcs32cat) {
  wchar32_t src[10] = {0};
  wchar32_t dst[20] = {0};
  wchar32_t result[20] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hEllo");
  wcs32_init_with_wcs(result, L"hEllohello");

  wchar32_t* ret = wcs32cat(dst, src);
  EXPECT_EQ(wcs32cmp(ret, result), 0);
}

TEST(Wchar32, wcs32chr) {
  wchar32_t str[10] = {0};
  wcs32_init_with_wcs(str, L"hEllo");

  wchar32_t* ret = wcs32chr(str, L'E');
  EXPECT_EQ(wcs32cmp(ret, str + 1), 0);

  ret = wcs32chr(str, L'x');
  EXPECT_EQ(ret, (wchar32_t*)NULL);
}

TEST(Wchar32, wcs32cpy) {
  wchar32_t src[10] = {0};
  wchar32_t dst[20] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hEllo");

  wchar32_t* ret = wcs32cpy(dst, src);
  EXPECT_EQ(wcs32cmp(dst, src), 0);
  EXPECT_EQ(wcs32cmp(ret, src), 0);
}

TEST(Wchar32, wcs32cspn) {
  wchar32_t src[20] = {0};
  wchar32_t dst[20] = {0};
  wcs32_init_with_wcs(src, L"Hi, hello, world");
  wcs32_init_with_wcs(dst, L", ");

  size_t ret = wcs32cspn(src, dst);
  ASSERT_EQ(ret, 2);
}

TEST(Wchar32, wcs32dup) {
  wchar32_t src[20] = {0};
  wcs32_init_with_wcs(src, L"Hi, hello, world");

  wchar32_t* ret = wcs32dup(src);
  EXPECT_EQ(wcs32cmp(ret, src), 0);
  TKMEM_FREE(ret);
}


TEST(Wchar32, wcs32lcat) {
  wchar32_t src[10] = {0};
  wchar32_t dst[20] = {0};
  wchar32_t result[20] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hEllo");
  wcs32_init_with_wcs(result, L"hEllohello");

  size_t ret = wcs32lcat(dst, src, 5);
  EXPECT_EQ(wcs32cmp(dst, result), 0);
  EXPECT_EQ(ret, 5);
  
  wcs32_init_with_wcs(dst, L"hEllo");
  ret = wcs32lcat(dst, src, 30);
  EXPECT_EQ(wcs32cmp(dst, result), 0);
  EXPECT_EQ(ret, 5);
}


TEST(Wchar32, wcs32lcpy) {
  wchar32_t src[10] = {0};
  wchar32_t dst[20] = {0};
  wchar32_t result[20] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hEllo");
  wcs32_init_with_wcs(result, L"hEllohello");

  size_t ret = wcs32lcpy(dst, src, 5);
  EXPECT_EQ(wcs32cmp(dst, src), 0);
  EXPECT_EQ(ret, 5);
  
  ret = wcs32lcpy(dst, src, 50);
  EXPECT_EQ(wcs32cmp(dst, src), 0);
  EXPECT_EQ(ret, 5);
}

TEST(Wchar32, wcs32ncasecmp) {
  wchar32_t src[10] = {0};
  wchar32_t dst[10] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hEllo");

  EXPECT_EQ(wcs32ncasecmp(dst, src, 5), 0);
  EXPECT_EQ(wcs32ncasecmp(dst, dst, 5), 0);
}

TEST(Wchar32, wcs32ncat) {
  wchar32_t src[10] = {0};
  wchar32_t dst[20] = {0};
  wchar32_t result[20] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hEllo");
  wcs32_init_with_wcs(result, L"hEllohello");

  wchar32_t* ret = wcs32ncat(dst, src, 5);
  EXPECT_EQ(wcs32cmp(dst, result), 0);
  EXPECT_EQ(ret, dst);
  
  wcs32_init_with_wcs(dst, L"hEllo");
  ret = wcs32ncat(dst, src, 30);
  EXPECT_EQ(wcs32cmp(dst, result), 0);
  EXPECT_EQ(ret, dst);
}

TEST(Wchar32, wcs32ncmp) {
  wchar32_t src[10] = {0};
  wchar32_t dst[10] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hello");

  EXPECT_EQ(wcs32ncmp(dst, src, 5), 0);
  EXPECT_EQ(wcs32ncmp(dst, dst, 5), 0);
}

TEST(Wchar32, wcs32ncpy) {
  wchar32_t src[10] = {0};
  wchar32_t dst[20] = {0};
  wchar32_t result[20] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hEllo");
  wcs32_init_with_wcs(result, L"hEllohello");

  wchar32_t* ret = wcs32ncpy(dst, src, 5);
  EXPECT_EQ(wcs32cmp(dst, src), 0);
  EXPECT_EQ(ret, dst);
  
  ret = wcs32ncpy(dst, src, 50);
  EXPECT_EQ(wcs32cmp(dst, src), 0);
  EXPECT_EQ(ret, dst);
}

TEST(Wchar32, wcs32nlen) {
  wchar32_t src[10] = {0};
  wchar32_t dst[20] = {0};
  wchar32_t result[20] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hEllo");
  wcs32_init_with_wcs(result, L"hEllohello");
  EXPECT_EQ(wcs32nlen(dst, 5), 5);
  EXPECT_EQ(wcs32nlen(dst, 50), 5);

  EXPECT_EQ(wcs32nlen(result, 5), 5);
  EXPECT_EQ(wcs32nlen(result, 50), 10);

  EXPECT_EQ(wcs32nlen(src, 5), 5);
  EXPECT_EQ(wcs32nlen(src, 50), 5);
}

TEST(Wchar32, wcs32pbrk) {
  wchar32_t src[20] = {0};
  wchar32_t dst[20] = {0};
  wcs32_init_with_wcs(src, L"Hi, hello, world");
  wcs32_init_with_wcs(dst, L", ");

  wchar32_t* ret = wcs32pbrk(src, dst);
  EXPECT_EQ(wcs32cmp(ret, src + 2), 0);
}

TEST(Wchar32, wcs32rchr) {
  wchar32_t src[20] = {0};
  wchar32_t dst[20] = {0};
  wcs32_init_with_wcs(src, L"Hi, hello, world");
  wcs32_init_with_wcs(dst, L", ");

  wchar32_t* ret = wcs32rchr(src, L',');
  EXPECT_EQ(ret, src + 9);
}

TEST(Wchar32, wcs32spn) {
  wchar32_t src[20] = {0};
  wchar32_t dst[20] = {0};
  wcs32_init_with_wcs(src, L"Hi, hello, world");
  wcs32_init_with_wcs(dst, L"Hi");

  size_t ret = wcs32spn(src, dst);
  ASSERT_EQ(ret, 2);
}

TEST(Wchar32, wcs32str) {
  wchar32_t src[20] = {0};
  wchar32_t dst[20] = {0};
  wcs32_init_with_wcs(src, L"Hi, hello, world");
  wcs32_init_with_wcs(dst, L"hello");

  wchar32_t* ret = wcs32str(src, dst);
  EXPECT_EQ(wcs32cmp(ret, src + 4), 0);
  
  ret = wcs32str(dst, src);
  EXPECT_EQ(ret, (wchar32_t*)NULL);
}

TEST(Wchar32, w32memchr) {
  wchar32_t str[10] = {0};
  wcs32_init_with_wcs(str, L"hEllo");

  wchar32_t* ret = w32memchr(str, L'E', 5);
  EXPECT_EQ(wcs32cmp(ret, str + 1), 0);

  ret = w32memchr(str, L'x', 5);
  EXPECT_EQ(ret, (wchar32_t*)NULL);
}

TEST(Wchar32, w32memcmp) {
  wchar32_t src[10] = {0};
  wchar32_t dst[10] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wcs32_init_with_wcs(dst, L"hello");

  EXPECT_EQ(w32memcmp(dst, src, 5), 0);
  EXPECT_EQ(w32memcmp(dst, dst, 5), 0);
}

TEST(Wchar32, w32memcpy) {
  wchar32_t src[10] = {0};
  wchar32_t dst[10] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wchar32_t* ret = w32memcpy(dst, src, 5);
  EXPECT_EQ(wcs32len(dst), 5);
  EXPECT_EQ(wcs32cmp(dst, src), 0);
  EXPECT_EQ(ret, dst);
}

TEST(Wchar32, w32memmove) {
  wchar32_t src[10] = {0};
  wchar32_t dst[10] = {0};
  wcs32_init_with_wcs(src, L"hello");
  wchar32_t* ret = w32memmove(dst, src, 5);
  EXPECT_EQ(wcs32len(dst), 5);
  EXPECT_EQ(wcs32cmp(dst, src), 0);
  EXPECT_EQ(ret, dst);
}

TEST(Wchar32, w32memset) {
  wchar32_t src[10] = {0};
  wchar32_t dst[10] = {0};
  memset(dst, 0, sizeof(dst));
  wcs32_init_with_wcs(src, L"AAAAA");
  wchar32_t* ret = w32memset(dst, 'A', 5);
  EXPECT_EQ(wcs32len(dst), 5);
  EXPECT_EQ(wcs32cmp(dst, src), 0);
  EXPECT_EQ(ret, dst);
}