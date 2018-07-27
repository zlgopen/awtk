/* vim: set expandtab tabstop=4 softtabstop=4 shiftwidth=4: */

/*
 * Line breaking in a Unicode sequence.  Designed to be used in a
 * generic text renderer.
 *
 * Copyright (C) 2008-2016 Wu Yongwei <wuyongwei at gmail dot com>
 * Copyright (C) 2013 Petr Filipsky <philodej at gmail dot com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the author be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 *    not claim that you wrote the original software.  If you use this
 *    software in a product, an acknowledgement in the product
 *    documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must
 *    not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 * The main reference is Unicode Standard Annex 14 (UAX #14):
 *      <URL:http://www.unicode.org/reports/tr14/>
 *
 * When this library was designed, this annex was at Revision 19, for
 * Unicode 5.0.0:
 *      <URL:http://www.unicode.org/reports/tr14/tr14-19.html>
 *
 * This library has been updated according to Revision 37, for
 * Unicode 9.0.0:
 *      <URL:http://www.unicode.org/reports/tr14/tr14-37.html>
 *
 * The Unicode Terms of Use are available at
 *      <URL:http://www.unicode.org/copyright.html>
 */

/**
 * @file    linebreak.c
 *
 * Implementation of the line breaking algorithm as described in Unicode
 * Standard Annex 14.
 *
 * @author  Wu Yongwei
 * @author  Petr Filipsky
 */

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "linebreak.h"
#include "linebreakdef.h"

/**
 * Special value used internally to indicate an undefined break result.
 */
#define LINEBREAK_UNDEFINED -1

/**
 * Size of the second-level index to the line breaking properties.
 */
#define LINEBREAK_INDEX_SIZE 40

/**
 * Enumeration of break actions.  They are used in the break action
 * pair table #baTable.
 */
enum BreakAction
{
    DIR_BRK,        /**< Direct break opportunity */
    IND_BRK,        /**< Indirect break opportunity */
    CMI_BRK,        /**< Indirect break opportunity for combining marks */
    CMP_BRK,        /**< Prohibited break for combining marks */
    PRH_BRK         /**< Prohibited break */
};

/**
 * Break action pair table.  This is a direct mapping of Table 2 of
 * Unicode Standard Annex 14, Revision 37, except the "CB" part.
 */
static enum BreakAction baTable[LBP_CB][LBP_CB] = {
    {   /* OP */
        PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK,
        CMP_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK, PRH_BRK },
    {   /* CL */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, PRH_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* CP */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, PRH_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* QU */
        PRH_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK },
    {   /* GL */
        IND_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK },
    {   /* NS */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* EX */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* SY */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK, IND_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* IS */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, IND_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* PR */
        IND_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, IND_BRK,
        IND_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK },
    {   /* PO */
        IND_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, IND_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* NU */
        IND_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* AL */
        IND_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* HL */
        IND_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* ID */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, IND_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* IN */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* HY */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, DIR_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* BA */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, DIR_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* BB */
        IND_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        IND_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK },
    {   /* B2 */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, PRH_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* ZW */
        DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK },
    {   /* CM */
        IND_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* WJ */
        IND_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK,
        IND_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK },
    {   /* H2 */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, IND_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, IND_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* H3 */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, IND_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* JL */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, IND_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* JV */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, IND_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, IND_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* JT */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, IND_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* RI */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        IND_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* EB */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, IND_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK },
    {   /* EM */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, IND_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, IND_BRK, DIR_BRK },
    {   /* ZWJ */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, IND_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        IND_BRK, DIR_BRK, IND_BRK, IND_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, IND_BRK, IND_BRK, IND_BRK, DIR_BRK },
    {   /* CB */
        DIR_BRK, PRH_BRK, PRH_BRK, IND_BRK, IND_BRK, DIR_BRK, PRH_BRK,
        PRH_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, PRH_BRK,
        CMI_BRK, PRH_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK,
        DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK, DIR_BRK },
};

/**
 * Struct for the second-level index to the line breaking properties.
 */
struct LineBreakPropertiesIndex
{
    utf32_t end;                          /**< End coding point */
    const struct LineBreakProperties *lbp;/**< Pointer to line breaking properties */
};

/**
 * Second-level index to the line breaking properties.
 */
static struct LineBreakPropertiesIndex lb_prop_index[LINEBREAK_INDEX_SIZE] =
{
    { 0xFFFFFFFF, lb_prop_default }
};

/**
 * Checks whether the \a str ends with \a suffix, which has length
 * \a suffix_len.
 *
 * @param str        string whose ending is to be checked
 * @param suffix     string to check
 * @param suffixLen  length of \a suffix
 * @return           non-zero if true; zero otherwise
 */
static __inline int ends_with(const char *str, const char *suffix,
                              unsigned suffixLen)
{
    unsigned len;
    if (str == NULL)
    {
        return 0;
    }
    len = strlen(str);
    if (len >= suffixLen &&
        memcmp(str + len - suffixLen, suffix, suffixLen) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#define ENDS_WITH(str, suffix) ends_with((str), (suffix), sizeof(suffix) - 1)

/**
 * Initializes the second-level index to the line breaking properties.
 * If it is not called, the performance of #get_char_lb_class_lang (and
 * thus the main functionality) can be pretty bad, especially for big
 * code points like those of Chinese.
 */
void init_linebreak(void)
{
    size_t i;
    size_t iPropDefault;
    size_t len;
    size_t step;

    len = 0;
    while (lb_prop_default[len].prop != LBP_Undefined)
        ++len;
    step = len / LINEBREAK_INDEX_SIZE;
    iPropDefault = 0;
    for (i = 0; i < LINEBREAK_INDEX_SIZE; ++i)
    {
        lb_prop_index[i].lbp = lb_prop_default + iPropDefault;
        iPropDefault += step;
        lb_prop_index[i].end = lb_prop_default[iPropDefault].start - 1;
    }
    lb_prop_index[--i].end = 0xFFFFFFFF;
}

/**
 * Gets the language-specific line breaking properties.
 *
 * @param lang  language of the text
 * @return      pointer to the language-specific line breaking
 *              properties array if found; \c NULL otherwise
 */
static const struct LineBreakProperties *get_lb_prop_lang(const char *lang)
{
    const struct LineBreakPropertiesLang *lbplIter;
    if (lang != NULL)
    {
        for (lbplIter = lb_prop_lang_map; lbplIter->lang != NULL; ++lbplIter)
        {
            if (strncmp(lang, lbplIter->lang, lbplIter->namelen) == 0)
            {
                return lbplIter->lbp;
            }
        }
    }
    return NULL;
}

/**
 * Gets the line breaking class of a character from a line breaking
 * properties array.
 *
 * @param ch   character to check
 * @param lbp  pointer to the line breaking properties array
 * @return     the line breaking class if found; \c LBP_XX otherwise
 */
static enum LineBreakClass get_char_lb_class(
        utf32_t ch,
        const struct LineBreakProperties *lbp)
{
    while (lbp->prop != LBP_Undefined && ch >= lbp->start)
    {
        if (ch <= lbp->end)
            return lbp->prop;
        ++lbp;
    }
    return LBP_XX;
}

/**
 * Gets the line breaking class of a character from the default line
 * breaking properties array.
 *
 * @param ch  character to check
 * @return    the line breaking class if found; \c LBP_XX otherwise
 */
static enum LineBreakClass get_char_lb_class_default(
        utf32_t ch)
{
    size_t i = 0;
    while (ch > lb_prop_index[i].end)
        ++i;
    assert(i < LINEBREAK_INDEX_SIZE);
    return get_char_lb_class(ch, lb_prop_index[i].lbp);
}

/**
 * Gets the line breaking class of a character for a specific
 * language.  This function will check the language-specific data first,
 * and then the default data if there is no language-specific property
 * available for the character.
 *
 * @param ch       character to check
 * @param lbpLang  pointer to the language-specific line breaking
 *                 properties array
 * @return         the line breaking class if found; \c LBP_XX
 *                 otherwise
 */
static enum LineBreakClass get_char_lb_class_lang(
        utf32_t ch,
        const struct LineBreakProperties *lbpLang)
{
    enum LineBreakClass lbcResult;

    /* Find the language-specific line breaking class for a character */
    if (lbpLang)
    {
        lbcResult = get_char_lb_class(ch, lbpLang);
        if (lbcResult != LBP_XX)
            return lbcResult;
    }

    /* Find the generic language-specific line breaking class, if no
     * language context is provided, or language-specific data are not
     * available for the specific character in the specified language */
    return get_char_lb_class_default(ch);
}

/**
 * Resolves the line breaking class for certain ambiguous or complicated
 * characters.  They are treated in a simplistic way in this
 * implementation.
 *
 * @param lbc   line breaking class to resolve
 * @param lang  language of the text
 * @return      the resolved line breaking class
 */
static enum LineBreakClass resolve_lb_class(
        enum LineBreakClass lbc,
        const char *lang)
{
    switch (lbc)
    {
    case LBP_AI:
        if (lang != NULL &&
                (strncmp(lang, "zh", 2) == 0 || /* Chinese */
                 strncmp(lang, "ja", 2) == 0 || /* Japanese */
                 strncmp(lang, "ko", 2) == 0))  /* Korean */
        {
            return LBP_ID;
        }
        else
        {
            return LBP_AL;
        }
    case LBP_CJ:
        /* `Strict' and `normal' line breaking.  See
         * <url:http://www.unicode.org/reports/tr14/#CJ>
         * for details. */
        if (ENDS_WITH(lang, "-strict"))
        {
            return LBP_NS;
        }
        else
        {
            return LBP_ID;
        }
    case LBP_SA:
    case LBP_SG:
    case LBP_XX:
        return LBP_AL;
    default:
        return lbc;
    }
}

/**
 * Treats specially for the first character in a line.
 *
 * @param[in,out] lbpCtx  pointer to the line breaking context
 * @pre                   \a lbpCtx->lbcCur has a valid line break class
 * @post                  \a lbpCtx->lbcCur has the updated line break class
 */
static void treat_first_char(
        struct LineBreakContext *lbpCtx)
{
    switch (lbpCtx->lbcCur)
    {
    case LBP_LF:
    case LBP_NL:
        lbpCtx->lbcCur = LBP_BK;        /* Rule LB5 */
        break;
    case LBP_SP:
        lbpCtx->lbcCur = LBP_WJ;        /* Leading space treated as WJ */
        break;
    default:
        break;
    }
}

/**
 * Tries telling the line break opportunity by simple rules.
 *
 * @param[in,out] lbpCtx  pointer to the line breaking context
 * @pre                   \a lbpCtx->lbcCur has the current line break
 *                        class; and \a lbpCtx->lbcNew has the line
 *                        break class for the next character
 * @post                  \a lbpCtx->lbcCur has the updated line break
 *                        class
 * @return                break result, one of #LINEBREAK_MUSTBREAK,
 *                        #LINEBREAK_ALLOWBREAK, and #LINEBREAK_NOBREAK
 *                        if identified; or #LINEBREAK_UNDEFINED if
 *                        table lookup is needed
 */
static int get_lb_result_simple(
        struct LineBreakContext *lbpCtx)
{
    if (lbpCtx->lbcCur == LBP_BK
        || (lbpCtx->lbcCur == LBP_CR && lbpCtx->lbcNew != LBP_LF))
    {
        return LINEBREAK_MUSTBREAK;     /* Rules LB4 and LB5 */
    }

    switch (lbpCtx->lbcNew)
    {
    case LBP_SP:
        return LINEBREAK_NOBREAK;       /* Rule LB7; no change to lbcCur */
    case LBP_BK:
    case LBP_LF:
    case LBP_NL:
        lbpCtx->lbcCur = LBP_BK;        /* Mandatory break after */
        return LINEBREAK_NOBREAK;       /* Rule LB6 */
    case LBP_CR:
        lbpCtx->lbcCur = LBP_CR;
        return LINEBREAK_NOBREAK;       /* Rule LB6 */
    default:
        return LINEBREAK_UNDEFINED;     /* Table lookup is needed */
    }
}

/**
 * Tells the line break opportunity by table lookup.
 *
 * @param[in,out] lbpCtx  pointer to the line breaking context
 * @pre                   \a lbpCtx->lbcCur has the current line break
 *                        class; \a lbpCtx->lbcLast has the line break
 *                        class for the last character; and \a
 *                        lbcCur->lbcNew has the line break class for
 *                        the next character
 * @post                  \a lbpCtx->lbcCur has the updated line break
 *                        class
 * @return                break result, one of #LINEBREAK_MUSTBREAK,
 *                        #LINEBREAK_ALLOWBREAK, and #LINEBREAK_NOBREAK
 */
static int get_lb_result_lookup(
        struct LineBreakContext *lbpCtx)
{
    int brk = LINEBREAK_UNDEFINED;

    assert(lbpCtx->lbcCur <= LBP_CB);
    assert(lbpCtx->lbcNew <= LBP_CB);
    switch (baTable[lbpCtx->lbcCur - 1][lbpCtx->lbcNew - 1])
    {
    case DIR_BRK:
        brk = LINEBREAK_ALLOWBREAK;
        break;
    case IND_BRK:
        brk = (lbpCtx->lbcLast == LBP_SP)
            ? LINEBREAK_ALLOWBREAK
            : LINEBREAK_NOBREAK;
        break;
    case CMI_BRK:
        brk = LINEBREAK_ALLOWBREAK;
        if (lbpCtx->lbcLast != LBP_SP)
        {
            brk = LINEBREAK_NOBREAK;
            return brk;                 /* Do not update lbcCur */
        }
        break;
    case CMP_BRK:
        brk = LINEBREAK_NOBREAK;
        if (lbpCtx->lbcLast != LBP_SP)
            return brk;                 /* Do not update lbcCur */
        break;
    case PRH_BRK:
        brk = LINEBREAK_NOBREAK;
        break;
    }

    /* Special processing due to rule LB21a */
    if (lbpCtx->fLb21aHebrew &&
        (lbpCtx->lbcCur == LBP_HY || lbpCtx->lbcCur == LBP_BA))
    {
        brk = LINEBREAK_NOBREAK;
        lbpCtx->fLb21aHebrew = 0;
    }
    else
    {
        lbpCtx->fLb21aHebrew = (lbpCtx->lbcCur == LBP_HL);
    }

    /* Special processing due to rule LB30a */
    if (lbpCtx->lbcCur == LBP_RI)
    {
        lbpCtx->cLb30aRI++;
        if (lbpCtx->cLb30aRI == 2 && lbpCtx->lbcNew == LBP_RI)
        {
            brk = LINEBREAK_ALLOWBREAK;
            lbpCtx->cLb30aRI = 0;
        }
    }
    else
    {
        lbpCtx->cLb30aRI = 0;
    }

    lbpCtx->lbcCur = lbpCtx->lbcNew;
    return brk;
}

/**
 * Initializes line breaking context for a given language.
 *
 * @param[in,out] lbpCtx  pointer to the line breaking context
 * @param[in]     ch      the first character to process
 * @param[in]     lang    language of the input
 * @post                  the line breaking context is initialized
 */
void lb_init_break_context(
        struct LineBreakContext *lbpCtx,
        utf32_t ch,
        const char *lang)
{
    lbpCtx->lang = lang;
    lbpCtx->lbpLang = get_lb_prop_lang(lang);
    lbpCtx->lbcLast = LBP_Undefined;
    lbpCtx->lbcNew = LBP_Undefined;
    lbpCtx->lbcCur = resolve_lb_class(
                        get_char_lb_class_lang(ch, lbpCtx->lbpLang),
                        lbpCtx->lang);
    lbpCtx->fLb21aHebrew = 0;
    lbpCtx->cLb30aRI = 0;
    treat_first_char(lbpCtx);
}

/**
 * Updates LineBreakingContext for the next code point and returns
 * the detected break.
 *
 * @param[in,out] lbpCtx  pointer to the line breaking context
 * @param[in]     ch      Unicode code point
 * @return                break result, one of #LINEBREAK_MUSTBREAK,
 *                        #LINEBREAK_ALLOWBREAK, and #LINEBREAK_NOBREAK
 * @post                  the line breaking context is updated
 */
int lb_process_next_char(
        struct LineBreakContext *lbpCtx,
        utf32_t ch )
{
    int brk;

    lbpCtx->lbcLast = lbpCtx->lbcNew;
    lbpCtx->lbcNew = get_char_lb_class_lang(ch, lbpCtx->lbpLang);
    brk = get_lb_result_simple(lbpCtx);
    switch (brk)
    {
    case LINEBREAK_MUSTBREAK:
        lbpCtx->lbcCur = resolve_lb_class(lbpCtx->lbcNew, lbpCtx->lang);
        treat_first_char(lbpCtx);
        break;
    case LINEBREAK_UNDEFINED:
        lbpCtx->lbcNew = resolve_lb_class(lbpCtx->lbcNew, lbpCtx->lang);
        brk = get_lb_result_lookup(lbpCtx);
        break;
    default:
        break;
    }
    return brk;
}

/**
 * Sets the line breaking information for a generic input string.
 *
 * Currently, this implementation has customization for the following
 * ISO 639-1 language codes (for \a lang):
 *
 *  - de (German)
 *  - en (English)
 *  - es (Spanish)
 *  - fr (French)
 *  - ja (Japanese)
 *  - ko (Korean)
 *  - ru (Russian)
 *  - zh (Chinese)
 *
 * In addition, a suffix <code>"-strict"</code> may be added to indicate
 * strict (as versus normal) line-breaking behaviour.  See the <a
 * href="http://www.unicode.org/reports/tr14/#CJ">Conditional Japanese
 * Starter section of UAX #14</a> for more details.
 *
 * @param[in]  s             input string
 * @param[in]  len           length of the input
 * @param[in]  lang          language of the input
 * @param[out] brks          pointer to the output breaking data,
 *                           containing #LINEBREAK_MUSTBREAK,
 *                           #LINEBREAK_ALLOWBREAK, #LINEBREAK_NOBREAK,
 *                           or #LINEBREAK_INSIDEACHAR
 * @param[in] get_next_char  function to get the next UTF-32 character
 */
void set_linebreaks(
        const void *s,
        size_t len,
        const char *lang,
        char *brks,
        get_next_char_t get_next_char)
{
    utf32_t ch;
    struct LineBreakContext lbCtx;
    size_t posCur = 0;
    size_t posLast = 0;

    --posLast;  /* To be ++'d later */
    ch = get_next_char(s, len, &posCur);
    if (ch == EOS)
        return;
    lb_init_break_context(&lbCtx, ch, lang);

    /* Process a line till an explicit break or end of string */
    for (;;)
    {
        for (++posLast; posLast < posCur - 1; ++posLast)
        {
            brks[posLast] = LINEBREAK_INSIDEACHAR;
        }
        assert(posLast == posCur - 1);
        ch = get_next_char(s, len, &posCur);
        if (ch == EOS)
            break;
        brks[posLast] = lb_process_next_char(&lbCtx, ch);
    }

    assert(posLast == posCur - 1 && posCur <= len);
    /* Break after the last character */
    brks[posLast] = LINEBREAK_MUSTBREAK;
    /* When the input contains incomplete sequences */
    while (posCur < len)
    {
        brks[posCur++] = LINEBREAK_INSIDEACHAR;
    }
}

/**
 * Sets the line breaking information for a UTF-8 input string.
 *
 * @param[in]  s     input UTF-8 string
 * @param[in]  len   length of the input
 * @param[in]  lang  language of the input
 * @param[out] brks  pointer to the output breaking data, containing
 *                   #LINEBREAK_MUSTBREAK, #LINEBREAK_ALLOWBREAK,
 *                   #LINEBREAK_NOBREAK, or #LINEBREAK_INSIDEACHAR
 * @see #set_linebreaks for a note about \a lang.
 */
void set_linebreaks_utf8(
        const utf8_t *s,
        size_t len,
        const char *lang,
        char *brks)
{
    set_linebreaks(s, len, lang, brks,
                   (get_next_char_t)ub_get_next_char_utf8);
}

/**
 * Sets the line breaking information for a UTF-16 input string.
 *
 * @param[in]  s     input UTF-16 string
 * @param[in]  len   length of the input
 * @param[in]  lang  language of the input
 * @param[out] brks  pointer to the output breaking data, containing
 *                   #LINEBREAK_MUSTBREAK, #LINEBREAK_ALLOWBREAK,
 *                   #LINEBREAK_NOBREAK, or #LINEBREAK_INSIDEACHAR
 * @see #set_linebreaks for a note about \a lang.
 */
void set_linebreaks_utf16(
        const utf16_t *s,
        size_t len,
        const char *lang,
        char *brks)
{
    set_linebreaks(s, len, lang, brks,
                   (get_next_char_t)ub_get_next_char_utf16);
}

/**
 * Sets the line breaking information for a UTF-32 input string.
 *
 * @param[in]  s     input UTF-32 string
 * @param[in]  len   length of the input
 * @param[in]  lang  language of the input
 * @param[out] brks  pointer to the output breaking data, containing
 *                   #LINEBREAK_MUSTBREAK, #LINEBREAK_ALLOWBREAK,
 *                   #LINEBREAK_NOBREAK, or #LINEBREAK_INSIDEACHAR
 * @see #set_linebreaks for a note about \a lang.
 */
void set_linebreaks_utf32(
        const utf32_t *s,
        size_t len,
        const char *lang,
        char *brks)
{
    set_linebreaks(s, len, lang, brks,
                   (get_next_char_t)ub_get_next_char_utf32);
}

/**
 * Tells whether a line break can occur between two Unicode characters.
 * This is a wrapper function to expose a simple interface.  Generally
 * speaking, it is better to use #set_linebreaks_utf32 instead, since
 * complicated cases involving combining marks, spaces, etc. cannot be
 * correctly processed.
 *
 * @param char1  the first Unicode character
 * @param char2  the second Unicode character
 * @param lang   language of the input
 * @return       one of #LINEBREAK_MUSTBREAK, #LINEBREAK_ALLOWBREAK,
 *               #LINEBREAK_NOBREAK, or #LINEBREAK_INSIDEACHAR
 */
int is_line_breakable(
        utf32_t char1,
        utf32_t char2,
        const char *lang)
{
    utf32_t s[2];
    char brks[2];
    s[0] = char1;
    s[1] = char2;
    set_linebreaks_utf32(s, 2, lang, brks);
    return brks[0];
}
