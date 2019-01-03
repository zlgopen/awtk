#include "tkc/matrix.h"
#include "gtest/gtest.h"

TEST(Matrix, basic) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  matrix_init(&m);
  matrix_transform_point(&m, 10, 20, &x, &y);

  ASSERT_EQ(x, 10);
  ASSERT_EQ(y, 20);
}

TEST(Matrix, translate) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  matrix_init(&m);
  matrix_translate(&m, 10, 20);
  matrix_transform_point(&m, 10, 20, &x, &y);

  ASSERT_EQ(x, 20);
  ASSERT_EQ(y, 40);
}

TEST(Matrix, rotate) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  matrix_init(&m);
  matrix_rotate(&m, 3.14159);
  matrix_transform_point(&m, 10, 20, &x, &y);
  ASSERT_EQ(x, -10);
  ASSERT_EQ(y, -20);

  matrix_rotate(&m, 3.14159);
  matrix_transform_point(&m, 10, 20, &x, &y);
  ASSERT_EQ(x, 10);
  ASSERT_EQ(y, 20);
}

TEST(Matrix, scale) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  matrix_init(&m);
  matrix_scale(&m, 2, 2);
  matrix_transform_point(&m, 10, 20, &x, &y);
  ASSERT_EQ(x, 20);
  ASSERT_EQ(y, 40);

  matrix_scale(&m, 0.5, 0.5);
  matrix_transform_point(&m, 10, 20, &x, &y);
  ASSERT_EQ(x, 10);
  ASSERT_EQ(y, 20);
}

TEST(Matrix, all) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  matrix_init(&m);
  matrix_scale(&m, 2, 2);
  matrix_translate(&m, 2, 2);
  matrix_scale(&m, 2, 2);
  matrix_rotate(&m, 3.14159 / 2);

  matrix_transform_point(&m, 10, 20, &x, &y);
  ASSERT_EQ(x, -76);
  ASSERT_EQ(y, 44);
}
