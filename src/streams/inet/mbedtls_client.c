#ifdef WITH_MBEDTLS

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mbedtls_client.h"

static ret_t mbedtls_conn_client_destroy(mbedtls_conn_t* conn) {
  mbedtls_conn_client_t* client = (mbedtls_conn_client_t*)conn;

  mbedtls_ssl_close_notify(&(conn->ssl));
  mbedtls_net_free(&(client->server_fd));
  mbedtls_x509_crt_free(&(client->cacert));
  mbedtls_ssl_free(&(conn->ssl));
  mbedtls_ssl_config_free(&(client->conf));
  mbedtls_ctr_drbg_free(&(client->ctr_drbg));
  mbedtls_entropy_free(&(client->entropy));

  return RET_OK;
}

mbedtls_conn_t* mbedtls_conn_client_create(const char* host, const char* port,
                                           const uint8_t* cas_pem, uint32_t cas_pem_len) {
  int ret = 0;
  int flags = 0;
  mbedtls_conn_t* conn = NULL;
  const char* pers = TK_MBEDTLS_PERS;
  mbedtls_conn_client_t* client = NULL;
  return_value_if_fail(host != NULL && port != NULL, NULL);
  client = TKMEM_ZALLOC(mbedtls_conn_client_t);
  return_value_if_fail(client != NULL, NULL);
  conn = (mbedtls_conn_t*)client;

  conn->destroy = mbedtls_conn_client_destroy;

#if defined(MBEDTLS_DEBUG_C)
  mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

  /*
   * 0. Initialize the RNG and the session data
   */
  mbedtls_net_init(&(client->server_fd));
  mbedtls_ssl_init(&(conn->ssl));
  mbedtls_ssl_config_init(&(client->conf));
  mbedtls_x509_crt_init(&(client->cacert));
  mbedtls_ctr_drbg_init(&(client->ctr_drbg));

  log_debug("\n  . Seeding the random number generator...");

  mbedtls_entropy_init(&(client->entropy));
  if ((ret = mbedtls_ctr_drbg_seed(&(client->ctr_drbg), mbedtls_entropy_func, &(client->entropy),
                                   (const unsigned char*)pers, strlen(pers))) != 0) {
    log_debug(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret);
    goto error;
  }

  log_debug(" ok\n");

  /*
   * 0. Initialize certificates
   */
  if (cas_pem != NULL) {
    log_debug("  . Loading the CA root certificate ...");
    ret = mbedtls_x509_crt_parse(&(client->cacert), (const unsigned char*)cas_pem, cas_pem_len);
    if (ret < 0) {
      log_debug(" failed\n  !  mbedtls_x509_crt_parse returned -0x%x\n\n", (unsigned int)-ret);
      goto error;
    }
  }
  log_debug(" ok (%d skipped)\n", ret);

  /*
   * 1. Start the connection
   */
  log_debug("  . Connecting to tcp/%s/%s...", host, port);
  if ((ret = mbedtls_net_connect(&(client->server_fd), host, port, MBEDTLS_NET_PROTO_TCP)) != 0) {
    log_debug(" failed\n  ! mbedtls_net_connect returned %d\n\n", ret);
    goto error;
  }

  log_debug(" ok\n");

  /*
   * 2. Setup stuff
   */
  log_debug("  . Setting up the SSL/TLS structure...");

  if ((ret = mbedtls_ssl_config_defaults(&(client->conf), MBEDTLS_SSL_IS_CLIENT,
                                         MBEDTLS_SSL_TRANSPORT_STREAM,
                                         MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
    log_debug(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret);
    goto error;
  }

  log_debug(" ok\n");

  /*
   * OPTIONAL is not optimal for security,
   * but makes interop easier in this simplified example 
   */
  mbedtls_ssl_conf_authmode(&(client->conf), MBEDTLS_SSL_VERIFY_OPTIONAL);
  mbedtls_ssl_conf_ca_chain(&(client->conf), &(client->cacert), NULL);
  mbedtls_ssl_conf_rng(&(client->conf), mbedtls_ctr_drbg_random, &(client->ctr_drbg));
  mbedtls_ssl_conf_dbg(&(client->conf), mbedtls_awtk_debug, NULL);

  if ((ret = mbedtls_ssl_setup(&(conn->ssl), &(client->conf))) != 0) {
    log_debug(" failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret);
    goto error;
  }

  if ((ret = mbedtls_ssl_set_hostname(&(conn->ssl), host)) != 0) {
    log_debug(" failed\n  ! mbedtls_ssl_set_hostname returned %d\n\n", ret);
    goto error;
  }

  mbedtls_ssl_set_bio(&(conn->ssl), &(client->server_fd), mbedtls_net_send, mbedtls_net_recv, NULL);

  /*
   * 4. Handshake
   */
  log_debug("  . Performing the SSL/TLS handshake...");

  while ((ret = mbedtls_ssl_handshake(&(conn->ssl))) != 0) {
    if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
      log_debug(" failed\n  ! mbedtls_ssl_handshake returned -0x%x\n\n", (unsigned int)-ret);
      goto error;
    }
  }

  log_debug(" ok\n");

  /*
   * 5. Verify the server certificate
   */
  log_debug("  . Verifying peer X.509 certificate...");

  /* In real life, we probably want to bail out when ret != 0 */
  if ((flags = mbedtls_ssl_get_verify_result(&(conn->ssl))) != 0) {
    char vrfy_buf[512];
    log_debug(" failed\n");
    mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", flags);
    log_debug("%s\n", vrfy_buf);
  } else {
    log_debug(" ok\n");
  }
  return conn;
error:
  mbedtls_conn_client_destroy(conn);

  return NULL;
}
#endif /*WITH_MBEDTLS*/
