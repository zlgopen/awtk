/*
*   $Id: e_mac.h,v 1.2 2006/05/30 04:37:12 darren Exp $
*
*   Copyright (c) 2001, Maarten L. Hekkelman
*
*   Author: Maarten L. Hekkelman <maarten@hekkelman.com>
*           http://www.hekkelman.com
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License. It is provided on an as-is basis and no
*   responsibility is accepted for its failure to perform as expected.
*
*   Configures ctags for Macintosh environment.
*/
#ifndef E_MAC_H
#define E_MAC_H


#define MACROS_USE_PATTERNS	1
#define DEFAULT_FILE_FORMAT 2
#define INTERNAL_SORT 1
#define TMPDIR "/tmp"
#define NEED_PROTO_TRUNCATE 1
#define STDC_HEADERS 1
#define HAVE_CLOCK 1
#define HAVE_FGETPOS 1
#define HAVE_OPENDIR 1
#define HAVE_REMOVE 1
#define HAVE_SETENV 1
#define HAVE_STRERROR 1
#define HAVE_STRSTR 1
#define HAVE_STRING_H 1
#define HAVE_SYS_DIR_H 1
#define HAVE_SYS_TIMES_H 1
#define HAVE_TIME_H 1
#define HAVE_SYS_TYPES_H 1

#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>

#endif
/* vi:set tabstop=4 shiftwidth=4: */
