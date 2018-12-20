#include "widgets/button.h"
#include "base/canvas.h"
#include "base/widget.h"
#include "base/layout.h"
#include "gtest/gtest.h"
#include <stdlib.h>
#include "ui_loader/ui_serializer.h"

#include <string>

using std::string;

TEST(UISerializer, basic) {
  str_t str;
  widget_t* w = button_create(NULL, 10, 20, 30, 40);

  str_init(&str, 1024);
  widget_to_xml(w, &str);

  ASSERT_EQ(string(str.str),
            "<button x=\"10\" y=\"20\" w=\"30\" h=\"40\" repeat=\"0\">\n</button>\n");

  str_reset(&str);
  widget_destroy(w);
}

TEST(UISerializer, layout_self) {
  str_t str;
  widget_t* w = button_create(NULL, 10, 20, 30, 40);

  widget_set_self_layout_params(w, "right:100", "middle:10", "fill", "10%");
  str_init(&str, 1024);
  widget_to_xml(w, &str);

  ASSERT_EQ(string(str.str),
            "<button x=\"10\" y=\"20\" w=\"30\" h=\"40\" repeat=\"0\" "
            "self_layout=\"default(x=r:100,y=m:10,w=0,h=10%)\">\n</button>\n");

  str_reset(&str);
  widget_destroy(w);
}

TEST(UISerializer, layout_self1) {
  str_t str;
  widget_t* w = button_create(NULL, 10, 20, 30, 40);

  widget_set_self_layout_params(w, "center:100", "bottom:10", "fill", "10%");
  str_init(&str, 1024);
  widget_to_xml(w, &str);

  ASSERT_EQ(string(str.str),
            "<button x=\"10\" y=\"20\" w=\"30\" h=\"40\" repeat=\"0\" "
            "self_layout=\"default(x=c:100,y=b:10,w=0,h=10%)\">\n</button>\n");

  str_reset(&str);
  widget_destroy(w);
}
