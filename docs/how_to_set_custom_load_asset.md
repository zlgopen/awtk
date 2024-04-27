# 如何自定义资源加载方式？

## 1 介绍

如果 AWTK 默认的资源加载方式无法满足需求，例如在无文件系统的嵌入式平台上使用外扩 flash 用来存储资源，可以自定义资源的加载方式，只需调用 assets_manager_set_custom_load_asset() 并注册资源加载回调函数即可，AWTK 加载资源时会优先调用用户自定义的加载方式，接口声明如下：

```c
typedef asset_info_t* (*assets_manager_load_asset_t)(void* ctx,          /* 回调函数上下文 */
                                                     asset_type_t type,  /* 资源的类型 */
                                                     uint16_t subtype,   /* 资源的子类型 */
                                                     const char* name);  /* 资源的名称 */

/**
 * @method assets_manager_set_custom_load_asset
 * 设置一个函数，该函数用于实现自定义加载资源。
 *
 * > 如果不支持文件系统，开发者可以设置一个加载资源的回调函数，从flash或其它地方读取资源。
 *
 * @param {assets_manager_t*} am asset manager对象。
 * @param {assets_manager_load_asset_t} custom_load_asset 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_set_custom_load_asset(assets_manager_t* am,
                                           assets_manager_load_asset_t custom_load_asset,
                                           void* ctx);
```

## 2 示例

此处以 STM32F429 平台为例，上面外扩了一块 32MB 的 SPI Flash，在本例子中会使用平台封装好的 W25QXX 库函数对这块 Flash 进行读写。

### 2.1 打包资源

在使用 AWTK Designer 进行界面设计时，修改"项目设置"里"资源打包方式"为"文件+常量"，打包完成后可以在res/assets/default/inc 中看到打包完成的常量文件，文件一般为 .res 或者 .data 后缀，.res 后缀的文件一般为未解码的图片或者字体资源。

### 2.2 写入资源

在运行 AWTK 程序之前，需要先将数据写入外扩 Flash，示例代码如下：

```c
/*
 * 此处以 home_page 界面的 UI 文件为例，将数据写入Flash。
 * 写入协议：前256个字节为名称，中间4个字节为资源大小，最后为资源数组。
*/

#include "res/assets/default/inc/ui/home_page.data"

char *ui_name = "home_page";
u32 ui_size = sizeof(ui_home_page) /* 此处的ui_home_page为home_page.data文件中的资源数组*/

W25QXX_Write((u8*)ui_name,0,sizeof(ui_name));      /* 写入资源名称作为资源的唯一标识符 */
W25QXX_Write((u8*)(&ui_size),0 + 256,sizeof(u32)); /* 写入资源大小用来计算下一资源的偏移量*/
W25QXX_Write((u8*)ui_home_page,0 + 260,ui_size);   /* 写入资源数组 */
```

### 2.3 注册自定义资源加载函数

接下来实现自定义资源加载函数，该函数需要在用户的 AWTK 应用程序中实现，示例代码如下：

```c
#define NAME_MAX_LEN 256
#define U32_SIZE 4
/* 自定义资源加载函数 */
asset_info_t* load_assets_from_flash(void* ctx, asset_type_t type, 
                                     uint16_t subtype, const char* name) {
  u32 assets_size;
  u32 start_position;
  u8 assets_size_char[U32_SIZE];
  u8 assets_name[NAME_MAX_LEN];
  asset_info_t* info = NULL;
  
  start_position = 0;
  /* 该Flash中未写入数据的地方读取后的值为0xFF，以此判断是否已经读取完毕 */
  while (*assets_name != 0xFF) {
     /* 读取资源名称 */
    W25QXX_Read(assets_name, start_position, NAME_MAX_LEN);
    /* 读取资源大小 */
    W25QXX_Read(assets_size_char, start_position + NAME_MAX_LEN, U32_SIZE);
    /* 将资源大小从数组转换为uint32类型 */
    assets_size = assets_size_char[3] << 24 | assets_size_char[2] << 16 | 
                  assets_size_char[1] << 8 | assets_size_char[0];
    if (tk_str_eq(assets_name, name)) {
      info = (asset_info_t*)malloc(assets_size);
      memset(info, 0, assets_size);
      /* 读取资源数组，该数组的类型就为asset_info_t类型 */
      W25QXX_Read((u8*)info, start_position + NAME_MAX_LEN + U32_SIZE, assets_size);
      return info;
    } else {
      start_position = start_position + NAME_MAX_LEN + U32_SIZE + assets_size;
    }
  }

  return info;
}
```

最后在项目application.c代码文件的application_on_launch()中注册该自定义资源加载函数，示例代码如下：

```c
static ret_t application_on_launch(void) {
  assets_manager_set_custom_load_asset(assets_manager(), load_assets_from_flash, NULL);
  return RET_OK;
}
```

