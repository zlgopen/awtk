#include "gtest/gtest.h"
#include "tkc/action_queue.h"

#define NR 10

static ret_t qaction_dummy_exec(qaction_t* action) {
  return RET_OK;
}

TEST(ActionQueue, basic) {
  qaction_t* r = NULL;
  action_queue_t* q = action_queue_create(NR);
  qaction_t* w = qaction_create(qaction_dummy_exec, NULL, 0);

  ASSERT_EQ(q != NULL, true);
  ASSERT_EQ(q->r, 0);
  ASSERT_EQ(q->w, 0);
  ASSERT_EQ(q->full, FALSE);
  ASSERT_EQ(q->capacity, 10);

  w->args[0] = 1234;
  ASSERT_EQ(action_queue_recv(q, &r), RET_FAIL);
  ASSERT_EQ(action_queue_send(q, w), RET_OK);
  ASSERT_EQ(action_queue_recv(q, &r), RET_OK);
  ASSERT_EQ(r, w);
  ASSERT_EQ(action_queue_recv(q, &r), RET_FAIL);

  qaction_destroy(w);
  action_queue_destroy(q);
}

TEST(ActionQueue, multi) {
  uint32_t i = 0;
  qaction_t* r = NULL;
  action_queue_t* q = action_queue_create(NR);

  for (i = 0; i < NR; i++) {
    qaction_t* w = qaction_create(qaction_dummy_exec, NULL, 0);
    w->args[0] = i;
    ASSERT_EQ(action_queue_send(q, w), RET_OK);
  }

  for (i = 0; i < NR; i++) {
    ASSERT_EQ(action_queue_recv(q, &r), RET_OK);
    ASSERT_EQ(r->args[0], i);
    qaction_destroy(r);
  }

  for (i = 0; i < NR; i++) {
    qaction_t* w = qaction_create(qaction_dummy_exec, NULL, 0);
    w->args[0] = i;
    ASSERT_EQ(action_queue_send(q, w), RET_OK);
  }

  action_queue_destroy(q);
}
