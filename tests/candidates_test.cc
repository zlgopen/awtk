#include "keyboard/candidates.h"
#include "gtest/gtest.h"

TEST(Candidates, cast) {
  widget_t* w = candidates_create(NULL, 10, 20, 30, 40);

  ASSERT_EQ(w, candidates_cast(w));

  widget_destroy(w);
}
