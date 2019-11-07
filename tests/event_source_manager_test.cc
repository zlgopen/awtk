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

TEST(EventSourceManager, tag) {
  idle_manager_t* tm = idle_manager_create();
  event_source_manager_t* manager = event_source_manager_default_create();
  event_source_t* source1 = event_source_idle_create(tm);
  event_source_t* source2 = event_source_idle_create(tm);

  event_source_set_tag(source1, tm);
  event_source_set_tag(source2, tm);
  ASSERT_EQ(event_source_manager_add(manager, source1), RET_OK);
  ASSERT_EQ(event_source_manager_add(manager, source2), RET_OK);
  ASSERT_EQ(event_source_manager_exist(manager, source1), TRUE);
  ASSERT_EQ(event_source_manager_exist(manager, source2), TRUE);
  ASSERT_EQ(event_source_manager_remove_by_tag(manager, tm), RET_OK);
  ASSERT_EQ(event_source_manager_exist(manager, source1), FALSE);
  ASSERT_EQ(event_source_manager_exist(manager, source2), FALSE);

  object_unref(OBJECT(source1));
  object_unref(OBJECT(source2));
  event_source_manager_destroy(manager);
  idle_manager_destroy(tm);
}
