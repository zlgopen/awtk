/**
 * File:   bitset.h
 * Author: AWTK Develop Team
 * Brief:  位集合。
 *
 * Copyright (c) 2026 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2026-10-26 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_BITSET_H
#define TK_BITSET_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class bitset_t
 * @annotation ["fake"]
 * @export none
 * 位集合。
 */

/* 一个字节的位数。 */
#define TK_BITSET_BITS_PER_BYTE 8

/**
 * @method TK_BITSET_POS
 * @annotation ["static"]
 * @export none
 * 计算位序号。
 * @param {INT_t} width 每个元素占用的位数。
 * @param {INT_t} index 元素序号。
 * @param {INT_t} bit 元素内的位序号(小于width)。
 *
 * @return {INT_t} 返回位序号。
 */
#define TK_BITSET_POS(width, index, bit) ((index) * (width) + (bit))

/**
 * @method TK_BITSET_BYTE_INDEX
 * @annotation ["static"]
 * @export none
 * 获取位序号对应的字节下标。
 * @param {INT_t} pos 位序号。
 *
 * @return {INT_t} 返回字节下标。
 */
#define TK_BITSET_BYTE_INDEX(pos) ((pos) / TK_BITSET_BITS_PER_BYTE)

/**
 * @method TK_BITSET_BIT_INDEX
 * @annotation ["static"]
 * @export none
 * 获取位序号在字节内的位下标。
 * @param {INT_t} pos 位序号。
 *
 * @return {INT_t} 返回字节内的位下标。
 */
#define TK_BITSET_BIT_INDEX(pos) ((pos) % TK_BITSET_BITS_PER_BYTE)

/**
 * @method TK_BITSET_BYTES_FOR_BITS
 * @annotation ["static"]
 * @export none
 * 计算nr个位需要的字节数。
 * @param {INT_t} nr 位数。
 *
 * @return {INT_t} 返回字节数。
 */
#define TK_BITSET_BYTES_FOR_BITS(nr) TK_BITSET_BYTE_INDEX((nr) + TK_BITSET_BITS_PER_BYTE - 1)

/**
 * @method TK_BITSET_BYTE
 * @annotation ["static"]
 * @export none
 * 获取位序号对应的字节地址(去掉const限定)。
 * @param {uint8_t*} data 位数据缓冲区。
 * @param {INT_t} pos 位序号。
 *
 * @return {uint8_t*} 返回字节地址。
 */
#define TK_BITSET_BYTE(data, pos) ((uint8_t*)(data) + TK_BITSET_BYTE_INDEX(pos))

/**
 * @method TK_BITSET_OP
 * @annotation ["static"]
 * @export none
 * 对指定位执行位操作，不检查边界。
 * @param {MACRO} OP 位操作宏(如TK\_TEST\_BIT)。
 * @param {uint8_t*} data 位数据缓冲区。
 * @param {INT_t} pos 位序号。
 *
 * @return {INT_t} 读取类操作返回1表示该位为1，否则返回0；其它操作无返回值。
 */
#define TK_BITSET_OP(OP, data, pos) (OP(*TK_BITSET_BYTE(data, pos), TK_BITSET_BIT_INDEX(pos)))

END_C_DECLS

#endif /*TK_BITSET_H*/
