#include "foo_client.h"
#include "foo_service.h"
#include "tkc/thread.h"
#include "tkc/platform.h"
#include "tkc/socket_pair.h"
#include "streams/inet/iostream_tcp.h"

#include "gtest/gtest.h"

TEST(foo, basic) {
  int socks[2];
  tk_socketpair(socks);
  tk_iostream_t* a_tcp = tk_iostream_tcp_create(socks[0]);
  tk_iostream_t* b_tcp = tk_iostream_tcp_create(socks[1]);
  ASSERT_NE(a_tcp, (tk_iostream_t*)NULL);
  ASSERT_NE(b_tcp, (tk_iostream_t*)NULL);

  tk_service_t* service = foo_service_create(a_tcp, NULL);
  foo_client_t* client = foo_client_create(b_tcp);

  ASSERT_NE(service, (tk_service_t*)NULL);
  ASSERT_NE(client, (foo_client_t*)NULL);

  tk_service_destroy(service);
  foo_client_destroy(client);
}

static void* thread_service_func(void* args) {
  tk_service_t* service = (tk_service_t*)args;

  while (tk_service_dispatch(service) != RET_QUIT) {
  }

  log_debug("foo service quit\n");

  return NULL;
}

static void test_foo(uint32_t retry_times, foo_service_args_t* args) {
  int socks[2];
  tk_socketpair(socks);
  tk_iostream_t* a_tcp = tk_iostream_tcp_create(socks[0]);
  tk_iostream_t* b_tcp = tk_iostream_tcp_create(socks[1]);
  ASSERT_NE(a_tcp, (tk_iostream_t*)NULL);
  ASSERT_NE(b_tcp, (tk_iostream_t*)NULL);

  tk_service_t* service = foo_service_create(a_tcp, args);
  foo_client_t* client = foo_client_create(b_tcp);

  foo_service_t* foo_service = (foo_service_t*)service;
  tk_service_set_retry_times(service, retry_times);
  tk_client_set_retry_times((tk_client_t*)client, retry_times);

  ASSERT_NE(service, (tk_service_t*)NULL);
  ASSERT_NE(client, (foo_client_t*)NULL);

  tk_thread_t* thread = tk_thread_create(thread_service_func, service);
  tk_thread_start(thread);

  sleep_ms(1000);
  str_t str;
  int value = 0;
  uint32_t version = 0;
  uint32_t ret_retry_times = 0;

  str_init(&str, 100);

  ASSERT_EQ(foo_client_login(client, "abc", "123"), RET_OK);
  if (args != NULL && args->auth != NULL) {
    ASSERT_EQ(foo_service->is_login, TRUE);
  }
  ASSERT_EQ(foo_client_logout(client), RET_OK);
  if (args != NULL && args->auth != NULL) {
    ASSERT_EQ(foo_service->is_login, FALSE);
  }

  ASSERT_EQ(foo_client_set_int(client, 100), RET_OK);
  ASSERT_EQ(foo_client_set_string(client, "abcd"), RET_OK);
  ASSERT_EQ(foo_service->value, 100);
  ASSERT_STREQ(foo_service->str.str, "abcd");

  ASSERT_EQ(foo_client_get_int(client, &value), RET_OK);
  ASSERT_EQ(value, 100);

  ASSERT_EQ(foo_client_get_string(client, &str), RET_OK);
  ASSERT_STREQ(str.str, "abcd");

  ASSERT_EQ(foo_client_get_version(client, &version), RET_OK);
  ASSERT_EQ(version, FOO_VERSION);

  ASSERT_EQ(foo_client_get_retry_times(client, &ret_retry_times), RET_OK);
  ASSERT_EQ(ret_retry_times, retry_times);

  foo_client_quit(client);
  tk_thread_join(thread);

  str_reset(&str);
  tk_service_destroy(service);
  foo_client_destroy(client);
  tk_thread_destroy(thread);
}

TEST(foo, retry_times0) {
  test_foo(0, NULL);
}

TEST(foo, retry_times3) {
  test_foo(3, NULL);
}

static ret_t tk_service_auth_dummy(tk_service_t* service, const char* username,
                                   const char* password) {
  return RET_OK;
}

static ret_t tk_service_logout_dummy(tk_service_t* service) {
  return RET_OK;
}

TEST(foo, login) {
  foo_service_args_t args;
  memset(&args, 0x00, sizeof(args));

  args.auth = tk_service_auth_dummy;
  args.logout = tk_service_logout_dummy;

  test_foo(0, &args);
}
