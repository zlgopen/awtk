#ifndef SVGTINY_DOM_H
#define SVGTINY_DOM_H

#include "tkc/object.h"
#include "xml/xml_parser.h"

typedef tk_object_t dom_document;
typedef tk_object_t dom_node;
typedef tk_object_t dom_nodelist;
typedef tk_object_t dom_element;
typedef tk_object_t dom_text;
typedef tk_object_t dom_string;

typedef struct _dom_xml_parser {
  XmlBuilder builder;
  str_t str;
  dom_document* doc;
  dom_node* node;
} dom_xml_parser;

typedef enum {
  DOM_XML_OK,
  DOM_XML_EXTERNAL_ERR,
} dom_xml_error;

typedef enum {
  DOM_NO_ERR,
  DOM_ERR,
} dom_exception;

typedef enum {
  DOM_UNKNOWN_NODE,
  DOM_ELEMENT_NODE,
  DOM_TEXT_NODE,
} dom_node_type;

#define dom_node_unref TK_OBJECT_UNREF
#define dom_string_ref TK_OBJECT_REF
#define dom_string_unref TK_OBJECT_UNREF
#define dom_nodelist_unref TK_OBJECT_UNREF

dom_xml_parser* dom_xml_parser_create(const char* enc, const char* int_enc, void* msg, void* mctx,
                                      dom_document** document);
dom_xml_error dom_xml_parser_parse_chunk(dom_xml_parser* parser, uint8_t* data, size_t len);
dom_xml_error dom_xml_parser_completed(dom_xml_parser* parser);
void dom_xml_parser_destroy(dom_xml_parser* parser);

dom_exception dom_document_get_document_element(dom_document* doc, dom_element** result);
dom_exception dom_document_get_element_by_id(dom_document* doc, dom_string* id,
                                             dom_element** result);

dom_exception dom_node_get_first_child(dom_node* node, dom_node** result);
dom_exception dom_node_get_next_sibling(dom_node* node, dom_node** result);
dom_exception dom_node_get_node_name(dom_node* node, dom_string** result);
dom_exception dom_node_get_node_type(dom_node* node, dom_node_type* result);

dom_exception dom_text_get_whole_text(dom_node* text, dom_string** result);

dom_exception dom_element_get_attribute(dom_element* element, dom_string* name, dom_string** value);
dom_exception dom_element_get_elements_by_tag_name(dom_element* element, dom_string* name,
                                                   dom_nodelist** result);

dom_exception dom_nodelist_get_length(dom_nodelist* list, uint32_t* length);
dom_exception dom_nodelist_item(dom_nodelist* list, uint32_t index, dom_node** node);

dom_exception dom_string_create_interned(const uint8_t* ptr, size_t len, dom_string** str);
bool_t dom_string_isequal(const dom_string* s1, const dom_string* s2);
bool_t dom_string_caseless_isequal(const dom_string* s1, const dom_string* s2);
bool_t dom_string_caseless_lwc_isequal(const dom_string* s1, const char* s2);
const char* dom_string_data(const dom_string* str);
size_t dom_string_byte_length(const dom_string* str);

#endif
