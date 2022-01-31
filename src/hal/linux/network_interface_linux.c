/**
 * File:   network_interface_linux.c
 * Author: AWTK Develop Team
 * Brief: network_interface for linux
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

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "hal/network_interface.h"

typedef struct _network_interface_linux_t {
  network_interface_t network_interface;
  char* ipaddr;
  char* macaddr;
} network_interface_linux_t;

static ret_t network_interface_linux_enable(network_interface_t* interface) {
  char command[128];
  int ret;
  return_value_if_fail(interface != NULL, RET_FAIL);
  snprintf(command, sizeof(command), "ifconfig %s up", interface->interface_name);
  ret = system(command);
  if (WEXITSTATUS(ret) != 0) return RET_FAIL;
  return RET_OK;
}

static ret_t network_interface_linux_disable(network_interface_t* interface) {
  char command[128];
  int ret;
  return_value_if_fail(interface != NULL, RET_FAIL);
  snprintf(command, sizeof(command), "ifconfig %s down", interface->interface_name);
  ret = system(command);
  if (WEXITSTATUS(ret) != 0) return RET_FAIL;
  return RET_OK;
}

static char* network_interface_linux_get_ipaddr(network_interface_t* interface) {
  char ipstr[16];
  uint8_t ipaddr[4];
  struct ifreq ifr;
  struct sockaddr_in* addr = NULL;
  network_interface_linux_t* linux_network_interface = (network_interface_linux_t*)interface;

  int fd = socket(PF_INET, SOCK_DGRAM, 0);
  return_value_if_fail(fd > 0, NULL);
  strcpy(ifr.ifr_name, interface->interface_name);
  if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
    close(fd);
    return NULL;
  }
  addr = (struct sockaddr_in*)&ifr.ifr_addr;
  *((u_int32_t*)ipaddr) = addr->sin_addr.s_addr;
  snprintf(ipstr, sizeof(ipstr), "%d.%d.%d.%d", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
  close(fd);
  if (linux_network_interface->ipaddr != NULL) TKMEM_FREE(linux_network_interface->ipaddr);
  linux_network_interface->ipaddr = tk_strdup(ipstr);
  return linux_network_interface->ipaddr;
}

static char* network_interface_linux_get_macaddr(network_interface_t* interface) {
  int fd;
  struct ifreq ifr;
  uint8_t m[6];
  char macstr[18];
  network_interface_linux_t* linux_network_interface = (network_interface_linux_t*)interface;

  fd = socket(PF_INET, SOCK_DGRAM, 0);
  return_value_if_fail(fd > 0, NULL);
  strcpy(ifr.ifr_name, interface->interface_name);
  if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
    close(fd);
    return NULL;
  }
  memcpy(m, ifr.ifr_hwaddr.sa_data, 6);
  snprintf(macstr, sizeof(macstr), "%02x:%02x:%02x:%02x:%02x:%02x", m[0], m[1], m[2], m[3], m[4],
           m[5]);
  close(fd);
  if (linux_network_interface->macaddr != NULL) TKMEM_FREE(linux_network_interface->macaddr);
  linux_network_interface->macaddr = tk_strdup(macstr);
  return linux_network_interface->macaddr;
}

static int network_interface_linux_eth_get_status(network_interface_t* interface) {
  char carrier_path[128];
  char carrier;
  int fd;
  snprintf(carrier_path, sizeof(carrier_path), "/sys/class/net/%s/carrier",
           interface->interface_name);
  fd = open(carrier_path, O_RDONLY);
  return_value_if_fail(fd > 0, -1);
  if (read(fd, &carrier, 1) <= 0) {
    close(fd);
    return 0;
  }
  close(fd);
  if (carrier == '1')
    return 1;
  else
    return 0;
}

static int network_interface_linux_eth_get_quality(network_interface_t* interface) {
  char speed_path[128];
  int fd;
  char speed[12];
  int val;
  snprintf(speed_path, sizeof(speed_path), "/sys/class/net/%s/speed", interface->interface_name);
  fd = open(speed_path, O_RDONLY);
  return_value_if_fail(fd > 0, -1);
  if (read(fd, speed, sizeof(speed)) <= 0) {
    close(fd);
    return -1;
  }
  close(fd);
  val = strtol(speed, NULL, 10);
  return val;
}

static ret_t network_interface_linux_set_ipaddr(network_interface_t* interface, const char* ipaddr,
                                                const char* netmask) {
  char command[128];
  int ret;
  return_value_if_fail(interface != NULL, RET_FAIL);
  snprintf(command, sizeof(command), "ifconfig %s  %s netmask %s", interface->interface_name,
           ipaddr, netmask);
  ret = system(command);
  if (WEXITSTATUS(ret) != 0) return RET_FAIL;
  return RET_OK;
}

static ret_t network_interface_linux_set_dns(network_interface_t* interface, const char* dns) {
  char command[128];
  int ret;
  return_value_if_fail(interface != NULL, RET_FAIL);
  snprintf(command, sizeof(command), "echo \"nameserver %s\" > /etc/resolv.conf", dns);
  ret = system(command);
  if (WEXITSTATUS(ret) != 0) return RET_FAIL;
  return RET_OK;
}

static ret_t network_interface_linux_set_dhcp(network_interface_t* interface) {
  char command[128];
  int ret;
  return_value_if_fail(interface != NULL, RET_FAIL);
  snprintf(command, sizeof(command), "udhcpc -i %s &", interface->interface_name);
  ret = system(command);
  if (WEXITSTATUS(ret) != 0) return RET_FAIL;
  return RET_OK;
}

static ret_t network_interface_linux_set_gateway(network_interface_t* interface,
                                                 const char* gateway, const char* dev) {
  return RET_OK;
}

static ret_t network_interface_linux_set_wifi_sta(network_interface_t* interface, const char* essid,
                                                  const char* passwd,
                                                  network_interface_wifi_auth_t auth) {
  return RET_OK;
}

static ret_t network_interface_linux_set_wifi_ap(network_interface_t* interface, const char* essid,
                                                 const char* passwd,
                                                 network_interface_wifi_auth_t auth,
                                                 uint8_t channel,
                                                 network_interface_wifi_freq_t freq,
                                                 uint8_t hidden) {
  return RET_OK;
}

static int network_interface_linux_wifi_get_status(network_interface_t* interface) {
  return 0;
}

static int network_interface_linux_wifi_get_quality(network_interface_t* interface) {
  return 0;
}

static int network_interface_linux_mobile_get_status(network_interface_t* interface) {
  return 0;
}

static int network_interface_linux_mobile_get_quality(network_interface_t* interface) {
  return 0;
}

static void network_interface_linux_destroy(network_interface_t* interface) {
  network_interface_linux_t* linux_network_interface = (network_interface_linux_t*)interface;
  TKMEM_FREE(interface->interface_name);
  TKMEM_FREE(interface);
  if (linux_network_interface->ipaddr != NULL) TKMEM_FREE(linux_network_interface->ipaddr);
  if (linux_network_interface->macaddr != NULL) TKMEM_FREE(linux_network_interface->macaddr);
}

static const network_interface_vtable_t s_linux_eth_vtable = {
    .type = "linux_eth",
    .desc = "vtable for linux eth device",
    .enable = network_interface_linux_enable,
    .disable = network_interface_linux_disable,
    .destroy = network_interface_linux_destroy,
    .get_ipaddr = network_interface_linux_get_ipaddr,
    .get_macaddr = network_interface_linux_get_macaddr,
    .get_quality = network_interface_linux_eth_get_quality,
    .get_status = network_interface_linux_eth_get_status,
    .set_ipaddr = network_interface_linux_set_ipaddr,
    .set_dhcp = network_interface_linux_set_dhcp,
    .set_dns = network_interface_linux_set_dns,
    .set_gateway = network_interface_linux_set_gateway,
};

static const network_interface_vtable_t s_linux_wifi_vtable = {
    .type = "linux_wifi",
    .desc = "vtable for linux wifi device",
    .enable = network_interface_linux_enable,
    .disable = network_interface_linux_disable,
    .destroy = network_interface_linux_destroy,
    .get_ipaddr = network_interface_linux_get_ipaddr,
    .get_macaddr = network_interface_linux_get_macaddr,
    .get_quality = network_interface_linux_wifi_get_quality,
    .get_status = network_interface_linux_wifi_get_status,
    .set_ipaddr = network_interface_linux_set_ipaddr,
    .set_dns = network_interface_linux_set_dns,
    .set_dhcp = network_interface_linux_set_dhcp,
    .set_gateway = network_interface_linux_set_gateway,
    .set_wif_sta = network_interface_linux_set_wifi_sta,
    .set_wifi_ap = network_interface_linux_set_wifi_ap,
};

static const network_interface_vtable_t s_linux_mobile_vtable = {
    .type = "linux_mobile",
    .desc = "vtable for linux mobile device",
    .enable = network_interface_linux_enable,
    .disable = network_interface_linux_disable,
    .destroy = network_interface_linux_destroy,
    .get_quality = network_interface_linux_mobile_get_quality,
    .get_status = network_interface_linux_mobile_get_status,
};

static const network_interface_vtable_t s_linux_unknown_vtable = {
    .type = "linux_unknown_network",
    .desc = "vtable for linux common network",
    .get_ipaddr = network_interface_linux_get_ipaddr,
    .get_macaddr = network_interface_linux_get_macaddr,
};

network_interface_t* network_interface_create(const char* interface,
                                              network_interface_type_t type) {
  network_interface_linux_t* linux_network_interface = TKMEM_ZALLOC(network_interface_linux_t);
  return_value_if_fail(linux_network_interface != NULL, NULL);

  linux_network_interface->network_interface.interface_name = tk_strdup(interface);
  linux_network_interface->network_interface.type = type;
  linux_network_interface->ipaddr = NULL;
  linux_network_interface->macaddr = NULL;

  if (type == NETWORK_INTERFACE_TYPE_ETH)
    linux_network_interface->network_interface.vt = &s_linux_eth_vtable;
  else if (type == NETWORK_INTERFACE_TYPE_WIFI)
    linux_network_interface->network_interface.vt = &s_linux_wifi_vtable;
  else if (type == NETWORK_INTERFACE_TYPE_MOBILE)
    linux_network_interface->network_interface.vt = &s_linux_mobile_vtable;
  else if (type == NETWORK_INTERFACE_TYPE_UNKNOWN)
    linux_network_interface->network_interface.vt = &s_linux_unknown_vtable;
  return (network_interface_t*)linux_network_interface;
}
