
#include "tkc/emitter.h"
#include "gtest/gtest.h"
#include <stdlib.h>

#include <string>
using std::string;

static ret_t on_event(void* ctx, event_t* e) {
  uint32_t* p = (uint32_t*)ctx;
  *p = *p + 1;
  (void)e;

  return RET_OK;
}

static ret_t on_remove(void* ctx, event_t* e) {
  uint32_t* p = (uint32_t*)ctx;
  *p = *p + 1;
  (void)e;

  return RET_REMOVE;
}

static ret_t on_remove_id(void* ctx, event_t* e) {
  uint32_t id = *((uint32_t*)ctx);
  emitter_t* emitter = (emitter_t*)e->target;

  emitter_off(emitter, id);

  return RET_REMOVE;
}

static ret_t on_stop(void* ctx, event_t* e) {
  uint32_t* p = (uint32_t*)ctx;
  *p = *p + 1;
  (void)e;

  return RET_STOP;
}

TEST(Emitter, basic) {
  event_t e;
  uint32_t n = 0;
  emitter_t emitter;
  emitter_init(&emitter);
  uint32_t type = 12;
  e = event_init(type, &emitter);
  ASSERT_EQ(emitter_on(NULL, type, on_event, NULL), 0);
  ASSERT_EQ(emitter_on(&emitter, type, NULL, NULL), 0);

  ASSERT_EQ(emitter_on(&emitter, type, on_event, &n) > 0, true);
  ASSERT_EQ(emitter_dispatch(&emitter, &e), RET_OK);
  ASSERT_EQ(n, 1);

  ASSERT_EQ(emitter_off_by_func(&emitter, type, on_event, &n), RET_OK);
  ASSERT_EQ(emitter_dispatch(&emitter, &e), RET_OK);
  ASSERT_EQ(n, 1);

  n = 0;
  ASSERT_EQ(emitter_on(&emitter, type, on_event, &n) > 0, true);
  ASSERT_EQ(emitter_dispatch(&emitter, &e), RET_OK);
  ASSERT_EQ(n, 1);

  n = 0;
  ASSERT_EQ(emitter_on(&emitter, type, on_event, &n) > 0, true);
  ASSERT_EQ(emitter_dispatch(&emitter, &e), RET_OK);
  ASSERT_EQ(n, 2);

  n = 0;
  ASSERT_EQ(emitter_on(&emitter, type, on_event, &n) > 0, true);
  ASSERT_EQ(emitter_dispatch(&emitter, &e), RET_OK);
  ASSERT_EQ(n, 3);

  emitter_deinit(&emitter);
}

TEST(Emitter, off) {
  event_t e;
  uint32_t n = 0;
  uint32_t id = 0;
  uint32_t type = 12;
  emitter_t* emitter = emitter_create();

  e = event_init(type, emitter);

  id = emitter_on(emitter, type, on_event, &n);
  ASSERT_EQ(id > TK_INVALID_ID, true);
  ASSERT_EQ(emitter_size(emitter), 1);
  ASSERT_EQ(emitter_find(emitter, id) != NULL, true);

  ASSERT_EQ(emitter_off(emitter, id), RET_OK);
  ASSERT_EQ(emitter_size(emitter), 0);
  ASSERT_EQ(emitter_find(emitter, id) == NULL, true);

  id = emitter_on(emitter, type, on_event, &n);
  ASSERT_EQ(id > TK_INVALID_ID, true);
  ASSERT_EQ(emitter_size(emitter), 1);
  ASSERT_EQ(emitter_find(emitter, id) != NULL, true);

  id = emitter_on(emitter, type, on_event, &n);
  ASSERT_EQ(id > TK_INVALID_ID, true);
  ASSERT_EQ(emitter_size(emitter), 2);
  ASSERT_EQ(emitter_find(emitter, id) != NULL, true);

  ASSERT_EQ(emitter_off(emitter, id), RET_OK);
  ASSERT_EQ(emitter_size(emitter), 1);

  emitter_destroy(emitter);
}

TEST(Emitter, remove) {
  event_t e;
  uint32_t n = 0;
  uint32_t id = 0;
  uint32_t type = 12;
  emitter_t* emitter = emitter_create();

  e = event_init(type, emitter);

  id = emitter_on(emitter, type, on_remove, &n);
  id = emitter_on(emitter, type, on_remove, &n);
  id = emitter_on(emitter, type, on_remove, &n);
  id = emitter_on(emitter, type, on_remove, &n);

  n = 0;
  ASSERT_EQ(emitter_dispatch(emitter, &e), RET_OK);
  ASSERT_EQ(emitter_size(emitter), 0);
  ASSERT_EQ(n, 4);

  emitter_destroy(emitter);
}

TEST(Emitter, stop) {
  event_t e;
  uint32_t n = 0;
  uint32_t id = 0;
  uint32_t type = 12;
  emitter_t* emitter = emitter_create();

  e = event_init(type, emitter);

  id = emitter_on(emitter, type, on_stop, &n);
  id = emitter_on(emitter, type, on_stop, &n);
  id = emitter_on(emitter, type, on_stop, &n);
  id = emitter_on(emitter, type, on_stop, &n);

  n = 0;
  ASSERT_EQ(emitter_size(emitter), 4);
  ASSERT_EQ(emitter_dispatch(emitter, &e), RET_STOP);
  ASSERT_EQ(emitter_size(emitter), 4);
  ASSERT_EQ(n, 1);

  emitter_destroy(emitter);
}

TEST(Emitter, remove_in_func) {
  event_t e;
  uint32_t n = 0;
  uint32_t id1 = 0;
  uint32_t id2 = 0;
  uint32_t type = 12;
  emitter_t* emitter = emitter_create();

  e = event_init(type, emitter);

  id1 = emitter_on(emitter, type, on_event, &n);
  id2 = emitter_on(emitter, type, on_remove_id, &id1);
  ASSERT_EQ(emitter_dispatch(emitter, &e), RET_OK);
  ASSERT_EQ(emitter_size(emitter), 0);

  id1 = emitter_on(emitter, type, on_event, &n);
  id2 = emitter_on(emitter, type, on_remove_id, &id2);
  ASSERT_EQ(emitter_dispatch(emitter, &e), RET_OK);
  ASSERT_EQ(emitter_size(emitter), 1);

  emitter_destroy(emitter);
}

TEST(Emitter, remove_item) {
  uint32_t i = 0;
  uint32_t n = 5000;
  emitter_t* emitter = emitter_create();

  for (i = 0; i < n; i++) {
    emitter_on(emitter, 123, on_event, NULL);
    ASSERT_EQ(emitter_size(emitter), i + 1);
  }

  ASSERT_EQ(emitter_get_item(emitter, 0), emitter->items);
  ASSERT_EQ(emitter_get_item(emitter, 1), emitter->items->next);
  ASSERT_EQ(emitter_get_item(emitter, 2), emitter->items->next->next);

  for (i = 0; i < n; i++) {
    uint32_t r = random() % emitter_size(emitter);
    ASSERT_EQ(emitter_remove_item(emitter, emitter_get_item(emitter, r)), RET_OK);
  }
  ASSERT_EQ(emitter_size(emitter), 0);

  emitter_destroy(emitter);
}

TEST(Emitter, off_by_tag) {
  uint32_t type = 12;
  emitter_t* emitter = emitter_create();

  emitter_on_with_tag(emitter, type, on_remove, NULL, 1);
  emitter_on_with_tag(emitter, type, on_remove, NULL, 1);

  emitter_on_with_tag(emitter, type, on_remove, NULL, 2);
  emitter_on_with_tag(emitter, type, on_remove, NULL, 3);

  emitter_on_with_tag(emitter, type, on_remove, NULL, 1);
  emitter_on_with_tag(emitter, type, on_remove, NULL, 1);

  ASSERT_EQ(emitter_size(emitter), 6);

  ASSERT_EQ(emitter_off_by_tag(emitter, 1), RET_OK);
  ASSERT_EQ(emitter_size(emitter), 2);

  ASSERT_EQ(emitter_off_by_tag(emitter, 3), RET_OK);
  ASSERT_EQ(emitter_size(emitter), 1);

  ASSERT_EQ(emitter_off_by_tag(emitter, 2), RET_OK);
  ASSERT_EQ(emitter_size(emitter), 0);

  emitter_destroy(emitter);
}
