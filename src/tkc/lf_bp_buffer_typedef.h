/**
 * File:   lf_bp_buffer_typedef.h
 * Author: AWTK Develop Team
 * Brief:  Lock Free Bipartite Buffer类型定义
 *
 * Copyright (c) 2024 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2026-03-20 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#ifndef TK_LF_BP_BUFFER_TYPEDEF_H
#define TK_LF_BP_BUFFER_TYPEDEF_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class tk_lf_bp_buffer_t
 * @export none
 * 无锁双分区缓冲区。
 * > 只适用于单生产者-单消费者（SPSC）场景。
 */
typedef struct _tk_lf_bp_buffer_t tk_lf_bp_buffer_t;

END_C_DECLS

#endif /*TK_LF_BP_BUFFER_TYPEDEF_H*/
