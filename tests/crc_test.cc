#include "tkc/crc.h"
#include "gtest/gtest.h"

TEST(crc, crc32_file) {
  ASSERT_EQ(tk_crc32_file("tests/testdata/test.png", 1024), 1567897941);
}

TEST(crc, crc32_modbus) {
  uint8_t data[] = {0x11, 0x05, 0x01, 0x30, 0xff, 0x00};
  ASSERT_EQ(tk_crc16_modbus(data, sizeof(data)), 0x598f);
}
