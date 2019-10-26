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

  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 4);
  ASSERT_EQ(ring_buffer_size(rb), 4);
  ASSERT_EQ(ring_buffer_free_size(rb), 28);

  ASSERT_EQ(ring_buffer_write(rb, "abcd", 4), 4);
  ASSERT_EQ(ring_buffer_size(rb), 8);
  ASSERT_EQ(ring_buffer_free_size(rb), 24);

  ASSERT_EQ(ring_buffer_write_len(rb, "1234", 4), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 12);
  ASSERT_EQ(ring_buffer_free_size(rb), 20);

  ASSERT_EQ(ring_buffer_write_len(rb, "abcd", 4), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 16);
  ASSERT_EQ(ring_buffer_free_size(rb), 16);

  ASSERT_EQ(ring_buffer_write_len(rb, "1234abcd1234abcd", 16), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 32);
  ASSERT_EQ(ring_buffer_free_size(rb), 0);

  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 0);
  ASSERT_EQ(ring_buffer_size(rb), 32);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, write_read) {
  char buff[32];
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 4);
  ASSERT_EQ(ring_buffer_size(rb), 4);
  ASSERT_EQ(ring_buffer_free_size(rb), 28);

  ASSERT_EQ(ring_buffer_read_len(rb, buff, 8), RET_FAIL);
  ASSERT_EQ(ring_buffer_read(rb, buff, 4), 4);
  ASSERT_STREQ(buff, "1234");
  ASSERT_EQ(ring_buffer_size(rb), 0);
  ASSERT_EQ(ring_buffer_free_size(rb), 32);

  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 4);
  ASSERT_EQ(ring_buffer_size(rb), 4);
  ASSERT_EQ(ring_buffer_free_size(rb), 28);

  ASSERT_EQ(ring_buffer_write(rb, "abcd", 4), 4);
  ASSERT_EQ(ring_buffer_size(rb), 8);
  ASSERT_EQ(ring_buffer_free_size(rb), 24);

  ASSERT_EQ(ring_buffer_write_len(rb, "1234", 4), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 12);
  ASSERT_EQ(ring_buffer_free_size(rb), 20);

  ASSERT_EQ(ring_buffer_write_len(rb, "abcd", 4), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 16);
  ASSERT_EQ(ring_buffer_free_size(rb), 16);

  ASSERT_EQ(ring_buffer_write_len(rb, "1234abcd1234abcd", 16), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 32);
  ASSERT_EQ(ring_buffer_free_size(rb), 0);

  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 0);
  ASSERT_EQ(ring_buffer_size(rb), 32);
  ASSERT_EQ(ring_buffer_is_full(rb), TRUE);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, write_peek) {
  char buff[32];
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 4);
  ASSERT_EQ(ring_buffer_size(rb), 4);
  ASSERT_EQ(ring_buffer_free_size(rb), 28);

  ASSERT_EQ(ring_buffer_peek(rb, buff, 4), 4);
  ASSERT_STREQ(buff, "1234");
  ASSERT_EQ(ring_buffer_size(rb), 4);
  ASSERT_EQ(ring_buffer_free_size(rb), 28);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, write_reset) {
  char buff[32];
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(ring_buffer_write(rb, "1234", 4), 4);
  ASSERT_EQ(ring_buffer_size(rb), 4);
  ASSERT_EQ(ring_buffer_free_size(rb), 28);

  ASSERT_EQ(ring_buffer_reset(rb), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 0);
  ASSERT_EQ(ring_buffer_free_size(rb), 32);
  ASSERT_EQ(ring_buffer_is_empty(rb), TRUE);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, random) {
  char buff[32];
  uint32_t i = 0;
  uint32_t n = 10240;
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  memset(buff, 0x00, sizeof(buff));

  ASSERT_EQ(ring_buffer_write(rb, "1234abcd", 8), 8);
  ASSERT_EQ(ring_buffer_size(rb), 8);
  ASSERT_EQ(ring_buffer_free_size(rb), 24);

  for (i = 0; i < n; i++) {
    if (ring_buffer_write(rb, "1234abcd", 8) < 8) {
      ring_buffer_reset(rb);
      ASSERT_EQ(ring_buffer_write(rb, "1234abcd", 8), 8);
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
  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 16);
  ASSERT_EQ(ring_buffer_size(rb), 16);

  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 16);
  ASSERT_EQ(ring_buffer_size(rb), 32);

  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 16);
  ASSERT_EQ(ring_buffer_size(rb), 48);

  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 16);
  ASSERT_EQ(ring_buffer_size(rb), 64);

  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 0);
  ASSERT_EQ(ring_buffer_size(rb), 64);
  ASSERT_EQ(ring_buffer_is_full(rb), TRUE);

  ASSERT_EQ(ring_buffer_read_len(rb, buff, 64), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 0);
  ASSERT_EQ(ring_buffer_is_empty(rb), TRUE);

  ASSERT_EQ(ring_buffer_write(rb, "12345678abcdefgh", 16), 16);
  ASSERT_EQ(ring_buffer_size(rb), 16);

  ring_buffer_destroy(rb);
}

TEST(RingBuffer, write_skip_read) {
  char buff[32];
  ring_buffer_t* rb = ring_buffer_create(32, 32);

  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(ring_buffer_write(rb, "1234567890", 10), 10);
  ASSERT_EQ(ring_buffer_skip(rb, 4), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 6);
  ASSERT_EQ(ring_buffer_read(rb, buff, 4), 4);
  ASSERT_STREQ(buff, "5678");
  ASSERT_EQ(ring_buffer_size(rb), 2);

  ASSERT_EQ(ring_buffer_write(rb, "1234567890", 10), 10);
  ASSERT_EQ(ring_buffer_size(rb), 12);
  ASSERT_EQ(ring_buffer_write(rb, "1234567890", 10), 10);
  ASSERT_EQ(ring_buffer_size(rb), 22);
  ASSERT_EQ(ring_buffer_write(rb, "abcdABCD", 8), 8);
  ASSERT_EQ(ring_buffer_size(rb), 30);
  ASSERT_EQ(ring_buffer_skip(rb, 22), RET_OK);
  ASSERT_EQ(ring_buffer_size(rb), 8);

  ASSERT_EQ(ring_buffer_read(rb, buff, 4), 4);
  ASSERT_STREQ(buff, "abcd");
  ASSERT_EQ(ring_buffer_size(rb), 4);
  ASSERT_EQ(ring_buffer_read(rb, buff, 4), 4);
  ASSERT_STREQ(buff, "ABCD");
  ASSERT_EQ(ring_buffer_size(rb), 0);

  ring_buffer_destroy(rb);
}
