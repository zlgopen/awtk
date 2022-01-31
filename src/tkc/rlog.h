/**
 * File:   rlog.h
 * Author: AWTK Develop Team
 * Brief:  rotate log api
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-11-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_RLOG_H
#define TK_RLOG_H

#include "tkc/fs.h"
#include "tkc/mutex_nest.h"

BEGIN_C_DECLS

/**
 * @class rlog_t
 * @annotation ["scriptable"]
 * 循环记录日志(支持多线程访问)。
 * 如果我们把日志写入文件中，随着时间的推移，日志文件会越来越大，最终占满这个磁盘，出现不可预料的错误。
 * rlog提供一个循环记录日志的功能，让日志文件始终不会超出指定的大小，达到指定大小后再从头开始写。
 *
 * > 为了实现简单，我们把日志文件分成0和1两个文件，先写文件0，到达指定最大值的一半时，再写文件1。
 * > 文件1到达指定最大值的一半时，删除文件0，并将文件1改名为文件0，重新创建文件1，继续写文件1，重复此过程。
 */
typedef struct _rlog_t {
  /*private*/
  fs_file_t* fp;
  char* buff;
  char* filename_pattern;

  uint32_t index;
  uint32_t max_size;
  uint32_t buff_size;
  tk_mutex_nest_t* mutex;
} rlog_t;

/**
 * @method rlog_create
 * 创建rlog对象。
 *
 * ```c
 * rlog_t* log = rlog_create("./logs/%d.log", 1020*1024, 256);
 * rlog_write(log, "hello\n");
 * rlog_destroy(log);
 * ```
 *
 * @annotation ["scriptable", "constructor"]
 * @param {const char*} filename_pattern 用来确定文件名的路径和文件名。
 * @param {uint32_t} max_size log文件占用最大磁盘空间(字节)。
 * @param {uint32_t} buff_size 用于指定print时的buff大小。
 *
 * @return {rlog_t*} 返回rlog对象。
 */
rlog_t* rlog_create(const char* filename_pattern, uint32_t max_size, uint32_t buff_size);

/**
 * @method rlog_write
 * 写入一条日志记录。
 *
 * @annotation ["scriptable"]
 * @param {rlog_t*} log 日志对象。
 * @param {const char*} str 日志字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rlog_write(rlog_t* log, const char* str);

/**
 * @method rlog_print
 * 写入一条日志记录。
 *
 * @param {rlog_t*} log 日志对象。
 * @param {const char*} format 格式字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rlog_print(rlog_t* log, const char* format, ...);

/**
 * @method rlog_destroy
 * 销毁日志对象。
 *
 * @param {rlog_t*} log 日志对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rlog_destroy(rlog_t* log);

/**
 * @method rlog_size
 * 获取日志大小。
 *
 * @param {rlog_t*} log 日志对象。
 * @param {uint32_t*} size 获取的日志大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rlog_size(rlog_t* log, uint32_t* size);

/**
 * @method rlog_clear
 * 清空日志。
 *
 * @param {rlog_t*} log 日志对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rlog_clear(rlog_t* log);

/**
 * @method rlog_read
 * 读取日志。
 *
 * @param {rlog_t*} log 日志对象。
 * @param {uint32_t} offs 日志偏移量。
 * @param {char*} buff 存放日志的缓存。
 * @param {uint32_t} buffsz 缓存大小。
 * @param {uint32_t*} readsz 读取出的日志大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rlog_read(rlog_t* log, uint32_t offs, char* buff, uint32_t buffsz, uint32_t* readsz);

END_C_DECLS

#endif /*TK_RLOG_H*/
