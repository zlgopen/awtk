#include "gtest/gtest.h"
#include "base/event_queue.h"

#define NR 10
TEST(EventQueue, basic) {
  uint16_t i = 0;
  event_queue_req_t r;
  event_queue_req_t w;
  event_queue_t* q = event_queue_create(NR);

  memset(&r, 0x00, sizeof(r));
  memset(&w, 0x00, sizeof(w));

  ASSERT_EQ(q != NULL, true);
  ASSERT_EQ(q->r, 0);
  ASSERT_EQ(q->w, 0);
  ASSERT_EQ(q->full, FALSE);
  ASSERT_EQ(q->capacity, 10);

  w.pointer_event.e.type = EVT_POINTER_DOWN;
  w.pointer_event.x = 100;
  w.pointer_event.y = 200;

  ASSERT_EQ(event_queue_recv(q, &r), RET_FAIL);
  ASSERT_EQ(event_queue_send(q, &w), RET_OK);
  ASSERT_EQ(event_queue_recv(q, &r), RET_OK);
  ASSERT_EQ(memcmp(&r, &w, sizeof(r)), 0);
  ASSERT_EQ(event_queue_recv(q, &r), RET_FAIL);

  for (i = 0; i < NR; i++) {
    w.pointer_event.e.type = i;
    ASSERT_EQ(event_queue_send(q, &w), RET_OK);
  }
  ASSERT_EQ(q->full, TRUE);
  ASSERT_EQ(event_queue_send(q, &w), RET_FAIL);
  for (i = 0; i < NR; i++) {
    ASSERT_EQ(event_queue_recv(q, &r), RET_OK);
    ASSERT_EQ(r.pointer_event.e.type, i);
  }
  ASSERT_EQ(event_queue_recv(q, &r), RET_FAIL);
  ASSERT_EQ(q->full, FALSE);

  w.pointer_event.e.type = 1;
  ASSERT_EQ(event_queue_send(q, &w), RET_OK);

  w.pointer_event.e.type = 2;
  ASSERT_EQ(event_queue_replace_last(q, &w), RET_OK);

  ASSERT_EQ(event_queue_recv(q, &r), RET_OK);
  ASSERT_EQ(memcmp(&r, &w, sizeof(r)), 0);

  event_queue_destroy(q);
}
