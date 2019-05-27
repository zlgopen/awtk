#include "base/keys.h"
#include "base/shortcut.h"
#include "gtest/gtest.h"

TEST(ShortCut, ctrl) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_a);
  s1.ctrl = TRUE;
  s1.lctrl = TRUE;
  s1.rctrl = TRUE;

  shortcut_init_with_str(&s2, "ctrl+a");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "ctrl+home");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, lctrl) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_a);
  s1.lctrl = TRUE;

  shortcut_init_with_str(&s2, "lctrl+a");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "lctrl+home");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, rctrl) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_A);
  s1.rctrl = TRUE;

  shortcut_init_with_str(&s2, "rctrl+A");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "rctrl+HOME");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, alt) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_a);
  s1.alt = TRUE;
  s1.lalt = TRUE;
  s1.ralt = TRUE;

  shortcut_init_with_str(&s2, "alt+a");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "alt+home");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, lalt) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_a);
  s1.lalt = TRUE;

  shortcut_init_with_str(&s2, "lalt+a");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "lalt+home");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, ralt) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_A);
  s1.ralt = TRUE;

  shortcut_init_with_str(&s2, "ralt+A");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "ralt+HOME");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, shift) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_a);
  s1.shift = TRUE;
  s1.lshift = TRUE;
  s1.rshift = TRUE;

  shortcut_init_with_str(&s2, "shift+a");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "shift+home");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, lshift) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_a);
  s1.lshift = TRUE;

  shortcut_init_with_str(&s2, "lshift+a");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "lshift+home");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, rshift) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_A);
  s1.rshift = TRUE;

  shortcut_init_with_str(&s2, "rshift+A");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "rshift+HOME");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, cmd) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_a);
  s1.cmd = TRUE;

  shortcut_init_with_str(&s2, "cmd+a");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "cmd+home");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, menu) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_a);
  s1.menu = TRUE;

  shortcut_init_with_str(&s2, "menu+a");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "menu+home");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}

TEST(ShortCut, lshift_lctrl) {
  shortcut_t s1;
  shortcut_t s2;

  shortcut_init(&s1, TK_KEY_a);
  s1.lctrl = TRUE;
  s1.lshift = TRUE;

  shortcut_init_with_str(&s2, "lshift+lctrl+a");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);

  s1.key = TK_KEY_HOME;
  shortcut_init_with_str(&s2, "lshift+lctrl+home");
  ASSERT_EQ(shortcut_equal(&s1, &s2), TRUE);
}
