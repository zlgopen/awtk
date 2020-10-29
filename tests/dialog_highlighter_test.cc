#include "gtest/gtest.h"
#include "dialog_highlighters/dialog_highlighter_default.h"
#include "base/dialog_highlighter_factory.h"

TEST(DialogHighLigters, basic) {
  const char* args = "default";
  dialog_highlighter_factory_t* f = dialog_highlighter_factory();
  dialog_highlighter_t* h = dialog_highlighter_factory_create_highlighter(f, args, NULL);

  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  ASSERT_EQ(dh->start_alpha, dh->end_alpha);
  ASSERT_EQ(dh->start_alpha, 0);

  dialog_highlighter_destroy(h);
}

TEST(DialogHighLigters, alpha) {
  const char* args = "default(alpha=123)";
  dialog_highlighter_factory_t* f = dialog_highlighter_factory();
  dialog_highlighter_t* h = dialog_highlighter_factory_create_highlighter(f, args, NULL);

  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  ASSERT_EQ(dh->start_alpha, dh->end_alpha);
  ASSERT_EQ(dh->start_alpha, 123);

  dialog_highlighter_destroy(h);
}

TEST(DialogHighLigters, start_alpha) {
  const char* args = "default(start_alpha=123)";
  dialog_highlighter_factory_t* f = dialog_highlighter_factory();
  dialog_highlighter_t* h = dialog_highlighter_factory_create_highlighter(f, args, NULL);

  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  ASSERT_EQ(dh->start_alpha, 123);
  ASSERT_EQ(dh->end_alpha, 0);

  dialog_highlighter_destroy(h);
}

TEST(DialogHighLigters, null) {
  const char* args = NULL;
  dialog_highlighter_factory_t* f = dialog_highlighter_factory();
  dialog_highlighter_t* h = dialog_highlighter_factory_create_highlighter(f, args, NULL);
  ASSERT_EQ(h == NULL, true);
}

TEST(DialogHighLigters, end_alpha) {
  const char* args = "default(end_alpha=123)";
  dialog_highlighter_factory_t* f = dialog_highlighter_factory();
  dialog_highlighter_t* h = dialog_highlighter_factory_create_highlighter(f, args, NULL);

  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  ASSERT_EQ(dh->end_alpha, 123);
  ASSERT_EQ(dh->start_alpha, 0);

  dialog_highlighter_destroy(h);
}

TEST(DialogHighLigters, start_end_alpha) {
  const char* args = "default(start_alpha=100, end_alpha=123)";
  dialog_highlighter_factory_t* f = dialog_highlighter_factory();
  dialog_highlighter_t* h = dialog_highlighter_factory_create_highlighter(f, args, NULL);

  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  ASSERT_EQ(dh->end_alpha, 123);
  ASSERT_EQ(dh->start_alpha, 100);

  dialog_highlighter_destroy(h);
}

TEST(DialogHighLigters, not_exist) {
  const char* args = "not_exist";
  dialog_highlighter_factory_t* f = dialog_highlighter_factory();
  dialog_highlighter_t* h = dialog_highlighter_factory_create_highlighter(f, args, NULL);
  ASSERT_EQ(h == NULL, TRUE);
  dialog_highlighter_destroy(h);
}
