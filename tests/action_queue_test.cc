#include "gtest/gtest.h"
#include "tkc/action_queue.h"

#define NR 10

TEST(ActionQueue, basic) {
  uint16_t i = 0;
  qaction_t r;
  qaction_t w;
  action_queue_t* q = action_queue_create(NR);

  memset(&r, 0x00, sizeof(r));
  memset(&w, 0x00, sizeof(w));

  ASSERT_EQ(q != NULL, true);
  ASSERT_EQ(q->r, 0);
  ASSERT_EQ(q->w, 0);
  ASSERT_EQ(q->full, FALSE);
  ASSERT_EQ(q->capacity, 10);

  w.extra[0] = 1234;
  ASSERT_EQ(action_queue_recv(q, &r), RET_FAIL);
  ASSERT_EQ(action_queue_send(q, &w), RET_OK);
  ASSERT_EQ(action_queue_recv(q, &r), RET_OK);
  ASSERT_EQ(memcmp(&r, &w, sizeof(r)), 0);
  ASSERT_EQ(action_queue_recv(q, &r), RET_FAIL);

  for (i = 0; i < NR; i++) {
    w.extra[0] = i;
    ASSERT_EQ(action_queue_send(q, &w), RET_OK);
  }

  ASSERT_EQ(q->full, TRUE);
  ASSERT_EQ(action_queue_send(q, &w), RET_FAIL);

  for (i = 0; i < NR; i++) {
    ASSERT_EQ(action_queue_recv(q, &r), RET_OK);
    ASSERT_EQ(r.extra[0], i);
  }
  ASSERT_EQ(action_queue_recv(q, &r), RET_FAIL);
  ASSERT_EQ(q->full, FALSE);

  ASSERT_EQ(action_queue_send(q, &w), RET_OK);
  ASSERT_EQ(action_queue_send(q, &w), RET_OK);
  ASSERT_EQ(action_queue_recv(q, &r), RET_OK);

  action_queue_destroy(q);
}
