#include "base/custom_props.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "base/layout.h"
#include "font_dummy.h"
#include "lcd_log.h"
#include "gtest/gtest.h"
#include <stdlib.h>

TEST(CustomProps, basic) {
  value_t v1;
  value_t v2;
  custom_props_t* obj = custom_props_create(1);

  value_set_wstr(&v1, L"custom_props");
  ASSERT_EQ(custom_props_set(obj, WIDGET_PROP_TEXT, &v1), RET_OK);
  ASSERT_EQ(custom_props_get(obj, WIDGET_PROP_TEXT, &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  value_set_wstr(&v1, L"custom_props1");
  ASSERT_EQ(custom_props_set(obj, WIDGET_PROP_TEXT, &v1), RET_OK);
  ASSERT_EQ(custom_props_get(obj, WIDGET_PROP_TEXT, &v2), RET_OK);
  ASSERT_EQ(wcscmp(v1.value.wstr, v2.value.wstr), 0);

  value_set_int(&v1, 200);
  ASSERT_EQ(custom_props_set(obj, WIDGET_PROP_REPEAT, &v1), RET_OK);
  ASSERT_EQ(custom_props_get(obj, WIDGET_PROP_REPEAT, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_int(&v1, 201);
  ASSERT_EQ(custom_props_set(obj, WIDGET_PROP_REPEAT, &v1), RET_OK);
  ASSERT_EQ(custom_props_get(obj, WIDGET_PROP_REPEAT, &v2), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  value_set_str(&v1, "name");
  ASSERT_EQ(custom_props_set(obj, WIDGET_PROP_NAME, &v1), RET_OK);
  ASSERT_EQ(custom_props_get(obj, WIDGET_PROP_NAME, &v2), RET_OK);
  ASSERT_EQ(strcmp(v1.value.str, v2.value.str), 0);

  value_set_str(&v1, "name1");
  ASSERT_EQ(custom_props_set(obj, WIDGET_PROP_NAME, &v1), RET_OK);
  ASSERT_EQ(custom_props_get(obj, WIDGET_PROP_NAME, &v2), RET_OK);
  ASSERT_EQ(strcmp(v1.value.str, v2.value.str), 0);

  custom_props_destroy(obj);
}
