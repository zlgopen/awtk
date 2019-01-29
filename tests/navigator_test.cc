#include "gtest/gtest.h"
#include "mvvm/base/navigator.h"

#include <string>
using std::string;

static ret_t dummy_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;
  value_set_int(&v, 123);

  navigator_request_on_result(req, &v);

  return RET_OK;
}

static ret_t default_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;
  value_set_int(&v, 456);

  navigator_request_on_result(req, &v);

  return RET_OK;
}

TEST(Navigator, regist) {
  navigator_t* nav = navigator_create();
  navigator_handler_t* handler = navigator_handler_create(dummy_on_request);

  ASSERT_EQ(navigator_has_handler(nav, "dummy"), FALSE);
  ASSERT_EQ(navigator_register_handler(nav, "dummy", handler), RET_OK);
  ASSERT_EQ(navigator_has_handler(nav, "dummy"), TRUE);
  ASSERT_EQ(navigator_unregister_handler(nav, "dummy"), RET_OK);
  ASSERT_EQ(navigator_has_handler(nav, "dummy"), FALSE);

  object_unref(OBJECT(nav));
}

TEST(Navigator, no_handler) {
  navigator_t* nav = navigator_create();
  navigator_request_t* req = navigator_request_create("dummy", NULL);

  ASSERT_EQ(navigator_handle_request(nav, req), RET_NOT_FOUND);

  object_unref(OBJECT(nav));
}

TEST(Navigator, handler_request) {
  navigator_t* nav = navigator_create();
  navigator_request_t* req = navigator_request_create("dummy", NULL);

  navigator_register_handler(nav, "dummy", navigator_handler_create(dummy_on_request));

  ASSERT_EQ(navigator_handle_request(nav, req), RET_OK);
  ASSERT_EQ(value_int(&(req->result)), 123);

  object_unref(OBJECT(nav));
}

TEST(Navigator, default_handler) {
  navigator_t* nav = navigator_create();
  navigator_request_t* req = navigator_request_create("dummy", NULL);

  navigator_register_handler(nav, NAVIGATOR_DEFAULT_HANDLER,
                             navigator_handler_create(default_on_request));

  ASSERT_EQ(navigator_handle_request(nav, req), RET_OK);
  ASSERT_EQ(value_int(&(req->result)), 456);

  object_unref(OBJECT(nav));
}

static string s_log;
static ret_t toast_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  s_log = string("toast:") + string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_CONTENT));

  return RET_OK;
}

TEST(Navigator, toast) {
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  navigator_register_handler(nav, NAVIGATOR_REQ_TOAST, navigator_handler_create(toast_on_request));
  ASSERT_EQ(navigator_toast("hello", 1000), RET_OK);
  ASSERT_EQ(s_log, string("toast:hello"));

  object_unref(OBJECT(nav));
  navigator_set(old);
}

static ret_t info_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  s_log = string("info:") + string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_CONTENT));

  return RET_OK;
}

TEST(Navigator, info) {
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  navigator_register_handler(nav, NAVIGATOR_REQ_INFO, navigator_handler_create(info_on_request));
  ASSERT_EQ(navigator_info("hello", " awtk"), RET_OK);
  ASSERT_EQ(s_log, string("info:hello awtk"));

  object_unref(OBJECT(nav));
  navigator_set(old);
}

static ret_t confirm_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;

  s_log = string("confirm:") + string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_CONTENT));

  value_set_int(&v, RET_FAIL);

  navigator_request_on_result(req, &v);

  return RET_OK;
}

TEST(Navigator, confirm) {
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  navigator_register_handler(nav, NAVIGATOR_REQ_CONFIRM,
                             navigator_handler_create(confirm_on_request));
  ASSERT_EQ(navigator_confirm("hello", " awtk"), RET_FAIL);
  ASSERT_EQ(s_log, string("confirm:hello awtk"));

  object_unref(OBJECT(nav));
  navigator_set(old);
}

static ret_t warn_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  s_log = string("warn:") + string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_CONTENT));

  return RET_OK;
}

TEST(Navigator, warn) {
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  navigator_register_handler(nav, NAVIGATOR_REQ_WARN, navigator_handler_create(warn_on_request));
  ASSERT_EQ(navigator_warn("hello", " awtk"), RET_OK);
  ASSERT_EQ(s_log, string("warn:hello awtk"));

  object_unref(OBJECT(nav));
  navigator_set(old);
}

static ret_t pick_dir_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;

  s_log = string("pick_dir:") + string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_DEFAULT));
  value_set_str(&v, "dir");

  navigator_request_on_result(req, &v);

  return RET_OK;
}

TEST(Navigator, pick_dir) {
  str_t str;
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  str_init(&str, 0);
  str_set(&str, " awtk");
  navigator_register_handler(nav, NAVIGATOR_REQ_PICK_DIR,
                             navigator_handler_create(pick_dir_on_request));
  ASSERT_EQ(navigator_pick_dir("hello", &str), RET_OK);
  ASSERT_EQ(s_log, string("pick_dir:hello awtk"));
  ASSERT_EQ(string(str.str), string("dir"));

  object_unref(OBJECT(nav));
  navigator_set(old);
}

static ret_t pick_color_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;
  s_log = string("pick_color:") + string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_DEFAULT));
  value_set_str(&v, "color");
  navigator_request_on_result(req, &v);

  return RET_OK;
}

TEST(Navigator, pick_color) {
  str_t str;
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  str_init(&str, 0);
  str_set(&str, " awtk");
  navigator_register_handler(nav, NAVIGATOR_REQ_PICK_COLOR,
                             navigator_handler_create(pick_color_on_request));
  ASSERT_EQ(navigator_pick_color("hello", &str), RET_OK);
  ASSERT_EQ(s_log, string("pick_color:hello awtk"));
  ASSERT_EQ(string(str.str), string("color"));

  object_unref(OBJECT(nav));
  navigator_set(old);
}

static ret_t pick_file_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;

  s_log = string("pick_file:") + string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_MINE_TYPES)) +
          string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_DEFAULT));
  value_set_str(&v, "file");

  navigator_request_on_result(req, &v);

  return RET_OK;
}

TEST(Navigator, pick_file) {
  str_t str;
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  str_init(&str, 0);
  str_set(&str, " awtk");
  navigator_register_handler(nav, NAVIGATOR_REQ_PICK_FILE,
                             navigator_handler_create(pick_file_on_request));
  ASSERT_EQ(navigator_pick_file("hello", "image/png", TRUE, &str), RET_OK);
  ASSERT_EQ(s_log, string("pick_file:helloimage/png awtk"));
  ASSERT_EQ(string(str.str), string("file"));

  object_unref(OBJECT(nav));
  navigator_set(old);
}
