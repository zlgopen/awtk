# 如何编译Windows 32位版本

* 1.在文件awtk\_config.py中设置TARGET\_ARCH为'x86'：

将：

```
#TARGET_ARCH='x86'
```

修改为：

```
TARGET_ARCH='x86'
```

* 2.对于XP，如果OpenGL渲染不正常。可以使用AGGE(或BGFX)试试。

将：

```
#NANOVG_BACKEND='AGGE'
```

修改为：

```
NANOVG_BACKEND='AGGE'
```

* 3.编译32位版本需要重新生成资源。

```
scripts/update_res.py clean
scripts/update_res.py all
```

