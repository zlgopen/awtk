//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//
// See implementation agg_font_freetype.cpp
//
//----------------------------------------------------------------------------

#ifndef AGG_FONT_FREETYPE_INCLUDED
#define AGG_FONT_FREETYPE_INCLUDED

#include <ft2build.h>
#include FT_FREETYPE_H


#include "agg_scanline_storage_aa.h"
#include "agg_scanline_storage_bin.h"
#include "agg_scanline_u.h"
#include "agg_scanline_bin.h"
#include "agg_path_storage_integer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_conv_curve.h"
  #include "agg_font_cache_manager.h"
#include "agg_font_cache_manager2.h"
#include "agg_trans_affine.h"

namespace agg {
namespace fman {


    //-----------------------------------------------font_engine_freetype_base
    class font_engine_freetype_base
    {
    public:
		struct prepared_glyph
		{
			unsigned glyph_code;
			unsigned glyph_index;
			unsigned data_size;
			glyph_data_type data_type;
			rect_i bounds;
			double advance_x;
			double advance_y;
		};
		
		class loaded_face
		{
		public:
			loaded_face( font_engine_freetype_base &engine, FT_Face face )
				: m_engine( engine )
				, m_ft_face( face )
				, m_dpi( 0 )
				, m_height( 0 )
				, m_width( 0 )
				, m_rendering( glyph_ren_native_gray8 )
				, m_hinting( false )
				, m_flip_y( true )
				, m_char_map( FT_ENCODING_NONE )
			{
				set_face_name();
			}

			~loaded_face()
			{
				FT_Done_Face( m_ft_face);
				delete[] m_face_name;
			}

			unsigned num_faces() const
			{
				return m_ft_face->num_faces;
			} 

			const char *name() const
			{
				return m_face_name;
			}

			unsigned resolution() const
			{
				return m_dpi;
			}

			double height() const
			{
				return m_height;
			}

			double width() const
			{
				return m_width;
			}

		    double ascent() const
			{
				return m_ft_face->ascender*height()/m_ft_face->height;
			}

	        double descent() const
			{
				return m_ft_face->descender*height()/m_ft_face->height;
			}

		    double ascent_b() const
			{
				return m_ft_face->bbox.yMax*height()/m_ft_face->height;
			}

	        double descent_b() const
			{
				return m_ft_face->bbox.yMin*height()/m_ft_face->height;
			}

			bool hinting() const
			{
				return m_hinting;
			}

			bool flip_y() const
			{
				return m_flip_y;
			}

			const trans_affine &transform() const
			{
				return m_affine;
			}

			FT_Encoding char_map() const
			{
				return m_char_map;
			}

	        void set_hinting( bool h )
			{
				m_hinting=h;
			}

			/*
			void set_resolution(int dpi)
				{ m_dpi=dpi; update_char_size(); }
			void set_height( double h )
				{ m_height=int(h*64.0); update_char_size(); }
			void set_width( double w )
				{ m_width=int(w*64.0); update_char_size(); }
	        void set_flip_y(bool f)
				{ m_flip_y=f; }
			void set_transform(const trans_affine& affine)
				{ m_affine=affine; }
			int set_char_map(FT_Encoding char_map)
				{ FT_Select_Charmap(m_ft_face, char_map); }
			*/
			void select_instance(
				double height,
				double width,
				bool hinting,
				glyph_rendering rendering )
			{
				rendering=capable_rendering(rendering);
				
				if( m_height != height ||
					m_width != width ||
					m_hinting != hinting ||
					m_rendering != rendering )
				{
					m_height = height;
					m_width = width;
					m_hinting = hinting;
					m_rendering = rendering;
					update_char_size();
				}
			}

			glyph_rendering capable_rendering( glyph_rendering rendering ) const
			{
				switch(rendering)
				{
				case glyph_ren_native_mono:
				case glyph_ren_native_gray8:
					break;

				case glyph_ren_outline:
					if(!FT_IS_SCALABLE(m_ft_face))
						rendering = glyph_ren_native_gray8;
					break;

				case glyph_ren_agg_mono:
					if(!FT_IS_SCALABLE(m_ft_face))
						rendering = glyph_ren_native_mono;
					break;

				case glyph_ren_agg_gray8:
					if(!FT_IS_SCALABLE(m_ft_face))
						rendering = glyph_ren_native_gray8;
					break;
				};
				return rendering;
			}

			void update_char_size()
			{
				if( m_dpi )
					FT_Set_Char_Size( m_ft_face, int(m_width*64), int(m_height*64), m_dpi, m_dpi );
				else
					FT_Set_Pixel_Sizes( m_ft_face, int(m_width), int(m_height) );
			}

			bool prepare_glyph(unsigned glyph_code, prepared_glyph *prepared ) const;
	        bool add_kerning(unsigned first, unsigned second, double* x, double* y);
			void write_glyph_to(prepared_glyph *prepared, int8u* data) const;

		private:
			void set_face_name();

			loaded_face(const loaded_face &);
			loaded_face &operator=(const loaded_face &);

		private:
			font_engine_freetype_base &m_engine;
			FT_Face m_ft_face;
			char *m_face_name;
			int m_dpi;
			double m_height;
			double m_width;
	        glyph_rendering m_rendering;
			bool m_hinting;
			bool m_flip_y;
			trans_affine m_affine;
			FT_Encoding m_char_map;
		};

        //--------------------------------------------------------------------
        typedef serialized_scanlines_adaptor_aa<int8u>    gray8_adaptor_type;
        typedef serialized_scanlines_adaptor_bin          mono_adaptor_type;
        typedef scanline_storage_aa8                      scanlines_aa_type;
        typedef scanline_storage_bin                      scanlines_bin_type;

        //--------------------------------------------------------------------
        ~font_engine_freetype_base();
        font_engine_freetype_base(bool flag32, void *ftMemory=0);

		// Load families and faces
        //--------------------------------------------------------------------
		loaded_face *load_face(const void* buffer, size_t bytes);
		loaded_face *load_face_file(const char* file_name);
		loaded_face *create_loaded_face(FT_Face ft_face); // internal
		void unload_face(loaded_face *face);

        // Set Gamma
        //--------------------------------------------------------------------
        template<class GammaF> void gamma(const GammaF& f)
        {
            m_rasterizer.gamma(f);
        }

	private:
        font_engine_freetype_base(const font_engine_freetype_base&);
        const font_engine_freetype_base& operator = (const font_engine_freetype_base&);

        bool            m_flag32;
        int             m_last_error;
        bool            m_library_initialized;
		FT_Library      m_library;    // handle to library    

        path_storage_integer<int16, 6>              m_path16;
        path_storage_integer<int32, 6>              m_path32;
        conv_curve<path_storage_integer<int16, 6> > m_curves16;
        conv_curve<path_storage_integer<int32, 6> > m_curves32;
        scanline_u8              m_scanline_aa;
        scanline_bin             m_scanline_bin;
        scanlines_aa_type        m_scanlines_aa;
        scanlines_bin_type       m_scanlines_bin;
        rasterizer_scanline_aa<> m_rasterizer;
    };




    //------------------------------------------------font_engine_freetype_int16
    // This class uses values of type int16 (10.6 format) for the vector cache. 
    // The vector cache is compact, but when rendering glyphs of height
    // more that 200 there integer overflow can occur.
    //
    class font_engine_freetype_int16 : public font_engine_freetype_base
    {
    public:
        typedef serialized_integer_path_adaptor<int16, 6>     path_adaptor_type;
        typedef font_engine_freetype_base::gray8_adaptor_type gray8_adaptor_type;
        typedef font_engine_freetype_base::mono_adaptor_type  mono_adaptor_type;
        typedef font_engine_freetype_base::scanlines_aa_type  scanlines_aa_type;
        typedef font_engine_freetype_base::scanlines_bin_type scanlines_bin_type;

        font_engine_freetype_int16(void *ftMemory = 0) : 
            font_engine_freetype_base(false, ftMemory) {}
    };

    //------------------------------------------------font_engine_freetype_int32
    // This class uses values of type int32 (26.6 format) for the vector cache. 
    // The vector cache is twice larger than in font_engine_freetype_int16, 
    // but it allows you to render glyphs of very large sizes.
    //
    class font_engine_freetype_int32 : public font_engine_freetype_base
    {
    public:
        typedef serialized_integer_path_adaptor<int32, 6>     path_adaptor_type;
        typedef font_engine_freetype_base::gray8_adaptor_type gray8_adaptor_type;
        typedef font_engine_freetype_base::mono_adaptor_type  mono_adaptor_type;
        typedef font_engine_freetype_base::scanlines_aa_type  scanlines_aa_type;
        typedef font_engine_freetype_base::scanlines_bin_type scanlines_bin_type;

        font_engine_freetype_int32(void *ftMemory = 0) : 
            font_engine_freetype_base(true, ftMemory) {}
    };


}
}

#endif
