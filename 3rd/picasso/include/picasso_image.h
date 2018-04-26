/**
 * \file picasso_image.h
 * \author Zhang Ji Peng <onecoolx@gmail.com>
 * \date 2012/1/31
 *
 * This file includes all interfaces of picasso extended image decoders.
 \verbatim

    Copyright (C) 2008 ~ 2016  Zhang Ji Peng

    All rights reserved.

    Picasso is a vector graphic library.

 \endverbatim
 */

#ifndef _PICASSO_EXT_IMAGE_DECODERS_H_
#define _PICASSO_EXT_IMAGE_DECODERS_H_

#include "picasso.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup extension Extensions
 * @{
 */
/**
 * \defgroup extimg XImage Extension
 * @{
 */
/**
 * \defgroup types XImage Object Types
 * @{
 */

/**
 * \brief A frame image data.
 */
typedef struct _psx_image_frame {
    /** picasso image object wrap */
    ps_image* img;
    /** image data buffer */
    ps_byte* data;
    /** sizeof image data in bytes */
    size_t size;
    /** current frame duration (in milliseconds, 0 ignore) */
    int duration;
}psx_image_frame;

/**
 * \brief A image object.
 */
typedef struct _psx_image {
    /** image width */
    int width;
    /** image height */
    int height;
    /** bytes of a scanline */
    int pitch;
    /** image format */
    ps_color_format format;
    /** image data frames */
    psx_image_frame* frames;
    /** number of frame */
    size_t num_frames; 
}psx_image;

/**
 * \def IMG_OBJ
 * \brief Get first ps_image obj.
 * \sa IMG_OBJ_AT_INDEX 
 */
#define IMG_OBJ(image) IMG_OBJ_AT_INDEX(image, 0)

/**
 * \def IMG_DATA
 * \brief Get first image frame data.
 * \sa IMG_DATA_AT_INDEX 
 */
#define IMG_DATA(image) IMG_DATA_AT_INDEX(image, 0)

/**
 * \def IMG_DATA_SIZE
 * \brief Get first image frame data size.
 * \sa IMG_DATA_SIZE_AT_INDEX
 */
#define IMG_DATA_SIZE(image) IMG_DATA_SIZE_AT_INDEX(image, 0)

/**
 * \def IMG_OBJ_AT_INDEX
 * \brief Get ps_image obj at index.
 * \sa IMG_OBJ
 */
#define IMG_OBJ_AT_INDEX(image, idx) (image->frames[idx].img)

/**
 * \def IMG_DATA_AT_INDEX
 * \brief Get image frame data at index.
 * \sa IMG_DATA
 */
#define IMG_DATA_AT_INDEX(image, idx) (image->frames[idx].data)

/**
 * \def IMG_DATA_SIZE_AT_INDEX
 * \brief Get image frame data size at index.
 * \sa IMG_DATA_SIZE
 */
#define IMG_DATA_SIZE_AT_INDEX(img, idx) (img->frames[idx].size)

/**
 * \def IMG_DURATION_AT_INDEX
 * \brief Get image frame duration at index.
 * \sa IMG_DATA_SIZE, IMG_DATA
 */
#define IMG_DURATION_AT_INDEX(img, idx) (img->frames[idx].duration)

/**
 * \brief Status code return by call APIs.
 */
enum {
    /** Successful no errors. */
    S_OK            = 0,
    /** Invalid params input. */
    S_BAD_PARAMS    = 1,
    /** Not support format or decoder module not found. */
    S_NOT_SUPPORT   = 2,
    /** Not enough memory. */
    S_OUT_OF_MEMORY = 3,
    /** Not initialize system. */
    S_INIT_FAILURE  = 4,
    /** Operation failed by internal errors. */
    S_FAILURE       = 5,
};

/** @} end of extimg types */

/**
 * \defgroup functions XImage Functions
 * @{
 */

/**
 * \fn int psx_image_init(void)
 * \brief Initialze the library and load resources.
 *
 * \return Result code returned.
 *
 * \sa psx_image_shutdown
 */
PEXPORT int PICAPI psx_image_init(void);

/**
 * \fn int psx_image_shutdown(void)
 * \brief Release resources and shutdoen.
 *
 * \return Result code returned.
 *
 * \sa psx_image_init
 */
PEXPORT int PICAPI psx_image_shutdown(void);

/**
 * \fn psx_image* psx_image_create_from_data(ps_byte* data, ps_color_format fmt,
 *                                                   int width, int height, int pitch, int* err_code)
 * \brief Create a new psx_image using a copy of given address in memory.
 *
 * \param data      A pointer to the destination in memory where the drawing is to be rendered.
 *                  The size of this memory block should be at least (pitch * height) bytes.
 * \param fmt       The Pixel format to use for the image.
 * \param width     The width, in pixels, of the required image.
 * \param height    The height, in pixels, of the required image.
 * \param pitch     The number of bytes per row, of the required image.
 * \param err_code  Pointer to a value to receiving the result code. can be NULL. 
 *
 * \return If the function succeeds, the return value is the pointer to a new psx_image object.
 *         If the function fails, the return value is NULL.
 *
 * \sa psx_image_load psx_image_load_from_memory psx_image_destroy
 */
PEXPORT psx_image* PICAPI psx_image_create_from_data(ps_byte* data, ps_color_format fmt,
                                                     int width, int height, int pitch, int* err_code);

/**
 * \fn psx_image* psx_image_load(const char* file_name, int* err_code)
 * \brief Create a new psx_image object and load from file. 
 *
 * \param file_name  The image file path which will be loaded, which is encoded by utf8. 
 * \param err_code   Pointer to a value to receiving the result code. can be NULL. 
 *
 * \return If successs, the return value is the pointer to new psx_image object.
 *         If fails, the return value is NULL, and result will be return by \a err_code.
 *
 * \sa psx_image_destroy psx_image_load_from_memory psx_image_create_from_data
 */
PEXPORT psx_image* PICAPI psx_image_load(const char* file_name, int* err_code);

/**
 * \fn psx_image* psx_image_load_from_memory(const ps_byte* data, size_t length, int* err_code)
 * \brief Create a new psx_image object and load data from memory.
 *
 * \param data       Pointer to data buffer in memeory. 
 * \param length     Data length bytes. 
 * \param err_code   Pointer to a value to receiving the result code. can be NULL. 
 *
 * \return If successs, the return value is the pointer to new psx_image object.
 *         If fails, the return value is NULL, and result will be return by \a err_code.
 *
 * \sa psx_image_destroy psx_image_load
 */
PEXPORT psx_image* PICAPI psx_image_load_from_memory(const ps_byte* data, size_t length, int* err_code);

/**
 * \brief Callback function for saving image data.
 */
typedef int (*image_writer_fn)(void* param, const ps_byte* data, size_t length);

/**
 * \fn int psx_image_save(const psx_image* image, image_writer_fn func, void* param, const char* type, float quality)
 * \brief Encoding psx_image to a gaving format and output it.
 *
 * \param image     Pointer to an psx_image object.
 * \param func      User define saving callback function.
 * \param param     User define saving callback param.
 * \param type      Image type short name. (i.e "png" "jpg" "bmp")
 * \param quality   Image encoding quality. (0.1 ~ 1.0)
 *
 * \return Result code returned.
 *
 * \sa psx_image_save_to_file
 */
PEXPORT int PICAPI psx_image_save(const psx_image* image, image_writer_fn func,
                                             void* param, const char* type, float quality);

/**
 * \fn int psx_image_save_to_file(const psx_image* image, const char* file_name, const char* type, float quality)
 * \brief Encoding psx_image to a gaving format and output to a file.
 *
 * \param image     Pointer to an psx_image object.
 * \param file_name The image file path which will be output, which is encoded by utf8.
 * \param type      Image type short name. (i.e "png" "jpg" "bmp")
 * \param quality   Image encoding quality. (0.1 ~ 1.0)
 *
 * \return Result code returned.
 *
 * \sa psx_image_save
 */
PEXPORT int PICAPI psx_image_save_to_file(const psx_image* image, const char* file_name,
                                                                  const char* type, float quality);

/**
 * \fn int psx_image_destroy(psx_image* image)
 * \brief Destroy the psx_image object and release resources.
 *
 * \param image  Pointer to an existing psx_image object.
 *
 * \return Result code returned.
 *
 * \sa psx_image_load psx_image_load_from_memory
 */
PEXPORT int PICAPI psx_image_destroy(psx_image* image);
/** @} end of extimg functions */
/** @} end of extimg */
/** @} end of extensions */

#ifdef __cplusplus
}
#endif

#endif /*_PICASSO_EXT_IMAGE_DECODERS_H_*/

