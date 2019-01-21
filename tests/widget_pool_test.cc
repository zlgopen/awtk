#include "gtest/gtest.h"
#include "base/widget_pool.h"
#include "widgets/label.h"

static widget_vtable_t s_enable_vtable;
static widget_vtable_t s_disable_vtable;

static widget_vtable_t* vtable_init(widget_vtable_t* vt, const char* type, bool_t enable_pool) {
  vt->type = type;
  vt->size = sizeof(widget_t);
  vt->enable_pool = enable_pool;

  return vt;
}

TEST(WidgetPool, basic) {
  widget_vtable_t* vt = vtable_init(&s_enable_vtable, "enable", TRUE);
  widget_vtable_t* d_vt = vtable_init(&s_disable_vtable, "disable", FALSE);

  widget_pool_t* pool = widget_pool_create(5);
  widget_t* e = widget_pool_create_widget(pool, vt);

  ASSERT_EQ(widget_pool_destroy_widget(pool, e), RET_OK);
  ASSERT_EQ(pool->cache_nr, 1);

  e = widget_pool_create_widget(pool, vt);
  ASSERT_EQ(pool->cache_nr, 0);

  ASSERT_EQ(widget_pool_destroy_widget(pool, e), RET_OK);
  ASSERT_EQ(pool->cache_nr, 1);

  e = widget_pool_create_widget(pool, d_vt);
  ASSERT_EQ(pool->cache_nr, 1);

  ASSERT_EQ(widget_pool_destroy_widget(pool, e), RET_OK);
  ASSERT_EQ(pool->cache_nr, 1);
  ASSERT_EQ(pool->cache_nr, widget_pool_count(pool));

  ASSERT_EQ(widget_pool_clear_cache(pool), RET_OK);
  ASSERT_EQ(pool->cache_nr, 0);
  ASSERT_EQ(pool->cache_nr, widget_pool_count(pool));

  widget_pool_destroy(pool);
}

TEST(WidgetPool, enable) {
  const widget_vtable_t* vt = vtable_init(&s_enable_vtable, "enable", TRUE);
  widget_pool_t* pool = widget_pool_create(5);
  widget_t* e = widget_pool_create_widget(pool, vt);

  ASSERT_EQ(widget_pool_destroy_widget(pool, e), RET_OK);
  ASSERT_EQ(pool->cache_nr, 1);

  e = widget_pool_create_widget(pool, vt);
  ASSERT_EQ(pool->cache_nr, 0);

  ASSERT_EQ(widget_pool_destroy_widget(pool, e), RET_OK);
  ASSERT_EQ(pool->cache_nr, 1);

  widget_pool_destroy(pool);
}

TEST(WidgetPool, disable) {
  const widget_vtable_t* vt = vtable_init(&s_disable_vtable, "disable", FALSE);
  widget_pool_t* pool = widget_pool_create(5);
  widget_t* e = widget_pool_create_widget(pool, vt);

  ASSERT_EQ(widget_pool_destroy_widget(pool, e), RET_OK);
  ASSERT_EQ(pool->cache_nr, 0);

  widget_pool_destroy(pool);
}

TEST(WidgetPool, disable_nr) {
  widget_t* widgets[100];
  widget_pool_t* pool = widget_pool_create(5);
  const widget_vtable_t* vt = vtable_init(&s_disable_vtable, "disable", FALSE);

  for (int32_t i = 0; i < ARRAY_SIZE(widgets); i++) {
    widgets[i] = widget_pool_create_widget(pool, vt);
    ASSERT_EQ(pool->cache_nr, 0);
  }

  for (int32_t i = 0; i < ARRAY_SIZE(widgets); i++) {
    ASSERT_EQ(widget_pool_destroy_widget(pool, widgets[i]), RET_OK);
    ASSERT_EQ(pool->cache_nr, 0);
  }

  widget_pool_destroy(pool);
}

TEST(WidgetPool, enable_nr) {
  widget_t* widgets[100];
  widget_pool_t* pool = widget_pool_create(ARRAY_SIZE(widgets));
  const widget_vtable_t* vt = vtable_init(&s_enable_vtable, "enable", TRUE);

  for (int32_t i = 0; i < ARRAY_SIZE(widgets); i++) {
    widgets[i] = widget_pool_create_widget(pool, vt);
    ASSERT_EQ(pool->cache_nr, 0);
  }

  for (int32_t i = 0; i < ARRAY_SIZE(widgets); i++) {
    ASSERT_EQ(widget_pool_destroy_widget(pool, widgets[i]), RET_OK);
    ASSERT_EQ(pool->cache_nr, i + 1);
  }

  widget_pool_destroy(pool);
}

TEST(WidgetPool, enable_nr_limit) {
  widget_t* widgets[100];
  widget_pool_t* pool = widget_pool_create(10);
  const widget_vtable_t* vt = vtable_init(&s_enable_vtable, "enable", TRUE);

  for (int32_t i = 0; i < ARRAY_SIZE(widgets); i++) {
    widgets[i] = widget_pool_create_widget(pool, vt);
    ASSERT_EQ(pool->cache_nr, 0);
  }

  for (int32_t i = 0; i < ARRAY_SIZE(widgets); i++) {
    ASSERT_EQ(widget_pool_destroy_widget(pool, widgets[i]), RET_OK);
    if (i < pool->max_cache_nr) {
      ASSERT_EQ(pool->cache_nr, i + 1);
    } else {
      ASSERT_EQ(pool->cache_nr, pool->max_cache_nr);
    }
  }

  widget_pool_destroy(pool);
}
