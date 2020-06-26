## bitmap\_flag\_t
### 概述
位图标志常量定义。
### 常量
<p id="bitmap_flag_t_consts">

| 名称 | 说明 | 
| -------- | ------- | 
| BITMAP\_FLAG\_NONE | 无特殊标志。 |
| BITMAP\_FLAG\_OPAQUE | 不透明图片。 |
| BITMAP\_FLAG\_IMMUTABLE | 图片内容不会变化。 |
| BITMAP\_FLAG\_TEXTURE | OpenGL Texture, bitmap的id是有效的texture id。 |
| BITMAP\_FLAG\_CHANGED | 如果是MUTABLE的图片，更新时需要设置此标志，底层可能会做特殊处理，比如更新图片到GPU。 |
| BITMAP\_FLAG\_PREMULTI\_ALPHA | 预乘alpha。 |
