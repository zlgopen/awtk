#include "base/canvas.h"
#include "widgets/label.h"
#include "base/widget.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(Label, basic) {
  value_t v1;
  value_t v2;
  widget_t* l = label_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(widget_set_text(l, L"OK"), RET_OK);
  ASSERT_EQ(wcscmp(l->text.str, L"OK"), 0);
  ASSERT_EQ(widget_set_text(l, L"Cancel"), RET_OK);
  ASSERT_EQ(wcscmp(l->text.str, L"Cancel"), 0);

  value_set_wstr(&v1, L"label");
  ASSERT_EQ(widget_set_prop(l, WIDGET_PROP_TEXT, &v1), RET_OK);
  ASSERT_EQ(widget_get_prop(l, WIDGET_PROP_TEXT, &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  widget_destroy(l);
}

TEST(Label, value) {
  widget_t* l = label_create(NULL, 10, 20, 30, 40);
  ASSERT_EQ(widget_set_value(l, 100), RET_OK);
  ASSERT_EQ(widget_get_value(l), 100);

  ASSERT_EQ(widget_add_value(l, 100), RET_OK);
  ASSERT_EQ(widget_get_value(l), 200);

  widget_destroy(l);
}

TEST(Label, clone) {
  value_t v1;
  widget_t* w1 = label_create(NULL, 10, 20, 30, 40);

  widget_set_name(w1, "label");
  value_set_wstr(&v1, L"label");
  ASSERT_EQ(widget_set_prop(w1, WIDGET_PROP_TEXT, &v1), RET_OK);

  widget_t* w2 = widget_clone(w1, NULL);

  ASSERT_EQ(widget_equal(w1, w2), TRUE);

  widget_destroy(w1);
  widget_destroy(w2);
}

TEST(Label, line_breaker_count) {
  ASSERT_EQ(line_breaker_count(L"a"), 1);
  ASSERT_EQ(line_breaker_count(L"ab"), 1);
  ASSERT_EQ(line_breaker_count(L"a\nb"), 2);
  ASSERT_EQ(line_breaker_count(L"\na\nb"), 3);
  ASSERT_EQ(line_breaker_count(L"\na\nb\n"), 4);
  ASSERT_EQ(line_breaker_count(L"\na\nb\r"), 4);
  ASSERT_EQ(line_breaker_count(L"\na\nb\r\n"), 4);
  ASSERT_EQ(line_breaker_count(L"\na\rb\r"), 4);
  ASSERT_EQ(line_breaker_count(L"\na\rb\r\r"), 5);
  ASSERT_EQ(line_breaker_count(L"\na\r\nb\r\r"), 5);
  ASSERT_EQ(line_breaker_count(L"\na\r\rb\r\r"), 6);
}

static void test_str(const wchar_t* s1, const wchar_t* s2, uint32_t size) {
  if (size == 0) {
    ASSERT_EQ(*s1, 0);
  } else {
    ASSERT_EQ(memcmp(s1, s2, size * sizeof(wchar_t)), 0);
  }
}

static ret_t test_on_line(void* ctx, uint32_t index, const wchar_t* str, uint32_t size) {
  const wchar_t** strs = (const wchar_t**)ctx;
  test_str(strs[index], str, size);

  return RET_OK;
}

TEST(Label, line_breaker_break1) {
  const wchar_t* strs[] = {L"a", NULL};
  ASSERT_EQ(line_breaker_break(L"a", test_on_line, (void*)strs), RET_OK);
}

TEST(Label, line_breaker_break2) {
  const wchar_t* strs[] = {L"a", L"b", NULL};
  ASSERT_EQ(line_breaker_break(L"a\rb", test_on_line, (void*)strs), RET_OK);
  ASSERT_EQ(line_breaker_break(L"a\nb", test_on_line, (void*)strs), RET_OK);
  ASSERT_EQ(line_breaker_break(L"a\r\nb", test_on_line, (void*)strs), RET_OK);
}

TEST(Label, line_breaker_break3) {
  const wchar_t* strs[] = {L"a", L"b", L"", NULL};
  ASSERT_EQ(line_breaker_break(L"a\rb\r", test_on_line, (void*)strs), RET_OK);
  ASSERT_EQ(line_breaker_break(L"a\nb\n", test_on_line, (void*)strs), RET_OK);
  ASSERT_EQ(line_breaker_break(L"a\r\nb\r\n", test_on_line, (void*)strs), RET_OK);
}
