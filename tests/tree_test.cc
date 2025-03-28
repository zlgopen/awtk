#include "gtest/gtest.h"
#include "tkc/tree.h"
#include "tkc/utils.h"

TEST(Tree, create) {
  tree_t* tree = tree_create(NULL, NULL);
  ASSERT_EQ(tree != NULL, TRUE);
  ASSERT_EQ(tree_is_empty(tree, NULL), TRUE);
  ASSERT_EQ(0, tree_size(tree, NULL));
  tree_destroy(tree);
}

TEST(Tree, insert_child) {
  tree_t tree;
  tree_node_t* node = NULL;
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);
  ASSERT_EQ(0, tree_size(&tree, NULL));

  node = tree_node_create(tk_pointer_from_int(0));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(0, tk_pointer_to_int(node->data));
  ASSERT_EQ(RET_OK, tree_set_root(&tree, node));
  ASSERT_EQ(tree.root == node, TRUE);
  ASSERT_EQ(1, tree_size(&tree, NULL));

  node = tree_node_create(tk_pointer_from_int(1));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(1, tk_pointer_to_int(node->data));
  ASSERT_EQ(RET_OK, tree_append_child_node(&tree, NULL, node));
  ASSERT_EQ(2, tree_size(&tree, NULL));

  node = tree_node_create(tk_pointer_from_int(2));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(2, tk_pointer_to_int(node->data));
  ASSERT_EQ(RET_OK, tree_prepend_child_node(&tree, NULL, node));
  ASSERT_EQ(3, tree_size(&tree, NULL));

  node = tree_node_create(tk_pointer_from_int(3));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(3, tk_pointer_to_int(node->data));
  ASSERT_EQ(RET_OK, tree_insert_child_node(&tree, NULL, 1, node));
  ASSERT_EQ(4, tree_size(&tree, NULL));

  tree_deinit(&tree);
}

TEST(Tree, insert_sibling) {
  tree_t tree;
  tree_node_t* node = NULL;
  tree_node_t* insert_pos = NULL;
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);
  ASSERT_EQ(0, tree_size(&tree, NULL));

  node = tree_node_create(tk_pointer_from_int(0));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(RET_OK, tree_set_root(&tree, node));
  ASSERT_EQ(tree.root != NULL, TRUE);

  insert_pos = tree_node_create(tk_pointer_from_int(1));
  ASSERT_EQ(insert_pos != NULL, TRUE);
  ASSERT_EQ(RET_OK, tree_append_child_node(&tree, NULL, insert_pos));

  node = tree_node_create(tk_pointer_from_int(2));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(2, tk_pointer_to_int(node->data));
  ASSERT_EQ(RET_OK, tree_append_sibling_node(&tree, insert_pos, node));
  ASSERT_EQ(3, tree_size(&tree, NULL));

  node = tree_node_create(tk_pointer_from_int(3));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(3, tk_pointer_to_int(node->data));
  ASSERT_EQ(RET_OK, tree_prepend_sibling_node(&tree, insert_pos, node));
  ASSERT_EQ(4, tree_size(&tree, NULL));

  node = tree_node_create(tk_pointer_from_int(4));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(4, tk_pointer_to_int(node->data));
  ASSERT_EQ(RET_OK, tree_insert_sibling_node(&tree, insert_pos, 1, node));
  ASSERT_EQ(5, tree_size(&tree, NULL));

  tree_deinit(&tree);
}

static ret_t build_tree_for_test(tree_t* tree) {
  return_value_if_fail(tree != NULL, RET_BAD_PARAMS);

  /**
   * 0
   * ├─ 1
   * │  ├─ 11
   * │  └─ 12
   * │     ├─ 121
   * │     ├─ 122
   * │     └─ 123
   * ├─ 2
   * │  └─ 21 → 22 → 23
   * │          └─ 221
   * │             ├─ 2211
   * │             └─ 2212
   * └─ 3
   *    ├─ 31
   *    └─ 32
   */

  // 创建根节点
  tree_node_t* root = tree_node_create(tk_pointer_from_int(0));
  tree_set_root(tree, root);

  // 第一层子节点（使用节点操作接口）
  tree_node_t* node1 = tree_node_create(tk_pointer_from_int(1));
  tree_prepend_child_node(tree, NULL, node1);

  tree_node_t* node3 = tree_node_create(tk_pointer_from_int(3));
  tree_append_child_node(tree, NULL, node3);

  tree_node_t* node2 = tree_node_create(tk_pointer_from_int(2));
  tree_insert_child_node(tree, NULL, 1, node2);

  // 第二层子节点
  tree_node_t* node11 = tree_node_create(tk_pointer_from_int(11));
  tree_prepend_child_node(tree, node1, node11);

  tree_node_t* node12 = tree_node_create(tk_pointer_from_int(12));
  tree_append_child_node(tree, node1, node12);

  tree_node_t* node21 = tree_node_create(tk_pointer_from_int(21));
  tree_append_child_node(tree, node2, node21);

  tree_node_t* node22 = tree_node_create(tk_pointer_from_int(22));
  tree_append_sibling_node(tree, node21, node22);

  tree_node_t* node23 = tree_node_create(tk_pointer_from_int(23));
  tree_append_sibling_node(tree, node22, node23);

  tree_node_t* node31 = tree_node_create(tk_pointer_from_int(31));
  tree_append_child_node(tree, node3, node31);

  tree_node_t* node32 = tree_node_create(tk_pointer_from_int(32));
  tree_append_child_node(tree, node3, node32);

  // 第三层子节点
  tree_append_child_node(tree, node12, tree_node_create(tk_pointer_from_int(121)));
  tree_append_child_node(tree, node12, tree_node_create(tk_pointer_from_int(122)));
  tree_append_child_node(tree, node12, tree_node_create(tk_pointer_from_int(123)));

  tree_node_t* node221 = tree_node_create(tk_pointer_from_int(221));
  tree_append_child_node(tree, node22, node221);

  // 第四层子节点
  tree_append_child_node(tree, node221, tree_node_create(tk_pointer_from_int(2211)));
  tree_append_child_node(tree, node221, tree_node_create(tk_pointer_from_int(2212)));

  return RET_OK;
}

static ret_t tree_node_gen_str_on_visit(void* ctx, const void* data) {
  const tree_node_t* node = (const tree_node_t*)data;
  str_t* str = (str_t*)ctx;
  str_append_int(str, tk_pointer_to_int(node->data));
  str_append_char(str, ' ');
  return RET_OK;
}

TEST(Tree, foreach) {
  tree_t tree;
  str_t str;
  str_init(&str, 1024);
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);

  ASSERT_EQ(build_tree_for_test(&tree), RET_OK);

  str_clear(&str);
  ASSERT_EQ(
      tree_foreach(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tree_node_gen_str_on_visit, &str),
      RET_OK);
  ASSERT_STREQ(str.str, "0 1 2 3 11 12 21 22 23 31 32 121 122 123 221 2211 2212 ");

  str_clear(&str);
  ASSERT_EQ(tree_foreach(&tree, NULL, TREE_FOREACH_TYPE_PREORDER, tree_node_gen_str_on_visit, &str),
            RET_OK);
  ASSERT_STREQ(str.str, "0 1 11 12 121 122 123 2 21 22 221 2211 2212 23 3 31 32 ");

  str_clear(&str);
  ASSERT_EQ(
      tree_foreach(&tree, NULL, TREE_FOREACH_TYPE_POSTORDER, tree_node_gen_str_on_visit, &str),
      RET_OK);
  ASSERT_STREQ(str.str, "11 121 122 123 12 1 21 2211 2212 221 22 23 2 31 32 3 0 ");

  tree_deinit(&tree);
  str_reset(&str);
}

TEST(Tree, find) {
  tree_t tree;
  tree_node_t* node = NULL;
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);

  ASSERT_EQ(build_tree_for_test(&tree), RET_OK);

  node = tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(123));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(123, tk_pointer_to_int(node->data));

  tree_deinit(&tree);
}

TEST(Tree, remove) {
  tree_t tree;
  tree_node_t* node = NULL;
  str_t str;
  str_init(&str, 1024);
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);

  ASSERT_EQ(build_tree_for_test(&tree), RET_OK);

  str_clear(&str);
  tree_remove(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(123));
  ASSERT_EQ(
      tree_foreach(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tree_node_gen_str_on_visit, &str),
      RET_OK);
  ASSERT_STREQ(str.str, "0 1 2 3 11 12 21 22 23 31 32 121 122 221 2211 2212 ");

  str_clear(&str);
  tree_remove(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(221));
  ASSERT_EQ(
      tree_foreach(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tree_node_gen_str_on_visit, &str),
      RET_OK);
  ASSERT_STREQ(str.str, "0 1 2 3 11 12 21 22 23 31 32 121 122 ");

  str_clear(&str);
  tree_remove(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(3));
  ASSERT_EQ(
      tree_foreach(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tree_node_gen_str_on_visit, &str),
      RET_OK);
  ASSERT_STREQ(str.str, "0 1 2 11 12 21 22 23 121 122 ");

  tree_deinit(&tree);
  str_reset(&str);
}

TEST(Tree, move) {
  tree_t tree;
  tree_node_t* node = NULL;
  str_t str;
  str_init(&str, 1024);
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);

  ASSERT_EQ(build_tree_for_test(&tree), RET_OK);

  // 移动 221 节点到 12 的子节点末尾
  tree_node_t* node221 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(221));
  tree_node_t* node12 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(12));

  ASSERT_EQ(tree_unlink_node(&tree, node221), RET_OK);
  ASSERT_EQ(tree_append_child_node(&tree, node12, node221), RET_OK);

  // 移动 22 节点到根节点的子节点末尾
  tree_node_t* node22 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(22));
  ASSERT_EQ(tree_unlink_node(&tree, node22), RET_OK);
  ASSERT_EQ(tree_append_child_node(&tree, tree.root, node22), RET_OK);

  // 移动 23 节点到 22 的兄弟节点末尾
  tree_node_t* node23 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(23));
  ASSERT_EQ(tree_unlink_node(&tree, node23), RET_OK);
  ASSERT_EQ(tree_append_sibling_node(&tree, node22, node23), RET_OK);

  str_clear(&str);
  ASSERT_EQ(
      tree_foreach(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tree_node_gen_str_on_visit, &str),
      RET_OK);
  ASSERT_STREQ(str.str, "0 1 2 3 22 23 11 12 21 31 32 121 122 123 221 2211 2212 ");

  tree_deinit(&tree);
  str_reset(&str);
}

TEST(Tree, depth) {
  tree_t tree;
  tree_node_t* node = NULL;
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);

  ASSERT_EQ(build_tree_for_test(&tree), RET_OK);

  node = tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(123));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(3, tree_depth(&tree, node));

  node = tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(221));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(3, tree_depth(&tree, node));

  node = tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(3));
  ASSERT_EQ(node != NULL, TRUE);
  ASSERT_EQ(1, tree_depth(&tree, node));

  tree_deinit(&tree);
}

TEST(Tree, height) {
  tree_t tree;
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);
  ASSERT_EQ(build_tree_for_test(&tree), RET_OK);

  tree_node_t* root = tree.root;
  tree_node_t* node2 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(2));
  tree_node_t* node22 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(22));
  tree_node_t* node2211 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(2211));

  // 根节点高度（最长路径：0→2→22→221→2211）
  ASSERT_EQ(tree_height(&tree, root), 4);

  // 节点2的高度（2→22→221→2211）
  ASSERT_EQ(tree_height(&tree, node2), 3);

  // 节点22的高度（22→221→2211）
  ASSERT_EQ(tree_height(&tree, node22), 2);

  // 叶子节点高度为0
  ASSERT_EQ(tree_height(&tree, node2211), 0);

  tree_deinit(&tree);
}

TEST(Tree, degree) {
  tree_t tree;
  ASSERT_EQ(tree_init(&tree, NULL, NULL), RET_OK);
  ASSERT_EQ(build_tree_for_test(&tree), RET_OK);

  tree_node_t* root = tree.root;
  tree_node_t* node1 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(1));
  tree_node_t* node2 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(2));
  tree_node_t* node12 =
      tree_find(&tree, NULL, TREE_FOREACH_TYPE_BREADTH_FIRST, tk_pointer_from_int(12));

  // 根节点的度为3（1/2/3三个子节点）
  ASSERT_EQ(tree_node_degree(root), 3);

  // 节点1的度为2（11/12两个子节点）
  ASSERT_EQ(tree_node_degree(node1), 2);

  // 节点11的度为0（叶子节点）
  ASSERT_EQ(tree_node_degree(tree_get_child_node(&tree, node1, 0)), 0);

  // 节点12的度为3（121/122/123三个子节点）
  ASSERT_EQ(tree_node_degree(node12), 3);

  // 树的度为3（来自节点12的度）
  ASSERT_EQ(tree_degree(&tree), 3);

  tree_deinit(&tree);
}
