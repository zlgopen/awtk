[![banner](images/awtk_ecology_banner.jpg)](awtk_ecology_intro.md)

如果您针对某个芯片或平台移植过 AWTK，或者为 AWTK 扩展过炫酷的控件，并希望共享这个适配代码，可以联系我们开源到 GitHub 上，我们愿意在 AWTK 技术上对您提供进一步的支持服务，大家可以一起把这个适配做稳做好，构建 AWTK 良好的生态。

[进一步了解我们的 AWTK 生态共建计划](awtk_ecology_intro.md)



## 索引

- [索引](#索引)
- [操作系统适配](#操作系统适配)
- [MCU芯片适配](#mcu芯片适配)
- [控件与示例](#控件与示例)
- [编程语言绑定](#编程语言绑定)
- [编译构建工具](#编译构建工具)
- [商业应用案例](#商业应用案例)



## 操作系统适配

- #### 桌面端平台

| 操作系统 | 下载地址                                | 作者                     |
| -------- | --------------------------------------- | ------------------------ |
| Windows  | https://github.com/zlgopen/awtk         | [ZLG](http://www.zlg.cn) |
| Linux    | https://github.com/zlgopen/awtk         | [ZLG](http://www.zlg.cn) |
| macOS    | https://github.com/zlgopen/awtk         | [ZLG](http://www.zlg.cn) |

- #### 嵌入式平台

| 操作系统     | 下载地址                                 | 作者                     |
| ------------ | ---------------------------------------- | ------------------------ |
| 嵌入式Linux  | https://github.com/zlgopen/awtk-linux-fb | [ZLG](http://www.zlg.cn) |
| AWorksLP     | 详见[MCU芯片适配](#MCU芯片适配)          | [ZLG](http://www.zlg.cn) |
| RT-Thread    | 详见[MCU芯片适配](#MCU芯片适配)          |                          |
| TencentOS    | 详见[MCU芯片适配](#MCU芯片适配)          |                          |
| FreeRTOS     | 详见[MCU芯片适配](#MCU芯片适配)          |                          |
| Nuttx        | 详见[MCU芯片适配](#MCU芯片适配)          |                          |
| SylixOS      | https://github.com/zlgopen/awtk-sylixos  |                          |
| MS-RTOS      | https://github.com/ms-rtos/awtk          |                          |

- #### 移动端和其他平台

| 操作系统  | 下载地址                                           | 作者                                      |
| --------- | -------------------------------------------------- | ----------------------------------------- |
| Android   | https://github.com/zlgopen/awtk-android            | [ZLG](http://www.zlg.cn)                  |
| iOS       | https://github.com/zlgopen/awtk-ios                | [ZLG](http://www.zlg.cn)                  |
| Web       | https://github.com/zlgopen/awtk-web                | [ZLG](http://www.zlg.cn)                  |
| 海思Linux | https://github.com/zlgopen/awtk-linux-fb/issues/28 | [kent52712](https://github.com/kent52712) |
| VSF平台   | https://github.com/vsfteam/vsf                     | [vsfteam](https://github.com/vsfteam)     |
| Wayland   | https://github.com/zjm1060/awtk-wayland            | [zjm1060](https://github.com/zjm1060)     |



## MCU芯片适配

以下按芯片厂商名字字母排序

- #### Anyka

| 芯片    | 操作系统  | 下载地址   | 作者                                  |
| ------- | --------- | ---------- | ------------------------------------- |
| AK376xD | RT-Thread | 请联系作者 | [Anyka](http://www.anyka.com)【原厂】 |
| AK376xD | Linux     | 请联系作者 | [Anyka](http://www.anyka.com)【原厂】 |

- #### ArtInChip

| 芯片     | 操作系统  | 下载地址                               | 作者                                          |
| -------- | --------- | -------------------------------------- | --------------------------------------------- |
| D21x系列 | Linux     | https://gitee.com/artinchip/d211       | [ArtInChip](http://www.artinchip.com)【原厂】 |
| D21x系列 | RT-Thread | https://gitee.com/artinchip/luban-lite | [ArtInChip](http://www.artinchip.com)【原厂】 |
| D13x系列 | RT-Thread | https://gitee.com/artinchip/luban-lite | [ArtInChip](http://www.artinchip.com)【原厂】 |
| D12x系列 | RT-Thread | https://gitee.com/artinchip/luban-lite | [ArtInChip](http://www.artinchip.com)【原厂】 |

- #### Bestechnic

| 芯片          | 操作系统 | 下载地址   | 作者                              |
| ------------- | -------- | ---------- | --------------------------------- |
| BES2600WM系列 | Nuttx    | 请联系作者 | [立功科技](http://www.zlgmcu.com) |

- #### Espressif

| 芯片  | 操作系统 | 下载地址                                | 作者                                      |
| ----- | -------- | --------------------------------------- | ----------------------------------------- |
| ESP32 | FreeRTOS | https://github.com/jason-mao/esp32-awtk | [jason-mao](https://github.com/jason-mao) |

- #### GigaDevice

| 芯片     | 操作系统 | 下载地址   | 作者                              |
| -------- | -------- | ---------- | --------------------------------- |
| GD32450Z | 无       | 请联系作者 | [立功科技](http://www.zlgmcu.com) |

- #### Ingenic

| 芯片  | 操作系统 | 下载地址   | 作者                                         |
| ----- | -------- | ---------- | -------------------------------------------- |
| M200  | Linux    | 请联系作者 | [Ingenic](http://www.ingenic.com.cn)【原厂】 |
| X1000 | Linux    | 请联系作者 | [Ingenic](http://www.ingenic.com.cn)【原厂】 |
| X1830 | Linux    | 请联系作者 | [Ingenic](http://www.ingenic.com.cn)【原厂】 |

- #### MindMotion

| 芯片         | 操作系统 | 下载地址   | 作者                                               |
| ------------ | -------- | ---------- | -------------------------------------------------- |
| MM32F103RET6 | 无       | 请联系作者 | [MindMotion](http://www.mindmotion.com.cn)【原厂】 |

- #### Nordic

| 芯片     | 操作系统 | 下载地址   | 作者                                              |
| -------- | -------- | ---------- | ------------------------------------------------- |
| NRF52840 | FreeRTOS | 请联系作者 | [jimmywong2003](https://github.com/jimmywong2003) |

- #### NXP

| 芯片            | 操作系统 | 下载地址                                           | 作者                                  |
| --------------- | -------- | -------------------------------------------------- | ------------------------------------- |
| RT1052          | AWorksLP | https://github.com/zlgopen/awtk-aworks-rt1052      | [ZLG](http://www.zlg.cn)              |
| RT1052          | 无       | https://github.com/zlgopen/awtk-easyarm-rt1052-raw | [立功科技](http://www.zlgmcu.com)     |
| RT1052          | FreeRTOS | https://github.com/chinachuli/awtk-imxrt1052-raw   | [褚立](https://github.com/chinachuli) |
| RT1020          | 无       | 请联系作者                                         | [立功科技](http://www.zlgmcu.com)     |
| RT1011          | 无       | 请联系作者                                         | [立功科技](http://www.zlgmcu.com)     |
| RT1176          | FreeRTOS | 请联系作者                                         | [立功科技](http://www.zlgmcu.com)     |
| RT500           | FreeRTOS | 请联系作者                                         | [立功科技](http://www.zlgmcu.com)     |
| i.MX6 Cortex-A7 | AWorksLP | 请联系作者                                         | [ZLG](http://www.zlg.cn)              |
| i.MX6 Cortex-A7 | Linux    | https://github.com/zlgopen/awtk-linux-fb           | [ZLG](http://www.zlg.cn)              |
| i.MX6 Cortex-A9 | Linux    | https://github.com/zlgopen/awtk-linux-fb           | [ZLG](http://www.zlg.cn)              |
| i.MX283         | Linux    | https://github.com/zlgopen/awtk-linux-fb           | [ZLG](http://www.zlg.cn)              |
| LPC54608        | 无       | 请联系作者                                         | [立功科技](http://www.zlgmcu.com)     |
| LPC54018        | 无       | 请联系作者                                         | [立功科技](http://www.zlgmcu.com)     |
| LPC1768         | 无       | https://github.com/zlgopen/awtk-lpc1768-raw        | [ZLG](http://www.zlg.cn)              |

- #### ST

| 芯片      | 操作系统  | 下载地址                                              | 作者                               |
| --------- | --------- | ----------------------------------------------------- | ---------------------------------- |
| STM32F103 | 无        | https://github.com/zlgopen/awtk-stm32f103ze-raw       | [ZLG](http://www.zlg.cn)           |
| STM32F103 | RT-Thread | https://github.com/zlgopen/awtk-stm32f103ze-rtthread  | [ZLG](http://www.zlg.cn)           |
| STM32F103 | TencentOS | https://github.com/zlgopen/awtk-stm32f103ze-tencentos | [ZLG](http://www.zlg.cn)           |
| STM32H743 | TencentOS | https://github.com/zlgopen/awtk-stm32h743iitx-tencentos | [ZLG](http://www.zlg.cn)         |
| STM32H743 | FreeRTOS  | https://github.com/zlgopen/awtk-stm32h743iitx-freertos | [ZLG](http://www.zlg.cn)          |
| STM32F429 | 无        | https://github.com/zlgopen/awtk-stm32f429igtx-raw     | [ZLG](http://www.zlg.cn)           |
| STM32F767 | 无        | https://github.com/zlgopen/awtk-stm32f767igtx-raw     | [ZLG](http://www.zlg.cn)           |
| STM32F769 | FreeRTOS  | https://github.com/wpmyj/awtk-stm32f769-freertos      | [王培明](https://github.com/wpmyj) |

- #### SigmaStar

| 芯片    | 操作系统 | 下载地址   | 作者                                                         |
| ------- | -------- | ---------- | ------------------------------------------------------------ |
| SSD201  | Linux    | 请联系作者 | [SigmaStar](http://www.comake.online/index.php?p=products_list&lanmu=2)【原厂】 |
| SSD202D | Linux    | 请联系作者 | [SigmaStar](http://www.comake.online/index.php?p=products_list&lanmu=2)【原厂】 |
| SSD212  | Linux    | 请联系作者 | [SigmaStar](http://www.comake.online/index.php?p=products_list&lanmu=2)【原厂】 |
| SSD210  | Linux    | 请联系作者 | [SigmaStar](http://www.comake.online/index.php?p=products_list&lanmu=2)【原厂】 |
| SSD222  | Linux    | 请联系作者 | [SigmaStar](http://www.comake.online/index.php?p=products_list&lanmu=2)【原厂】 |
| SSD268G | Linux    | 请联系作者 | [SigmaStar](http://www.comake.online/index.php?p=products_list&lanmu=2)【原厂】 |

> 以上信息如果有误，或需要补充的，请联系 chentan@zlg.cn 修正
>



## 控件与示例

- #### 内置控件示例

| 示例名称  | 下载地址                                | 作者                     |
| --------- | --------------------------------------- | ------------------------ |
| demoui    | https://github.com/zlgopen/awtk         | [ZLG](http://www.zlg.cn) |
| demouiold | https://github.com/zlgopen/awtk         | [ZLG](http://www.zlg.cn) |
| c-demos   | https://github.com/zlgopen/awtk-c-demos | [ZLG](http://www.zlg.cn) |

- #### DEMO应用示例

| 示例名称   | 下载地址                                                | 作者                     |
| ---------- | ------------------------------------------------------- | ------------------------ |
| Hello入门  | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK示例列表获取 | [ZLG](http://www.zlg.cn) |
| 图表演示   | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK示例列表获取 | [ZLG](http://www.zlg.cn) |
| 新风系统   | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK示例列表获取 | [ZLG](http://www.zlg.cn) |
| 音乐播放器 | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK示例列表获取 | [ZLG](http://www.zlg.cn) |
| 智能手表   | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK示例列表获取 | [ZLG](http://www.zlg.cn) |

- #### 基础扩展控件

| 控件名称 | 下载地址                                                  | 作者                     |
| -------- | --------------------------------------------------------- | ------------------------ |
| 二维码   | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |
| 下拉按钮 | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |
| 日期选择 | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |
| 旋转文本 | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |
| 数字显示 | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |
| 蒙板视图 | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |

- #### 图表扩展控件

| 控件名称 | 下载地址                                                  | 作者                     |
| -------- | --------------------------------------------------------- | ------------------------ |
| 饼图     | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |
| 柱状图   | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |
| 曲线图   | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |
| 表格视图 | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |

- #### 多媒体扩展控件

| 控件名称 | 下载地址                                                  | 作者                     |
| -------- | --------------------------------------------------------- | ------------------------ |
| 摄像头   | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |
| 视频动画 | 安装[AWStudio](https://awtk.zlg.cn)后在AWTK插件管理器获取 | [ZLG](http://www.zlg.cn) |

- #### Web扩展控件

| 控件名称 | 下载地址                                         | 作者                     |
| -------- | ------------------------------------------------ | ------------------------ |
| HTML视图 | https://github.com/zlgopen/awtk-widget-html-view | [ZLG](http://www.zlg.cn) |



## 编程语言绑定

- #### C

| 解析引擎 | 下载地址                        | 作者                     |
| -------- | ------------------------------- | ------------------------ |
| C99      | https://github.com/zlgopen/awtk | [ZLG](http://www.zlg.cn) |

- #### C++

| 解析引擎 | 下载地址                            | 作者                     |
| -------- | ----------------------------------- | ------------------------ |
| C++98    | https://github.com/zlgopen/awtk-cpp | [ZLG](http://www.zlg.cn) |

- #### JavaScript

| 解析引擎    | 下载地址                                    | 作者                     |
| ----------- | ------------------------------------------- | ------------------------ |
| JerryScript | https://github.com/zlgopen/awtk-jerryscript | [ZLG](http://www.zlg.cn) |
| QuickJS     | https://github.com/zlgopen/awtk-quickjs     | [ZLG](http://www.zlg.cn) |
| NodeJS      | https://github.com/zlgopen/awtk-nodejs      | [ZLG](http://www.zlg.cn) |

- #### Python

| 解析引擎    | 下载地址                                    | 作者                     |
| ----------- | ------------------------------------------- | ------------------------ |
| Python3     | https://github.com/zlgopen/awtk-python      | [ZLG](http://www.zlg.cn) |

- #### Lua

| 解析引擎 | 下载地址                            | 作者                     |
| -------- | ----------------------------------- | ------------------------ |
| Lua      | https://github.com/zlgopen/awtk-lua | [ZLG](http://www.zlg.cn) |

- #### Java

| 解析引擎 | 下载地址                                | 作者                     |
| -------- | --------------------------------------- | ------------------------ |
| JDK      | https://github.com/zlgopen/awtk-java    | [ZLG](http://www.zlg.cn) |
| MiniJVM  | https://github.com/zlgopen/awtk-minijvm | [ZLG](http://www.zlg.cn) |

- #### Go

| 解析引擎 | 下载地址                             | 作者                     |
| -------- | ------------------------------------ | ------------------------ |
| Go       | https://github.com/zlgopen/awtk-go   | [ZLG](http://www.zlg.cn) |



## 编译构建工具

| 构建工具 | 编译平台 | 下载地址                                    | 作者                                    |
| -------- | -------- | ------------------------------------------- | --------------------------------------- |
| SCons    | PC ALL   | https://github.com/zlgopen/awtk             | [ZLG](http://www.zlg.cn)                |
| SCons    | ARMCC    | https://github.com/zlgopen/awtk-scons-armcc | [ZLG](http://www.zlg.cn)                |
| SCons    | MinGW    | https://github.com/zlgopen/awtk-scons-mingw | [ZLG](http://www.zlg.cn)                |
| CMake    | MSVC     | https://github.com/zlgopen/awtk-msvc        | [ZLG](http://www.zlg.cn)                |
| CMake    | macOS    | https://github.com/oskycar/awtktest_cmake   | [allenyang](https://github.com/oskycar) |
| SCons    | Windows  | https://gitee.com/zlgopen/awtk-scons-cross-compile | [ZLG](http://www.zlg.cn)         |


## 商业应用案例

- #### 智能家居
- #### 智能穿戴
- #### 汽车电子
- #### 医疗电子
- #### 工业控制
