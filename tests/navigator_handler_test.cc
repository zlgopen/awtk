#include "gtest/gtest.h"
#include "mvvm/base/navigator_handler.h"

#include <string>
using std::string;

static ret_t dummy_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;
  value_set_int(&v, object_get_prop_int(OBJECT(req), NAVIGATOR_ARG_MIN, 0));

  navigator_request_on_result(req, &v);

  return RET_OK;
}

TEST(NavigatorHandler, basic) {
  navigator_request_t* req = navigator_request_create(NAVIGATOR_REQ_TOAST, NULL);
  navigator_handler_t* handler = navigator_handler_create(dummy_on_request);

  object_set_prop_int(OBJECT(req), NAVIGATOR_ARG_MIN, 123);

  navigator_handler_on_request(handler, req);

  ASSERT_EQ(value_int(&(req->result)), object_get_prop_int(OBJECT(req), NAVIGATOR_ARG_MIN, 0));

  object_unref(OBJECT(req));
  object_unref(OBJECT(handler));
}
