# 支持可独立安装的小应用程序 (applet)

AWTK 应用程序通常是一个单体的应用程序，里面所有的窗口都共享一个资源包（目录），不能单独卸载和安装。但是在有的设备里，AWTK 应用程序又扮演着"系统"的角色，比如在智能手表中，用户可以自己安装喜欢的表盘甚至应用程序，为了方便说明，我们可以独立安装的这类应用程序成为小应用程序 (applet)。

小应用程序 (applet) 的资源必须是独立的，才方便安装和卸载。运行时，先到自己的资源目录中找资源，如果找不到，再到系统中去找。AWTK 最近对此做了支持。具体使用方法如下：

## 1. 使用方法

### 1.1 设置小应用程序 (applet) 资源所在的根目录

**所有的小应用程序 (applet) 的资源都安装到该目录下。**

```c
/**
 * @method assets_managers_set_applet_res_root
 * 设置小应用程序 (applet) 的资源根目录。
 * @param {const char*} res_root 资源根目录。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t assets_managers_set_applet_res_root(const char* res_root);
```

如：
```c
assets_managers_set_applet_res_root("applets/");
```

### 1.2 为窗口指定 applet_name 属性

为窗口指定 applet\_name 属性后，窗口优先到对应的 applet 中查找引用的资源，如果找不到就到系统中去查找。

> 在开发小程序应用 (applet)，其中所有窗口都必须指定同一个 applet 名称。

```c
  /** 
   * @property {char*} applet_name
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 小应用程序 (applet) 的名称。
   *
   * > 如果该窗口属于某个独立的小程序应用 (applet)，需要指定它的名称，以便到对应的资源目录查找资源。
   */
  char* applet_name;
```

示例：

```xml
<window text="foo" anim_hint="htranslate" applet_name="foo">
...
</window>
```

### 1.3 打开小应用程序 (applet) 中的窗口

要进入小应用程序 (applet)，就要打开其中的窗口。在调用 window\_open 时就要指定小应用程序 (applet) 的名称。

格式为：小应用程序 (applet) 名 . 窗口名

示例：

```c
window_open("foo.main");
```

## 2. 小应用程序 (applet) 的管理

对小应用程序 (applet) 的管理，比如安装、卸载和查看，需要应用程序自己处理。

## 3. 完整示例

[完整示例](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/applets.c)