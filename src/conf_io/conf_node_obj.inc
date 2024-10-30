/**
 * File:   conf_node_obj.inc
 * Author: AWTK Develop Team
 * Brief:  conf node object
 *
 * Copyright (c) 2023 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-09-15 Shen ZhaoKun <xianjimli@hotmail.com> created
 *
 */

#include "tkc/object.h"
#include "tkc/named_value.h"
#include "conf_io/conf_node.h"

typedef struct _conf_node_obj_t {
  tk_object_t object;

  /*private*/
  conf_doc_t* doc;
  conf_node_t* node;
} conf_node_obj_t;

conf_node_obj_t* conf_node_obj_cast(tk_object_t* obj);
#define CONF_NODE_OBJ(obj) conf_node_obj_cast(obj)

static ret_t conf_node_obj_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  conf_node_obj_t* o = CONF_NODE_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  return_value_if_fail(o->doc != NULL && o->node != NULL, RET_FAIL);

  return conf_doc_get_ex(o->doc, o->node, name, v);
}

static ret_t conf_node_obj_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  conf_node_obj_t* o = CONF_NODE_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  return_value_if_fail(o->doc != NULL && o->node != NULL, RET_FAIL);

  return conf_doc_set_ex(o->doc, o->node, name, v);
}

static ret_t conf_node_obj_remove_prop(tk_object_t* obj, const char* name) {
  conf_node_obj_t* o = CONF_NODE_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  return_value_if_fail(o->doc != NULL && o->node != NULL, RET_FAIL);

  return conf_doc_remove_child_by_name(o->doc, o->node, name);
}

static ret_t conf_node_obj_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  conf_node_obj_t* o = CONF_NODE_OBJ(obj);
  conf_node_t* child = NULL;
  ret_t ret = RET_OK;
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  return_value_if_fail(on_prop != NULL, RET_BAD_PARAMS);
  return_value_if_fail(o->doc != NULL && o->node != NULL, RET_FAIL);

  for (child = conf_node_get_first_child(o->node); child != NULL && RET_OK == ret;
       child = child->next) {
    named_value_t nv;
    named_value_init(&nv, conf_node_get_name(child), NULL);
    ret = conf_doc_get_ex(o->doc, o->node, nv.name, &nv.value);
    if (RET_OK == ret) {
      ret = on_prop(ctx, (const void*)&nv);
    }
    named_value_deinit(&nv);
  }

  return ret;
}

static const object_vtable_t s_conf_node_obj_vtable = {
    .type = "conf_node_obj",
    .desc = "conf node object",
    .size = sizeof(conf_node_obj_t),
    .is_collection = FALSE,
    .get_prop = conf_node_obj_get_prop,
    .set_prop = conf_node_obj_set_prop,
    .remove_prop = conf_node_obj_remove_prop,
    .foreach_prop = conf_node_obj_foreach_prop,
};

static const object_vtable_t s_conf_node_obj_array_vtable = {
    .type = "conf_node_obj",
    .desc = "conf node object array",
    .size = sizeof(conf_node_obj_t),
    .is_collection = TRUE,
    .get_prop = conf_node_obj_get_prop,
    .set_prop = conf_node_obj_set_prop,
    .remove_prop = conf_node_obj_remove_prop,
    .foreach_prop = conf_node_obj_foreach_prop,
};

tk_object_t* conf_node_obj_create(conf_doc_t* doc, conf_node_t* node) {
  conf_node_obj_t* obj = NULL;
  return_value_if_fail(doc != NULL && node != NULL, NULL);

  if (CONF_NODE_ARRAY == node->node_type) {
    obj = CONF_NODE_OBJ(tk_object_create(&s_conf_node_obj_array_vtable));
  } else {
    obj = CONF_NODE_OBJ(tk_object_create(&s_conf_node_obj_vtable));
  }
  return_value_if_fail(obj != NULL, NULL);

  obj->doc = doc;
  obj->node = node;

  return TK_OBJECT(obj);
}

conf_node_obj_t* conf_node_obj_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && (obj->vt == &s_conf_node_obj_vtable ||
                                       obj->vt == &s_conf_node_obj_array_vtable),
                       NULL);
  return (conf_node_obj_t*)(obj);
}
