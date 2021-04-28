/*
 *  Common code for SSL test programs
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef MBEDTLS_PROGRAMS_SSL_SSL_TEST_LIB_H
#define MBEDTLS_PROGRAMS_SSL_SSL_TEST_LIB_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_calloc calloc
#define mbedtls_free free
#define mbedtls_time time
#define mbedtls_time_t time_t
#define mbedtls_printf printf
#define mbedtls_fprintf fprintf
#define mbedtls_snprintf snprintf
#define mbedtls_exit exit
#define MBEDTLS_EXIT_SUCCESS EXIT_SUCCESS
#define MBEDTLS_EXIT_FAILURE EXIT_FAILURE
#endif

#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/hmac_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/timing.h"
#include "mbedtls/base64.h"

#if defined(MBEDTLS_USE_PSA_CRYPTO) || defined(MBEDTLS_TEST_USE_PSA_CRYPTO_RNG)
#include "psa/crypto.h"
#include "mbedtls/psa_util.h"
#endif

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
#include "mbedtls/memory_buffer_alloc.h"
#endif

#include "tkc/types_def.h"

struct _mbedtls_conn_t;
typedef struct _mbedtls_conn_t mbedtls_conn_t;

typedef ret_t (*mbedtls_conn_destroy_t)(mbedtls_conn_t* conn);

struct _mbedtls_conn_t {
  int32_t sock;
  mbedtls_ssl_context ssl;
  mbedtls_conn_destroy_t destroy;
};

ret_t mbedtls_conn_destroy(mbedtls_conn_t* conn);

#define DEBUG_LEVEL 0

#ifndef TK_MBEDTLS_PERS
#define TK_MBEDTLS_PERS "awtk_pers"
#endif /*TK_MBEDTLS_PERS*/

void mbedtls_awtk_debug(void* ctx, int level, const char* file, int line, const char* str);

#endif /* MBEDTLS_PROGRAMS_SSL_SSL_TEST_LIB_H */
