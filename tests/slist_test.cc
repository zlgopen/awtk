#include "tkc/utils.h"
#include "tkc/slist.h"
#include "gtest/gtest.h"

#include <string>

#define TO_POINTER(n) (((char*)NULL) + n)
#define TO_INT(p) (((char*)(p)) - ((char*)NULL))

using std::string;

static ret_t visit_dump(void* ctx, const void* data) {
  char text[32];
  string& str = *(string*)ctx;
  int32_t n = (const char*)data - (const char*)NULL;
  tk_snprintf(text, sizeof(text), "%d:", n);

  str += text;

  return RET_OK;
}

TEST(SList, basic) {
  slist_t slist;
  slist_t* s = &slist;

  slist_init(s, NULL, NULL);

  ASSERT_EQ(slist_size(s), 0);
  ASSERT_EQ(slist_append(s, TO_POINTER(1)), RET_OK);

  ASSERT_EQ(slist_size(s), 1);

  ASSERT_EQ(slist_append(s, TO_POINTER(2)), RET_OK);

  ASSERT_EQ(slist_size(s), 2);

  ASSERT_EQ(slist_append(s, TO_POINTER(3)), RET_OK);

  ASSERT_EQ(slist_size(s), 3);

  ASSERT_EQ(slist_prepend(s, TO_POINTER(4)), RET_OK);

  ASSERT_EQ(slist_size(s), 4);

  ASSERT_EQ(slist_prepend(s, TO_POINTER(5)), RET_OK);

  ASSERT_EQ(slist_size(s), 5);

  slist_deinit(s);
}

TEST(SList, find) {
  slist_t slist;
  slist_t* s = &slist;

  slist_init(s, NULL, NULL);

  ASSERT_EQ(slist_size(s), 0);

  ASSERT_EQ(slist_append(s, TO_POINTER(1)), RET_OK);

  ASSERT_EQ(TO_INT(slist_find(s, TO_POINTER(1))), 1);

  ASSERT_EQ(slist_append(s, TO_POINTER(2)), RET_OK);

  ASSERT_EQ(TO_INT(slist_find(s, TO_POINTER(2))), 2);

  slist_deinit(s);
}

TEST(SList, tail_pop) {
  slist_t slist;
  slist_t* s = &slist;

  slist_init(s, NULL, NULL);

  ASSERT_EQ(slist_size(s), 0);

  ASSERT_EQ(slist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(slist_append(s, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(slist_append(s, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(slist_append(s, TO_POINTER(4)), RET_OK);
  ASSERT_EQ(slist_append(s, TO_POINTER(5)), RET_OK);


  ASSERT_EQ(TO_INT(slist_tail_pop(s)), 5);
  ASSERT_EQ(TO_INT(slist_head_pop(s)), 1);

  ASSERT_EQ(slist_size(s), 3);
  
  ASSERT_EQ(TO_INT(slist_tail_pop(s)), 4);
  ASSERT_EQ(TO_INT(slist_head_pop(s)), 2);

  ASSERT_EQ(slist_size(s), 1);

  ASSERT_EQ(slist_append(s, TO_POINTER(10)), RET_OK);
  ASSERT_EQ(slist_prepend(s, TO_POINTER(0)), RET_OK);

  ASSERT_EQ(slist_size(s), 3);

  ASSERT_EQ(TO_INT(slist_tail_pop(s)), 10);
  ASSERT_EQ(TO_INT(slist_head_pop(s)), 0);

  ASSERT_EQ(TO_INT(slist_tail_pop(s)), 3);
  ASSERT_EQ(slist_head_pop(s) == NULL, TRUE);
  ASSERT_EQ(slist_tail_pop(s) == NULL, TRUE);

  slist_deinit(s);
}

TEST(SList, remove) {
  slist_t slist;
  slist_t* s = &slist;

  slist_init(s, NULL, NULL);

  ASSERT_EQ(slist_size(s), 0);

  ASSERT_EQ(slist_append(s, TO_POINTER(1)), RET_OK);

  ASSERT_EQ(TO_INT(slist_find(s, TO_POINTER(1))), 1);

  ASSERT_EQ(slist_remove(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(slist_count(s, TO_POINTER(1)), 0);

  ASSERT_EQ(slist_append(s, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(TO_INT(slist_find(s, TO_POINTER(2))), 2);

  ASSERT_EQ(slist_remove(s, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(slist_count(s, TO_POINTER(2)), 0);

  slist_deinit(s);
}

TEST(SList, foreach) {
  string log;
  slist_t slist;
  slist_t* s = &slist;
  slist_init(s, NULL, NULL);

  ASSERT_EQ(slist_size(s), 0);

  ASSERT_EQ(slist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(slist_append(s, TO_POINTER(2)), RET_OK);
  log = "";
  slist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "1:2:");

  ASSERT_EQ(slist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(slist_append(s, TO_POINTER(2)), RET_OK);
  log = "";
  slist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "1:2:1:2:");

  ASSERT_EQ(slist_append(s, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(slist_append(s, TO_POINTER(4)), RET_OK);
  log = "";
  slist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "1:2:1:2:3:4:");

  slist_deinit(s);
}
