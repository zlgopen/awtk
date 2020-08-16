# slide_view/pages 各个页面缺省的焦点控件

正常情况下，一个窗口只能指定一个初始焦点。

对于 pages/slideview 来说，在没有触摸屏的情况下，仅用键盘操作，可能希望每一个页面都有一个初始焦点，此时可以用 pages/slideview 的直接子控件 view 的属性 default\_focused\_child 来该页面的初始焦点控件，default\_focused\_child 的值可以是子控件的类型或名称。

```xml
<window anim_hint="htranslate">
  <slide_view x="0" y="0" w="100%" h="100%" >
    <view x="0" y="0" w="100%" h="100%" 
        children_layout="default(r=0,c=1,s=0,m=0)"
        default_focused_child="list_view">

        <list_view w="100%" h="100%" item_height="80" focused="true" >
        ...
        </list_view>
    </view>
    
    <view x="0" y="0" w="100%" h="100%" 
        children_layout="default(r=0,c=1,s=0,m=0)"
        default_focused_child="list_view">

        <list_view w="100%" h="100%" item_height="80" >
        ...
        </list_view>
    </view>
  </slide_view>
```

> 注意：仍然需用 focused="true"指定窗口的初始焦点，窗口的初始焦点一定要在缺省页面上。 
