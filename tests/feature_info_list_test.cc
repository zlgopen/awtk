/**
 * File:   feature_info_list_test.cc
 * Brief:  feature_info_list 单元测试（含内存对齐）
 */

#include "gtest/gtest.h"
#include "tkc/feature_info_list.h"
#include "tkc/mem.h"

/* 测试用特征：size=3 -> align=4 */
typedef struct _feature_a_t {
  uint8_t data[3];
} feature_a_t;

static const feature_info_list_feature_info_t s_feature_a_info = {
    sizeof(feature_a_t),
    NULL,
    NULL,
};

/* 测试用特征：size=5 -> align=8 */
typedef struct _feature_b_t {
  uint8_t data[5];
} feature_b_t;

static const feature_info_list_feature_info_t s_feature_b_info = {
    sizeof(feature_b_t),
    NULL,
    NULL,
};

/* 测试用特征：size=8 -> align=8 */
typedef struct _feature_c_t {
  uint64_t value;
} feature_c_t;

static const feature_info_list_feature_info_t s_feature_c_info = {
    sizeof(feature_c_t),
    NULL,
    NULL,
};

/* 计算大于等于 size 的最小 2 的幂（与 feature_info_list 内部逻辑一致） */
static uint32_t calc_align(uint32_t size) {
  if (size == 0) {
    return 1;
  }
  size--;
  size |= size >> 1;
  size |= size >> 2;
  size |= size >> 4;
  size |= size >> 8;
  size |= size >> 16;
  size++;
  return size;
}

/* 验证 feature 指针按 align 对齐 */
static void assert_feature_aligned(void* owner, uint32_t offset, uint32_t size) {
  uint32_t align = calc_align(size);
  uintptr_t addr = (uintptr_t)owner + offset;
  ASSERT_EQ(addr % align, 0u) << "offset=" << offset << " size=" << size << " align=" << align;
}

TEST(FeatureInfoList, alignment_single_feature) {
  feature_info_list_t* list = feature_info_list_create();
  ASSERT_NE(list, nullptr);
  ASSERT_EQ(feature_info_list_append(list, &s_feature_c_info), RET_OK);

  void* owner = TKMEM_ALLOC(256);
  ASSERT_NE(owner, nullptr);
  ASSERT_EQ(feature_info_list_attach(list, owner, 0), RET_OK);

  feature_info_list_item_t* item = feature_info_list_find(list, &s_feature_c_info);
  ASSERT_NE(item, nullptr);
  assert_feature_aligned(owner, item->offset, 8);
  ASSERT_EQ(list->features_size, 8u);

  feature_info_list_detach(list);
  feature_info_list_destroy(list);
  TKMEM_FREE(owner);
}

TEST(FeatureInfoList, alignment_multi_feature_with_padding) {
  feature_info_list_t* list = feature_info_list_create();
  ASSERT_NE(list, nullptr);
  ASSERT_EQ(feature_info_list_append(list, &s_feature_a_info), RET_OK); /* size=3, align=4 */
  ASSERT_EQ(feature_info_list_append(list, &s_feature_b_info), RET_OK); /* size=5, align=8 */

  void* owner = TKMEM_ALLOC(256);
  ASSERT_NE(owner, nullptr);
  ASSERT_EQ(feature_info_list_attach(list, owner, 0), RET_OK);

  feature_info_list_item_t* item_a = feature_info_list_find(list, &s_feature_a_info);
  feature_info_list_item_t* item_b = feature_info_list_find(list, &s_feature_b_info);
  ASSERT_NE(item_a, nullptr);
  ASSERT_NE(item_b, nullptr);

  assert_feature_aligned(owner, item_a->offset, 3);
  assert_feature_aligned(owner, item_b->offset, 5);

  /* feature_a: offset=0, size=3; 需 padding 使下一项 8 字节对齐: (8-3)%8=5 */
  /* feature_b: offset=8, size=5; features_size = 3+5+5 = 13 */
  ASSERT_EQ(item_a->offset, 0u);
  ASSERT_EQ(item_b->offset, 8u);
  ASSERT_EQ(list->features_size, 13u);

  feature_info_list_detach(list);
  feature_info_list_destroy(list);
  TKMEM_FREE(owner);
}

TEST(FeatureInfoList, alignment_base_offset_requires_padding) {
  feature_info_list_t* list = feature_info_list_create();
  ASSERT_NE(list, nullptr);
  ASSERT_EQ(feature_info_list_append(list, &s_feature_c_info), RET_OK); /* size=8, align=8 */

  void* owner = TKMEM_ALLOC(256);
  ASSERT_NE(owner, nullptr);
  /* base_offset=5 时，需 padding 3 才能使 8 字节对齐 */
  ASSERT_EQ(feature_info_list_attach(list, owner, 5), RET_OK);

  feature_info_list_item_t* item = feature_info_list_find(list, &s_feature_c_info);
  ASSERT_NE(item, nullptr);
  assert_feature_aligned(owner, item->offset, 8);
  /* offset = 5 + 3 = 8, features_size = 8 + 3 = 11 */
  ASSERT_EQ(item->offset, 8u);
  ASSERT_EQ(list->features_size, 11u);

  feature_info_list_detach(list);
  feature_info_list_destroy(list);
  TKMEM_FREE(owner);
}

TEST(FeatureInfoList, alignment_three_features) {
  feature_info_list_t* list = feature_info_list_create();
  ASSERT_NE(list, nullptr);
  ASSERT_EQ(feature_info_list_append(list, &s_feature_a_info), RET_OK); /* 3 -> align 4 */
  ASSERT_EQ(feature_info_list_append(list, &s_feature_b_info), RET_OK);  /* 5 -> align 8 */
  ASSERT_EQ(feature_info_list_append(list, &s_feature_c_info), RET_OK); /* 8 -> align 8 */

  void* owner = TKMEM_ALLOC(256);
  ASSERT_NE(owner, nullptr);
  ASSERT_EQ(feature_info_list_attach(list, owner, 0), RET_OK);

  feature_info_list_item_t* item_a = feature_info_list_find(list, &s_feature_a_info);
  feature_info_list_item_t* item_b = feature_info_list_find(list, &s_feature_b_info);
  feature_info_list_item_t* item_c = feature_info_list_find(list, &s_feature_c_info);
  ASSERT_NE(item_a, nullptr);
  ASSERT_NE(item_b, nullptr);
  ASSERT_NE(item_c, nullptr);

  assert_feature_aligned(owner, item_a->offset, 3);
  assert_feature_aligned(owner, item_b->offset, 5);
  assert_feature_aligned(owner, item_c->offset, 8);

  /* a: 0, size 3; pad 5 -> next at 8
   * b: 8, size 5; pad 3 -> next at 16
   * c: 16, size 8
   * features_size = 3+5 + 5+3 + 8+3 = 24 */
  ASSERT_EQ(item_a->offset, 0u);
  ASSERT_EQ(item_b->offset, 8u);
  ASSERT_EQ(item_c->offset, 16u);
  ASSERT_EQ(list->features_size, 24u);

  /* 验证 FEATURE_INFO_LIST_ITEM_GET_FEATURE 得到的指针可安全访问 */
  feature_a_t* pa = (feature_a_t*)FEATURE_INFO_LIST_ITEM_GET_FEATURE(item_a, owner);
  feature_b_t* pb = (feature_b_t*)FEATURE_INFO_LIST_ITEM_GET_FEATURE(item_b, owner);
  feature_c_t* pc = (feature_c_t*)FEATURE_INFO_LIST_ITEM_GET_FEATURE(item_c, owner);
  ASSERT_NE(pa, nullptr);
  ASSERT_NE(pb, nullptr);
  ASSERT_NE(pc, nullptr);
  pc->value = 0x12345678;
  ASSERT_EQ(pc->value, 0x12345678u);

  feature_info_list_detach(list);
  feature_info_list_destroy(list);
  TKMEM_FREE(owner);
}
