/**
 * File:   network_shell.c
 * Author: AWTK Develop Team
 * Brief:  awtk hal shell
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
 * 2021-04-19 Zhang Zhongji <zhangzhongji@zlg.cn> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/thread.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/tokenizer.h"
#include "network_shell.h"

#ifndef WIN32
#include <readline/history.h>
#include <readline/readline.h>

static char* command_generator(const char* text, int state);
static char** command_completion(const char* text, int start, int end) {
  char** matches = NULL;

  if (start == 0) {
    matches = rl_completion_matches(text, command_generator);
  }

  return (matches);
}
static ret_t aw_read_line_init(void) {
  rl_readline_name = "awtk";
  rl_attempted_completion_function = command_completion;

  return RET_OK;
}

static char* aw_read_line(const char* tips) {
  return readline(tips);
}

static ret_t aw_read_line_free(char* line) {
  free(line);
  return RET_OK;
}

static ret_t aw_read_line_add_history(char* line) {
  add_history(line);
  return RET_OK;
}

#else
static char s_line[1024];
static ret_t aw_read_line_init(void) {
  return RET_OK;
}

static char* aw_read_line(const char* tips) {
  printf("%s", tips);
  fflush(stdout);
  fgets(s_line, sizeof(s_line) - 1, stdin);

  return s_line;
}

static ret_t aw_read_line_free(char* line) {
  return RET_OK;
}

static ret_t aw_read_line_add_history(char* line) {
  return RET_OK;
}
#endif /**/

#include "tkc/data_reader_mem.h"
#include "conf_io/conf_ubjson.h"

#define KNRM "\x1B[0m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KMAG "\x1B[35m"

typedef ret_t (*cmd_line_func_t)(network_interface_t* network_interface, tokenizer_t* tokenizer);

typedef struct _cmd_entry_t {
  const char* name;
  const char* alias;
  const char* desc;
  const char* help;
  cmd_line_func_t func;
} cmd_entry_t;

//网卡接口使能函数。
static ret_t func_network_interface_enable(network_interface_t* interface, tokenizer_t* tokenizer) {
  ret_t ret = RET_FAIL;
  ret = network_interface_enable(interface);
  log_debug("%s:%s\n", __FUNCTION__, ret == RET_OK ? "OK" : "FAIL");
  return RET_OK;
}

//网卡接口禁能函数。
static ret_t func_network_interface_disable(network_interface_t* interface,
                                            tokenizer_t* tokenizer) {
  ret_t ret = RET_FAIL;
  ret = network_interface_disable(interface);
  log_debug("%s:%s\n", __FUNCTION__, ret == RET_OK ? "OK" : "FAIL");
  return RET_OK;
}

//网卡接口获取IP地址。
static ret_t func_network_interface_get_ipaddr(network_interface_t* interface,
                                               tokenizer_t* tokenizer) {
  char* ip = NULL;
  ip = network_interface_get_ipaddr(interface);
  log_debug("%s:%s\n", __FUNCTION__, ip != NULL ? ip : "FAIL");

  return RET_OK;
}

//网卡接口获取MAC地址。
static ret_t func_network_interface_get_macaddr(network_interface_t* interface,
                                                tokenizer_t* tokenizer) {
  char* mac = NULL;
  mac = network_interface_get_macaddr(interface);
  log_debug("%s:%s\n", __FUNCTION__, mac != NULL ? mac : "FAIL");
  return RET_OK;
}

//网卡接口获取状态。
static ret_t func_network_interface_get_status(network_interface_t* interface,
                                               tokenizer_t* tokenizer) {
  int status = 0;
  status = network_interface_get_status(interface);
  log_debug("%s:%d\n", __FUNCTION__, status);
  return RET_OK;
}

//网卡接口获取通信质量。
static ret_t func_network_interface_get_quality(network_interface_t* interface,
                                                tokenizer_t* tokenizer) {
  int quality = 0;
  quality = network_interface_get_quality(interface);
  log_debug("%s:%d\n", __FUNCTION__, quality);
  return RET_OK;
}

//网卡接口设置IP地址。
static ret_t func_network_interface_set_ipaddr(network_interface_t* interface,
                                               tokenizer_t* tokenizer) {
  ret_t ret = RET_FAIL;
  char* ipaddr = tk_strdup(tokenizer_next(tokenizer));
  char* netmask = tk_strdup(tokenizer_next(tokenizer));
  return_value_if_fail(ipaddr != NULL && netmask != NULL, RET_BAD_PARAMS);

  ret = network_interface_set_ipaddr(interface, ipaddr, netmask);

  log_debug("%s:%s\n", __FUNCTION__, ret == RET_OK ? "OK" : "FAIL");
  TKMEM_FREE(ipaddr);
  TKMEM_FREE(netmask);

  return RET_OK;
}

//网卡接口设置DNS。
static ret_t func_network_interface_set_dns(network_interface_t* interface,
                                            tokenizer_t* tokenizer) {
  ret_t ret = RET_FAIL;
  char* dns = tk_strdup(tokenizer_next(tokenizer));
  return_value_if_fail(dns != NULL, RET_BAD_PARAMS);

  ret = network_interface_set_dns(interface, dns);

  log_debug("%s:%s\n", __FUNCTION__, ret == RET_OK ? "OK" : "FAIL");
  TKMEM_FREE(dns);

  return RET_OK;
}

//网卡接口设置dhcp。
static ret_t func_network_interface_set_dhcp(network_interface_t* interface,
                                             tokenizer_t* tokenizer) {
  ret_t ret = RET_FAIL;
  ret = network_interface_set_dhcp(interface);
  log_debug("%s:%s\n", __FUNCTION__, ret == RET_OK ? "OK" : "FAIL");
  return RET_OK;
}

//网卡接口设置网关
static ret_t func_network_interface_set_gateway(network_interface_t* interface,
                                                tokenizer_t* tokenizer) {
  ret_t ret = RET_FAIL;
  char* gateway = tk_strdup(tokenizer_next(tokenizer));
  char* dev = tk_strdup(tokenizer_next(tokenizer));
  return_value_if_fail(gateway != NULL && dev != NULL, RET_BAD_PARAMS);

  ret = network_interface_set_gateway(interface, gateway, dev);

  log_debug("%s:%s\n", __FUNCTION__, ret == RET_OK ? "OK" : "FAIL");
  TKMEM_FREE(gateway);
  TKMEM_FREE(dev);

  return RET_OK;
}

//网卡接口设置WIFI STA模式。
static ret_t func_network_interface_set_wifi_sta(network_interface_t* interface,
                                                 tokenizer_t* tokenizer) {
  ret_t ret = RET_FAIL;
  char* essid = tk_strdup(tokenizer_next(tokenizer));
  char* passwd = tk_strdup(tokenizer_next(tokenizer));
  char* auth = tk_strdup(tokenizer_next(tokenizer));
  return_value_if_fail(essid != NULL && passwd && auth != NULL, RET_BAD_PARAMS);

  ret = network_interface_set_wifi_sta(interface, essid, passwd, tk_atoi(auth));

  log_debug("%s:%s\n", __FUNCTION__, ret == RET_OK ? "OK" : "FAIL");
  TKMEM_FREE(essid);
  TKMEM_FREE(passwd);
  TKMEM_FREE(auth);

  return RET_OK;
}

//网卡接口设置WIFI AP模式。
static ret_t func_network_interface_set_wifi_ap(network_interface_t* interface,
                                                tokenizer_t* tokenizer) {
  ret_t ret = RET_FAIL;
  char* essid = tk_strdup(tokenizer_next(tokenizer));
  char* passwd = tk_strdup(tokenizer_next(tokenizer));
  char* auth = tk_strdup(tokenizer_next(tokenizer));
  char* channel = tk_strdup(tokenizer_next(tokenizer));
  char* freq = tk_strdup(tokenizer_next(tokenizer));
  char* hidden = tk_strdup(tokenizer_next(tokenizer));
  return_value_if_fail(essid != NULL && passwd != NULL && auth != NULL && channel != NULL &&
                           freq != NULL && hidden != NULL,
                       RET_BAD_PARAMS);

  ret = network_interface_set_wifi_ap(interface, essid, passwd, tk_atoi(auth), tk_atoi(channel),
                                      tk_atoi(freq), tk_atoi(hidden));

  log_debug("%s:%s\n", __FUNCTION__, ret == RET_OK ? "OK" : "FAIL");
  TKMEM_FREE(essid);
  TKMEM_FREE(passwd);
  TKMEM_FREE(auth);
  TKMEM_FREE(channel);
  TKMEM_FREE(freq);
  TKMEM_FREE(hidden);

  return RET_OK;
}

static ret_t func_quit(network_interface_t* interface, tokenizer_t* tokenizer) {
  return RET_STOP;
}

static ret_t func_help(network_interface_t* interface, tokenizer_t* tokenizer);
static const cmd_entry_t s_cmds[] = {
    {"enable", "e", "Enable network interface", "enable", func_network_interface_enable},
    {"disable", "d", "Disable network interface", "disable", func_network_interface_disable},
    {"get_ip", "ip", "Get IP address of network interface", "get_ip",
     func_network_interface_get_ipaddr},
    {"get_mac", "mac", "Get mac address of network interface", "get_mac",
     func_network_interface_get_macaddr},
    {"get_status", "s", "Get status of network interface", "get_status",
     func_network_interface_get_status},
    {"get_quality", "qua", "Get quality of network interface", "get_quality",
     func_network_interface_get_quality},
    {"set_ip", "sip", "Set IP address of network interface", "set_ip ip_address netmask",
     func_network_interface_set_ipaddr},
    {"set_dns", "sdns", "Set dns of network interface", "set_dns dns",
     func_network_interface_set_dns},
    {"set_dhcp", "dhcp", "Enable dhcp of network interface", "set_dhcp",
     func_network_interface_set_dhcp},
    {"set_gateway", "sgw", "Set gateway of network interface", "set_gateway gateway dev_name",
     func_network_interface_set_gateway},
    {"set_wifi_sta", "ssta", "Set wifi sta of network interface", "set_wifi_sta essid passwd auth",
     func_network_interface_set_wifi_sta},
    {"set_wifi_ap", "sap", "Set wifi ap of network interface",
     "set_wifi_ap essid passwd auth channel freq hidden", func_network_interface_set_wifi_ap},
    {"quit", "q", "Quit loop", "quit", func_quit},
    {NULL, NULL, NULL}};

static char* command_generator(const char* text, int state) {
  static int list_index, len;

  if (!state) {
    list_index = 0;
    len = strlen(text);
  }

  while (s_cmds[list_index].name != NULL) {
    const cmd_entry_t* iter = s_cmds + list_index;
    list_index++;

    if (iter->name == NULL) {
      break;
    }

    if (strncmp(iter->name, text, len) == 0) {
      return strdup(iter->name);
    }

    if (strncmp(iter->help, text, len) == 0) {
      return strdup(iter->help);
    }
  }

  return ((char*)NULL);
}

static ret_t func_help(network_interface_t* interface, tokenizer_t* tokenizer) {
  uint32_t i = 0;
  printf(KMAG "================================================\n" KNRM);
  while (s_cmds[i].name != NULL) {
    const cmd_entry_t* iter = s_cmds + i;
    printf(KYEL "%u: %s(%s)\n" KNRM, i, iter->desc, iter->alias);
    printf("------------------------------\n");
    printf(" # %s\n", iter->help);
    printf(KGRN "------------------------------------------------\n" KNRM);
    i++;
  }
  printf(KMAG "================================================\n" KNRM);

  return RET_OK;
}

static ret_t register_functions(tk_object_t* obj) {
  uint32_t i = 0;
  while (s_cmds[i].name != NULL) {
    const cmd_entry_t* iter = s_cmds + i;
    tk_object_set_prop_pointer(obj, iter->alias, iter->func);
    tk_object_set_prop_pointer(obj, iter->name, iter->func);
    i++;
  }
  return RET_OK;
}

static ret_t network_shell_exec(network_interface_t* network_interface, tk_object_t* obj,
                                const char* line) {
  tokenizer_t t;
  ret_t ret = RET_OK;
  const char* name = NULL;
  cmd_line_func_t func = NULL;

  return_value_if_fail(network_interface != NULL && network_interface->vt != NULL && line != NULL,
                       RET_BAD_PARAMS);
  tokenizer_init(&t, line, strlen(line), " \t\r\n");

  name = tokenizer_next(&t);
  if (name == NULL || *name == '\0') {
    return RET_OK;
  }

  func = (cmd_line_func_t)tk_object_get_prop_pointer(obj, name);
  if (func == NULL) {
    func = func_help;
  }

  ret = func(network_interface, &t);
  if (ret == RET_BAD_PARAMS) {
    func_help(network_interface, &t);
  }

  return ret;
}

ret_t network_shell_run(network_interface_t* network_interface) {
  tk_object_t* obj = object_default_create();
  return_value_if_fail(network_interface != NULL, RET_BAD_PARAMS);

  aw_read_line_init();
  register_functions(obj);

  while (network_interface != NULL) {
    char* line = aw_read_line(KMAG "[hal] # " KNRM);
    if (line == NULL) {
      break;
    }

    if (network_shell_exec(network_interface, obj, line) == RET_STOP) {
      aw_read_line_free(line);
      break;
    }

    aw_read_line_add_history(line);
    aw_read_line_free(line);
  }

  return RET_OK;
}
