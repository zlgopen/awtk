#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/platform.h"
#include "common/utils.h"
#include "base/font.h"
#include "font_loader/font_loader_bitmap.h"

static uint8_t uint8_get_2bit(uint8_t v, uint8_t index) {
  uint8_t vv = (v >> (index * 2)) & 0b11;

  return vv;
}

static uint8_t uint8_get_4bit(uint8_t v, uint8_t index) {
  uint8_t vv = (v >> (index * 4)) & 0b1111;

  return vv;
}

static ret_t print_pixel(uint8_t c) {
  printf("%02x ", (int)c);
  return RET_OK;
}

static ret_t font_dump(uint8_t* data, uint32_t size, bool_t dump_data) {
  uint32_t i = 0;
  font_bitmap_header_t* header = (font_bitmap_header_t*)data;

  log_debug("version:%u\n", (uint32_t)(header->version));
  log_debug("char_nr:%u\n", (uint32_t)(header->char_nr));
  log_debug("format:%u\n", (uint32_t)(header->format));
  log_debug("font_size:%u\n", (uint32_t)(header->font_size));
  log_debug("ascent:%d\n", (int32_t)(header->ascent));
  log_debug("descent:%d\n", (int32_t)(header->descent));
  log_debug("line_gap:%u\n", (uint32_t)(header->line_gap));
  log_debug("============================================\n");
  for (i = 0; i < header->char_nr; i++) {
    log_debug("\n--------------------------------------------\n");
    font_bitmap_index_t* iter = header->index + i;
    glyph_t* g = (glyph_t*)(data + iter->offset);
    log_debug("char: %u(0x%04x)\n", (uint32_t)(iter->c), (uint32_t)(iter->c));
    log_debug("size: %u(0x%04x)\n", (uint32_t)(iter->size), (uint32_t)(iter->size));
    log_debug("offset:%u(0x%04x)\n", (uint32_t)(iter->offset), (uint32_t)(iter->offset));
    if (iter->offset) {
      log_debug("x:%d\n", (int)(g->x));
      log_debug("y:%d\n", (int)(g->y));
      log_debug("w:%d\n", (int)(g->w));
      log_debug("h:%d\n", (int)(g->h));
      log_debug("foramt:%d\n", (int)(g->format));
      log_debug("advance:%d\n", (int)(g->advance));
      log_debug("pitch:%d\n", (int)(g->pitch));
    } else {
      log_debug("no data\n");
    }

    if (dump_data && iter->offset) {
      int32_t x = 0;
      int32_t y = 0;
      uint8_t* p = data + iter->offset + sizeof(glyph_t) - sizeof(g->data);
      for (y = 0; y < g->h; y++) {
        for (x = 0; x < g->w;) {
          switch (g->format) {
            case GLYPH_FMT_ALPHA: {
              print_pixel(*p);
              x++;
              break;
            }
            case GLYPH_FMT_MONO: {
              log_debug("%02x ", *p);
              x += 8;
              break;
            }
            case GLYPH_FMT_ALPHA2: {
              print_pixel(uint8_get_2bit(*p, 0) << 6);
              print_pixel(uint8_get_2bit(*p, 1) << 4);
              print_pixel(uint8_get_2bit(*p, 2) << 2);
              print_pixel(uint8_get_2bit(*p, 3));
              x += 4;
              break;
            }
            case GLYPH_FMT_ALPHA4: {
              print_pixel(uint8_get_4bit(*p, 0) << 4);
              print_pixel(uint8_get_4bit(*p, 1) << 4);
              x += 2;
              break;
            }
            default: {
              assert(!"not supported");
              break;
            }
          }
          p++;
        }
        log_debug("\n");
      }
      p += g->pitch;
    }
  }

  return RET_OK;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage:%s font.bin [all]\n", argv[0]);
  } else {
    uint32_t size = 0;
    const char* filename = argv[1];
    void* data = file_read(filename, &size);
    bool_t dump_data = argc == 3 ? TRUE : FALSE;

    if (data == NULL) {
      printf("file %s not found\n", filename);
    } else {
      font_dump((uint8_t*)data, size, dump_data);
      TKMEM_FREE(data);
    }
  }
  return 0;
}
