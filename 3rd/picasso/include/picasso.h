/**
 * \file picasso.h
 * \author Zhang Ji Peng <onecoolx@gmail.com>
 * \date 2014/6/16
 *
 * This file includes all interfaces of Picasso
 *
 \verbatim

    Copyright (C) 2008 ~ 2014  Zhang Ji Peng

    All rights reserved.

    Picasso is a vector graphic library.

 \endverbatim
 */

#ifndef _PICASSO_H_
#define _PICASSO_H_

#ifdef DLL_EXPORT
    #if defined(WIN32) || defined(WINCE)
        #ifdef EXPORT
            #define PICAPI  __stdcall
            #define PEXPORT __declspec(dllexport)
        #else
            #define PICAPI  __stdcall
            #define PEXPORT __declspec(dllimport)
        #endif
    #else
        #define PICAPI
        #if (defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))) || (defined(__clang__))
            #define PEXPORT __attribute__((visibility("default")))
        #else
            #define PEXPORT
        #endif
    #endif
#else
#define PICAPI
#define PEXPORT
#endif

#include "pconfig.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup data Data Types 
 * @{
 */

/**
 * \typedef ps_bool
 * \brief boolean type
 * \sa ps_uchar16, ps_schar16, ps_byte
 */
typedef int ps_bool;

/**
 * \def True
 * \brief define true value
 * \sa False
 */
#define True    1

/**
 * \def False
 * \brief define false value
 * \sa True
 */
#define False   0

/**
 * \typedef ps_uchar16
 * \brief 16 bit unsigned unicode character
 * \sa ps_bool, ps_schar16, ps_byte
 */
typedef uint16_t ps_uchar16;

/**
 * \typedef ps_schar16
 * \brief 16 bit signed unicode character
 * \sa ps_bool, ps_uchar16, ps_byte
 */
typedef int16_t ps_schar16;

/**
 * \typedef ps_byte
 * \brief byte data type
 * \sa ps_bool, ps_uchar16, ps_schar16
 */
typedef unsigned char ps_byte;

/** @} end of data types*/

/**
 * \defgroup objects Graphic Object Types 
 * @{
 */

/**
 * \typedef ps_context
 * \brief An opaque type represents a Picasso drawing environment.
 * \sa ps_canvas
 */
typedef struct _ps_context ps_context;

/**
 * \typedef ps_canvas
 * \brief An opaque type represents a pixel buffer.
 * \sa ps_context, ps_image
 */
typedef struct _ps_canvas ps_canvas;

/**
 * \typedef ps_image
 * \brief An opaque type represents an image.
 * \sa ps_canvas, ps_pattern
 */
typedef struct _ps_image ps_image;

/**
 * \typedef ps_pattern
 * \brief An opaque type represents a pattern.
 * \sa ps_canvas, ps_image
 */
typedef struct _ps_pattern ps_pattern;

/**
 * \typedef ps_gradient
 * \brief An opaque type represents a gradient.
 * \sa ps_path, ps_matrix
 */
typedef struct _ps_gradient ps_gradient;

/**
 * \typedef ps_matrix
 * \brief An opaque type represents a transform matrix.
 * \sa ps_path, ps_context
 */
typedef struct _ps_matrix ps_matrix;

/**
 * \typedef ps_path
 * \brief An opaque type represents a graphic path.
 * \sa ps_matrix, ps_context
 */
typedef struct _ps_path ps_path;

/**
 * \typedef ps_mask
 * \brief An opaque type represents an alpha mask.
 * \sa ps_path, ps_image, ps_canvas
 */
typedef struct _ps_mask ps_mask;

/**
 * \typedef ps_font
 * \brief An opaque type represents a font.
 * \sa ps_context
 */
typedef struct _ps_font ps_font;

/**
 * \brief A character glyph of a font.
 */
typedef struct _ps_glyph {
    /** private glyph data. */
    void* glyph;
}ps_glyph;

/**
 * \brief A structure that contains width and height values.
 */
typedef struct _ps_size {
    /** width value. */
    float w;
    /** height value. */
    float h;
}ps_size;

/**
 * \brief A structure that contains location and dimensions of a rectangle.
 */
typedef struct _ps_rect {
    /** locate left of the rectangle. */
    float x;
    /** locate top of the rectangle. */
    float y;
    /** the width of the rectangle. */
    float w;
    /** the height of the rectangle. */
    float h;
}ps_rect;

/**
 * \brief A structure that contains a point in a two-dimensional coordinate system.
 */
typedef struct _ps_point {
    /** the x coordinate of the point. */
    float x;
    /** the y coordinate of the point. */
    float y;
}ps_point;

/**
 * \brief A structure that contains rgba values for a color.
 */
typedef struct _ps_color {
    /** the red component for a color (0 ~ 1). */
    float r;
    /** the green component for a color (0 ~ 1). */
    float g;
    /** the blue component for a color (0 ~ 1). */
    float b;
    /** the alpha component for a color (0 ~ 1). */
    float a;
}ps_color;

/** @} end of graphic object types*/


/**
 * \defgroup common Common Functions 
 * @{
 */

/**
 * \fn int ps_version(void)
 * \brief Return current version of picasso.
 * \sa ps_last_status
 */
PEXPORT int PICAPI ps_version(void);

/**
 * \fn ps_bool ps_initialize(void)
 * \brief Initialize the picasso drawing environment.
 *
 *  It must be the first to be call.
 *
 * \return  True if is initialized, otherwise False.
 *
 * \sa ps_shutdown
 */
PEXPORT ps_bool PICAPI ps_initialize(void);

/**
 * \fn void ps_shutdown(void)
 * \brief Shutdown the picasso drawing environment.
 *
 *  It must be call in the end.
 *
 * \sa ps_initialize
 */
PEXPORT void PICAPI ps_shutdown(void);

/**
 * \brief Status code return by call ps_last_status function.
 */
typedef enum _ps_status {
    /** 
     * The successful status no errors.
     */
    STATUS_SUCCEED,
    /** 
     * Out of memory. 
     */
    STATUS_OUT_OF_MEMORY,
    /** 
     * Invalid argument input. 
     */
    STATUS_INVALID_ARGUMENT,
    /** 
     * Not support this feature. 
     */
    STATUS_NOT_SUPPORT,
    /**
     * Device is not ready.
     */
    STATUS_DEVICE_ERROR,
    /**
     * Mismatching error.
     */
    STATUS_MISMATCHING_FORMAT,
    /** 
     * Unknown error. 
     */
    STATUS_UNKNOWN_ERROR,
}ps_status;

/**
 * \fn ps_status ps_last_status(void)
 * \brief Return the last status code of picasso.
 * \sa ps_version
 */
PEXPORT ps_status PICAPI ps_last_status(void);
/** @} end of common functions*/


/**
 * \defgroup graphic Graphic Functions 
 * @{
 */

/**
 * \defgroup context Context
 * @{
 */

/**
 * \fn ps_context* ps_context_create(ps_canvas* canvas, ps_context* shared_context)
 * \brief Create a new graphic context for a canvas.
 *
 * \param canvas         Pointer to an existing canvas object.
 * \param shared_context Resources shared context, NULL mean resources not shared. 
 *
 * \return If the function succeeds, the return value is the pointer to a new context object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_context_set_canvas, ps_context_get_canvas, ps_context_ref, ps_context_unref
 */
PEXPORT ps_context* PICAPI ps_context_create(ps_canvas* canvas, ps_context* shared_context);

/**
 * \fn ps_context* ps_context_ref(ps_context* ctx)
 * \brief Increases the reference count of the context by 1.
 *
 * \param ctx  Pointer to an existing context object.
 *
 * \return If the function succeeds, the return value is the pointer to the context object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_context_set_canvas, ps_context_get_canvas, ps_context_create, ps_context_unref
 */
PEXPORT ps_context* PICAPI ps_context_ref(ps_context* ctx);

/**
 * \fn void ps_context_unref(ps_context* ctx)
 * \brief Decrements the reference count for the context object. 
 *        If the reference count on the context falls to 0, the context is freed.
 *
 * \param ctx  Pointer to an existing context object.
 *
 * \sa ps_context_set_canvas, ps_context_get_canvas, ps_context_create, ps_context_ref
 */
PEXPORT void PICAPI ps_context_unref(ps_context* ctx);

/**
 * \fn ps_canvas* ps_context_set_canvas(ps_context* ctx, ps_canvas* canvas)
 * \brief Set a new canvas into a context, and return the old one.
 *
 * \param ctx     Pointer to an existing context object.
 * \param canvas  Pointer to an existing canvas object to be set.
 *
 * \return If the function succeeds, the return value is a pointer to the canvas being replaced.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_context_create, ps_context_ref, ps_context_unref, ps_context_get_canvas
 */
PEXPORT ps_canvas* PICAPI ps_context_set_canvas(ps_context* ctx, ps_canvas* canvas);

/**
 * \fn ps_canvas* ps_context_get_canvas(ps_context* ctx)
 * \brief Get the canvas from the context.
 *
 * \param ctx     Pointer to an existing context object.
 *
 * \return If the function succeeds, the return value is a pointer to the canvas.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_context_create, ps_context_ref, ps_context_unref, ps_context_set_canvas
 */
PEXPORT ps_canvas* PICAPI ps_context_get_canvas(ps_context* ctx);

/** @} end of context functions*/

/**
 * \defgroup canvas Canvas
 * @{
 */

/**
 * \brief Pixel formats of canvas.
 */
typedef enum _ps_color_format {
    /** 
     * 32bpp, 8bpc, RGBA color space.
     */
    COLOR_FORMAT_RGBA,
    /** 
     * 32bpp, 8bpc, ARGB color space.
     */
    COLOR_FORMAT_ARGB,
    /** 
     * 32bpp, 8bpc, ABGR color space.
     */
    COLOR_FORMAT_ABGR,
    /** 
     * 32bpp, 8bpc, BGRA color space.
     */
    COLOR_FORMAT_BGRA,
    /** 
     * 24bpp, 8bpc, RGB color space.
     */
    COLOR_FORMAT_RGB,
    /** 
     * 24bpp, 8bpc, BGR color space.
     */
    COLOR_FORMAT_BGR,
    /** 
     * 16bpp, 5/6bpc, RGB color space.
     */
    COLOR_FORMAT_RGB565,
    /** 
     * 16bpp, 5bpc, RGB color space.
     */
    COLOR_FORMAT_RGB555,
    /** 
     * Unsupported color space.
     */
    COLOR_FORMAT_UNKNOWN,
}ps_color_format;

/**
 * \fn ps_canvas* ps_canvas_create(ps_color_format fmt, int width, int height)
 * \brief Create a new canvas using the given parameters.
 *
 * \param fmt     The Pixel format to use for the canvas.
 * \param width   The width, in pixels, of the required canvas.
 * \param height  The height, in pixels, of the required canvas.
 *
 * \return If the function succeeds, the return value is the pointer to a new canvas object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_canvas_create_with_data, ps_canvas_create_compatible, ps_canvas_create_from_canvas,
 *     ps_canvas_create_from_image, ps_canvas_ref, ps_canvas_unref
 */
PEXPORT ps_canvas* PICAPI ps_canvas_create(ps_color_format fmt, int width, int height);

/**
 * \fn ps_canvas* ps_canvas_create_with_data(ps_byte* data, ps_color_format fmt, int width, int height, int pitch)
 * \brief Create a new canvas using a given address in memory.
 *
 * \param data    A pointer to the destination in memory where the drawing is to be rendered. 
 *                   The size of this memory block should be at least (pitch * height) bytes.
 * \param fmt     The Pixel format to use for the canvas.
 * \param width   The width, in pixels, of the required canvas.
 * \param height  The height, in pixels, of the required canvas.
 * \param pitch   The number of bytes per row, of the required canvas.
 *
 * \return If the function succeeds, the return value is the pointer to a new canvas object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_canvas_create, ps_canvas_create_compatible, ps_canvas_create_from_canvas,
 *     ps_canvas_create_from_image, ps_canvas_ref, ps_canvas_unref, ps_canvas_replace_data
 */
PEXPORT ps_canvas* PICAPI ps_canvas_create_with_data(ps_byte* data, ps_color_format fmt,
                                                            int width, int height, int pitch);

/**
 * \fn ps_canvas* ps_canvas_create_compatible(const ps_canvas* canvas, int width, int height)
 * \brief Create a new canvas to compatible with an existing canvas.
 *
 * \param canvas  A pointer to an existing canvas.
 * \param width   The width, in pixels, of the required canvas. 
 *                   If it is not more than zero, the width will be equal to the width of the reference canvas.
 * \param height  The height, in pixels, of the required canvas.
 *                   If it is not more than zero, the height will be equal to the height of the reference canvas.
 *
 * \return If the function succeeds, the return value is the pointer to a new canvas object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_canvas_create, ps_canvas_create_with_data, ps_canvas_create_from_canvas,
 *     ps_canvas_create_from_image, ps_canvas_ref, ps_canvas_unref
 */
PEXPORT ps_canvas* PICAPI ps_canvas_create_compatible(const ps_canvas* canvas,
                                                                int width, int height);

/**
 * \fn ps_canvas* ps_canvas_create_from_canvas(ps_canvas* canvas, const ps_rect* rect)
 * \brief Create a new canvas using part of an existing canvas in same pixel buffer.
 *
 * \param canvas  A pointer to an existing canvas.
 * \param rect    The rectangle area of the canvas from the parent canvas. 
 *                   If it is NULL, the canvas's width and height will be equal to the parant canvas.
 *
 * \return If the function succeeds, the return value is the pointer to a new canvas object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_canvas_create, ps_canvas_create_with_data, ps_canvas_create_compatible,
 *     ps_canvas_create_from_image, ps_canvas_ref, ps_canvas_unref
 */
PEXPORT ps_canvas* PICAPI ps_canvas_create_from_canvas(ps_canvas* canvas, const ps_rect* rect);

/**
 * \fn ps_canvas* ps_canvas_create_from_image(ps_image* img, const ps_rect* rect)
 * \brief Create a new canvas using part of an existing ps_image object in same pixel buffer.
 *
 * \param img     A pointer to an existing ps_image object.
 * \param rect    The rectangle area of the canvas from the ps_image. 
 *                If it is NULL, the canvas's width and height will be equal to ps_image object.
 *
 * \return If the function succeeds, the return value is the pointer to a new canvas object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_canvas_create, ps_canvas_create_with_data, ps_canvas_create_compatible,
 *     ps_canvas_create_from_canvas, ps_canvas_ref, ps_canvas_unref
 */
PEXPORT ps_canvas* PICAPI ps_canvas_create_from_image(ps_image* img, const ps_rect* rect);

/**
 * \fn ps_canvas* PICAPI ps_canvas_replace_data(ps_canvas* canvas, ps_byte* data,
 *                                              ps_color_format fmt, int width, int height, int pitch);
 * \brief Replace a canvas target rendering buffer address in memory.
 *
 * \param canvas  Pointer to an existing canvas object.
 * \param data    A pointer to the destination in memory where the drawing is to be rendered. 
 *                The size of this memory block should be at least (pitch * height) bytes.
 * \param fmt     The Pixel format to use for the canvas.
 * \param width   The width, in pixels, of the required canvas.
 * \param height  The height, in pixels, of the required canvas.
 * \param pitch   The number of bytes per row, of the required canvas.
 *
 * \return If the function succeeds, the return value is the pointer to a new canvas object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_canvas_create_with_data
 */
PEXPORT ps_canvas* PICAPI ps_canvas_replace_data(ps_canvas* canvas, ps_byte* data,
                                                 ps_color_format fmt, int width, int height, int pitch);

/**
 * \fn ps_canvas* ps_canvas_ref(ps_canvas* canvas)
 * \brief Increases the reference count of the canvas by 1.
 *
 * \param canvas  Pointer to an existing canvas object.
 *
 * \return If the function succeeds, the return value is the pointer to the canvas object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_canvas_create, ps_canvas_create_with_data, ps_canvas_create_compatible,
 *        ps_canvas_create_from_canvas, ps_canvas_create_from_image, ps_canvas_unref
 */
PEXPORT ps_canvas* PICAPI ps_canvas_ref(ps_canvas* canvas);

/**
 * \fn void ps_canvas_unref(ps_canvas* canvas)
 * \brief Decrements the reference count for the canvas object. 
 *        If the reference count on the canvas falls to 0, the canvas is freed.
 *
 * \param canvas  Pointer to an existing canvas object.
 *
 * \sa ps_canvas_create, ps_canvas_create_with_data, ps_canvas_create_compatible,
 *        ps_canvas_create_from_canvas, ps_canvas_create_from_image, ps_canvas_ref
 */
PEXPORT void PICAPI ps_canvas_unref(ps_canvas* canvas);

/**
 * \fn ps_size ps_canvas_get_size(const ps_canvas* canvas)
 * \brief Return the size of the canvas.
 *
 * \param canvas  Pointer to an existing canvas object.
 *
 * \return If the function succeeds, the return a valid width and height contain in size struct.
 *         If the function fails, the return value is (0, 0).
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_canvas_get_format
 */
PEXPORT ps_size PICAPI ps_canvas_get_size(const ps_canvas* canvas);

/**
 * \fn ps_color_format ps_canvas_get_format(const ps_canvas* canvas)
 * \brief Return the pixel format of the canvas.
 *
 * \param canvas  Pointer to an existing canvas object.
 *
 * \return If the function succeeds, the return a valid color format.
 *         If the function fails, the return value is COLOR_FORMAT_UNKNOWN.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_canvas_get_size
 */
PEXPORT ps_color_format PICAPI ps_canvas_get_format(const ps_canvas* canvas);

/**
 * \fn void ps_canvas_set_mask(ps_canvas* canvas, const ps_mask* mask)
 * \brief Set a new mask into an existing canvas object.
 *
 * \param canvas  Pointer to an existing canvas object.
 * \param mask    Pointer to an existing mask object to be set. 
 *                 
 * \sa ps_canvas_reset_mask
 */
PEXPORT void PICAPI ps_canvas_set_mask(ps_canvas* canvas, const ps_mask* mask);

/**
 * \fn void ps_canvas_reset_mask(ps_canvas* canvas)
 * \brief Clear the mask from the canvas object.
 *
 * \param canvas  Pointer to an existing canvas object.
 *                 
 * \sa ps_canvas_set_mask
 */
PEXPORT void PICAPI ps_canvas_reset_mask(ps_canvas* canvas);

/**
 * \fn void ps_canvas_bitblt(ps_canvas* src, const ps_rect* rect, 
 *                                     ps_canvas* dst, const ps_point* location)
 * \brief Copy raster data between two canvas objects. 
 *
 * \param src          The pointer to source canvas object.
 * \param rect         The rectangle area will be copied in source canvas, NULL mean the whole area.
 * \param dst          The pointer to destination canvas object.
 * \param location  The location of the start point at destination canvas object.
 *
 * \sa ps_canvas_get_size, ps_canvas_get_format
 */
PEXPORT void PICAPI ps_canvas_bitblt(ps_canvas* src, const ps_rect* rect,
                                                ps_canvas* dst, const ps_point* location);
/** @} end of canvas functions*/

/**
 * \defgroup image Image
 * @{
 */

/**
 * \fn ps_image* ps_image_create(ps_color_format fmt, int width, int height)
 * \brief Create a new image using the given parameters.
 *
 * \param fmt     The Pixel format to use for the image.
 * \param width   The width, in pixels, of the required image.
 * \param height  The height, in pixels, of the required image.
 *
 * \return If the function succeeds, the return value is the pointer to a new image object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_image_create_with_data, ps_image_create_compatible, ps_image_create_from_canvas,
 *     ps_image_create_from_data, ps_image_create_from_image, ps_image_ref, ps_image_unref
 */
PEXPORT ps_image* PICAPI ps_image_create(ps_color_format fmt, int width, int height);

/**
 * \fn ps_image* ps_image_create_with_data(ps_byte* data, ps_color_format fmt, int width, int height, int pitch)
 * \brief Create a new image using a given address in memory.
 *
 * \param data    A pointer to the destination in memory where the drawing is to be rendered. 
 *                   The size of this memory block should be at least (pitch * height) bytes.
 * \param fmt     The Pixel format to use for the image.
 * \param width   The width, in pixels, of the required image.
 * \param height  The height, in pixels, of the required image.
 * \param pitch   The number of bytes per row, of the required image.
 *
 * \return If the function succeeds, the return value is the pointer to a new image object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_image_create, ps_image_create_compatible, ps_image_create_from_canvas,
 *     ps_image_create_from_data, ps_image_create_from_image, ps_image_ref, ps_image_unref
 */
PEXPORT ps_image* PICAPI ps_image_create_with_data(ps_byte* data, ps_color_format fmt,
                                int width, int height, int pitch);
/**
 * \fn ps_image* ps_image_create_from_data(ps_byte* data, ps_color_format fmt, int width, int height, int pitch)
 * \brief Create a new image using a copy of given address in memory.
 *
 * \param data    A pointer to the destination in memory where the drawing is to be rendered. 
 *                   The size of this memory block should be at least (pitch * height) bytes.
 * \param fmt     The Pixel format to use for the image.
 * \param width   The width, in pixels, of the required image.
 * \param height  The height, in pixels, of the required image.
 * \param pitch   The number of bytes per row, of the required image.
 *
 * \return If the function succeeds, the return value is the pointer to a new image object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_image_create, ps_image_create_compatible, ps_image_create_from_canvas,
 *     ps_image_create_with_data, ps_image_create_from_image, ps_image_ref, ps_image_unref
 */
PEXPORT ps_image* PICAPI ps_image_create_from_data(ps_byte* data, ps_color_format fmt,
                                int width, int height, int pitch);
/**
 * \fn ps_image* ps_image_create_compatible(const ps_canvas* canvas, int width, int height)
 * \brief Create a new image to compatible with an existing canvas.
 *
 * \param canvas  A pointer to an existing canvas.
 * \param width   The width, in pixels, of the required canvas. 
 *                   If it is not more than zero, the width will be equal to the width of the reference canvas.
 * \param height  The height, in pixels, of the required canvas.
 *                   If it is not more than zero, the height will be equal to the height of the reference canvas.
 *
 * \return If the function succeeds, the return value is the pointer to a new image object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_image_create, ps_image_create_with_data, ps_image_create_from_canvas,
 *     ps_image_create_from_image, ps_image_ref, ps_image_unref
 */
PEXPORT ps_image* PICAPI ps_image_create_compatible(const ps_canvas* canvas, int width, int height);

/**
 * \fn ps_image* ps_image_create_from_canvas(ps_canvas* canvas, const ps_rect* rect)
 * \brief Create a new image using part of an existing canvas in same pixel buffer.
 *
 * \param canvas  A pointer to an existing canvas.
 * \param rect    The rectangle area of the image from the ps_canvas. 
 *                   If it is NULL, the image's width and height will be equal to the parant canvas.
 *
 * \return If the function succeeds, the return value is the pointer to a new image object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_image_create, ps_image_create_with_data, ps_image_create_compatible,
 *     ps_image_create_from_data, ps_image_create_from_image, ps_image_ref, ps_image_unref
 */
PEXPORT ps_image* PICAPI ps_image_create_from_canvas(ps_canvas* canvas, const ps_rect* rect);

/**
 * \fn ps_image* ps_image_create_from_image(ps_image* img, const ps_rect* rect)
 * \brief Create a new image using part of an existing ps_image object in same pixel buffer.
 *
 * \param img        A pointer to an existing ps_image object.
 * \param rect    The rectangle area of the image from the parent image. 
 *                   If it is NULL, the image's width and height will be equal to parent image object.
 *
 * \return If the function succeeds, the return value is the pointer to a new image object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_image_create, ps_image_create_with_data, ps_image_create_compatible,
 *     ps_image_create_from_data, ps_image_create_from_canvas, ps_image_ref, ps_image_unref
 */
PEXPORT ps_image* PICAPI ps_image_create_from_image(ps_image* img, const ps_rect* rect);

/**
 * \fn ps_image* ps_image_ref(ps_image* img)
 * \brief Increases the reference count of the image by 1.
 *
 * \param img  Pointer to an existing image object.
 *
 * \return If the function succeeds, the return value is the pointer to the image object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_image_create, ps_image_create_with_data, ps_image_create_compatible,
 *        ps_image_create_from_data, ps_image_create_from_canvas, ps_image_create_from_image, ps_image_unref
 */
PEXPORT ps_image* PICAPI ps_image_ref(ps_image* img);

/**
 * \fn void ps_image_unref(ps_image* img)
 * \brief Decrements the reference count for the image object. 
 *        If the reference count on the image falls to 0, the image is freed.
 *
 * \param img  Pointer to an existing image object.
 *
 * \sa ps_image_create, ps_image_create_with_data, ps_image_create_compatible,
 *        ps_image_create_from_data, ps_image_create_from_canvas, ps_image_create_from_image, ps_image_ref
 */
PEXPORT void PICAPI ps_image_unref(ps_image* img);

/**
 * \fn void ps_image_set_allow_transparent(ps_image* img, ps_bool allow)
 * \brief Set whether the image allowed be transparent, False is default.
 *
 * \param img    Pointer to an existing image object.
 * \param allow  Boolean value whether transparent is allowed.
 *
 * \sa ps_image_get_format, ps_image_get_size
 */
PEXPORT void PICAPI ps_image_set_allow_transparent(ps_image* img, ps_bool allow);

/**
 * \fn void ps_image_set_transparent_color(ps_image* img, const ps_color* color)
 * \brief Set the transparent color for the image.
 *
 * \param img    Pointer to an existing image object.
 * \param color  The color for the transparent, NULL for disable. 
 *
 * \sa ps_image_get_format, ps_image_get_size
 */
PEXPORT void PICAPI ps_image_set_transparent_color(ps_image* img, const ps_color* color);

/**
 * \fn ps_size ps_image_get_size(const ps_image* img)
 * \brief Return the size of the image.
 *
 * \param img  Pointer to an existing image object.
 *
 * \return If the function succeeds, the return a valid width and height contain in size struct.
 *         If the function fails, the return value is (0, 0).
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_image_get_format, ps_image_set_allow_transparent
 */
PEXPORT ps_size PICAPI ps_image_get_size(const ps_image* img);

/**
 * \fn ps_color_format ps_image_get_format(const ps_image* img)
 * \brief Return the pixel format of the image.
 *
 * \param img  Pointer to an existing image object.
 *
 * \return If the function succeeds, the return a valid color format.
 *         If the function fails, the return value is COLOR_FORMAT_UNKNOWN.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_image_get_size, ps_image_set_allow_transparent
 */
PEXPORT ps_color_format PICAPI ps_image_get_format(const ps_image* img);

/** @} end of image functions*/

/**
 * \defgroup pattern Pattern
 * @{
 */

/**
 * \brief Fill type of pattern.
 */
typedef enum _ps_wrap_type {
    /** 
     * Wrap repeat, the pattern is repeated.
     */
    WRAP_TYPE_REPEAT,
    /** 
     * Wrap reflect the pattern is reflected.
     */
    WRAP_TYPE_REFLECT,
}ps_wrap_type;

/**
 * \fn ps_pattern* ps_pattern_create_image(const ps_image* img, ps_wrap_type x_wrap, 
 *                                                 ps_wrap_type y_wrap, const ps_matrix* transform)
 * \brief Create a new pattern with an existing image.
 *
 * \param img        An existing image object which will be a tiled of pattern object.
 * \param x_wrap     The horizontal wrap type, of the required pattern.
 * \param y_wrap     The vertical wrap type, of the required pattern.
 * \param transform  A matrix that represents a transform for every tiled. If no transform needed, pass NULL.
 *
 * \return If the function succeeds, the return value is the pointer to a new pattern object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_pattern_ref, ps_pattern_unref
 */
PEXPORT ps_pattern* PICAPI ps_pattern_create_image(const ps_image* img, ps_wrap_type x_wrap,
                                                ps_wrap_type y_wrap, const ps_matrix* transform);

/**
 * \fn void ps_pattern_transform(ps_pattern* pattern, const ps_matrix* matrix)
 * \brief Transform the pattern object.
 * 
 * \param pattern  Pointer to an existing pattern object.
 * \param matrix   Pointer to an existing matrix object.
 *
 * \sa ps_pattern_create_image, ps_pattern_ref, ps_pattern_unref
 */
PEXPORT void PICAPI ps_pattern_transform(ps_pattern* pattern, const ps_matrix* matrix);

/**
 * \fn ps_pattern* ps_pattern_ref(ps_pattern* pattern)
 * \brief Increases the reference count of the pattern by 1.
 *
 * \param pattern  Pointer to an existing pattern object.
 *
 * \return If the function succeeds, the return value is the pointer to the pattern object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_pattern_create_image, ps_pattern_unref
 */
PEXPORT ps_pattern* PICAPI ps_pattern_ref(ps_pattern* pattern);

/**
 * \fn void ps_pattern_unref(ps_pattern* pattern)
 * \brief Decrements the reference count for the pattern object. 
 *        If the reference count on the pattern falls to 0, the pattern is freed.
 *
 * \param pattern  Pointer to an existing pattern object.
 *
 * \sa ps_pattern_create_image, ps_pattern_ref
 */
PEXPORT void PICAPI ps_pattern_unref(ps_pattern* pattern);

/** @} end of pattern functions*/

/**
 * \defgroup gradient Gradient
 * @{
 */

/**
 * \brief Fill type of outside the gradient area.
 */
typedef enum _ps_gradient_spread {
    /** 
     * Spread pad, the area is filled with closest stop color.
     */
    GRADIENT_SPREAD_PAD,
    /** 
     * Spread repeat, the gradient is repeated outside the area.
     */
    GRADIENT_SPREAD_REPEAT,
    /** 
     * Spread reflect, the gradient is reflected outside the area.
     */
    GRADIENT_SPREAD_REFLECT,
}ps_gradient_spread;

/**
 * \fn ps_gradient* ps_gradient_create_linear(ps_gradient_spread spread, 
 *                                                    const ps_point* start, const ps_point* end)
 * \brief Create a new gradient that varies along the line defined by provided starting and ending points.
 *
 * \param spread  The spread type of the gradient.
 * \param start   The starting point , of the required gradient.
 * \param end     The ending point, of the required gradient.
 *
 * \return If the function succeeds, the return value is the pointer to a new gradient object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_gradient_create_radial, ps_gradient_create_conic, ps_gradient_ref, ps_gradient_unref
 */
PEXPORT ps_gradient* PICAPI ps_gradient_create_linear(ps_gradient_spread spread,
                                                    const ps_point* start, const ps_point* end);

/**
 * \fn ps_gradient* ps_gradient_create_radial(ps_gradient_spread spread, 
 *                                    const ps_point* start, float sradius, const ps_point* end, float eradius)
 * \brief Create a new gradient that varies along the area defined by provided starting and ending circles.
 *
 * \param spread  The spread type of the gradient.
 * \param start   The center point of the starting circle, of the required gradient.
 * \param sradius The radius of the starting circle, of the required gradient. 
 * \param end     The center point of the ending circle, of the required gradient.
 * \param eradius The radius of the ending circle, of the required gradient. 
 *
 * \return If the function succeeds, the return value is the pointer to a new gradient object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_gradient_create_linear, ps_gradient_create_conic, ps_gradient_ref, ps_gradient_unref
 */
PEXPORT ps_gradient* PICAPI ps_gradient_create_radial(ps_gradient_spread spread,
                        const ps_point* start, float sradius, const ps_point* end, float eradius);

/**
 * \fn ps_gradient* ps_gradient_create_conic(ps_gradient_spread spread,
 *                                    const ps_point* origin, float sangle)
 * \brief Create a new gradient that varies along the area defined by provided concentric circles.
 *
 * \param spread  The spread type of the gradient.
 * \param origin  The center point of conic, of the required gradient.
 * \param sangle  The angle, int radians, which the first color.
 *
 * \return If the function succeeds, the return value is the pointer to a new gradient object.
 *         If the function fails, the return value is NULL.
 *
 * \note  Conic Gradient support reflect mode only, paramter spread will be ignored.
 *           To get extended error information, call \a ps_last_status.
 *
 * \sa ps_gradient_create_linear, ps_gradient_create_radial, ps_gradient_ref, ps_gradient_unref
 */
PEXPORT ps_gradient* PICAPI ps_gradient_create_conic(ps_gradient_spread spread,
                                    const ps_point* origin, float sangle);

/**
 * \fn void ps_gradient_transform(ps_gradient* gradient, const ps_matrix* matrix)
 * \brief Transform the gradient object.
 * 
 * \param gradient Pointer to an existing gradient object.
 * \param matrix   Pointer to an existing matrix object.
 *
 * \sa ps_gradient_create_linear, ps_gradient_create_radial,
 *     ps_gradient_create_conic, ps_gradient_ref, ps_gradient_unref
 */
PEXPORT void PICAPI ps_gradient_transform(ps_gradient* gradient, const ps_matrix* matrix);

/**
 * \fn ps_gradient* ps_gradient_ref(ps_gradient* gradient)
 * \brief Increases the reference count of the gradient by 1.
 *
 * \param gradient  Pointer to an existing gradient object.
 *
 * \return If the function succeeds, the return value is the pointer to the gradient object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_gradient_create_linear, ps_gradient_create_radial,
 *     ps_gradient_create_conic, ps_gradient_unref
 */
PEXPORT ps_gradient* PICAPI ps_gradient_ref(ps_gradient* gradient);

/**
 * \fn void ps_gradient_unref(ps_gradient* gradient)
 * \brief Decrements the reference count for the gradient object. 
 *        If the reference count on the gradient falls to 0, the gradient is freed.
 *
 * \param gradient  Pointer to an existing gradient object.
 *
 * \sa ps_gradient_create_linear, ps_gradient_create_radial,
 *     ps_gradient_create_conic, ps_gradient_ref
 */
PEXPORT void PICAPI ps_gradient_unref(ps_gradient* gradient);

/**
 * \fn void ps_gradient_add_color_stop(ps_gradient* gradient, float offset, const ps_color* color)
 * \brief Add a color stop to a gradient. The offset specifies the location along the gradient's control vector.
 *
 * \param gradient  Pointer to an existing gradient object.
 * \param offset    An offset position in control vector (range 0 ~ 1).
 * \param color     Color for the stop.
 *
 * \sa ps_gradient_clear_color_stops
 */
PEXPORT void PICAPI ps_gradient_add_color_stop(ps_gradient* gradient,
                                                float offset, const ps_color* color);

/**
 * \fn void ps_gradient_clear_color_stops(ps_gradient* gradient)
 * \brief Clear color stops from gradient's control vector.
 *
 * \param gradient  Pointer to an existing gradient object.
 *
 * \sa ps_gradient_add_color_stop
 */
PEXPORT void PICAPI ps_gradient_clear_color_stops(ps_gradient* gradient);

/** @} end of gradient functions*/

/**
 * \defgroup mask Mask
 * @{
 */

/**
 * \fn ps_mask* ps_mask_create_with_data(ps_byte* data, int width, int height)
 * \brief Create a new mask using a given data block.
 *
 * \param data    A pointer to the mask data block in memory.
 *                   Each byte of data is a aplha value, from transparent to opaque (0 ~ 255). 
 *                   The size of this memory block should be at least (width * height) bytes.
 * \param width   The width, in pixels, of the required mask.
 * \param height  The height, in pixels, of the required mask.
 *
 * \return If the function succeeds, the return value is the pointer to a new mask object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_mask_ref, ps_mask_unref, ps_canvas_set_mask, ps_canvas_reset_mask
 */
PEXPORT ps_mask* PICAPI ps_mask_create_with_data(ps_byte* data, int width, int height);

/**
 * \fn ps_mask* ps_mask_ref(ps_mask* mask)
 * \brief Increases the reference count of the mask by 1.
 *
 * \param mask  Pointer to an existing mask object.
 *
 * \return If the function succeeds, the return value is the pointer to the mask object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_mask_create_with_data, ps_mask_unref, ps_canvas_set_mask, ps_canvas_reset_mask
 */
PEXPORT ps_mask* PICAPI ps_mask_ref(ps_mask* mask);

/**
 * \fn void ps_mask_unref(ps_mask* mask)
 * \brief Decrements the reference count for the mask object. 
 *        If the reference count on the mask falls to 0, the mask is freed.
 *
 * \param mask  Pointer to an existing mask object.
 *
 * \sa ps_mask_create_with_data, ps_mask_ref, ps_canvas_set_mask, ps_canvas_reset_mask
 */
PEXPORT void PICAPI ps_mask_unref(ps_mask* mask);

/**
 * \fn void ps_mask_add_color_filter(ps_mask* mask, const ps_color* color)
 * \brief Add a color filter to a mask.
 *
 * \param mask   Pointer to an existing mask object.
 * \param color  A color for be masked. Only the colors specified by filter can be masked. 
 *                  If no color is specified, all colors can be masked.
 *
 * \sa ps_mask_clear_color_filters
 */
PEXPORT void PICAPI ps_mask_add_color_filter(ps_mask* mask, const ps_color* color);

/**
 * \fn void ps_mask_clear_color_filters(ps_mask* mask)
 * \brief Clear all colors from mask's filter.
 *
 * \param mask  Pointer to an existing mask object.
 *
 * \sa ps_mask_add_color_filter
 */
PEXPORT void PICAPI ps_mask_clear_color_filters(ps_mask* mask);

/** @} end of mask functions*/

/**
 * \defgroup drawing Drawing
 * @{
 */

/**
 * \defgroup fill Fill
 * @{
 */

/**
 * \fn void ps_set_source_color(ps_context* ctx, const ps_color* color)
 * \brief Set a Color to the context, it is used to fill a graphic object.
 *
 * \param ctx     Pointer to an existing context object.
 * \param color   The color to be set.
 *
 * \sa ps_set_source_image, ps_set_source_pattern, ps_set_source_gradient, ps_set_source_canvas
 */
PEXPORT void PICAPI ps_set_source_color(ps_context* ctx, const ps_color* color);

/**
 * \fn void ps_set_source_image(ps_context* ctx, const ps_image* image)
 * \brief Set a image to the context, it is used to fill a graphic object.
 *
 * \param ctx     Pointer to an existing context object.
 * \param image   The image to be set.
 *
 * \note The image's color format must be equal with target canvas, otherwish will 
 *       be failed and \a STATUS_MISMATCHING_FORMAT will be set.
 *
 * \sa ps_set_source_color, ps_set_source_pattern, ps_set_source_gradient, ps_set_source_canvas
 */
PEXPORT void PICAPI ps_set_source_image(ps_context* ctx, const ps_image* image);

/**
 * \fn void ps_set_source_pattern(ps_context* ctx, const ps_pattern* pattern)
 * \brief Set a pattern to the context, it is used to fill a graphic object.
 *
 * \param ctx     Pointer to an existing context object.
 * \param pattern The pattern to be set.
 *
 * \note The pattern's image color format must be equal with target canvas, otherwish will 
 *       be failed and \a STATUS_MISMATCHING_FORMAT will be set.
 *
 * \sa ps_set_source_color, ps_set_source_image, ps_set_source_gradient, ps_set_source_canvas
 */
PEXPORT void PICAPI ps_set_source_pattern(ps_context* ctx, const ps_pattern* pattern);

/**
 * \fn void ps_set_source_gradient(ps_context* ctx, const ps_gradient* gradient)
 * \brief Set a gradient to the context, it is used to fill a graphic object.
 *
 * \param ctx      Pointer to an existing context object.
 * \param gradient The gradient to be set.
 *
 * \sa ps_set_source_color, ps_set_source_image, ps_set_source_pattern, ps_set_source_canvas
 */
PEXPORT void PICAPI ps_set_source_gradient(ps_context* ctx, const ps_gradient* gradient);

/**
 * \fn void ps_set_source_canvas(ps_context* ctx, const ps_canvas* canvas)
 * \brief Set a canvas to the context, it is used to fill a graphic object.
 *
 * \param ctx      Pointer to an existing context object.
 * \param canvas   The canvas to be set.
 *
 * \note The source canvas's color format must be equal with target canvas, otherwish will 
 *       be failed and \a STATUS_MISMATCHING_FORMAT will be set.
 *
 * \sa ps_set_source_color, ps_set_source_image, ps_set_source_pattern, ps_set_source_gradient
 */
PEXPORT void PICAPI ps_set_source_canvas(ps_context* ctx, const ps_canvas* canvas);

/** @} end of fill functions*/

/**
 * \defgroup stroke Stroke
 * @{
 */

/**
 * \brief Styles for rendering the endpoint of a stroked line.
 */
typedef enum _ps_line_cap {
    /**
     * A line with a squared-off end. The line to extend only to the endpoint.(Default)
     */
    LINE_CAP_BUTT,
    /**
     * A line with a rounded end. The line to extend beyond the endpoint.
     */
    LINE_CAP_ROUND,
    /**
     * A line with a squared-off end. The line to extend beyond the endpoint.
     */
    LINE_CAP_SQUARE,
}ps_line_cap;

/**
 * \fn void ps_set_line_cap(ps_context* ctx, ps_line_cap line_cap)
 * \brief Set the style for the endpoint of lines in a graphics context.
 *
 * \param ctx         Pointer to an existing context object.
 * \param line_cap    The cap style of the lines.
 *
 * \sa ps_set_line_join, ps_set_line_width, ps_set_miter_limit, ps_set_stroke_color, 
 *     ps_set_line_dash, ps_reset_line_dash, ps_set_line_inner_join
 */
PEXPORT void PICAPI ps_set_line_cap(ps_context* ctx, ps_line_cap line_cap);

/**
 * \brief Junction types for stroked lines.
 */
typedef enum _ps_line_join {
    /**
     * A join with a sharp corner.(Default)
     */
    LINE_JOIN_MITER,
    /**
     * A join with a sharp corner revert.
     */
    LINE_JOIN_MITER_REVERT,
    /**
     * A join with a miter rounded end.
     */
    LINE_JOIN_MITER_ROUND,
    /**
     * A join with a rounded end.
     */
    LINE_JOIN_ROUND,
    /**
     * A join with a squared-off end.
     */
    LINE_JOIN_BEVEL,
}ps_line_join;

/**
 * \fn void ps_set_line_join(ps_context* ctx, ps_line_join line_join)
 * \brief Set the style for the joins of connected lines in a graphics context.
 *
 * \param ctx         Pointer to an existing context object.
 * \param line_join   The join style of the lines.
 *
 * \sa ps_set_line_cap, ps_set_line_width, ps_set_miter_limit, ps_set_stroke_color, 
 *     ps_set_line_dash, ps_reset_line_dash, ps_set_line_inner_join 
 */
PEXPORT void PICAPI ps_set_line_join(ps_context* ctx, ps_line_join line_join);

/**
 * \brief Inner junction types for stroked lines.
 */
typedef enum _ps_line_inner_join {
    /**
     * A inner join with a sharp corner.(Default)
     */
    LINE_INNER_MITER,
    /**
     * A inner join with a squared-off end.
     */
    LINE_INNER_BEVEL,
    /**
     * A inner join with a jag end.
     */
    LINE_INNER_JAG,
    /**
     * A inner join with a rounded end.
     */
    LINE_INNER_ROUND,
}ps_line_inner_join;

/**
 * \fn void ps_set_line_inner_join(ps_context* ctx, ps_line_inner_join line_inner_join)
 * \brief Set the style for the inner joins of connected lines in a graphics context.
 *
 * \param ctx               Pointer to an existing context object.
 * \param line_inner_join   The inner join style of the lines.
 *
 * \sa ps_set_line_cap, ps_set_line_width, ps_set_miter_limit, ps_set_stroke_color, 
 *     ps_set_line_dash, ps_reset_line_dash, ps_set_line_join 
 */
PEXPORT void PICAPI ps_set_line_inner_join(ps_context* ctx, ps_line_inner_join line_inner_join);

/**
 * \fn float ps_set_line_width(ps_context* ctx, float width)
 * \brief Set the line width for a graphics context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param width   The line width to use, in pixels, must be greater than 0.
 * 
 * \return If the function succeeds, the return value is the old width.
 *         If the function fails, the return value is 0.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_set_line_cap, ps_set_line_join, ps_set_miter_limit, ps_set_stroke_color, 
 *     ps_set_line_dash, ps_reset_line_dash, ps_set_line_inner_join
 */
PEXPORT float PICAPI ps_set_line_width(ps_context* ctx, float width);

/**
 * \fn float ps_set_miter_limit(ps_context* ctx, float limit)
 * \brief Set the miter limit for the joins of connected lines in a graphics context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param limit   The miter limit to use.
 * 
 * \return If the function succeeds, the return value is the old miter limit.
 *         If the function fails, the return value is 0.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_set_line_cap, ps_set_line_join, ps_set_line_width, ps_set_stroke_color, 
 *     ps_set_line_dash, ps_reset_line_dash, ps_set_line_inner_join
 */
PEXPORT float PICAPI ps_set_miter_limit(ps_context* ctx, float limit);

/**
 * \fn void ps_set_stroke_color(ps_context* ctx, const ps_color* color)
 * \brief Set a color to the context, it is used to stroke a graphic object.
 *
 * \param ctx     Pointer to an existing context object.
 * \param color   The color to be set.
 *
 * \sa ps_set_line_cap, ps_set_line_join, ps_set_line_width, ps_set_miter_limit, 
 *     ps_set_line_dash, ps_reset_line_dash, ps_set_line_inner_join
 */
PEXPORT void PICAPI ps_set_stroke_color(ps_context* ctx, const ps_color* color);

/**
 * \fn void ps_set_line_dash(ps_context* ctx, float start, float* dashes, unsigned int num_dashes)
 * \brief Set the pattern for dashed lines in the context.
 *
 * \param ctx        Pointer to an existing context object.
 * \param start      A value that specifies how far into the dash pattern the line start.
 * \param dashes     An array of values that specify the length of the painted segments and 
 *                      unpainted segments.
 * \param num_dashes The number of elements in the array. 
 *
 * \sa ps_set_line_cap, ps_set_line_join, ps_set_line_width, ps_set_miter_limit, 
 *     ps_set_stroke_color, ps_reset_line_dash, ps_set_line_inner_join
 */
PEXPORT void PICAPI ps_set_line_dash(ps_context* ctx, float start,
                                        float* dashes, unsigned int num_dashes);

/**
 * \fn void ps_reset_line_dash(ps_context* ctx)
 * \brief Clear the dashs from the context, and set to solid.
 *
 * \param ctx  Pointer to an existing context object.
 *                 
 * \sa ps_set_line_cap, ps_set_line_join, ps_set_line_width, ps_set_miter_limit, 
 *     ps_set_stroke_color, ps_set_line_dash, ps_set_line_inner_join
 */
PEXPORT void PICAPI ps_reset_line_dash(ps_context* ctx);

/** @} end of stroke functions*/

/**
 * \defgroup shadow Shadow
 * @{
 */

/**
 * \fn void ps_set_shadow(ps_context* ctx, float x_offset, float y_offset, float blur)
 * \brief Enables shadowing in a context.
 *
 * \param ctx      Pointer to an existing context object.
 * \param x_offset Offset on horizontal from origin (0,0).
 * \param y_offset Offset on vertical from origin (0,0).
 * \param blur     Specifying the amount of blur (0 ~ 1). 
 *
 * \sa ps_set_shadow_color, ps_reset_shadow
 */
PEXPORT void PICAPI ps_set_shadow(ps_context* ctx, float x_offset, float y_offset, float blur);

/**
 * \fn void ps_set_shadow_color(ps_context* ctx, const ps_color* color)
 * \brief Set the color of shadow. Default is a black color with 1/3 alpha.
 *
 * \param ctx    Pointer to an existing context object.
 * \param color  Specifies the color of the shadow.  
 *
 * \sa ps_set_shadow, ps_reset_shadow
 */
PEXPORT void PICAPI ps_set_shadow_color(ps_context* ctx, const ps_color* color);

/**
 * \fn void ps_reset_shadow(ps_context* ctx)
 * \brief Disables shadowing in a context.
 *
 * \param ctx  Pointer to an existing context object.
 *
 * \sa ps_set_shadow_color, ps_set_shadow
 */
PEXPORT void PICAPI ps_reset_shadow(ps_context* ctx);

/** @} end of shadow functions*/

/**
 * \defgroup attr Attribute
 * @{
 */
/**
 * \brief Compositing operations for graphics context.
 */
typedef enum _ps_composite {
    /** Clear operation. */
    COMPOSITE_CLEAR,
    /** Source operation. */
    COMPOSITE_SRC,
    /** Source over operation. (Default) */
    COMPOSITE_SRC_OVER,
    /** Source in operation. */
    COMPOSITE_SRC_IN,
    /** Source out operation. */
    COMPOSITE_SRC_OUT,
    /** Source atop operation. */
    COMPOSITE_SRC_ATOP,
    /** Destination operation. */
    COMPOSITE_DST,
    /** Destination over operation. */
    COMPOSITE_DST_OVER,
    /** Destination in operation. */
    COMPOSITE_DST_IN,
    /** Destination out operation. */
    COMPOSITE_DST_OUT,
    /** Destination atop operation. */
    COMPOSITE_DST_ATOP,
    /** Xor operation. */
    COMPOSITE_XOR,
    /** Source darker operation. */
    COMPOSITE_DARKEN,
    /** Source lighter operation. */
    COMPOSITE_LIGHTEN,
    /** Overlay operation. */
    COMPOSITE_OVERLAY,
    /** Screen operation. */
    COMPOSITE_SCREEN,
    /** Multiply operation. */
    COMPOSITE_MULTIPLY,
    /** Plus operation. */
    COMPOSITE_PLUS,
    /** Minus operation. */
    COMPOSITE_MINUS,
    /** Exclusion operation. */
    COMPOSITE_EXCLUSION,
    /** Difference operation. */
    COMPOSITE_DIFFERENCE,
    /** Softlight operation. */
    COMPOSITE_SOFTLIGHT,
    /** Hardlight operation. */
    COMPOSITE_HARDLIGHT,
    /** Burn operation. */
    COMPOSITE_BURN,
    /** Dodge operation. */
    COMPOSITE_DODGE,
    /** Contrast operation. */
    COMPOSITE_CONTRAST,
    /** Invert operation. */
    COMPOSITE_INVERT,
    /** Invert blend operation. */
    COMPOSITE_INVERT_BLEND,
    /** Error value. */
    COMPOSITE_ERROR,
}ps_composite;

/**
 * \fn ps_composite ps_set_composite_operator(ps_context* ctx, ps_composite composite)
 * \brief Set composites opertaions for graphics context.
 *
 * \param ctx        Pointer to an existing context object.
 * \param composite  The Composite operation to be set.
 *
 * \return If the function succeeds, the return value is the old operation.
 *         If the function fails, the return value is COMPOSITE_ERROR.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_set_filter, ps_set_fill_rule
 */
PEXPORT ps_composite PICAPI ps_set_composite_operator(ps_context* ctx, ps_composite composite);

/**
 * \brief Graphics quality for rendering an image.
 */
typedef enum _ps_filter {
    /** 
     * No interpolation filter. 
     */
    FILTER_NEAREST,
    /**
     * Bilinear interpolation filter. (Default)
     */
    FILTER_BILINEAR,
    /**
     * Gaussian interpolation filter.
     */
    FILTER_GAUSSIAN,
    /**
     * Error value.
     */
    FILTER_UNKNOWN,
}ps_filter;

/**
 * \fn ps_filter ps_set_filter(ps_context* ctx, ps_filter filter)
 * \brief Set interpolation filter for graphic context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param filter  The interpolation filter to be set.
 *
 * \return If the function succeeds, the return value is the old filter.
 *         If the function fails, the return value is FILTER_UNKNOWN.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_set_composite_operator, ps_set_fill_rule
 */
PEXPORT ps_filter PICAPI ps_set_filter(ps_context* ctx, ps_filter filter);

/**
 * \brief Fill rules for graphics.
 */
typedef enum _ps_fill_rule {
    /**
     * Nonzero winding fill rule. (Default)
     */
    FILL_RULE_WINDING,
    /**
     * Even-odd fill rule.
     */
    FILL_RULE_EVEN_ODD,
    /**
     * Error value.
     */
    FILL_RULE_ERROR,
}ps_fill_rule;

/**
 * \fn ps_fill_rule ps_set_fill_rule(ps_context* ctx, ps_fill_rule rule)
 * \brief Set fill rule for graphic context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param rule    The fill rule to be set.
 *
 * \return If the function succeeds, the return value is the old rule.
 *         If the function fails, the return value is FILL_RULE_ERROR.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_set_composite_operator, ps_set_filter
 */
PEXPORT ps_fill_rule PICAPI ps_set_fill_rule(ps_context* ctx, ps_fill_rule rule);

/**
 * \fn float ps_set_alpha(ps_context* ctx, float alpha)
 * \brief Set the opacity level for objects drawn in graphic context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param alpha   A value that specifies the opacity level from transparent to opaque(0 ~ 1). 
 *                   Default level is opaque (1).
 *
 * \return If the function succeeds, the return value is the old level.
 *         If the function fails, the return value is 0.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_set_gamma, ps_set_blur, ps_set_antialias
 */
PEXPORT float PICAPI ps_set_alpha(ps_context* ctx, float alpha);

/**
 * \fn float ps_set_gamma(ps_context* ctx, float gamma)
 * \brief Set the gamma value for the graphic context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param gamma   A value that specifies the gamma (0 ~ 3). 
 *                   Default value is 1.
 *
 * \return If the function succeeds, the return value is the old one.
 *         If the function fails, the return value is 0.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_set_alpha, ps_set_blur, ps_set_antialias
 */
PEXPORT float PICAPI ps_set_gamma(ps_context* ctx, float gamma);

/**
 * \fn void ps_set_antialias(ps_context* ctx, ps_bool antialias)
 * \brief Set whether the Anti-aliasing should be turn on.
 *
 * \param ctx          Pointer to an existing context object.
 * \param antialias  Boolean value whether the anti-aliasing is turn on. (True default)
 *
 * \sa ps_set_alpha, ps_set_blur, ps_set_gamma
 */
PEXPORT void PICAPI ps_set_antialias(ps_context* ctx, ps_bool antialias);

/**
 * \fn float ps_set_blur(ps_context* ctx, float blur)
 * \brief Set the blur level for the graphic context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param blur    A value that specifies the blur level (0 ~ 1). 
 *                   Default value is none blur (0).
 *
 * \return If the function succeeds, the return value is the old level.
 *         If the function fails, the return value is 0.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_set_alpha, ps_set_gamma, ps_set_antialias
 */
PEXPORT float PICAPI ps_set_blur(ps_context* ctx, float blur);

/** @} end of attr functions*/

/**
 * \defgroup paint Paint
 * @{
 */

/**
 * \fn void ps_stroke(ps_context* ctx)
 * \brief Stroke the current path according to the line attributes.
 *           After called, the current path will be cleared from the context.
 *
 * \param ctx Pointer to an existing context object.
 *
 * \sa ps_fill, ps_paint, ps_clear
 */
PEXPORT void PICAPI ps_stroke(ps_context* ctx);

/**
 * \fn void ps_fill(ps_context* ctx)
 * \brief Fill the current path according to the source attributes.
 *           After called, the current path will be cleared from the context.
 *
 * \param ctx Pointer to an existing context object.
 *
 * \sa ps_stroke, ps_paint, ps_clear
 */
PEXPORT void PICAPI ps_fill(ps_context* ctx);

/**
 * \fn void ps_paint(ps_context* ctx)
 * \brief Fill and stroke the current path according to the source and line attributes.
 *           After called, the current path will be cleared from the context.
 *
 * \param ctx Pointer to an existing context object.
 *
 * \sa ps_stroke, ps_fill, ps_clear
 */
PEXPORT void PICAPI ps_paint(ps_context* ctx);

/**
 * \fn void ps_clear(ps_context* ctx)
 * \brief Clear the current context with source color.
 *
 * \param ctx Pointer to an existing context object.
 *
 * \sa ps_stroke, ps_fill, ps_paint
 */
PEXPORT void PICAPI ps_clear(ps_context* ctx);

/** @} end of paint functions*/

/**
 * \defgroup clip Clip
 * @{
 */

/**
 * \fn void ps_clip(ps_context* ctx)
 * \brief Cliping the current path, using the current fill rule.
 *           After called, the current path will be cleared from the context.
 *
 * \param ctx Pointer to an existing context object.
 *
 * \sa ps_clip_path, ps_clip_rect, ps_clip_rects, ps_reset_clip
 */
PEXPORT void PICAPI ps_clip(ps_context* ctx);

/**
 * \fn void ps_clip_path(ps_context* ctx, const ps_path* path, ps_fill_rule rule)
 * \brief Cliping specified path, using the given fill rule.
 *
 * \param ctx  Pointer to an existing context object.
 * \param path The path which will be clipped.
 * \param rule The rule of the path.
 *
 * \sa ps_clip, ps_clip_rect, ps_clip_rects, ps_reset_clip
 */
PEXPORT void PICAPI ps_clip_path(ps_context* ctx, const ps_path* path, ps_fill_rule rule);

/**
 * \fn void ps_clip_rect(ps_context* ctx, const ps_rect* rect)
 * \brief Cliping specified rectangle.
 *
 * \param ctx  Pointer to an existing context object.
 * \param rect The rectangle which will be clipped.
 *
 * \sa ps_clip, ps_clip_path, ps_clip_rects, ps_reset_clip, ps_clip_device_rect
 */
PEXPORT void PICAPI ps_clip_rect(ps_context* ctx, const ps_rect* rect);

/**
 * \fn void ps_clip_device_rect(ps_context* ctx, const ps_rect* rect)
 * \brief The fast wap to Cliping specified rectangle, the clip rect can not be rotated
 *           by world matrix.
 *
 * \param ctx  Pointer to an existing context object.
 * \param rect The rectangle which will be clipped.
 *
 * \sa ps_clip, ps_clip_path, ps_clip_rects, ps_reset_clip, ps_clip_rect
 */
PEXPORT void PICAPI ps_clip_device_rect(ps_context* ctx, const ps_rect* rect);

/* compatible macro for old version */
#define ps_clip_fast_rect ps_clip_device_rect

/**
 * \fn void ps_clip_rects(ps_context* ctx, const ps_rect* rects, unsigned int num_rects)
 * \brief Cliping specified area defined by an array of rectangles.
 *
 * \param ctx       Pointer to an existing context object.
 * \param rects     The array of rectangles which will be clipped.
 * \param num_rects The number of array entries.
 *
 * \sa ps_clip, ps_clip_path, ps_clip_rect, ps_reset_clip
 */
PEXPORT void PICAPI ps_clip_rects(ps_context* ctx, const ps_rect* rects, unsigned int num_rects);

/**
 * \fn void ps_reset_clip(ps_context* ctx)
 * \brief Clear the cliping area from the context.
 *
 * \param ctx  Pointer to an existing context object.
 *                 
 * \sa ps_clip, ps_clip_path, ps_clip_rect, ps_clip_rects
 */
PEXPORT void PICAPI ps_reset_clip(ps_context* ctx);

/** @} end of clip functions*/

/**
 * \defgroup state State
 * @{
 */
/**
 * \fn void ps_save(ps_context* ctx)
 * \brief Pushes a copy of the current graphics state on to stack for context.
 *
 * \param ctx  Pointer to an existing context object.
 *                The graphics state saved are:
 *                - Image filter
 *                - Font object
 *                - Gamma value
 *                - Alpha value
 *                - Blur value
 *                - Text stroke color
 *                - Text fill color
 *                - Compositing method
 *                - Transform matrix
 *                - Line width
 *                - Line cap
 *                - Line join
 *                - Line dash
 *                - Stroke color
 *                - Source object
 *                - Fill color
 *                - Fill rule
 *                - Clip region
 *                - Shadow parameters
 *                 
 * \sa ps_restore
 */
PEXPORT void PICAPI ps_save(ps_context* ctx);

/**
 * \fn void ps_restore(ps_context* ctx)
 * \brief Set the current graphics state to the state most recently saved.
 *
 * \param ctx  Pointer to an existing context object.
 *                 
 * \sa ps_save
 */
PEXPORT void PICAPI ps_restore(ps_context* ctx);

/** @} end of state functions*/

/** @} end of drawing functions*/

/**
 * \defgroup font Font
 * @{
 */
/**
 * \brief Charset for a font.
 */
typedef enum _ps_charset {
    /** 
     * Ansi ISO-8859-x charset 
     */
    CHARSET_ANSI,
    /** 
     * Unicode UCS-2 charset 
     */
    CHARSET_UNICODE,
}ps_charset;

/**
 * \brief Font weight
 */
typedef enum _ps_font_weight {
    /**
     * Regular weight.
     */
    FONT_WEIGHT_REGULAR = 400,
    /**
     * Medium weight.
     */
    FONT_WEIGHT_MEDIUM = 500,
    /**
     * Bold weight.
     */
    FONT_WEIGHT_BOLD = 700,
    /**
     * Heavy weight.
     */
    FONT_WEIGHT_HEAVY = 900,
}ps_font_weight;

/**
 * \fn ps_font* ps_font_create(const char* name, ps_charset charset, float size, int weight, ps_bool italic)
 * \brief Create a font object using the given parameters.
 *
 * \param name     The font family name. 
 * \param charset  The charset type. 
 * \param size     The font size.
 * \param weight   The font weight.
 * \param italic   Whether or not it is italic.
 *
 * \return If the function succeeds, the return value is the pointer to a new font object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_font_create_copy, ps_font_ref, ps_font_unref
 */
PEXPORT ps_font* PICAPI ps_font_create(const char* name, ps_charset charset,
                                                    float size, int weight, ps_bool italic);

/**
 * \fn ps_font* ps_font_create_copy(const ps_font* font)
 * \brief Create a copy from an existing font object.
 *
 * \param font  Pointer to an existing font object.
 *
 * \return If the function succeeds, the return value is the pointer to a new font object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_font_create, ps_font_ref, ps_font_unref
 */
PEXPORT ps_font* PICAPI ps_font_create_copy(const ps_font* font);

/**
 * \fn ps_font* ps_font_ref(ps_font* font)
 * \brief Increases the reference count of the font by 1.
 *
 * \param font  Pointer to an existing font object.
 *
 * \return If the function succeeds, the return value is the pointer to the font object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_font_create, ps_font_create_copy, ps_font_unref
 */
PEXPORT ps_font* PICAPI ps_font_ref(ps_font* font);

/**
 * \fn void ps_font_unref(ps_font* font)
 * \brief Decrements the reference count for the font object. 
 *        If the reference count on the font falls to 0, the font is freed.
 *
 * \param font  Pointer to an existing font object.
 *
 * \sa ps_font_create, ps_font_create_copy, ps_font_ref
 */
PEXPORT void PICAPI ps_font_unref(ps_font* font);

/**
 * \fn void ps_font_set_size(ps_font* font, float size)
 * \brief Set size for a font object.
 *
 * \param font  Pointer to an existing font object.
 * \param size  Size for the font.
 *
 * \sa ps_font_set_weight, ps_font_set_italic, ps_font_set_charset, 
 *        ps_font_set_hint, ps_font_set_flip
 */
PEXPORT void PICAPI ps_font_set_size(ps_font* font, float size);

/**
 * \fn void ps_font_set_weight(ps_font* font, int weight)
 * \brief Set weight for a font object.
 *
 * \param font    Pointer to an existing font object.
 * \param weight  Weight for the font.
 *
 * \sa ps_font_set_size, ps_font_set_italic, ps_font_set_charset, 
 *        ps_font_set_hint, ps_font_set_flip
 */
PEXPORT void PICAPI ps_font_set_weight(ps_font* font, int weight);

/**
 * \fn void ps_font_set_italic(ps_font* font, ps_bool italic)
 * \brief Set italic for a font object.
 *
 * \param font    Pointer to an existing font object.
 * \param italic  Whether or not italic for the font. (False default)
 *
 * \sa ps_font_set_size, ps_font_set_weight, ps_font_set_charset, 
 *        ps_font_set_hint, ps_font_set_flip
 */
PEXPORT void PICAPI ps_font_set_italic(ps_font* font, ps_bool italic);

/**
 * \fn void ps_font_set_charset(ps_font* font, ps_charset charset)
 * \brief Set charset for a font object.
 *
 * \param font    Pointer to an existing font object.
 * \param charset Charset for the font.
 *
 * \sa ps_font_set_size, ps_font_set_weight, ps_font_set_italic, 
 *        ps_font_set_hint, ps_font_set_flip
 */
PEXPORT void PICAPI ps_font_set_charset(ps_font* font, ps_charset charset);

/**
 * \fn void ps_font_set_hint(ps_font* font, ps_bool hint)
 * \brief Set hiting for a font object.
 *
 * \param font  Pointer to an existing font object.
 * \param hint     Whether or not auto hiting for the font. (True default)
 *
 * \sa ps_font_set_size, ps_font_set_weight, ps_font_set_italic, 
 *        ps_font_set_charset, ps_font_set_flip
 */
PEXPORT void PICAPI ps_font_set_hint(ps_font* font, ps_bool hint);

/**
 * \fn void ps_font_set_flip(ps_font* font, ps_bool flip)
 * \brief Set flip for a font object.
 *
 * \param font  Pointer to an existing font object.
 * \param flip     Whether or not flip y for the font. (False default)
 *
 * \sa ps_font_set_size, ps_font_set_weight, ps_font_set_italic, 
 *        ps_font_set_charset, ps_font_set_hint
 */
PEXPORT void PICAPI ps_font_set_flip(ps_font* font, ps_bool flip);

/**
 * \brief A structure that contains font information.
 */
typedef struct _ps_font_info {
    /**
     * Size, the height of glyph.
     */
    float size;
    /**
     * Ascent, maximum distance above the baseline.
     */
    float ascent;
    /**
     * Descent, maximum distance below the baseline.
     */
    float descent;
    /**
     * Leading, the spaceing between consecutive lines of text.
     */
    float leading;
    /**
     * UnitsEm, the number of glyph space units per em.
     */
    unsigned int unitsEM;
}ps_font_info;

/**
 * \fn ps_bool ps_get_font_info(ps_context* ctx, ps_font_info* info)
 * \brief Return the font information from the graphics context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param info  Pointer to a structure to receiving the font information.
 *
 * \return  True if is success, otherwise False.
 *
 * \sa ps_set_font
 */
PEXPORT ps_bool PICAPI ps_get_font_info(ps_context* ctx, ps_font_info* info);

/**
 * \fn ps_font* ps_set_font(ps_context* ctx, const ps_font* font)
 * \brief Set a new font to graphics context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param font  The new font to be set.
 *
 * \return If the function succeeds, the return value is the pointer to old font object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_get_font_info
 */
PEXPORT ps_font* PICAPI ps_set_font(ps_context* ctx, const ps_font* font);

/** @} end of font functions*/

/**
 * \defgroup text Text
 * @{
 */

/**
 * \brief Text rendering type
 */
typedef enum _ps_text_type {
    /** 
     * Smooth rendering.
     */
    TEXT_TYPE_SMOOTH,
    /**
     * Mono rendering.
     */
    TEXT_TYPE_MONO,
    /**
     * OutLine rendering.
     */
    TEXT_TYPE_STROKE,
}ps_text_type;

/**
 * \fn ps_size ps_get_text_extent(ps_context* ctx, const void* text, unsigned int length)
 * \brief Get extent for text using current font which selected to graphic context.
 *
 * \param ctx       Pointer to an existing context object.
 * \param text    Text which being tested. It can be latin-1 or unicode encoding.
 * \param length  Length of the text.
 *
 * \return If the function succeeds, the return value is the size of the text.
 *         If the function fails, the return value is (0,0).
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_glyph_get_extent
 */
PEXPORT ps_size PICAPI ps_get_text_extent(ps_context* ctx, const void* text, unsigned int length);

/**
 * \fn void ps_set_text_color(ps_context* ctx, const ps_color * color)
 * \brief Set the text fill color for the graphic context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param color    The text fill color.          
 *
 * \sa ps_transform, ps_set_text_matrix, ps_set_text_antialias,
 *     ps_set_text_stroke_color, ps_set_text_render_type, ps_set_text_kerning
 */
PEXPORT void PICAPI ps_set_text_color(ps_context* ctx, const ps_color * color);

/**
 * \fn void ps_set_text_stroke_color(ps_context* ctx, const ps_color * color)
 * \brief Set the text stroke color for the graphic context.
 *
 * \param ctx     Pointer to an existing context object.
 * \param color    The text stroke color.          
 *
 * \sa ps_set_text_color, ps_set_text_matrix, ps_set_text_antialias,
 *     ps_text_transform, ps_set_text_render_type, ps_set_text_kerning
 */
PEXPORT void PICAPI ps_set_text_stroke_color(ps_context* ctx, const ps_color * color);

/**
 * \fn void ps_text_transform(ps_context* ctx, const ps_matrix* matrix)
 * \brief Transform text matrix for the graphic context.
 *           The text matrix is not a part of graphic state -- saving and restoring has no 
 *           effect on the text matrix. The text matrix is an attribute of graphic context.
 *
 * \param ctx       Pointer to an existing context object.
 * \param matrix  Pointer to an existing matrix object.
 *
 * \sa ps_set_text_matrix, ps_set_text_antialias, ps_set_text_color, 
 *     ps_set_text_stroke_color, ps_set_text_render_type, ps_set_text_kerning
 */
PEXPORT void PICAPI ps_text_transform(ps_context* ctx, const ps_matrix* matrix);

/**
 * \fn void ps_set_text_matrix(ps_context* ctx, const ps_matrix* matrix)
 * \brief Set text matrix for the graphic context.
 *           The text matrix is not a part of graphic state -- saving and restoring has no 
 *           effect on the text matrix. The text matrix is an attribute of graphic context.
 *
 * \param ctx       Pointer to an existing context object.
 * \param matrix  Pointer to an existing matrix object.
 *
 * \sa ps_text_transform, ps_set_text_antialias, ps_set_text_color, 
 *     ps_set_text_stroke_color, ps_set_text_render_type, ps_set_text_kerning
 */
PEXPORT void PICAPI ps_set_text_matrix(ps_context* ctx, const ps_matrix* matrix);

/**
 * \fn void ps_set_text_render_type(ps_context* ctx, ps_text_type type)
 * \brief Set rendering type for text.
 *
 * \param ctx       Pointer to an existing context object.
 * \param type       Rendering type for the text.
 *
 * \sa ps_set_text_matrix, ps_set_text_color, ps_set_text_stroke_color, 
 *     ps_set_text_antialias, ps_text_transform, ps_set_text_kerning
 */
PEXPORT void PICAPI ps_set_text_render_type(ps_context* ctx, ps_text_type type);

/**
 * \fn void ps_set_text_antialias(ps_context* ctx, ps_bool antialias)
 * \brief Set whether the font allowed be anti-aliasing.
 *
 * \param ctx         Pointer to an existing context object.
 * \param antialias Boolean value whether anti-aliasing is allowed. (True default)
 *
 * \sa ps_set_text_matrix, ps_set_text_color, ps_set_text_stroke_color, 
 *     ps_set_text_render_type, ps_text_transform, ps_set_text_kerning
 */
PEXPORT void PICAPI ps_set_text_antialias(ps_context* ctx, ps_bool antialias);

/**
 * \fn void ps_set_text_kerning(ps_context* ctx, ps_bool kerning)
 * \brief Set whether the font auto kerning is allowed.
 *
 * \param ctx         Pointer to an existing context object.
 * \param kerning   Boolean value whether auto kerning is allowed. (True default)
 *
 * \sa ps_set_text_matrix, ps_set_text_color, ps_set_text_stroke_color, 
 *     ps_set_text_render_type, ps_text_transform, ps_set_text_antialias
 */
PEXPORT void PICAPI ps_set_text_kerning(ps_context* ctx, ps_bool kerning);

/**
 * \fn void ps_text_out_length(ps_context* ctx, float x, float y, const char* text, unsigned int length)
 * \brief Draw single byte characters (latin-1) at location in user space.
 *
 * \param ctx       Pointer to an existing context object.
 * \param x       The X-coordinate at which to draw the text.    
 * \param y       The Y-coordinate at which to draw the text.    
 * \param text       The text string to be draw.
 * \param length  The length of the text string.     
 *
 * \sa ps_wide_text_out_length, ps_draw_text
 */
PEXPORT void PICAPI ps_text_out_length(ps_context* ctx, float x, float y,
                                                const char* text, unsigned int length);

/**
 * \fn void ps_wide_text_out_length(ps_context* ctx, float x, float y, const ps_uchar16* text, unsigned int length)
 * \brief Draw unicode characters (ucs-2) at location in user space.
 *
 * \param ctx       Pointer to an existing context object.
 * \param x       The X-coordinate at which to draw the text.    
 * \param y       The Y-coordinate at which to draw the text.    
 * \param text       The unicode text string to be draw.
 * \param length  The length of the text string.     
 *
 * \sa ps_text_out_length, ps_draw_text
 */
PEXPORT void PICAPI ps_wide_text_out_length(ps_context* ctx, float x, float y,
                                                const ps_uchar16* text, unsigned int length);
/**
 * \brief Draw mode for rending text.
 */
typedef enum _ps_draw_text_type {
    /**
     * Perform a fill operation on the text.
     */
    DRAW_TEXT_FILL,
    /**
     * Perform a stroke operation on the text.
     */
    DRAW_TEXT_STROKE,
    /**
     * Perform fill,then stroke operation on the text.
     */
    DRAW_TEXT_BOTH,
}ps_draw_text_type;

/**
 * \brief Text align mode for drawing text.
 */
typedef enum _ps_text_align {
    /**
     * Centers text horizontally in the rectangle.
     */
    TEXT_ALIGN_CENTER = 0,
    /**
     * Justifies the text to the top of the rectangle.
     */
    TEXT_ALIGN_TOP       = 1,
    /**
     * Justifies the text to the bottom of the rectangle.
     */
    TEXT_ALIGN_BOTTOM = 2,
    /**
     * Aligns text to left.
     */
    TEXT_ALIGN_LEFT   = 4,
    /**
     * Aligns text to right.
     */
    TEXT_ALIGN_RIGHT  = 8,
}ps_text_align;

/**
 * \fn void ps_draw_text(ps_context* ctx, const ps_rect* area, const void* text, unsigned int length,
 *                                                                         ps_draw_text_type type, ps_text_align align)
 * \brief Draw text in a rectangle area, using font object which is selected in graphic context.
 *
 * \param ctx       Pointer to an existing context object.
 * \param area    The rectangle area which text to be drawn.
 * \param text       The unicode text string to be draw.
 * \param length  The length of the text string.     
 * \param type    The rending type of text.
 * \param align   The text align mode.
 *
 * \sa ps_text_out_length, ps_wide_text_out_length
 */
PEXPORT void PICAPI ps_draw_text(ps_context* ctx, const ps_rect* area, const void* text,
                                unsigned int length, ps_draw_text_type type, ps_text_align align);

/**
 * \fn ps_bool ps_get_glyph(ps_context* ctx, int ch, ps_glyph* glyph)
 * \brief Get the glyph from a given character, using font object which is selected in graphic context.
 *
 * \param ctx       Pointer to an existing context object.
 * \param ch      The character value.
 * \param glyph   Pointer to a structure to receiving the glyph.
 *
 * \return  True if is success, otherwise False.
 *
 * \sa ps_show_glyphs, ps_get_path_from_glyph
 */
PEXPORT ps_bool PICAPI ps_get_glyph(ps_context* ctx, int ch, ps_glyph* glyph);

/**
 * \fn void ps_show_glyphs(ps_context* ctx, float x, float y, ps_glyph* glyphs, unsigned int length)
 * \brief Draw an array of glyphs at location in user space.
 *
 * \param ctx       Pointer to an existing context object.
 * \param x       The X-coordinate at which to draw the glyphs.    
 * \param y       The Y-coordinate at which to draw the glyphs.    
 * \param glyphs  The array of glyphs.
 * \param length  The length of array.     
 *
 * \sa ps_get_path_from_glyph
 */
PEXPORT void PICAPI ps_show_glyphs(ps_context* ctx, float x, float y,
                                                    ps_glyph* glyphs, unsigned int length);

/**
 * \fn ps_bool ps_get_path_from_glyph(ps_context* ctx, const ps_glyph* glyph, ps_path* path)
 * \brief Get the path from a given glyph object.
 *
 * \param ctx       Pointer to an existing context object.
 * \param glyph   The glyph object.
 * \param path    Pointer to a structure to receiving the path.
 *
 * \return  True if is success, otherwise False.
 *
 * \sa ps_show_glyphs
 */
PEXPORT ps_bool PICAPI ps_get_path_from_glyph(ps_context* ctx,
                                        const ps_glyph* glyph, ps_path* path);

/**
 * \fn ps_size ps_glyph_get_extent(const ps_glyph* glyph)
 * \brief Get extent for a glyph object.
 *
 * \param glyph  Pointer to an existing glyph object.
 *
 * \return If the function succeeds, the return value is the size of the glyph.
 *         If the function fails, the return value is (0,0).
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_get_text_extent
 */
PEXPORT ps_size PICAPI ps_glyph_get_extent(const ps_glyph* glyph);

/** @} end of text functions*/

/**
 * \defgroup transform Transform
 * @{
 */

/**
 * \fn void ps_translate(ps_context* ctx, float tx, float ty)
 * \brief Changes the origin of the user coordinate system in graphic context.
 *
 * \param ctx  Pointer to an existing context object.
 * \param tx   The amount, in pixels, which to translate in x direction.
 * \param ty   The amount, in pixels, which to translate in y direction.
 *
 * \sa ps_scale, ps_shear, ps_rotate, ps_identity, ps_transform
 */
PEXPORT void PICAPI ps_translate(ps_context* ctx, float tx, float ty);

/**
 * \fn void ps_scale(ps_context* ctx, float sx, float sy)
 * \brief Changes the scale of the user coordinate system in graphic context.
 *
 * \param ctx  Pointer to an existing context object.
 * \param sx   The scale factor for the X dimension.
 * \param sy   The scale factor for the Y dimension.
 *
 * \sa ps_translate, ps_shear, ps_rotate, ps_identity, ps_transform
 */
PEXPORT void PICAPI ps_scale(ps_context* ctx, float sx, float sy);

/**
 * \fn void ps_shear(ps_context* ctx, float shx, float shy)
 * \brief Changes the shear of the user coordinate system in graphic context.
 *
 * \param ctx  Pointer to an existing context object.
 * \param shx  The shear factor for the horizontal.
 * \param shy  The shear factor for the vertical.
 *
 * \sa ps_translate, ps_scale, ps_rotate, ps_identity, ps_transform
 */
PEXPORT void PICAPI ps_shear(ps_context* ctx, float shx, float shy);

/**
 * \fn void ps_rotate(ps_context* ctx, float angle)
 * \brief Rotates the user coordinate system in graphic context.
 *
 * \param ctx       Pointer to an existing context object.
 * \param angle     The angle, in radians, which to rotate the specified context.
 *
 * \sa ps_translate, ps_scale, ps_shear, ps_identity, ps_transform
 */
PEXPORT void PICAPI ps_rotate(ps_context* ctx, float angle);

/**
 * \fn void ps_identity(ps_context* ctx)
 * \brief Reset the current transformation matrix to identity matrix.
 *
 * \param ctx  Pointer to an existing context object.
 *
 * \sa ps_translate, ps_scale, ps_shear, ps_rotate, ps_transform
 */
PEXPORT void PICAPI ps_identity(ps_context* ctx);

/**
 * \fn void ps_transform(ps_context* ctx, const ps_matrix* matrix)
 * \brief Transforms the user coordinate system in graphic context using a specified matrix.
 *
 * \param ctx        Pointer to an existing context object.
 * \param matrix  The matrix to apply to the specified context's current transformation matrix. 
 *
 * \sa ps_translate, ps_scale, ps_shear, ps_rotate, ps_identity
 */
PEXPORT void PICAPI ps_transform(ps_context* ctx, const ps_matrix* matrix);

/**
 * \fn void ps_set_matrix(ps_context* ctx, const ps_matrix* matrix)
 * \brief Modifies the current transformation matrix to the given matrix.
 *
 * \param ctx        Pointer to an existing context object.
 * \param matrix  The transformation matrix to be set.
 *
 * \sa ps_get_matrix
 */
PEXPORT void PICAPI ps_set_matrix(ps_context* ctx, const ps_matrix* matrix);

/**
 * \fn ps_bool ps_get_matrix(ps_context* ctx, ps_matrix* matrix)
 * \brief Get the current transformation matrix from graphic context.
 *
 * \param ctx        Pointer to an existing context object.
 * \param matrix  Pointer to a matrix object to receiving the current transformation.
 *
 * \return  True if is success, otherwise False.
 *
 * \sa ps_set_matrix
 */
PEXPORT ps_bool PICAPI ps_get_matrix(ps_context* ctx, ps_matrix* matrix);

/**
 * \fn void ps_world_to_viewport(ps_context* ctx, ps_point* point)
 * \brief Transform a coordinate from device space to user space.
 *
 * \param ctx       Pointer to an existing context object.
 * \param point  The point which to be transformed.
 *
 * \sa ps_viewport_to_world
 */
PEXPORT void PICAPI ps_world_to_viewport(ps_context* ctx, ps_point* point);

/**
 * \fn void ps_viewport_to_world(ps_context* ctx, ps_point* point)
 * \brief Transform a coordinate from user space to device space.
 *
 * \param ctx       Pointer to an existing context object.
 * \param point  The point which to be transformed.
 *
 * \sa ps_world_to_viewport
 */
PEXPORT void PICAPI ps_viewport_to_world(ps_context* ctx, ps_point* point);

/** @} end of transform functions*/

/**
 * \defgroup matrix Matrix
 * @{
 */

/**
 * \fn ps_matrix* ps_matrix_create(void)
 * \brief Create a identity matrix object.
 *
 * \return If the function succeeds, the return value is the pointer to a new matrix object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_matrix_create_init, ps_matrix_create_copy, ps_matrix_ref, ps_matrix_unref
 */
PEXPORT ps_matrix* PICAPI ps_matrix_create(void);

/**
 * \fn ps_matrix* ps_matrix_create_init(float sx, float shy, float shx, 
 *                                                     float sy, float tx, float ty)
 * \brief Create a matrix with given parameters.
 *
 * \param sx  The horizontal scaling factor.
 * \param shy The vertical skewing factor.
 * \param shx The horizontal skewing factor.
 * \param sy  The vertical scaling factor.
 * \param tx  The horizontal translating factor. 
 * \param ty  The vertical translating factor.
 *
 * \return If the function succeeds, the return value is the pointer to a new matrix object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_matrix_create, ps_matrix_create_copy, ps_matrix_ref, ps_matrix_unref
 */
PEXPORT ps_matrix* PICAPI ps_matrix_create_init(float sx, float shy, float shx,
                                                            float sy, float tx, float ty);

/**
 * \fn ps_matrix* ps_matrix_create_copy(const ps_matrix* matrix)
 * \brief Create a matrix copy from an exist one.
 *
 * \param matrix Pointer to an existing matrix object.
 *
 * \return If the function succeeds, the return value is the pointer to a new matrix object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_matrix_create_init, ps_matrix_create, ps_matrix_ref, ps_matrix_unref
 */
PEXPORT ps_matrix* PICAPI ps_matrix_create_copy(const ps_matrix* matrix);

/**
 * \fn ps_matrix* ps_matrix_ref(ps_matrix* matrix)
 * \brief Increases the reference count of the matrix by 1.
 *
 * \param matrix  Pointer to an existing matrix object.
 *
 * \return If the function succeeds, the return value is the pointer to the matrix object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_matrix_create_init, ps_matrix_create, ps_matrix_create_copy, ps_matrix_unref
 */
PEXPORT ps_matrix* PICAPI ps_matrix_ref(ps_matrix* matrix);

/**
 * \fn void ps_matrix_unref(ps_matrix* matrix)
 * \brief Decrements the reference count for the matrix object. 
 *        If the reference count on the matrix falls to 0, the matrix is freed.
 *
 * \param matrix  Pointer to an existing matrix object.
 *
 * \sa ps_matrix_create_init, ps_matrix_create, ps_matrix_create_copy, ps_matrix_ref
 */
PEXPORT void PICAPI ps_matrix_unref(ps_matrix* matrix);

/**
 * \fn void ps_matrix_init(ps_matrix* matrix, float sx, float shy, 
 *                                         float shx, float sy, float tx, float ty)
 * \brief Initialize an existing matrix object with given parameters.
 *
 * \param matrix  Pointer to an existing matrix object. 
 * \param sx        The horizontal scaling factor.
 * \param shy       The vertical skewing factor.
 * \param shx       The horizontal skewing factor.
 * \param sy        The vertical scaling factor.
 * \param tx        The horizontal translating factor. 
 * \param ty        The vertical translating factor.
 *
 * \sa ps_matrix_translate, ps_matrix_scale, ps_matrix_shear, ps_matrix_rotate
 */
PEXPORT void PICAPI ps_matrix_init(ps_matrix* matrix, float sx, float shy,
                                            float shx, float sy, float tx, float ty);

/**
 * \fn void ps_matrix_translate(ps_matrix* matrix, float tx, float ty)
 * \brief Modify a matrix by translating.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param tx         The amount, in pixels, which to translate in x direction.
 * \param ty      The amount, in pixels, which to translate in y direction.
 *
 * \sa ps_matrix_scale, ps_matrix_shear, ps_matrix_rotate
 */
PEXPORT void PICAPI ps_matrix_translate(ps_matrix* matrix, float tx, float ty);

/**
 * \fn void ps_matrix_scale(ps_matrix* matrix, float sx, float sy)
 * \brief Modify a matrix by scaling.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param sx         The scale factor for the X dimension.
 * \param sy         The scale factor for the Y dimension.
 *
 * \sa ps_matrix_translate, ps_matrix_shear, ps_matrix_rotate
 */
PEXPORT void PICAPI ps_matrix_scale(ps_matrix* matrix, float sx, float sy);

/**
 * \fn void ps_matrix_rotate(ps_matrix* matrix, float angle)
 * \brief Modify a matrix by rotating.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param angle      The angle, in radians, which to rotate the specified matrix.
 *
 * \sa ps_matrix_translate, ps_matrix_shear, ps_matrix_scale
 */
PEXPORT void PICAPI ps_matrix_rotate(ps_matrix* matrix, float angle);

/**
 * \fn void ps_matrix_shear(ps_matrix* matrix, float shx, float shy)
 * \brief Modify a matrix by shearing.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param shx     The shear factor for the horizontal.
 * \param shy     The shear factor for the vertical.
 *
 * \sa ps_matrix_translate, ps_matrix_scale, ps_matrix_rotate
 */
PEXPORT void PICAPI ps_matrix_shear(ps_matrix* matrix, float shx, float shy);

/**
 * \fn void ps_matrix_invert(ps_matrix* matrix)
 * \brief Modify a matrix by inverting.
 *
 * \param matrix  Pointer to an existing matrix object.
 *
 * \sa ps_matrix_flip_x, ps_matrix_flip_y, ps_matrix_reset
 */
PEXPORT void PICAPI ps_matrix_invert(ps_matrix* matrix);

/**
 * \fn void ps_matrix_reset(ps_matrix* matrix)
 * \brief Reset a matrix to identity matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 *
 * \sa ps_matrix_flip_x, ps_matrix_flip_y, ps_matrix_invert
 */
PEXPORT void PICAPI ps_matrix_reset(ps_matrix* matrix);

/**
 * \fn void ps_matrix_flip_x(ps_matrix* matrix)
 * \brief Flip a matrix in the horizontal direction.
 *
 * \param matrix  Pointer to an existing matrix object.
 *
 * \sa ps_matrix_reset, ps_matrix_flip_y, ps_matrix_invert
 */
PEXPORT void PICAPI ps_matrix_flip_x(ps_matrix* matrix);

/**
 * \fn void ps_matrix_flip_y(ps_matrix* matrix)
 * \brief Flip a matrix in the vertical direction.
 *
 * \param matrix  Pointer to an existing matrix object.
 *
 * \sa ps_matrix_reset, ps_matrix_flip_x, ps_matrix_invert
 */
PEXPORT void PICAPI ps_matrix_flip_y(ps_matrix* matrix);

/**
 * \fn void ps_matrix_multiply(ps_matrix* result, const ps_matrix* a, const ps_matrix* b)
 * \brief Multiplies the matrix in a and b together and stores the result in result.
 *
 * \param result  Pointer to an existing matrix object in which to store the result.
 * \param a       Pointer to an existing matrix object.
 * \param b       Pointer to an existing matrix object.
 *
 * \sa ps_matrix_is_identity, ps_matrix_is_equal ps_matrix_get_determinant
 */
PEXPORT void PICAPI ps_matrix_multiply(ps_matrix* result, const ps_matrix* a, const ps_matrix* b);

/**
 * \fn ps_bool ps_matrix_is_identity(const ps_matrix* matrix)
 * \brief Checks whether the matrix is the identity transform matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 *
 * \return  True if is identity, otherwise False.
 *
 * \sa ps_matrix_is_equal, ps_matrix_get_determinant, ps_matrix_multiply
 */
PEXPORT ps_bool PICAPI ps_matrix_is_identity(const ps_matrix* matrix);

/**
 * \fn ps_bool ps_matrix_is_equal(const ps_matrix* a, const ps_matrix* b)
 * \brief Checks whether two matrix are equal.
 *
 * \param a  Pointer to an existing matrix object.
 * \param b  Pointer to an existing matrix object.
 *
 * \return  True if a and b are equal, otherwise False.
 *
 * \sa ps_matrix_is_identity, ps_matrix_get_determinant, ps_matrix_multiply
 */
PEXPORT ps_bool PICAPI ps_matrix_is_equal(const ps_matrix* a, const ps_matrix* b);

/**
 * \fn float ps_matrix_get_determinant(const ps_matrix* matrix)
 * \brief Return the determinant from a matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 *
 * \return The determinant of the matrix.
 *
 * \sa ps_matrix_is_equal, ps_matrix_is_identity, ps_matrix_multiply
 */
PEXPORT float PICAPI ps_matrix_get_determinant(const ps_matrix* matrix);

/**
 * \fn void ps_matrix_set_translate_factor(ps_matrix* matrix, float tx, float ty)
 * \brief Set the translate factors to the matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param tx         The amount, in pixels, which the translate factor in x direction.
 * \param ty      The amount, in pixels, which the translate factor in y direction.
 *
 * \sa ps_matrix_get_translate_factor
 */
PEXPORT void PICAPI ps_matrix_set_translate_factor(ps_matrix* matrix, float tx, float ty);

/**
 * \fn ps_bool ps_matrix_get_translate_factor(ps_matrix* matrix, float *tx, float *ty)
 * \brief Get the translate factors from the matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param tx         Pointer to a buffer to receiving the translate factor in x direction.
 * \param ty      Pointer to a buffer to receiving the translate factor in y direction.
 *
 * \return  True if is success, otherwise False.
 *
 * \sa ps_matrix_set_translate_factor
 */
PEXPORT ps_bool PICAPI ps_matrix_get_translate_factor(ps_matrix* matrix, float *tx, float *ty);

/**
 * \fn void ps_matrix_set_scale_factor(ps_matrix* matrix, float sx, float sy)
 * \brief Set the scale factors to the matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param sx      The scale factor in x dimension.
 * \param sy       The scale factor in y dimension.
 *
 * \sa ps_matrix_get_scale_factor
 */
PEXPORT void PICAPI ps_matrix_set_scale_factor(ps_matrix* matrix, float sx, float sy);

/**
 * \fn ps_bool ps_matrix_get_scale_factor(ps_matrix* matrix, float *sx, float *sy)
 * \brief Get the scale factors from the matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param sx         Pointer to a buffer to receiving the scale factor in x dimension.
 * \param sy      Pointer to a buffer to receiving the scale factor in y dimension.
 *
 * \return  True if is success, otherwise False.
 *
 * \sa ps_matrix_set_scale_factor
 */
PEXPORT ps_bool PICAPI ps_matrix_get_scale_factor(ps_matrix* matrix, float *sx, float *sy);

/**
 * \fn void ps_matrix_set_shear_factor(ps_matrix* matrix, float shx, float shy)
 * \brief Set the shear factors to the matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param shx     The shear factor for the horizontal.
 * \param shy       The shear factor for the vertical.
 *
 * \sa ps_matrix_get_shear_factor
 */
PEXPORT void PICAPI ps_matrix_set_shear_factor(ps_matrix* matrix, float shx, float shy);

/**
 * \fn ps_bool ps_matrix_get_shear_factor(ps_matrix* matrix, float *shx, float *shy)
 * \brief Get the shear factors from the matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param shx     Pointer to a buffer to receiving the shear factor in horizontal.
 * \param shy     Pointer to a buffer to receiving the shear factor in vertical.
 *
 * \return  True if is success, otherwise False.
 *
 * \sa ps_matrix_set_shear_factor
 */
PEXPORT ps_bool PICAPI ps_matrix_get_shear_factor(ps_matrix* matrix, float *shx, float *shy);

/**
 * \fn void ps_matrix_transform_point(const ps_matrix* matrix, ps_point* point)
 * \brief Transform an existing point using the matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param point   An existing point which will be transformed. 
 *
 * \sa ps_matrix_transform_rect, ps_matrix_transform_path
 */
PEXPORT void PICAPI ps_matrix_transform_point(const ps_matrix* matrix, ps_point* point);

/**
 * \fn void ps_matrix_transform_rect(const ps_matrix* matrix, ps_rect* rect)
 * \brief Transform an existing rectangle using the matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param rect    An existing rectangle which will be transformed. 
 *
 * \sa ps_matrix_transform_point, ps_matrix_transform_path
 */
PEXPORT void PICAPI ps_matrix_transform_rect(const ps_matrix* matrix, ps_rect* rect);

/**
 * \fn void ps_matrix_transform_path(const ps_matrix* matrix, ps_path* path)
 * \brief Transform an existing path using the matrix.
 *
 * \param matrix  Pointer to an existing matrix object.
 * \param path    An existing path which will be transformed. 
 *
 * \sa ps_matrix_transform_point, ps_matrix_transform_rect
 */
PEXPORT void PICAPI ps_matrix_transform_path(const ps_matrix* matrix, ps_path* path);

/** @} end of matrix functions*/

/**
 * \defgroup geometry Geometry
 * @{
 */

/**
 * \fn void ps_new_path(ps_context* ctx)
 * \brief Create a new empty path in the graphic context, clear the old one.
 *
 * \param ctx  Pointer to an existing context object.
 *
 * \sa ps_new_sub_path, ps_set_path, ps_close_path
 */
PEXPORT void PICAPI ps_new_path(ps_context* ctx);

/**
 * \fn void ps_new_sub_path(ps_context* ctx)
 * \brief Close the current path, and add a new empty sub path in the graphic context.
 *
 * \param ctx  Pointer to an existing context object.
 *
 * \sa ps_new_path, ps_set_path, ps_close_path
 */
PEXPORT void PICAPI ps_new_sub_path(ps_context* ctx);

/**
 * \fn void ps_set_path(ps_context* ctx, const ps_path* path)
 * \brief Replace the current path in the graphic context.
 *
 * \param ctx   Pointer to an existing context object.
 * \param path  The path will be set to graphic context.
 *
 * \sa ps_new_path, ps_new_sub_path, ps_close_path
 */
PEXPORT void PICAPI ps_set_path(ps_context* ctx, const ps_path* path);

/**
 * \fn void ps_close_path(ps_context* ctx)
 * \brief Close the current path in the graphic context.
 *
 * \param ctx  Pointer to an existing context object.
 *
 * \sa ps_new_path, ps_new_sub_path, ps_set_path
 */
PEXPORT void PICAPI ps_close_path(ps_context* ctx);

/**
 * \fn void ps_move_to(ps_context* ctx, const ps_point* point)
 * \brief Begin a new sub path, and set the current point.
 *
 * \param ctx    Pointer to an existing context object.
 * \param point  The point which will be set. 
 *
 * \sa ps_line_to, ps_bezier_curve_to, ps_quad_curve_to
 *     ps_arc, ps_tangent_arc, ps_rectangle, ps_rounded_rect, ps_ellipse
 */
PEXPORT void PICAPI ps_move_to(ps_context* ctx, const ps_point* point);

/**
 * \fn void ps_line_to(ps_context* ctx, const ps_point* point)
 * \brief Add a line to the current path from the current point to given point.
 *
 * \param ctx    Pointer to an existing context object.
 * \param point  The point which will line to.
 *
 * \sa ps_move_to, ps_bezier_curve_to, ps_quad_curve_to
 *     ps_arc, ps_tangent_arc, ps_rectangle, ps_rounded_rect, ps_ellipse
 */
PEXPORT void PICAPI ps_line_to(ps_context* ctx, const ps_point* point);

/**
 * \fn void ps_bezier_curve_to(ps_context* ctx, const ps_point* fcp, 
 *                                        const ps_point* scp, const ps_point* ep)
 * \brief Add a cubic bezier spline to the current path from current point to end point.
 *
 * \param ctx  Pointer to an existing context object.
 * \param fcp  The first control point of the curve.
 * \param scp  The second control point of the curve.
 * \param ep   The end point of the curve.
 *
 * \sa ps_move_to, ps_line_to, ps_quad_curve_to, ps_arc, 
 *     ps_tangent_arc, ps_rectangle, ps_rounded_rect, ps_ellipse
 */
PEXPORT void PICAPI ps_bezier_curve_to(ps_context* ctx, const ps_point* fcp,
                                           const ps_point* scp, const ps_point* ep);

/**
 * \fn void ps_quad_curve_to(ps_context* ctx, const ps_point* cp, const ps_point* ep)
 * \brief Add a quadratic bezier spline to the current path from current point to end point.
 *
 * \param ctx  Pointer to an existing context object.
 * \param cp   The control point of the curve.
 * \param ep   The end point of the curve.
 *
 * \sa ps_move_to, ps_line_to, ps_bezier_curve_to, ps_arc, 
 *     ps_tangent_arc, ps_rectangle, ps_rounded_rect, ps_ellipse
 */
PEXPORT void PICAPI ps_quad_curve_to(ps_context* ctx, const ps_point* cp, const ps_point* ep);

/**
 * \fn void ps_arc(ps_context* ctx, const ps_point* cp, float radius, 
 *                                        float sangle, float eangle, ps_bool clockwise)
 * \brief Add a circular arc to the current path. 
 *
 * \param ctx        Pointer to an existing context object.
 * \param cp         The center point of the arc.
 * \param radius     The radius of the arc.
 * \param sangle     The start angle, in radians.
 * \param eangle     The end angle, in radians.
 * \param clockwise  True is clockwise, False is counter clockwise.
 *
 * \sa ps_move_to, ps_line_to, ps_bezier_curve_to, ps_quad_curve_to, 
 *     ps_tangent_arc, ps_rectangle, ps_rounded_rect, ps_ellipse
 */
PEXPORT void PICAPI ps_arc(ps_context* ctx, const ps_point* cp, float radius,
                                           float sangle, float eangle, ps_bool clockwise);

/**
 * \fn void ps_tangent_arc(ps_context* ctx, const ps_rect* rect, float sangle, float sweep)
 * \brief Add a circular arc which is inner tangent from a rectangle.
 *
 * \param ctx     Pointer to an existing context object.
 * \param rect    The rectangle which the arc to fit in.
 * \param sangle  The start angle, in radians.
 * \param sweep   The sweep angle, in radians.
 *
 * \sa ps_move_to, ps_line_to, ps_bezier_curve_to, ps_quad_curve_to, 
 *     ps_arc, ps_rectangle, ps_rounded_rect, ps_ellipse
 */
PEXPORT void PICAPI ps_tangent_arc(ps_context* ctx, const ps_rect* rect, float sangle, float sweep);

/**
 * \fn void ps_rectangle(ps_context* ctx, const ps_rect* rect)
 * \brief Add a rectangle to the current path.
 *
 * \param ctx   Pointer to an existing context object.
 * \param rect  The rectangle which will be added. 
 *
 * \sa ps_move_to, ps_line_to, ps_bezier_curve_to, ps_quad_curve_to, 
 *     ps_arc, ps_tangent_arc, ps_rounded_rect, ps_ellipse
 */
PEXPORT void PICAPI ps_rectangle(ps_context* ctx, const ps_rect* rect);

/**
 * \fn void ps_rounded_rect(ps_context* ctx, const ps_rect* rect, float ltx, float lty, float rtx, 
 *                                                  float rty, float lbx, float lby, float rbx, float rby)
 * \brief Add a rounded rectangle to the current path.
 *
 * \param ctx   Pointer to an existing context object.
 * \param rect  The rectangle which will be added. 
 * \param ltx   The left top horizontal radius. 
 * \param lty   The left top vertical radius.
 * \param rtx   The right top horizontal radius. 
 * \param rty   The right top vertical radius.
 * \param lbx   The left bottom horizontal radius. 
 * \param lby   The left bottom vertical radius.
 * \param rbx   The right bottom horizontal radius. 
 * \param rby   The right bottom vertical radius.
 *
 * \sa ps_move_to, ps_line_to, ps_bezier_curve_to, ps_quad_curve_to, 
 *     ps_arc, ps_tangent_arc, ps_rectangle, ps_ellipse
 */
PEXPORT void PICAPI ps_rounded_rect(ps_context* ctx, const ps_rect* rect, float ltx,
                      float lty, float rtx, float rty, float lbx, float lby, float rbx, float rby);

/**
 * \fn void ps_ellipse(ps_context* ctx, const ps_rect* rect)
 * \brief Adds an ellipse to the current path which fits inside the specified rectangle.
 *
 * \param ctx   Pointer to an existing context object.
 * \param rect  The rectangle which the ellipse to fit in.
 *
 * \sa ps_move_to, ps_line_to, ps_bezier_curve_to, ps_quad_curve_to, 
 *     ps_arc, ps_tangent_arc, ps_rectangle, ps_rounded_rect
 */
PEXPORT void PICAPI ps_ellipse(ps_context* ctx, const ps_rect* rect);

/** @} end of geometry functions*/

/**
 * \defgroup path Path
 * @{
 */

/**
 * \fn ps_path* ps_path_create(void)
 * \brief Create a new empty path object.
 *
 * \return If the function succeeds, the return value is the pointer to a new path object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_path_create_copy, ps_path_ref, ps_path_unref
 */
PEXPORT ps_path* PICAPI ps_path_create(void);

/**
 * \fn ps_path* ps_path_create_copy(const ps_path* path)
 * \brief Create a copy from an existing path object.
 *
 * \param path  Pointer to an existing path object.
 *
 * \return If the function succeeds, the return value is the pointer to a new path object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_path_create, ps_path_ref, ps_path_unref
 */
PEXPORT ps_path* PICAPI ps_path_create_copy(const ps_path* path);

/**
 * \fn ps_path* ps_path_ref(ps_path* path)
 * \brief Increases the reference count of the path by 1.
 *
 * \param path  Pointer to an existing path object.
 *
 * \return If the function succeeds, the return value is the pointer to the path object.
 *         If the function fails, the return value is NULL.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_path_create, ps_path_create_copy, ps_path_unref
 */
PEXPORT ps_path* PICAPI ps_path_ref(ps_path* path);

/**
 * \fn void ps_path_unref(ps_path* path)
 * \brief Decrements the reference count for the path object. 
 *        If the reference count on the path falls to 0, the path is freed.
 *
 * \param path  Pointer to an existing path object.
 *
 * \sa ps_path_create, ps_path_create_copy, ps_path_ref
 */
PEXPORT void PICAPI ps_path_unref(ps_path* path);

/**
 * \fn void ps_path_move_to(ps_path* path, const ps_point* point)
 * \brief Begin a new sub path, and set the current point in the path.
 *
 * \param path   Pointer to an existing path object.
 * \param point  The point which will be set. 
 *
 * \sa ps_path_line_to, ps_path_bezier_to, ps_path_quad_to
 *     ps_path_arc_to, ps_path_tangent_arc_to
 */
PEXPORT void PICAPI ps_path_move_to(ps_path* path, const ps_point* point);

/**
 * \fn void ps_path_line_to(ps_path* path, const ps_point* point)
 * \brief Add a line to the path from the current point to given point.
 *
 * \param path   Pointer to an existing path object.
 * \param point  The point which will be set. 
 *
 * \sa ps_path_move_to, ps_path_bezier_to, ps_path_quad_to
 *     ps_path_arc_to, ps_path_tangent_arc_to
 */
PEXPORT void PICAPI ps_path_line_to(ps_path* path, const ps_point* point);

/**
 * \fn void ps_path_tangent_arc_to(ps_path* path, float radius, const ps_point* tp, const ps_point* ep)
 * \brief Add an arc to the path which tangent at two line. 
 *
 * \param path    Pointer to an existing path object.
 * \param radius  The radius of the arc.
 * \param tp      The point which the first tangent line from current point to.
 * \param ep      The point which the second tangent line from \a tp to.
 *
 * \sa ps_path_move_to, ps_path_bezier_to, ps_path_quad_to
 *     ps_path_arc_to, ps_path_line_to
 */
PEXPORT void PICAPI ps_path_tangent_arc_to(ps_path* path, float radius,
                                            const ps_point* tp, const ps_point* ep);

/**
 * \fn void ps_path_arc_to(ps_path* path, float radiusX, float radiusY, float angle, 
 *                                                 ps_bool large_arc, ps_bool clockwise, const ps_point* ep)
 * \brief Add an arc to the path, using radius, angle and end point.
 *
 * \param path       Pointer to an existing path object.
 * \param radiusX    The horizontal radius of arc.
 * \param radiusY    The vertical radius of arc.
 * \param angle      The angle of arc, in radians.
 * \param large_arc  True is large arc, False is small arc.
 * \param clockwise  True is clockwise, False is counter clockwise.
 * \param ep         The end point of the arc.
 *
 * \sa ps_path_move_to, ps_path_bezier_to, ps_path_quad_to
 *     ps_path_tangent_arc_to, ps_path_line_to
 */
PEXPORT void PICAPI ps_path_arc_to(ps_path* path, float radiusX, float radiusY, float angle,
                                        ps_bool large_arc, ps_bool clockwise, const ps_point* ep);

/**
 * \fn void ps_path_bezier_to(ps_path* path, const ps_point* fcp, const ps_point* scp, const ps_point* ep)
 * \brief Add a cubic bezier spline to the path from current point to end point.
 *
 * \param path  Pointer to an existing path object.
 * \param fcp      The first control point of the curve.
 * \param scp      The second control point of the curve.
 * \param ep       The end point of the curve.
 *
 * \sa ps_path_move_to, ps_path_arc_to, ps_path_quad_to
 *     ps_path_tangent_arc_to, ps_path_line_to
 */
PEXPORT void PICAPI ps_path_bezier_to(ps_path* path, const ps_point* fcp,
                                                 const ps_point* scp, const ps_point* ep);

/**
 * \fn void ps_path_quad_to(ps_path* path, const ps_point* cp, const ps_point* ep)
 * \brief Add a quadratic bezier spline to the path from current point to end point.
 *
 * \param path  Pointer to an existing path object.
 * \param cp      The control point of the curve.
 * \param ep       The end point of the curve.
 *
 * \sa ps_path_move_to, ps_path_arc_to, ps_path_bezier_to
 *     ps_path_tangent_arc_to, ps_path_line_to
 */
PEXPORT void PICAPI ps_path_quad_to(ps_path* path, const ps_point* cp, const ps_point* ep);

/**
 * \fn void ps_path_sub_close(ps_path* path)
 * \brief Close the sub path, and begin a new one.
 *
 * \param path  Pointer to an existing path object.
 *
 * \sa ps_path_get_length, ps_path_clear, ps_path_is_empty
 */
PEXPORT void PICAPI ps_path_sub_close(ps_path* path);

/**
 * \fn float ps_path_get_length(const ps_path* path)
 * \brief Return The length of the path.
 *
 * \param path  Pointer to an existing path object.
 *
 * \return If the function succeeds, the return value is the length of the path object.
 *         If the function fails, the return value is 0.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_path_sub_close, ps_path_clear, ps_path_is_empty
 */
PEXPORT float PICAPI ps_path_get_length(const ps_path* path);

/**
 * \fn void ps_path_clear(ps_path* path)
 * \brief Clear the path to empty.
 *
 * \param path  Pointer to an existing path object.
 *
 * \sa ps_path_sub_close, ps_path_get_length, ps_path_is_empty
 */
PEXPORT void PICAPI ps_path_clear(ps_path* path);

/**
 * \fn ps_bool ps_path_is_empty(const ps_path* path)
 * \brief Checks whether the path is empty. 
 *
 * \param path  Pointer to an existing path object.
 *
 * \sa ps_path_sub_close, ps_path_get_length, ps_path_clear
 */
PEXPORT ps_bool PICAPI ps_path_is_empty(const ps_path* path);

/**
 * \fn unsigned int ps_path_get_vertex_count(const ps_path* path)
 * \brief Return the count of vertices in the path.
 *
 * \param path  Pointer to an existing path object.
 *
 * \return If the function succeeds, the return value is the vertices count in the path object.
 *         If the function fails, the return value is 0.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_path_get_vertex, ps_path_bounding_rect, ps_path_contains
 */
PEXPORT unsigned int PICAPI ps_path_get_vertex_count(const ps_path* path);

/**
 * \brief Path command for vertices.
 */
typedef enum _ps_path_cmd {
    /**
     * Stop command.
     */
    PATH_CMD_STOP     = 0,
    /**
     * Move to command.
     */
    PATH_CMD_MOVE_TO  = 1,
    /**
     * Line to command.
     */
    PATH_CMD_LINE_TO  = 2,
    /**
     * Quad curve to command.
     */
    PATH_CMD_CURVE3   = 3,
    /**
     * Bezier curve to command.
     */
    PATH_CMD_CURVE4   = 4,
    /**
     * End polyline command.
     */
    PATH_CMD_END_POLY = 0x0F,
}ps_path_cmd;

/**
 * \fn ps_path_cmd ps_path_get_vertex(const ps_path* path, unsigned int index, ps_point * point)
 * \brief Get a vertex from the path by index, and return the vertex command.
 *
 * \param path   Pointer to an existing path object.
 * \param index  The index of the vertex.
 * \param point  Pointer to a structure to receiving the vertex.
 *
 * \return If the function succeeds, the return value is the command of the vertex.
 *         If the function fails, the return value is PATH_CMD_STOP.
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_path_get_vertex_count, ps_path_bounding_rect, ps_path_contains
 */
PEXPORT ps_path_cmd PICAPI ps_path_get_vertex(const ps_path* path,
                                                unsigned int index, ps_point * point);

/**
 * \fn ps_rect ps_path_bounding_rect(const ps_path* path)
 * \brief Get the bounding rectangle of the path.
 *
 * \param path   Pointer to an existing path object.
 *
 * \return If the function succeeds, the return value is the bounding rectangle of the path.
 *         If the function fails, the return value is empty rectangle (0,0,0,0).
 *
 * \note To get extended error information, call \a ps_last_status.
 *
 * \sa ps_path_get_vertex_count, ps_path_get_vertex, ps_path_contains
 */
PEXPORT ps_rect PICAPI ps_path_bounding_rect(const ps_path* path);

/**
 * \fn ps_bool ps_path_contains(const ps_path* path, const ps_point* point, ps_fill_rule rule)
 * \brief Check whether a point is contained in the path.
 *
 * \param path   Pointer to an existing path object.
 * \param point  The point to be checked.
 * \param rule   The filling rule for the path.
 *
 * \return  True if the point is contained, otherwise False.
 *
 * \sa ps_path_get_vertex_count, ps_path_get_vertex, ps_path_bounding_rect
 */
PEXPORT ps_bool PICAPI ps_path_contains(const ps_path* path,
                                        const ps_point* point, ps_fill_rule rule);

/**
 * \fn void ps_path_add_line(ps_path* path, const ps_point* p1, const ps_point* p2)
 * \brief Add a line to the path.
 *
 * \param path  Pointer to an existing path object.
 * \param p1    The start point for the line.
 * \param p2    The end point for the line.
 *
 * \sa ps_path_add_arc, ps_path_add_rect, ps_path_add_ellipse, ps_path_add_rounded_rect
 */
PEXPORT void PICAPI ps_path_add_line(ps_path* path, const ps_point* p1, const ps_point* p2);

/**
 * \fn void ps_path_add_arc(ps_path* path, const ps_point* cp, float radius, float sangle,
 *                                                                float eangle, ps_bool clockwise)
 * \brief Add a arc to the path.
 *
 * \param path      Pointer to an existing path object.
 * \param cp        The center point of the arc.
 * \param radius    The radius of the arc.
 * \param sangle    The start angle, in radians.
 * \param eangle    The end angle, in radians.
 * \param clockwise True is clockwise, False is counter clockwise.
 *
 * \sa ps_path_add_line, ps_path_add_rect, ps_path_add_ellipse, ps_path_add_rounded_rect
 */
PEXPORT void PICAPI ps_path_add_arc(ps_path* path, const ps_point* cp, float radius,
                                            float sangle, float eangle, ps_bool clockwise);

/**
 * \fn void ps_path_add_rect(ps_path* path, const ps_rect* rect)
 * \brief Add a rectangle to the path.
 *
 * \param path  Pointer to an existing path object.
 * \param rect  The rectangle to be added.
 *
 * \sa ps_path_add_arc, ps_path_add_line, ps_path_add_ellipse, ps_path_add_rounded_rect
 */
PEXPORT void PICAPI ps_path_add_rect(ps_path* path, const ps_rect* rect);

/**
 * \fn void ps_path_add_ellipse(ps_path* path, const ps_rect* rect)
 * \brief Add an ellipse to the path.
 *
 * \param path  Pointer to an existing path object.
 * \param rect  The rectangle to enclose the ellipse.
 *
 * \sa ps_path_add_arc, ps_path_add_line, ps_path_add_rect, ps_path_add_rounded_rect
 */
PEXPORT void PICAPI ps_path_add_ellipse(ps_path* path, const ps_rect* rect);

/**
 * \fn void ps_path_add_rounded_rect(ps_path* path, const ps_rect* rect, float ltx, float lty,
 *                                            float rtx, float rty, float lbx, float lby, float rbx, float rby)
 * \brief Add a rounded rectangle to the path.
 *
 * \param path  Pointer to an existing path object.
 * \param rect  The rectangle which will be added. 
 * \param ltx   The left top horizontal radius. 
 * \param lty   The left top vertical radius.
 * \param rtx   The right top horizontal radius. 
 * \param rty   The right top vertical radius.
 * \param lbx   The left bottom horizontal radius. 
 * \param lby   The left bottom vertical radius.
 * \param rbx   The right bottom horizontal radius. 
 * \param rby   The right bottom vertical radius.
 *
 * \sa ps_path_add_arc, ps_path_add_line, ps_path_add_rect, ps_path_add_ellipse
 */
PEXPORT void PICAPI ps_path_add_rounded_rect(ps_path* path, const ps_rect* rect, float ltx,
                      float lty, float rtx, float rty, float lbx, float lby, float rbx, float rby);
/**
 * \brief Path clipping operations.
 */
typedef enum _ps_path_op {
    /**
     * Union.
     */
    PATH_OP_UNION,
    /**
     * Intersection. 
     */
    PATH_OP_INTERSECT,
    /**
     * Exclusive or.
     */
    PATH_OP_XOR,
    /**
     * Difference.
     */
    PATH_OP_DIFF,
}ps_path_operation;

/**
 * \fn void ps_path_clipping(ps_path* result, ps_path_operation op, const ps_path* a, const ps_path* b)
 *
 * \brief Clipping two path with the operation and get the result path.
 *
 * \param result  Pointer to an existing path object for result.
 * \param op      The specified operation for clipping.
 * \param a          The source path for clipping.
 * \param b          The path which will be clipping.
 *
 * \sa ps_path_get_vertex, ps_path_get_vertex_count
 */
PEXPORT void PICAPI ps_path_clipping(ps_path* result, ps_path_operation op,
                                                const ps_path* a, const ps_path* b);

/** @} end of path functions*/
/** @} end of graphic functions*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_PICASSO_H_*/
