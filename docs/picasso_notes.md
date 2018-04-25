## 用picasso实现vgcanvas遇到的一些问题。

### 颜色格式

SDL的Texture为SDL\_PIXELFORMAT\_RGBA8888时，picasso的canvas的格式要使用COLOR\_FORMAT\_ABGR。

### 矩阵变换

矩阵变换存在两个问题：

* 在生成路径时，picasso没有立即用矩阵对顶点进行变化，所以如果在生成路径的过程中修改矩阵，会影响之前的路径，这与HTML5 Canvas中的行为不符。所以修改了picasso_api.cpp，用矩阵立即对路径的顶点进行变化。

* picasso的矩阵与常规做法不同。比如，旋转并不是基于world原点而是基于view原点。这与HTML5 Canvas中的行为不符。所以矩阵变化在外部完成，再同步到picasso中。

### 图片格式

在picasso的canvas格式使用COLOR\_FORMAT\_ABGR时，其图片采样格式与stb加载的格式相反。所以修改了gfx\_span\_image\_filters.h，让RGBA的图片按固定格式采样。
