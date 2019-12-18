#include "gtest/gtest.h"

#include "tkc/thread.h"
#include "tkc/socket_pair.h"
#include "streams/shdlc/iostream_shdlc.h"
#include "streams/inet/iostream_tcp.h"
#include "streams/noisy/iostream_noisy.h"

uint8_t rbuff[2048];
uint8_t sbuff[2048];
uint32_t data_size;
uint32_t compress_threshold = 1024;

static void* server_thread_entry1(void* args) {
  tk_iostream_t* b_io = TK_IOSTREAM(args);
  tk_ostream_t* os = tk_iostream_get_ostream(b_io);

  object_set_prop_int(OBJECT(os), TK_STREAM_PROP_COMPRESS_THRESHOLD, compress_threshold);
  assert(tk_ostream_write(os, sbuff, data_size) == data_size);

  return NULL;
}

TEST(IOStreamSHDLC, basic) {
  int socks[2];
  tk_socketpair(socks);
  tk_iostream_t* a_tcp = tk_iostream_tcp_create(socks[0]);
  tk_iostream_t* a_io = tk_iostream_shdlc_create(a_tcp);
  tk_iostream_t* b_tcp = tk_iostream_tcp_create(socks[1]);
  tk_iostream_t* b_io = tk_iostream_shdlc_create(b_tcp);
  tk_istream_t* is = tk_iostream_get_istream(a_io);
  tk_ostream_t* os = tk_iostream_get_ostream(a_io);

  ASSERT_EQ(object_get_prop_int(OBJECT(is), TK_STREAM_PROP_FD, 0), socks[0]);
  ASSERT_EQ(object_get_prop_int(OBJECT(os), TK_STREAM_PROP_FD, 0), socks[0]);

  ASSERT_EQ(object_set_prop_int(OBJECT(is), TK_STREAM_PROP_TIMEOUT, 1234), RET_OK);
  ASSERT_EQ(object_set_prop_int(OBJECT(os), TK_STREAM_PROP_TIMEOUT, 1235), RET_OK);
  ASSERT_EQ(object_set_prop_int(OBJECT(os), TK_STREAM_PROP_COMPRESS_THRESHOLD, 2048), RET_OK);
  ASSERT_EQ(object_set_prop_int(OBJECT(os), TK_STREAM_PROP_RETRY_TIMES, 12), RET_OK);

  ASSERT_EQ(object_get_prop_int(OBJECT(is), TK_STREAM_PROP_TIMEOUT, 0), 1234);
  ASSERT_EQ(object_get_prop_int(OBJECT(os), TK_STREAM_PROP_TIMEOUT, 0), 1235);
  ASSERT_EQ(object_get_prop_int(OBJECT(os), TK_STREAM_PROP_COMPRESS_THRESHOLD, 0), 2048);
  ASSERT_EQ(object_get_prop_int(OBJECT(os), TK_STREAM_PROP_RETRY_TIMES, 0), 12);

  OBJECT_UNREF(a_tcp);
  OBJECT_UNREF(a_io);
  OBJECT_UNREF(b_tcp);
  OBJECT_UNREF(b_io);
}

static void gen_data() {
  uint32_t n = sizeof(sbuff);
  for (uint32_t i = 0; i < n; i++) {
    sbuff[i] = random() / 256;
  }
}

TEST(IOStreamSHDLC, small) {
  int socks[2];
  tk_socketpair(socks);
  tk_iostream_t* a_tcp = tk_iostream_tcp_create(socks[0]);
  tk_iostream_t* a_io = tk_iostream_shdlc_create(a_tcp);
  tk_iostream_t* b_tcp = tk_iostream_tcp_create(socks[1]);
  tk_iostream_t* b_io = tk_iostream_shdlc_create(b_tcp);
  tk_istream_t* is = tk_iostream_get_istream(a_io);
  tk_thread_t* t = tk_thread_create(server_thread_entry1, b_io);

  data_size = 64;
  gen_data();
  tk_thread_start(t);
  ASSERT_EQ(tk_istream_read_len(is, rbuff, data_size, 30000), data_size);
  ASSERT_EQ(memcmp(rbuff, sbuff, data_size), 0);

  tk_thread_join(t);

  OBJECT_UNREF(a_tcp);
  OBJECT_UNREF(a_io);
  OBJECT_UNREF(b_tcp);
  OBJECT_UNREF(b_io);
}

TEST(IOStreamSHDLC, large) {
  int socks[2];
  tk_socketpair(socks);
  tk_iostream_t* a_tcp = tk_iostream_tcp_create(socks[0]);
  tk_iostream_t* a_io = tk_iostream_shdlc_create(a_tcp);
  tk_iostream_t* b_tcp = tk_iostream_tcp_create(socks[1]);
  tk_iostream_t* b_io = tk_iostream_shdlc_create(b_tcp);
  tk_istream_t* is = tk_iostream_get_istream(a_io);
  tk_ostream_t* os = tk_iostream_get_ostream(a_io);
  tk_thread_t* t = tk_thread_create(server_thread_entry1, b_io);

  data_size = sizeof(rbuff);
  gen_data();
  tk_thread_start(t);
  ASSERT_EQ(tk_istream_read_len(is, rbuff, data_size, 30000), data_size);
  ASSERT_EQ(memcmp(rbuff, sbuff, data_size), 0);

  tk_thread_join(t);

  OBJECT_UNREF(a_tcp);
  OBJECT_UNREF(a_io);
  OBJECT_UNREF(b_tcp);
  OBJECT_UNREF(b_io);
}

static void* server_thread_entry_noisy(void* args) {
  tk_iostream_t* b_io = TK_IOSTREAM(args);
  tk_ostream_t* os = tk_iostream_get_ostream(b_io);

  object_set_prop_int(OBJECT(os), TK_STREAM_PROP_COMPRESS_THRESHOLD, compress_threshold);
  assert(tk_ostream_write(os, sbuff, data_size) == data_size);

  return NULL;
}

TEST(IOStreamSHDLC, noisy) {
  int socks[2];
  tk_socketpair(socks);
  tk_iostream_t* a_tcp = tk_iostream_tcp_create(socks[0]);
  tk_iostream_t* a_io = tk_iostream_shdlc_create(a_tcp);

  tk_iostream_t* b_tcp = tk_iostream_tcp_create(socks[1]);
  tk_iostream_t* b_noisy = tk_iostream_noisy_create(b_tcp);
  tk_iostream_t* b_io = tk_iostream_shdlc_create(b_noisy);

  tk_istream_t* is = tk_iostream_get_istream(a_io);
  tk_thread_t* t = tk_thread_create(server_thread_entry_noisy, b_io);

  data_size = 4;
  gen_data();
  tk_thread_start(t);
  ASSERT_EQ(tk_istream_read_len(is, rbuff, data_size, 30000), data_size);
  ASSERT_EQ(memcmp(rbuff, sbuff, data_size), 0);

  tk_thread_join(t);

  OBJECT_UNREF(a_tcp);
  OBJECT_UNREF(a_io);
  OBJECT_UNREF(b_tcp);
  OBJECT_UNREF(b_io);
}
