#include "tkc/utils.h"
#include "tkc/object_fifo_default.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

static int int_cmp(const void* a, const void* b) {
  int ia = *(const int*)a;
  int ib = *(const int*)b;

  return ia - ib;
}

static ret_t on_fifo_event(void* ctx, event_t* e) {
  string& log = *(string*)ctx;
  char buf[32];

  tk_snprintf(buf, sizeof(buf), "%d,", e->type);
  log += buf;

  return RET_OK;
}

TEST(ObjectFifo, create) {
  tk_object_t* obj = object_fifo_default_create(4, sizeof(int));
  object_fifo_default_t* fifo = OBJECT_FIFO_DEFAULT(obj);

  ASSERT_NE(obj, (tk_object_t*)NULL);
  ASSERT_EQ(fifo->capacity, 4u);
  ASSERT_EQ(fifo->size, 0u);
  ASSERT_EQ(fifo->cursor, 0u);
  ASSERT_EQ(fifo->unit_size, (uint32_t)sizeof(int));
  ASSERT_NE(fifo->buffer, (uint8_t*)NULL);

  ASSERT_EQ(object_fifo_get_capacity(obj), 4u);
  ASSERT_EQ(object_fifo_get_size(obj), 0u);
  ASSERT_EQ(object_fifo_get_cursor(obj), 0u);
  ASSERT_EQ(object_fifo_get_unit_size(obj), (uint32_t)sizeof(int));
  ASSERT_EQ(object_fifo_get_buffer(obj), fifo->buffer);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, create_invalid) {
  ASSERT_EQ(object_fifo_default_create(0, sizeof(int)), (tk_object_t*)NULL);
  ASSERT_EQ(object_fifo_default_create(4, 0), (tk_object_t*)NULL);
}

TEST(ObjectFifo, push_pop) {
  int v = 0;
  int* p = NULL;
  tk_object_t* obj = object_fifo_default_create(4, sizeof(int));

  v = 1;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  v = 2;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  v = 3;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);

  ASSERT_EQ(object_fifo_get_size(obj), 3u);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 1);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 1), 2);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 2), 3);
  ASSERT_EQ(object_fifo_get(obj, 3), (void*)NULL);

  p = (int*)object_fifo_pop(obj);
  ASSERT_EQ(*p, 1);
  ASSERT_EQ(object_fifo_get_size(obj), 2u);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 2);

  p = (int*)object_fifo_pop(obj);
  ASSERT_EQ(*p, 2);
  p = (int*)object_fifo_pop(obj);
  ASSERT_EQ(*p, 3);
  ASSERT_EQ(object_fifo_get_size(obj), 0u);
  ASSERT_EQ(object_fifo_pop(obj), (void*)NULL);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, npush_npop) {
  int data[3] = {10, 20, 30};
  tk_object_t* obj = object_fifo_default_create(8, sizeof(int));

  ASSERT_EQ(object_fifo_npush(obj, data, 3), RET_OK);
  ASSERT_EQ(object_fifo_get_size(obj), 3u);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 10);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 1), 20);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 2), 30);

  ASSERT_EQ(object_fifo_npop(obj, 2), RET_OK);
  ASSERT_EQ(object_fifo_get_size(obj), 1u);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 30);

  ASSERT_NE(object_fifo_npop(obj, 2), RET_OK);
  ASSERT_EQ(object_fifo_npop(obj, 1), RET_OK);
  ASSERT_EQ(object_fifo_get_size(obj), 0u);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, wrap) {
  int v = 0;
  tk_object_t* obj = object_fifo_default_create(3, sizeof(int));

  v = 1;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  v = 2;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  v = 3;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  ASSERT_EQ(object_fifo_get_size(obj), 3u);

  ASSERT_EQ(*(int*)object_fifo_pop(obj), 1);
  ASSERT_EQ(*(int*)object_fifo_pop(obj), 2);
  ASSERT_EQ(object_fifo_get_size(obj), 1u);

  v = 4;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  v = 5;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  ASSERT_EQ(object_fifo_get_size(obj), 3u);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 3);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 1), 4);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 2), 5);

  v = 6;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  ASSERT_EQ(object_fifo_get_size(obj), 3u);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 4);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 1), 5);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 2), 6);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, push_head_pop_tail) {
  int v = 0;
  tk_object_t* obj = object_fifo_default_create(4, sizeof(int));

  v = 1;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  v = 2;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);

  v = 0;
  ASSERT_EQ(object_fifo_push_head(obj, &v), RET_OK);
  ASSERT_EQ(object_fifo_get_size(obj), 3u);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 0);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 1), 1);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 2), 2);

  ASSERT_EQ(*(int*)object_fifo_pop_tail(obj), 2);
  ASSERT_EQ(object_fifo_get_size(obj), 2u);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 0);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 1), 1);

  ASSERT_EQ(object_fifo_npop_tail(obj, 2), RET_OK);
  ASSERT_EQ(object_fifo_get_size(obj), 0u);
  ASSERT_EQ(object_fifo_pop_tail(obj), (void*)NULL);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, npush_head) {
  int data[2] = {8, 9};
  int v = 1;
  tk_object_t* obj = object_fifo_default_create(8, sizeof(int));

  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  v = 2;
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  ASSERT_EQ(object_fifo_npush_head(obj, data, 2), RET_OK);
  ASSERT_EQ(object_fifo_get_size(obj), 4u);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 8);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 1), 9);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 2), 1);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 3), 2);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, set) {
  int data[3] = {1, 2, 3};
  int update[2] = {20, 30};
  tk_object_t* obj = object_fifo_default_create(8, sizeof(int));

  ASSERT_EQ(object_fifo_npush(obj, data, 3), RET_OK);
  ASSERT_EQ(object_fifo_set(obj, 1, update, 2), RET_OK);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 1);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 1), 20);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 2), 30);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, set_reverse) {
  int data[3] = {1, 2, 3};
  int v = 99;
  tk_object_t* obj = object_fifo_default_create(8, sizeof(int));

  ASSERT_EQ(object_fifo_npush(obj, data, 3), RET_OK);
  ASSERT_EQ(object_fifo_set_reverse(obj, 0, &v, 1), RET_OK);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 1);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 1), 2);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 2), 99);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, find) {
  int data[4] = {10, 20, 30, 40};
  int key = 30;
  int missing = 50;
  tk_object_t* obj = object_fifo_default_create(8, sizeof(int));

  ASSERT_EQ(object_fifo_npush(obj, data, 4), RET_OK);
  ASSERT_EQ(*(int*)object_fifo_find(obj, &key), 30);
  ASSERT_EQ(object_fifo_find_index(obj, &key), 2);
  ASSERT_EQ(object_fifo_find(obj, &missing), (void*)NULL);
  ASSERT_EQ(object_fifo_find_index(obj, &missing), -1);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, sort_bsearch) {
  int data[5] = {40, 10, 30, 20, 50};
  int key = 30;
  int missing = 25;
  int32_t low = -1;
  tk_object_t* obj = object_fifo_default_create(8, sizeof(int));

  ASSERT_EQ(object_fifo_npush(obj, data, 5), RET_OK);
  ASSERT_EQ(object_fifo_sort(obj, int_cmp), RET_OK);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 0), 10);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 1), 20);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 2), 30);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 3), 40);
  ASSERT_EQ(*(int*)object_fifo_get(obj, 4), 50);

  ASSERT_EQ(object_fifo_bsearch_index(obj, int_cmp, &key), 2);
  ASSERT_EQ(*(int*)object_fifo_bsearch(obj, int_cmp, &key), 30);
  ASSERT_EQ(object_fifo_bsearch_index(obj, int_cmp, &missing), -1);
  ASSERT_EQ(object_fifo_bsearch_index_ex(obj, int_cmp, &missing, &low), -1);
  ASSERT_EQ(low, 2);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, part_clone) {
  int data[4] = {1, 2, 3, 4};
  tk_object_t* obj = object_fifo_default_create(8, sizeof(int));
  tk_object_t* clone = NULL;

  ASSERT_EQ(object_fifo_npush(obj, data, 4), RET_OK);
  clone = object_fifo_part_clone(obj, 1, 2);
  ASSERT_NE(clone, (tk_object_t*)NULL);
  ASSERT_EQ(object_fifo_get_size(clone), 2u);
  ASSERT_EQ(*(int*)object_fifo_get(clone, 0), 2);
  ASSERT_EQ(*(int*)object_fifo_get(clone, 1), 3);

  TK_OBJECT_UNREF(clone);
  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, clear) {
  int data[3] = {1, 2, 3};
  tk_object_t* obj = object_fifo_default_create(4, sizeof(int));

  ASSERT_EQ(object_fifo_npush(obj, data, 3), RET_OK);
  ASSERT_EQ(object_fifo_clear(obj), RET_OK);
  ASSERT_EQ(object_fifo_get_size(obj), 0u);
  ASSERT_EQ(object_fifo_get_cursor(obj), 0u);
  ASSERT_EQ(object_fifo_clear(obj), RET_OK);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, set_capacity) {
  int data[3] = {1, 2, 3};
  tk_object_t* obj = object_fifo_default_create(4, sizeof(int));

  ASSERT_EQ(object_fifo_npush(obj, data, 3), RET_OK);
  ASSERT_EQ(object_fifo_set_capacity(obj, 8), RET_OK);
  ASSERT_EQ(object_fifo_get_capacity(obj), 8u);
  ASSERT_EQ(object_fifo_get_size(obj), 0u);
  ASSERT_EQ(object_fifo_get_cursor(obj), 0u);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, compare) {
  int a = 1;
  int b = 2;
  int c = 1;
  tk_object_t* obj = object_fifo_default_create(4, sizeof(int));

  ASSERT_EQ(object_fifo_compare(obj, &a, &c), 0);
  ASSERT_NE(object_fifo_compare(obj, &a, &b), 0);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, events) {
  string log;
  string expect;
  char buf[32];
  int v = 1;
  tk_object_t* obj = object_fifo_default_create(4, sizeof(int));

  emitter_on(EMITTER(obj), EVT_OBJECT_FIFO_WILL_PUSH, on_fifo_event, &log);
  emitter_on(EMITTER(obj), EVT_OBJECT_FIFO_PUSH, on_fifo_event, &log);
  emitter_on(EMITTER(obj), EVT_OBJECT_FIFO_WILL_POP, on_fifo_event, &log);
  emitter_on(EMITTER(obj), EVT_OBJECT_FIFO_POP, on_fifo_event, &log);

  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);
  ASSERT_EQ(*(int*)object_fifo_pop(obj), 1);

  tk_snprintf(buf, sizeof(buf), "%d,", EVT_OBJECT_FIFO_WILL_PUSH);
  expect = buf;
  tk_snprintf(buf, sizeof(buf), "%d,", EVT_OBJECT_FIFO_PUSH);
  expect += buf;
  tk_snprintf(buf, sizeof(buf), "%d,", EVT_OBJECT_FIFO_WILL_POP);
  expect += buf;
  tk_snprintf(buf, sizeof(buf), "%d,", EVT_OBJECT_FIFO_POP);
  expect += buf;
  ASSERT_EQ(log, expect);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, block_event) {
  string log;
  string expect;
  char buf[32];
  int v = 1;
  tk_object_t* obj = object_fifo_default_create(4, sizeof(int));

  emitter_on(EMITTER(obj), EVT_OBJECT_FIFO_WILL_PUSH, on_fifo_event, &log);
  emitter_on(EMITTER(obj), EVT_OBJECT_FIFO_PUSH, on_fifo_event, &log);
  emitter_on(EMITTER(obj), EVT_OBJECT_FIFO_VALUE_CHANGE, on_fifo_event, &log);

  ASSERT_EQ(object_fifo_set_block_event(obj, TRUE), RET_OK);
  ASSERT_EQ(object_fifo_push(obj, &v), RET_OK);

  /* block_event 时走 VALUE_CHANGE，且 WILL/PUSH 各触发一次 */
  tk_snprintf(buf, sizeof(buf), "%d,", EVT_OBJECT_FIFO_VALUE_CHANGE);
  expect = buf;
  expect += buf;
  ASSERT_EQ(log, expect);

  TK_OBJECT_UNREF(obj);
}

TEST(ObjectFifo, event_cast) {
  object_fifo_set_event_t set_evt;
  object_fifo_push_event_t push_evt;
  object_fifo_push_head_event_t push_head_evt;
  object_fifo_pop_event_t pop_evt;
  object_fifo_pop_tail_event_t pop_tail_evt;
  object_fifo_value_change_event_t value_change_evt;
  event_t* e = NULL;

  e = object_fifo_set_event_init(&set_evt, EVT_OBJECT_FIFO_SET, NULL);
  ASSERT_NE(e, (event_t*)NULL);
  ASSERT_NE(object_fifo_set_event_cast(e), (object_fifo_set_event_t*)NULL);

  e = object_fifo_push_event_init(&push_evt, EVT_OBJECT_FIFO_PUSH, NULL);
  ASSERT_NE(object_fifo_push_event_cast(e), (object_fifo_push_event_t*)NULL);

  e = object_fifo_push_head_event_init(&push_head_evt, EVT_OBJECT_FIFO_PUSH_HEAD, NULL);
  ASSERT_NE(object_fifo_push_head_event_cast(e), (object_fifo_push_head_event_t*)NULL);

  e = object_fifo_pop_event_init(&pop_evt, EVT_OBJECT_FIFO_POP, NULL);
  ASSERT_NE(object_fifo_pop_event_cast(e), (object_fifo_pop_event_t*)NULL);

  e = object_fifo_pop_tail_event_init(&pop_tail_evt, EVT_OBJECT_FIFO_POP_TAIL, NULL);
  ASSERT_NE(object_fifo_pop_tail_event_cast(e), (object_fifo_pop_tail_event_t*)NULL);

  e = object_fifo_value_change_event_init(&value_change_evt, EVT_OBJECT_FIFO_VALUE_CHANGE, NULL);
  ASSERT_NE(object_fifo_value_change_event_cast(e), (object_fifo_value_change_event_t*)NULL);
}
