#include <stdio.h>
#include "platform/agg_platform_support.h"
#include "agg2d.h"

#define pix_format agg::pix_format_bgra32

enum { flip_y = true };



class the_application : public agg::platform_support
{
    Agg2D m_graphics;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_graphics()
    {
    }

    ~the_application()
    {
    }

    virtual void on_init()
    {
    }


    virtual void on_draw()
    {
        m_graphics.attach(rbuf_window().buf(),
                          rbuf_window().width(),
                          rbuf_window().height(),
                          rbuf_window().stride());

        m_graphics.clearAll(255, 255, 255);
        //m_graphics.clearAll(0, 0, 0);

        //m_graphics.blendMode(Agg2D::BlendSub);
        //m_graphics.blendMode(Agg2D::BlendAdd);

        m_graphics.antiAliasGamma(1.4);

        // Set flipText(true) if you have the Y axis upside down.
        //m_graphics.flipText(true);


        // ClipBox.
        //m_graphics.clipBox(50, 50, rbuf_window().width() - 50, rbuf_window().height() - 50);

        // Transfornations - Rotate around (300,300) to 5 degree
        //m_graphics.translate(-300, -300);
        //m_graphics.rotate(Agg2D::deg2Rad(5.0));
        //m_graphics.translate(300, 300);

        // Viewport - set 0,0,600,600 to the actual window size
        // preserving aspect ratio and placing the viewport in the center.
        // To ignore aspect ratio use Agg2D::Anisotropic
        // Note that the viewport just adds transformations to the current
        // affine matrix. So that, set the viewport *after* all transformations!
        m_graphics.viewport(0, 0, 600, 600,
                            0, 0, width(), height(),
                            //Agg2D::Anisotropic);
                            Agg2D::XMidYMid);


        // Rounded Rect
        m_graphics.lineColor(0, 0, 0);
        m_graphics.noFill();
        m_graphics.roundedRect(0.5, 0.5, 600-0.5, 600-0.5, 20.0);


        // Reglar Text
        m_graphics.font("Times New Roman", 14.0, false, false);
        m_graphics.fillColor(0, 0, 0);
        m_graphics.noLine();
        m_graphics.text(100, 20, "Regular Raster Text -- Fast, but can't be rotated");

        // Outlined Text
        m_graphics.font("Times New Roman", 50.0, false, false, Agg2D::VectorFontCache);
        m_graphics.lineColor(50, 0, 0);
        m_graphics.fillColor(180, 200, 100);
        m_graphics.lineWidth(1.0);
        m_graphics.text(100.5, 50.5, "Outlined Text");

        // Text Alignment
        m_graphics.line(250.5-150, 150.5,    250.5+150, 150.5);
        m_graphics.line(250.5,     150.5-20, 250.5,     150.5+20);
        m_graphics.line(250.5-150, 200.5,    250.5+150, 200.5);
        m_graphics.line(250.5,     200.5-20, 250.5,     200.5+20);
        m_graphics.line(250.5-150, 250.5,    250.5+150, 250.5);
        m_graphics.line(250.5,     250.5-20, 250.5,     250.5+20);
        m_graphics.line(250.5-150, 300.5,    250.5+150, 300.5);
        m_graphics.line(250.5,     300.5-20, 250.5,     300.5+20);
        m_graphics.line(250.5-150, 350.5,    250.5+150, 350.5);
        m_graphics.line(250.5,     350.5-20, 250.5,     350.5+20);
        m_graphics.line(250.5-150, 400.5,    250.5+150, 400.5);
        m_graphics.line(250.5,     400.5-20, 250.5,     400.5+20);
        m_graphics.line(250.5-150, 450.5,    250.5+150, 450.5);
        m_graphics.line(250.5,     450.5-20, 250.5,     450.5+20);
        m_graphics.line(250.5-150, 500.5,    250.5+150, 500.5);
        m_graphics.line(250.5,     500.5-20, 250.5,     500.5+20);
        m_graphics.line(250.5-150, 550.5,    250.5+150, 550.5);
        m_graphics.line(250.5,     550.5-20, 250.5,     550.5+20);


        m_graphics.fillColor(100, 50, 50);
        m_graphics.noLine();
        //m_graphics.textHints(false);
        m_graphics.font("Times New Roman", 40.0, false, false, Agg2D::VectorFontCache);

        m_graphics.textAlignment(Agg2D::AlignLeft, Agg2D::AlignBottom);
        m_graphics.text(250.0,     150.0, "Left-Bottom", true, 0, 0);

        m_graphics.textAlignment(Agg2D::AlignCenter, Agg2D::AlignBottom);
        m_graphics.text(250.0,     200.0, "Center-Bottom", true, 0, 0);

        m_graphics.textAlignment(Agg2D::AlignRight, Agg2D::AlignBottom);
        m_graphics.text(250.0,     250.0, "Right-Bottom", true, 0, 0);

        m_graphics.textAlignment(Agg2D::AlignLeft, Agg2D::AlignCenter);
        m_graphics.text(250.0,     300.0, "Left-Center", true, 0, 0);

        m_graphics.textAlignment(Agg2D::AlignCenter, Agg2D::AlignCenter);
        m_graphics.text(250.0,     350.0, "Center-Center", true, 0, 0);

        m_graphics.textAlignment(Agg2D::AlignRight, Agg2D::AlignCenter);
        m_graphics.text(250.0,     400.0, "Right-Center", true, 0, 0);

        m_graphics.textAlignment(Agg2D::AlignLeft, Agg2D::AlignTop);
        m_graphics.text(250.0,     450.0, "Left-Top", true, 0, 0);

        m_graphics.textAlignment(Agg2D::AlignCenter, Agg2D::AlignTop);
        m_graphics.text(250.0,     500.0, "Center-Top", true, 0, 0);

        m_graphics.textAlignment(Agg2D::AlignRight, Agg2D::AlignTop);
        m_graphics.text(250.0,     550.0, "Right-Top", true, 0, 0);


        // Gradients (Aqua Buttons)
        //=======================================
        m_graphics.font("Verdana", 20.0, false, false, Agg2D::VectorFontCache);
        double xb1 = 400;
        double yb1 = 80;
        double xb2 = xb1 + 150;
        double yb2 = yb1 + 36;

        m_graphics.fillColor(Agg2D::Color(0,50,180,180));
        m_graphics.lineColor(Agg2D::Color(0,0,80, 255));
        m_graphics.lineWidth(1.0);
        m_graphics.roundedRect(xb1, yb1, xb2, yb2, 12, 18);

        m_graphics.lineColor(Agg2D::Color(0,0,0,0));
        m_graphics.fillLinearGradient(xb1, yb1, xb1, yb1+30,
                                      Agg2D::Color(100,200,255,255),
                                      Agg2D::Color(255,255,255,0));
        m_graphics.roundedRect(xb1+3, yb1+2.5, xb2-3, yb1+30, 9, 18, 1, 1);

        m_graphics.fillColor(Agg2D::Color(0,0,50, 200));
        m_graphics.noLine();
        m_graphics.textAlignment(Agg2D::AlignCenter, Agg2D::AlignCenter);
        m_graphics.text((xb1 + xb2) / 2.0, (yb1 + yb2) / 2.0, "Aqua Button", true, 0.0, 0.0);

        m_graphics.fillLinearGradient(xb1, yb2-20, xb1, yb2-3,
                                      Agg2D::Color(0,  0,  255,0),
                                      Agg2D::Color(100,255,255,255));
        m_graphics.roundedRect(xb1+3, yb2-20, xb2-3, yb2-2, 1, 1, 9, 18);


        // Aqua Button Pressed
        xb1 = 400;
        yb1 = 30;
        xb2 = xb1 + 150;
        yb2 = yb1 + 36;

        m_graphics.fillColor(Agg2D::Color(0,50,180,180));
        m_graphics.lineColor(Agg2D::Color(0,0,0,  255));
        m_graphics.lineWidth(2.0);
        m_graphics.roundedRect(xb1, yb1, xb2, yb2, 12, 18);

        m_graphics.lineColor(Agg2D::Color(0,0,0,0));
        m_graphics.fillLinearGradient(xb1, yb1+2, xb1, yb1+25,
                                      Agg2D::Color(60, 160,255,255),
                                      Agg2D::Color(100,255,255,0));
        m_graphics.roundedRect(xb1+3, yb1+2.5, xb2-3, yb1+30, 9, 18, 1, 1);

        m_graphics.fillColor(Agg2D::Color(0,0,50, 255));
        m_graphics.noLine();
        m_graphics.textAlignment(Agg2D::AlignCenter, Agg2D::AlignCenter);
        m_graphics.text((xb1 + xb2) / 2.0, (yb1 + yb2) / 2.0, "Aqua Pressed", 0.0, 0.0);

        m_graphics.fillLinearGradient(xb1, yb2-25, xb1, yb2-5,
                                      Agg2D::Color(0,  180,255,0),
                                      Agg2D::Color(0,  200,255,255));
        m_graphics.roundedRect(xb1+3, yb2-25, xb2-3, yb2-2, 1, 1, 9, 18);




        // Basic Shapes -- Ellipse
        //===========================================
        m_graphics.lineWidth(3.5);
        m_graphics.lineColor(20,  80,  80);
        m_graphics.fillColor(200, 255, 80, 200);
        m_graphics.ellipse(450, 200, 50, 90);


        // Paths
        //===========================================
        m_graphics.resetPath();
        m_graphics.fillColor(255, 0, 0, 100);
        m_graphics.lineColor(0, 0, 255, 100);
        m_graphics.lineWidth(2);
        m_graphics.moveTo(300/2, 200/2);
        m_graphics.horLineRel(-150/2);
        m_graphics.arcRel(150/2, 150/2, 0, 1, 0, 150/2, -150/2);
        m_graphics.closePolygon();
        m_graphics.drawPath();

        m_graphics.resetPath();
        m_graphics.fillColor(255, 255, 0, 100);
        m_graphics.lineColor(0, 0, 255, 100);
        m_graphics.lineWidth(2);
        m_graphics.moveTo(275/2, 175/2);
        m_graphics.verLineRel(-150/2);
        m_graphics.arcRel(150/2, 150/2, 0, 0, 0, -150/2, 150/2);
        m_graphics.closePolygon();
        m_graphics.drawPath();


        m_graphics.resetPath();
        m_graphics.noFill();
        m_graphics.lineColor(127, 0, 0);
        m_graphics.lineWidth(5);
        m_graphics.moveTo(600/2, 350/2);
        m_graphics.lineRel(50/2, -25/2);
        m_graphics.arcRel(25/2, 25/2, Agg2D::deg2Rad(-30), 0, 1, 50/2, -25/2);
        m_graphics.lineRel(50/2, -25/2);
        m_graphics.arcRel(25/2, 50/2, Agg2D::deg2Rad(-30), 0, 1, 50/2, -25/2);
        m_graphics.lineRel(50/2, -25/2);
        m_graphics.arcRel(25/2, 75/2, Agg2D::deg2Rad(-30), 0, 1, 50/2, -25/2);
        m_graphics.lineRel(50, -25);
        m_graphics.arcRel(25/2, 100/2, Agg2D::deg2Rad(-30), 0, 1, 50/2, -25/2);
        m_graphics.lineRel(50/2, -25/2);
        m_graphics.drawPath();


        // Master Alpha. From now on everything will be translucent
        //===========================================
        m_graphics.masterAlpha(0.85);


        // Image Transformations
        //===========================================
        Agg2D::Image img(rbuf_img(0).buf(),
                         rbuf_img(0).width(),
                         rbuf_img(0).height(),
                         rbuf_img(0).stride());
        m_graphics.imageFilter(Agg2D::Bilinear);

        //m_graphics.imageResample(Agg2D::NoResample);
        //m_graphics.imageResample(Agg2D::ResampleAlways);
        m_graphics.imageResample(Agg2D::ResampleOnZoomOut);

        // Set the initial image blending operation as BlendDst, that actually
        // does nothing.
        //-----------------
        m_graphics.imageBlendMode(Agg2D::BlendDst);


        // Transform the whole image to the destination rectangle
        //-----------------
        //m_graphics.transformImage(img, 450, 200, 595, 350);

        // Transform the rectangular part of the image to the destination rectangle
        //-----------------
        //m_graphics.transformImage(img, 60, 60, img.width()-60, img.height()-60,
        //                          450, 200, 595, 350);

        // Transform the whole image to the destination parallelogram
        //-----------------
        //double parl[6] = { 450, 200, 595, 220, 575, 350 };
        //m_graphics.transformImage(img, parl);

        // Transform the rectangular part of the image to the destination parallelogram
        //-----------------
        //double parl[6] = { 450, 200, 595, 220, 575, 350 };
        //m_graphics.transformImage(img, 60, 60, img.width()-60, img.height()-60, parl);

        // Transform image to the destination path. The scale is determined by a rectangle
        //-----------------
        //m_graphics.resetPath();
        //m_graphics.moveTo(450, 200);
        //m_graphics.cubicCurveTo(595, 220, 575, 350, 595, 350);
        //m_graphics.lineTo(470, 340);
        //m_graphics.transformImagePath(img, 450, 200, 595, 350);


        // Transform image to the destination path.
        // The scale is determined by a rectangle
        //-----------------
        m_graphics.resetPath();
        m_graphics.moveTo(450, 200);
        m_graphics.cubicCurveTo(595, 220, 575, 350, 595, 350);
        m_graphics.lineTo(470, 340);
        m_graphics.transformImagePath(img, 60, 60, img.width()-60, img.height()-60,
                                      450, 200, 595, 350);

        // Transform image to the destination path.
        // The transformation is determined by a parallelogram
        //m_graphics.resetPath();
        //m_graphics.moveTo(450, 200);
        //m_graphics.cubicCurveTo(595, 220, 575, 350, 595, 350);
        //m_graphics.lineTo(470, 340);
        //double parl[6] = { 450, 200, 595, 220, 575, 350 };
        //m_graphics.transformImagePath(img, parl);

        // Transform the rectangular part of the image to the destination path.
        // The transformation is determined by a parallelogram
        //m_graphics.resetPath();
        //m_graphics.moveTo(450, 200);
        //m_graphics.cubicCurveTo(595, 220, 575, 350, 595, 350);
        //m_graphics.lineTo(470, 340);
        //double parl[6] = { 450, 200, 595, 220, 575, 350 };
        //m_graphics.transformImagePath(img, 60, 60, img.width()-60, img.height()-60, parl);


        // Add/Sub/Contrast Blending Modes
        m_graphics.noLine();
        m_graphics.fillColor(70, 70, 0);
        m_graphics.blendMode(Agg2D::BlendAdd);
        m_graphics.ellipse(500, 280, 20, 40);

        m_graphics.fillColor(255, 255, 255);
        m_graphics.blendMode(Agg2D::BlendContrast);
        m_graphics.ellipse(500+40, 280, 20, 40);



        // Radial gradient.
        m_graphics.blendMode(Agg2D::BlendAlpha);
        m_graphics.fillRadialGradient(400, 500, 40,
                                      Agg2D::Color(255, 255, 0, 0),
                                      Agg2D::Color(0, 0, 127),
                                      Agg2D::Color(0, 255, 0, 0));
        m_graphics.ellipse(400, 500, 40, 40);

    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
    }

    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
    }

};






int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("Agg2DDemo");

    char buf[256];
    const char* img_name = "spheres";
    if(argc >= 2) img_name = argv[1];
    if(!app.load_img(0, img_name))
    {
        if(strcmp(img_name, "spheres") == 0)
        {
            sprintf(buf, "File not found: %s%s. Download http://www.antigrain.com/%s%s\n"
                         "or copy it from another directory if available.",
                    img_name, app.img_ext(), img_name, app.img_ext());
        }
        else
        {
            sprintf(buf, "File not found: %s%s", img_name, app.img_ext());
        }
        app.message(buf);
        return 1;
    }

    if(app.init(600, 600, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}






