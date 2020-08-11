/**
 * File:   conf_node.h
 * Author: AWTK Develop Team
 * Brief:  conf node
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-05-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CONF_NODE_H
#define TK_CONF_NODE_H

#include "tkc/types_def.h"
#include "tkc/value.h"
#include "tkc/tokenizer.h"

BEGIN_C_DECLS

struct _conf_node_t;
typedef struct _conf_node_t conf_node_t;

/**
 * @class conf_doc_t
 * 代表整个配置文件。
 */
typedef struct _conf_doc_t {
  /**
   * @property {conf_node_t*} root
   * 根节点。
   */
  conf_node_t* root;

  /*private*/
  conf_node_t* prealloc_nodes;
  uint32_t prealloc_nodes_index;
  uint32_t prealloc_nodes_used;
  uint32_t prealloc_nodes_nr;
  tokenizer_t tokenizer;
  uint32_t max_deep_level;
} conf_doc_t;

/**
 * @method conf_doc_create
 *
 * 构造函数。
 *
 * @param {uint32_t} prealloc_nodes_nr 预先分配的节点数。
 *
 * @return {conf_doc_t*} 返回doc对象。
 */
conf_doc_t* conf_doc_create(uint32_t prealloc_nodes_nr);

/**
 * @method conf_doc_create_node
 *
 * 创建一个空节点。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} name 节点名称。
 *
 * @return {conf_node_t*} 返回节点对象。
 */
conf_node_t* conf_doc_create_node(conf_doc_t* doc, const char* name);

/**
 * @method conf_doc_destroy_node
 *
 * 销毁节点对象。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {conf_node_t*} node 节点对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_destroy_node(conf_doc_t* doc, conf_node_t* node);

/**
 * @method conf_doc_append_sibling
 *
 * 追加一个兄弟节点。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {conf_node_t*} node 节点对象。
 * @param {conf_node_t*} sibling 待追加节点对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_append_sibling(conf_doc_t* doc, conf_node_t* node, conf_node_t* sibling);

/**
 * @method conf_doc_append_child
 *
 * 追加一个子节点。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {conf_node_t*} node 节点对象。
 * @param {conf_node_t*} child 待追加节点对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_append_child(conf_doc_t* doc, conf_node_t* node, conf_node_t* child);

/**
 * @method conf_doc_remove_sibling
 *
 * 删除指定的兄弟节点。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {conf_node_t*} node 节点对象。
 * @param {conf_node_t*} sibling 待删除节点对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_remove_sibling(conf_doc_t* doc, conf_node_t* node, conf_node_t* sibling);

/**
 * @method conf_doc_remove_child
 *
 * 删除指定的子节点。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {conf_node_t*} node 节点对象。
 * @param {conf_node_t*} child 待删除节点对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_remove_child(conf_doc_t* doc, conf_node_t* node, conf_node_t* child);

/**
 * @method conf_doc_remove_child_by_name
 *
 * 删除指定的子节点。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {conf_node_t*} node 节点对象。
 * @param {const char*} name 待删除节点的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_remove_child_by_name(conf_doc_t* doc, conf_node_t* node, const char* name);

/**
 * @method conf_doc_remove_children
 *
 * 删除全部子节点。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {conf_node_t*} node 节点对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_remove_children(conf_doc_t* doc, conf_node_t* node);

/**
 * @method conf_doc_set
 *
 * 设置指定路径节点的值。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 * @param {const value_t*} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_set(conf_doc_t* doc, const char* path, const value_t* v);

/**
 * @method conf_doc_set_int
 *
 * 设置指定路径节点的值。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 * @param {int32_t} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_set_int(conf_doc_t* doc, const char* path, int32_t v);

/**
 * @method conf_doc_set_bool
 *
 * 设置指定路径节点的值。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 * @param {bool_t} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_set_bool(conf_doc_t* doc, const char* path, bool_t v);

/**
 * @method conf_doc_set_float
 *
 * 设置指定路径节点的值。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 * @param {float} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_set_float(conf_doc_t* doc, const char* path, float v);

/**
 * @method conf_doc_set_str
 *
 * 设置指定路径节点的值。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 * @param {const char*} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_set_str(conf_doc_t* doc, const char* path, const char* v);

/**
 * @method conf_doc_get
 *
 * 获取指定路径节点的值。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 * @param {value_t*} v 用于返回值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_get(conf_doc_t* doc, const char* path, value_t* v);

/**
 * @method conf_doc_get_int
 *
 * 获取指定路径节点的值。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 * @param {int32_t} defval 缺省值。
 *
 * @return {int32_t} 返回值。
 */
int32_t conf_doc_get_int(conf_doc_t* doc, const char* path, int32_t defval);

/**
 * @method conf_doc_get_bool
 *
 * 获取指定路径节点的值。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 * @param {bool_t} defval 缺省值。
 *
 * @return {bool_t} 返回值。
 */
bool_t conf_doc_get_bool(conf_doc_t* doc, const char* path, bool_t defval);

/**
 * @method conf_doc_get_float
 *
 * 获取指定路径节点的值。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 * @param {float} defval 缺省值。
 *
 * @return {float} 返回值。
 */
float conf_doc_get_float(conf_doc_t* doc, const char* path, float defval);

/**
 * @method conf_doc_get_str
 *
 * 获取指定路径节点的值。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 * @param {const char*} defval 缺省值。
 *
 * @return {const char*} 返回值。
 */
const char* conf_doc_get_str(conf_doc_t* doc, const char* path, const char* defval);

/**
 * @method conf_doc_remove
 *
 * 删除指定路径的节点。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_remove(conf_doc_t* doc, const char* path);

/**
 * @method conf_doc_clear
 *
 * 删除指定路径的节点的全部子节点。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_clear(conf_doc_t* doc, const char* path);

/**
 * @method conf_doc_move_up
 *
 * 和前一个兄弟节点交换位置。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_move_up(conf_doc_t* doc, const char* path);

/**
 * @method conf_doc_move_down
 *
 * 和后一个兄弟节点交换位置。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_move_down(conf_doc_t* doc, const char* path);

/**
 * @method conf_doc_exists
 *
 * 判断指定路径的节点是否存在。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 *
 * @return {bool_t} 返回TRUE表示成功，FALSE表示失败。
 */
bool_t conf_doc_exists(conf_doc_t* doc, const char* path);

/**
 * @method conf_doc_is_first
 *
 * 检查指定节点是否在兄弟节点中排第一。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
bool_t conf_doc_is_first(conf_doc_t* doc, const char* path);

/**
 * @method conf_doc_is_last
 *
 * 检查指定节点是否在兄弟节点中排最后。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
bool_t conf_doc_is_last(conf_doc_t* doc, const char* path);

/**
 * @method conf_doc_add_child
 *
 * 追加一个子节点。
 *
 * @param {conf_doc_t*} doc 文档对象。
 * @param {const char*} path 节点的路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_add_child(conf_doc_t* doc, const char* path);

/**
 * @method conf_doc_destroy
 *
 * 析构函数。
 *
 * @param {conf_doc_t*} doc 文档对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_doc_destroy(conf_doc_t* doc);

/**
 * @enum conf_node_type_t 
 * @prefix CONF_NODE_
 * 节点类型。
 */
typedef enum _conf_node_type_t {
  /**
   * @const CONF_NODE_NONE 
   * 无效节点。
   */
  CONF_NODE_NONE = 0,
  /**
   * @const CONF_NODE_OBJECT 
   * 对象节点。
   */
  CONF_NODE_OBJECT,
  /**
   * @const CONF_NODE_ARRAY 
   * 数组节点。
   */
  CONF_NODE_ARRAY,
  /**
   * @const CONF_NODE_SIMPLE 
   * 简单节点。
   */
  CONF_NODE_SIMPLE
} conf_node_type_t;

/**
 * @enum conf_node_value_t 
 * @prefix CONF_NODE_VALUE_
 * 节点值得类型。
 */
typedef enum _conf_node_value_t {
  /**
   * @const CONF_NODE_VALUE_NONE 
   * 无效类型。
   */
  CONF_NODE_VALUE_NONE = 0,
  /**
   * @const CONF_NODE_VALUE_BOOL 
   * bool_t类型。
   */
  CONF_NODE_VALUE_BOOL,
  /**
   * @const CONF_NODE_VALUE_INT8 
   * int8_t类型。
   */
  CONF_NODE_VALUE_INT8,
  /**
   * @const CONF_NODE_VALUE_UINT8 
   * uint8_t类型。
   */
  CONF_NODE_VALUE_UINT8,
  /**
   * @const CONF_NODE_VALUE_INT16 
   * int16_t类型。
   */
  CONF_NODE_VALUE_INT16,
  /**
   * @const CONF_NODE_VALUE_UINT16 
   * uint16_t类型。
   */
  CONF_NODE_VALUE_UINT16,
  /**
   * @const CONF_NODE_VALUE_INT32
   * int32_t类型。
   */
  CONF_NODE_VALUE_INT32,
  /**
   * @const CONF_NODE_VALUE_UINT32
   * uint32_t类型。
   */
  CONF_NODE_VALUE_UINT32,
  /**
   * @const CONF_NODE_VALUE_INT64
   * int64_t类型。
   */
  CONF_NODE_VALUE_INT64,
  /**
   * @const CONF_NODE_VALUE_UINT64
   * uint64_t类型。
   */
  CONF_NODE_VALUE_UINT64,
  /**
   * @const CONF_NODE_VALUE_FLOAT32
   * float类型。
   */
  CONF_NODE_VALUE_FLOAT32,
  /**
   * @const CONF_NODE_VALUE_DOUBLE
   * double类型。
   */
  CONF_NODE_VALUE_DOUBLE,
  /**
   * @const CONF_NODE_VALUE_STRING
   * 字符串类型。
   */
  CONF_NODE_VALUE_STRING,
  /**
   * @const CONF_NODE_VALUE_SMALL_STR
   * 短字符(len<=7)串类型。
   */
  CONF_NODE_VALUE_SMALL_STR,
  /**
   * @const CONF_NODE_VALUE_NODE
   * 节点类型。
   */
  CONF_NODE_VALUE_NODE
} conf_node_value_t;

/**
 * @class conf_node_t
 * 节点。
 *
 */
struct _conf_node_t {
  /**
   * @property {conf_node_t*} next 
   * 下一个兄弟节点。
   */
  conf_node_t* next;
  /**
   * @property {conf_node_t*} parent
   * 父节点。
   */
  conf_node_t* parent;

  /**
   * @property {uint8_t} value_type
   * 值的类型。
   */
  uint8_t value_type : 4;
  /**
   * @property {uint8_t} node_type
   * 节点类型。
   */
  uint8_t node_type : 3;

  /*private*/
  uint8_t is_small_name : 1;

  union {
    char* str;
    char small_str[8];
  } name;

  union {
    bool_t b;
    int8_t i8;
    uint8_t u8;
    int16_t i16;
    uint16_t u16;
    int32_t i32;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
    float f32;
    double f64;
    char* str;
    char small_str[8];
    conf_node_t* first_child;
  } value;
};

/**
 * @method conf_node_get_name
 *
 * 获得节点的名称。
 *
 * @param {conf_node_t*} node 节点对象。
 *
 * @return {const char*} 返回节点名称。
 */
const char* conf_node_get_name(conf_node_t* node);

/**
 * @method conf_node_find_child
 *
 * 查找指定名称的子节点。
 *
 * @param {conf_node_t*} node 节点对象。
 * @param {const char*} name 节点名称。
 *
 * @return {conf_node_t*} 返回节点对象。
 */
conf_node_t* conf_node_find_child(conf_node_t* node, const char* name);

/**
 * @method conf_node_find_sibling
 *
 * 查找指定名称的兄弟节点。
 *
 * @param {conf_node_t*} node 节点对象。
 * @param {const char*} name 节点名称。
 *
 * @return {conf_node_t*} 返回节点对象。
 */
conf_node_t* conf_node_find_sibling(conf_node_t* node, const char* name);

/**
 * @method conf_node_set_value
 *
 * 设置节点的值。
 *
 * @param {conf_node_t*} node 节点对象。
 * @param {const value_t*} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_node_set_value(conf_node_t* node, const value_t* v);

/**
 * @method conf_node_get_value
 *
 * 设置节点的值。
 *
 * @param {conf_node_t*} node 节点对象。
 * @param {value_t*} v 值(返回)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_node_get_value(conf_node_t* node, value_t* v);

/**
 * @method conf_node_get_first_child
 *
 * 获取第一个子节点。
 *
 * @param {conf_node_t*} node 节点对象。
 *
 * @return {conf_node_t*} 返回第一个子节点。
 */
conf_node_t* conf_node_get_first_child(conf_node_t* node);

/**
 * @method conf_node_set_first_child
 *
 * 设置第一个子节点。
 *
 * @param {conf_node_t*} node 节点对象。
 * @param {conf_node_t*} child 子节点对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t conf_node_set_first_child(conf_node_t* node, conf_node_t* child);

#define CONF_NODE_ROOT_NAME "root"

#define CONF_SPECIAL_ATTR_SIZE "#size"
#define CONF_SPECIAL_ATTR_NAME "#name"
#define CONF_SPECIAL_ATTR_INDEX "#index"

END_C_DECLS

#endif /*TK_CONF_NODE_H*/
