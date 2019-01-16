#include "gtest/gtest.h"
#include "base/widget_pool.h"
#include "widgets/label.h"

static const widget_vtable_t s_enable_vtable = {
    .size = sizeof(widget_t), .type = "enable", .enable_pool = TRUE};

static const widget_vtable_t s_disable_vtable = {.size = sizeof(widget_t), .type = "disable"};

TEST(WidgetPool, basic) {
  const widget_vtable_t* vt = &s_enable_vtable;
  const widget_vtable_t* d_vt = &s_disable_vtable;

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
  const widget_vtable_t* vt = &s_enable_vtable;
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
  const widget_vtable_t* vt = &s_disable_vtable;
  widget_pool_t* pool = widget_pool_create(5);
  widget_t* e = widget_pool_create_widget(pool, vt);

  ASSERT_EQ(widget_pool_destroy_widget(pool, e), RET_OK);
  ASSERT_EQ(pool->cache_nr, 0);

  widget_pool_destroy(pool);
}

TEST(WidgetPool, disable_nr) {
  const widget_vtable_t* vt = &s_disable_vtable;
  widget_pool_t* pool = widget_pool_create(5);
  widget_t* widgets[100];

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
  const widget_vtable_t* vt = &s_enable_vtable;
  widget_t* widgets[100];
  widget_pool_t* pool = widget_pool_create(ARRAY_SIZE(widgets));

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
  const widget_vtable_t* vt = &s_enable_vtable;
  widget_t* widgets[100];
  widget_pool_t* pool = widget_pool_create(10);

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
