/**
 * File:   hal_shell.h
 * Author: AWTK Develop Team
 * Brief:  shell of hal
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
 * 2021-04-19 Zhang Zhongji <zhangzhongji@zlg.cn> created
 *
 */

#ifndef NETWORK_SHELL_H
#define NETWORK_SHELL_H

#include "tkc/types_def.h"
#include "hal/network_interface.h"

BEGIN_C_DECLS

/**
 * @class hal_shell_t
 * @annotation ["fake"]
 * a shell to access network interface
 */

/**
 * @method hal_shell_run
 * @export none
 * 运行shell。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t network_shell_run(network_interface_t* network_interface);

END_C_DECLS

#endif /*NETWORK_SHELL_H*/
