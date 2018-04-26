/**
 * \file picasso_image_plugin.h
 * \author Zhang Ji Peng <onecoolx@gmail.com>
 * \date 2012/1/31
 *
 * This file includes all interfaces of image decoder backend.
 \verbatim

    Copyright (C) 2008 ~ 2016  Zhang Ji Peng

    All rights reserved.

    Picasso is a vector graphic library.

 \endverbatim
 */

#ifndef _PICASSO_IMAGE_PLUGININ_INTERFACE_H_
#define _PICASSO_IMAGE_PLUGININ_INTERFACE_H_

#include "picasso.h"
#include "picasso_image.h"

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
 * \defgroup types XImage Coder Plugin Data Types
 * @{
 */

/**
 * \brief Image reader header define.
 */
typedef struct _psx_image_header {
    /**  Private data for operator. */
    void* priv;
    /** Image width. */
    int width;
    /** Image height. */
    int height;
    /** Image pitch of scanline. */
    int pitch;
    /** Image depth. */
    int depth;
    /** Image bpp */
    int bpp;
    /** Image color format */
    int format;
    /** Image has alpha channel. (false: 0, true: 1) */
    int alpha;
    /** Image frame count. */
    int frames;
}psx_image_header;

/**
 * \brief The image operator define.
 * \sa psx_image_register_operator psx_image_unregister_operator
 */
typedef struct _psx_image_operator {
    /** Create a image reader header. */
    int (*read_header_info)(const ps_byte* data, size_t data_len, psx_image_header* header);
    /** Read a frame of raw data. */
    int (*decode_image_data)(psx_image_header* header, const psx_image* image, psx_image_frame* frame, int idx, ps_byte* buffer, size_t buffer_len);
    /** Release reader resources. */
    int (*release_read_header_info)(psx_image_header* header);
    /** Create a image writer header. */ 
    int (*write_header_info)(const psx_image* image, image_writer_fn func,
                                         void* param, float quality, psx_image_header* header);
    /** Write image data frames. */ 
    int (*encode_image_data)(psx_image_header* header, const psx_image* image, psx_image_frame* frame, int idx, const ps_byte* buffer, size_t buffer_len, int* ret);
    /** Release writer resources. */ 
    int (*release_write_header_info)(psx_image_header* header);
}psx_image_operator;

/**
 * \brief Priority level for image_operator.
 * \sa psx_image_register_operator
 */
typedef enum _psx_priority_level {
    /** Low level operator. */
    PRIORITY_EXTENTED = -1,
    /** Default level operator. */
    PRIORITY_DEFAULT  = 0,
    /** High level operator. */
    PRIORITY_MASTER   = 1,
}psx_priority_level;

/** @} end of plugin types */

/**
 * \defgroup interface XImage Coders Register functions
 * @{
 */

/**
 * \fn int psx_image_register_operator(const char* type, const ps_byte* signature, size_t sig_offset, size_t sig_len,
 *                                                   psx_priority_level level, psx_image_operator* coder)
 * \brief Register the image operator.
 *
 * \param type           The image operator short name. (i.e "png" "jpg" "gif" "bmp")
 * \param signature      The image signature.
 * \param sig_offset     The image signature offset from the beginning of the image data. (usually is 0)
 * \param sig_len        The image signature length.
 * \param level          The image operator priority level.
 * \param coder          The pointer to a image_operator object.
 *
 * \return Result code returned.
 *
 * \sa psx_image_unregister_operator
 */
PEXPORT int psx_image_register_operator(const char* type, const ps_byte* signature, size_t sig_offset, size_t sig_len,
                                                                    psx_priority_level level, psx_image_operator* coder);

/**
 * \fn int psx_image_unregister_operator(psx_image_operator* coder)
 * \brief Unregister the image operator. 
 *
 * \param coder The image operator which will be unregister.
 *
 * \return Result code returned.
 *
 * \sa psx_image_register_operator
 */
PEXPORT int psx_image_unregister_operator(psx_image_operator* coder);

/** @} end of image register functions */

/**
 * \defgroup coder XImage Coders Plugin side functions
 * @{
 */

/**
 * \fn void psx_image_module_init(void)
 * \brief Initialze the image module.
 *
 * \sa psx_image_module_shutdown psx_image_module_get_string
 */
PEXPORT void psx_image_module_init(void);

/**
 * \fn void psx_image_module_shutdown(void)
 * \brief Shutdown the image module.
 *
 * \sa psx_image_module_init psx_image_module_get_string
 */
PEXPORT void psx_image_module_shutdown(void);

/**
 * \def MODULE_NAME
 * \brief Get module name
 */
#define MODULE_NAME  1

/**
 * \fn char* psx_image_module_get_string(int id)
 * \brief Get the string info about module.
 * 
 * \param id  The information index.
 *
 * \return If successs, return the pointer of string. Can not be modify and free.
 *         If fails, return NULL.
 *
 * \sa psx_image_module_init psx_image_module_shutdown
 */
PEXPORT const char* psx_image_module_get_string(int id);

/** @} end of plugin side functions */
/** @} end of extimg */
/** @} end of extensions */

#ifdef __cplusplus
}
#endif

#endif /*_PICASSO_IMAGE_PLUGININ_INTERFACE_H_*/

