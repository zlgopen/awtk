# 多主题实时切换

AWTK 支持多主题，并在不重启的情况下，实时切换到不同的主题。

## 1. 资源目录结构

在 assets 目录下再分主题目录，下面的例子有 default 和 dark 两个主题：

```
assets
├── dark
│   ├── inc 
│   │   ├── data
│   │   ├── fonts
│   │   ├── images
│   │   ├── scripts
│   │   ├── strings
│   │   ├── styles
│   │   ├── ui
│   │   └── xml 
│   └── raw 
│       └── styles
└── default
    ├── inc 
    │   ├── data
    │   ├── fonts
    │   ├── images
    │   ├── scripts
    │   ├── strings
    │   ├── styles
    │   ├── ui
    │   └── xml 
    └── raw 
        ├── data
        ├── fonts
        ├── images
        ├── scripts
        ├── strings
        ├── styles
        ├── ui
        └── xml
```

### 注意事项：

* default 主题必须存在，其它主题是可选的。

* default 主题必须具有完整的资源文件，其它主题中只需要放其特有的文件即可。

* 查找资源的顺序为：先在当前主题中查找资源，如果找不到，就去 default 主题去查找。这样可以最大程度上共享资源数据。

* 目前只有支持文件系统的平台才支持多主题实时切换。

## 2. 资源生成

如果需要支持多主题实时切换，请参考下面的例子，去修改 update_res.py 资源生成脚本：

```
themes = ['default', 'dark']

def run():
    for theme in themes:
        print('========================='+theme+' begin =========================');
        common.init(AWTK_ROOT, ASSETS_ROOT, theme, ASSET_C)
        common.updateRes()
        print('========================='+theme+' end =========================\n');
```

## 3. 相关函数

#### 3.1 在初始时指定主题，请用下面的函数：

```
/**
 * @method assets_manager_set_theme
 * 设置当前的主题。
 *
 * @param {assets_manager_t*} am asset manager 对象。
 * @param {const char*} theme 主题名称。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t assets_manager_set_theme(assets_manager_t* am, const char* theme);

```

### 3.1 运行时动态切换主题，请用下面的函数

```
/**
 * @method widget_set_theme
 * 设置 theme 的名称，用于动态切换主题。名称与当前主题名称相同，则重新加载全部资源。
 *
 * > 目前只支持带有文件系统的平台。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {char*} name 主题的名称。
 *
 * @return {ret_t} 返回 RET_OK 表示成功，否则表示失败。
 */
ret_t widget_set_theme(widget_t* widget, const char* name);
```

## 4. 事件

当主题切换完成后，会通过窗口触发 EVT\_THEME\_CHANGED 事件。

```
  /**
   * @const EVT_THEME_CHANGED
   * 主题变化 (event_t)。
   */
  EVT_THEME_CHANGED,
```

## 5. 其它

如果同一个项目要支持多种不同的大小屏幕，而且屏幕大小差别比较大，无法简单的用 layout 方式来适配时，利用以上的机制，可以把特殊的资源放到不同的主题中，在不同的屏幕大小运行时，切换到对应的主题上，也不失为一种解决问题的方法。

## 6. 参考

[资源目录变更通知](assets_dir_changed.md)
