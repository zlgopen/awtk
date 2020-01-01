/**
 * File:   tk_iostream.h
 * Author: AWTK Develop Team
 * Brief:  input/ouput stream interface
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IOSTREAM_CONST_H
#define TK_IOSTREAM_CONST_H

BEGIN_C_DECLS

#define TK_STREAM_PROP_FD "fd"
#define TK_STREAM_PROP_IS_OK "is_ok"
#define TK_STREAM_PROP_IS_EOS "is_eos"
#define TK_STREAM_PROP_TIMEOUT "timeout"
#define TK_STREAM_PROP_RETRY_TIMES "retry_times"
#define TK_STREAM_PROP_HAS_BUFFERED_DATA "has_buffered_data"
#define TK_STREAM_PROP_COMPRESS_THRESHOLD "compress_threshold"

#define TK_STREAM_CMD_IFLUSH "iflush"
#define TK_STREAM_CMD_OFLUSH "oflush"

END_C_DECLS

#endif /*TK_IOSTREAM_CONST_H*/
