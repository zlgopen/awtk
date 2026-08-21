#include "tkc/crc.h"
#include "gtest/gtest.h"

TEST(crc, crc32_file) {
  ASSERT_EQ(tk_crc32_file("tests/testdata/test.png", 1024), 2727069354);
}

TEST(crc, crc16_modbus_sample_frame) {
  uint8_t data[] = {0x11, 0x05, 0x01, 0x30, 0xff, 0x00};
  ASSERT_EQ(tk_crc16_modbus(data, sizeof(data)), 0x598f);
}

TEST(crc, crc16_modbus_check_ascii) {
  const char* data = "123456789";
  ASSERT_EQ(tk_crc16_modbus(data, 9), 0x4b37);
}

TEST(crc, crc16_block_matches_fscript_vectors) {
  ASSERT_EQ(tk_crc16(PPPINITFCS16, "1", 1), 12173);
  ASSERT_EQ(tk_crc16(PPPINITFCS16, "123", 3), 25419);
}

TEST(crc, crc16_byte_matches_block) {
  const uint8_t data[] = {'1', '2', '3'};
  uint16_t crc = PPPINITFCS16;
  uint32_t i = 0;

  for (i = 0; i < sizeof(data); i++) {
    crc = tk_crc16_byte(crc, data[i]);
  }

  ASSERT_EQ(crc, tk_crc16(PPPINITFCS16, data, sizeof(data)));
  ASSERT_EQ(crc, 25419);
}

TEST(crc, crc16_empty_keeps_init) {
  ASSERT_EQ(tk_crc16(PPPINITFCS16, "", 0), PPPINITFCS16);
}

TEST(crc, crc16_check_ascii) {
  const char* data = "123456789";
  ASSERT_EQ(tk_crc16(PPPINITFCS16, data, 9), 0x6f91);
}

TEST(crc, crc32_block_matches_fscript_vectors) {
  ASSERT_EQ(tk_crc32(PPPINITFCS32, "1", 1), 2082672712u);
  ASSERT_EQ(tk_crc32(PPPINITFCS32, "123", 3), 2008521773u);
}

TEST(crc, crc32_byte_matches_block) {
  const uint8_t data[] = {'1', '2', '3'};
  uint32_t crc = PPPINITFCS32;
  uint32_t i = 0;

  for (i = 0; i < sizeof(data); i++) {
    crc = tk_crc32_byte(crc, data[i]);
  }

  ASSERT_EQ(crc, tk_crc32(PPPINITFCS32, data, sizeof(data)));
  ASSERT_EQ(crc, 2008521773u);
}

TEST(crc, crc32_empty_keeps_init) {
  ASSERT_EQ(tk_crc32(PPPINITFCS32, "", 0), PPPINITFCS32);
}

TEST(crc, crc32_null_data_keeps_init) {
  ASSERT_EQ(tk_crc32(PPPINITFCS32, NULL, 8), PPPINITFCS32);
}

TEST(crc, crc32_check_ascii) {
  const char* data = "123456789";
  ASSERT_EQ(tk_crc32(PPPINITFCS32, data, 9), 0x340bc6d9u);
}

TEST(crc, chksum_matches_fscript_vectors) {
  ASSERT_EQ(tk_chksum(0, "1", 1), 65486);
  ASSERT_EQ(tk_chksum(0, "123", 3), 65385);
}

TEST(crc, chksum_byte_accumulates) {
  uint16_t sum = 0;
  sum = tk_chksum_byte(sum, '1');
  sum = tk_chksum_byte(sum, '2');
  sum = tk_chksum_byte(sum, '3');
  ASSERT_EQ(sum, (uint16_t)('1' + '2' + '3'));
  ASSERT_EQ((uint16_t)(0xffff - sum), tk_chksum(0, "123", 3));
}
