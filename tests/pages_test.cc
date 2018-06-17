#include "base/pages.h"
#include "base/view.h"
#include "gtest/gtest.h"

TEST(Pages, basic) {
  widget_t* pages = pages_create(NULL, 0, 0, 100, 100);
  widget_t* p0 = view_create(pages, 0, 0, 100, 100);
  widget_t* p1 = view_create(pages, 0, 0, 100, 100);
  widget_t* p2 = view_create(pages, 0, 0, 100, 100);

  widget_set_name(p0, "p0");
  widget_set_name(p1, "p1");
  widget_set_name(p2, "p2");

  ASSERT_EQ(PAGES(pages)->active, 0);

  pages_set_active(pages, 1);
  ASSERT_EQ(PAGES(pages)->active, 1);

  pages_set_active_by_name(pages, "p2");
  ASSERT_EQ(PAGES(pages)->active, 2);

  pages_set_active_by_name(pages, "not found");
  ASSERT_EQ(PAGES(pages)->active, 2);

  widget_destroy(pages);
}
