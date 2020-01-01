/**
 * File:   rom_file.h
 * Author: AWTK Develop Team
 * Brief:  posix file api for rom data.
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-06-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ROM_FS_H
#define TK_ROM_FS_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class rom_t
 * @annotation ["fake"]
 * rom_file接口。
 * 
 * > 主要用于给google pinyin输入提供兼容的文件系统API。
 *
 * 示例：
 *
 * ```c
 * char rbuff[100];
 * size_t len = 0;
 * const char* buff = "hello world";
 * rom_file_t* f = rom_fopen_buff((const uint8_t*)buff, sizeof(buff));
 * len = rom_fread(rbuff, 2, 1, f);
 * rom_fclose(f);
 * ```
 *
 */
typedef struct _rom_file_t {
  uint32_t cursor;
  uint32_t capacity;
  const uint8_t* data;
} rom_file_t;

/**
 * @method rom_fopen
 *
 * 打开文件。
 *
 * @param {const char*} name 文件名。
 * @param {const char*} mode 打开方式。
 *
 * @return {ret_t} 返回不是NULL表示成功，否则表示失败。
 */
rom_file_t* rom_fopen(const char* name, const char* mode);

/**
 * @method rom_fopen_buff
 * 打开文件。
 * @param {const uint8_t*} data 数据。
 * @param {uint32_t} capacity 容量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
rom_file_t* rom_fopen_buff(const uint8_t* data, uint32_t capacity);

/**
 * @method rom_ftell
 *
 * 返回文件当前位置。
 *
 * @param {rom_file_t*} f rom_file_t对象。
 *
 * @return {long} 返回当前位置。
 */
long rom_ftell(rom_file_t* f);

/**
 * @method rom_feof
 *
 * 判断是否到文件末尾。
 *
 * @param {rom_file_t*} f rom_file_t对象。
 *
 * @return {int} 返回当前位置。
 */
int rom_feof(rom_file_t* f);

int rom_ferror(rom_file_t* f);

/**
 * @method rom_fseek
 *
 * 设置文件当前位置。
 *
 * @param {rom_file_t*} f rom_file_t对象。
 * @param {long} offset 偏移量。
 * @param {int} whence 要从哪个位置开始偏移， SEEK_SET表示从头开始，SEEK_CUR表示从当前位置开始，SEEK_END表示从文件末尾开始。
 *
 * @return {int} 返回0表示成功。
 */
int rom_fseek(rom_file_t* f, long offset, int whence);

/**
 * @method rom_fread
 *
 * 读取文件。
 *
 * @param {void*} ptr 数据的缓冲区。
 * @param {size_t} size 缓冲区大小。
 * @param {size_t} nitems 元素的个数。
 * @param {rom_file_t*} f rom_file_t对象。
 *
 * @return {size_t} 返回实际读取的字节数。
 */
size_t rom_fread(void* ptr, size_t size, size_t nitems, rom_file_t* f);

/**
 * @method rom_fwrite
 *
 * 写文件。
 *> 本函数什么也不做。
 * @param {void*} ptr 数据的缓冲区。
 * @param {size_t} size 缓冲区大小。
 * @param {size_t} nitems 元素的个数。
 * @param {rom_file_t*} f rom_file_t对象。
 *
 * @return {size_t} 返回实际写的字节数。
 */
size_t rom_fwrite(const void* ptr, size_t size, size_t nitems, rom_file_t* f);

/**
 * @method rom_fclose
 *
 * 关闭文件。
 *
 * @param {rom_file_t*} f rom_file_t对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rom_fclose(rom_file_t* f);

END_C_DECLS

#endif /*TK_ROM_FS_H*/
