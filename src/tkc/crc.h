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

#define CONFIG_ENABLE_FCS16
#define CONFIG_ENABLE_FCS32
#define CONFIG_ENABLE_CHECKSUM

#ifdef CONFIG_ENABLE_CHECKSUM
#define INITCHECKSUM 0x0000
#define GOODCHECKSUM 0x0000
uint16_t tk_chksum_byte(uint16_t sum, uint8_t data);
uint16_t tk_chksum(uint16_t sum, const uint8_t* data, int data_length);
#endif

#ifdef CONFIG_ENABLE_FCS16
#define PPPINITFCS16 0xffff /* Initial FCS value */
#define PPPGOODFCS16 0xf0b8 /* Good final FCS value */
uint16_t tk_crc16_byte(uint16_t crc, uint8_t data);
uint16_t tk_crc16(uint16_t crc, const uint8_t* data, int data_length);
#endif

#ifdef CONFIG_ENABLE_FCS32
#define PPPINITFCS32 0xffffffff /* Initial FCS value */
#define PPPGOODFCS32 0xdebb20e3 /* Good final FCS value */
uint32_t tk_crc32_byte(uint32_t crc, uint8_t data);
uint32_t tk_crc32(uint32_t crc, const uint8_t* buf, int size);
#endif

END_C_DECLS

#endif /*TK_CRC_H*/
