/**
 * File:   file_browser_view.c
 * Author: AWTK Develop Team
 * Brief:  file manager/browser/choosor
 *
 * Copyright (c) 2020 - 2020 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "file_browser_view.h"
#include "base/date_time_format.h"

#define SORT_BY_NAME "name"
#define SORT_BY_TYPE "type"
#define SORT_BY_SIZE "size"
#define SORT_BY_MTIME "mtime"

#ifndef FB_DATE_TIME_FORMAT
#define FB_DATE_TIME_FORMAT "YY-MM-DD hh:mm:ss"
#endif /*FB_DATE_TIME_FORMAT*/

ret_t file_browser_view_reload(widget_t* widget);

ret_t file_browser_view_set_init_dir(widget_t* widget, const char* init_dir) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL, RET_BAD_PARAMS);

  file_browser_view->init_dir = tk_str_copy(file_browser_view->init_dir, init_dir);
  file_browser_set_cwd(file_browser_view->fb, init_dir);

  return RET_OK;
}

ret_t file_browser_view_set_filter(widget_t* widget, const char* filter) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL && filter != NULL, RET_BAD_PARAMS);

  file_browser_view->filter = tk_str_copy(file_browser_view->filter, filter);

  if (tk_str_eq(filter, STR_FILTER_FILES_ONLY)) {
    file_browser_set_filter(file_browser_view->fb, fb_filter_files_only, NULL);
  } else if (tk_str_eq(filter, STR_FILTER_DIR_ONLY)) {
    file_browser_set_filter(file_browser_view->fb, fb_filter_directories_only, NULL);
  } else if (filter != NULL) {
    file_browser_set_filter(file_browser_view->fb, fb_filter_by_ext_names,
                            file_browser_view->filter);
  } else {
    file_browser_set_filter(file_browser_view->fb, NULL, NULL);
  }

  return RET_OK;
}

static ret_t file_browser_view_sync_sort(widget_t* widget) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  const char* sort_by = file_browser_view->sort_by;
  bool_t sort_ascending = file_browser_view->sort_ascending;

  if (sort_by != NULL) {
    if (tk_str_eq(sort_by, SORT_BY_NAME)) {
      file_browser_sort_by_name(file_browser_view->fb, sort_ascending);
    } else if (tk_str_eq(sort_by, SORT_BY_SIZE)) {
      file_browser_sort_by_name(file_browser_view->fb, sort_ascending);
    } else if (tk_str_eq(sort_by, SORT_BY_MTIME)) {
      file_browser_sort_by_mtime(file_browser_view->fb, sort_ascending);
    } else if (tk_str_eq(sort_by, SORT_BY_TYPE)) {
      file_browser_sort_by_type(file_browser_view->fb, sort_ascending);
    }
  }

  return RET_OK;
}

ret_t file_browser_view_set_ignore_hidden_files(widget_t* widget, bool_t ignore_hidden_files) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL, RET_BAD_PARAMS);

  file_browser_view->ignore_hidden_files = ignore_hidden_files;
  file_browser_set_ignore_hidden_files(file_browser_view->fb, ignore_hidden_files);

  return RET_OK;
}

ret_t file_browser_view_set_sort_ascending(widget_t* widget, bool_t sort_ascending) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL, RET_BAD_PARAMS);

  file_browser_view->sort_ascending = sort_ascending;
  file_browser_view_sync_sort(widget);

  return RET_OK;
}

ret_t file_browser_view_set_show_check_button(widget_t* widget, bool_t show_check_button) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL, RET_BAD_PARAMS);

  file_browser_view->show_check_button = show_check_button;
  file_browser_view_reload(widget);

  return RET_OK;
}

ret_t file_browser_view_set_sort_by(widget_t* widget, const char* sort_by) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL, RET_BAD_PARAMS);

  file_browser_view->sort_by = tk_str_copy(file_browser_view->sort_by, sort_by);
  file_browser_view_sync_sort(widget);

  return RET_OK;
}

static ret_t file_browser_view_get_prop(widget_t* widget, const char* name, value_t* v) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(FILE_BROWSER_VIEW_PROP_INIT_DIR, name)) {
    value_set_str(v, file_browser_view->init_dir);
    return RET_OK;
  } else if (tk_str_eq(FILE_BROWSER_VIEW_PROP_IGNORE_HIDDEN_FILES, name)) {
    file_browser_view_set_ignore_hidden_files(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(FILE_BROWSER_VIEW_PROP_SORT_ASCENDING, name)) {
    file_browser_view_set_sort_ascending(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(FILE_BROWSER_VIEW_PROP_SHOW_CHECK_BUTTON, name)) {
    file_browser_view_set_show_check_button(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(FILE_BROWSER_VIEW_PROP_SORT_BY, name)) {
    value_set_str(v, file_browser_view->sort_by);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t file_browser_view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(FILE_BROWSER_VIEW_PROP_INIT_DIR, name)) {
    file_browser_view_set_init_dir(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(FILE_BROWSER_VIEW_PROP_IGNORE_HIDDEN_FILES, name)) {
    file_browser_view_set_ignore_hidden_files(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(FILE_BROWSER_VIEW_PROP_SORT_ASCENDING, name)) {
    file_browser_view_set_sort_ascending(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(FILE_BROWSER_VIEW_PROP_SHOW_CHECK_BUTTON, name)) {
    file_browser_view_set_show_check_button(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(FILE_BROWSER_VIEW_PROP_SORT_BY, name)) {
    file_browser_view_set_sort_by(widget, value_str(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t file_browser_view_on_destroy(widget_t* widget) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(widget != NULL && file_browser_view != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(file_browser_view->filter);
  TKMEM_FREE(file_browser_view->sort_by);
  TKMEM_FREE(file_browser_view->init_dir);
  file_browser_destroy(file_browser_view->fb);

  widget_destroy(file_browser_view->file_template);
  widget_destroy(file_browser_view->folder_template);
  darray_deinit(&(file_browser_view->selected_items));
  darray_deinit(&(file_browser_view->file_items_cache));
  darray_deinit(&(file_browser_view->folder_items_cache));

  return RET_OK;
}

static ret_t file_browser_view_on_paint_self(widget_t* widget, canvas_t* c) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);

  (void)file_browser_view;

  return RET_OK;
}

static ret_t file_browser_view_reload_in_idle(const idle_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);

  file_browser_view_reload(widget);

  return RET_REMOVE;
}

static ret_t file_browser_view_on_item_clicked(void* ctx, event_t* e) {
  widget_t* target = WIDGET(e->target);
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(ctx);

  if (tk_str_eq(target->name, FILE_BROWSER_VIEW_RETURN_UP)) {
    file_browser_up(file_browser_view->fb);
    idle_add(file_browser_view_reload_in_idle, file_browser_view);
  } else if (tk_str_eq(target->name, FILE_BROWSER_VIEW_FOLDER)) {
    uint32_t index = widget_index_of(target);
    fb_item_t* info = file_browser_get_item(file_browser_view->fb, index - 1);

    file_browser_enter(file_browser_view->fb, info->name);
    idle_add(file_browser_view_reload_in_idle, file_browser_view);
  } else {
    uint32_t index = widget_index_of(target);
    fb_item_t* info = file_browser_get_item(file_browser_view->fb, index - 1);

    if (file_browser_view->selected_file != NULL) {
      widget_set_text_utf8(file_browser_view->selected_file, info->name);
    }
  }

  widget_dispatch_simple_event(WIDGET(ctx), EVT_VALUE_CHANGED);

  return RET_OK;
}

static ret_t file_browser_view_recycle_items(widget_t* widget) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  widget_t* container = file_browser_view->container;
  darray_t* file_items_cache = &(file_browser_view->file_items_cache);
  darray_t* folder_items_cache = &(file_browser_view->folder_items_cache);

  if (container->children != NULL) {
    WIDGET_FOR_EACH_CHILD_BEGIN(container, iter, i)
    widget_remove_child_prepare(container, iter);
    if (tk_str_eq(iter->name, FILE_BROWSER_VIEW_FILE)) {
      darray_push(file_items_cache, iter);
    } else if (tk_str_eq(iter->name, FILE_BROWSER_VIEW_FOLDER)) {
      darray_push(folder_items_cache, iter);
    }
    container->children->elms[i] = NULL;
    WIDGET_FOR_EACH_CHILD_END();
    container->children->size = 0;
  }

  return RET_OK;
}

static widget_t* file_browser_view_create_file_item(widget_t* widget) {
  widget_t* item = NULL;
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  widget_t* container = file_browser_view->container;
  darray_t* cache = &(file_browser_view->file_items_cache);

  if (cache->size > 0) {
    item = WIDGET(darray_pop(cache));
    widget_add_child(container, item);
  } else {
    item = widget_clone(file_browser_view->file_template, container);
    widget_on(item, EVT_CLICK, file_browser_view_on_item_clicked, widget);
  }

  return item;
}

static widget_t* file_browser_view_create_folder_item(widget_t* widget) {
  widget_t* item = NULL;
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  widget_t* container = file_browser_view->container;
  darray_t* cache = &(file_browser_view->folder_items_cache);

  if (cache->size > 0) {
    item = WIDGET(darray_pop(cache));
    widget_add_child(container, item);
  } else {
    item = widget_clone(file_browser_view->folder_template, container);
    widget_on(item, EVT_CLICK, file_browser_view_on_item_clicked, widget);
  }
  return item;
}

ret_t file_browser_view_reload(widget_t* widget) {
  uint32_t i = 0;
  uint32_t nr = 0;
  widget_t* item = NULL;
  widget_t* item_child = NULL;
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  widget_t* container = file_browser_view->container;
  return_value_if_fail(container != NULL, RET_BAD_PARAMS);

  file_browser_view_recycle_items(widget);

  if (file_browser_view->cwd != NULL) {
    widget_set_text_utf8(file_browser_view->cwd, file_browser_view->fb->cwd);
  }

  item = file_browser_view->return_up_template;
  widget_add_child(container, item);

  nr = file_browser_get_items_nr(file_browser_view->fb);
  for (i = 0; i < nr; i++) {
    fb_item_t* info = file_browser_get_item(file_browser_view->fb, i);
    if (info->is_dir) {
      item = file_browser_view_create_folder_item(widget);
    } else {
      value_t v;
      char prop[TK_NAME_LEN + 1];
      const char* ext = strrchr(info->name, '.');
      item = file_browser_view_create_file_item(widget);
      item_child = widget_lookup(item, FILE_BROWSER_VIEW_ICON, TRUE);

      if (item_child != NULL && ext != NULL) {
        ext++;
        tk_snprintf(prop, sizeof(prop), "icon_%s", ext);

        if (widget_get_prop(widget, prop, &v) == RET_OK && value_str(&v) != NULL) {
          widget_set_prop_str(item_child, WIDGET_PROP_IMAGE, value_str(&v));
        }
      }
    }

    item_child = widget_lookup(item, FILE_BROWSER_VIEW_NAME, TRUE);
    if (item_child != NULL) {
      widget_set_text_utf8(item_child, info->name);
    }

    item_child = widget_lookup(item, FILE_BROWSER_VIEW_SIZE, TRUE);
    if (item_child != NULL) {
      wstr_t* str = &(item_child->text);
      wstr_from_int(str, info->size);
    }

    item_child = widget_lookup(item, FILE_BROWSER_VIEW_MTIME, TRUE);
    if (item_child != NULL) {
      wstr_t* str = &(item_child->text);
      wstr_format_time(str, FB_DATE_TIME_FORMAT, info->mtime);
    }

    item_child = widget_lookup(item, FILE_BROWSER_VIEW_CTIME, TRUE);
    if (item_child != NULL) {
      wstr_t* str = &(item_child->text);
      wstr_format_time(str, FB_DATE_TIME_FORMAT, info->ctime);
    }

    item_child = widget_lookup_by_type(item, WIDGET_TYPE_CHECK_BUTTON, TRUE);
    if (item_child != NULL) {
      widget_set_visible(item_child, file_browser_view->show_check_button, FALSE);
    }
  }

  widget_layout(widget);

  return RET_OK;
}

static ret_t file_browser_view_init_ui(widget_t* widget) {
  widget_t* template = NULL;
  widget_t* container = NULL;
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);

  container = widget_lookup(widget, FILE_BROWSER_VIEW_CONTAINER, TRUE);
  return_value_if_fail(container != NULL, RET_BAD_PARAMS);
  file_browser_view->container = container;
  file_browser_view->cwd = widget_lookup(widget, FILE_BROWSER_VIEW_CWD, TRUE);
  file_browser_view->selected_file = widget_lookup(widget, FILE_BROWSER_VIEW_SELECTED_FILE, TRUE);

  template = widget_lookup(container, FILE_BROWSER_VIEW_FILE, TRUE);
  return_value_if_fail(template != NULL, RET_BAD_PARAMS);
  file_browser_view->file_template = template;
  widget_remove_child(template->parent, template);

  template = widget_lookup(container, FILE_BROWSER_VIEW_FOLDER, TRUE);
  return_value_if_fail(template != NULL, RET_BAD_PARAMS);
  file_browser_view->folder_template = template;
  widget_remove_child(template->parent, template);

  template = widget_lookup(container, FILE_BROWSER_VIEW_RETURN_UP, TRUE);
  return_value_if_fail(template != NULL, RET_BAD_PARAMS);
  file_browser_view->return_up_template = template;
  widget_remove_child(template->parent, template);
  widget_on(template, EVT_CLICK, file_browser_view_on_item_clicked, widget);
  file_browser_view->inited = TRUE;

  return RET_OK;
}

static ret_t file_browser_view_on_event(widget_t* widget, event_t* e) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(widget != NULL && file_browser_view != NULL, RET_BAD_PARAMS);

  switch (e->type) {
    case EVT_WIDGET_LOAD: {
      file_browser_view_init_ui(widget);
      break;
    }
  }

  return RET_OK;
}

const char* s_file_browser_view_properties[] = {
    FILE_BROWSER_VIEW_PROP_SORT_BY, FILE_BROWSER_VIEW_PROP_INIT_DIR,
    FILE_BROWSER_VIEW_PROP_SORT_ASCENDING, FILE_BROWSER_VIEW_PROP_IGNORE_HIDDEN_FILES, NULL};

TK_DECL_VTABLE(file_browser_view) = {.size = sizeof(file_browser_view_t),
                                     .type = WIDGET_TYPE_FILE_BROWSER_VIEW,
                                     .clone_properties = s_file_browser_view_properties,
                                     .persistent_properties = s_file_browser_view_properties,
                                     .parent = TK_PARENT_VTABLE(widget),
                                     .create = file_browser_view_create,
                                     .on_paint_self = file_browser_view_on_paint_self,
                                     .set_prop = file_browser_view_set_prop,
                                     .get_prop = file_browser_view_get_prop,
                                     .on_event = file_browser_view_on_event,
                                     .on_destroy = file_browser_view_on_destroy};

widget_t* file_browser_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(file_browser_view), x, y, w, h);
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL, NULL);

  file_browser_view->sort_ascending = TRUE;
  file_browser_view->show_check_button = FALSE;
  file_browser_view->ignore_hidden_files = TRUE;
  file_browser_view->fb = file_browser_create(os_fs());
  darray_init(&(file_browser_view->selected_items), 10, NULL, NULL);
  darray_init(&(file_browser_view->file_items_cache), 10, (tk_destroy_t)widget_unref, NULL);
  darray_init(&(file_browser_view->folder_items_cache), 10, (tk_destroy_t)widget_unref, NULL);

  return widget;
}

widget_t* file_browser_view_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, file_browser_view), NULL);

  return widget;
}

#include "base/widget_factory.h"

ret_t file_browser_view_register(void) {
  return widget_factory_register(widget_factory(), WIDGET_TYPE_FILE_BROWSER_VIEW,
                                 file_browser_view_create);
}

darray_t* file_browser_view_get_selected_items(widget_t* widget) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  widget_t* container = file_browser_view->container;
  darray_t* arr = &(file_browser_view->selected_items);

  darray_clear(arr);

  if (container->children != NULL) {
    WIDGET_FOR_EACH_CHILD_BEGIN(container, iter, i)
    if (i > 0) {
      widget_t* checkable = widget_lookup_by_type(iter, WIDGET_TYPE_CHECK_BUTTON, TRUE);

      if (checkable != NULL && widget_get_value(checkable)) {
        fb_item_t* info = file_browser_get_item(file_browser_view->fb, i - 1);
        darray_push(arr, info);
      }
    }
    WIDGET_FOR_EACH_CHILD_END();
  }

  return arr;
}

const char* file_browser_view_get_cwd(widget_t* widget) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL, NULL);

  return file_browser_view->fb->cwd;
}

ret_t file_browser_view_create_dir(widget_t* widget, const char* name) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL, RET_BAD_PARAMS);

  return file_browser_create_dir(file_browser_view->fb, name);
}

ret_t file_browser_view_create_file(widget_t* widget, const char* name, const char* data,
                                    uint32_t size) {
  file_browser_view_t* file_browser_view = FILE_BROWSER_VIEW(widget);
  return_value_if_fail(file_browser_view != NULL, RET_BAD_PARAMS);

  return file_browser_create_file(file_browser_view->fb, name, data, size);
}
