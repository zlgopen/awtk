/**
 * File:   harfbuzz_data_loader.c
 * Author: AWTK Develop Team
 * Brief:  stb truetype font loader
 *
 * Copyright (c) 2018 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-01-20 Zhao Yingjian <zhaoyingjian@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/buffer.h"
#include "tkc/asset_info.h"
#include "base/types_def.h"
#include "base/assets_manager.h"
#include "harfbuzz_data_loader.h"

/* 版本号 */
#define VERSION_MAGIC 0x1

#define BIN_SEPARATOR_LEN 1 // '\0'的长度

#define BIN_LINEBREAK "\n"
#define BIN_SEPARATOR "\0"
#define BIN_EOF_CHECK 0xFEFE
#define BIN_DOUBLE_SEPARATOR 0xFCFC
#define BIN_BEGIN_MAGIC "HARFBUZZ_INFO"
#define BIN_TABLE_INFO_MAGIC "TABLE_INFO"
#define BIN_TABLE_INFO_SIZE_MAGIC "TABLE_INFO_SIZE"

#ifndef DEFAULT_HARFBUZZ_BIN_NAME
#define DEFAULT_HARFBUZZ_BIN_NAME "_s_harfbuzz_data_info.bin"
#endif /* DEFAULT_HARFBUZZ_BIN_NAME */

static harfbuzz_data_loader_t* s_harfbuzz_data_loader = NULL;

static ret_t destroy_hb_loader_from_bin(harfbuzz_data_loader_t* hb_loader) {
  uint32_t i = 0;
  uint32_t j = 0;
  uint32_t ttf_size = 0;
  uint32_t str_size = 0;
  hb_str_info_t* str_info = NULL;
  hb_ttf_info_t* ttf_info = NULL;
  return_value_if_fail(hb_loader != NULL, RET_BAD_PARAMS);

  if (hb_loader->ttf_info != NULL) {
    ttf_size = hb_loader->ttf_size;
    for (i = 0; i < ttf_size; ++i) {
      ttf_info = &hb_loader->ttf_info[i];
      if (ttf_info == NULL) {
        continue;
      }
      if (ttf_info->font_name != NULL) {
        TKMEM_FREE(ttf_info->font_name);
      }
      for (j = 0; j < ttf_info->str_size; ++j) {
        str_info = &ttf_info->str_info[j];
        if (str_info->str == NULL) {
          continue;
        }
        if (str_info->str != NULL) {
          TKMEM_FREE(str_info->str);
        }
  
        if (str_info->hb_info != NULL) {
          TKMEM_FREE(str_info->hb_info);
        }
  
        str_info->hb_info_size = 0;
      }
      ttf_info->str_size = 0;
      TKMEM_FREE(ttf_info->str_info);

    }
    hb_loader->ttf_size = 0;
    TKMEM_FREE(hb_loader->ttf_info);
  }
  TKMEM_FREE(hb_loader);

  return RET_OK;
}

/* 验证二进制文件是否为harfbuzz info表 */
static ret_t check_begin_magic(rbuffer_t* rbuffer) {
  const char* ch = NULL;
  return_value_if_fail(rbuffer != NULL, RET_BAD_PARAMS);
  if (rbuffer_has_more(rbuffer)) {
    rbuffer_read_string(rbuffer, &ch);
  }

  if (ch != NULL && tk_str_eq(ch, BIN_BEGIN_MAGIC)) {
    return RET_OK;
  }

  return RET_FAIL;
}

/* 验证版本号是否一致 */
static ret_t check_version_magic(rbuffer_t* rbuffer) {
  int32_t version = 0;
  return_value_if_fail(rbuffer != NULL, RET_BAD_PARAMS);
  if (rbuffer_has_more(rbuffer)) {
    rbuffer_read_int32(rbuffer, &version);
  }

  if (version == VERSION_MAGIC) {
    return RET_OK;
  }

  return RET_FAIL;
}

/* 获取信息表的字符串数量 */
static int32_t read_table_info_size(rbuffer_t* rbuffer) {
  const char* ch = NULL;
  int32_t table_info_size = 0;
  return_value_if_fail(rbuffer != NULL, -1);
  if (rbuffer_has_more(rbuffer)) {
    rbuffer_read_string(rbuffer, &ch);
  }

  if (ch != NULL && tk_str_eq_with_len(ch, BIN_TABLE_INFO_SIZE_MAGIC, tk_strlen(BIN_TABLE_INFO_SIZE_MAGIC))) {
    rbuffer_read_int32(rbuffer, &table_info_size);
    rbuffer_skip(rbuffer, tk_strlen(BIN_LINEBREAK));
  }

  return table_info_size;
}

/* 读表数据，并且生成harfbuzz_data_loader_t对象, 对象需要手动销毁 */
static harfbuzz_data_loader_t* gen_harfbuzz_data_loader(rbuffer_t* rbuffer, int32_t table_size) {
  int32_t i = 0;
  int32_t j = 0;
  int32_t g = 0;
  int32_t gid = 0;
  int32_t cluster = 0;
  uint16_t check = 0;
  int32_t xoffset = 0;
  int32_t yoffset = 0;
  int32_t xadvance = 0;
  int32_t yadvance = 0;
  int32_t bidi_type = 0;
  uint32_t str_len = 0;
  wchar_t* wstr = NULL;
  const char* ch = NULL;
  const char* font_name = NULL;
  uint32_t str_size = 0;
  uint32_t glyph_size = 0;
  hb_str_info_t* str_info = NULL;
  hb_ttf_info_t* ttf_info = NULL;
  harfbuzz_data_loader_t* hb_loader = NULL;
  return_value_if_fail(rbuffer != NULL && table_size > 0, NULL);

  rbuffer_read_string(rbuffer, &ch);
  if (!tk_str_eq_with_len(ch, BIN_TABLE_INFO_MAGIC, tk_strlen(BIN_TABLE_INFO_MAGIC))) {
    return NULL;
  }
  rbuffer_skip(rbuffer, tk_strlen(BIN_LINEBREAK));

  hb_loader = TKMEM_CALLOC(1, sizeof(harfbuzz_data_loader_t));
  ttf_info = TKMEM_CALLOC(table_size, sizeof(hb_ttf_info_t));

  hb_loader->ttf_info = ttf_info;
  hb_loader->ttf_size = table_size;

  /* 遍历字符串 */
  for (i = 0; i < table_size; ++i) {
    rbuffer_read_string(rbuffer, &font_name);
    if (font_name == NULL) {
      log_debug("gen table info error\n");
      destroy_hb_loader_from_bin(hb_loader);
      break;
    }
    rbuffer_read_uint32(rbuffer, &str_size);
    rbuffer_skip(rbuffer, BIN_SEPARATOR_LEN);
    ttf_info[i].font_name = tk_strdup(font_name);
    str_info = TKMEM_CALLOC(str_size, sizeof(hb_str_info_t));
    ttf_info[i].str_info = str_info;
    ttf_info[i].str_size = str_size;
    
    for (j = 0; j < str_size; ++j) {
      rbuffer_read_string(rbuffer, &ch);
      if (ch == NULL) {
        log_debug("gen table info error\n");
        destroy_hb_loader_from_bin(hb_loader);
        break;
      }
      /* 转宽字符 */
      wstr = tk_wstr_dup_utf8(ch);

      /* 验证0xFCFC */
      rbuffer_read_uint16(rbuffer, &check);
      if (check != BIN_DOUBLE_SEPARATOR) {
        log_debug("gen table info error\n");
        destroy_hb_loader_from_bin(hb_loader);
        break;
      }

      /* 获取当前字符串的字符位置信息数量 */
      rbuffer_read_uint32(rbuffer, &glyph_size);
      rbuffer_skip(rbuffer, BIN_SEPARATOR_LEN);
      rbuffer_read_uint32(rbuffer, &str_len);
      rbuffer_skip(rbuffer, BIN_SEPARATOR_LEN);

      /* 赋值到str_info并申请内存 */
      str_info[j].str = wstr;
      str_info[j].str_len = str_len;
      str_info[j].hb_info_size = glyph_size;
      if (glyph_size > 0) {
        str_info[j].hb_info = TKMEM_CALLOC(glyph_size, sizeof(hb_info_t));
      }

      /* 遍历当前字符串的字符位置信息 */
      for (g = 0; g < glyph_size; ++g) {
        rbuffer_read_int32(rbuffer, &gid);
        rbuffer_read_int32(rbuffer, &cluster);
        rbuffer_read_int32(rbuffer, &xoffset);
        rbuffer_read_int32(rbuffer, &yoffset);
        rbuffer_read_int32(rbuffer, &xadvance);
        rbuffer_read_int32(rbuffer, &yadvance);
        rbuffer_read_int32(rbuffer, &bidi_type);
        rbuffer_skip(rbuffer, BIN_SEPARATOR_LEN);

        str_info[j].hb_info[g].glyphid = gid;
        str_info[j].hb_info[g].cluster = cluster;
        str_info[j].hb_info[g].x_offset = xoffset;
        str_info[j].hb_info[g].y_offset = yoffset;
        str_info[j].hb_info[g].x_advance = xadvance;
        str_info[j].hb_info[g].y_advance = yadvance;
        str_info[j].hb_info[g].bidi_type = bidi_type;
      }
      rbuffer_skip(rbuffer, tk_strlen(BIN_LINEBREAK));
    }
    rbuffer_skip(rbuffer, tk_strlen(BIN_LINEBREAK));
  }

  rbuffer_read_uint16(rbuffer, &check);
  if (check == BIN_EOF_CHECK && !rbuffer_has_more(rbuffer)) {
    log_debug("Read harfbuzz bin file success.\n");
  } else {
    log_debug("Read harfbuzz bin file error.\n");
    destroy_hb_loader_from_bin(hb_loader);
  }

  return hb_loader;
}

static harfbuzz_data_loader_t* harfbuzz_data_loader_create_from_bin(const char* name) {
  int32_t table_size = 0;
  rbuffer_t* rbuffer = NULL;
  const asset_info_t* bin = NULL;
  harfbuzz_data_loader_t* loader = NULL;
  assets_manager_t* am = assets_manager();
  return_value_if_fail(name != NULL, NULL);

  bin = assets_manager_ref(am, ASSET_TYPE_DATA, name);
  return_value_if_fail(bin != NULL, NULL);

  rbuffer = rbuffer_create(bin->data, bin->size);
  if (rbuffer == NULL) {
    assets_manager_unref(am, bin);
    return NULL;
  }

  if (check_begin_magic(rbuffer) != RET_OK) {
    log_debug("Not support harfbuzz bin.\n");
    assets_manager_unref(am, bin);
    rbuffer_destroy(rbuffer);
    return NULL;
  }

  if (check_version_magic(rbuffer) != RET_OK) {
    log_debug("Not support harfbuzz bin version.\n");
    assets_manager_unref(am, bin);
    rbuffer_destroy(rbuffer);
    return NULL;
  }

  table_size = read_table_info_size(rbuffer);
  if (table_size <= 0) {
    assets_manager_unref(am, bin);
    rbuffer_destroy(rbuffer);
    return NULL;
  }

  loader = gen_harfbuzz_data_loader(rbuffer, table_size);

  assets_manager_unref(am, bin);
  rbuffer_destroy(rbuffer);

  return loader;
}

harfbuzz_data_loader_t* harfbuzz_data_loader(void) {
  return &(*s_harfbuzz_data_loader);
}

harfbuzz_data_loader_t* harfbuzz_data_loader_create(const char* name) {
  if (name == NULL || *name == '\0') {
    name = DEFAULT_HARFBUZZ_BIN_NAME;
  }

  return harfbuzz_data_loader_create_from_bin(name);
}

ret_t harfbuzz_data_loader_set(harfbuzz_data_loader_t* loader) {
  s_harfbuzz_data_loader = loader;

  return RET_OK;
}

hb_str_info_t* harfbuzz_data_loader_find_info(harfbuzz_data_loader_t* loader, const char* font_name, const wchar_t* str, uint32_t str_len) {
  uint32_t i = 0;
  int32_t ret = 0;
  int32_t mid = 0;
  int32_t left = 0;
  int32_t right = 0;
  hb_str_info_t* info = NULL;
  hb_str_info_t* str_info = NULL;
  hb_ttf_info_t* ttf_info = NULL;

  if (loader == NULL || str == NULL) {
    return NULL;
  }

  ttf_info = loader->ttf_info;
  for (i = 0; i < loader->ttf_size; ++i) {
    if (tk_str_eq(ttf_info[i].font_name, font_name)) {
      // 整形数据中的字符串已经按照长度排序，先用二分法匹配字符串长度
      mid = 0;
      left = 0;
      right = ttf_info[i].str_size - 1;
      while (left <= right) {
        mid = (left + right) / 2;
        info = &(ttf_info[i].str_info[mid]);
        if (info != NULL && info->str_len == str_len) {
          // 字符串长度相同情况下，整形数据的字符串按照内容大小排序，再次用二分法匹配字符串内容
          ret = wcsncmp(info->str, str, info->str_len);
          if (ret == 0) {
            return info;
          } else if (ret > 0) {
            right = mid - 1;
          } else {
            left = mid + 1; 
          }
        } else if (info != NULL && info->str_len > str_len) {
          right = mid - 1; 
        } else {
          left = mid + 1; 
        }
      }
    }
  }

  return NULL;
}

ret_t harfbuzz_data_loader_destroy(harfbuzz_data_loader_t* loader) {
  return_value_if_fail(loader != NULL, RET_OK);

  destroy_hb_loader_from_bin(loader);

  return RET_OK;
}

harfbuzz_data_loader_t* harfbuzz_data_loader_ref(harfbuzz_data_loader_t* loader, const char* name) {
  
  if (name == NULL || *name == '\0') {
    name = DEFAULT_HARFBUZZ_BIN_NAME;
  }

  if (loader == NULL) {
    loader = harfbuzz_data_loader_create_from_bin(name);
    if (loader != NULL) {
      loader->refcount = 1;
    }
  } else {
    loader->refcount++;
  }

  return loader;
}

ret_t harfbuzz_data_loader_unref(harfbuzz_data_loader_t* loader) {
  return_value_if_fail(loader != NULL, RET_BAD_PARAMS);

  if (loader->refcount > 1) {
    loader->refcount--;
  } else if (loader->refcount == 1) {
    loader->refcount = 0;
    destroy_hb_loader_from_bin(loader); 
  }

  return RET_OK;
}
