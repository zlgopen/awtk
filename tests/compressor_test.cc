#include "gtest/gtest.h"
#include "compressors/compressor_miniz.h"

static void compressor_test(compressor_t* c) {
  wbuffer_t wb1;
  wbuffer_t wb2;
  const char* str =
      "12314314asdfkmasldfkzkjfasopfeiq02wp3ri23232342342424aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaa0";
  wbuffer_init_extendable(&wb1);
  wbuffer_init_extendable(&wb2);

  ASSERT_EQ(compressor_compress(c, str, strlen(str) + 1, &wb1), RET_OK);
  ASSERT_EQ(compressor_uncompress(c, wb1.data, wb1.cursor, &wb2), RET_OK);
  ASSERT_STREQ(str, (char*)(wb2.data));

  wbuffer_deinit(&wb1);
  wbuffer_deinit(&wb2);
}

TEST(Compressor, miniz) {
  compressor_t* c = compressor_miniz_create(COMPRESSOR_RATIO_FIRST);

  compressor_test(c);

  compressor_destroy(c);
}
