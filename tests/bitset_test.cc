#include "tkc/bitset.h"
#include "tkc/raw_darray.h"
#include "gtest/gtest.h"

TEST(Bitset, consts) {
  ASSERT_EQ(TK_BITSET_BITS_PER_BYTE, 8);
  ASSERT_EQ(TK_BITSET_BYTES_FOR_BITS(0), 0);
  ASSERT_EQ(TK_BITSET_BYTES_FOR_BITS(1), 1);
  ASSERT_EQ(TK_BITSET_BYTES_FOR_BITS(7), 1);
  ASSERT_EQ(TK_BITSET_BYTES_FOR_BITS(8), 1);
  ASSERT_EQ(TK_BITSET_BYTES_FOR_BITS(9), 2);
  ASSERT_EQ(TK_BITSET_BYTES_FOR_BITS(64), 8);
  ASSERT_EQ(TK_BITSET_BYTES_FOR_BITS(65), 9);
  /*nr的类型由调用方决定：传64位就不会在nr+7处溢出(宏内部没有强转)*/
  ASSERT_EQ(TK_BITSET_BYTES_FOR_BITS((uint64_t)0xFFFFFFFFu), (uint64_t)0x20000000);

  /*每个元素占1位*/
  ASSERT_EQ(TK_BITSET_POS(1, 0, 0), 0);
  ASSERT_EQ(TK_BITSET_POS(1, 7, 0), 7);
  ASSERT_EQ(TK_BITSET_POS(1, 8, 0), 8);
  /*每个元素占3位*/
  ASSERT_EQ(TK_BITSET_POS(3, 0, 2), 2);
  ASSERT_EQ(TK_BITSET_POS(3, 1, 0), 3);
  ASSERT_EQ(TK_BITSET_POS(3, 3, 1), 10);
  /*每个元素占5位：32个元素占20个字节*/
  ASSERT_EQ(TK_BITSET_POS(5, 0, 4), 4);
  ASSERT_EQ(TK_BITSET_POS(5, 31, 4), 159);
  ASSERT_EQ(TK_BITSET_BYTES_FOR_BITS(32 * 5), 20);

  ASSERT_EQ(TK_BITSET_BYTE_INDEX(0), 0);
  ASSERT_EQ(TK_BITSET_BYTE_INDEX(7), 0);
  ASSERT_EQ(TK_BITSET_BYTE_INDEX(8), 1);
  ASSERT_EQ(TK_BITSET_BYTE_INDEX(17), 2);

  ASSERT_EQ(TK_BITSET_BIT_INDEX(0), 0);
  ASSERT_EQ(TK_BITSET_BIT_INDEX(7), 7);
  ASSERT_EQ(TK_BITSET_BIT_INDEX(8), 0);
  ASSERT_EQ(TK_BITSET_BIT_INDEX(17), 1);
}

TEST(Bitset, ops) {
  uint8_t buff[3];
  const uint8_t* cbits = buff;

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(TK_BITSET_BYTE(buff, 0), buff);
  ASSERT_EQ(TK_BITSET_BYTE(buff, 7), buff);
  ASSERT_EQ(TK_BITSET_BYTE(buff, 17), buff + 2);

  TK_BITSET_OP(TK_SET_BIT, buff, 0);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, buff, 0), 1);
  ASSERT_EQ(buff[0], 0x01);

  TK_BITSET_OP(TK_SET_BIT, buff, 7);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, buff, 7), 1);
  ASSERT_EQ(buff[0], 0x81);

  TK_BITSET_OP(TK_SET_BIT, buff, 8);
  TK_BITSET_OP(TK_SET_BIT, buff, 16);
  ASSERT_EQ(buff[1], 0x01);
  ASSERT_EQ(buff[2], 0x01);

  TK_BITSET_OP(TK_CLEAR_BIT, buff, 7);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, buff, 7), 0);
  ASSERT_EQ(buff[0], 0x01);

  TK_BITSET_OP(TK_TOGGLE_BIT, buff, 1);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, buff, 1), 1);
  TK_BITSET_OP(TK_TOGGLE_BIT, buff, 1);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, buff, 1), 0);

  TK_SET_OR_CLEAR_BIT(*TK_BITSET_BYTE(buff, 2), TK_BITSET_BIT_INDEX(2), TRUE);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, buff, 2), 1);
  TK_SET_OR_CLEAR_BIT(*TK_BITSET_BYTE(buff, 2), TK_BITSET_BIT_INDEX(2), FALSE);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, buff, 2), 0);

  /*和TK_SET_BIT等宏的结果一致*/
  memset(buff, 0x00, sizeof(buff));
  TK_BITSET_OP(TK_SET_BIT, buff, 6);
  TK_SET_BIT(buff[0], 6);
  ASSERT_EQ(buff[0], 0x40);
  ASSERT_EQ(TK_TEST_BIT(buff[0], 6), TK_BITSET_OP(TK_TEST_BIT, buff, 6));

  /*每个元素占3位：第3个元素的第1位*/
  memset(buff, 0x00, sizeof(buff));
  TK_BITSET_OP(TK_SET_BIT, buff, TK_BITSET_POS(3, 3, 1));
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, buff, TK_BITSET_POS(3, 3, 1)), 1);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, buff, TK_BITSET_POS(3, 3, 0)), 0);
  ASSERT_EQ(buff[1], 0x04);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, buff, TK_BITSET_POS(3, 4, 0)), 0);

  /*传const指针也能写：TK_BITSET_BYTE会去掉const限定*/
  memset(buff, 0x00, sizeof(buff));
  TK_BITSET_OP(TK_SET_BIT, cbits, 5);
  ASSERT_EQ(buff[0], 0x20);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, cbits, 5), 1);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, cbits, 4), 0);
}

/*和raw_darray组合使用：字节数由元素个数算出，位图当作普通字节缓冲区*/
TEST(Bitset, raw_darray) {
  uint32_t i = 0;
  uint32_t nr = 67;
  uint32_t bytes = TK_BITSET_BYTES_FOR_BITS(nr);
  TK_RAW_DARRAY_T(uint8_t) bits = TK_RAW_DARRAY_CREATE(uint8_t, bytes);

  ASSERT_TRUE(bits != NULL);
  ASSERT_EQ(TK_RAW_DARRAY_CAPACITY(bits), bytes);

  ASSERT_EQ(TK_RAW_DARRAY_RESIZE(bits, bytes), RET_OK);
  ASSERT_EQ(TK_RAW_DARRAY_SIZE(bits), bytes);

  /*create/resize都不清零数据区*/
  memset(bits, 0x00, TK_RAW_DARRAY_SIZE(bits));
  for (i = 0; i < nr; i++) {
    ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, bits, i), 0);
  }

  TK_BITSET_OP(TK_SET_BIT, bits, nr - 1);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, bits, nr - 1), 1);
  /*最后一位落在第9个字节的bit2，尾部的空闲位保持0*/
  ASSERT_EQ(TK_BITSET_BYTE(bits, nr - 1), bits + 8);
  ASSERT_EQ(TK_BITSET_BIT_INDEX(nr - 1), 2);
  ASSERT_EQ(bits[8], 0x04);

  ASSERT_EQ(TK_RAW_DARRAY_DESTROY(bits), RET_OK);
}

typedef enum _bitset_bit_t {
  BITSET_BIT_USED = 0,
  BITSET_BIT_LOCKED,
  BITSET_BIT_DIRTY,
  BITSET_BIT_EXPANDED,
  BITSET_BIT_RESERVED,
  BITSET_BIT_NR
} bitset_bit_t;

static const uint32_t s_nr = 32;

static int32_t find_free_block(const uint8_t* bits) {
  uint32_t i = 0;

  for (i = 0; i < s_nr; i++) {
    if (!TK_BITSET_OP(TK_TEST_BIT, bits, TK_BITSET_POS(BITSET_BIT_NR, i, BITSET_BIT_USED))) {
      return (int32_t)i;
    }
  }

  return -1;
}

TEST(Bitset, allocator_usage) {
  uint8_t bits[TK_BITSET_BYTES_FOR_BITS(s_nr * BITSET_BIT_NR)];
  uint32_t i = 0;

  ASSERT_EQ(BITSET_BIT_NR, 5);
  ASSERT_EQ(sizeof(bits), 20);

  memset(bits, 0x00, sizeof(bits));
  ASSERT_EQ(find_free_block(bits), 0);

  for (i = 0; i < 3; i++) {
    TK_BITSET_OP(TK_SET_BIT, bits, TK_BITSET_POS(BITSET_BIT_NR, i, BITSET_BIT_USED));
  }
  ASSERT_EQ(find_free_block(bits), 3);

  TK_BITSET_OP(TK_CLEAR_BIT, bits, TK_BITSET_POS(BITSET_BIT_NR, 1, BITSET_BIT_USED));
  ASSERT_EQ(find_free_block(bits), 1);

  /*每块5位：块0的USED在bit0，块1的EXPANDED在bit8，块2的USED在bit10*/
  TK_BITSET_OP(TK_SET_BIT, bits, TK_BITSET_POS(BITSET_BIT_NR, 1, BITSET_BIT_EXPANDED));
  ASSERT_EQ(bits[0], 0x01);
  ASSERT_EQ(bits[1], 0x05);
  ASSERT_EQ(find_free_block(bits), 1);

  for (i = 0; i < s_nr; i++) {
    TK_BITSET_OP(TK_SET_BIT, bits, TK_BITSET_POS(BITSET_BIT_NR, i, BITSET_BIT_USED));
  }
  ASSERT_EQ(find_free_block(bits), -1);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, bits, TK_BITSET_POS(BITSET_BIT_NR, 0, BITSET_BIT_USED)), 1);
  ASSERT_EQ(TK_BITSET_OP(TK_TEST_BIT, bits, TK_BITSET_POS(BITSET_BIT_NR, 0, BITSET_BIT_DIRTY)), 0);

  /*只设置每个块的RESERVED位：第0块落在bit4，第31块落在最后一个字节的bit7*/
  memset(bits, 0x00, sizeof(bits));
  for (i = 0; i < s_nr; i++) {
    TK_BITSET_OP(TK_SET_BIT, bits, TK_BITSET_POS(BITSET_BIT_NR, i, BITSET_BIT_RESERVED));
  }
  ASSERT_EQ(find_free_block(bits), 0);
  ASSERT_EQ(bits[0], 0x10);
  ASSERT_EQ(bits[19], 0x84);

  memset(bits, 0x00, sizeof(bits));
  ASSERT_EQ(find_free_block(bits), 0);
}
