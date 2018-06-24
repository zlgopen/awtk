
#include "base/array.h"
#include "base/theme.h"
#include "base/widget.h"
#include "base/buffer.h"
#include "tools/theme_gen/theme_gen.h"
#include "gtest/gtest.h"
#include <stdlib.h>

#include <string>
using std::string;

void GenThemeData(uint8_t* buff, uint32_t size, uint32_t type_nr, uint32_t state_nr,
                  uint32_t name_nr) {
  ThemeGen g;
  for (uint32_t type = 0; type < type_nr; type++) {
    for (uint32_t state = 0; state < state_nr; state++) {
      Style s(type, 0, state);
      for (uint32_t name = 0; name < name_nr; name++) {
        char str[32];
        snprintf(str, sizeof(str), "%d", name);
        s.AddInt(name, name);
        s.AddString(name, str);
      }
      g.AddStyle(s);
    }
  }

  g.Output(buff, size);
}

TEST(Theme, saveLoad) {
  uint8_t v8;
  uint16_t v16;
  uint32_t v32;
  uint8_t buff[7];
  uint8_t* p = buff;

  save_uint8(p, 0x1f);
  save_uint16(p, 0x2f2f);
  save_uint32(p, 0x3f3f3f3f);

  p = buff;
  load_uint8(p, v8);
  ASSERT_EQ(v8, 0x1f);

  load_uint16(p, v16);
  ASSERT_EQ(v16, 0x2f2f);

  load_uint32(p, v32);
  ASSERT_EQ(v32, 0x3f3f3f3f);
}

TEST(Theme, basic) {
  uint8_t buff[4*10240];
  uint32_t type_nr = WIDGET_NR;
  uint32_t state_nr = 5;
  uint32_t name_nr = 5;
  theme_t t;
  style_t s;

  GenThemeData(buff, sizeof(buff), type_nr, state_nr, name_nr);
  t.data = buff;

  for (uint32_t type = WIDGET_NONE + 1; type < WIDGET_NR; type++) {
    for (uint32_t state = 0; state < state_nr; state++) {
      s.data = theme_find_style(&t, type, 0, state);
      ASSERT_EQ(s.data != NULL, true);
      for (uint32_t name = 0; name < name_nr; name++) {
        uint32_t v = style_get_int(&s, name, 0);
        ASSERT_EQ(v, name);
        v = atoi(style_get_str(&s, name, NULL));
        ASSERT_EQ(v, name);
      }
    }
  }
}
