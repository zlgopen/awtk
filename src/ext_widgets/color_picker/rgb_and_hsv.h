/*
 * Simple-color-conversions
 *
 * Copyright 2016 Jake Besworth
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/*
 * color.h
 *
 * Portable, simple, compact, color conversion library between color spaces, most notibly RGB ->
 * CYM(K) and HSV Formulas derived from "Color Space Conversions" by Adrian Ford and Alan Roberts.
 *
 * Note: RGB values are 0-255 (unsigned char)
 *       CMY(K) values are 0.00-1.00 (0 to 100%) (float)
 *       HSV values are Hue: 0-360 (0 to 360 degrees) (float), Saturation: 0.00-1.00 (0 to 100%)
 * (float), and Value: 0.00-1.00 (0 to 100%) (float)
 *
 */

#ifndef COLOR_
#define COLOR_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Converts RGB values to CMY storing in given addresses
 *
 * @param r         Value of Red 0-255
 * @param g         Value of Green 0-255
 * @param b         Value of Blue 0-255
 * @param c         Address of Cyan
 * @param m         Address of Magenta
 * @param y         Address of Yellow
 *
 * @return void
 */
void convertRGBtoCMY(const unsigned char r, const unsigned char g, const unsigned char b, float* c,
                     float* m, float* y);

/*
 * Converts CMY values to RGB storing in given addresses
 *
 * @param c         Value of Cyan 0.00-1.00
 * @param m         Value of Magenta 0.00-1.00
 * @param y         Value of Yellow 0.00-1.00
 * @param r         Address of Red
 * @param g         Address of Green
 * @param b         Address of Blue
 *
 * @return void
 */
void convertCMYtoRGB(const float c, const float m, const float y, unsigned char* r,
                     unsigned char* g, unsigned char* b);

/*
 * Converts CMY values to CMYK storing in given addresses
 * Note: function overwrites input parameters, maybe can be changed later to accept some consts
 * first
 *
 * @param c         Address of Cyan value 0.00-1.00
 * @param m         Address of Magenta Value 0.00-1.00
 * @param y         Address of Yellow Value 0.00-1.00
 * @param k         Address of Black Value 0.00-1.00
 *
 * @return void
 */
void convertCMYtoCMYK(float* c, float* m, float* y, float* k);

/*
 * Converts CMYK values to CMY storing in given addresses
 * Note: function overwrites input parameters, maybe can be changed later to accept some consts
 * first
 *
 * @param c         Address of Cyan value 0.00-1.00
 * @param m         Address of Magenta Value 0.00-1.00
 * @param y         Address of Yellow Value 0.00-1.00
 * @param k         Value of Black Value 0.00-1.00
 *
 * @return void
 */
void convertCMYKtoCMY(float* c, float* m, float* y, const float k);

/*
 * Converts RGB values to CMYK storing in given addresses
 *
 * @param r         Value of Red 0-255
 * @param g         Value of Green 0-255
 * @param b         Value of Blue 0-255
 * @param c         Address of Cyan value 0.00-1.00
 * @param m         Address of Magenta Value 0.00-1.00
 * @param y         Address of Yellow Value 0.00-1.00
 * @param k         Address of Black Value 0.00-1.00
 *
 * @return void
 */
void convertRGBtoCMYK(const unsigned char r, const unsigned char g, const unsigned char b, float* c,
                      float* m, float* y, float* k);

/*
 * Converts CMYK values to RGB storing in given addresses
 *
 * @param c         Value of Cyan 0.00-1.00
 * @param m         Value of Magenta Value 0.00-1.00
 * @param y         Value of Yellow Value 0.00-1.00
 * @param k         Value of Black Value 0.00-1.00
 * @param r         Address of Red
 * @param g         Address of Green
 * @param b         Address of Blue
 *
 * @return void
 */
void convertCMYKtoRGB(const float c, const float m, const float y, const float k, unsigned char* r,
                      unsigned char* g, unsigned char* b);

/*
 * Converts RGB values to HSV storing in given addresses
 *
 * @param r         Value of Red 0-255
 * @param g         Value of Green 0-255
 * @param b         Value of Blue 0-255
 * @param h         Address of Hue
 * @param s         Address of Saturation
 * @param v         Address of Value
 *
 * @return void
 */
void convertRGBtoHSV(const unsigned char r, const unsigned char g, const unsigned char b, float* h,
                     float* s, float* v);

/*
 * Converts HSV values to RGB storing in given addresses
 *
 * @param h         Value of Hue 0-360
 * @param s         Value of Saturation 0.00-1.00
 * @param v         Value of Value 0.00-1.00
 * @param r         Address of Red
 * @param g         Adderss of Green
 * @param b         Address of Blue
 *
 * @return void
 */
void convertHSVtoRGB(const float h, const float s, const float v, unsigned char* r,
                     unsigned char* g, unsigned char* b);

#ifdef __cplusplus
}
#endif

#endif
