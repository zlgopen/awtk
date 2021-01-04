/*
    Copyright 2016-2019 (C) Alexey Dynda

    This file is part of Tiny Protocol Library.

    Protocol Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Protocol Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Protocol Library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TK_CRC_H
#define TK_CRC_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class crc_t
 * crc相关函数。
 */
#define CONFIG_ENABLE_FCS16
#define CONFIG_ENABLE_FCS32
#define CONFIG_ENABLE_CHECKSUM

#ifdef CONFIG_ENABLE_CHECKSUM
#define INITCHECKSUM 0x0000
#define GOODCHECKSUM 0x0000

/**
 * @method tk_chksum
 * 计算检验和。
 * @annotation ["static"]
 * @param {uint16_t} sum 初始值(一般用INITCHECKSUM)
 * @param {const uint8_t*} data 数据。 
 * @param {int} size 数据长度。
 *
 * @return {uint16_t} 返回计算结果。
 */
uint16_t tk_chksum(uint16_t sum, const uint8_t* data, int size);

/**
 * @method tk_chksum_byte
 * 计算单子节的检验和。
 * @annotation ["static"]
 * @param {uint16_t} sum 初始值(一般用0，或者之前的值)
 * @param {uint8_t} data 数据。 
 *
 * @return {uint16_t} 返回计算结果。
 */
uint16_t tk_chksum_byte(uint16_t sum, uint8_t data);

#endif

#ifdef CONFIG_ENABLE_FCS16
#define PPPINITFCS16 0xffff /* Initial FCS value */
#define PPPGOODFCS16 0xf0b8 /* Good final FCS value */
/**
 * @method tk_crc16
 * 计算crc16。
 * @annotation ["static"]
 * @param {uint16_t} crc 初始值(一般用PPPINITFCS16)
 * @param {const uint8_t*} data 数据。 
 * @param {int} size 数据长度。
 *
 * @return {uint16_t} 返回计算结果。
 */
uint16_t tk_crc16(uint16_t crc, const uint8_t* data, int size);

/**
 * @method tk_crc16_byte
 * 计算单子节的crc16。
 * @annotation ["static"]
 * @param {uint16_t} crc 初始值(一般用PPPINITFCS16，或者之前的值)
 * @param {uint8_t} data 数据。 
 *
 * @return {uint16_t} 返回计算结果。
 */
uint16_t tk_crc16_byte(uint16_t crc, uint8_t data);
#endif

#ifdef CONFIG_ENABLE_FCS32
#define PPPINITFCS32 0xffffffff /* Initial FCS value */
#define PPPGOODFCS32 0xdebb20e3 /* Good final FCS value */
/**
 * @method tk_crc32
 * 计算crc32。
 * @annotation ["static"]
 * @param {uint32_t} crc 初始值(一般用PPPINITFCS32)
 * @param {const uint8_t*} data 数据。 
 * @param {int} size 数据长度。
 *
 * @return {uint32_t} 返回计算结果。
 */
uint32_t tk_crc32(uint32_t crc, const uint8_t* data, int size);

/**
 * @method tk_crc32_byte
 * 计算单子节的crc32。
 * @annotation ["static"]
 * @param {uint32_t} crc 初始值(一般用PPPINITFCS32，或者之前的值)
 * @param {uint8_t} data 数据。 
 *
 * @return {uint32_t} 返回计算结果。
 */
uint32_t tk_crc32_byte(uint32_t crc, uint8_t data);
#endif

END_C_DECLS

#endif /*TK_CRC_H*/
