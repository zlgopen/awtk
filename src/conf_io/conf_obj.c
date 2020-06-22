
/**
 * File:   conf_obj.h
 * Author: AWTK Develop Team
 * Brief:  conf obj
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
 * 2020-05-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object.h"
#include "conf_io/conf_obj.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"

typedef struct _conf_obj_t {
  object_t object;

  /*private*/
  char* url;
  conf_doc_t* doc;
  conf_doc_save_t save;
  conf_doc_load_t load;
  bool_t readonly;
} conf_obj_t;

static conf_obj_t* conf_obj_cast(object_t* obj);
#define CONF_OBJ(obj) conf_obj_cast((object_t*)obj)

static ret_t conf_obj_move_up(object_t* obj, const char* name) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return conf_doc_move_up(o->doc, name);
}

static ret_t conf_obj_move_down(object_t* obj, const char* name) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return conf_doc_move_down(o->doc, name);
}

static ret_t conf_obj_remove_prop(object_t* obj, const char* name) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->readonly) {
    return RET_NOT_IMPL;
  }

  return conf_doc_remove(o->doc, name);
}

static ret_t conf_obj_clear(object_t* obj, const char* name) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->readonly) {
    return RET_NOT_IMPL;
  }

  return conf_doc_clear(o->doc, name);
}

static ret_t conf_obj_set_prop(object_t* obj, const char* name, const value_t* v) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->readonly) {
    return RET_NOT_IMPL;
  }

  return conf_doc_set(o->doc, name, v);
}

static ret_t conf_obj_get_prop(object_t* obj, const char* name, value_t* v) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return conf_doc_get(o->doc, name, v);
}

ret_t conf_obj_save(object_t* obj) {
  ret_t ret = RET_FAIL;
  data_writer_t* writer = NULL;
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  writer = data_writer_factory_create_writer(data_writer_factory(), o->url);
  return_value_if_fail(writer != NULL, RET_FAIL);

  ret = o->save(o->doc, writer);
  data_writer_destroy(writer);

  return ret;
}

static ret_t conf_obj_load(object_t* obj) {
  data_reader_t* reader = NULL;
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  reader = data_reader_factory_create_reader(data_reader_factory(), o->url);
  return_value_if_fail(reader != NULL, RET_FAIL);

  o->doc = o->load(reader);
  data_reader_destroy(reader);

  if (o->doc == NULL) {
    o->doc = conf_doc_create(10);
  }

  return RET_OK;
}

static ret_t conf_obj_load_or_create(object_t* obj) {
  conf_obj_t* o = CONF_OBJ(obj);

  conf_obj_load(obj);

  if (o->doc == NULL) {
    o->doc = conf_doc_create(20);
  }

  if (o->doc != NULL) {
    if (o->doc->root == NULL) {
      o->doc->root = conf_doc_create_node(o->doc, CONF_NODE_ROOT_NAME);
    }
  }

  return RET_OK;
}

static ret_t conf_obj_reload(object_t* obj) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  conf_doc_destroy(o->doc);
  o->doc = NULL;

  return conf_obj_load_or_create(obj);
}

static bool_t conf_obj_can_exec(object_t* obj, const char* name, const char* args) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->readonly) {
    return FALSE;
  }

  if (tk_str_ieq(name, OBJECT_CMD_SAVE)) {
    return TRUE;
  } else if (tk_str_ieq(name, OBJECT_CMD_RELOAD)) {
    return TRUE;
  } else if (tk_str_ieq(name, OBJECT_CMD_MOVE_UP)) {
    return conf_doc_exists(o->doc, args) && !conf_doc_is_first(o->doc, args);
  } else if (tk_str_ieq(name, OBJECT_CMD_MOVE_DOWN)) {
    return conf_doc_exists(o->doc, args) && !conf_doc_is_last(o->doc, args);
  } else if (tk_str_ieq(name, OBJECT_CMD_CLEAR)) {
    return conf_doc_exists(o->doc, args);
  } else if (tk_str_ieq(name, OBJECT_CMD_REMOVE)) {
    return conf_doc_exists(o->doc, args);
  } else if (tk_str_ieq(name, OBJECT_CMD_ADD)) {
    return conf_doc_exists(o->doc, args);
  }

  return FALSE;
}

static ret_t conf_obj_exec(object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_NOT_IMPL;
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->readonly) {
    return RET_NOT_IMPL;
  }

  if (tk_str_ieq(name, OBJECT_CMD_SAVE)) {
    ret = conf_obj_save(obj);
  } else if (tk_str_ieq(name, OBJECT_CMD_RELOAD)) {
    conf_obj_reload(obj);
    ret = RET_ITEMS_CHANGED;
  } else if (tk_str_ieq(name, OBJECT_CMD_MOVE_UP)) {
    if (conf_obj_move_up(obj, args) == RET_OK) {
      ret = RET_ITEMS_CHANGED;
    } else {
      ret = RET_FAIL;
    }
  } else if (tk_str_ieq(name, OBJECT_CMD_MOVE_DOWN)) {
    if (conf_obj_move_down(obj, args) == RET_OK) {
      ret = RET_ITEMS_CHANGED;
    } else {
      ret = RET_FAIL;
    }
  } else if (tk_str_ieq(name, OBJECT_CMD_CLEAR)) {
    if (conf_obj_clear(obj, args) == RET_OK) {
      ret = RET_ITEMS_CHANGED;
    } else {
      ret = RET_FAIL;
    }
  } else if (tk_str_ieq(name, OBJECT_CMD_REMOVE)) {
    if (conf_obj_remove_prop(obj, args) == RET_OK) {
      ret = RET_ITEMS_CHANGED;
    } else {
      ret = RET_FAIL;
    }
  } else if (tk_str_ieq(name, OBJECT_CMD_ADD)) {
    if (conf_doc_add_child(o->doc, args) == RET_OK) {
      ret = RET_ITEMS_CHANGED;
    } else {
      ret = RET_FAIL;
    }
  } else {
    return RET_NOT_IMPL;
  }

  if (ret == RET_ITEMS_CHANGED) {
    emitter_dispatch_simple_event(EMITTER(obj), EVT_PROPS_CHANGED);
    emitter_dispatch_simple_event(EMITTER(obj), EVT_ITEMS_CHANGED);
  }

  return RET_OK;
}

static ret_t conf_obj_destroy(object_t* obj) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  conf_doc_destroy(o->doc);
  TKMEM_FREE(o->url);
  o->doc = NULL;

  return RET_OK;
}

conf_doc_t* conf_obj_get_doc(object_t* conf) {
  conf_obj_t* o = CONF_OBJ(conf);
  return_value_if_fail(o != NULL, NULL);

  return_value_if_fail(conf != NULL, NULL);

  return o->doc;
}

static const object_vtable_t s_conf_obj_vtable = {.type = "conf_obj",
                                                  .desc = "conf_obj",
                                                  .size = sizeof(conf_obj_t),
                                                  .is_collection = FALSE,
                                                  .exec = conf_obj_exec,
                                                  .can_exec = conf_obj_can_exec,
                                                  .remove_prop = conf_obj_remove_prop,
                                                  .get_prop = conf_obj_get_prop,
                                                  .set_prop = conf_obj_set_prop,
                                                  .on_destroy = conf_obj_destroy};

static conf_obj_t* conf_obj_cast(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_conf_obj_vtable, NULL);

  return (conf_obj_t*)obj;
}

object_t* conf_obj_create(conf_doc_save_t save, conf_doc_load_t load, const char* url,
                          bool_t create_if_not_exist) {
  conf_obj_t* o = NULL;
  object_t* obj = NULL;
  return_value_if_fail(save != NULL && load != NULL && url != NULL && *url, NULL);

  obj = object_create(&s_conf_obj_vtable);
  o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, NULL);

  o->save = save;
  o->load = load;
  o->url = tk_strdup(url);
  if (o->url == NULL) {
    OBJECT_UNREF(o);
  }
  return_value_if_fail(o != NULL, NULL);

  conf_obj_load_or_create(obj);

  if (o->doc == NULL || o->doc->root == NULL) {
    TKMEM_FREE(o->url);
    OBJECT_UNREF(obj);
  }

  return obj;
}

ret_t conf_obj_set_readonly(object_t* conf, bool_t readonly) {
  conf_obj_t* o = CONF_OBJ(conf);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  o->readonly = readonly;

  return RET_OK;
}
