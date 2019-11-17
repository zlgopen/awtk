# 如何让文本滚动起来

在很多时候，特别是在小屏幕的硬件上，控件比较小而要显示的文本比较长。

此时，我们需要在控件获得焦点时/或点击时，让文本滚动起来，以便让用户看到完整的信息。

AWTK 提供了 hscroll\_label\_t 控件，可以轻松满足文本滚动的需求。

## 一、基本用法

示例：

```
<window anim_hint="htranslate" text="hscroll_label" children_layout="default(c=1,h=30,xm=10,s=5)">
  <hscroll_label text="炫酷的 GUI 引擎。" /> 
  <hscroll_label text="炫酷的 GUI 引擎。" style="right"/> 

  <hscroll_label 
    lull="1000"
    loop="true"
    yoyo="true"
    ellipses="true"
    focusable="true"
    text="(always ellipses loop yoyo) 为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。" /> 

  <hscroll_label 
    focusable="true"
    only_focus="true"
    text="(only_focus noloop) 为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。" /> 

  <hscroll_label 
    loop="true"
    ellipses="true"
    focusable="true"
    only_focus="true"
    text="(only_focus ellipses loop) 为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。" /> 

  <hscroll_label 
    style="green"
    loop="true"
    yoyo="true"
    ellipses="true"
    focusable="true"
    only_focus="true"
    text="(only_focus ellipses loop yoyo) 为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。" /> 

</window>

```

## 二、扩展用法

但有时用的不是单纯的文本控件，而是列表项、多选按钮、单选按钮或其它功能的控件，那该怎么办呢？

其实也很简单，把 hscroll\_label 作为该控件的子控件，并指定 only\_parent\_focus 属性为 true 即可。

示例：

```
<window anim_hint="htranslate" move_focus_prev_key="up" move_focus_next_key="down" text="Basic Controls">
  <row x="0" y="180" w="-50" h="90" children_layout="default(r=1,c=2,m=2)">
    <column children_layout="default(r=3,c=1,ym=2,s=10)" >
      <check_button name="r1" focusable="true" >
        <hscroll_label style="green" only_parent_focus="true" x="right" y="middle" w="-30" h="100%"
          text="1.AWTK 为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。" /> 
      </check_button>

      <check_button name="r2" focusable="true">
        <hscroll_label style="green" only_parent_focus="true" x="right" y="middle" w="-30" h="100%"
          text="2.AWTK 为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。" /> 
      </check_button>

      <check_button name="r3" value="true" focusable="true">
        <hscroll_label style="green" only_parent_focus="true" x="right" y="middle" w="-30" h="100%"
          text="3.AWTK 为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。" /> 
      </check_button>
    </column>
  </row>
</window>
```
