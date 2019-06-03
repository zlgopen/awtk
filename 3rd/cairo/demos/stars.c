/*
 * Copyright © 2003 USC, Information Sciences Institute
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of the
 * University of Southern California not be used in advertising or
 * publicity pertaining to distribution of the software without
 * specific, written prior permission. The University of Southern
 * California makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * THE UNIVERSITY OF SOUTHERN CALIFORNIA DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL THE UNIVERSITY OF
 * SOUTHERN CALIFORNIA BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Carl D. Worth <cworth@isi.edu>
 */

#include <cairo.h>
#include <math.h>

#ifndef M_PI 
#define M_PI 3.1415926
#endif/*M_PI*/

void
draw_stars (cairo_t *cr, int width, int height);

void
star_path (cairo_t *cr);

#define WIDTH 600
#define HEIGHT 275
#define STRIDE (WIDTH * 4)

unsigned char image[STRIDE*HEIGHT];

int
main (void)
{
    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create_for_data (image, CAIRO_FORMAT_ARGB32,
						   WIDTH, HEIGHT, STRIDE);

    cr = cairo_create (surface);

    cairo_rectangle (cr, 0, 0, WIDTH, HEIGHT);
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_fill (cr);

    draw_stars (cr, WIDTH, HEIGHT);

    cairo_destroy (cr);

    cairo_surface_destroy (surface);

    return 0;
}

void
star_path (cairo_t *cr)
{
    int i;
    double theta = 4 * M_PI / 5.0;

    cairo_move_to (cr, 0, 0);
    for (i=0; i < 4; i++) {
	cairo_rel_line_to (cr, 1.0, 0);
	cairo_rotate (cr, theta);
    }
    cairo_close_path (cr);
}

void
draw_stars (cairo_t *cr, int width, int height)
{
    cairo_set_source_rgb (cr, 0, 0, 0);

    cairo_save (cr);
    {
	cairo_translate (cr, 5, height / 2.6);
	cairo_scale (cr, height, height);
	star_path (cr);
	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
	cairo_fill (cr);
    }
    cairo_restore (cr);

    cairo_save (cr);
    {
	cairo_translate (cr, width - height - 5, height / 2.6);
	cairo_scale (cr, height, height);
	star_path (cr);
	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_fill (cr);
    }
    cairo_restore (cr);
}
