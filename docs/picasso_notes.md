## 用 picasso 实现 vgcanvas 遇到的一些问题。

### 颜色格式

SDL 的 Texture 为 SDL\_PIXELFORMAT\_RGBA8888 时，picasso 的 canvas 的格式要使用 COLOR\_FORMAT\_ABGR。

### 矩阵变换

矩阵变换存在两个问题：

* 在生成路径时，picasso 没有立即用矩阵对顶点进行变化，所以如果在生成路径的过程中修改矩阵，会影响之前的路径，这与 HTML5 Canvas 中的行为不符。所以修改了 picasso_api.cpp，用矩阵立即对路径的顶点进行变化。

* picasso 的矩阵与常规做法不同。比如，旋转并不是基于 world 原点而是基于 view 原点。这与 HTML5 Canvas 中的行为不符。所以矩阵变化在外部完成，再同步到 picasso 中。

### 图片格式

在 picasso 的 canvas 格式使用 COLOR\_FORMAT\_ABGR 时，其图片采样格式与 stb 加载的格式相反。所以修改了 gfx\_span\_image\_filters.h，让 RGBA 的图片按固定格式采样。
