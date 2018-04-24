/***************************************************************************/
/*                                                                         */
/*  ftadvanc.h                                                             */
/*                                                                         */
/*    FreeType access the glyph advances       (specification only).       */
/*                                                                         */
/*  Copyright 1996-2001, 2002, 2003, 2004, 2005, 2006, 2007 by             */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/

#ifndef __FREETYPE_ADVANCE_H__
#define __FREETYPE_ADVANCE_H__

#include <ft2build.h>
#include FT_FREETYPE_H

  /*************************************************************************/
  /*                                                                       */
  /* <Const>                                                               */
  /*    FT_ADVANCE_FLAG_FAST_ONLY                                          */
  /*                                                                       */
  /* <Description>                                                         */
  /*    a bit-flag to be or-ed to the 'flags' parameter of the             */
  /*    @FT_Get_Advance and @FT_Get_Advances.                              */
  /*                                                                       */
  /*    when set, it indicates that you want these functions to fail       */
  /*    if the corresponding hinting mode or font driver doesn't           */
  /*    allow for very quick advance computation.                          */
  /*                                                                       */
  /*    typically, unscaled, unhinted, bitmapped and light-hinted glyphs   */
  /*    can have their advance width(s) computed very quickly.             */
  /*                                                                       */
  /*    not so for normal and bytecode hinted modes, which require         */
  /*    loading/scaling/hinting the glyph outline instead, which is        */
  /*    extremely slow by comparison                                       */
  /*                                                                       */
#define  FT_ADVANCE_FLAG_FAST_ONLY   0x20000000U


  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    FT_Get_Advance                                                     */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Retrieve the advance of a given glyph outline in a FT_Face.        */
  /*    by default, the unhinted advance is returned in font units         */
  /*                                                                       */
  /* <Input>                                                               */
  /*    face       :: source FT_Face handle                                */
  /*    gindex     :: glyph index                                          */
  /*    load_flags :: a set of bit-flags similar to those used             */
  /*                  when calling @FT_Load_Glyph, used to determine       */
  /*                  what kind of advances you need.                      */
  /* <Output>                                                              */
  /*    padvance :: the advance value, in either font units or 16.16       */
  /*                format.                                                */
  /*                                                                       */
  /*                if @FT_LOAD_VERTICAL_LAYOUT is set, this is the        */
  /*                vertical advance corresponding to a vertical layout.   */
  /*                otherwise, it's the horizontal advance in an           */
  /*                horizontal layout.                                     */
  /* <Return>                                                              */
  /*    FreeType error code.  0 means success.                             */
  /*                                                                       */
  /* <Note>                                                                */
  /*    This function may fail if you use @FT_ADVANCE_FLAG_FAST_ONLY and   */
  /*    if the corresponding's font backend doesn't have a quick way to    */
  /*    retrieve the advances.                                             */
  /*                                                                       */
  /*    A scaled advance is returned in 16.16 format, but isn't            */
  /*    transformed by the affine transform specified by @FT_Set_Transform */
  /*                                                                       */
  FT_EXPORT( FT_Error )
  FT_Get_Advance( FT_Face    face,
                  FT_UInt    gindex,
                  FT_UInt    load_flags,
                  FT_Fixed  *padvance );

  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    FT_Get_Advances                                                    */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Retrieve the advance of several glyph outlines in a FT_Face.       */
  /*    by default, the unhinted advances are returned in font units       */
  /*                                                                       */
  /* <Input>                                                               */
  /*    face        :: source FT_Face handle                               */
  /*    start       :: first glyph index                                   */
  /*    count       :: number of advances you want to retrieve             */
  /*    load_flags  :: a set of bit-flags similar to those used when       */
  /*                   calling @FT_Load_Glyph.                             */
  /*                                                                       */
  /* <Output>                                                              */
  /*    padvance :: the advances, in either font units or 16.16 format.    */
  /*                this array must contain at least 'count' elements      */
  /*                                                                       */
  /*                if @FT_LOAD_VERTICAL_LAYOUT is set, these are the      */
  /*                vertical advances corresponding to a vertical layout.  */
  /*                otherwise, they are the horizontal advances in an      */
  /*                horizontal layout.                                     */
  /* <Return>                                                              */
  /*    FreeType error code.  0 means success.                             */
  /*                                                                       */
  /* <Note>                                                                */
  /*    This function may fail if you use @FT_ADVANCE_FLAG_FAST_ONLY and   */
  /*    if the corresponding's font backend doesn't have a quick way to    */
  /*    retrieve the advances.                                             */
  /*                                                                       */
  /*    Scaled advances are returned in 16.16 format, but aren't           */
  /*    transformed by the affine transform specified by @FT_Set_Transform */
  /*                                                                       */
  FT_EXPORT( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    start,
                   FT_UInt    count,
                   FT_UInt    load_flags,
                   FT_Fixed  *padvances );

/* */

#endif /* __FREETYPE_ADVANCE_H__ */
