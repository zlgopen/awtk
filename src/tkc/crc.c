/**
 * File:   crc.c
 * Author: AWTK Develop Team
 * Brief:  crc and checksum helpers.
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-10-08 Li XianJing <xianjimli@hotmail.com> created
 * 2026-08-21 AWTK Develop Team reimplemented from public CRC parameters
 *
 */

/**
 * Algorithms follow public parameterizations (reflected / LSB-first):
 * - FCS-16: poly 0x8408 (x^16 + x^12 + x^5 + 1), init 0xFFFF
 * - CRC-16/MODBUS: poly 0xA001 (x^16 + x^15 + x^2 + 1), init 0xFFFF
 * - CRC-32/ISO-3309: poly 0xEDB88320, init 0xFFFFFFFF, no final xor
 *
 * Lookup tables are generated at runtime from these polynomials.
 */

#include "tkc/crc.h"

#ifdef CONFIG_ENABLE_FCS32
#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#endif /*CONFIG_ENABLE_FCS32*/

#define CRC_TABLE_SIZE 256
#define CRC_BYTE_BITS 8
#define CRC_LSB_MASK 0x01
#define CRC_BYTE_MASK 0xff
#define CRC16_FCS_POLY_REFLECTED 0x8408
#define CRC16_MODBUS_POLY_REFLECTED 0xa001
#define CRC32_IEEE_POLY_REFLECTED 0xedb88320u
#define CRC16_MODBUS_INIT 0xffff
#define CHKSUM_ONES_COMPLEMENT 0xffff
#define CRC32_FILE_BLOCK_SIZE_MIN 256
#define CRC32_FILE_BLOCK_SIZE_MAX (1024 * 1024)

#ifdef CONFIG_ENABLE_FCS16
static uint16_t s_crc16_fcs_table[CRC_TABLE_SIZE];
static uint16_t s_crc16_modbus_table[CRC_TABLE_SIZE];
#endif /*CONFIG_ENABLE_FCS16*/

#ifdef CONFIG_ENABLE_FCS32
static uint32_t s_crc32_table[CRC_TABLE_SIZE];
#endif /*CONFIG_ENABLE_FCS32*/

static uint8_t s_crc_tables_ready = FALSE;

#ifdef CONFIG_ENABLE_FCS16
static uint16_t crc16_reflect_table_item(uint16_t index, uint16_t poly) {
  uint32_t bit = 0;
  uint16_t crc = index;

  for (bit = 0; bit < CRC_BYTE_BITS; bit++) {
    if (crc & CRC_LSB_MASK) {
      crc = (crc >> 1) ^ poly;
    } else {
      crc = crc >> 1;
    }
  }

  return crc;
}

static uint16_t crc16_update_byte(uint16_t crc, uint8_t data, const uint16_t* table) {
  return (crc >> CRC_BYTE_BITS) ^ table[(crc ^ data) & CRC_BYTE_MASK];
}
#endif /*CONFIG_ENABLE_FCS16*/

#ifdef CONFIG_ENABLE_FCS32
static uint32_t crc32_reflect_table_item(uint32_t index, uint32_t poly) {
  uint32_t bit = 0;
  uint32_t crc = index;

  for (bit = 0; bit < CRC_BYTE_BITS; bit++) {
    if (crc & CRC_LSB_MASK) {
      crc = (crc >> 1) ^ poly;
    } else {
      crc = crc >> 1;
    }
  }

  return crc;
}

static uint32_t crc32_update_byte(uint32_t crc, uint8_t data, const uint32_t* table) {
  return (crc >> CRC_BYTE_BITS) ^ table[(crc ^ (uint32_t)data) & CRC_BYTE_MASK];
}
#endif /*CONFIG_ENABLE_FCS32*/

static void crc_ensure_tables(void) {
  uint32_t i = 0;

  if (s_crc_tables_ready) {
    return;
  }

  for (i = 0; i < CRC_TABLE_SIZE; i++) {
#ifdef CONFIG_ENABLE_FCS16
    s_crc16_fcs_table[i] = crc16_reflect_table_item((uint16_t)i, CRC16_FCS_POLY_REFLECTED);
    s_crc16_modbus_table[i] = crc16_reflect_table_item((uint16_t)i, CRC16_MODBUS_POLY_REFLECTED);
#endif /*CONFIG_ENABLE_FCS16*/
#ifdef CONFIG_ENABLE_FCS32
    s_crc32_table[i] = crc32_reflect_table_item(i, CRC32_IEEE_POLY_REFLECTED);
#endif /*CONFIG_ENABLE_FCS32*/
  }

  s_crc_tables_ready = TRUE;
}

#ifdef CONFIG_ENABLE_FCS16

uint16_t tk_crc16_byte(uint16_t crc, uint8_t data) {
  crc_ensure_tables();

  return crc16_update_byte(crc, data, s_crc16_fcs_table);
}

uint16_t tk_crc16(uint16_t crc, const void* data, int data_length) {
  const uint8_t* pdata = (const uint8_t*)data;

  crc_ensure_tables();

  while (data_length > 0) {
    crc = crc16_update_byte(crc, *pdata, s_crc16_fcs_table);
    pdata++;
    data_length--;
  }

  return crc;
}

uint16_t tk_crc16_modbus(const void* data, uint32_t len) {
  uint16_t crc = CRC16_MODBUS_INIT;
  const uint8_t* pdata = (const uint8_t*)data;

  crc_ensure_tables();

  while (len > 0) {
    crc = crc16_update_byte(crc, *pdata, s_crc16_modbus_table);
    pdata++;
    len--;
  }

  return crc;
}

#endif /*CONFIG_ENABLE_FCS16*/

#ifdef CONFIG_ENABLE_CHECKSUM

uint16_t tk_chksum_byte(uint16_t sum, uint8_t data) {
  return sum + data;
}

uint16_t tk_chksum(uint16_t sum, const void* data, int data_length) {
  const uint8_t* pdata = (const uint8_t*)data;

  while (data_length > 0) {
    sum = sum + *pdata;
    pdata++;
    data_length--;
  }

  return (uint16_t)(CHKSUM_ONES_COMPLEMENT - sum);
}

#endif /*CONFIG_ENABLE_CHECKSUM*/

#ifdef CONFIG_ENABLE_FCS32

uint32_t tk_crc32(uint32_t init, const void* data, int size) {
  int i = 0;
  uint32_t crc = init;
  const uint8_t* ptr = (const uint8_t*)data;

  crc_ensure_tables();

  if (ptr != NULL) {
    for (i = 0; i < size; i++) {
      crc = crc32_update_byte(crc, *ptr, s_crc32_table);
      ptr++;
    }
  }

  return crc;
}

uint32_t tk_crc32_byte(uint32_t crc, uint8_t data) {
  crc_ensure_tables();

  return crc32_update_byte(crc, data, s_crc32_table);
}

uint32_t tk_crc32_file(const char* filename, uint32_t block_size) {
  int32_t size = 0;
  fs_file_t* fp = NULL;
  void* buff = NULL;
  uint32_t crc32 = PPPINITFCS32;

  return_value_if_fail(filename != NULL, crc32);

  fp = fs_open_file(os_fs(), filename, "rb");
  return_value_if_fail(fp != NULL, crc32);
  block_size = tk_max_int(block_size, CRC32_FILE_BLOCK_SIZE_MIN);
  block_size = tk_min_int(block_size, CRC32_FILE_BLOCK_SIZE_MAX);

  buff = TKMEM_ALLOC(block_size + 1);
  goto_error_if_fail(buff != NULL);

  while (!fs_file_eof(fp)) {
    size = fs_file_read(fp, buff, block_size);
    if (size <= 0) {
      break;
    }
    crc32 = tk_crc32(crc32, buff, size);
  }

error:
  fs_file_close(fp);
  TKMEM_FREE(buff);

  return crc32;
}
#endif /*CONFIG_ENABLE_FCS32*/
