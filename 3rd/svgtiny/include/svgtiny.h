/*
 * This file is part of Libsvgtiny
 * Licensed under the MIT License,
 *                http://opensource.org/licenses/mit-license.php
 * Copyright 2008 James Bursa <james@semichrome.net>
 */

#ifndef SVGTINY_H
#define SVGTINY_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef int svgtiny_colour;
#define svgtiny_TRANSPARENT 0x1000000
#define svgtiny_LINEAR_GRADIENT 0x2000000
#ifdef __riscos__
#define svgtiny_RGB(r, g, b) ((b) << 16 | (g) << 8 | (r))
#define svgtiny_RED(c) ((c) & 0xff)
#define svgtiny_GREEN(c) (((c) >> 8) & 0xff)
#define svgtiny_BLUE(c) (((c) >> 16) & 0xff)
#else
#define svgtiny_RGB(r, g, b) ((r) << 16 | (g) << 8 | (b))
#define svgtiny_RED(c) (((c) >> 16) & 0xff)
#define svgtiny_GREEN(c) (((c) >> 8) & 0xff)
#define svgtiny_BLUE(c) ((c) & 0xff)
#endif

struct svgtiny_linear_gradient {
	float sx;
	float sy;
	float ex;
	float ey;
	svgtiny_colour icolor;
	svgtiny_colour ocolor;
};

struct svgtiny_shape {
	float *path;
	unsigned int path_length;
	char *text;
	float text_x, text_y;
	svgtiny_colour fill;
	svgtiny_colour stroke;
	struct svgtiny_linear_gradient fill_linear_gradient;
	struct svgtiny_linear_gradient stroke_linear_gradient;
	float stroke_width;
};

struct svgtiny_diagram {
	int width, height;

	struct svgtiny_shape *shape;
	unsigned int shape_count;

	unsigned short error_line;
	const char *error_message;
};

typedef enum {
	svgtiny_OK,
	svgtiny_OUT_OF_MEMORY,
	svgtiny_LIBDOM_ERROR,
	svgtiny_NOT_SVG,
	svgtiny_SVG_ERROR
} svgtiny_code;

enum {
	svgtiny_PATH_MOVE,
	svgtiny_PATH_CLOSE,
	svgtiny_PATH_LINE,
	svgtiny_PATH_BEZIER
};

struct svgtiny_named_color {
	const char *name;
	svgtiny_colour color;
};


struct svgtiny_diagram *svgtiny_create(void);
svgtiny_code svgtiny_parse(struct svgtiny_diagram *diagram,
		const char *buffer, size_t size, const char *url,
		int width, int height);
void svgtiny_free(struct svgtiny_diagram *svg);

END_C_DECLS

#endif
