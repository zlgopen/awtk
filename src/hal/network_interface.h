/**
 * File:   network_interface.h
 * Author: AWTK Develop Team
 * Brief:  network_interface 
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-02-3 pengguowen<pengguowen@zlg.cn> created
 *
 */

#ifndef _NETWORK_INTERFACE_H_
#define _NETWORK_INTERFACE_H_

#include "tkc/types_def.h"
#include "tkc/object.h"

BEGIN_C_DECLS

typedef struct _network_interface_t network_interface_t;
/**
 * @enum network_interface_wifi_auth_t 
 * 属性描述WIFI的认证方式。
 */
typedef enum _network_interface_wifi_auth_t {
  /**
   * @const NETWORK_INTERFACE_WIFI_AUTH_WEP 
   * WEP认证。
   */
  NETWORK_INTERFACE_WIFI_AUTH_WEP = 1,
  /**
   * @const NETWORK_INTERFACE_WIFI_AUTH_WPAWPA2
   * WPA/WPA2 认证。
   */
  NETWORK_INTERFACE_WIFI_AUTH_WPAWPA2,
  /**
   * @const NETWORK_INTERFACE_WIFI_AUTH_NONE
   * 无认证。
   */
  NETWORK_INTERFACE_WIFI_AUTH_NONE
} network_interface_wifi_auth_t;

/**
 * @enum network_interface_wifi_freq_t 
 * 属性描述WIFI的频段。
 */
typedef enum _network_interface_wifi_freq_t {
  /**
   * @const NETWORK_INTERFACE_WIFI_FREQ_2_4G
   * 2.4G频段。
   */
  NETWORK_INTERFACE_WIFI_FREQ_2_4G = 1,
  /**
   * @const NETWORK_INTERFACE_WIFI_FREQ_5G
   * 5G频段。
   */
  NETWORK_INTERFACE_WIFI_FREQ_5G
} network_interface_wifi_freq_t;

/**
 * @enum network_interface_type_t
 * 属性描述网卡类型。
 */
typedef enum _network_interface_type_t {
  /**
   * @const NETWORK_INTERFACE_TYPE_ETH
   * 以太网。
   */
  NETWORK_INTERFACE_TYPE_ETH = 1,
  /**
   * @const NETWORK_INTERFACE_TYPE_WIFI
   * WIFI。
   */
  NETWORK_INTERFACE_TYPE_WIFI,
  /**
   * @const NETWORK_INTERFACE_TYPE_MOBILE
   * 3G/4G/5G。
   */
  NETWORK_INTERFACE_TYPE_MOBILE,
  /**
   * @const NETWORK_INTERFACE_TYPE_UNKNOWN
   * 未知类型。
   */
  NETWORK_INTERFACE_TYPE_UNKNOWN
} network_interface_type_t;

typedef void (*network_interface_destroy_t)(network_interface_t* interface);
typedef ret_t (*network_interface_enable_t)(network_interface_t* interface);
typedef ret_t (*network_interface_disable_t)(network_interface_t* interface);
typedef char* (*network_interface_get_ipaddr_t)(network_interface_t* interface);
typedef char* (*network_interface_get_macaddr_t)(network_interface_t* interface);
typedef int (*network_interface_get_status_t)(network_interface_t* interface);
typedef int (*network_interface_get_quality_t)(network_interface_t* interface);
typedef ret_t (*network_interface_set_ipaddr_t)(network_interface_t* interface, const char* ipaddr,
                                                const char* netmask);
typedef ret_t (*network_interface_set_dns_t)(network_interface_t* interface, const char* dns);
typedef ret_t (*network_interface_set_dhcp_t)(network_interface_t* interface);
typedef ret_t (*network_interface_set_gateway_t)(network_interface_t* interface,
                                                 const char* gateway, const char* dev);
typedef ret_t (*network_interface_set_wifi_sta_t)(network_interface_t* interface, const char* essid,
                                                  const char* passwd,
                                                  network_interface_wifi_auth_t auth);
typedef ret_t (*network_interface_set_wifi_ap_t)(network_interface_t* interface, const char* essid,
                                                 const char* passwd,
                                                 network_interface_wifi_auth_t auth,
                                                 uint8_t channel,
                                                 network_interface_wifi_freq_t freq,
                                                 uint8_t hidden);

typedef struct _network_interface_vtable_t {
  const char* type;
  const char* desc;
  network_interface_enable_t enable;
  network_interface_disable_t disable;
  network_interface_destroy_t destroy;
  network_interface_get_ipaddr_t get_ipaddr;
  network_interface_get_macaddr_t get_macaddr;
  network_interface_get_status_t get_status;
  network_interface_get_quality_t get_quality;
  network_interface_set_ipaddr_t set_ipaddr;
  network_interface_set_dns_t set_dns;
  network_interface_set_gateway_t set_gateway;
  network_interface_set_dhcp_t set_dhcp;
  network_interface_set_wifi_ap_t set_wifi_ap;
  network_interface_set_wifi_sta_t set_wif_sta;
} network_interface_vtable_t;

/**
 * @class network_interface_t 
 * 网卡接口。
 */
struct _network_interface_t {
  const network_interface_vtable_t* vt;
  char* interface_name;
  network_interface_type_t type;
};

/**
 * @method network_interface_create 
 * 网卡接口创建函数。
 *
 * @param {char *} interface 网卡名。
 * @param {type} network_interface_type_t 网卡接口类型。
 *
 * @return {network_interface_t} 网卡接口对象。
 */
network_interface_t* network_interface_create(const char* interface, network_interface_type_t type);

/**
 * @method network_interface_enable
 * 网卡接口使能函数。
 *
 * @param {network_interface_t *} interface 网卡对象。
 *
 * @return {ret_t} RET_OK表示成功，否则失败。
 */
ret_t network_interface_enable(network_interface_t* interface);

/**
 * @method network_interface_disable
 * 网卡接口禁能函数。
 *
 * @param {network_interface_t *} interface 网卡对象。
 *
 * @return {ret_t} RET_OK表示成功，否则失败。
 */
ret_t network_interface_disable(network_interface_t* interface);

/**
 * @method network_interface_get_ipaddr
 * 网卡接口获取IP地址。
 *
 * @param {network_interface_t *} interface 网卡对象。
 *
 * @return {char *} 返回IP地址成功，NULL失败。
 */
char* network_interface_get_ipaddr(network_interface_t* interface);

/**
 * @method network_interface_get_macaddr
 * 网卡接口获取MAC地址。
 *
 * @param {network_interface_t *} interface 网卡对象。
 *
 * @return {char *} 返回MAC地址成功，NULL失败。
 */
char* network_interface_get_macaddr(network_interface_t* interface);

/**
 * @method network_interface_get_status
 * 网卡接口获取状态。
 *
 * @param {network_interface_t *} interface 网卡对象。
 *
 * @return {int} 0表示未连接，1表示连接成功，-1表示操作失败。
 */
int network_interface_get_status(network_interface_t* interface);

/**
 * @method network_interface_get_quality
 * 网卡接口获取通信质量。
 *
 * @param {network_interface_t *} interface 网卡对象。
 *
 * @return {int} 对于无线网络返回信号RSSI的绝对值， 对于有线网络，返回10、100、1000M带宽。
 */
int network_interface_get_quality(network_interface_t* interface);

/**
 * @method network_interface_set_ipaddr
 * 网卡接口设置IP地址。
 *
 * @param {network_interface_t *} interface 网卡对象。
 * @param {char *} ipaddr 网卡IP。
 * @param {char *} netmask 网卡MASK。
 *
 * @return {ret_t} RET_OK表示成功，否则失败。
 */
ret_t network_interface_set_ipaddr(network_interface_t* interface, const char* ipaddr,
                                   const char* netmask);

/**
 * @method network_interface_set_dns
 * 网卡接口设置DNS。
 *
 * @param {network_interface_t *} interface 网卡对象。
 * @param {char *} dns 网卡dns。
 *
 * @return {ret_t} RET_OK表示成功，否则失败。
 */
ret_t network_interface_set_dns(network_interface_t* interface, const char* dns);

/**
 * @method network_interface_set_dhcp
 * 网卡接口设置DNS。
 *
 * @param {network_interface_t *} interface 网卡对象。
 *
 * @return {ret_t} RET_OK表示成功，否则失败。
 */
ret_t network_interface_set_dhcp(network_interface_t* interface);

/**
 * @method network_interface_set_gateway
 * 网卡接口设置DNS。
 *
 * @param {network_interface_t *} interface 网卡对象。
 * @param {char *} gateway 网卡网关。
 * @param {char *} dev 出口网卡设备名。
 *
 * @return {ret_t} RET_OK表示成功，否则失败。
 */
ret_t network_interface_set_gateway(network_interface_t* interface, const char* gateway,
                                    const char* dev);

/**
 * @method network_interface_set_wifi_sta
 * 网卡接口设置WIFI STA模式。
 *
 * @param {network_interface_t *} interface 网卡对象。
 * @param {char *} essid wifi帐号 。 
 * @param {char *} passwd wifi密码。
 * @param {network_interface_wifi_auth_t  } auth wifi认证方式。
 *
 * @return {ret_t} RET_OK表示成功，否则失败。
 */
ret_t network_interface_set_wifi_sta(network_interface_t* interface, const char* essid,
                                     const char* passwd, network_interface_wifi_auth_t auth);

/**
 * @method network_interface_set_wifi_ap
 * 网卡接口设置WIFI AP模式。
 *
 * @param {network_interface_t *} interface 网卡对象。
 * @param {char *} essid wifi帐号 。 
 * @param {char *} passwd wifi密码。
 * @param {network_interface_wifi_auth_t  } auth wifi认证方式。
 * @param {uint8_t } channel wifi通道。
 * @param {network_interface_wifi_freq_t  } freq wifi频段。
 * @param {uint8_t } hidden wifi是否隐藏。
 *
 * @return {ret_t} RET_OK表示成功，否则失败。
 */
ret_t network_interface_set_wifi_ap(network_interface_t* interface, const char* essid,
                                    const char* passwd, network_interface_wifi_auth_t auth,
                                    uint8_t channel, network_interface_wifi_freq_t freq,
                                    uint8_t hidden);

/**
 * @method network_interface_destroy
 * 网卡接口销毁函数。
 *
 * @param {network_interface_t *} interface 网卡对象。
 *
 * @return {void}。
 */
void network_interface_destroy(network_interface_t* interface);

END_C_DECLS
#endif
