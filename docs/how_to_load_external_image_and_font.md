# 如何加载外部图片和字库

在嵌入式平台下，有些开发板的 Flash 空间比较紧张，运行 AWTK 应用程序的时候，可能会出现 Flash 不够用的情况。此时可以将占用空间比较大的图片和字体资源，放到其他存储介质上（如：SD卡）。

在 AWTK 中，存放图片或者字体都有默认的目录结构，如：图片存放在 default/images/xx 目录下，字体存放在 default/fonts 目录下。如果需要加载其他目录的图片或者字体，AWTK 也提供了相应的方法，下面介绍如何在有文件系统和无文件系统时，加载外部图片和字体。

## 1 有文件系统

有文件系统时，如果需要加载非 AWTK 默认目录结构中的图片或字体，可采用"file:// + 图片或字体所在路径"的形式，具体内容请看下文。

### 1.1 加载外部图片

例如，要在 home_page 页面显示 E:/designer.png 和 awtk.png（目录结构如下）两张图片。

```bash
Demo
  |-- design/
  |-- res/
  |-- my-res/awtk.png
```

有下面两种实现方式：

（1）XML文件方式，修改XML文件内容如下：

```xml
<window name="home_page ">
  <image name="image1" x="0" y="0" w="50" h="50" draw_type="default" image="file://E:/designer.png"/>
  <image name="image" x="0" y="60" w="50" h="50" draw_type="default" image="file://my-res/awtk.png"/>
</window>
```

（2）C代码方式，在C代码中可通过调用 image_set_image() 函数实现，代码如下：

```c
widget_t* image = widget_lookup(win, "image1", TRUE);
image_set_image(image, "file://my-res/awtk.png");
```

### 1.2 加载外部字体

要实现加载外部字体 E:/default.ttf，步骤如下：

步骤一：修改 res/assets/__assets_default.inc 资源文件，将：

```c
assets_manager_preload(am, ASSET_TYPE_FONT, "default");
```

修改为：

```c
assets_manager_preload(am, ASSET_TYPE_FONT, "file://E:/default.ttf");
```

步骤二：调用 system_info_set_default_font 函数设置缺省字体，代码如下：

```c
system_info_set_default_font(system_info(), "file://E:/default.ttf");
```

## 2 无文件系统

在无文件系统中，如果需要加载非 AWTK 默认目录结构中的图片或字体，可以将图片或字体数据作为参数传给 assets_manager_add_data() 函数，该函数将这些数据添加到 AWTK 资源管理器中，然后就可以通过文件名的方式使用图片或字体了。

### 2.1 相关函数

assets_manager_add_data() 函数说明：

- 函数原型：

```c
/**
 * @method assets_manager_add_data
 * 向资源管理器中增加一个资源data。
 * 备注：同一份资源多次调用会出现缓存叠加的问题，导致内存泄露
 * @param {assets_manager_t*} am asset manager对象。
 * @param {const char*} name 待增加的资源的名字。
 * @param {uint16_t} type 待增加的资源的主类型枚举。
 * @param {uint16_t} subtype 待增加的资源的子类型枚举。
 * @param {uint8_t*} buff 待增加的资源的data数据。
 * @param {uint32_t} size 待增加的资源的data数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t assets_manager_add_data(assets_manager_t* am, const char* name, uint16_t type, 
                              uint16_t subtype, uint8_t* buff, uint32_t size);
```

type参数资源主类型取值详见下表：

| 资源主类型           | 说明           |
| -------------------- | -------------- |
| ASSET_TYPE_NONE      | 无效资源       |
| **ASSET_TYPE_FONT**  | **字体资源**   |
| **ASSET_TYPE_IMAGE** | **图片资源**   |
| ASSET_TYPE_STYLE     | 窗体样式资源   |
| ASSET_TYPE_UI        | UI数据资源     |
| ASSET_TYPE_XML       | XML数据资源    |
| ASSET_TYPE_STRINGS   | 字符串数据资源 |
| ASSET_TYPE_SCRIPT    | JS等脚本资源   |
| ASSET_TYPE_DATA      | 其它数据资源   |

subtype 参数字体资源子类型详见下表：

| 资源的主类型枚举     | 说明     |
| -------------------- | -------- |
| ASSET_TYPE_FONT_NONE | 无效字体 |
| ASSET_TYPE_FONT_TTF  | TTF字体  |
| ASSET_TYPE_FONT_BMP  | 位图字体 |

subtype参数图片资源子类型详见下表：

| 图片资源子类型           | 说明              |
| ------------------------ | ----------------- |
| ASSET_TYPE_IMAGE_NONE    | 未知图片类型      |
| ASSET_TYPE_IMAGE_RAW     | Raw图片类型       |
| **ASSET_TYPE_IMAGE_BMP** | **位图图片类型**  |
| **ASSET_TYPE_IMAGE_PNG** | **PNG图片类型**   |
| **ASSET_TYPE_IMAGE_JPG** | **JPG图片类型**   |
| ASSET_TYPE_IMAGE_BSVG    | BSVG图片类型      |
| ASSET_TYPE_IMAGE_GIF     | GIF图片类型       |
| ASSET_TYPE_IMAGE_WEBP    | WEBP图片类型      |
| ASSET_TYPE_IMAGE_LZ4     | LZ4压缩的图片类型 |
| ASSET_TYPE_IMAGE_OTHER   | 其它图片类型      |

### 2.2 加载外部图片

例如，要在 home_page 页面显示 E:/awtk.png 这张图片，实现步骤如下：

（1）添加图片。调用函数 assets_manager_add_data() 向资源管理中添加图片资源，代码如下：

> 注：由于此处是在 PC 上演示，所以调用了 file_read() 函数读取图片数据，在实际的嵌入式应用场景中，需要通过其他方式读取图片数据。

```c
uint32_t size = 0;
uint8_t* data = (uint8_t*)file_read("E:/awtk.png", &size);
assets_manager_add_data(assets_manager(), "awtk",
                            ASSET_TYPE_IMAGE,ASSET_TYPE_IMAGE_PNG, data, size);
```

（2）使用图片。在上面的步骤（1）中，已经向 AWTK 资源管理器中添加好了 awtk.png 图片，在 XML 文件或者 C 代码中可以通过指定图片文件名的方式使用图片。

例如，要在 XML 文件中使用 awtk.png 这张图，代码如下：

```xml
<window name="home_page ">
  <image name="image" x="0" y="60" w="50" h="50" draw_type="default" image="awtk"/>
</window>
```

### 2.3 加载外部字体

例如，要将 E:/default_full.ttf 字体加载到应用程序中，代码如下，实现步骤如下：

步骤一：获取字体数据。（此处为了方便演示调用 file_read() 函数读取字体，也可以用其他方式读取字体）。

步骤二：调用函数 assets_manager_add_data() 函数向资源管理中添加字体资源。

步骤三：使用字体。调用 system_info_set_default_font() 函数设置默认字体为 default_full，应用程序将使用该字体。

```c
uint32_t size = 0;
uint8_t* data = (uint8_t*)file_read("E:/default_full.ttf", &size);
assets_manager_add_data(assets_manager(), "default_full", ASSET_TYPE_FONT, 
                        ASSET_TYPE_FONT_TTF,data, size);
TKMEM_FREE(data);

system_info_set_default_font(system_info(), "default_full");
```
