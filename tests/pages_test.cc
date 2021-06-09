#include "widgets/pages.h"
#include "widgets/view.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

static ret_t pages_on_change(void* ctx, event_t* e) {
  string& s = *(string*)ctx;

  if (e->type == EVT_VALUE_CHANGED) {
    s += "changed:";
  } else if (e->type == EVT_VALUE_WILL_CHANGE) {
    s += "will_change:";
  }

  return RET_OK;
}

TEST(Pages, basic) {
  string str;
  widget_t* pages = pages_create(NULL, 0, 0, 100, 100);
  widget_t* p0 = view_create(pages, 0, 0, 100, 100);
  widget_t* p1 = view_create(pages, 0, 0, 100, 100);
  widget_t* p2 = view_create(pages, 0, 0, 100, 100);

  widget_set_name(p0, "p0");
  widget_set_name(p1, "p1");
  widget_set_name(p2, "p2");

  widget_on(pages, EVT_VALUE_CHANGED, pages_on_change, &str);
  widget_on(pages, EVT_VALUE_WILL_CHANGE, pages_on_change, &str);

  pages_set_active(pages, 0);
  ASSERT_EQ(PAGES(pages)->active, 0);
  str = "";

  pages_set_active(pages, 1);
  ASSERT_EQ(PAGES(pages)->active, 1);
  ASSERT_EQ(str, "will_change:changed:");

  pages_set_active_by_name(pages, "p2");
  ASSERT_EQ(PAGES(pages)->active, 2);
  ASSERT_EQ(str, "will_change:changed:will_change:changed:");

  pages_set_active_by_name(pages, "not found");
  ASSERT_EQ(PAGES(pages)->active, 2);

  widget_destroy(pages);
}

TEST(Pages, cast) {
  widget_t* pages = pages_create(NULL, 0, 0, 100, 100);

  ASSERT_EQ(pages, pages_cast(pages));

  widget_destroy(pages);
}
