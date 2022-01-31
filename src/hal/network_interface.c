/**
 * File:   network_interface.c
 * Author: AWTK Develop Team
 * Brief: network_interface 
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "network_interface.h"

ret_t network_interface_enable(network_interface_t* interface) {
  return_value_if_fail(interface != NULL && interface->vt != NULL && interface->vt->enable != NULL,
                       RET_FAIL);
  return interface->vt->enable(interface);
}

ret_t network_interface_disable(network_interface_t* interface) {
  return_value_if_fail(interface != NULL && interface->vt != NULL && interface->vt->disable != NULL,
                       RET_FAIL);
  return interface->vt->disable(interface);
}

void network_interface_destroy(network_interface_t* interface) {
  if (interface != NULL && interface->vt != NULL && interface->vt->destroy != NULL)
    interface->vt->destroy(interface);
}

char* network_interface_get_ipaddr(network_interface_t* interface) {
  return_value_if_fail(
      interface != NULL && interface->vt != NULL && interface->vt->get_ipaddr != NULL, NULL);
  return interface->vt->get_ipaddr(interface);
}

char* network_interface_get_macaddr(network_interface_t* interface) {
  return_value_if_fail(
      interface != NULL && interface->vt != NULL && interface->vt->get_macaddr != NULL, NULL);
  return interface->vt->get_macaddr(interface);
}

int network_interface_get_status(network_interface_t* interface) {
  return_value_if_fail(
      interface != NULL && interface->vt != NULL && interface->vt->get_status != NULL, -1);
  return interface->vt->get_status(interface);
}

int network_interface_get_quality(network_interface_t* interface) {
  return_value_if_fail(
      interface != NULL && interface->vt != NULL && interface->vt->get_quality != NULL, -1);
  return interface->vt->get_quality(interface);
}

ret_t network_interface_set_ipaddr(network_interface_t* interface, const char* ipaddr,
                                   const char* netmask) {
  return_value_if_fail(
      interface != NULL && interface->vt != NULL && interface->vt->set_ipaddr != NULL, RET_FAIL);
  return interface->vt->set_ipaddr(interface, ipaddr, netmask);
}

ret_t network_interface_set_dns(network_interface_t* interface, const char* dns) {
  return_value_if_fail(interface != NULL && interface->vt != NULL && interface->vt->set_dns != NULL,
                       RET_FAIL);
  return interface->vt->set_dns(interface, dns);
}

ret_t network_interface_set_gateway(network_interface_t* interface, const char* gateway,
                                    const char* dev) {
  return_value_if_fail(
      interface != NULL && interface->vt != NULL && interface->vt->set_gateway != NULL, RET_FAIL);
  return interface->vt->set_gateway(interface, gateway, dev);
}

ret_t network_interface_set_dhcp(network_interface_t* interface) {
  return_value_if_fail(
      interface != NULL && interface->vt != NULL && interface->vt->set_dhcp != NULL, RET_FAIL);
  return interface->vt->set_dhcp(interface);
}

ret_t network_interface_set_wifi_sta(network_interface_t* interface, const char* essid,
                                     const char* passwd, network_interface_wifi_auth_t auth) {
  return_value_if_fail(
      interface != NULL && interface->vt != NULL && interface->vt->set_wif_sta != NULL, RET_FAIL);
  return_value_if_fail(interface->type == NETWORK_INTERFACE_TYPE_WIFI, RET_FAIL);
  return interface->vt->set_wif_sta(interface, essid, passwd, auth);
}

ret_t network_interface_set_wifi_ap(network_interface_t* interface, const char* essid,
                                    const char* passwd, network_interface_wifi_auth_t auth,
                                    uint8_t channel, network_interface_wifi_freq_t freq,
                                    uint8_t hidden) {
  return_value_if_fail(
      interface != NULL && interface->vt != NULL && interface->vt->set_wifi_ap != NULL, RET_FAIL);
  return_value_if_fail(interface->type == NETWORK_INTERFACE_TYPE_WIFI, RET_FAIL);
  return interface->vt->set_wifi_ap(interface, essid, passwd, auth, channel, freq, hidden);
}
