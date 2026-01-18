/**
 * File:   data_reader.c
 * Author: AWTK Develop Team
 * Brief:  data_reader
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License http for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2023-12-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/url.h"
#include "tkc/utils.h"
#include "tkc/buffer.h"
#include "tkc/data_reader_http.h"
#include "tkc/log.h"
#include "streams/stream_factory.h"

typedef struct _data_reader_http_t {
  data_reader_t data_reader;

  uint64_t size;
  uint8_t* data;
} data_reader_http_t;

static const char* url_get_path(const char* url) {
  const char* p = NULL;
  return_value_if_fail(url != NULL, NULL);

  p = strstr(url, "://");
  if (p != NULL) {
    p += 3;
    p = strchr(p, '/');
    if (p == NULL) {
      p = "/";
    }
  }

  return p;
}

static uint8_t* data_reader_http_get_chunked_data(uint8_t* data, uint32_t* size) {
  uint8_t* p = data;
  uint8_t* q = data;
  uint32_t chunk_size = 0;

  return_value_if_fail(data != NULL && size != NULL, NULL);

  while (TRUE) {
    if (p[0] == '\r' && p[1] == '\n') {
      break;
    }
    p++;
  }

  chunk_size = tk_strtol((const char*)q, NULL, 16);
  p += 2;

  if (chunk_size == 0) {
    *size = 0;
    return NULL;
  }

  *size = chunk_size;
  return p;
}

static ret_t data_reader_http_decode_chuncked_data(data_reader_http_t* http, wbuffer_t* wb) {
  uint32_t size = 0;
  uint32_t total_size = 0;
  uint8_t* src = NULL;
  uint8_t* dst = NULL;
  uint8_t* dst_end = NULL;

  return_value_if_fail(http != NULL && wb != NULL, RET_BAD_PARAMS);
  http->data = TKMEM_ALLOC(wb->cursor + 1);
  return_value_if_fail(http->data != NULL, RET_OOM);

  src = wb->data;
  dst = http->data;
  dst_end = http->data + wb->cursor;
  
  while ((src = data_reader_http_get_chunked_data(src, &size)) != NULL) {
    /* 检查边界：确保不会超出分配的内存 */
    if (dst + size > dst_end) {
      log_warn("data_reader_http_decode_chuncked_data: chunk size exceeds buffer\n");
      TKMEM_FREE(http->data);
      http->data = NULL;
      return RET_FAIL;
    }
    
    memcpy(dst, src, size);
    dst += size;
    total_size += size;
    src += size + 2;
  }

  http->size = total_size;
  if (http->size < (uint64_t)(dst_end - http->data)) {
    http->data[http->size] = '\0';
  }

  log_debug("%s\n", http->data);

  return RET_OK;
}

static ret_t data_reader_http_get(data_reader_http_t* http, const char* url) {
  char buff[1024];
  int32_t nr = 0;
  url_t* aurl = NULL;
  const char* p = NULL;
  tk_iostream_t* io = NULL;
  return_value_if_fail(url != NULL && http != NULL, RET_BAD_PARAMS);

  aurl = url_create(url);
  return_value_if_fail(aurl != NULL, RET_BAD_PARAMS);

  io = tk_stream_factory_create_iostream(url);
  return_value_if_fail(io != NULL, RET_BAD_PARAMS);

  tk_snprintf(buff, sizeof(buff) - 1, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
              url_get_path(url), aurl->host);
  nr = tk_iostream_write(io, buff, strlen(buff));
  url_destroy(aurl);
  goto_error_if_fail(nr > 0);

  nr = tk_iostream_read(io, buff, sizeof(buff) - 1);
  if (nr > 0) {
    buff[nr] = '\0';
    p = strstr(buff, "Content-Length:");

    if (p != NULL) {
      p += strlen("Content-Length:");
      while (*p == ' ') {
        p++;
      }
      /* 验证 Content-Length 值，防止整数溢出和过大分配 */
      int64_t content_length = tk_atoi(p);
      goto_error_if_fail(content_length > 0);
      goto_error_if_fail(content_length <= HTTP_MAX_CONTENT_LENGTH);
      
      http->size = (uint64_t)content_length;

      http->data = TKMEM_ALLOC(http->size + 1);
      goto_error_if_fail(http->data != NULL);

      p = strstr(buff, "\r\n\r\n");
      goto_error_if_fail(p != NULL);

      p += 4;
      nr = nr - (p - buff);
      /* 确保不会复制超过分配的内存 */
      if (nr > 0) {
        uint32_t copy_size = tk_min(nr, (int32_t)http->size);
        memcpy(http->data, p, copy_size);
        nr = copy_size;
      }

      if (http->size > (uint64_t)nr) {
        uint64_t remaining = http->size - nr;
        int32_t read_nr = tk_iostream_read_len(io, http->data + nr, (uint32_t)remaining, TK_ISTREAM_DEFAULT_TIMEOUT);
        goto_error_if_fail(read_nr > 0);
        /* 确保读取的字节数不超过预期 */
        if ((uint64_t)read_nr > remaining) {
          log_warn("data_reader_http_get: read more than expected\n");
          read_nr = (int32_t)remaining;
        }
      }

      http->data[http->size] = '\0';
    } else if (strstr(buff, "Transfer-Encoding: chunked") != NULL) {
      wbuffer_t wb;

      wbuffer_init_extendable(&wb);
      wbuffer_extend_capacity(&wb, 10240);

      p = strstr(buff, "\r\n\r\n");
      goto_error_if_fail(p != NULL);

      p += 4;
      nr = nr - (p - buff);
      wbuffer_write_binary(&wb, p, nr);

      while ((nr = tk_iostream_read(io, buff, sizeof(buff) - 1)) > 0) {
        if (wbuffer_write_binary(&wb, buff, nr) != RET_OK) {
          wbuffer_deinit(&wb);
          return RET_OOM;
        }
      }

      data_reader_http_decode_chuncked_data(http, &wb);
      wbuffer_deinit(&wb);
    } else {
      goto error;
    }
  }

  TK_OBJECT_UNREF(io);

  return RET_OK;
error:
  TK_OBJECT_UNREF(io);

  return RET_FAIL;
}

static int32_t data_reader_http_read(data_reader_t* reader, uint64_t offset, void* data,
                                     uint32_t size) {
  data_reader_http_t* http = (data_reader_http_t*)reader;
  return_value_if_fail(http != NULL && data != NULL, 0);
  return_value_if_fail(offset <= http->size, 0);
  size = tk_min(size, http->size - offset);

  memcpy(data, http->data + offset, size);

  return size;
}

static uint64_t data_reader_http_get_size(data_reader_t* reader) {
  data_reader_http_t* http = (data_reader_http_t*)reader;
  return_value_if_fail(http != NULL, 0);

  return http->size;
}

static ret_t data_reader_http_destroy(data_reader_t* reader) {
  data_reader_http_t* http = (data_reader_http_t*)reader;

  TKMEM_FREE(http->data);
  TKMEM_FREE(http);

  return RET_OK;
}

static const data_reader_vtable_t s_data_reader_http_vtable = {
    .read = data_reader_http_read,
    .get_size = data_reader_http_get_size,
    .destroy = data_reader_http_destroy,
};

data_reader_t* data_reader_http_create(const char* url) {
  data_reader_http_t* http = NULL;
  return_value_if_fail(url != NULL, NULL);
  http = TKMEM_ZALLOC(data_reader_http_t);
  return_value_if_fail(http != NULL, NULL);

  if (data_reader_http_get(http, url) != RET_OK) {
    TKMEM_FREE(http->data);
    TKMEM_FREE(http);
    return NULL;
  }

  http->data_reader.vt = &s_data_reader_http_vtable;

  return (data_reader_t*)http;
}
