#include "svgtiny_dom.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/object_array.h"

typedef enum {
  DOM_TYPE_DOCUMENT,
  DOM_TYPE_ID_MAP,
  DOM_TYPE_ELEMENT,
  DOM_TYPE_TEXT,
  DOM_TYPE_ATTRS,
} dom_obj_type;

#define DOM_PROP_TYPE "type"
#define DOM_PROP_PARENT "parent"
#define DOM_PROP_CHILDREN "children"
#define DOM_PROP_NEXT_CHILD "next_child"
#define DOM_PROP_ID_MAP "id_map"
#define DOM_PROP_ATTRS "attrs"
#define DOM_PROP_TEXT "text"

static inline tk_object_t* dom_xml_parser_create_obj(dom_obj_type type, tk_object_t* parent) {
  tk_object_t* obj = object_default_create_ex(FALSE);
  tk_object_t* children = NULL;
  tk_object_t* prev_child = NULL;
  return_value_if_fail(obj != NULL, NULL);
  tk_object_set_prop_int32(obj, DOM_PROP_TYPE, type);
  if (parent) {
    children = tk_object_get_prop_object(parent, DOM_PROP_CHILDREN);
    if (children == NULL) {
      children = object_array_create();
      tk_object_set_prop_object(parent, DOM_PROP_CHILDREN, children);
      prev_child = NULL;
    } else {
      object_array_t* arr = OBJECT_ARRAY(children);
      prev_child = value_object(arr->props + (arr->size - 1));
    }
    tk_object_set_prop_pointer(obj, DOM_PROP_PARENT, parent);
    tk_object_set_prop_object(children, "-1", obj);
    if (prev_child) {
      tk_object_set_prop_pointer(prev_child, DOM_PROP_NEXT_CHILD, obj);
    }
  }
  return obj;
}

static void dom_xml_parser_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  dom_xml_parser* parser = (dom_xml_parser*)thiz;
  str_t* str = &parser->str;
  uint32_t i = 0;
  dom_node* node = NULL;
  dom_node* attr = NULL;

  node = dom_xml_parser_create_obj(DOM_TYPE_ELEMENT, parser->node);
  return_if_fail(node != NULL);
  attr = dom_xml_parser_create_obj(DOM_TYPE_ATTRS, NULL);
  return_if_fail(attr != NULL);

  tk_object_set_name(node, tag);
  tk_object_set_prop_object(node, DOM_PROP_ATTRS, attr);

  while (attrs[i]) {
    const char* name = attrs[i];
    const char* value = attrs[i + 1];

    str_clear(str);
    str_decode_xml_entity(str, value);
    tk_object_set_prop_str(attr, name, str->str);

    if (tk_str_eq(name, "id") || tk_str_end_with(name, ":id")) {
      tk_object_t* id_map = tk_object_get_prop_object(parser->doc, DOM_PROP_ID_MAP);
      if (id_map == NULL) {
        id_map = dom_xml_parser_create_obj(DOM_TYPE_ID_MAP, NULL);
        tk_object_set_prop_object(parser->doc, DOM_PROP_ID_MAP, id_map);
        tk_object_unref(id_map);
      }
      tk_object_set_prop_object(id_map, str->str, node);
    }

    i += 2;
  }

  parser->node = node;

  TK_OBJECT_UNREF(node);
  TK_OBJECT_UNREF(attr);
}

static void dom_xml_parser_on_end(XmlBuilder* thiz, const char* tag) {
  dom_xml_parser* parser = (dom_xml_parser*)thiz;

  if (parser->node != NULL) {
    parser->node = (tk_object_t*)tk_object_get_prop_pointer(parser->node, DOM_PROP_PARENT);
  }
}

static void dom_xml_parser_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  dom_xml_parser* parser = (dom_xml_parser*)thiz;
  str_t* str = &parser->str;

  if (text != NULL && length > 0) {
    str_clear(str);
    str_decode_xml_entity(str, text);
    str_trim(str, "\r\n\t ");
    if (str->size > 0) {
      dom_node* node = dom_xml_parser_create_obj(DOM_TYPE_TEXT, parser->node);
      return_if_fail(node != NULL);
      tk_object_set_prop_str(node, DOM_PROP_TEXT, str->str);
      TK_OBJECT_UNREF(node);
    }
  }
}

static void dom_xml_parser_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  log_debug("%d:%d %s\n", line, row, message);
}

dom_xml_parser* dom_xml_parser_create(const char* enc, const char* int_enc, void* msg, void* mctx,
                                      dom_document** document) {
  dom_document* doc = NULL;
  dom_xml_parser* parser = (dom_xml_parser*)TKMEM_ALLOC(sizeof(dom_xml_parser));
  return_value_if_fail(parser != NULL, NULL);

  memset(parser, 0, sizeof(dom_xml_parser));
  parser->builder.on_start = dom_xml_parser_on_start;
  parser->builder.on_end = dom_xml_parser_on_end;
  parser->builder.on_text = dom_xml_parser_on_text;
  parser->builder.on_error = dom_xml_parser_on_error;

  str_init(&parser->str, 100);

  doc = dom_xml_parser_create_obj(DOM_TYPE_DOCUMENT, NULL);
  if (doc == NULL) {
    dom_xml_parser_destroy(parser);
    return NULL;
  }

  parser->doc = doc;
  parser->node = doc;
  *document = doc;
  return parser;
}
dom_xml_error dom_xml_parser_parse_chunk(dom_xml_parser* parser, uint8_t* data, size_t len) {
  return_value_if_fail(parser != NULL, DOM_XML_EXTERNAL_ERR);

  XmlParser* p = xml_parser_create();
  xml_parser_set_builder(p, (XmlBuilder*)parser);
  xml_parser_parse(p, (const char*)data, len);
  xml_parser_destroy(p);

  return DOM_XML_OK;
}
dom_xml_error dom_xml_parser_completed(dom_xml_parser* parser) {
  return_value_if_fail(parser != NULL && parser->str.size > 0, DOM_XML_EXTERNAL_ERR);

  return DOM_XML_OK;
}
void dom_xml_parser_destroy(dom_xml_parser* parser) {
  return_if_fail(parser != NULL);

  str_reset(&parser->str);

  TKMEM_FREE(parser);
}

dom_exception dom_document_get_document_element(dom_document* doc, dom_element** result) {
  object_array_t* children = NULL;
  uint32_t i;
  return_value_if_fail(doc != NULL && result != NULL, DOM_ERR);

  children = OBJECT_ARRAY(tk_object_get_prop_object(doc, DOM_PROP_CHILDREN));
  if (children == NULL) {
    return DOM_ERR;
  }

  for (i = 0; i < children->size; i++) {
    value_t* iter = children->props + i;
    tk_object_t* child = value_object(iter);
    if (dom_string_caseless_lwc_isequal(child, "svg")) {
      *result = child;
      TK_OBJECT_REF(*result);
      return DOM_NO_ERR;
    }
  }
  return DOM_ERR;
}
dom_exception dom_document_get_element_by_id(dom_document* doc, dom_string* id,
                                             dom_element** result) {
  tk_object_t* id_map = NULL;
  return_value_if_fail(doc != NULL && id != NULL && id->name != NULL && result != NULL, DOM_ERR);

  id_map = tk_object_get_prop_object(doc, DOM_PROP_ID_MAP);
  return_value_if_fail(id_map != NULL, DOM_ERR);

  *result = tk_object_get_prop_object(id_map, id->name);
  TK_OBJECT_REF(*result);
  return DOM_NO_ERR;
}

dom_exception dom_node_get_first_child(dom_node* node, dom_node** result) {
  tk_object_t* children = NULL;
  return_value_if_fail(node != NULL && result != NULL, DOM_ERR);

  children = tk_object_get_prop_object(node, DOM_PROP_CHILDREN);
  if (children == NULL) {
    return DOM_ERR;
  }

  *result = tk_object_get_prop_object(children, "0");
  if (*result) {
    TK_OBJECT_REF(*result);
  }
  return DOM_NO_ERR;
}
dom_exception dom_node_get_next_sibling(dom_node* node, dom_node** result) {
  return_value_if_fail(node != NULL && result != NULL, DOM_ERR);

  *result = (tk_object_t*)tk_object_get_prop_pointer(node, DOM_PROP_NEXT_CHILD);
  if (*result) {
    TK_OBJECT_REF(*result);
  }
  return DOM_NO_ERR;
}
dom_exception dom_node_get_node_name(dom_node* node, dom_string** result) {
  return_value_if_fail(node != NULL && result != NULL, DOM_ERR);

  *result = node;
  TK_OBJECT_REF(*result);
  return DOM_NO_ERR;
}
dom_exception dom_node_get_node_type(dom_node* node, dom_node_type* result) {
  dom_obj_type type = DOM_TYPE_DOCUMENT;
  return_value_if_fail(node != NULL && result != NULL, DOM_ERR);

  type = tk_object_get_prop_int32(node, DOM_PROP_TYPE, type);
  if (type == DOM_TYPE_ELEMENT) {
    *result = DOM_ELEMENT_NODE;
  } else if (type == DOM_TYPE_TEXT) {
    *result = DOM_TEXT_NODE;
  } else {
    *result = DOM_UNKNOWN_NODE;
  }
  return DOM_NO_ERR;
}

dom_exception dom_text_get_whole_text(dom_text* text, dom_string** result) {
  const char* str = NULL;
  return_value_if_fail(text != NULL && result != NULL, DOM_ERR);

  str = tk_object_get_prop_str(text, DOM_PROP_TEXT);
  return_value_if_fail(TK_STR_IS_NOT_EMPTY(str), DOM_ERR);

  return dom_string_create_interned((const uint8_t*)str, strlen(str), result);
}

static int32_t named_value_compare_by_attr_name(named_value_t* nv, const char* name) {
  const char* real_name;
  return_value_if_fail(nv != NULL && nv->name != NULL && name != NULL, -1);

  real_name = strstr(nv->name, ":");
  if (real_name != NULL) {
    return tk_str_cmp(real_name + 1, name);
  } else {
    return -1;
  }
}

dom_exception dom_element_get_attribute(dom_element* element, dom_string* name,
                                        dom_string** value) {
  tk_object_t* attr = NULL;
  const char* str = NULL;
  return_value_if_fail(element != NULL && name != NULL && name->name != NULL && value != NULL,
                       DOM_ERR);

  attr = tk_object_get_prop_object(element, DOM_PROP_ATTRS);
  return_value_if_fail(attr != NULL, DOM_ERR);

  str = tk_object_get_prop_str(attr, name->name);
  if (TK_STR_IS_EMPTY(str)) {
    value_t* v = object_default_find_prop(attr, (tk_compare_t)named_value_compare_by_attr_name, name->name);
    str = v ? value_str(v) : NULL;
    if (TK_STR_IS_EMPTY(str)) {
      *value = NULL;
      return DOM_NO_ERR;
    }
  }

  return dom_string_create_interned((const uint8_t*)str, strlen(str), value);
}
dom_exception dom_element_get_elements_by_tag_name(dom_element* element, dom_string* name,
                                                   dom_nodelist** result) {
  object_array_t* children = NULL;
  uint32_t i;
  return_value_if_fail(element != NULL && name != NULL && name->name != NULL && result != NULL,
                       DOM_ERR);

  children = OBJECT_ARRAY(tk_object_get_prop_object(element, DOM_PROP_CHILDREN));
  if (children == NULL) {
    return DOM_ERR;
  }

  *result = object_array_create();
  return_value_if_fail(*result != NULL, DOM_ERR);

  for (i = 0; i < children->size; i++) {
    value_t* iter = children->props + i;
    tk_object_t* child = value_object(iter);
    if (dom_string_caseless_isequal(child, name)) {
      object_array_push(*result, iter);
    }
  }
  return DOM_NO_ERR;
}

dom_exception dom_nodelist_get_length(dom_nodelist* list, uint32_t* length) {
  object_array_t* arr = OBJECT_ARRAY(list);
  return_value_if_fail(arr != NULL && length != NULL, DOM_ERR);

  *length = arr->size;
  return DOM_NO_ERR;
}
dom_exception dom_nodelist_item(dom_nodelist* list, uint32_t index, dom_node** node) {
  value_t v;
  return_value_if_fail(list != NULL && node != NULL, DOM_ERR);

  return_value_if_fail(object_array_get(list, index, &v) == RET_OK, DOM_ERR);

  *node = value_object(&v);
  TK_OBJECT_REF(*node);
  return DOM_NO_ERR;
}

dom_exception dom_string_create_interned(const uint8_t* ptr, size_t len, dom_string** str) {
  tk_object_t* obj = NULL;
  return_value_if_fail(ptr != NULL && len > 0 && str != NULL, DOM_ERR);

  obj = object_default_create_ex(FALSE);
  return_value_if_fail(obj != NULL, DOM_ERR);

  obj->name = tk_strndup((const char*)ptr, len);
  *str = obj;
  return DOM_NO_ERR;
}
bool_t dom_string_isequal(const dom_string* s1, const dom_string* s2) {
  return tk_str_eq(s1->name, s2->name);
}
bool_t dom_string_caseless_isequal(const dom_string* s1, const dom_string* s2) {
  return tk_str_ieq(s1->name, s2->name);
}
bool_t dom_string_caseless_lwc_isequal(const dom_string* s1, const char* s2) {
  return tk_str_ieq(s1->name, s2);
}
const char* dom_string_data(const dom_string* str) {
  return str->name;
}
size_t dom_string_byte_length(const dom_string* str) {
  return strlen(str->name);
}
