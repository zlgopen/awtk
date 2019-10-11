#include "gtest/gtest.h"

#include "tkc/thread.h"
#include "tkc/socket_pair.h"
#include "streams/shdlc/iostream_shdlc.h"
#include "streams/inet/iostream_tcp.h"

static uint8_t rbuff[7];
static uint8_t sbuff[7] = {1, 2, 3, 4, 5, 6, 7};

static void* server_thread_entry1(void* args) {
  tk_iostream_t* b_io = TK_IOSTREAM(args);
  tk_istream_t* is = tk_iostream_get_istream(b_io);
  tk_ostream_t* os = tk_iostream_get_ostream(b_io);

  assert(tk_ostream_write(os, sbuff, sizeof(sbuff)) == sizeof(sbuff));

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
  tk_thread_t* t = tk_thread_create(server_thread_entry1, b_io);

  tk_thread_start(t);
  ASSERT_EQ(tk_istream_read_len(is, rbuff, sizeof(rbuff), 30000), sizeof(rbuff));
  tk_thread_join(t);

  OBJECT_UNREF(a_tcp);
  OBJECT_UNREF(a_io);
  OBJECT_UNREF(b_tcp);
  OBJECT_UNREF(b_io);
}
