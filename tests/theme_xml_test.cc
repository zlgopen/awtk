#include "base/theme.h"
#include "base/widget.h"
#include "gtest/gtest.h"
#include "base/style_factory.h"
#include "base/theme_xml.h"
#include <stdlib.h>

#include <string>
using std::string;

TEST(ThemeXml, basic0) {
  theme_t* theme = NULL;
  const uint8_t* style_data = NULL;
  const char* str = "<widget><style><normal margin=\"-10\" bg_color=\"#fafbfc\"/></style></widget>";
  style_t* s = style_factory_create_style(NULL, theme_get_style_type(theme));

  theme = theme_xml_create(str);
  style_data = theme_find_style(theme, WIDGET_TYPE_NONE, TK_DEFAULT_STYLE, WIDGET_STATE_NORMAL);

  ASSERT_EQ(style_data != NULL, true);
  ASSERT_EQ(style_set_style_data(s, style_data, WIDGET_STATE_NORMAL), RET_OK);
  ASSERT_EQ(style_get_int(s, STYLE_ID_MARGIN, 0), -10);
  ASSERT_EQ(style_get_uint(s, STYLE_ID_BG_COLOR, 0), 0xfffcfbfau);
  style_destroy(s);
  theme_destroy(theme);
}
