#include "gtest/gtest.h"
#include "tkc/ring_buffer.h"

TEST(RingBuffer, basic) {
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  ASSERT_EQ(ring_buffer_is_empty(rb), TRUE);
  ASSERT_EQ(ring_buffer_is_full(rb), FALSE);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, write) {
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 4u);
  ASSERT_EQ(ring_buffer_size(rb), 4u);
  ASSERT_EQ(ring_buffer_free_size(rb), 28u);

  ASSERT_EQ(ring_buffer_write(rb, "abcd", 4), 4u);
  ASSERT_EQ(ring_buffer_size(rb), 8u);
  ASSERT_EQ(ring_buffer_free_size(rb), 24u);

  ASSERT_EQ(ring_buffer_write_len(rb, "1234", 4), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 12u);
  ASSERT_EQ(ring_buffer_free_size(rb), 20u);

  ASSERT_EQ(ring_buffer_write_len(rb, "abcd", 4), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 16u);
  ASSERT_EQ(ring_buffer_free_size(rb), 16u);

  ASSERT_EQ(ring_buffer_write_len(rb, "1234abcd1234abcd", 16), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 32u);
  ASSERT_EQ(ring_buffer_free_size(rb), 0u);

  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 0u);
  ASSERT_EQ(ring_buffer_size(rb), 32u);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, write_read) {
  char buff[32];
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 4u);
  ASSERT_EQ(ring_buffer_size(rb), 4u);
  ASSERT_EQ(ring_buffer_free_size(rb), 28u);

  ASSERT_EQ(ring_buffer_read_len(rb, buff, 8), RET_FAIL);
  ASSERT_EQ(ring_buffer_read(rb, buff, 4), 4u);
  ASSERT_STREQ(buff, "1234");
  ASSERT_EQ(ring_buffer_size(rb), 0u);
  ASSERT_EQ(ring_buffer_free_size(rb), 32u);

  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 4u);
  ASSERT_EQ(ring_buffer_size(rb), 4u);
  ASSERT_EQ(ring_buffer_free_size(rb), 28u);

  ASSERT_EQ(ring_buffer_write(rb, "abcd", 4), 4u);
  ASSERT_EQ(ring_buffer_size(rb), 8u);
  ASSERT_EQ(ring_buffer_free_size(rb), 24u);

  ASSERT_EQ(ring_buffer_write_len(rb, "1234", 4), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 12u);
  ASSERT_EQ(ring_buffer_free_size(rb), 20u);

  ASSERT_EQ(ring_buffer_write_len(rb, "abcd", 4), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 16u);
  ASSERT_EQ(ring_buffer_free_size(rb), 16u);

  ASSERT_EQ(ring_buffer_write_len(rb, "1234abcd1234abcd", 16), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 32u);
  ASSERT_EQ(ring_buffer_free_size(rb), 0u);

  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 0u);
  ASSERT_EQ(ring_buffer_size(rb), 32u);
  ASSERT_EQ(ring_buffer_is_full(rb), TRUE);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, write_peek) {
  char buff[32];
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 4u);
  ASSERT_EQ(ring_buffer_size(rb), 4u);
  ASSERT_EQ(ring_buffer_free_size(rb), 28u);

  ASSERT_EQ(ring_buffer_peek(rb, buff, 4), 4u);
  ASSERT_STREQ(buff, "1234");
  ASSERT_EQ(ring_buffer_size(rb), 4u);
  ASSERT_EQ(ring_buffer_free_size(rb), 28u);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, write_reset) {
  char buff[32];
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 4u);
  ASSERT_EQ(ring_buffer_size(rb), 4u);
  ASSERT_EQ(ring_buffer_free_size(rb), 28u);

  ASSERT_EQ(ring_buffer_reset(rb), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 0u);
  ASSERT_EQ(ring_buffer_free_size(rb), 32u);
  ASSERT_EQ(ring_buffer_is_empty(rb), TRUE);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, random) {
  char buff[32];
  uint32_t i = 0;
  uint32_t n = 10240;
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  memset(buff, 0x00, sizeof(buff));

  ASSERT_EQ(ring_buffer_write(rb, "1234abcd", 8), 8u);
  ASSERT_EQ(ring_buffer_size(rb), 8u);
  ASSERT_EQ(ring_buffer_free_size(rb), 24u);

  for (i = 0; i < n; i++) {
    if (ring_buffer_write(rb, "1234abcd", 8) < 8) {
      ring_buffer_reset(rb);
      ASSERT_EQ(ring_buffer_write(rb, "1234abcd", 8), 8u);
    }

    uint32_t size = random() % ring_buffer_size(rb);
    ASSERT_EQ(ring_buffer_read(rb, buff, size), size);
  }

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, extendable) {
  char buff[64 + 1];
  ring_buffer_t* rb = ring_buffer_create(32, 64);

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 16u);
  ASSERT_EQ(ring_buffer_size(rb), 16u);

  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 16u);
  ASSERT_EQ(ring_buffer_size(rb), 32u);

  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 16u);
  ASSERT_EQ(ring_buffer_size(rb), 48u);

  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 16u);
  ASSERT_EQ(ring_buffer_size(rb), 64u);

  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 0u);
  ASSERT_EQ(ring_buffer_size(rb), 64u);
  ASSERT_EQ(ring_buffer_is_full(rb), TRUE);

  ASSERT_EQ(ring_buffer_read_len(rb, buff, 64), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 0u);
  ASSERT_EQ(ring_buffer_is_empty(rb), TRUE);

  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 16u);
  ASSERT_EQ(ring_buffer_size(rb), 16u);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, write_skip_read) {
  char buff[32];
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(ring_buffer_write(rb, "1234567890", 10), 10u);
  ASSERT_EQ(ring_buffer_skip(rb, 4), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 6u);
  ASSERT_EQ(ring_buffer_read(rb, buff, 4), 4u);
  ASSERT_STREQ(buff, "5678");
  ASSERT_EQ(ring_buffer_size(rb), 2u);

  ASSERT_EQ(ring_buffer_write(rb, "1234567890", 10), 10u);
  ASSERT_EQ(ring_buffer_size(rb), 12u);
  ASSERT_EQ(ring_buffer_write(rb, "1234567890", 10), 10u);
  ASSERT_EQ(ring_buffer_size(rb), 22u);
  ASSERT_EQ(ring_buffer_write(rb, "abcdABCD", 8), 8u);
  ASSERT_EQ(ring_buffer_size(rb), 30u);
  ASSERT_EQ(ring_buffer_skip(rb, 22), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 8u);

  ASSERT_EQ(ring_buffer_read(rb, buff, 4), 4u);
  ASSERT_STREQ(buff, "abcd");
  ASSERT_EQ(ring_buffer_size(rb), 4u);
  ASSERT_EQ(ring_buffer_read(rb, buff, 4), 4u);
  ASSERT_STREQ(buff, "ABCD");
  ASSERT_EQ(ring_buffer_size(rb), 0u);

  ring_buffer_destroy(rb);
}
