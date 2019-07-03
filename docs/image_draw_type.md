# AWTK中的图片显示方式

## 一、显示方式

AWTK提供了多种图片显示方式：

* 居中显示(center)。将图片按原大小显示在目标矩形的中央。
* 缩放显示(scale)。将图片缩放至目标矩形的大小(不保证宽高成比例)。
* 自动缩放显示(scale\_auto)。将图片缩放至目标矩形的宽度或高度(选取最小的比例)，并居中显示。
* 宽度缩放显示(scale\_w)。将图片缩放至目标矩形的宽度，高度按此比例进行缩放，超出不部分不显示。
* 高度缩放显示(scale\_h)。将图片缩放至目标矩形的高度，宽度按此比例进行缩放，超出不部分不显示。
* 平铺显示(repeat)。
* 水平方向平铺显示，垂直方向缩放(repeat\_x)。
* 垂直方向平铺显示，水平方向缩放(repeat\_y)。
* 9宫格显示(patch9)。将图片分成等大小的9块，4个角按原大小显示在目标矩形的4个角，左右上下和中间5块分别缩放显示在对应的目标区域。
* 水平方向3宫格显示，垂直方向居中(patch3\_x)。将图片在水平方向上分成等大小的3块，左右两块按原大小显示在目标矩形的左右，中间一块缩放显示在目标区域中间剩余部分。
* 垂直方向3宫格显示，水平方向居中(patch3\_y)。将图片在垂直方向上分成等大小的3块，上下两块按原大小显示在目标矩形的上下，中间一块缩放显示在目标区域中间剩余部分。
* 水平方向3宫格显示，垂直方向缩放(patch3\_x\_scale\_y)。将图片在水平方向上分成等大小的3块，左右两块按原大小显示在目标矩形的左右，中间一块缩放显示在目标区域中间剩余部分。
* 垂直方向3宫格显示，水平方向缩放(patch3\_y\_scale\_x)。将图片在垂直方向上分成等大小的3块，上下两块按原大小显示在目标矩形的上下，中间一块缩放显示在目标区域中间剩余部分。

## 二、在程序中使用(仅限于image控件)

```
image_set_draw_type(img, IMAGE_DRAW_CENTER);
```

## 三、在XML界面描述文件中使用(仅限于image控件)

```
<window name="main" x="0" y="0" w="320" h="480">
  <image style="border" x="0" y="0" w="50%" h="50%" image="earth" draw_type="center"/>
  <image style="border" x="0" y="50%" w="100%" h="50%" image="earth" draw_type="repeat"/>
</window>
```

## 四、在主题中使用(适用于所有控件的背景)

```
<progress_bar>
  <style name="img_v"  bg_image="slider_v_bg" fg_image="slider_v_fg" text_color="black" 
    bg_image_draw_type="patch3_y" fg_image_draw_type="patch3_y">
    <normal/>
  </style>
</progress_bar>
```

## 五、查看实际效果

images.xml展示了各种绘制方式。

```
./bin/preview_ui demos/assets/raw/ui/images.xml
```

