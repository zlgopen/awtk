#include <math.h>
#include <stdio.h>
#include <time.h>
#include "agg_rendering_buffer.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_conv_dash.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_pattern_filters_rgba.h"
#include "agg_renderer_outline_aa.h"
#include "agg_renderer_outline_image.h"
#include "agg_arc.h"
#include "agg_bezier_arc.h"
#include "agg_pixfmt_rgb.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_bezier_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"


enum flip_y_e { flip_y = true };

typedef agg::pixfmt_bgr24 pixfmt;


void bezier4_point(double x1, double y1, double x2, double y2,
                   double x3, double y3, double x4, double y4,
                   double mu,
                   double* x, double* y)
{
   double mum1, mum13, mu3;

   mum1 = 1 - mu;
   mum13 = mum1 * mum1 * mum1;
   mu3 = mu * mu * mu;

   *x = mum13*x1 + 3*mu*mum1*mum1*x2 + 3*mu*mu*mum1*x3 + mu3*x4;
   *y = mum13*y1 + 3*mu*mum1*mum1*y2 + 3*mu*mu*mum1*y3 + mu3*y4;
}




class the_application : public agg::platform_support
{
    agg::rgba8 m_ctrl_color;
    agg::bezier_ctrl<agg::rgba8> m_curve1;
    agg::slider_ctrl<agg::rgba8> m_angle_tolerance;
    agg::slider_ctrl<agg::rgba8> m_approximation_scale;
    agg::slider_ctrl<agg::rgba8> m_cusp_limit;
    agg::slider_ctrl<agg::rgba8> m_width;
    agg::cbox_ctrl<agg::rgba8>   m_show_points;
    agg::cbox_ctrl<agg::rgba8>   m_show_outline;
    agg::rbox_ctrl<agg::rgba8>   m_curve_type;
    agg::rbox_ctrl<agg::rgba8>   m_case_type;
    agg::rbox_ctrl<agg::rgba8>   m_inner_join;
    agg::rbox_ctrl<agg::rgba8>   m_line_join;
    agg::rbox_ctrl<agg::rgba8>   m_line_cap;

    int m_cur_case_type;

public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_scanline;
    typedef agg::rasterizer_scanline_aa<> rasterizer_scanline;
    typedef agg::scanline_u8 scanline;


    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_ctrl_color(agg::rgba(0, 0.3, 0.5, 0.8)),
        m_angle_tolerance    (5.0,       5.0, 240.0,       12.0,  !flip_y),
        m_approximation_scale(5.0,    17+5.0, 240.0,    17+12.0,  !flip_y),
        m_cusp_limit         (5.0, 17+17+5.0, 240.0, 17+17+12.0,  !flip_y),
        m_width              (245.0,     5.0, 495.0,       12.0,  !flip_y),
        m_show_points        (250.0, 15+5, "Show Points",         !flip_y),
        m_show_outline       (250.0, 30+5, "Show Stroke Outline", !flip_y),
        m_curve_type         (535.0,   5.0, 535.0+115.0,   55.0,  !flip_y),
        m_case_type          (535.0,  60.0, 535.0+115.0,   195.0, !flip_y),
        m_inner_join         (535.0, 200.0, 535.0+115.0,   290.0, !flip_y),
        m_line_join          (535.0, 295.0, 535.0+115.0,   385.0, !flip_y),
        m_line_cap           (535.0, 395.0, 535.0+115.0,   455.0, !flip_y),
        m_cur_case_type(-1)
    {
        m_curve1.line_color(m_ctrl_color);

        m_curve1.curve(170, 424, 13, 87, 488, 423, 26, 333);
        //m_curve1.curve(26.000, 333.000, 276.000, 126.000, 402.000, 479.000, 26.000, 333.000); // Loop with p1==p4
        //m_curve1.curve(378.000, 439.000, 378.000, 497.000, 487.000, 432.000, 14.000, 338.000); // Narrow loop
        //m_curve1.curve(288.000, 283.000, 232.000, 89.000, 66.000, 197.000, 456.000, 241.000); // Loop
        //m_curve1.curve(519.000, 142.000, 97.000, 147.000, 69.000, 147.000, 30.000, 144.000); // Almost straight
        //m_curve1.curve(100, 100, 200, 100, 100, 200, 200, 200); // A "Z" case
        //m_curve1.curve(150, 150, 350, 150, 150, 150, 350, 150); // Degenerate
        //m_curve1.curve(409, 330, 300, 200, 200, 200, 401, 263); // Strange cusp
        //m_curve1.curve(129, 233, 172, 320, 414, 253, 344, 236); // Curve cap
        //m_curve1.curve(100,100, 100,200, 100,100, 110,100); // A "boot"
        //m_curve1.curve(225, 150, 60, 150, 460, 150, 295, 150); // 2----1----4----3
        //m_curve1.curve(162.2, 248.801, 162.2, 248.801, 266, 284, 394, 335);  // Coinciding 1-2
        //m_curve1.curve(162.200, 248.801, 162.200, 248.801, 257.000, 301.000, 394.000, 335.000); // Coinciding 1-2
        //m_curve1.curve(394.000, 335.000, 257.000, 301.000, 162.200, 248.801, 162.200, 248.801); // Coinciding 3-4
        //m_curve1.curve(84.200000,302.80100, 84.200000,302.80100, 79.000000,292.40100, 97.001000,304.40100); // From tiger.svg
        //m_curve1.curve(97.001000,304.40100, 79.000000,292.40100, 84.200000,302.80100, 84.200000,302.80100); // From tiger.svg opposite dir
        //m_curve1.curve(475, 157, 200, 100, 453, 100, 222, 157); // Cusp, failure for Adobe SVG
        add_ctrl(m_curve1);
        m_curve1.no_transform();

        m_angle_tolerance.label("Angle Tolerance=%.0f deg");
        m_angle_tolerance.range(0, 90);
        m_angle_tolerance.value(15);
        add_ctrl(m_angle_tolerance);
        m_angle_tolerance.no_transform();

        m_approximation_scale.label("Approximation Scale=%.3f");
        m_approximation_scale.range(0.1, 5);
        m_approximation_scale.value(1.0);
        add_ctrl(m_approximation_scale);
        m_approximation_scale.no_transform();

        m_cusp_limit.label("Cusp Limit=%.0f deg");
        m_cusp_limit.range(0, 90);
        m_cusp_limit.value(0);
        add_ctrl(m_cusp_limit);
        m_cusp_limit.no_transform();

        m_width.label("Width=%.2f");
        m_width.range(-50, 100);
        m_width.value(50.0);
        add_ctrl(m_width);
        m_width.no_transform();

        add_ctrl(m_show_points);
        m_show_points.no_transform();
        m_show_points.status(true);

        add_ctrl(m_show_outline);
        m_show_outline.no_transform();
        m_show_outline.status(true);

        m_curve_type.add_item("Incremental");
        m_curve_type.add_item("Subdiv");
        m_curve_type.cur_item(1);
        add_ctrl(m_curve_type);
        m_curve_type.no_transform();

        m_case_type.text_size(7);
        m_case_type.text_thickness(1.0);
        m_case_type.add_item("Random");
        m_case_type.add_item("13---24");
        m_case_type.add_item("Smooth Cusp 1");
        m_case_type.add_item("Smooth Cusp 2");
        m_case_type.add_item("Real Cusp 1");
        m_case_type.add_item("Real Cusp 2");
        m_case_type.add_item("Fancy Stroke");
        m_case_type.add_item("Jaw");
        m_case_type.add_item("Ugly Jaw");
        add_ctrl(m_case_type);
        m_case_type.no_transform();

        m_inner_join.text_size(8);
        m_inner_join.add_item("Inner Bevel");
        m_inner_join.add_item("Inner Miter");
        m_inner_join.add_item("Inner Jag");
        m_inner_join.add_item("Inner Round");
        m_inner_join.cur_item(3);
        add_ctrl(m_inner_join);
        m_inner_join.no_transform();

        m_line_join.text_size(8);
        m_line_join.add_item("Miter Join");
        m_line_join.add_item("Miter Revert");
        m_line_join.add_item("Round Join");
        m_line_join.add_item("Bevel Join");
        m_line_join.add_item("Miter Round");

        m_line_join.cur_item(1);
        add_ctrl(m_line_join);
        m_line_join.no_transform();

        m_line_cap.text_size(8);
        m_line_cap.add_item("Butt Cap");
        m_line_cap.add_item("Square Cap");
        m_line_cap.add_item("Round Cap");
        m_line_cap.cur_item(0);
        add_ctrl(m_line_cap);
        m_line_cap.no_transform();
    }


    template<class Curve> double measure_time(Curve& curve)
    {
        start_timer();
        for(int i = 0; i < 100; i++)
        {
            double x, y;
            curve.init(m_curve1.x1(), m_curve1.y1(),
                       m_curve1.x2(), m_curve1.y2(),
                       m_curve1.x3(), m_curve1.y3(),
                       m_curve1.x4(), m_curve1.y4());
            curve.rewind(0);
            while(!agg::is_stop(curve.vertex(&x, &y)));
        }
        return elapsed_time() * 10;
    }


    template<class Path> 
    bool find_point(const Path& path, double dist, unsigned* i, unsigned* j)
    {
        int k;
        *j = path.size() - 1;
          
        for(*i = 0; (*j - *i) > 1; ) 
        {
            if(dist < path[k = (*i + *j) >> 1].dist) *j = k; 
            else                                     *i = k;
        }
        return true;
    }

    struct curve_point
    {
        curve_point() {}
        curve_point(double x1, double y1, double mu1) : x(x1), y(y1), mu(mu1) {}
        double x, y, dist, mu;
    };

    template<class Curve> double calc_max_error(Curve& curve, double scale, 
                                                double* max_angle_error)
    {
        curve.approximation_scale(m_approximation_scale.value() * scale);
        curve.init(m_curve1.x1(), m_curve1.y1(),
                   m_curve1.x2(), m_curve1.y2(),
                   m_curve1.x3(), m_curve1.y3(),
                   m_curve1.x4(), m_curve1.y4());

        agg::pod_bvector<agg::vertex_dist, 8> curve_points;
        unsigned cmd;
        double x, y;
        curve.rewind(0);
        while(!agg::is_stop(cmd = curve.vertex(&x, &y)))
        {
            if(agg::is_vertex(cmd))
            {
                curve_points.add(agg::vertex_dist(x, y));
            }
        }
        unsigned i;
        double curve_dist = 0;
        for(i = 1; i < curve_points.size(); i++)
        {
            curve_points[i - 1].dist = curve_dist;
            curve_dist += agg::calc_distance(curve_points[i-1].x, curve_points[i-1].y, 
                                             curve_points[i].x,   curve_points[i].y);
        }
        curve_points[curve_points.size() - 1].dist = curve_dist;
        
        agg::pod_bvector<curve_point, 8> reference_points;
        for(i = 0; i < 4096; i++)
        {
            double mu = i / 4095.0;
            bezier4_point(m_curve1.x1(), m_curve1.y1(),
                          m_curve1.x2(), m_curve1.y2(),
                          m_curve1.x3(), m_curve1.y3(),
                          m_curve1.x4(), m_curve1.y4(),
                          mu, &x, &y);
            reference_points.add(curve_point(x, y, mu));
        }

        double reference_dist = 0;
        for(i = 1; i < reference_points.size(); i++)
        {
            reference_points[i - 1].dist = reference_dist;
            reference_dist += agg::calc_distance(reference_points[i-1].x, reference_points[i-1].y, 
                                                 reference_points[i].x,   reference_points[i].y);
        }
        reference_points[reference_points.size() - 1].dist = reference_dist;


        unsigned idx1 = 0;
        unsigned idx2 = 1;
        double max_error = 0;
        for(i = 0; i < reference_points.size(); i++)
        {
            if(find_point(curve_points, reference_points[i].dist, &idx1, &idx2))
            {
                double err = fabs(agg::calc_line_point_distance(curve_points[idx1].x,  curve_points[idx1].y,
                                                                curve_points[idx2].x,  curve_points[idx2].y,
                                                                reference_points[i].x, reference_points[i].y));
                if(err > max_error) max_error = err;
            }
        }

        double aerr = 0;
        for(i = 2; i < curve_points.size(); i++)
        {
            double a1 = atan2(curve_points[i-1].y - curve_points[i-2].y, 
                              curve_points[i-1].x - curve_points[i-2].x);
            double a2 = atan2(curve_points[i].y - curve_points[i-1].y, 
                              curve_points[i].x - curve_points[i-1].x);

            double da = fabs(a1 - a2);
            if(da >= agg::pi) da = 2*agg::pi - da;
            if(da > aerr) aerr = da;
        }


        *max_angle_error = aerr * 180.0 / agg::pi;
        return max_error * scale;
    }



    virtual void on_draw()
    {
        pixfmt pf(rbuf_window());
        renderer_base ren_base(pf);
        ren_base.clear(agg::rgba(1.0, 1.0, 0.95));
        renderer_scanline ren(ren_base);

        rasterizer_scanline ras;
        scanline sl;

        agg::path_storage path;

        double x, y;
        double curve_time = 0;

        path.remove_all();
        agg::curve4 curve;
        curve.approximation_method(agg::curve_approximation_method_e(m_curve_type.cur_item()));
        curve.approximation_scale(m_approximation_scale.value());
        curve.angle_tolerance(agg::deg2rad(m_angle_tolerance.value()));
        curve.cusp_limit(agg::deg2rad(m_cusp_limit.value()));
        curve_time = measure_time(curve);
        double max_angle_error_01 = 0;
        double max_angle_error_1 = 0;
        double max_angle_error1 = 0;
        double max_angle_error_10 = 0;
        double max_angle_error_100 = 0;
        double max_error_01 = 0;
        double max_error_1 = 0;
        double max_error1 = 0;
        double max_error_10 = 0;
        double max_error_100 = 0;

        max_error_01   = calc_max_error(curve, 0.01, &max_angle_error_01);
        max_error_1    = calc_max_error(curve, 0.1,  &max_angle_error_1);
        max_error1     = calc_max_error(curve, 1,    &max_angle_error1);
        max_error_10   = calc_max_error(curve, 10,   &max_angle_error_10);
        max_error_100  = calc_max_error(curve, 100,  &max_angle_error_100);

        curve.approximation_scale(m_approximation_scale.value());
        curve.angle_tolerance(agg::deg2rad(m_angle_tolerance.value()));
        curve.cusp_limit(agg::deg2rad(m_cusp_limit.value()));
        curve.init(m_curve1.x1(), m_curve1.y1(),
                   m_curve1.x2(), m_curve1.y2(),
                   m_curve1.x3(), m_curve1.y3(),
                   m_curve1.x4(), m_curve1.y4());

        path.concat_path(curve);
//path.move_to(m_curve1.x1(), m_curve1.y1());
//path.line_to(m_curve1.x2(), m_curve1.y2());
//path.line_to(m_curve1.x3(), m_curve1.y3());
//path.line_to(m_curve1.x4(), m_curve1.y4());


        agg::conv_stroke<agg::path_storage> stroke(path);
        stroke.width(m_width.value());
        stroke.line_join(agg::line_join_e(m_line_join.cur_item()));
        stroke.line_cap(agg::line_cap_e(m_line_cap.cur_item()));
        stroke.inner_join(agg::inner_join_e(m_inner_join.cur_item()));
        stroke.inner_miter_limit(1.01);

        ras.add_path(stroke);
        ren.color(agg::rgba(0, 0.5, 0, 0.5));
        agg::render_scanlines(ras, sl, ren);

        unsigned cmd;
        unsigned num_points1 = 0;
        path.rewind(0);
        while(!agg::is_stop(cmd = path.vertex(&x, &y)))
        {
            if(m_show_points.status())
            {
                agg::ellipse ell(x, y, 1.5, 1.5, 8);
                ras.add_path(ell);
                ren.color(agg::rgba(0,0,0, 0.5));
                agg::render_scanlines(ras, sl, ren);
            }
            ++num_points1;
        }

        if(m_show_outline.status())
        {
            // Draw a stroke of the stroke to see the internals
            //--------------
            agg::conv_stroke<agg::conv_stroke<agg::path_storage> > stroke2(stroke);
            ras.add_path(stroke2);
            ren.color(agg::rgba(0,0,0, 0.5));
            agg::render_scanlines(ras, sl, ren);
        }

        // Check ellipse and arc for the number of points
        //---------------
        //agg::ellipse a(100, 100, m_width.value(), m_width.value(), 0);
        //ras.add_path(a);
        //ren.color(agg::rgba(0.5,0,0, 0.5));
        //agg::render_scanlines(ras, sl, ren);
        //a.rewind(0);
        //while(!agg::is_stop(cmd = a.vertex(&x, &y)))
        //{
        //    if(agg::is_vertex(cmd))
        //    {
        //        agg::ellipse ell(x, y, 1.5, 1.5, 8);
        //        ras.add_path(ell);
        //        ren.color(agg::rgba(0,0,0,0.5));
        //        agg::render_scanlines(ras, sl, ren);
        //    }
        //}


        // Check a circle with huge radius (10,000,000) and high approximation accuracy
        //---------------
        //double circle_pnt_count = 0;
        //agg::bezier_arc ell(0,0, 10000000, 10000000, 0, 2*agg::pi);
        //agg::conv_curve<agg::bezier_arc, agg::curve3_div, agg::curve4_div3> crv(ell);
        //crv.approximation_scale(10.0);
        //crv.rewind(0);
        //while(crv.vertex(&x, &y)) ++circle_pnt_count;


        char buf[512]; 
        agg::gsv_text t;
        t.size(8.0);

        agg::conv_stroke<agg::gsv_text> pt(t);
        pt.line_cap(agg::round_cap);
        pt.line_join(agg::round_join);
        pt.width(1.5);

        sprintf(buf, "Num Points=%d Time=%.2fmks\n\n"
                     " Dist Error: x0.01=%.5f x0.1=%.5f x1=%.5f x10=%.5f x100=%.5f\n\n"
                     "Angle Error: x0.01=%.1f x0.1=%.1f x1=%.1f x10=%.1f x100=%.1f", 
                num_points1, curve_time, 
                max_error_01,  
                max_error_1,   
                max_error1,   
                max_error_10,  
                max_error_100,
                max_angle_error_01,
                max_angle_error_1,
                max_angle_error1,
                max_angle_error_10,
                max_angle_error_100);

        t.start_point(10.0, 85.0);
        t.text(buf);

        ras.add_path(pt);
        ren.color(agg::rgba(0,0,0));
        agg::render_scanlines(ras, sl, ren);

        agg::render_ctrl(ras, sl, ren_base, m_curve1);
        agg::render_ctrl(ras, sl, ren_base, m_angle_tolerance);
        agg::render_ctrl(ras, sl, ren_base, m_approximation_scale);
        agg::render_ctrl(ras, sl, ren_base, m_cusp_limit);
        agg::render_ctrl(ras, sl, ren_base, m_width);
        agg::render_ctrl(ras, sl, ren_base, m_show_points);
        agg::render_ctrl(ras, sl, ren_base, m_show_outline);
        agg::render_ctrl(ras, sl, ren_base, m_curve_type);
        agg::render_ctrl(ras, sl, ren_base, m_case_type);
        agg::render_ctrl(ras, sl, ren_base, m_inner_join);
        agg::render_ctrl(ras, sl, ren_base, m_line_join);
        agg::render_ctrl(ras, sl, ren_base, m_line_cap);
    }

    
    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
        if(key == ' ')
        {
            FILE* fd = fopen(full_file_name("coord"), "w");
            fprintf(fd, "%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f", 
                         m_curve1.x1(), m_curve1.y1(), 
                         m_curve1.x2(), m_curve1.y2(), 
                         m_curve1.x3(), m_curve1.y3(), 
                         m_curve1.x4(), m_curve1.y4());
            fclose(fd);
        }
    }

    virtual void on_ctrl_change()
    {
        if(m_case_type.cur_item() != m_cur_case_type)
        {
            switch(m_case_type.cur_item())
            {
            case 0: //m_case_type.add_item("Random");
                {
                    int w = int(width() - 120);
                    int h = int(height() - 80);
                    m_curve1.curve(rand() % w, rand() % h + 80, rand() % w, rand() % h + 80,
                                   rand() % w, rand() % h + 80, rand() % w, rand() % h + 80);
                }
                break;

            case 1: //m_case_type.add_item("13---24");
                m_curve1.curve(150, 150, 350, 150, 150, 150, 350, 150);
                //m_curve1.curve(252, 227, 16, 227, 506, 227, 285, 227);
                //m_curve1.curve(252, 227, 16, 227, 387, 227, 285, 227);
                break;

            case 2: //m_case_type.add_item("Smooth Cusp 1");
                m_curve1.curve(50, 142, 483, 251, 496, 62, 26, 333);
                break;

            case 3: //m_case_type.add_item("Smooth Cusp 2");
                m_curve1.curve(50, 142, 484, 251, 496, 62, 26, 333);
                break;

            case 4: //m_case_type.add_item("Real Cusp 1");
                m_curve1.curve(100, 100, 300, 200, 200, 200, 200, 100);
                break;

            case 5: //m_case_type.add_item("Real Cusp 2");
                m_curve1.curve(475, 157, 200, 100, 453, 100, 222, 157);
                break;

            case 6: //m_case_type.add_item("Fancy Stroke");
                m_curve1.curve(129, 233, 32, 283, 258, 285, 159, 232);
                m_width.value(100);
                break;

            case 7: //m_case_type.add_item("Jaw");
                m_curve1.curve(100, 100, 300, 200, 264, 286, 264, 284);
                break;

            case 8: //m_case_type.add_item("Ugly Jaw");
                m_curve1.curve(100, 100, 413, 304, 264, 286, 264, 284);
                break;
            }
            force_redraw();
            m_cur_case_type = m_case_type.cur_item();
        }
    }
};


int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("AGG Example");
    if(app.init(655, 520, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}





