/**
 * File:   awtk_tkc.h
 * Author: AWTK Develop Team
 * Brief:  awtk toolkit c libs
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
 * 2018-12-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TKC_H
#define TK_TKC_H

#include "tkc/action_queue.h"
#include "tkc/action_thread.h"
#include "tkc/action_thread_pool.h"
#include "tkc/asset_info.h"
#include "tkc/buffer.h"
#include "tkc/color.h"
#include "tkc/color_parser.h"
#include "tkc/compressor.h"
#include "tkc/cond.h"
#include "tkc/cond_var.h"
#include "tkc/crc.h"
#include "tkc/darray.h"
#include "tkc/data_reader.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_reader_file.h"
#include "tkc/data_writer.h"
#include "tkc/data_writer_factory.h"
#include "tkc/data_writer_file.h"
#include "tkc/date_time.h"
#include "tkc/easing.h"
#include "tkc/emitter.h"
#include "tkc/endian.h"
#include "tkc/event.h"
#include "tkc/event_source.h"
#include "tkc/event_source_fd.h"
#include "tkc/event_source_idle.h"
#include "tkc/event_source_manager.h"
#include "tkc/event_source_manager_default.h"
#include "tkc/event_source_timer.h"
#include "tkc/expr_eval.h"
#include "tkc/fs.h"
#include "tkc/func_call_parser.h"
#include "tkc/func_desc.h"
#include "tkc/idle_info.h"
#include "tkc/idle_manager.h"
#include "tkc/int_str.h"
#include "tkc/iostream.h"
#include "tkc/istream.h"
#include "tkc/log.h"
#include "tkc/matrix.h"
#include "tkc/mem.h"
#include "tkc/mime_types.h"
#include "tkc/mutex.h"
#include "tkc/named_value.h"
#include "tkc/object.h"
#include "tkc/object_array.h"
#include "tkc/object_default.h"
#include "tkc/ostream.h"
#include "tkc/path.h"
#include "tkc/platform.h"
#include "tkc/qaction.h"
#include "tkc/rect.h"
#include "tkc/ring_buffer.h"
#include "tkc/rom_fs.h"
#include "tkc/semaphore.h"
#include "tkc/slist.h"
#include "tkc/socket_pair.h"
#include "tkc/socket_helper.h"
#include "tkc/str.h"
#include "tkc/str_str.h"
#include "tkc/stream_const.h"
#include "tkc/thread.h"
#include "tkc/time_now.h"
#include "tkc/timer_info.h"
#include "tkc/timer_manager.h"
#include "tkc/tokenizer.h"
#include "tkc/types_def.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "tkc/value.h"
#include "tkc/value_desc.h"
#include "tkc/waitable_action_queue.h"
#include "tkc/wstr.h"

#endif /*TK_TKC_H*/
