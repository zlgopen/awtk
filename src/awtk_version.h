/**
 * File:   awtk_version.h
 * Author: AWTK Develop Team
 * Brief:  awtk version
 *
 * Copyright (c) 2021 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-08-06 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */
#ifndef AWTK_VERSION_H
#define AWTK_VERSION_H

#define AWTK_VERSION_MAJOR 1
#define AWTK_VERSION_MINOR 7
#define AWTK_VERSION_MICRO 0
#define AWTK_VERSION_RELEASE_NUMBER 0

#define AWTK_VERSION_EXPERIMENTAL 0x7FFFFFFF

/* awtk develop released a stable version  */
#ifndef AWTK_VERSION_RELEASE_ID
#define AWTK_VERSION_RELEASE_ID AWTK_VERSION_EXPERIMENTAL
#endif /*AWTK_VERSION_RELEASE_ID*/

#endif /*AWTK_VERSION_H*/
