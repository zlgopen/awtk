# 资源目录变更通知

为了支持多个主题在运行时实时切换，我们对目录结构做了一些调整： 在资源目录下增加主题子目录。

如：

* 旧的目录结果如下：

```
assets
  inc
  raw
```

* 新的目录结果如下：

```
assets
  default
    inc
    raw
```

资源更新脚本也需要做相应改动：

旧的 scripts/update\_res.py

```
common.init(AWTK_ROOT, ASSETS_ROOT, ASSET_C);
common.updateRes()
```

新的 scripts/update\_res.py

```
common.init(AWTK_ROOT, ASSETS_ROOT, 'default', ASSET_C);
common.updateRes()
```
 
