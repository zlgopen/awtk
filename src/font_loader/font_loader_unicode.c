/**
 * File:   font_loader_unicode.c
 * Author: AWTK Develop Team
 * Brief:  font loader unicode helpers
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
 * 2026-10-30 Yang Zewu <yangzewu@zlg.cn> created
 *
 */

#include "font_loader/font_loader_unicode.h"

uint32_t font_loader_get_codepoint(const wchar_t* str, uint32_t size, uint32_t* index,
                                   uint32_t* code_unit_count) {
  uint32_t c = 0;
  return_value_if_fail(str != NULL && index != NULL && *index < size && code_unit_count != NULL, 0);

  // 先取当前位置的码元，UTF-32 或 BMP 字符可以直接作为 Unicode 标量值使用。
  *code_unit_count = 1;
  c = (uint32_t)str[*index];

  // 只有两字节 wchar_t 才可能出现代理对。
  // RTL 重排后可能先定位到低代理，需要回退到高代理，保证 index 指向码点起始位置。
  if (sizeof(wchar_t) == 2 && c >= 0xDC00 && c <= 0xDFFF && *index > 0) {
    // 取前一个码元，判断它是否是当前低代理对应的高代理。
    uint32_t prev = (uint32_t)str[*index - 1];
    if (prev >= 0xD800 && prev <= 0xDBFF) {
      // 回退到高代理位置，后续统一从高代理开始合成完整码点。
      (*index)--;
      c = prev;
    }
  }

  // 两字节 wchar_t 平台下，非 BMP 字符由高代理和低代理两个码元组成。
  // 这里把一对代理转换成一个 Unicode 标量值，避免上层按半个字符处理。
  if (sizeof(wchar_t) == 2 && c >= 0xD800 && c <= 0xDBFF && *index + 1 < size) {
    // 读取后一个码元，确认它是合法的低代理。
    uint32_t next = (uint32_t)str[*index + 1];
    if (next >= 0xDC00 && next <= 0xDFFF) {
      // Unicode 标准的代理对合成公式，结果范围从 U+10000 开始。
      c = 0x10000 + ((c - 0xD800) << 10) + (next - 0xDC00);
      // 代理对占用两个 UTF-16 码元，调用方据此推进字符串下标。
      *code_unit_count = 2;
    }
  }

  return c;
}

// 统一遍历码点，返回已遍历的码点数量，并通过 str_index 返回对应码元位置。
static uint32_t font_loader_traverse_codepoints(const wchar_t* str, uint32_t size,
                                                uint32_t codepoint_index, uint32_t* str_index) {
  uint32_t index = 0;
  uint32_t count = 0;
  uint32_t code_unit_count = 0;
  return_value_if_fail(str != NULL && str_index != NULL, 0);

  *str_index = 0;
  while (index < size && count < codepoint_index) {
    // 复制当前位置，避免 font_loader_get_codepoint 修改遍历用的 index。
    uint32_t pos = index;
    (void)font_loader_get_codepoint(str, size, &pos, &code_unit_count);
    *str_index = index;
    return_value_if_fail(code_unit_count > 0, count);

    // index 按码元推进，count 按 Unicode 码点计数。
    index += code_unit_count;
    count++;
  }

  *str_index = index;
  return count;
}

uint32_t font_loader_get_str_index_from_codepoint_index(const wchar_t* str, uint32_t size,
                                                        uint32_t codepoint_index) {
  uint32_t index = 0;
  // 只关心目标码点对应的码元位置，遍历数量由内部函数统一维护。
  (void)font_loader_traverse_codepoints(str, size, codepoint_index, &index);
  return index;
}

uint32_t font_loader_get_codepoint_count(const wchar_t* str, uint32_t size) {
  uint32_t index = 0;
  // 使用最大码点序号遍历完整字符串，只取返回的码点数量。
  return font_loader_traverse_codepoints(str, size, UINT32_MAX, &index);
}
