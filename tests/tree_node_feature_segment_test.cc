#include "gtest/gtest.h"
#include "tkc/utils.h"
#include "tkc/tree_node_feature_segment.h"

static ret_t build_tree_for_test(tree_t* tree) {
  return_value_if_fail(tree != NULL, RET_BAD_PARAMS);

  /**
   * 0
   * ├── 1
   * │   ├── 11
   * │   └── 12
   * │       ├── 121
   * │       ├── 122
   * │       └── 123
   * ├── 2
   * │   ├── 21
   * │   ├── 22
   * │   │   └── 221
   * │   │       ├── 2211
   * │   │       └── 2212
   * │   └── 23
   * └── 3
   *     ├── 31
   *     └── 32
   */

  // 创建根结点
  tree_node_t* root = tree_create_node(tree, tk_pointer_from_int(0));
  tree_set_root(tree, root);

  // 第一层子结点（使用结点操作接口）
  tree_node_t* node1 = tree_create_node(tree, tk_pointer_from_int(1));
  tree_prepend_child_node(tree, NULL, node1);

  tree_node_t* node3 = tree_create_node(tree, tk_pointer_from_int(3));
  tree_append_child_node(tree, NULL, node3);

  tree_node_t* node2 = tree_create_node(tree, tk_pointer_from_int(2));
  tree_insert_child_node(tree, NULL, 1, node2);

  // 第二层子结点
  tree_node_t* node11 = tree_create_node(tree, tk_pointer_from_int(11));
  tree_prepend_child_node(tree, node1, node11);

  tree_node_t* node12 = tree_create_node(tree, tk_pointer_from_int(12));
  tree_append_child_node(tree, node1, node12);

  tree_node_t* node21 = tree_create_node(tree, tk_pointer_from_int(21));
  tree_append_child_node(tree, node2, node21);

  tree_node_t* node22 = tree_create_node(tree, tk_pointer_from_int(22));
  tree_append_sibling_node(tree, node21, node22);

  tree_node_t* node23 = tree_create_node(tree, tk_pointer_from_int(23));
  tree_append_sibling_node(tree, node22, node23);

  tree_node_t* node31 = tree_create_node(tree, tk_pointer_from_int(31));
  tree_append_child_node(tree, node3, node31);

  tree_node_t* node32 = tree_create_node(tree, tk_pointer_from_int(32));
  tree_append_child_node(tree, node3, node32);

  // 第三层子结点
  tree_append_child_node(tree, node12, tree_create_node(tree, tk_pointer_from_int(121)));
  tree_append_child_node(tree, node12, tree_create_node(tree, tk_pointer_from_int(122)));
  tree_append_child_node(tree, node12, tree_create_node(tree, tk_pointer_from_int(123)));

  tree_node_t* node221 = tree_create_node(tree, tk_pointer_from_int(221));
  tree_append_child_node(tree, node22, node221);

  // 第四层子结点
  tree_append_child_node(tree, node221, tree_create_node(tree, tk_pointer_from_int(2211)));
  tree_append_child_node(tree, node221, tree_create_node(tree, tk_pointer_from_int(2212)));

  return RET_OK;
}

static ret_t tree_to_string_range_on_node(const tree_node_t* node, str_t* result, void* ctx) {
  tree_t* tree = (tree_t*)(ctx);
  tree_node_feature_segment_t* segment_feature =
      (tree_node_feature_segment_t*)tree_get_node_feature(tree, node,
                                                          tree_node_feature_segment_info());
  return str_append_format(result, (TK_NAME_LEN + 1) * 2, "[%d,%d]", segment_feature->start,
                           segment_feature->end);
}

TEST(TreeNodeFeatureSegment, basic) {
  tree_t tree;
  feature_info_list_t* features = feature_info_list_create();
  ASSERT_EQ(features != NULL, TRUE);
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);
  ASSERT_EQ(feature_info_list_append(features, tree_node_feature_segment_info()), RET_OK);
  ASSERT_EQ(tree_set_node_features(&tree, features), RET_OK);

  ASSERT_EQ(tree_get_node_size(&tree), sizeof(tree_node_t) + sizeof(tree_node_feature_segment_t));

  tree_deinit(&tree);
}

TEST(TreeNodeFeatureSegment, update_range_by_order) {
  tree_t tree;
  feature_info_list_t* features = feature_info_list_create();
  ASSERT_EQ(features != NULL, TRUE);
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);
  ASSERT_EQ(feature_info_list_append(features, tree_node_feature_segment_info()), RET_OK);
  ASSERT_EQ(tree_set_node_features(&tree, features), RET_OK);

  ASSERT_EQ(build_tree_for_test(&tree), RET_OK);
  ASSERT_EQ(segment_tree_update_range_by_order(&tree), RET_OK);

  str_t str;
  str_init(&str, 1024);
  tree_to_string_handler_t handler = {};
  handler.on_node = tree_to_string_range_on_node;
  handler.ctx = &tree;
  tree_to_string(&tree, NULL, &str, &handler);
  ASSERT_STREQ(str.str,
               "[0,10]\n"
               "├── [0,3]\n"
               "│   ├── [0,0]\n"
               "│   └── [1,3]\n"
               "│       ├── [1,1]\n"
               "│       ├── [2,2]\n"
               "│       └── [3,3]\n"
               "├── [4,8]\n"
               "│   ├── [4,4]\n"
               "│   ├── [5,7]\n"
               "│   │   └── [5,6]\n"
               "│   │       ├── [5,5]\n"
               "│   │       └── [6,6]\n"
               "│   └── [8,8]\n"
               "└── [9,10]\n"
               "    ├── [9,9]\n"
               "    └── [10,10]");
  str_reset(&str);

  tree_deinit(&tree);
}

TEST(TreeNodeFeatureSegment, node_is_ancestor) {
  tree_t tree;
  feature_info_list_t* features = feature_info_list_create();
  ASSERT_EQ(features != NULL, TRUE);
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);
  ASSERT_EQ(feature_info_list_append(features, tree_node_feature_segment_info()), RET_OK);
  ASSERT_EQ(tree_set_node_features(&tree, features), RET_OK);

  ASSERT_EQ(build_tree_for_test(&tree), RET_OK);
  ASSERT_EQ(segment_tree_update_range_by_order(&tree), RET_OK);

  tree_node_t* node1 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(1));
  tree_node_t* node2 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(2));
  tree_node_t* node122 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(122));
  tree_node_t* node22 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(22));
  tree_node_t* node221 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(221));

  ASSERT_EQ(segment_tree_node_is_ancestor(&tree, node122, node1), TRUE);
  ASSERT_EQ(segment_tree_node_is_ancestor(&tree, node122, node2), FALSE);
  ASSERT_EQ(segment_tree_node_is_ancestor(&tree, node1, node122), FALSE);
  ASSERT_EQ(segment_tree_node_is_ancestor(&tree, node2, node122), FALSE);
  ASSERT_EQ(segment_tree_node_is_ancestor(&tree, node221, node22), TRUE);
  ASSERT_EQ(segment_tree_node_is_ancestor(&tree, node22, node221), FALSE);

  tree_deinit(&tree);
}
