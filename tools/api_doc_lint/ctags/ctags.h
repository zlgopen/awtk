/*
*   $Id: ctags.h,v 1.4 2006/05/30 04:37:11 darren Exp $
*
*   Copyright (c) 1996-2002, Darren Hiebert
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License.
*
*   Program definitions
*/
#ifndef _CTAGS_H
#define _CTAGS_H


#include"entry.h"

/*
*   MACROS
*/
#ifndef PROGRAM_VERSION
# define PROGRAM_VERSION "5.6"
#endif
#define PROGRAM_NAME  "Exuberant Ctags"
#define PROGRAM_URL   "http://ctags.sourceforge.net"
#define AUTHOR_NAME   "Darren Hiebert"
#define AUTHOR_EMAIL  "dhiebert@users.sourceforge.net"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int (*close_ctags)(void);
	int (*parse_file)(const char *filename);
	int (*parse_buffer)(const char *filename, const char *buf, size_t len);
} ctags_export_t;
  typedef void (*EntryCallbackFunc)(const tagEntryInfo* const, char* const,  void *);
  ctags_export_t * get_ctags(EntryCallbackFunc callback, void * callback_param);
#ifdef __cplusplus
}
#endif

#endif	/* _CTAGS_H */

/* vi:set tabstop=4 shiftwidth=4: */
