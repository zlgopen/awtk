#include "tkc/utils.h"
#include "tkc/dlist.h"
#include "tkc/mem_allocator_fixed_block.h"
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

TEST(DList, basic) {
  dlist_t dlist;
  dlist_t* s = &dlist;

  dlist_init(s, NULL, NULL);

  ASSERT_EQ(dlist_size(s), 0);
  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);

  ASSERT_EQ(dlist_size(s), 1);

  ASSERT_EQ(dlist_append(s, TO_POINTER(2)), RET_OK);

  ASSERT_EQ(dlist_size(s), 2);

  ASSERT_EQ(dlist_append(s, TO_POINTER(3)), RET_OK);

  ASSERT_EQ(dlist_size(s), 3);

  ASSERT_EQ(dlist_prepend(s, TO_POINTER(4)), RET_OK);

  ASSERT_EQ(dlist_size(s), 4);

  ASSERT_EQ(dlist_prepend(s, TO_POINTER(5)), RET_OK);

  ASSERT_EQ(dlist_size(s), 5);

  dlist_deinit(s);
}

TEST(DList, node_allocator) {
  dlist_t dlist;
  dlist_t* s = &dlist;

  dlist_init(s, NULL, NULL);

  ASSERT_EQ(dlist_set_node_allocator(s, mem_allocator_fixed_block_create(sizeof(dlist_node_t), 2)),
            RET_OK);

  ASSERT_EQ(dlist_size(s), 0);
  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);

  ASSERT_EQ(dlist_size(s), 1);

  ASSERT_EQ(dlist_append(s, TO_POINTER(2)), RET_OK);

  ASSERT_EQ(dlist_size(s), 2);

  ASSERT_EQ(dlist_append(s, TO_POINTER(3)), RET_OK);

  ASSERT_EQ(dlist_size(s), 3);

  ASSERT_EQ(dlist_prepend(s, TO_POINTER(4)), RET_OK);

  ASSERT_EQ(dlist_size(s), 4);

  ASSERT_EQ(dlist_prepend(s, TO_POINTER(5)), RET_OK);

  ASSERT_EQ(dlist_size(s), 5);

  dlist_deinit(s);
}

TEST(DList, find) {
  dlist_t dlist;
  dlist_t* s = &dlist;

  dlist_init(s, NULL, NULL);

  ASSERT_EQ(dlist_size(s), 0);

  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);

  ASSERT_EQ(TO_INT(dlist_find(s, TO_POINTER(1))), 1);

  ASSERT_EQ(dlist_append(s, TO_POINTER(2)), RET_OK);

  ASSERT_EQ(TO_INT(dlist_find(s, TO_POINTER(2))), 2);

  dlist_deinit(s);
}

TEST(DList, find_ex) {
  dlist_t dlist;
  dlist_t* s = &dlist;

  dlist_init(s, NULL, NULL);

  ASSERT_EQ(dlist_size(s), 0);

  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);

  ASSERT_EQ(TO_INT(dlist_find_ex(s, pointer_compare, TO_POINTER(1), FALSE)), 1);

  ASSERT_EQ(dlist_append(s, TO_POINTER(2)), RET_OK);

  ASSERT_EQ(TO_INT(dlist_find_ex(s, pointer_compare, TO_POINTER(2), FALSE)), 2);

  ASSERT_EQ(TO_INT(dlist_find_ex(s, compare_always_equal, TO_POINTER(2), FALSE)), 1);

  dlist_deinit(s);
}

TEST(DList, tail_pop) {
  dlist_t dlist;
  dlist_t* s = &dlist;

  dlist_init(s, NULL, NULL);

  ASSERT_EQ(dlist_size(s), 0);

  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(4)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(5)), RET_OK);

  ASSERT_EQ(TO_INT(dlist_tail(s)), 5);
  ASSERT_EQ(TO_INT(dlist_tail(s)), 5);
  ASSERT_EQ(TO_INT(dlist_tail(s)), 5);
  ASSERT_EQ(dlist_size(s), 5);

  ASSERT_EQ(TO_INT(dlist_head(s)), 1);
  ASSERT_EQ(TO_INT(dlist_head(s)), 1);
  ASSERT_EQ(TO_INT(dlist_head(s)), 1);
  ASSERT_EQ(dlist_size(s), 5);

  ASSERT_EQ(TO_INT(dlist_tail_pop(s)), 5);
  ASSERT_EQ(TO_INT(dlist_tail(s)), 4);

  ASSERT_EQ(TO_INT(dlist_head_pop(s)), 1);
  ASSERT_EQ(TO_INT(dlist_head(s)), 2);

  ASSERT_EQ(dlist_size(s), 3);

  ASSERT_EQ(TO_INT(dlist_tail_pop(s)), 4);
  ASSERT_EQ(TO_INT(dlist_head_pop(s)), 2);

  ASSERT_EQ(dlist_size(s), 1);

  ASSERT_EQ(dlist_append(s, TO_POINTER(10)), RET_OK);
  ASSERT_EQ(dlist_prepend(s, TO_POINTER(0)), RET_OK);

  ASSERT_EQ(dlist_size(s), 3);

  ASSERT_EQ(TO_INT(dlist_tail_pop(s)), 10);
  ASSERT_EQ(TO_INT(dlist_head_pop(s)), 0);

  ASSERT_EQ(TO_INT(dlist_tail_pop(s)), 3);
  ASSERT_EQ(dlist_head_pop(s) == NULL, true);
  ASSERT_EQ(dlist_tail_pop(s) == NULL, true);

  dlist_deinit(s);
}

TEST(DList, remove) {
  dlist_t dlist;
  dlist_t* s = &dlist;

  dlist_init(s, NULL, NULL);

  ASSERT_EQ(dlist_is_empty(s), TRUE);
  ASSERT_EQ(dlist_size(s), 0);

  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_is_empty(s), FALSE);

  ASSERT_EQ(TO_INT(dlist_find(s, TO_POINTER(1))), 1);

  ASSERT_EQ(dlist_remove(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_count(s, TO_POINTER(1)), 0);
  ASSERT_EQ(dlist_is_empty(s), TRUE);

  ASSERT_EQ(dlist_append(s, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(TO_INT(dlist_find(s, TO_POINTER(2))), 2);
  ASSERT_EQ(dlist_is_empty(s), FALSE);

  ASSERT_EQ(dlist_remove(s, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_count(s, TO_POINTER(2)), 0);
  ASSERT_EQ(dlist_is_empty(s), TRUE);

  dlist_deinit(s);
}

TEST(DList, remove_ex) {
  dlist_t dlist;
  dlist_t* s = &dlist;

  dlist_init(s, NULL, NULL);

  ASSERT_EQ(dlist_is_empty(s), TRUE);
  ASSERT_EQ(dlist_size(s), 0);

  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(dlist_is_empty(s), FALSE);

  ASSERT_EQ(dlist_remove_ex(s, pointer_compare, TO_POINTER(1), 1, FALSE), RET_OK);
  ASSERT_EQ(dlist_size(s), 4);

  ASSERT_EQ(dlist_remove_ex(s, pointer_compare, TO_POINTER(1), 2, FALSE), RET_OK);
  ASSERT_EQ(dlist_size(s), 2);

  ASSERT_EQ(dlist_remove_ex(s, compare_always_equal, TO_POINTER(1), 2, FALSE), RET_OK);
  ASSERT_EQ(dlist_size(s), 0);

  dlist_deinit(s);
}

TEST(DList, foreach) {
  string log;
  dlist_t dlist;
  dlist_t* s = &dlist;
  dlist_init(s, NULL, NULL);

  ASSERT_EQ(dlist_size(s), 0);

  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(2)), RET_OK);
  log = "";
  dlist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "1:2:");

  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(2)), RET_OK);
  log = "";
  dlist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "1:2:1:2:");

  ASSERT_EQ(dlist_append(s, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(4)), RET_OK);
  log = "";
  dlist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "1:2:1:2:3:4:");

  dlist_deinit(s);
}

static ret_t remove_data(void* ctx, const void* data) {
  char text[32];
  string& str = *(string*)ctx;
  int32_t n = (const char*)data - (const char*)NULL;
  tk_snprintf(text, sizeof(text), "%d:", n);

  str += text;
  if (n % 2 == 0) {
    return RET_OK;
  } else {
    return RET_REMOVE;
  }
}

TEST(DList, foreach_ex) {
  string log;
  dlist_t dlist;
  dlist_t* s = &dlist;
  dlist_init(s, NULL, NULL);

  ASSERT_EQ(dlist_size(s), 0);
  ASSERT_EQ(dlist_append(s, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(dlist_append(s, TO_POINTER(4)), RET_OK);

  log = "";
  dlist_foreach(s, remove_data, &log);
  ASSERT_EQ(log, "1:2:3:4:");

  log = "";
  dlist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "2:4:");
  dlist_deinit(s);
}

TEST(DList, insert) {
  string log;
  dlist_t dlist;
  dlist_t* s = &dlist;

  dlist_init(s, NULL, NULL);

  ASSERT_EQ(dlist_size(s), 0);
  ASSERT_EQ(dlist_insert(s, 0, TO_POINTER(0)), RET_OK);
  ASSERT_EQ(dlist_insert(s, 1, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_insert(s, 1, TO_POINTER(1)), RET_OK);
  log = "";
  dlist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "0:1:2:");

  ASSERT_EQ(dlist_insert(s, 0, TO_POINTER(5)), RET_OK);
  log = "";
  dlist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "5:0:1:2:");

  ASSERT_EQ(dlist_insert(s, 3, TO_POINTER(6)), RET_OK);
  log = "";
  dlist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "5:0:1:6:2:");

  ASSERT_EQ(dlist_insert(s, 10, TO_POINTER(10)), RET_OK);
  log = "";
  dlist_foreach(s, visit_dump, &log);
  ASSERT_EQ(log, "5:0:1:6:2:10:");

  dlist_deinit(s);
}

TEST(DList, reverse) {
  string log;
  dlist_t dlist;

  dlist_init(&dlist, NULL, NULL);

  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(4)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(5)), RET_OK);

  log = "";
  dlist_foreach(&dlist, visit_dump, &log);
  ASSERT_EQ(log, "1:2:3:4:5:");

  ASSERT_EQ(dlist_reverse(&dlist), RET_OK);

  log = "";
  dlist_foreach(&dlist, visit_dump, &log);
  ASSERT_EQ(log, "5:4:3:2:1:");

  dlist_deinit(&dlist);
}

TEST(DList, foreach_reverse) {
  string log;
  dlist_t dlist;

  dlist_init(&dlist, NULL, NULL);

  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(4)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(5)), RET_OK);

  log = "";
  dlist_foreach(&dlist, visit_dump, &log);
  ASSERT_EQ(log, "1:2:3:4:5:");

  log = "";
  dlist_foreach_reverse(&dlist, visit_dump, &log);
  ASSERT_EQ(log, "5:4:3:2:1:");

  dlist_deinit(&dlist);
}

TEST(DList, remove_last) {
  string log;
  dlist_t dlist;

  dlist_init(&dlist, NULL, NULL);

  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, TO_POINTER(5)), RET_OK);

  ASSERT_EQ(dlist_remove_last(&dlist, TO_POINTER(2)), RET_OK);

  log = "";
  dlist_foreach(&dlist, visit_dump, &log);
  ASSERT_EQ(log, "1:2:3:5:");

  dlist_deinit(&dlist);
}

TEST(DList, find_last) {
  dlist_t dlist;

  dlist_init(&dlist, NULL, (tk_compare_t)tk_str_icmp);

  ASSERT_EQ(dlist_append(&dlist, (void*)"name"), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, (void*)"id"), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, (void*)"NAME"), RET_OK);
  ASSERT_EQ(dlist_append(&dlist, (void*)"ID"), RET_OK);

  ASSERT_STREQ((char*)(dlist_find(&dlist, (void*)"name")), "name");
  ASSERT_STREQ((char*)(dlist_find_last(&dlist, (void*)"name")), "NAME");

  dlist_deinit(&dlist);
}
