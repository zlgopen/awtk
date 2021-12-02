#include "tkc/platform.h"
#include "streams/inet/iostream_mbedtls.h"
#include "streams/inet/mbedtls_client.h"

int main(int argc, char* argv[]) {
  int32_t ret = 0;
  mbedtls_conn_t* conn = NULL;
  const char* request = "GET / HTTP/1.1\r\nHost: localhost\r\n";

  platform_prepare();
  conn = mbedtls_conn_client_create("localhost", "4433", (const uint8_t*)mbedtls_test_cas_pem,
                                    mbedtls_test_cas_pem_len);

  if (conn != NULL) {
    char buff[1024] = {0};
    tk_iostream_t* io = tk_iostream_mbedtls_create(conn);
    tk_istream_t* in = tk_iostream_get_istream(io);
    tk_ostream_t* out = tk_iostream_get_ostream(io);
    ret = tk_ostream_write(out, request, strlen(request));
    log_debug("send %d %s\n", ret, request);

    ret = tk_istream_read(in, buff, sizeof(buff));
    log_debug("recv %d %s\n", ret, buff);

    TK_OBJECT_UNREF(io);
  }

  return 0;
}
