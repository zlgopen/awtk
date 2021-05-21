
# 如何使用 stb 外部硬件 YUV 转 RGB

​	AWTK 解码图片是使用 stb 类库来解码的，但是 stb 的内部是软解码的，而且 jpg 一般都是使用 yuv 的颜色空间的，所以在 stb 解码 jpg 的时候，往往需要把 yuv 的颜色空间转化为 rgb 的颜色空间，但是传统的软转换效率会比较低（就算是查表也是比较慢的），所以 awtk 的 stb 提供了一个可以外部接入硬件转换的功能。

### 一，开启 stb 外部硬件 YUV 转 RGB

​	开启该功能，需要定义 HAS_STB_YUV_TO_RGB_G2D 宏，然后需要实现 stbi__YCbCr_to_RGB_g2d 函数，该函数是给用户在外部写硬件转换的代码，可选定义的宏是 STB_YUV_DATA_PIXEL_BTYE，该宏是用于定义 yuv 的数据流的字节数，默认为 4 字节（32 位对齐）。

~~~c
/**
 * @method stbi__YCbCr_to_RGB_g2d
 * stb 外部的硬件 YUV 转换 RGB
 *
 * @annotation ["static"]
 * @param {unsigned char*} out_data 输出的 RGB 数据流。
 * @param {int} out_data_size 输出的 RGB 数据流的长度。
 * @param {unsigned char*} yuv_data 输入的 YUV 的数据流（YUV 的格式为 packed 模式的 YUV444，默认是 4 字节对齐）。
 * @param {int} yuv_data_size 输入的 YUV 的数据流长度。
 * @param {int} w 输入图片的宽
 * @param {int} h 输入图片的高
 * @param {int*} out_channel_order 返回 RGB 数据流的类型（0 代表 R 在低位，1 代表 R 在高位）
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
int stbi__YCbCr_to_RGB_g2d(unsigned char* out_data, int out_data_size, unsigned char* yuv_data, int yuv_data_size, int w, int h, int* out_channel_order);
~~~

