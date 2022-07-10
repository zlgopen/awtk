#include "widgets/grid.h"
#include "widgets/button.h"
#include "gtest/gtest.h"

TEST(Grid, basic1) {
  widget_t* w = grid_create(NULL, 10, 20, 300, 120);
  grid_t* g = GRID(w);

  ASSERT_EQ(grid_set_show_grid(w, TRUE), RET_OK);
  ASSERT_EQ(g->show_grid, TRUE);

  ASSERT_EQ(grid_set_show_grid(w, FALSE), RET_OK);
  ASSERT_EQ(g->show_grid, FALSE);

  ASSERT_EQ(grid_set_rows(w, 2), RET_OK);
  ASSERT_EQ(g->rows, 2);

  ASSERT_EQ(grid_set_columns_definition(w, "col(w=100,margin=2)"), RET_OK);
  ASSERT_STREQ(g->columns_definition, "col(w=100,margin=2)");
  ASSERT_EQ(g->cols_definition.size, 1);

  widget_destroy(w);
  idle_dispatch();
}

TEST(Grid, layout) {
  widget_t* w = grid_create(NULL, 10, 20, 200, 80);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  widget_t* b3 = button_create(w, 0, 0, 0, 0);
  widget_t* b4 = button_create(w, 0, 0, 0, 0);

  grid_t* g = GRID(w);

  grid_set_rows(w, 2);
  ASSERT_EQ(grid_set_columns_definition(w, "col(w=0.4,margin=2);col(w=0.6,margin=2);"), RET_OK);
  ASSERT_EQ(g->cols_definition.size, 2);
  widget_layout(w);

  ASSERT_EQ(b1->x, 2);
  ASSERT_EQ(b1->y, 2);
  ASSERT_EQ(b1->w, 76);
  ASSERT_EQ(b1->h, 36);

  ASSERT_EQ(b2->x, 82);
  ASSERT_EQ(b2->y, 2);
  ASSERT_EQ(b2->w, 116);
  ASSERT_EQ(b2->h, 36);

  ASSERT_EQ(b3->x, 2);
  ASSERT_EQ(b3->y, 42);
  ASSERT_EQ(b3->w, 76);
  ASSERT_EQ(b3->h, 36);

  ASSERT_EQ(b4->x, 82);
  ASSERT_EQ(b4->y, 42);
  ASSERT_EQ(b4->w, 116);
  ASSERT_EQ(b4->h, 36);

  widget_destroy(w);
  idle_dispatch();
}

TEST(Grid, layout2) {
  widget_t* w = grid_create(NULL, 10, 20, 200, 80);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  widget_t* b3 = button_create(w, 0, 0, 0, 0);
  widget_t* b4 = button_create(w, 0, 0, 0, 0);

  grid_t* g = GRID(w);

  grid_set_rows(w, 2);
  ASSERT_EQ(grid_set_columns_definition(w, "col(w=80,margin=2);col(w=120,margin=2);"), RET_OK);
  ASSERT_EQ(g->cols_definition.size, 2);
  widget_layout(w);

  ASSERT_EQ(b1->x, 2);
  ASSERT_EQ(b1->y, 2);
  ASSERT_EQ(b1->w, 76);
  ASSERT_EQ(b1->h, 36);

  ASSERT_EQ(b2->x, 82);
  ASSERT_EQ(b2->y, 2);
  ASSERT_EQ(b2->w, 116);
  ASSERT_EQ(b2->h, 36);

  ASSERT_EQ(b3->x, 2);
  ASSERT_EQ(b3->y, 42);
  ASSERT_EQ(b3->w, 76);
  ASSERT_EQ(b3->h, 36);

  ASSERT_EQ(b4->x, 82);
  ASSERT_EQ(b4->y, 42);
  ASSERT_EQ(b4->w, 116);
  ASSERT_EQ(b4->h, 36);

  widget_destroy(w);
  idle_dispatch();
}

TEST(Grid, layout3) {
  widget_t* w = grid_create(NULL, 10, 20, 200, 80);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  widget_t* b3 = button_create(w, 0, 0, 0, 0);
  widget_t* b4 = button_create(w, 0, 0, 0, 0);

  grid_t* g = GRID(w);

  grid_set_rows(w, 2);
  ASSERT_EQ(grid_set_columns_definition(w, "col(w=80,margin=2);col(w=-80,margin=2);"), RET_OK);
  ASSERT_EQ(g->cols_definition.size, 2);
  widget_layout(w);

  ASSERT_EQ(b1->x, 2);
  ASSERT_EQ(b1->y, 2);
  ASSERT_EQ(b1->w, 76);
  ASSERT_EQ(b1->h, 36);

  ASSERT_EQ(b2->x, 82);
  ASSERT_EQ(b2->y, 2);
  ASSERT_EQ(b2->w, 116);
  ASSERT_EQ(b2->h, 36);

  ASSERT_EQ(b3->x, 2);
  ASSERT_EQ(b3->y, 42);
  ASSERT_EQ(b3->w, 76);
  ASSERT_EQ(b3->h, 36);

  ASSERT_EQ(b4->x, 82);
  ASSERT_EQ(b4->y, 42);
  ASSERT_EQ(b4->w, 116);
  ASSERT_EQ(b4->h, 36);

  widget_destroy(w);
  idle_dispatch();
}

TEST(Grid, layout4) {
  widget_t* w = grid_create(NULL, 10, 20, 200, 80);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  widget_t* b3 = button_create(w, 0, 0, 0, 0);
  widget_t* b4 = button_create(w, 0, 0, 0, 0);

  grid_t* g = GRID(w);

  grid_set_rows(w, 2);
  ASSERT_EQ(grid_set_columns_definition(w, "col(w=0.4,margin=2);col(w=-0.4,margin=2);"), RET_OK);
  ASSERT_EQ(g->cols_definition.size, 2);
  widget_layout(w);

  ASSERT_EQ(b1->x, 2);
  ASSERT_EQ(b1->y, 2);
  ASSERT_EQ(b1->w, 76);
  ASSERT_EQ(b1->h, 36);

  ASSERT_EQ(b2->x, 82);
  ASSERT_EQ(b2->y, 2);
  ASSERT_EQ(b2->w, 116);
  ASSERT_EQ(b2->h, 36);

  ASSERT_EQ(b3->x, 2);
  ASSERT_EQ(b3->y, 42);
  ASSERT_EQ(b3->w, 76);
  ASSERT_EQ(b3->h, 36);

  ASSERT_EQ(b4->x, 82);
  ASSERT_EQ(b4->y, 42);
  ASSERT_EQ(b4->w, 116);
  ASSERT_EQ(b4->h, 36);

  widget_destroy(w);
  idle_dispatch();
}

TEST(Grid, layout5) {
  widget_t* w = grid_create(NULL, 10, 20, 200, 80);
  widget_t* b1 = button_create(w, 0, 0, 0, 0);
  widget_t* b2 = button_create(w, 0, 0, 0, 0);
  widget_t* b3 = button_create(w, 0, 0, 0, 0);
  widget_t* b4 = button_create(w, 0, 0, 0, 0);

  grid_t* g = GRID(w);

  grid_set_rows(w, 2);
  ASSERT_EQ(
      grid_set_columns_definition(w, "col(w=0.4,margin=2);col(w=0,fill_available=true,margin=2);"),
      RET_OK);
  ASSERT_EQ(g->cols_definition.size, 2);
  widget_layout(w);

  ASSERT_EQ(b1->x, 2);
  ASSERT_EQ(b1->y, 2);
  ASSERT_EQ(b1->w, 76);
  ASSERT_EQ(b1->h, 36);

  ASSERT_EQ(b2->x, 82);
  ASSERT_EQ(b2->y, 2);
  ASSERT_EQ(b2->w, 116);
  ASSERT_EQ(b2->h, 36);

  ASSERT_EQ(b3->x, 2);
  ASSERT_EQ(b3->y, 42);
  ASSERT_EQ(b3->w, 76);
  ASSERT_EQ(b3->h, 36);

  ASSERT_EQ(b4->x, 82);
  ASSERT_EQ(b4->y, 42);
  ASSERT_EQ(b4->w, 116);
  ASSERT_EQ(b4->h, 36);

  widget_destroy(w);
  idle_dispatch();
}
