/**
 * File:   network_interface_windows.c
 * Author: AWTK Develop Team
 * Brief: network_interface for windows
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
 * 2021-03-15 zhangzhongji<zhangzhongji@zlg.cn> add imp
 *
 */

#include "tkc/darray.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "hal/network_interface.h"

#ifdef MINGW
network_interface_t* network_interface_create(const char* interface_name,
                                              network_interface_type_t type) {
  return NULL;
}
#else
#include <stdio.h>
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <SetupAPI.h>
#include <cfgmgr32.h>
#include <WbemIdl.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "SetupAPI.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "OleAut32.lib")
#pragma comment(lib, "Iphlpapi.lib")

typedef struct _network_interface_windows_t {
  network_interface_t network_interface;
  char* ipaddr;
  char* macaddr;
  /*private*/
  wchar_t* adapter_name;
  wchar_t* friendly_name;
  wchar_t* description_name;
  bool_t is_init_;
  IWbemLocator* p_instance_;
  IWbemServices* p_service_;
  IEnumWbemClassObject* p_enum_;
  IWbemClassObject* p_obj_;
  IWbemClassObject* p_config;
  VARIANT path_;
  int last_error_code_;
  darray_t* arg;
  ULONG64 speed;  //单位M
  int status;
} network_interface_windows_t;

typedef enum _info_type_t {
  LOCAL_IF_IP,
  LOCAL_IF_MAC,
  LOCAL_IF_STATUS,
  LOCAL_IF_SPEED,
  LOCAL_IF_DESC
} info_type_t;

static char* wchar_to_char(const wchar_t* wc) {
  int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
  char* c = (char*)malloc(len + 1);

  if (c == NULL) {
    log_debug("wchar_to_char malloc failed.");
    return NULL;
  }

  WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), c, len, NULL, NULL);
  c[len] = '\0';
  return c;
}

static wchar_t* char_to_wchar(const char* c) {
  int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
  wchar_t* w = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));

  if (w == NULL) {
    log_debug("char_to_wchar malloc failed.");
    return NULL;
  }

  MultiByteToWideChar(CP_ACP, 0, c, strlen(c), w, len);
  w[len] = '\0';
  return w;
}

static ret_t get_ip_adapter_address_by_friendly_name(
    network_interface_windows_t* windows_network_interface, info_type_t info_type) {
  PIP_ADAPTER_ADDRESSES current_addresses = NULL;
  ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
  ULONG family = AF_INET;
  ULONG out_buff_len = 0;
  ret_t ret = RET_FAIL;
  out_buff_len = sizeof(IP_ADAPTER_ADDRESSES);
  PIP_ADAPTER_ADDRESSES adapter_addresses = NULL;
  adapter_addresses = (IP_ADAPTER_ADDRESSES*)malloc(out_buff_len);
  bool_t if_found = FALSE;
  wchar_t* friendly_name = NULL;
  wchar_t* adapter_name = NULL;
  return_value_if_fail(windows_network_interface != NULL, RET_FAIL);

  friendly_name = char_to_wchar(windows_network_interface->network_interface.interface_name);
  if (friendly_name == NULL) {
    return RET_FAIL;
  }

  // Make an initial call to GetAdaptersAddresses to get the
  // size needed into the out_buff_len variable
  if (GetAdaptersAddresses(family, flags, NULL, adapter_addresses, &out_buff_len) ==
      ERROR_BUFFER_OVERFLOW) {
    free(adapter_addresses);
    adapter_addresses = (IP_ADAPTER_ADDRESSES*)malloc(out_buff_len);
  }

  if (adapter_addresses == NULL) {
    log_debug("Memory allocation failed for IP_ADAPTER_ADDRESSES struct.");
    return RET_FAIL;
  }

  if (ERROR_SUCCESS ==
      GetAdaptersAddresses(family, flags, NULL, adapter_addresses, &out_buff_len)) {
    current_addresses = adapter_addresses;
    while (current_addresses) {
      if (wcscmp(current_addresses->FriendlyName, friendly_name) == 0) {
        if_found = TRUE;
        break;
      }

      current_addresses = current_addresses->Next;
    }

    if (if_found) {
      switch (info_type) {
        case LOCAL_IF_STATUS:
          windows_network_interface->status =
              current_addresses->OperStatus == IfOperStatusUp ? 1 : 0;
          break;
        case LOCAL_IF_SPEED:
          windows_network_interface->speed =
              current_addresses->TransmitLinkSpeed / 1000000;  //单位M
          break;
        case LOCAL_IF_DESC:
          if (windows_network_interface->description_name != NULL) {
            TKMEM_FREE(windows_network_interface->description_name)
          }

          windows_network_interface->description_name = tk_wstrdup(current_addresses->Description);

          adapter_name = char_to_wchar(current_addresses->AdapterName);
          if (adapter_name != NULL) {
            if (windows_network_interface->adapter_name != NULL) {
              TKMEM_FREE(windows_network_interface->adapter_name)
            }

            windows_network_interface->adapter_name = tk_wstrdup(adapter_name);
            free(adapter_name);
          }

        default:
          break;
      }

      ret = RET_OK;
    } else {
      ret = RET_FAIL;
    }
  } else {
    ret = RET_FAIL;
  }

  free(adapter_addresses);
  free(friendly_name);

  return ret;
}

static ret_t get_adapter_info_by_dev_desc(network_interface_windows_t* windows_network_interface,
                                          info_type_t info_type) {
  uint8_t m[6];
  char macstr[18];
  ret_t ret = RET_FAIL;
  PIP_ADAPTER_INFO current_ip_adapter_info = NULL;
  PIP_ADAPTER_INFO ip_adapter_info = NULL;
  unsigned long stSize = sizeof(IP_ADAPTER_INFO);
  int nRel = 0;
  bool_t if_found = FALSE;
  return_value_if_fail(
      windows_network_interface != NULL && windows_network_interface->description_name != NULL,
      RET_FAIL);

  ip_adapter_info = (PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
  if (ip_adapter_info == NULL) {
    return RET_FAIL;
  }

  nRel = GetAdaptersInfo(ip_adapter_info, &stSize);
  if (ERROR_BUFFER_OVERFLOW == nRel) {
    free(ip_adapter_info);
    ip_adapter_info = (PIP_ADAPTER_INFO)malloc(stSize);
    nRel = GetAdaptersInfo(ip_adapter_info, &stSize);
  }

  if (ERROR_SUCCESS == nRel) {
    char* dev_name = wchar_to_char(windows_network_interface->description_name);
    current_ip_adapter_info = ip_adapter_info;

    while (current_ip_adapter_info) {
      if (strcmp(current_ip_adapter_info->Description, dev_name) == 0) {
        if_found = TRUE;
        break;
      }

      current_ip_adapter_info = current_ip_adapter_info->Next;
    }

    if (if_found) {
      switch (info_type) {
        case LOCAL_IF_IP:
          if (windows_network_interface->ipaddr != NULL) {
            TKMEM_FREE(windows_network_interface->ipaddr);
          }
          windows_network_interface->ipaddr =
              tk_strdup(current_ip_adapter_info->IpAddressList.IpAddress.String);
          break;
        case LOCAL_IF_MAC:
          if (windows_network_interface->macaddr != NULL) {
            TKMEM_FREE(windows_network_interface->macaddr);
          }

          memcpy(m, current_ip_adapter_info->Address, 6);
          snprintf(macstr, sizeof(macstr), "%02x:%02x:%02x:%02x:%02x:%02x", m[0], m[1], m[2], m[3],
                   m[4], m[5]);
          windows_network_interface->macaddr = tk_strdup(macstr);
          break;
        default:
          break;
      }

      ret = RET_OK;
    } else {
      ret = RET_FAIL;
    }

    free(dev_name);
  } else {
    ret = RET_FAIL;
  }

  //释放内存空间
  if (ip_adapter_info) {
    free(ip_adapter_info);
  }

  return ret;
}

static bool_t NetCardStateChange(const wchar_t* dev_des, bool_t Enabled) {
  //---------------------------------------------------------------------------
  //功能:NetCardStateChange 网卡的启用与禁用
  //参数:NetCardPoint 是 PNetCardStruct 的指针.
  //参数:Enabled   true = 启用   FALSE = 禁用
  //---------------------------------------------------------------------------
  DWORD device_id = 0;
  HDEVINFO dev_info = 0;
  char* dev_name = NULL;
  char dev_des_name[255];
  bool_t is_found = FALSE;

  if (INVALID_HANDLE_VALUE ==
      (dev_info = SetupDiGetClassDevs(NULL, NULL, 0, DIGCF_PRESENT | DIGCF_ALLCLASSES))) {
    log_debug("SetupDiGetClassDevs == INVALID_HANDLE_VALUE.");
    return FALSE;
  }

  SP_DEVINFO_DATA device_info_data = {sizeof(SP_DEVINFO_DATA)};
  DWORD status, problem;

  if (!SetupDiEnumDeviceInfo(dev_info, device_id, &device_info_data)) {
    log_debug("SetupDiEnumDeviceInfo failed.");
    return FALSE;
  }

  dev_name = wchar_to_char(dev_des);
  while (SetupDiEnumDeviceInfo(dev_info, device_id++, &device_info_data)) {
    SetupDiGetDeviceRegistryProperty(dev_info, &device_info_data, SPDRP_DEVICEDESC, NULL,
                                     dev_des_name, 254, NULL);

    if (strcmp(dev_des_name, dev_name) == 0) {
      is_found = TRUE;
      break;
    }
  }

  free(dev_name);
  if (is_found == FALSE) {
    log_debug("dev not found.");
    return FALSE;
  }

  //读取网卡状态
  if (CM_Get_DevNode_Status(&status, &problem, device_info_data.DevInst, 0) != CR_SUCCESS) {
    log_debug("CM_Get_DevNode_Status failed.");
    return FALSE;
  }

  SP_PROPCHANGE_PARAMS prop_change_params = {sizeof(SP_CLASSINSTALL_HEADER)};
  prop_change_params.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
  prop_change_params.Scope = DICS_FLAG_GLOBAL;

  if (Enabled) {
    //如果网卡是启用中的,则不处理
    if (!((status & DN_HAS_PROBLEM) && (CM_PROB_DISABLED == problem))) {
      log_debug("enable dev ok.");
      return TRUE;
    }

    prop_change_params.StateChange = DICS_ENABLE;
  } else {
    if ((status & DN_HAS_PROBLEM) && (CM_PROB_DISABLED == problem)) {
      log_debug("disable dev ok.");
      return TRUE;
    }

    if (!((status & DN_DISABLEABLE) && (CM_PROB_HARDWARE_DISABLED != problem))) {
      log_debug("(!((status & DN_DISABLEABLE) && (CM_PROB_HARDWARE_DISABLED != problem))).");
      return FALSE;
    }

    prop_change_params.StateChange = DICS_DISABLE;
  }

  //功能:设置网卡有效或无效
  if (!SetupDiSetClassInstallParams(dev_info, &device_info_data,
                                    (SP_CLASSINSTALL_HEADER*)&prop_change_params,
                                    sizeof(prop_change_params))) {
    log_debug("SetupDiSetClassInstallParams failed.");
    return FALSE;
  }

  if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, dev_info, &device_info_data)) {
    log_debug("SetupDiCallClassInstaller failed.");
    return FALSE;
  }

  return TRUE;
}

static ret_t network_interface_windows_enable(network_interface_t* network_interface) {
  bool_t ret = FALSE;
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)network_interface;
  return_value_if_fail(
      windows_network_interface != NULL && windows_network_interface->description_name != NULL,
      RET_FAIL);

  if (wcslen(windows_network_interface->description_name) != 0) {
    ret = NetCardStateChange(windows_network_interface->description_name, TRUE);
  }

  return ret ? RET_OK : RET_FAIL;
}

static ret_t network_interface_windows_disable(network_interface_t* network_interface) {
  bool_t ret = FALSE;
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)network_interface;
  return_value_if_fail(
      windows_network_interface != NULL && windows_network_interface->description_name != NULL,
      RET_FAIL);

  if (wcslen(windows_network_interface->description_name) != 0) {
    ret = NetCardStateChange(windows_network_interface->description_name, FALSE);
  }

  return ret ? RET_OK : RET_FAIL;
}

static char* network_interface_windows_get_ipaddr(network_interface_t* network_interface) {
  ret_t ret = RET_FAIL;
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)network_interface;
  return_value_if_fail(windows_network_interface != NULL, NULL);

  ret = get_adapter_info_by_dev_desc(windows_network_interface, LOCAL_IF_IP);
  if (ret == RET_OK) {
    return windows_network_interface->ipaddr;
  } else {
    return NULL;
  }
}

static char* network_interface_windows_get_macaddr(network_interface_t* network_interface) {
  ret_t ret = RET_FAIL;
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)network_interface;
  return_value_if_fail(windows_network_interface != NULL, NULL);

  ret = get_adapter_info_by_dev_desc(windows_network_interface, LOCAL_IF_MAC);
  if (ret == RET_OK) {
    return windows_network_interface->macaddr;
  } else {
    return NULL;
  }
}

static int network_interface_windows_eth_get_status(network_interface_t* network_interface) {
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)network_interface;
  return_value_if_fail(windows_network_interface != NULL, 0);

  if (RET_OK ==
      get_ip_adapter_address_by_friendly_name(windows_network_interface, LOCAL_IF_STATUS)) {
    return windows_network_interface->status;
  } else {
    return 0;
  }
}

static int network_interface_windows_eth_get_quality(network_interface_t* network_interface) {
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)network_interface;
  return_value_if_fail(windows_network_interface != NULL, -1);

  if (RET_OK ==
      get_ip_adapter_address_by_friendly_name(windows_network_interface, LOCAL_IF_SPEED)) {
    return windows_network_interface->speed;
  } else {
    return -1;
  }
}

static bool_t init_com_for_service(network_interface_windows_t* network_interface) {
  wchar_t query_tmp[256] = {0};
  return_value_if_fail(network_interface != NULL, FALSE);
  if (network_interface->is_init_) {
    return TRUE;
  }

  // Step 1: Initialize COM.
  HRESULT hres = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
  if (FAILED(hres)) {
    log_debug("CoInitializeEx failed: %d", hres);
    return FALSE;
  }

  /*
	// Step 2: Set general COM security levels
	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM negotiates service
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
	);

	if (FAILED(hres)) {
    log_debug("CoInitializeSecurity failed: %d", hres);
		return FALSE;
	}
	*/

  // Step 3:  Obtain the initial locator to WMI
  hres = CoCreateInstance(&CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, &IID_IWbemLocator,
                          (LPVOID*)&network_interface->p_instance_);

  if (FAILED(hres)) {
    log_debug("CoCreateInstance failed: %d", hres);
    return FALSE;
  }

  // Step 4: Connect to the local root\cimv2 namespace and obtain pointer pSvc to make IWbemServices calls.
  hres = network_interface->p_instance_->lpVtbl->ConnectServer(
      network_interface->p_instance_, L"ROOT\\CIMV2", NULL, NULL, NULL, 0, NULL, NULL,
      &network_interface->p_service_);

  if (FAILED(hres)) {
    log_debug("ConnectServer failed: %d", hres);
    return FALSE;
  }

  // Step 5:  Set security levels for the proxy
  hres = CoSetProxyBlanket((IUnknown*)network_interface->p_service_,  // Indicates the proxy to set
                           RPC_C_AUTHN_WINNT,                         // RPC_C_AUTHN_xxx
                           RPC_C_AUTHZ_NONE,                          // RPC_C_AUTHZ_xxx
                           NULL,                                      // Server principal name
                           RPC_C_AUTHN_LEVEL_CALL,                    // RPC_C_AUTHN_LEVEL_xxx
                           RPC_C_IMP_LEVEL_IMPERSONATE,               // RPC_C_IMP_LEVEL_xxx
                           NULL,                                      // client identity
                           EOAC_NONE                                  // proxy capabilities
  );

  if (FAILED(hres)) {
    log_debug("CoSetProxyBlanket failed: %d", hres);
    return FALSE;
  }

  // 通过适配器名称来找到指定的适配器对象.
  swprintf_s(query_tmp, 256,
             L"SELECT * FROM Win32_NetworkAdapterConfiguration WHERE SettingID = \"%ls\"",
             network_interface->adapter_name);

  hres = network_interface->p_service_->lpVtbl->ExecQuery(
      network_interface->p_service_,
      //SysAllocString(L"WQL"),
      L"WQL", query_tmp, WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY, NULL,
      &network_interface->p_enum_);

  if (FAILED(hres)) {
    log_debug("ExecQuery failed: %d", hres);
    return FALSE;
  }

  // Get the adapter object.
  ULONG num = 0;
  hres = network_interface->p_enum_->lpVtbl->Next(network_interface->p_enum_, WBEM_INFINITE, 1,
                                                  &network_interface->p_obj_, &num);

  if (FAILED(hres)) {
    log_debug("Next failed: %d", hres);
    return FALSE;
  }

  if (num < 1) {
    log_debug("Next failed num < 1");
    return FALSE;
  }

  VariantInit(&network_interface->path_);
  hres = network_interface->p_obj_->lpVtbl->Get(network_interface->p_obj_, L"__PATH", 0,
                                                &network_interface->path_, NULL, NULL);

  if (FAILED(hres)) {
    log_debug("Get failed: %d", hres);
    return FALSE;
  }

  hres = network_interface->p_service_->lpVtbl->GetObject(network_interface->p_service_,
                                                          L"Win32_NetworkAdapterConfiguration", 0,
                                                          NULL, &network_interface->p_config, NULL);

  if (FAILED(hres)) {
    log_debug("GetObject failed: %d", hres);
    return FALSE;
  }

  network_interface->is_init_ = TRUE;
  return TRUE;
}

static bool_t local_exec_method(network_interface_windows_t* network_interface,
                                const wchar_t* method, IWbemClassObject* params_instance) {
  return_value_if_fail(network_interface != NULL && method != NULL, FALSE);

  bool_t rt = FALSE;
  int last_error_code_ = 0;
  IWbemClassObject* results = NULL;

  auto res = network_interface->p_service_->lpVtbl->ExecMethod(
      network_interface->p_service_, network_interface->path_.bstrVal, method, 0, NULL,
      params_instance, &results, NULL);
  if (SUCCEEDED(res)) {
    VARIANT vtRet;
    VariantInit(&vtRet);
    if (!FAILED(results->lpVtbl->Get(results, L"ReturnValue", 0, &vtRet, NULL, 0))) {
      if (vtRet.uintVal == 0 || vtRet.uintVal == 1) {
        rt = TRUE;
      } else {
        last_error_code_ = vtRet.uintVal;
        log_debug("failed, result:: %d", last_error_code_);
      }
    } else {
      log_debug("ExecMethod Get ReturnValue failed: %d", res);
    }

    VariantClear(&vtRet);
    results->lpVtbl->Release(results);
  } else {
    log_debug("ExecMethod failed: %d", res);
  }

  if (params_instance) {
    params_instance->lpVtbl->Release(params_instance);
  }
  return rt;
}

static ret_t network_interface_windows_set_dhcp(network_interface_t* network_interface) {
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)(network_interface);

  if (!init_com_for_service(windows_network_interface)) {
    return RET_FAIL;
  }

  return local_exec_method(windows_network_interface, L"EnableDHCP", NULL) ? RET_OK : RET_FAIL;
}

static SAFEARRAY* create_SAFEARRAY(darray_t* arg) {
  BSTR ip = NULL;
  return_value_if_fail(arg != NULL && arg->size > 0, NULL);

  SAFEARRAY* psa = SafeArrayCreateVector(VT_BSTR, 0, arg->size);
  long idx[] = {0};

  for (int i = 0; i < arg->size; ++i) {
    wchar_t* tmp = char_to_wchar(darray_get(arg, i));
    if (tmp != NULL) {
      ip = SysAllocString(tmp);
      if (FAILED(SafeArrayPutElement(psa, idx, ip))) {
        SafeArrayDestroy(psa);
        free(tmp);
        log_debug("SafeArrayPutElement failed.");
        return NULL;
      }

      SysFreeString(ip);
      free(tmp);

    } else {
      SafeArrayDestroy(psa);
      log_debug("tmp == NULL.");
      return NULL;
    }
  }

  return psa;
}

static void clear(network_interface_windows_t* network_interface) {
  if (network_interface->p_config) {
    network_interface->p_config->lpVtbl->Release(network_interface->p_config);
    network_interface->p_config = NULL;
  }
  if (network_interface->p_obj_) {
    network_interface->p_obj_->lpVtbl->Release(network_interface->p_obj_);
    network_interface->p_obj_ = NULL;
  }
  if (network_interface->p_enum_) {
    network_interface->p_enum_->lpVtbl->Release(network_interface->p_enum_);
    network_interface->p_enum_ = NULL;
  }
  if (network_interface->p_service_) {
    network_interface->p_service_->lpVtbl->Release(network_interface->p_service_);
    network_interface->p_service_ = NULL;
  }
  if (network_interface->p_instance_) {
    network_interface->p_instance_->lpVtbl->Release(network_interface->p_instance_);
    network_interface->p_instance_ = NULL;
  }
  if (network_interface->is_init_) {
    CoUninitialize();
  }

  network_interface->is_init_ = FALSE;
}

static ret_t network_interface_windows_set_ipaddr(network_interface_t* network_interface,
                                                  const char* ipaddr, const char* netmask) {
  bool_t rt = FALSE;
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)(network_interface);
  if (!init_com_for_service(windows_network_interface)) return rt ? RET_OK : RET_FAIL;

  IWbemClassObject* params = NULL;
  IWbemClassObject* paramsInst = NULL;
  windows_network_interface->p_config->lpVtbl->GetMethod(windows_network_interface->p_config,
                                                         L"EnableStatic", 0, &params, NULL);
  params->lpVtbl->SpawnInstance(params, 0, &paramsInst);

  darray_clear(windows_network_interface->arg);
  darray_push(windows_network_interface->arg, ipaddr);
  SAFEARRAY* p1 = create_SAFEARRAY(windows_network_interface->arg);

  VARIANT paramVt;
  paramVt.vt = VT_ARRAY | VT_BSTR;
  paramVt.parray = p1;
  paramsInst->lpVtbl->Put(paramsInst, L"IPAddress", 0, &paramVt, 0);
  SafeArrayDestroy(p1);

  darray_clear(windows_network_interface->arg);
  darray_push(windows_network_interface->arg, netmask);
  p1 = create_SAFEARRAY(windows_network_interface->arg);
  paramVt.parray = p1;
  paramsInst->lpVtbl->Put(paramsInst, L"SubnetMask", 0, &paramVt, 0);
  SafeArrayDestroy(p1);

  rt = local_exec_method(windows_network_interface, L"EnableStatic", paramsInst);

  if (params) {
    params->lpVtbl->Release(params);
  }

  return rt ? RET_OK : RET_FAIL;
}

static ret_t network_interface_windows_set_dns(network_interface_t* network_interface,
                                               const char* dns) {
  bool_t rt = FALSE;
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)(network_interface);
  if (!init_com_for_service(windows_network_interface)) {
    return rt;
  }

  IWbemClassObject* params = NULL;
  IWbemClassObject* paramsInst = NULL;
  windows_network_interface->p_config->lpVtbl->GetMethod(
      windows_network_interface->p_config, L"SetDNSServerSearchOrder", 0, &params, NULL);
  params->lpVtbl->SpawnInstance(params, 0, &paramsInst);

  darray_clear(windows_network_interface->arg);
  darray_push(windows_network_interface->arg, dns);
  SAFEARRAY* p1 = create_SAFEARRAY(windows_network_interface->arg);
  VARIANT paramVt;
  paramVt.vt = VT_ARRAY | VT_BSTR;
  paramVt.parray = p1;
  paramsInst->lpVtbl->Put(paramsInst, L"DNSServerSearchOrder", 0, &paramVt, 0);
  SafeArrayDestroy(p1);

  rt = local_exec_method(windows_network_interface, L"SetDNSServerSearchOrder", paramsInst);

  if (params) {
    params->lpVtbl->Release(params);
  }

  return rt ? RET_OK : RET_FAIL;
}

static ret_t network_interface_windows_set_gateway(network_interface_t* network_interface,
                                                   const char* gateway, const char* dev) {
  bool_t rt = FALSE;
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)(network_interface);
  if (!init_com_for_service(windows_network_interface)) return rt;

  IWbemClassObject* params = NULL;
  IWbemClassObject* paramsInst = NULL;
  windows_network_interface->p_config->lpVtbl->GetMethod(windows_network_interface->p_config,
                                                         L"SetGateways", 0, &params, NULL);
  params->lpVtbl->SpawnInstance(params, 0, &paramsInst);

  darray_clear(windows_network_interface->arg);
  darray_push(windows_network_interface->arg, gateway);
  SAFEARRAY* p1 = create_SAFEARRAY(windows_network_interface->arg);

  VARIANT paramVt;
  paramVt.vt = VT_ARRAY | VT_BSTR;
  paramVt.parray = p1;
  paramsInst->lpVtbl->Put(paramsInst, L"DefaultIPGateway", 0, &paramVt, 0);
  paramVt.vt = VT_UINT;
  paramVt.uintVal = 1;
  paramsInst->lpVtbl->Put(paramsInst, L"GatewayCostMetric", 0, &paramVt, 0);

  rt = local_exec_method(windows_network_interface, L"SetGateways", paramsInst);
  if (params) {
    params->lpVtbl->Release(params);
  }

  return rt ? RET_OK : RET_FAIL;
}

static ret_t network_interface_windows_set_wifi_sta(network_interface_t* network_interface,
                                                    const char* essid, const char* passwd,
                                                    network_interface_wifi_auth_t auth) {
  return RET_OK;
}

static ret_t network_interface_windows_set_wifi_ap(network_interface_t* network_interface,
                                                   const char* essid, const char* passwd,
                                                   network_interface_wifi_auth_t auth,
                                                   uint8_t channel,
                                                   network_interface_wifi_freq_t freq,
                                                   uint8_t hidden) {
  return RET_OK;
}

static int network_interface_windows_wifi_get_status(network_interface_t* network_interface) {
  return 0;
}

static int network_interface_windows_wifi_get_quality(network_interface_t* network_interface) {
  return 0;
}

static int network_interface_windows_mobile_get_status(network_interface_t* network_interface) {
  return 0;
}

static int network_interface_windows_mobile_get_quality(network_interface_t* network_interface) {
  return 0;
}

static void network_interface_windows_destroy(network_interface_t* network_interface) {
  network_interface_windows_t* windows_network_interface =
      (network_interface_windows_t*)network_interface;
  TKMEM_FREE(network_interface->interface_name);
  TKMEM_FREE(network_interface);
  if (windows_network_interface->ipaddr != NULL) TKMEM_FREE(windows_network_interface->ipaddr);
  if (windows_network_interface->macaddr != NULL) TKMEM_FREE(windows_network_interface->macaddr);
  if (windows_network_interface->description_name != NULL)
    TKMEM_FREE(windows_network_interface->description_name);
  if (windows_network_interface->adapter_name != NULL)
    TKMEM_FREE(windows_network_interface->adapter_name);
  if (windows_network_interface->arg != NULL) darray_destroy(windows_network_interface->arg);
  clear(windows_network_interface);
}

static const network_interface_vtable_t s_windows_eth_vtable = {
    .type = "linux_eth",
    .desc = "vtable for linux eth device",
    .enable = network_interface_windows_enable,
    .disable = network_interface_windows_disable,
    .destroy = network_interface_windows_destroy,
    .get_ipaddr = network_interface_windows_get_ipaddr,
    .get_macaddr = network_interface_windows_get_macaddr,
    .get_quality = network_interface_windows_eth_get_quality,
    .get_status = network_interface_windows_eth_get_status,
    .set_ipaddr = network_interface_windows_set_ipaddr,
    .set_dhcp = network_interface_windows_set_dhcp,
    .set_dns = network_interface_windows_set_dns,
    .set_gateway = network_interface_windows_set_gateway,
};

static const network_interface_vtable_t s_windows_wifi_vtable = {
    .type = "linux_wifi",
    .desc = "vtable for linux wifi device",
    .enable = network_interface_windows_enable,
    .disable = network_interface_windows_disable,
    .destroy = network_interface_windows_destroy,
    .get_ipaddr = network_interface_windows_get_ipaddr,
    .get_macaddr = network_interface_windows_get_macaddr,
    .get_quality = network_interface_windows_wifi_get_quality,
    .get_status = network_interface_windows_wifi_get_status,
    .set_ipaddr = network_interface_windows_set_ipaddr,
    .set_dns = network_interface_windows_set_dns,
    .set_dhcp = network_interface_windows_set_dhcp,
    .set_gateway = network_interface_windows_set_gateway,
    .set_wif_sta = network_interface_windows_set_wifi_sta,
    .set_wifi_ap = network_interface_windows_set_wifi_ap,
};

static const network_interface_vtable_t s_windows_mobile_vtable = {
    .type = "linux_mobile",
    .desc = "vtable for linux mobile device",
    .enable = network_interface_windows_enable,
    .disable = network_interface_windows_disable,
    .destroy = network_interface_windows_destroy,
    .get_quality = network_interface_windows_mobile_get_quality,
    .get_status = network_interface_windows_mobile_get_status,
};

static const network_interface_vtable_t s_windows_unknown_vtable = {
    .type = "linux_unknown_network",
    .desc = "vtable for linux common network",
    .get_ipaddr = network_interface_windows_get_ipaddr,
    .get_macaddr = network_interface_windows_get_macaddr,
};

network_interface_t* network_interface_create(const char* interface_name,
                                              network_interface_type_t type) {
  PIP_ADAPTER_ADDRESSES address = NULL;
  network_interface_windows_t* windows_network_interface =
      TKMEM_ZALLOC(network_interface_windows_t);
  return_value_if_fail(windows_network_interface != NULL, NULL);

  windows_network_interface->network_interface.interface_name = tk_strdup(interface_name);
  windows_network_interface->network_interface.type = type;
  windows_network_interface->ipaddr = NULL;
  windows_network_interface->macaddr = NULL;
  windows_network_interface->friendly_name = NULL;
  windows_network_interface->description_name = NULL;
  windows_network_interface->p_instance_ = NULL;
  windows_network_interface->p_service_ = NULL;
  windows_network_interface->p_enum_ = NULL;
  windows_network_interface->p_obj_ = NULL;
  windows_network_interface->p_config = NULL;
  windows_network_interface->is_init_ = FALSE;
  windows_network_interface->adapter_name = NULL;

  windows_network_interface->arg = darray_create(2, NULL, NULL);

  if (type == NETWORK_INTERFACE_TYPE_ETH)
    windows_network_interface->network_interface.vt = &s_windows_eth_vtable;
  else if (type == NETWORK_INTERFACE_TYPE_WIFI)
    windows_network_interface->network_interface.vt = &s_windows_wifi_vtable;
  else if (type == NETWORK_INTERFACE_TYPE_MOBILE)
    windows_network_interface->network_interface.vt = &s_windows_mobile_vtable;
  else if (type == NETWORK_INTERFACE_TYPE_UNKNOWN)
    windows_network_interface->network_interface.vt = &s_windows_unknown_vtable;

  // 获取接口描述名
  get_ip_adapter_address_by_friendly_name(windows_network_interface, LOCAL_IF_DESC);

  return (network_interface_t*)windows_network_interface;
}
#endif /*WIN*/
