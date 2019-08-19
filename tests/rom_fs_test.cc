#include "tkc/rom_fs.h"
#include "gtest/gtest.h"

TEST(ROMFS, basic) {
  char rbuff[100];
  const char* buff = "hello world";
  rom_file_t* f = rom_fopen_buff((const uint8_t*)buff, sizeof(buff));
  ASSERT_EQ(rom_fread(rbuff, 2, 1, f), 1);
  ASSERT_EQ(memcmp(rbuff, "he", 2), 0);
  rom_fclose(f);
}
