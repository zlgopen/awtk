#include "tkc/platform.h"
#include "streams/inet/iostream_mbedtls.h"
#include "streams/inet/mbedtls_server.h"

int main(int argc, char* argv[]) {
  int32_t ret = 0;
  mbedtls_server_t* server = NULL;

  platform_prepare();
  server =
      mbedtls_server_create("4433", (const uint8_t*)mbedtls_test_srv_crt, mbedtls_test_srv_crt_len,
                            (const uint8_t*)mbedtls_test_srv_key, mbedtls_test_srv_key_len,
                            (const uint8_t*)mbedtls_test_cas_pem, mbedtls_test_cas_pem_len);

  if (server != NULL) {
    char buff[1024];
    mbedtls_conn_t* conn = mbedtls_server_accept(server);
    tk_iostream_t* io = tk_iostream_mbedtls_create(conn);
    tk_istream_t* in = tk_iostream_get_istream(io);
    tk_ostream_t* out = tk_iostream_get_ostream(io);
    ret = tk_istream_read(in, buff, sizeof(buff));
    ret = tk_ostream_write(out, "hello\r\n", 7);

    OBJECT_UNREF(io);
    mbedtls_server_destroy(server);
  }

  return 0;
}
