/*
*   $Id: parsers.h,v 1.12 2006/05/30 04:37:12 darren Exp $
*
*   Copyright (c) 2000-2003, Darren Hiebert
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License.
*
*   External interface to all language parsing modules.
*
*   To add a new language parser, you need only modify this single source
*   file to add the name of the parser definition function.
*/
#ifndef _PARSERS_H
#define _PARSERS_H

/* Add the name of any new parser definition function here */
#define PARSER_LIST \
	CParser, \
	CppParser, \
  CsharpParser, \
  JavaParser, \
  VeraParser

#endif  /* _PARSERS_H */

/* vi:set tabstop=4 shiftwidth=4: */
