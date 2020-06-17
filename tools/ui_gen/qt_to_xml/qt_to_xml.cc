/**
 * File:   qt to xml
 * Author: AWTK Develop Team
 * Brief:  qt to xml
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-03-14 Li XianJing <xianjimli@hotmail.com> created.
 *
 */

#include <map>
#include <vector>
#include <string>
#include "tkc/mem.h"
#include "common/utils.h"
#include "xml/xml_parser.h"

using std::string;
using std::vector;

typedef struct _key_value_t {
  const char* key;
  const char* value;
} key_value_t;

static const key_value_t qt_tk_map[] = {{"QLabel", "label"},
                                        {"QLineEdit", "edit"},
                                        {"QWidget", "view"},
                                        {"QGraphicsView", "image"},
                                        {"QComboBox", "combo_box"},
                                        {"QPushButton", "button"},
                                        {"QCheckBox", "check_button"},
                                        {"QRadioButton", "radio_button"},
                                        {"QProgressBar", "progress_bar"},
                                        {"QSpinBox", "spin_box"},
                                        {"QDoubleSpinBox", "spin_box"},
                                        {"QSlider", "slider"},
                                        {"QDialog", "dialog"},
                                        {"QMainWindow", "window"},
                                        {NULL, NULL}};

static string map_type(const string& type) {
  uint32_t i = 0;

  for (i = 0; i < sizeof(qt_tk_map) / sizeof(qt_tk_map[0]); i++) {
    const key_value_t* iter = qt_tk_map + i;
    if (iter->key == NULL) {
      break;
    }

    if (iter->key == type) {
      return iter->value;
    }
  }

  return type;
}

class WidgetInfo {
 public:
  WidgetInfo(WidgetInfo* parent, const string& type) {
    this->parent = parent;
    this->type = map_type(type);
    this->supported = this->type != type;

    if (parent != NULL) {
      parent->children.push_back(this);
    }
  }

  ~WidgetInfo() {
  }

 public:
  string type;
  bool supported;
  string properties;
  WidgetInfo* parent;
  vector<WidgetInfo*> children;
};

typedef struct _xml_builder_t {
  XmlBuilder builder;

  string tag;
  string property;
  WidgetInfo* root;
  WidgetInfo* widget;
} xml_builder_t;

static void qt_to_xml_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  uint32_t i = 0;
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (strcmp(tag, "widget") == 0 || strcmp(tag, "action") == 0) {
    const char* widget_name = NULL;
    const char* widget_class = tag;

    while (attrs[i]) {
      const char* name = attrs[i];
      const char* value = attrs[i + 1];

      if (strcmp(name, "class") == 0) {
        widget_class = value;
      } else if (strcmp(name, "name") == 0) {
        widget_name = value;
      }

      i += 2;
    }

    b->widget = new WidgetInfo(b->widget, widget_class);
    b->widget->properties = string("name=\"") + string(widget_name) + string("\"");
    if (b->root == NULL) {
      b->root = b->widget;
    }

    if (string(widget_class) == string("QDoubleSpinBox")) {
      b->widget->properties = "input_type=\"float\" ";
    } else if (string(widget_class) == string("QSpinBox")) {
      b->widget->properties = "input_type=\"int\" ";
    }

  } else if (strcmp(tag, "property") == 0) {
    const char* property = NULL;
    while (attrs[i]) {
      const char* name = attrs[i];
      const char* value = attrs[i + 1];

      if (strcmp(name, "name") == 0) {
        property = value;
      }

      i += 2;
    }
    b->property = property;
  } else {
    b->tag = tag;
  }

  return;
}

static void qt_to_xml_on_end(XmlBuilder* thiz, const char* tag) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (strcmp(tag, "widget") == 0) {
    b->widget = b->widget->parent;
  }

  return;
}

static void qt_to_xml_on_text(XmlBuilder* thiz, const char* str, size_t length) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  char* text = (char*)malloc(length + 1);

  memcpy(text, str, length);
  text[length] = '\0';

  if (b->tag == "x") {
    b->widget->properties += string(" x=\"") + string(text) + string("\"");
  } else if (b->tag == "y") {
    b->widget->properties += string(" y=\"") + string(text) + string("\"");
  } else if (b->tag == "width") {
    b->widget->properties += string(" w=\"") + string(text) + string("\"");
  } else if (b->tag == "height") {
    b->widget->properties += string(" h=\"") + string(text) + string("\"");
  } else if (b->property.size() > 0) {
    b->widget->properties += " " + b->property + string("=\"") + string(text) + string("\"");
  }
  free(text);

  return;
}

static void qt_to_xml_on_comment(XmlBuilder* thiz, const char* text, size_t length) {
  (void)thiz;
  (void)text;
  (void)length;

  return;
}

static void qt_to_xml_on_pi(XmlBuilder* thiz, const char* tag, const char** attrs) {
  (void)thiz;
  (void)tag;
  (void)attrs;
  return;
}

static void qt_to_xml_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  (void)thiz;
  printf("parse error: %d:%d %s\n", line, row, message);
  return;
}

static void qt_to_xml_destroy(XmlBuilder* thiz) {
  (void)thiz;
  return;
}

static XmlBuilder* builder_init(xml_builder_t& b) {
  b.root = NULL;
  b.widget = NULL;
  b.builder.on_start = qt_to_xml_on_start;
  b.builder.on_end = qt_to_xml_on_end;
  b.builder.on_text = qt_to_xml_on_text;
  b.builder.on_error = qt_to_xml_on_error;
  b.builder.on_comment = qt_to_xml_on_comment;
  b.builder.on_pi = qt_to_xml_on_pi;
  b.builder.destroy = qt_to_xml_destroy;

  return &(b.builder);
}

static void write_and_destroy_widget_info(WidgetInfo* info, FILE* fp) {
  if (info->supported) {
    if (strstr(info->properties.c_str(), "x=") == NULL) {
      info->properties += " x=\"0\"";
    }
    if (strstr(info->properties.c_str(), "y=") == NULL) {
      info->properties += " y=\"0\"";
    }
    if (strstr(info->properties.c_str(), "w=") == NULL) {
      info->properties += " w=\"100%\"";
    }
    if (strstr(info->properties.c_str(), "h=") == NULL) {
      info->properties += " h=\"100%\"";
    }

    if (info->children.size() > 0) {
      fprintf(fp, "<%s %s>\n", info->type.c_str(), info->properties.c_str());
      for (vector<WidgetInfo*>::const_iterator iter = info->children.begin();
           iter != info->children.end(); iter++) {
        write_and_destroy_widget_info(*iter, fp);
      }
      fprintf(fp, "</%s>\n", info->type.c_str());
    } else {
      fprintf(fp, "<%s %s />\n", info->type.c_str(), info->properties.c_str());
    }
  }
  delete info;
}

bool qt_to_xml(const char* input_file, const char* output_file) {
  xml_builder_t b;
  return_value_if_fail(input_file != NULL && output_file != NULL, false);

  XmlParser* parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(b));
  xml_parser_parse_file(parser, input_file);
  xml_parser_destroy(parser);

  WidgetInfo* root = b.root;
  if (root != NULL) {
    FILE* fp = fopen(output_file, "wb+");
    if (fp != NULL) {
      write_and_destroy_widget_info(root, fp);
      fclose(fp);
    }
  }

  return true;
}

int main(int argc, char** argv) {
  const char* in_filename = NULL;
  const char* out_filename = NULL;

  platform_prepare();

  if (argc != 3) {
    printf("Usage: %s in_filename out_filename\n", argv[0]);
    printf("   Ex: %s demo1.ui demo1.xml\n", argv[0]);
    return 0;
  }

  in_filename = argv[1];
  out_filename = argv[2];

  qt_to_xml(in_filename, out_filename);

  return 0;
}
