/**
 * File:   utf8.c
 * Author: AWTK Develop Team
 * Brief:  utf8 encode decode
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include <string.h>

// 获取UTF-8编码的首字节所表示的字符长度
static int utf8_get_char_len(unsigned char c) {
  if (c < 0x80) return 1; // 单字节ASCII
  if ((c & 0xE0) == 0xC0) return 2; // 2字节序列
  if ((c & 0xF0) == 0xE0) return 3; // 3字节序列
  if ((c & 0xF8) == 0xF0) return 4; // 4字节序列
  if ((c & 0xFC) == 0xF8) return 5; // 5字节序列（历史/扩展）
  if ((c & 0xFE) == 0xFC) return 6; // 6字节序列（历史/扩展）
  return -1; // 非法首字节
}

// 获取UTF-8首字节的字节数
uint32_t tk_utf8_get_bytes_of_leading(uint8_t c) {
  return (uint32_t)utf8_get_char_len(c);
}

// 解析UTF-8编码，获取下一个Unicode码点
static int32_t utf8_get_char(const char* p, const char** next) {
  unsigned char c = (unsigned char)*p;
  int len = utf8_get_char_len(c);
  if (len == -1) return -1;
  // 取出首字节的有效位
  int32_t result = c & ((1 << (8 - len)) - 1);
  // 依次拼接后续字节的低6位
  for (int i = 1; i < len; ++i) {
    if ((p[i] & 0xC0) != 0x80) return -1; // 非法续字节
    result = (result << 6) | (p[i] & 0x3F);
  }
  if (next) *next = p + len;
  return result;
}

// 将单个Unicode码点编码为UTF-8字节序列
static int unichar_to_utf8(uint32_t c, char* outbuf) {
  int len = 0;
  // 判断需要多少字节表示
  if (c < 0x80) len = 1;
  else if (c < 0x800) len = 2;
  else if (c < 0x10000) len = 3;
  else if (c < 0x200000) len = 4;
  else if (c < 0x4000000) len = 5;
  else len = 6;
  if (outbuf) {
    // 低位字节，倒序填充
    for (int i = len - 1; i > 0; --i) {
      outbuf[i] = (c & 0x3F) | 0x80;
      c >>= 6;
    }
    // 首字节填充
    static const unsigned char first_byte_mark[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};
    outbuf[0] = c | first_byte_mark[len];
  }
  return len;
}

// UTF-16转UTF-8，支持代理对，自动处理wchar_t宽度
char* tk_utf8_from_utf16_ex(const wchar_t* in, uint32_t in_size, char* out, uint32_t out_size) {
  if (!in || !out || out_size == 0) return NULL;
  uint32_t i = 0, j = 0;
  while (i < in_size && in[i] != 0) {
    uint32_t wc = (uint32_t)in[i];
    // 处理UTF-16代理对（仅wchar_t为2字节时）
    if (sizeof(wchar_t) == 2 && wc >= 0xD800 && wc <= 0xDBFF && (i + 1) < in_size) {
      uint32_t wc2 = (uint32_t)in[i + 1];
      if (wc2 >= 0xDC00 && wc2 <= 0xDFFF) {
        wc = 0x10000 + (((wc - 0xD800) << 10) | (wc2 - 0xDC00));
        i++;
      }
    }
    char tmp[8] = {0};
    int len = unichar_to_utf8(wc, tmp);
    if (j + len >= out_size) break; // 缓冲区不足
    memcpy(out + j, tmp, len);
    j += len;
    i++;
  }
  if (j < out_size) out[j] = '\0';
  else out[out_size - 1] = '\0';
  if (i < in_size && in[i] != 0) return NULL; // 未全部转换
  return out;
}

// UTF-16转UTF-8（自动计算输入长度）
char* tk_utf8_from_utf16(const wchar_t* str, char* out, uint32_t out_size) {
  if (!str || !out || out_size == 0) return NULL;
  return tk_utf8_from_utf16_ex(str, (uint32_t)wcslen(str), out, out_size);
}

// UTF-8转UTF-16（自动计算输入长度）
wchar_t* tk_utf8_to_utf16(const char* str, wchar_t* out, uint32_t out_size) {
  if (!str || !out || out_size == 0) return NULL;
  return tk_utf8_to_utf16_ex(str, (uint32_t)strlen(str), out, out_size);
}

// UTF-8转UTF-16，支持wchar_t为2字节和4字节
wchar_t* tk_utf8_to_utf16_ex(const char* str, uint32_t size, wchar_t* out, uint32_t out_size) {
  if (!str || !out || out_size == 0) return NULL;
  uint32_t i = 0;
  const char* p = str;
  const char* end = str + size;
  while (p < end && (i + 1) < out_size) {
    int32_t val = utf8_get_char(p, &p);
    if (val == -1) return NULL; // 非法UTF-8
    if (sizeof(wchar_t) == 4) {
      out[i++] = (wchar_t)val;
    } else if (sizeof(wchar_t) == 2) {
      if (val < 0x10000) {
        out[i++] = (wchar_t)val;
      } else {
        val -= 0x10000;
        out[i++] = (wchar_t)(0xD800 + (val >> 10)); // 高代理项
        if ((i + 1) < out_size)
          out[i++] = (wchar_t)(0xDC00 + (val & 0x3FF)); // 低代理项
        else
          return NULL;
      }
    }
  }
  out[i] = 0;
  return out;
}

// 复制UTF-16字符串并转为UTF-8
char* tk_utf8_dup_utf16(const wchar_t* in, int32_t size) {
  if (!in) return NULL;
  if (size < 0) size = (int32_t)wcslen(in);
  uint32_t out_size = size * 4 + 2;
  char* out = (char*)TKMEM_ALLOC(out_size);
  if (!out) return NULL;
  memset(out, 0, out_size);
  return tk_utf8_from_utf16_ex(in, size, out, out_size - 1);
}

// 截断不完整的UTF-8字符，保证字符串结尾合法
char* tk_utf8_trim_invalid_char(char* str) {
  if (!str) return str;
  char* p = str;
  while (*p) {
    uint32_t n = tk_utf8_get_bytes_of_leading((uint8_t)*p);
    for (uint32_t i = 1; i < n; ++i) {
      if ((p[i] & 0xC0) != 0x80 || p[i] == '\0') {
        *p = '\0'; // 截断到此处
        return str;
      }
    }
    p += n;
  }
  return str;
}
