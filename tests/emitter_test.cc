
#include "base/emitter.h"
#include <stdlib.h>
#include "base/array.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

static ret_t on_event(void* ctx, event_t* e) {
  uint32_t* p = (uint32_t*)ctx;
  *p = *p + 1;
  (void)e;

  return RET_OK;
}

TEST(Emitter, basic) {
  event_t e;
  uint32_t n = 0;
  emitter_t emitter;
  emitter_init(&emitter);
  uint16_t type = 12;
  e.type = type;
  ASSERT_EQ(emitter_on(NULL, type, on_event, NULL), RET_BAD_PARAMS);
  ASSERT_EQ(emitter_on(&emitter, type, NULL, NULL), RET_BAD_PARAMS);

  ASSERT_EQ(emitter_on(&emitter, type, on_event, &n), RET_OK);
  ASSERT_EQ(emitter_dispatch(&emitter, &e), RET_OK);
  ASSERT_EQ(n, 1);

  ASSERT_EQ(emitter_off(&emitter, type, on_event, &n), RET_OK);
  ASSERT_EQ(emitter_dispatch(&emitter, &e), RET_OK);
  ASSERT_EQ(n, 1);

  n = 0;
  ASSERT_EQ(emitter_on(&emitter, type, on_event, &n), RET_OK);
  ASSERT_EQ(emitter_dispatch(&emitter, &e), RET_OK);
  ASSERT_EQ(n, 1);

  n = 0;
  ASSERT_EQ(emitter_on(&emitter, type, on_event, &n), RET_OK);
  ASSERT_EQ(emitter_dispatch(&emitter, &e), RET_OK);
  ASSERT_EQ(n, 2);

  n = 0;
  ASSERT_EQ(emitter_on(&emitter, type, on_event, &n), RET_OK);
  ASSERT_EQ(emitter_dispatch(&emitter, &e), RET_OK);
  ASSERT_EQ(n, 3);

  emitter_deinit(&emitter);
}
