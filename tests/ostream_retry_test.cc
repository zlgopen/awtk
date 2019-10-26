#include "gtest/gtest.h"
#include "streams/mem/ostream_mem.h"
#include "streams/misc/ostream_retry.h"

TEST(OStreamRetry, basic) {
  uint8_t buff[13];

  tk_ostream_t* osmem = tk_ostream_mem_create(buff, sizeof(buff) - 1, 0, FALSE);
  tk_ostream_mem_t* mem = TK_OSTREAM_MEM(osmem);
  tk_ostream_t* osretry = tk_ostream_retry_create(osmem);
  tk_ostream_retry_t* retry = TK_OSTREAM_RETRY(osretry);

  memset(buff, 0x00, sizeof(buff));

  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"1234", 4), 4);
  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"1234", 4), 4);
  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"1234", 4), 4);
  ASSERT_EQ(retry->pending_bytes, 0);
  ASSERT_EQ(retry->pending_packets, 0);
  ASSERT_STREQ((char*)buff, "123412341234");
  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"abcd", 4), 4);
  ASSERT_EQ(retry->pending_bytes, 4);
  ASSERT_EQ(retry->pending_packets, 1);
  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"ABCD", 4), 4);
  ASSERT_EQ(retry->pending_bytes, 8);
  ASSERT_EQ(retry->pending_packets, 2);

  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"1234", 4), 4);
  ASSERT_EQ(retry->pending_bytes, 12);
  ASSERT_EQ(retry->pending_packets, 3);
  mem->cursor = 0;
  tk_ostream_flush(osretry);
  ASSERT_EQ(retry->pending_bytes, 0);
  ASSERT_EQ(retry->pending_packets, 0);
  ASSERT_STREQ((char*)buff, "abcdABCD1234");

  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"1234", 4), 4);
  ASSERT_EQ(retry->pending_bytes, 4);
  ASSERT_EQ(retry->pending_packets, 1);
  mem->cursor = 0;
  memset(buff, 0x00, sizeof(buff));
  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"abcd", 4), 4);
  ASSERT_EQ(retry->pending_bytes, 0);
  ASSERT_EQ(retry->pending_packets, 0);
  ASSERT_STREQ((char*)buff, "1234abcd");

  OBJECT_UNREF(osmem);
  OBJECT_UNREF(osretry);
}

TEST(OStreamRetry, max_retry) {
  uint8_t buff[13];

  tk_ostream_t* osmem = tk_ostream_mem_create(buff, sizeof(buff) - 1, 0, FALSE);
  tk_ostream_t* osretry = tk_ostream_retry_create(osmem);
  tk_ostream_retry_t* retry = TK_OSTREAM_RETRY(osretry);
  tk_ostream_retry_set_max_retry_times(osretry, 1);
  ASSERT_EQ(retry->max_retry_times, 1);

  memset(buff, 0x00, sizeof(buff));

  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"1234", 4), 4);
  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"1234", 4), 4);
  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"1234", 4), 4);
  ASSERT_EQ(retry->pending_bytes, 0);
  ASSERT_EQ(retry->pending_packets, 0);
  ASSERT_STREQ((char*)buff, "123412341234");
  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"abcd", 4), 4);
  ASSERT_EQ(retry->pending_bytes, 4);
  ASSERT_EQ(retry->pending_packets, 1);

  ASSERT_EQ(tk_ostream_flush(osretry), RET_OK);
  ASSERT_EQ(retry->discard_bytes, 4);
  ASSERT_EQ(retry->discard_packets, 1);

  ASSERT_EQ(tk_ostream_write(osretry, (const uint8_t*)"abcd", 4), 4);
  ASSERT_EQ(tk_ostream_flush(osretry), RET_OK);
  ASSERT_EQ(retry->discard_bytes, 8);
  ASSERT_EQ(retry->discard_packets, 2);

  OBJECT_UNREF(osmem);
  OBJECT_UNREF(osretry);
}
