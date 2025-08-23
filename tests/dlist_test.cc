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

TEST(DList, remove_ex1) {
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

  ASSERT_EQ(dlist_remove_ex(s, pointer_compare, TO_POINTER(1), -1, FALSE), RET_OK);
  ASSERT_EQ(dlist_size(s), 2);
  
  ASSERT_EQ(dlist_remove_ex(s, pointer_compare, TO_POINTER(2), -1, FALSE), RET_OK);
  ASSERT_EQ(dlist_size(s), 1);
  
  ASSERT_EQ(dlist_remove_ex(s, pointer_compare, TO_POINTER(3), -1, FALSE), RET_OK);
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

TEST(DList, get) {
  dlist_t dlist;
  dlist_t* d = &dlist;
  uint32_t i = 0;
  dlist_init(d, NULL, NULL);

  // 测试空列表的情况
  ASSERT_EQ(dlist_get(d, 0), (void*)NULL);
  ASSERT_EQ(dlist_get(d, 100), (void*)NULL);

  // 添加元素并测试正常索引
  for (i = 0; i < 1000; i++) {
    ASSERT_EQ(dlist_append(d, TO_POINTER(i)), RET_OK);
    ASSERT_EQ(TO_INT(dlist_get(d, i)), i);
  }

  // 测试边界情况
  ASSERT_EQ(TO_INT(dlist_get(d, 0)), 0);      // 第一个元素
  ASSERT_EQ(TO_INT(dlist_get(d, 999)), 999);  // 最后一个元素
  
  // 测试超出范围的索引，应该返回NULL
  ASSERT_EQ(dlist_get(d, 1000), (void*)NULL);  // 超出范围
  ASSERT_EQ(dlist_get(d, 1001), (void*)NULL);  // 远超范围
  ASSERT_EQ(dlist_get(d, UINT32_MAX), (void*)NULL);  // 最大值

  // 验证列表大小没有改变
  ASSERT_EQ(dlist_size(d), 1000);
  
  dlist_deinit(d);
}

TEST(DList, edge_cases) {
  dlist_t dlist;
  dlist_t* d = &dlist;
  dlist_init(d, NULL, NULL);

  // 测试空列表的各种操作
  ASSERT_EQ(dlist_size(d), 0);
  ASSERT_EQ(dlist_is_empty(d), TRUE);
  ASSERT_EQ(dlist_head(d), (void*)NULL);
  ASSERT_EQ(dlist_tail(d), (void*)NULL);
  ASSERT_EQ(dlist_head_pop(d), (void*)NULL);
  ASSERT_EQ(dlist_tail_pop(d), (void*)NULL);
  ASSERT_EQ(dlist_find(d, TO_POINTER(1)), (void*)NULL);
  ASSERT_EQ(dlist_count(d, TO_POINTER(1)), 0);

  // 测试移除不存在的元素
  ASSERT_EQ(dlist_remove(d, TO_POINTER(1)), RET_NOT_FOUND);
  ASSERT_EQ(dlist_remove_last(d, TO_POINTER(1)), RET_NOT_FOUND);

  // 测试单个元素的情况
  ASSERT_EQ(dlist_append(d, TO_POINTER(100)), RET_OK);
  ASSERT_EQ(dlist_size(d), 1);
  ASSERT_EQ(dlist_is_empty(d), FALSE);
  ASSERT_EQ(TO_INT(dlist_head(d)), 100);
  ASSERT_EQ(TO_INT(dlist_tail(d)), 100);
  ASSERT_EQ(TO_INT(dlist_get(d, 0)), 100);
  ASSERT_EQ(dlist_get(d, 1), (void*)NULL);

  // 测试单个元素的head_pop
  ASSERT_EQ(TO_INT(dlist_head_pop(d)), 100);
  ASSERT_EQ(dlist_size(d), 0);
  ASSERT_EQ(dlist_is_empty(d), TRUE);

  // 重新添加一个元素，测试tail_pop
  ASSERT_EQ(dlist_append(d, TO_POINTER(200)), RET_OK);
  ASSERT_EQ(TO_INT(dlist_tail_pop(d)), 200);
  ASSERT_EQ(dlist_size(d), 0);

  dlist_deinit(d);
}

TEST(DList, insert_edge_cases) {
  dlist_t dlist;
  dlist_t* d = &dlist;
  dlist_init(d, NULL, NULL);

  // 测试在空列表中插入
  ASSERT_EQ(dlist_insert(d, 0, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_size(d), 1);
  ASSERT_EQ(TO_INT(dlist_get(d, 0)), 1);

  // 测试在超出范围的位置插入（应该追加到末尾）
  ASSERT_EQ(dlist_insert(d, 100, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_size(d), 2);
  ASSERT_EQ(TO_INT(dlist_get(d, 1)), 2);

  // 测试在中间位置插入
  ASSERT_EQ(dlist_insert(d, 1, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(dlist_size(d), 3);
  ASSERT_EQ(TO_INT(dlist_get(d, 0)), 1);
  ASSERT_EQ(TO_INT(dlist_get(d, 1)), 3);
  ASSERT_EQ(TO_INT(dlist_get(d, 2)), 2);

  dlist_deinit(d);
}

TEST(DList, remove_edge_cases) {
  dlist_t dlist;
  dlist_t* d = &dlist;
  dlist_init(d, NULL, NULL);

  // 添加重复元素
  ASSERT_EQ(dlist_append(d, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(d, TO_POINTER(2)), RET_OK);
  ASSERT_EQ(dlist_append(d, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_append(d, TO_POINTER(3)), RET_OK);
  ASSERT_EQ(dlist_append(d, TO_POINTER(1)), RET_OK);

  // 测试计数功能
  ASSERT_EQ(dlist_count(d, TO_POINTER(1)), 3);
  ASSERT_EQ(dlist_count(d, TO_POINTER(2)), 1);
  ASSERT_EQ(dlist_count(d, TO_POINTER(4)), 0);

  // 测试移除第一个匹配项
  ASSERT_EQ(dlist_remove(d, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_count(d, TO_POINTER(1)), 2);
  ASSERT_EQ(dlist_size(d), 4);

  // 测试移除最后一个匹配项
  ASSERT_EQ(dlist_remove_last(d, TO_POINTER(1)), RET_OK);
  ASSERT_EQ(dlist_count(d, TO_POINTER(1)), 1);
  ASSERT_EQ(dlist_size(d), 3);

  // 测试移除不存在的元素
  ASSERT_EQ(dlist_remove(d, TO_POINTER(999)), RET_NOT_FOUND);
  ASSERT_EQ(dlist_size(d), 3);

  dlist_deinit(d);
}
