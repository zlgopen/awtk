#include "gtest/gtest.h"
#include "tkc/object_evt_proxy.h"
#include "tkc/object_default.h"

static int s_callback_called = 0;
static event_t s_last_event;

static ret_t test_subscribe_callback(tk_object_t* subscriber, event_t* e, void* ctx) {
  s_callback_called++;
  s_last_event = *e;
  return RET_OK;
}

static int s_callback2_called = 0;

static ret_t test_subscribe_callback2(tk_object_t* subscriber, event_t* e, void* ctx) {
  s_callback2_called++;
  return RET_OK;
}

static bool_t test_evt_filter_accept(tk_object_t* publisher, event_t* e, void* ctx) {
  return TRUE;
}

static bool_t test_evt_filter_reject(tk_object_t* publisher, event_t* e, void* ctx) {
  return FALSE;
}

TEST(ObjectEvtProxy, basic) {
  tk_object_t* proxy = NULL;
  tk_object_t* publisher = NULL;
  ret_t ret;

  proxy = object_evt_proxy_create();
  ASSERT_TRUE(proxy != NULL);

  publisher = object_default_create();
  ASSERT_TRUE(publisher != NULL);

  ret = object_evt_proxy_register(proxy, "test_topic", publisher, EVT_PROP_CHANGED, NULL);
  ASSERT_EQ(ret, RET_OK);

  ret = object_evt_proxy_subscribe(proxy, "test_topic", test_subscribe_callback, NULL);
  ASSERT_EQ(ret, RET_OK);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 123);
  tk_object_set_prop(publisher, "test_prop", &v);
  ASSERT_EQ(s_callback_called, 1);
  ASSERT_EQ((int)s_last_event.type, (int)EVT_PROP_CHANGED);
  ASSERT_EQ(s_last_event.target, publisher);

  ret = object_evt_proxy_unsubscribe(proxy, "test_topic", test_subscribe_callback, NULL);
  ASSERT_EQ(ret, RET_OK);

  s_callback_called = 0;
  value_set_int(&v, 456);
  tk_object_set_prop(publisher, "test_prop", &v);
  ASSERT_EQ(s_callback_called, 0);

  ret = object_evt_proxy_unregister(proxy, "test_topic", publisher);
  ASSERT_EQ(ret, RET_OK);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, register_duplicate_topic) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();
  ret_t ret;

  ret = object_evt_proxy_register(proxy, "duplicate_topic", publisher, EVT_PROP_CHANGED, NULL);
  ASSERT_EQ(ret, RET_OK);

  ret = object_evt_proxy_register(proxy, "duplicate_topic", publisher, EVT_PROP_CHANGED, NULL);
  ASSERT_EQ(ret, RET_FAIL);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, evt_filter_accept) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();
  object_evt_proxy_register_opt_t reg_opt;
  memset(&reg_opt, 0, sizeof(reg_opt));
  reg_opt.evt_filter = test_evt_filter_accept;

  ASSERT_EQ(object_evt_proxy_register(proxy, "filtered", publisher, EVT_PROP_CHANGED, &reg_opt),
            RET_OK);
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "filtered", test_subscribe_callback, NULL), RET_OK);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "prop", &v);
  ASSERT_EQ(s_callback_called, 1);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, evt_filter_reject) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();
  object_evt_proxy_register_opt_t reg_opt;
  memset(&reg_opt, 0, sizeof(reg_opt));
  reg_opt.evt_filter = test_evt_filter_reject;

  ASSERT_EQ(object_evt_proxy_register(proxy, "filtered", publisher, EVT_PROP_CHANGED, &reg_opt),
            RET_OK);
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "filtered", test_subscribe_callback, NULL), RET_OK);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "prop", &v);
  ASSERT_EQ(s_callback_called, 0);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, null_topic_subscribe_matches_all) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* pub1 = object_default_create();
  tk_object_t* pub2 = object_default_create();

  ASSERT_EQ(object_evt_proxy_register(proxy, "topic_a", pub1, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_register(proxy, "topic_b", pub2, EVT_PROP_CHANGED, NULL), RET_OK);

  ASSERT_EQ(object_evt_proxy_subscribe(proxy, NULL, test_subscribe_callback, NULL), RET_OK);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(pub1, "p", &v);
  ASSERT_GE(s_callback_called, 1);

  int count_after_pub1 = s_callback_called;
  tk_object_set_prop(pub2, "p", &v);
  ASSERT_GT(s_callback_called, count_after_pub1);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(pub1);
  TK_OBJECT_UNREF(pub2);
}

TEST(ObjectEvtProxy, multiple_topics_multiple_publishers) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* pub1 = object_default_create();
  tk_object_t* pub2 = object_default_create();

  ASSERT_EQ(object_evt_proxy_register(proxy, "topic1", pub1, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_register(proxy, "topic2", pub2, EVT_PROP_CHANGED, NULL), RET_OK);

  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topic1", test_subscribe_callback, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topic2", test_subscribe_callback2, NULL), RET_OK);

  s_callback_called = 0;
  s_callback2_called = 0;
  value_t v;
  value_set_int(&v, 10);
  tk_object_set_prop(pub1, "p", &v);
  ASSERT_EQ(s_callback_called, 1);
  ASSERT_EQ(s_callback2_called, 0);

  s_callback_called = 0;
  s_callback2_called = 0;
  tk_object_set_prop(pub2, "p", &v);
  ASSERT_EQ(s_callback_called, 0);
  ASSERT_EQ(s_callback2_called, 1);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(pub1);
  TK_OBJECT_UNREF(pub2);
}

TEST(ObjectEvtProxy, unregister_with_shared_publisher) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();

  ASSERT_EQ(object_evt_proxy_register(proxy, "topicA", publisher, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_register(proxy, "topicB", publisher, EVT_PROP_CHANGED, NULL), RET_OK);

  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topicA", test_subscribe_callback, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topicB", test_subscribe_callback2, NULL), RET_OK);

  ASSERT_EQ(object_evt_proxy_unregister(proxy, "topicA", publisher), RET_OK);

  s_callback_called = 0;
  s_callback2_called = 0;
  value_t v;
  value_set_int(&v, 99);
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback_called, 0);
  ASSERT_EQ(s_callback2_called, 1);

  ASSERT_EQ(object_evt_proxy_unregister(proxy, "topicB", publisher), RET_OK);

  s_callback2_called = 0;
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback2_called, 0);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, subscribe_with_subscriber_obj) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();
  tk_object_t* subscriber = object_default_create();
  object_evt_proxy_subscribe_opt_t sub_opt;
  memset(&sub_opt, 0, sizeof(sub_opt));
  sub_opt.subscriber = subscriber;

  ASSERT_EQ(object_evt_proxy_register(proxy, "topic", publisher, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topic", test_subscribe_callback, &sub_opt), RET_OK);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback_called, 1);

  ASSERT_EQ(object_evt_proxy_unsubscribe(proxy, "topic", test_subscribe_callback, &sub_opt),
            RET_OK);

  s_callback_called = 0;
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback_called, 0);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
  TK_OBJECT_UNREF(subscriber);
}

TEST(ObjectEvtProxy, cast_wrong_type) {
  tk_object_t* obj = object_default_create();
  ASSERT_TRUE(OBJECT_EVT_PROXY(obj) == NULL);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectEvtProxy, unregister_not_found) {
  tk_object_t* proxy = object_evt_proxy_create();
  ASSERT_NE(object_evt_proxy_unregister(proxy, "nonexistent", NULL), RET_OK);
  TK_OBJECT_UNREF(proxy);
}

TEST(ObjectEvtProxy, destroy_proxy_before_publisher) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();

  ASSERT_EQ(object_evt_proxy_register(proxy, "topic", publisher, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topic", test_subscribe_callback, NULL), RET_OK);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback_called, 1);

  TK_OBJECT_UNREF(proxy);

  s_callback_called = 0;
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback_called, 0);

  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, bad_params) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();

  ASSERT_NE(object_evt_proxy_register(NULL, "t", publisher, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_NE(object_evt_proxy_register(proxy, NULL, publisher, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_NE(object_evt_proxy_register(proxy, "", publisher, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_NE(object_evt_proxy_register(proxy, "t", NULL, EVT_PROP_CHANGED, NULL), RET_OK);

  ASSERT_NE(object_evt_proxy_subscribe(NULL, "t", test_subscribe_callback, NULL), RET_OK);
  ASSERT_NE(object_evt_proxy_subscribe(proxy, "t", NULL, NULL), RET_OK);

  ASSERT_NE(object_evt_proxy_unregister(NULL, "t", publisher), RET_OK);
  ASSERT_NE(object_evt_proxy_unregister(proxy, NULL, publisher), RET_OK);
  ASSERT_NE(object_evt_proxy_unregister(proxy, "t", NULL), RET_OK);
  ASSERT_NE(object_evt_proxy_unregister(proxy, "", publisher), RET_OK);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, same_topic_different_publishers) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* pub1 = object_default_create();
  tk_object_t* pub2 = object_default_create();

  ASSERT_EQ(object_evt_proxy_register(proxy, "topic", pub1, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_register(proxy, "topic", pub2, EVT_PROP_CHANGED, NULL), RET_OK);

  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topic", test_subscribe_callback, NULL), RET_OK);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(pub1, "p", &v);
  ASSERT_EQ(s_callback_called, 1);

  s_callback_called = 0;
  tk_object_set_prop(pub2, "p", &v);
  ASSERT_EQ(s_callback_called, 1);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(pub1);
  TK_OBJECT_UNREF(pub2);
}

TEST(ObjectEvtProxy, same_topic_different_evt_types) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();

  ASSERT_EQ(object_evt_proxy_register(proxy, "topic", publisher, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_register(proxy, "topic", publisher, EVT_PROP_WILL_CHANGE, NULL),
            RET_OK);

  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topic", test_subscribe_callback, NULL), RET_OK);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback_called, 2);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, same_topic_different_filters) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();
  object_evt_proxy_register_opt_t opt_accept;
  object_evt_proxy_register_opt_t opt_reject;
  memset(&opt_accept, 0, sizeof(opt_accept));
  memset(&opt_reject, 0, sizeof(opt_reject));
  opt_accept.evt_filter = test_evt_filter_accept;
  opt_reject.evt_filter = test_evt_filter_reject;

  ASSERT_EQ(object_evt_proxy_register(proxy, "topic", publisher, EVT_PROP_CHANGED, &opt_accept),
            RET_OK);
  ASSERT_EQ(object_evt_proxy_register(proxy, "topic", publisher, EVT_PROP_CHANGED, &opt_reject),
            RET_OK);

  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topic", test_subscribe_callback, NULL), RET_OK);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback_called, 1);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, unregister_preserves_shared_publisher_across_topics) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();

  ASSERT_EQ(object_evt_proxy_register(proxy, "topicA", publisher, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_register(proxy, "topicB", publisher, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_register(proxy, "topicA", publisher, EVT_PROP_WILL_CHANGE, NULL),
            RET_OK);

  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topicA", test_subscribe_callback, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topicB", test_subscribe_callback2, NULL), RET_OK);

  ASSERT_EQ(object_evt_proxy_unregister(proxy, "topicA", publisher), RET_OK);

  s_callback_called = 0;
  s_callback2_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback_called, 0);
  ASSERT_EQ(s_callback2_called, 1);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, empty_topic_subscribe_matches_all) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* pub1 = object_default_create();
  tk_object_t* pub2 = object_default_create();

  ASSERT_EQ(object_evt_proxy_register(proxy, "topic_a", pub1, EVT_PROP_CHANGED, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_register(proxy, "topic_b", pub2, EVT_PROP_CHANGED, NULL), RET_OK);

  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "", test_subscribe_callback, NULL), RET_OK);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(pub1, "p", &v);
  ASSERT_EQ(s_callback_called, 1);

  s_callback_called = 0;
  tk_object_set_prop(pub2, "p", &v);
  ASSERT_EQ(s_callback_called, 1);

  ASSERT_EQ(object_evt_proxy_unsubscribe(proxy, "", test_subscribe_callback, NULL), RET_OK);

  s_callback_called = 0;
  tk_object_set_prop(pub1, "p", &v);
  ASSERT_EQ(s_callback_called, 0);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(pub1);
  TK_OBJECT_UNREF(pub2);
}

TEST(ObjectEvtProxy, wildcard_and_topic_subscribe_both_fire) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();

  ASSERT_EQ(object_evt_proxy_register(proxy, "topic", publisher, EVT_PROP_CHANGED, NULL), RET_OK);

  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topic", test_subscribe_callback, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, NULL, test_subscribe_callback2, NULL), RET_OK);

  s_callback_called = 0;
  s_callback2_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback_called, 1);
  ASSERT_EQ(s_callback2_called, 1);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, duplicate_subscribe_rejected) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();

  ASSERT_EQ(object_evt_proxy_register(proxy, "topic", publisher, EVT_PROP_CHANGED, NULL), RET_OK);

  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topic", test_subscribe_callback, NULL), RET_OK);
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "topic", test_subscribe_callback, NULL), RET_FAIL);

  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "p", &v);
  ASSERT_EQ(s_callback_called, 1);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, unsubscribe_not_found) {
  tk_object_t* proxy = object_evt_proxy_create();

  ASSERT_NE(object_evt_proxy_unsubscribe(proxy, "nonexistent", test_subscribe_callback, NULL),
            RET_OK);
  ASSERT_NE(object_evt_proxy_unsubscribe(proxy, NULL, test_subscribe_callback, NULL), RET_OK);

  TK_OBJECT_UNREF(proxy);
}

typedef struct {
  int* order;
  int* count;
} priority_test_ctx_t;

// 低优先级回调
static ret_t test_priority_callback_low(tk_object_t* s, event_t* e, void* ctx) {
  priority_test_ctx_t* test_ctx = (priority_test_ctx_t*)ctx;
  int index = (*test_ctx->count)++;
  test_ctx->order[index] = 1;
  return RET_OK;
}

// 高优先级回调
static ret_t test_priority_callback_high(tk_object_t* s, event_t* e, void* ctx) {
  priority_test_ctx_t* test_ctx = (priority_test_ctx_t*)ctx;
  int index = (*test_ctx->count)++;
  test_ctx->order[index] = 2;
  return RET_OK;
}

// 中优先级回调
static ret_t test_priority_callback_medium(tk_object_t* s, event_t* e, void* ctx) {
  priority_test_ctx_t* test_ctx = (priority_test_ctx_t*)ctx;
  int index = (*test_ctx->count)++;
  test_ctx->order[index] = 3;
  return RET_OK;
}

TEST(ObjectEvtProxy, subscription_priority_order) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();
  int order[3] = {0};
  int count = 0;
  priority_test_ctx_t test_ctx = {order, &count};

  ASSERT_EQ(object_evt_proxy_register(proxy, "priority_topic", publisher, EVT_PROP_CHANGED, NULL),
            RET_OK);

  object_evt_proxy_subscribe_opt_t opt_low;
  memset(&opt_low, 0, sizeof(opt_low));
  opt_low.priority = 10;
  opt_low.callback_ctx = &test_ctx;
  ASSERT_EQ(
      object_evt_proxy_subscribe(proxy, "priority_topic", test_priority_callback_low, &opt_low),
      RET_OK);

  object_evt_proxy_subscribe_opt_t opt_high;
  memset(&opt_high, 0, sizeof(opt_high));
  opt_high.priority = 100;
  opt_high.callback_ctx = &test_ctx;
  ASSERT_EQ(
      object_evt_proxy_subscribe(proxy, "priority_topic", test_priority_callback_high, &opt_high),
      RET_OK);

  object_evt_proxy_subscribe_opt_t opt_medium;
  memset(&opt_medium, 0, sizeof(opt_medium));
  opt_medium.priority = 50;
  opt_medium.callback_ctx = &test_ctx;
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "priority_topic", test_priority_callback_medium,
                                       &opt_medium),
            RET_OK);

  // 触发事件
  value_t v;
  value_set_int(&v, 0);
  tk_object_set_prop(publisher, "test", &v);

  // 验证执行顺序：高优先级(2) -> 中优先级(3) -> 低优先级(1)
  ASSERT_EQ(order[0], 2);
  ASSERT_EQ(order[1], 3);
  ASSERT_EQ(order[2], 1);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

static ret_t test_stop_callback(tk_object_t* subscriber, event_t* e, void* ctx) {
  *(int*)ctx = 1;
  return RET_STOP;
}

static ret_t test_remove_callback(tk_object_t* subscriber, event_t* e, void* ctx) {
  *(int*)ctx = 1;
  return RET_REMOVE;
}

static ret_t test_ok_callback(tk_object_t* subscriber, event_t* e, void* ctx) {
  *(int*)ctx = 1;
  return RET_OK;
}

TEST(ObjectEvtProxy, subscription_ret_stop_behavior) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();
  int stop_called = 0;
  int after_stop_called = 0;

  ASSERT_EQ(object_evt_proxy_register(proxy, "stop_test_topic", publisher, EVT_PROP_CHANGED, NULL),
            RET_OK);

  // 先注册返回RET_STOP的回调
  object_evt_proxy_subscribe_opt_t stop_opt;
  memset(&stop_opt, 0, sizeof(stop_opt));
  stop_opt.callback_ctx = &stop_called;
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "stop_test_topic", test_stop_callback, &stop_opt),
            RET_OK);
  // 再注册普通回调
  object_evt_proxy_subscribe_opt_t after_opt;
  memset(&after_opt, 0, sizeof(after_opt));
  after_opt.callback_ctx = &after_stop_called;
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "stop_test_topic", test_ok_callback, &after_opt),
            RET_OK);

  // 触发事件
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "test_prop", &v);

  // 验证只有第一个回调被调用，第二个应该被阻止
  ASSERT_EQ(stop_called, 1);
  ASSERT_EQ(after_stop_called, 0);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, subscription_ret_remove_behavior) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();
  int remove_called = 0;
  int after_remove_called = 0;

  ASSERT_EQ(
      object_evt_proxy_register(proxy, "remove_test_topic", publisher, EVT_PROP_CHANGED, NULL),
      RET_OK);

  // 注册返回RET_REMOVE的回调
  object_evt_proxy_subscribe_opt_t remove_opt;
  memset(&remove_opt, 0, sizeof(remove_opt));
  remove_opt.callback_ctx = &remove_called;
  ASSERT_EQ(
      object_evt_proxy_subscribe(proxy, "remove_test_topic", test_remove_callback, &remove_opt),
      RET_OK);

  // 注册普通回调用于验证后续事件
  object_evt_proxy_subscribe_opt_t after_opt;
  memset(&after_opt, 0, sizeof(after_opt));
  after_opt.callback_ctx = &after_remove_called;
  ASSERT_EQ(object_evt_proxy_subscribe(proxy, "remove_test_topic", test_ok_callback, &after_opt),
            RET_OK);

  // 第一次触发事件
  value_t v;
  value_set_int(&v, 1);
  tk_object_set_prop(publisher, "test_prop", &v);
  ASSERT_EQ(remove_called, 1);
  ASSERT_EQ(after_remove_called, 1);

  // 重置计数器
  remove_called = 0;
  after_remove_called = 0;

  // 第二次触发事件，验证返回RET_REMOVE的回调不再被调用
  value_set_int(&v, 2);
  tk_object_set_prop(publisher, "test_prop", &v);
  ASSERT_EQ(remove_called, 0);
  ASSERT_EQ(after_remove_called, 1);

  ASSERT_NE(
      object_evt_proxy_unsubscribe(proxy, "remove_test_topic", test_remove_callback, &remove_opt),
      RET_OK);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}
