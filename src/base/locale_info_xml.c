/**
 * File:   locale_info_xml.c
 * Author: AWTK Develop Team
 * Brief:  locale_info_xml
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-04-08 Li PeiHuang <lipeihuang@zlg.cn> created
 *
 */

#include "tkc/fs.h"
#include "base/locale_info_xml.h"
#include "base/assets_manager.h"
#include "base/locale_info.h"

static const char* locale_info_xml_get_language_str(locale_info_t* locale_info) {
  locale_info_xml_t* xml_info = (locale_info_xml_t*)locale_info;
  if (xml_info == NULL) {
    return NULL;
  }

  str_clear(xml_info->language);
  str_set(xml_info->language, locale_info->language);
  str_append(xml_info->language, "_");
  str_append(xml_info->language, locale_info->country);

  return xml_info->language->str;
}

static int32_t locale_info_xml_get_node_size(locale_info_xml_t* xml_info, conf_node_t* node) {
  if (node == NULL) {  // node == NULL, 则获取root节点的size
    return conf_doc_get_int(xml_info->doc, "root.#size", -1);
  } else {
    return conf_node_count_children(node);
  }
}

static conf_node_t* locale_info_xml_get_node_by_text(locale_info_t* locale_info, const char* text) {
  value_t v;
  char path[MAX_PATH + 1] = {0};
  const char* name = NULL;
  conf_node_t* name_node = NULL;
  locale_info_xml_t* xml_info = (locale_info_xml_t*)locale_info;
  int32_t size = locale_info_xml_get_node_size(xml_info, NULL);

  for (int32_t i = 0; i < size; i++) {
    memset(path, 0, sizeof(path));
    tk_snprintf(path, sizeof(path), "root.[%d].name", i);

    name_node = conf_doc_find_node(xml_info->doc, NULL, path, FALSE);
    if (name_node == NULL || conf_node_get_value(name_node, &v) != RET_OK) {
      continue;
    }

    name = value_str(&v);
    if (tk_str_eq(name, text)) {
      return name_node->parent;
    }
  }

  return NULL;
}

static conf_node_t* locale_info_xml_get_node_by_language(locale_info_t* locale_info,
                                                         conf_node_t* node, const char* language) {
  value_t v;
  char path[MAX_PATH + 1] = {0};
  const char* name = NULL;
  conf_node_t* name_node = NULL;
  locale_info_xml_t* xml_info = (locale_info_xml_t*)locale_info;
  int32_t size = locale_info_xml_get_node_size(xml_info, node);

  for (int32_t i = 0; i < size; i++) {
    memset(path, 0, sizeof(path));
    tk_snprintf(path, sizeof(path), "[%d].name", i);

    name_node = conf_doc_find_node(xml_info->doc, node, path, FALSE);
    if (name_node == NULL || conf_node_get_value(name_node, &v) != RET_OK) {
      continue;
    }

    name = value_str(&v);
    if (tk_str_eq(name, language)) {
      return name_node->parent;
    }
  }

  return NULL;
}

static const char* locale_info_xml_get_node_text(locale_info_t* locale_info, conf_node_t* node) {
  locale_info_xml_t* xml_info = (locale_info_xml_t*)locale_info;
  conf_node_t* text_node = NULL;

  if (node == NULL) {
    return conf_doc_get_str(xml_info->doc, "root.@text", NULL);
  } else {
    text_node = conf_doc_find_node(xml_info->doc, node, "@text", FALSE);
    if (conf_node_get_value(text_node, xml_info->text_value) == RET_OK) {
      return value_str(xml_info->text_value);
    }
  }

  return NULL;
}

static const char* locale_info_xml_get_tr(void* ctx, const char* text) {
  locale_info_t* info = (locale_info_t*)ctx;
  if (info == NULL) {
    return NULL;
  }

  const char* lang = locale_info_xml_get_language_str(info);
  conf_node_t* t_node = locale_info_xml_get_node_by_text(info, text);
  if (t_node != NULL) {
    conf_node_t* l_node = locale_info_xml_get_node_by_language(info, t_node, lang);
    if (l_node != NULL) {
      return locale_info_xml_get_node_text(info, l_node);
    }
  }

  return NULL;
}

locale_info_t* locale_info_xml_create(const char* language, const char* country) {
  locale_info_t* info = locale_info_create(language, country);
  locale_info_xml_t* xml_info = TKMEM_ZALLOC(locale_info_xml_t);
  memcpy(xml_info, info, sizeof(locale_info_t));
  TKMEM_FREE(info);

  xml_info->doc = NULL;
  xml_info->strings_url = str_create(0);
  xml_info->language = str_create(0);
  xml_info->text_value = value_create();

  info = (locale_info_t*)xml_info;
  info->assets_manager = NULL;
  locale_info_set_fallback_tr2(info, locale_info_xml_get_tr, info);

  return info;
}

static str_t* get_strings_content(const char* path, str_t* result) {
  if (fs_file_exist(os_fs(), path)) {
    uint32_t size;
    char* content = (char*)file_read(path, &size);
    if (content != NULL) {
      str_set(result, "<root>");
      str_append(result, content);
      str_append(result, "</root>");

      TKMEM_FREE(content);
    }
  }

  return result;
}

static ret_t locale_info_xml_reload(locale_info_t* locale_info, const char* url) {
  locale_info_xml_t* xml_info = (locale_info_xml_t*)locale_info;

  assert(xml_info->strings_url != NULL);

  if (xml_info->doc != NULL) {
    conf_doc_destroy(xml_info->doc);
    xml_info->doc = NULL;
  }

  str_set(xml_info->strings_url, url);

  if (TK_STR_IS_NOT_EMPTY(url)) {
    str_t* content = str_create(0);
    content = get_strings_content(url, content);
    xml_info->doc = conf_doc_load_xml(content->str);
    str_destroy(content);
  }

  return RET_OK;
}

ret_t locale_info_xml_set_url(locale_info_t* locale_info, const char* url) {
  return_value_if_fail(locale_info != NULL, RET_BAD_PARAMS);

  return locale_info_xml_reload(locale_info, url);
}

static ret_t locale_info_xml_on_asset_manager_clear_cache(void* ctx, event_t* e) {
  locale_info_t* locale_info = (locale_info_t*)ctx;
  locale_info_xml_t* xml_info = (locale_info_xml_t*)locale_info;
  assets_event_t* evt = (assets_event_t*)e;

  if (evt->type == ASSET_TYPE_STRINGS) {
    return locale_info_xml_reload(locale_info, xml_info->strings_url->str);
  }

  return RET_OK;
}

ret_t locale_info_xml_set_assets_manager(locale_info_t* locale_info, assets_manager_t* am) {
  const char* res_root = am != NULL ? assets_manager_get_res_root(am) : NULL;
  const char* theme = am != NULL ? assets_manager_get_theme_name(am) : NULL;
  return_value_if_fail(locale_info != NULL, RET_BAD_PARAMS);

  if (locale_info->assets_manager != NULL) {
    emitter_off_by_ctx(EMITTER(locale_info->assets_manager), locale_info);
  }

  return_value_if_fail(locale_info_set_assets_manager(locale_info, am) == RET_OK, RET_FAIL);

  if (TK_STR_IS_EMPTY(theme)) {
    theme = "default";
  }

  if (TK_STR_IS_NOT_EMPTY(res_root)) {
    char path[MAX_PATH + 1] = {0};
    tk_snprintf(path, sizeof(path), "%s/%s/strings/strings.xml", res_root, theme);
    if (!fs_file_exist(os_fs(),
                       path)) {  // 找不到指定主题的strings.xml文件，则使用默认主题的strings.xml文件
      memset(path, 0, sizeof(path));
      tk_snprintf(path, sizeof(path), "%s/default/strings/strings.xml", res_root);
    }

    locale_info_xml_reload(locale_info, path);
  } else {
    locale_info_xml_reload(locale_info, NULL);
  }

  if (am != NULL) {
    emitter_on(EMITTER(am), EVT_ASSET_MANAGER_CLEAR_CACHE,
               locale_info_xml_on_asset_manager_clear_cache, locale_info);
  }

  return RET_OK;
}

ret_t locale_info_xml_destroy(locale_info_t* locale_info) {
  locale_info_xml_t* xml_info = (locale_info_xml_t*)locale_info;

  if (xml_info != NULL) {
    if (locale_info->assets_manager != NULL) {
      emitter_off_by_ctx(EMITTER(locale_info->assets_manager), locale_info);
    }

    if (xml_info->doc != NULL) {
      conf_doc_destroy(xml_info->doc);
      xml_info->doc = NULL;
    }

    if (xml_info->strings_url != NULL) {
      str_destroy(xml_info->strings_url);
      xml_info->strings_url = NULL;
    }

    if (xml_info->language != NULL) {
      str_destroy(xml_info->language);
      xml_info->language = NULL;
    }

    if (xml_info->text_value != NULL) {
      value_destroy(xml_info->text_value);
      xml_info->text_value = NULL;
    }

    locale_info_destroy(locale_info);
  }

  return RET_OK;
}