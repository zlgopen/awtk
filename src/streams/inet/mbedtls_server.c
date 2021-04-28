#ifdef WITH_MBEDTLS

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mbedtls_server.h"

static ret_t mbedtls_conn_server_destroy(mbedtls_conn_t* conn) {
  int ret = 0;
  mbedtls_conn_server_t* server_conn = (mbedtls_conn_server_t*)conn;

  while ((ret = mbedtls_ssl_close_notify(&(conn->ssl))) < 0) {
    if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
      log_debug(" failed\n  ! mbedtls_ssl_close_notify returned %d\n\n", ret);
      break;
    }
  }

  mbedtls_net_free(&(server_conn->client_fd));
  mbedtls_ssl_free(&(conn->ssl));

  return RET_OK;
}

mbedtls_conn_t* mbedtls_server_accept(mbedtls_server_t* server) {
  int ret = 0;
  mbedtls_conn_t* conn = NULL;
  mbedtls_conn_server_t* server_conn = NULL;
  return_value_if_fail(server != NULL, NULL);
  server_conn = TKMEM_ZALLOC(mbedtls_conn_server_t);
  return_value_if_fail(server_conn != NULL, NULL);
  conn = (mbedtls_conn_t*)server_conn;
  conn->destroy = mbedtls_conn_server_destroy;

  mbedtls_net_init(&(server_conn->client_fd));
  mbedtls_ssl_init(&(conn->ssl));

  if ((ret = mbedtls_ssl_setup(&(conn->ssl), &(server->conf))) != 0) {
    log_debug(" failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret);
    goto error;
  }

  log_debug("  . Waiting for a remote connection ...");

  if ((ret = mbedtls_net_accept(&(server->listen_fd), &(server_conn->client_fd), NULL, 0, NULL)) !=
      0) {
    log_debug(" failed\n  ! mbedtls_net_accept returned %d\n\n", ret);
    goto error;
  }

  mbedtls_ssl_set_bio(&(conn->ssl), &(server_conn->client_fd), mbedtls_net_send, mbedtls_net_recv,
                      NULL);

  log_debug("  . Performing the SSL/TLS handshake...");
  while ((ret = mbedtls_ssl_handshake(&(conn->ssl))) != 0) {
    if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
      log_debug(" failed\n  ! mbedtls_ssl_handshake returned %d\n\n", ret);
      goto error;
    }
  }

  log_debug(" ok\n");
  return conn;

error:
  mbedtls_conn_server_destroy(conn);

  return NULL;
}

mbedtls_server_t* mbedtls_server_create(const char* port, const uint8_t* srv_crt,
                                        uint32_t srv_crt_len, const uint8_t* srv_key,
                                        uint32_t srv_key_len, const uint8_t* cas_pem,
                                        uint32_t cas_pem_len) {
  int ret = 0;
  mbedtls_server_t* server = NULL;
  const char* pers = TK_MBEDTLS_PERS;
  return_value_if_fail(srv_crt != NULL && srv_key != NULL && cas_pem != NULL, NULL);
  server = TKMEM_ZALLOC(mbedtls_server_t);
  return_value_if_fail(server != NULL, NULL);

  mbedtls_net_init(&(server->listen_fd));
  mbedtls_ssl_config_init(&(server->conf));
#if defined(MBEDTLS_SSL_CACHE_C)
  mbedtls_ssl_cache_init(&(server->cache));
#endif
  mbedtls_x509_crt_init(&(server->srvcert));
  mbedtls_pk_init(&(server->pkey));
  mbedtls_entropy_init(&(server->entropy));
  mbedtls_ctr_drbg_init(&(server->ctr_drbg));

#if defined(MBEDTLS_DEBUG_C)
  mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

  /*
   * 1. Load the certificates and private RSA key
   */
  log_debug("\n  . Loading the server cert. and key...");

  /*
   * This demonstration program uses embedded test certificates.
   * Instead, you may want to use mbedtls_x509_crt_parse_file() to read the
   * server and CA certificates, as well as mbedtls_pk_parse_keyfile().
   */
  ret = mbedtls_x509_crt_parse(&(server->srvcert), (const unsigned char*)srv_crt, srv_crt_len);
  if (ret != 0) {
    log_debug(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
    goto error;
  }

  ret = mbedtls_x509_crt_parse(&(server->srvcert), (const unsigned char*)cas_pem, cas_pem_len);
  if (ret != 0) {
    log_debug(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
    goto error;
  }

  ret = mbedtls_pk_parse_key(&(server->pkey), (const unsigned char*)srv_key, srv_key_len, NULL, 0);
  if (ret != 0) {
    log_debug(" failed\n  !  mbedtls_pk_parse_key returned %d\n\n", ret);
    goto error;
  }

  /*
   * 2. Setup the listening TCP socket
   */
  log_debug("  . Bind on https://localhost:%s/ ...", port);

  if ((ret = mbedtls_net_bind(&(server->listen_fd), NULL, port, MBEDTLS_NET_PROTO_TCP)) != 0) {
    log_debug(" failed\n  ! mbedtls_net_bind returned %d\n\n", ret);
    goto error;
  }

  /*
   * 3. Seed the RNG
   */
  log_debug("  . Seeding the random number generator...");

  if ((ret = mbedtls_ctr_drbg_seed(&(server->ctr_drbg), mbedtls_entropy_func, &(server->entropy),
                                   (const unsigned char*)pers, strlen(pers))) != 0) {
    log_debug(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret);
    goto error;
  }

  /*
   * 4. Setup stuff
   */
  log_debug("  . Setting up the SSL data....");

  if ((ret = mbedtls_ssl_config_defaults(&(server->conf), MBEDTLS_SSL_IS_SERVER,
                                         MBEDTLS_SSL_TRANSPORT_STREAM,
                                         MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
    log_debug(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret);
    goto error;
  }

  mbedtls_ssl_conf_rng(&(server->conf), mbedtls_ctr_drbg_random, &(server->ctr_drbg));
  mbedtls_ssl_conf_dbg(&(server->conf), mbedtls_awtk_debug, NULL);

#if defined(MBEDTLS_SSL_CACHE_C)
  mbedtls_ssl_conf_session_cache(&(server->conf), &(server->cache), mbedtls_ssl_cache_get,
                                 mbedtls_ssl_cache_set);
#endif

  mbedtls_ssl_conf_ca_chain(&(server->conf), server->srvcert.next, NULL);
  if ((ret = mbedtls_ssl_conf_own_cert(&(server->conf), &(server->srvcert), &(server->pkey))) !=
      0) {
    log_debug(" failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\n", ret);
    goto error;
  }

  return server;

error:
  mbedtls_server_destroy(server);

  return NULL;
}

ret_t mbedtls_server_destroy(mbedtls_server_t* server) {
  return_value_if_fail(server != NULL, RET_BAD_PARAMS);

  mbedtls_net_free(&(server->listen_fd));
  mbedtls_x509_crt_free(&(server->srvcert));
  mbedtls_pk_free(&(server->pkey));
  mbedtls_ssl_config_free(&(server->conf));

#if defined(MBEDTLS_SSL_CACHE_C)
  mbedtls_ssl_cache_free(&(server->cache));
#endif /*MBEDTLS_SSL_CACHE_C*/

  mbedtls_ctr_drbg_free(&(server->ctr_drbg));
  mbedtls_entropy_free(&(server->entropy));

  return RET_OK;
}
#endif /*WITH_MBEDTLS*/
