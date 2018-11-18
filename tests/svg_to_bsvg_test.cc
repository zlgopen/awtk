#include "base/rect.h"
#include "base/utils.h"
#include "gtest/gtest.h"
#include "svg/svg_to_bsvg.h"

static const char* s_template = \
      "<?xml version=\"1.0\" standalone=\"no\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
      "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg t=\"1542187942584\" "
      "class=\"icon\" style=\"\" viewBox=\"0 0 1024 1024\" version=\"1.1\" "
      "xmlns=\"http://www.w3.org/2000/svg\" p-id=\"1907\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"16\" height=\"16\"><defs><style "
      "type=\"text/css\"></style></defs>%s</svg>";

static char s_buff[1024];

TEST(SVGToBSVG, path) {
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const char* content = "<path d=\"M409.6 281.6v460.8l128-128h179.2L409.6 281.6z\" p-id=\"1908\">";
  tk_snprintf(s_buff, sizeof(s_buff)-1, s_template, content);

  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);
}


TEST(SVGToBSVG, rect) {
  uint32_t* out = NULL;
  uint32_t out_length = 0;
  const char* content = "<rect x=\"10\" y=\"10\" width=\"30\" height=\"30\" stroke=\"black\" fill=\"transparent\" stroke-width=\"5\"/>";

  tk_snprintf(s_buff, sizeof(s_buff)-1, s_template, content);
  svg_to_bsvg(s_buff, strlen(s_buff), &out, &out_length);
}
