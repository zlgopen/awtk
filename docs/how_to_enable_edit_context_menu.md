# 如何启用编辑器的上下文菜单？

> 目前只支持右键激活，不支持长按。

## 创建菜单界面资源

* 文件名必须为 edit\_menu.xml

* 目前支持的菜单项有：
    * 拷贝。控件名必须为 copy。
    * 剪切。控件名必须为 cut。
    * 粘贴。控件名必须为 paste。
    * 全选。控件名必须为 select\_all。

界面的风格和布局可以自行修改，控件名称满足上述要求即可。

## 示例：

```xml
<popup w="128" h="120" close_when_click="true" children_layout="default(c=1,r=4,m=3,s=2)">
   <button tr_text="Copy" focusable="true" name="copy"/>
   <button tr_text="Cut" focusable="true" name="cut"/>
   <button tr_text="Paste" focusable="true" name="paste"/>
   <button tr_text="Select All" focusable="true" name="select_all"/>
</popup>
```
