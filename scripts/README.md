# 工具脚本

## 一、update\_res.py 资源生成工具

```
Usage: ./scripts/update_res.py action[clean|all|font|image|ui|style|string] dpi[x1|x2] image_options[bgra+bgr565|rgba]
Example:
./scripts/update_res.py all
./scripts/update_res.py clean
./scripts/update_res.py style
./scripts/update_res.py all x1 bgra+bgr565
```

第一个参数action是必须的，其取值如下：

* clean 清除之前生成的资源文件。
* all 重新生成全部资源。
* font 仅生成有变化的字体资源。
* image 仅生成有变化的图像资源。
* ui 仅生成有变化的UI资源。
* style 仅生成有变化的主题资源。
* string 仅生成有变化的字符串资源。

> 除非你清楚你在做什么，指定第一个参数就可以了。

第二个参数dpi是可选的。用来指定图片的DPI，缺省为x1，即普通屏幕的图片。
> 如果定义了WITH\_FS\_RES(即支持文件系统)，AWTK自动从文件系统中加载对应DPI的图片，此时这个参数没有意义。

第三个参数image\_options是可选的。用来指定生成位图的图片格式，缺省为bgra+bgr565。

> 如果定义了WITH\_FS\_RES(即支持文件系统)或者WITH\_STB\_IMAGE(支持png/jpg)，就不会使用生成的位图，此时这个参数没有意义。




