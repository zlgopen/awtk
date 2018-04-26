/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "device.h"
#include "graphic_path.h"

#include "picasso.h"
#include "picasso_global.h"
#include "picasso_objects.h"
#include "picasso_matrix.h"

namespace picasso {

//inner function
static void _matrix_transform_rect(const trans_affine & matrix, ps_rect* rect)
{
    if ((matrix.shy() == FLT_TO_SCALAR(0.0)) && (matrix.shx() == FLT_TO_SCALAR(0.0))) {
        scalar x = matrix.sx() * FLT_TO_SCALAR(rect->x) + matrix.tx();
        scalar y = matrix.sy() * FLT_TO_SCALAR(rect->y) + matrix.ty();
        scalar w = matrix.sx() * FLT_TO_SCALAR(rect->w);
        scalar h = matrix.sy() * FLT_TO_SCALAR(rect->h);
        if ( w < 0 ) {
            w = -w;
            x -= w-1;
        }
        if ( h < 0 ) {
            h = -h;
            y -= h-1;
        }
        rect->x = SCALAR_TO_FLT(x); rect->y = SCALAR_TO_FLT(y);
        rect->w = SCALAR_TO_FLT(w); rect->h = SCALAR_TO_FLT(h);
    } else {
        scalar x, y, w, h, x0, y0;
        scalar xmin, ymin, xmax, ymax;
        x0 = x = FLT_TO_SCALAR(rect->x); y0 = y = FLT_TO_SCALAR(rect->y);
        w = FLT_TO_SCALAR(rect->w); h = FLT_TO_SCALAR(rect->h);
        matrix.transform(&x0, &y0);
        xmin = xmax = x0;
        ymin = ymax = y0;

        x = FLT_TO_SCALAR(rect->x+rect->w+1); y = FLT_TO_SCALAR(rect->y);
        matrix.transform(&x, &y);
        xmin = MIN(xmin, x);
        ymin = MIN(ymin, y);
        xmax = MAX(xmax, x);
        ymax = MAX(ymax, y);

        x = FLT_TO_SCALAR(rect->x+rect->w+1); y = FLT_TO_SCALAR(rect->y+rect->h+1);
        matrix.transform(&x, &y);
        xmin = MIN(xmin, x);
        ymin = MIN(ymin, y);
        xmax = MAX(xmax, x);
        ymax = MAX(ymax, y);

        x = FLT_TO_SCALAR(rect->x); y = FLT_TO_SCALAR(rect->y+rect->h+1);
        matrix.transform(&x, &y);
        xmin = MIN(xmin, x);
        ymin = MIN(ymin, y);
        xmax = MAX(xmax, x);
        ymax = MAX(ymax, y);

        //rebuild rect
        w = xmax - xmin;
        h = ymax - ymin;

        xmin -= (xmin - x0) / w;
        ymin -= (ymin - y0) / h;
        xmax -= (xmax - x0) / w;
        ymax -= (ymax - y0) / h;

        rect->x = SCALAR_TO_FLT(xmin); rect->y = SCALAR_TO_FLT(ymin);
        rect->w = SCALAR_TO_FLT(xmax-xmin+1); rect->h = SCALAR_TO_FLT(ymax-ymin+1);
    }
}

} //namespace picasso

#ifdef __cplusplus
extern "C" {
#endif

ps_matrix* PICAPI ps_matrix_create_init(float xx, float yx, float xy, float yy, float x0, float y0)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    ps_matrix *p = (ps_matrix*)mem_malloc(sizeof(ps_matrix));
    if (p) {
        p->refcount = 1;
        new ((void*)&(p->matrix)) picasso::trans_affine(FLT_TO_SCALAR(xx), FLT_TO_SCALAR(yx),
                               FLT_TO_SCALAR(xy), FLT_TO_SCALAR(yy), FLT_TO_SCALAR(x0), FLT_TO_SCALAR(y0));
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_matrix* PICAPI ps_matrix_create(void)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    ps_matrix *p = (ps_matrix*)mem_malloc(sizeof(ps_matrix));
    if (p) {
        p->refcount = 1;
        new ((void*)&(p->matrix)) picasso::trans_affine;
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_matrix* PICAPI ps_matrix_create_copy(const ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    ps_matrix *p = (ps_matrix*)mem_malloc(sizeof(ps_matrix));
    if (p) {
        p->refcount = 1;
        new ((void*)&(p->matrix))picasso::trans_affine(matrix->matrix);
        global_status = STATUS_SUCCEED;
        return p;
    } else {
        global_status = STATUS_OUT_OF_MEMORY;
        return 0;
    }
}

ps_matrix* PICAPI ps_matrix_ref(ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }
    matrix->refcount++;
    global_status = STATUS_SUCCEED;
    return matrix;
}

void PICAPI ps_matrix_unref(ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    matrix->refcount--;
    if (matrix->refcount <= 0) {
        (&matrix->matrix)->trans_affine::~trans_affine();
        mem_free(matrix);
    }
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_init(ps_matrix* matrix, float xx, float yx, float xy,
                                                        float yy, float x0, float y0)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    matrix->matrix = picasso::trans_affine(FLT_TO_SCALAR(xx), FLT_TO_SCALAR(yx),
                    FLT_TO_SCALAR(xy), FLT_TO_SCALAR(yy), FLT_TO_SCALAR(x0), FLT_TO_SCALAR(y0));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_translate(ps_matrix* matrix, float tx, float ty)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    matrix->matrix.translate(FLT_TO_SCALAR(tx), FLT_TO_SCALAR(ty));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_scale(ps_matrix* matrix, float sx, float sy)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    matrix->matrix.scale(FLT_TO_SCALAR(sx), FLT_TO_SCALAR(sy));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_rotate(ps_matrix* matrix, float angle)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    matrix->matrix.rotate(FLT_TO_SCALAR(angle));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_shear(ps_matrix* matrix, float sx, float sy)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    matrix->matrix.shear(FLT_TO_SCALAR(sx), FLT_TO_SCALAR(sy));
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_invert(ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    matrix->matrix.invert();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_flip_x(ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    matrix->matrix.flip_x();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_flip_y(ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    matrix->matrix.flip_y();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_reset(ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    matrix->matrix.reset();
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_multiply(ps_matrix* result, const ps_matrix* a, const ps_matrix* b)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!result || !a || !b) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    result->matrix = a->matrix * b->matrix;
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_transform_point(const ps_matrix* matrix, ps_point* point)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix || !point) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }
    scalar x = FLT_TO_SCALAR(point->x);
    scalar y = FLT_TO_SCALAR(point->y);

    matrix->matrix.transform(&x, &y);

    point->x = SCALAR_TO_FLT(x);
    point->y = SCALAR_TO_FLT(y);
    global_status = STATUS_SUCCEED;
}

ps_bool PICAPI ps_matrix_is_equal(const ps_matrix* a, const ps_matrix* b)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return False;
    }

    if (!a || !b) {
        global_status = STATUS_INVALID_ARGUMENT;
        return False;
    }

    global_status = STATUS_SUCCEED;
    if (a == b)
        return True;

    if (a->matrix == b->matrix)
        return True;

    return False;
}

ps_bool PICAPI ps_matrix_is_identity(const ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return False;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return False;
    }

    global_status = STATUS_SUCCEED;
    return matrix->matrix.is_identity() ? True : False;
}

float PICAPI ps_matrix_get_determinant(const ps_matrix* matrix)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return 0;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return 0;
    }

    global_status = STATUS_SUCCEED;
    return SCALAR_TO_FLT(matrix->matrix.determinant());
}

void PICAPI ps_matrix_set_translate_factor(ps_matrix* matrix, float tx, float ty)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    matrix->matrix.tx(FLT_TO_SCALAR(tx));
    matrix->matrix.ty(FLT_TO_SCALAR(ty));
    global_status = STATUS_SUCCEED;
}

ps_bool PICAPI ps_matrix_get_translate_factor(ps_matrix* matrix, float *tx, float *ty)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return False;
    }

    if (!matrix || !tx || !ty) {
        global_status = STATUS_INVALID_ARGUMENT;
        return False;
    }

    *tx = SCALAR_TO_FLT(matrix->matrix.tx());
    *ty = SCALAR_TO_FLT(matrix->matrix.ty());
    global_status = STATUS_SUCCEED;
    return True;
}

void PICAPI ps_matrix_set_scale_factor(ps_matrix* matrix, float sx, float sy)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    matrix->matrix.sx(FLT_TO_SCALAR(sx));
    matrix->matrix.sy(FLT_TO_SCALAR(sy));
    global_status = STATUS_SUCCEED;
}

ps_bool PICAPI ps_matrix_get_scale_factor(ps_matrix* matrix, float *sx, float *sy)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return False;
    }

    if (!matrix || !sx || !sy) {
        global_status = STATUS_INVALID_ARGUMENT;
        return False;
    }

    *sx = SCALAR_TO_FLT(matrix->matrix.sx());
    *sy = SCALAR_TO_FLT(matrix->matrix.sy());
    global_status = STATUS_SUCCEED;
    return True;
}

void PICAPI ps_matrix_set_shear_factor(ps_matrix* matrix, float shx, float shy)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    matrix->matrix.shx(FLT_TO_SCALAR(shx));
    matrix->matrix.shy(FLT_TO_SCALAR(shy));
    global_status = STATUS_SUCCEED;
}

ps_bool PICAPI ps_matrix_get_shear_factor(ps_matrix* matrix, float *shx, float *shy)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return False;
    }

    if (!matrix || !shx || !shy) {
        global_status = STATUS_INVALID_ARGUMENT;
        return False;
    }

    *shx = SCALAR_TO_FLT(matrix->matrix.shx());
    *shy = SCALAR_TO_FLT(matrix->matrix.shy());
    global_status = STATUS_SUCCEED;
    return True;
}

void PICAPI ps_matrix_transform_rect(const ps_matrix* matrix, ps_rect* rect)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix || !rect) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    picasso::_matrix_transform_rect(matrix->matrix, rect);
    global_status = STATUS_SUCCEED;
}

void PICAPI ps_matrix_transform_path(const ps_matrix* matrix, ps_path* path)
{
    if (!picasso::is_valid_system_device()) {
        global_status = STATUS_DEVICE_ERROR;
        return;
    }

    if (!matrix || !path) {
        global_status = STATUS_INVALID_ARGUMENT;
        return;
    }

    path->path.transform_all_paths(matrix->matrix);
    global_status = STATUS_SUCCEED;
}

#ifdef __cplusplus
}
#endif
