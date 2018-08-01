/**
 * File:   ui_serializer.c
 * Author: AWTK Develop Team
 * Brief:  ui_serializer
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utf8.h"
#include "base/utils.h"
#include "ui_loader/ui_serializer.h"

ret_t ui_widget_serialize_prop(ui_builder_t* writer, const char* name, value_t* value) {
  str_t* str = &(((ui_xml_writer_t*)writer)->temp);

  return_value_if_fail(str_from_value(str, value) == RET_OK, RET_OOM);

  return ui_builder_on_widget_prop(writer, name, str->str);
}

ret_t ui_widget_serialize_children_layout(ui_builder_t* writer, children_layout_t* layout) {
  char temp[32];
  str_t* str = &(((ui_xml_writer_t*)writer)->temp);

  if (layout->cols_is_width) {
    tk_snprintf(temp, sizeof(temp) - 1, "w:%d", (int)(layout->cols));
  } else {
    tk_snprintf(temp, sizeof(temp) - 1, "c:%d", (int)(layout->cols));
  }
  str_append(str, temp);
  str_append(str, " ");

  if (layout->rows_is_height) {
    tk_snprintf(temp, sizeof(temp) - 1, "h:%d", (int)(layout->rows));
  } else {
    tk_snprintf(temp, sizeof(temp) - 1, "r:%d", (int)(layout->rows));
  }
  str_append(str, temp);
  str_append(str, " ");

  if (layout->x_margin) {
    tk_snprintf(temp, sizeof(temp) - 1, "x:%d", (int)(layout->x_margin));
    str_append(str, temp);
  }

  if (layout->y_margin) {
    tk_snprintf(temp, sizeof(temp) - 1, "y:%d", (int)(layout->y_margin));
    str_append(str, temp);
  }

  if (layout->spacing) {
    tk_snprintf(temp, sizeof(temp) - 1, "s:%d", (int)(layout->spacing));
    str_append(str, temp);
  }

  return ui_builder_on_widget_prop(writer, "layout", str->str);
}

ret_t ui_widget_serialize(ui_builder_t* writer, widget_t* widget) {
  widget_desc_t desc;

  if (widget->auto_created) {
    return RET_OK;
  }

  memset(&desc, 0x00, sizeof(desc));
  tk_strncpy(desc.type, widget->vt->type, NAME_LEN);

  if (widget->layout_params && widget->layout_params->self.inited) {
    desc.layout = widget->layout_params->self;
  } else {
    desc.layout.x = widget->x;
    desc.layout.y = widget->y;
    desc.layout.w = widget->w;
    desc.layout.h = widget->h;
  }

  ui_builder_on_widget_start(writer, &desc);

  if (widget->name != NULL) {
    ui_builder_on_widget_prop(writer, WIDGET_PROP_NAME, widget->name);
  }
  if (widget->tr_text != NULL) {
    ui_builder_on_widget_prop(writer, WIDGET_PROP_TR_TEXT, widget->tr_text);
  }
  if (widget->text.size) {
    uint32_t size = widget->text.size * 3 + 1;
    char* text = TKMEM_ALLOC(size);
    utf8_from_utf16(widget->text.str, text, size);
    ui_builder_on_widget_prop(writer, "text", text);
    TKMEM_FREE(text);
  }

  if (widget->vt->clone_properties || widget->vt->persistent_properties) {
    value_t v;
    uint32_t i = 0;
    const char** properties = widget->vt->persistent_properties;
    if (properties == NULL) {
      properties = widget->vt->clone_properties;
    }
    for (i = 0; properties[i] != NULL; i++) {
      const char* prop = properties[i];
      if (widget_get_prop(widget, prop, &v) == RET_OK) {
        ui_widget_serialize_prop(writer, prop, &v);
      }
    }
  }

  if (widget->layout_params != NULL && widget->layout_params->children.inited) {
    ui_widget_serialize_children_layout(writer, &(widget->layout_params->children));
  }

  ui_builder_on_widget_prop_end(writer);
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  ui_widget_serialize(writer, iter);
  WIDGET_FOR_EACH_CHILD_END();
  ui_builder_on_widget_end(writer);

  return RET_OK;
}

ret_t widget_to_xml(widget_t* widget, str_t* str) {
  ui_xml_writer_t ui_xml_writer;
  return_value_if_fail(widget != NULL && str != NULL, RET_BAD_PARAMS);

  return ui_widget_serialize(ui_xml_writer_init(&ui_xml_writer, str), widget);
}
