#include "gtest/gtest.h"
#include "tkc/socket_pair.h"
#include "tkc/event_source_idle.h"
#include "tkc/event_source_timer.h"
#include "tkc/event_source_manager.h"
#include "tkc/event_source_manager_default.h"

TEST(EventSourceManager, basic) {
  idle_manager_t* tm = idle_manager_create();
  event_source_manager_t* manager = event_source_manager_default_create();
  event_source_t* source = event_source_idle_create(tm);

  ASSERT_EQ(event_source_manager_exist(manager, source), FALSE);
  ASSERT_EQ(event_source_manager_add(manager, source), RET_OK);
  ASSERT_EQ(event_source_manager_exist(manager, source), TRUE);
  ASSERT_EQ(event_source_manager_remove(manager, source), RET_OK);
  ASSERT_EQ(event_source_manager_exist(manager, source), FALSE);

  object_unref(OBJECT(source));
  event_source_manager_destroy(manager);
  idle_manager_destroy(tm);
}
