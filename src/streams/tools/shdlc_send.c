#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/platform.h"
#include "streams/inet/iostream_tcp.h"
#include "streams/noisy/iostream_noisy.h"
#include "streams/shdlc/iostream_shdlc.h"
#include "tkc/socket_helper.h"

void do_send(tk_iostream_t* tcp, const char* msg, uint32_t times) {
  uint32_t i = 0;
  int32_t s_ret = 0;
  int32_t r_ret = 0;
  char buff[1024 * 50];
  tk_iostream_t* noisy = tk_iostream_noisy_create(tcp);
  tk_iostream_t* iostream = tk_iostream_shdlc_create(noisy);
  tk_istream_t* istream = tk_iostream_get_istream(iostream);
  tk_ostream_t* ostream = tk_iostream_get_ostream(iostream);

  for (i = 0; i < times; i++) {
    tk_snprintf(buff, sizeof(buff), "(%u) %s", i, msg);

    s_ret = tk_ostream_write(ostream, (uint8_t*)buff, strlen(buff));
    memset(buff, 0x00, sizeof(buff));
    r_ret = tk_istream_read(istream, (uint8_t*)buff, sizeof(buff));

    log_debug("send=%d recv=%d data: %s\n", s_ret, r_ret, buff);
  }

  object_unref(OBJECT(iostream));

  return;
}

int main(int argc, char* argv[]) {
  int port = 0;
  int sock = 0;
  uint32_t times = 10;
  char* content = NULL;
  const char* msg = NULL;
  const char* host = NULL;

  if (argc != 5) {
    printf("Usage: %s host port msg|@filename times\n", argv[0]);
    return 0;
  }

  socket_init();
  platform_prepare();
  TK_ENABLE_CONSOLE();

  host = argv[1];
  port = tk_atoi(argv[2]);
  msg = argv[3];
  times = tk_atoi(argv[4]);

  if (msg[0] == '@') {
    uint32_t size = 0;
    const char* filename = msg + 1;
    content = file_read(filename, &size);
    msg = content;
  }

  sock = tcp_connect(host, port);
  socket_set_blocking(sock, FALSE);
  do_send(tk_iostream_tcp_create(sock), msg, times);

  TKMEM_FREE(content);

  socket_deinit();

  return 0;
}
