#ifndef TK_MBEDTLS_SERVER_H
#define TK_MBEDTLS_SERVER_H

#include "mbedtls_helper.h"

#if defined(MBEDTLS_SSL_CACHE_C)
#include "mbedtls/ssl_cache.h"
#endif

typedef struct _mbedtls_server_t {
  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;
  mbedtls_ssl_config conf;
  mbedtls_x509_crt srvcert;
  mbedtls_pk_context pkey;
  mbedtls_net_context listen_fd;
#if defined(MBEDTLS_SSL_CACHE_C)
  mbedtls_ssl_cache_context cache;
#endif
} mbedtls_server_t;

typedef struct _mbedtls_conn_server_t {
  mbedtls_conn_t mbedtls_conn;
  mbedtls_net_context client_fd;
  mbedtls_server_t* server;
} mbedtls_conn_server_t;

mbedtls_server_t* mbedtls_server_create(const char* port, const uint8_t* srv_crt,
                                        uint32_t srv_crt_len, const uint8_t* srv_key,
                                        uint32_t srv_key_len, const uint8_t* cas_pem,
                                        uint32_t cas_pem_len);

mbedtls_conn_t* mbedtls_server_accept(mbedtls_server_t* server);

ret_t mbedtls_server_destroy(mbedtls_server_t* server);

#endif /* TK_MBEDTLS_SERVER_H */
