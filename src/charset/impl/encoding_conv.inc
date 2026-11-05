#include "wchar.inc"
#include "conv_common.inc"
#include "Unicode/gbk_to_utf8.map"
#include "Unicode/utf8_to_gbk.map"

static void gbk_to_utf8(unsigned char* src, int src_size, unsigned char* dest, int dest_size) {
  int len = src_size;
  LocalToUtf(src, len, dest, LUmapGBK, lengthof(LUmapGBK), NULL, 0, NULL, PG_GBK);

  return;
}

static void utf8_to_gbk(unsigned char* src, int src_size, unsigned char* dest, int dest_size) {
  int len = src_size;
  UtfToLocal(src, len, dest, ULmapGBK, lengthof(ULmapGBK), NULL, 0, NULL, PG_GBK);

  return;
}

static ret_t encoding_convert_from_utf8(const char* from_str, uint32_t from_size,
                                   encoding_name_t to, char* to_str, uint32_t to_size) {
  if(to == ENCODING_GBK) {
    utf8_to_gbk((unsigned char*)from_str, from_size, (unsigned char*)to_str, to_size);
    return RET_OK;
  } else {
    return RET_NOT_IMPL;
  }
}

static ret_t encoding_convert_to_utf8(encoding_name_t from, const char* from_str, uint32_t from_size,
                                  char* to_str, uint32_t to_size) {
  if(from == ENCODING_GBK) {
    gbk_to_utf8((unsigned char*)from_str, from_size, (unsigned char*)to_str, to_size);
    return RET_OK;
  } else {
    return RET_NOT_IMPL;
  }
}

static ret_t encoding_convert_impl(encoding_name_t from, const char* from_str, uint32_t from_size,
                                   encoding_name_t to, char* to_str, uint32_t to_size) {
  uint32_t max_from_size = ((to_size - 2) * 2)/3;
  from_size = tk_min(max_from_size, from_size);
  from_size = tk_max(0, (int)from_size);

  if(from == ENCODING_UTF8) {
    return encoding_convert_from_utf8(from_str, from_size, to, to_str, to_size);
  } else if(to == ENCODING_UTF8) {
    return encoding_convert_to_utf8(from, from_str, from_size, to_str, to_size); 
  } else {
    return RET_NOT_IMPL;
  }
}
