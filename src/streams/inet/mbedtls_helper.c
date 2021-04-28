#ifdef WITH_MBEDTLS

#include "tkc/utils.h"
#include "mbedtls_helper.h"

void mbedtls_awtk_debug(void* ctx, int level, const char* file, int line, const char* str) {
  const char *p, *basename;

  /* Extract basename from file */
  for (p = basename = file; *p != '\0'; p++)
    if (*p == '/' || *p == '\\') basename = p + 1;

  log_debug("%s:%04d: |%d| %s", basename, line, level, str);
}

ret_t mbedtls_conn_destroy(mbedtls_conn_t* conn) {
  return_value_if_fail(conn != NULL, RET_BAD_PARAMS);

  if (conn->destroy != NULL) {
    conn->destroy(conn);
  }

  return RET_OK;
}

#endif /*WITH_MBEDTLS*/
