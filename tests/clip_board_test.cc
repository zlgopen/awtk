#include "gtest/gtest.h"
#include "clip_board/clip_board_default.h"

#include <string>
using std::string;

TEST(ClipBoard, basic) {
  clip_board_t* save_cl = clip_board();
  clip_board_t* cl = clip_board_default_create();
  ASSERT_EQ(clip_board_set(cl), RET_OK);

  ASSERT_EQ(string(clip_board_get_text()), string(""));

  ASSERT_EQ(clip_board_set_text("abc"), RET_OK);
  ASSERT_EQ(string(clip_board_get_text()), string("abc"));

  ASSERT_EQ(clip_board_set_text("123abc"), RET_OK);
  ASSERT_EQ(string(clip_board_get_text()), string("123abc"));

  ASSERT_EQ(clip_board_set(save_cl), RET_OK);
  clip_board_destroy(cl);
}
