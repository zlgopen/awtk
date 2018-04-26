
#include <stdio.h>
#include <stdlib.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "../include/picasso.h"
#include "drawFunc.h"

#define RGB555 0

static ps_context *context;
static ps_canvas *canvas;
static BITMAP bmp;
static BITMAP abmp;
static HDC sdc;
static int LoadBmpWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message) {
        case MSG_CREATE:
            {
            unsigned char* pdr = 0;
            int w,h,p;
            RECT r;
            GetClientRect(hWnd, &r);
            HDC hdc = GetClientDC(hWnd);
#if RGB555
            sdc = CreateMemDC(640, 480, 16, MEMDC_FLAG_SWSURFACE, 0x7C00, 0x3E0, 0x1F, 0x8000);    
#else
            sdc = CreateMemDC(640, 480, 16, MEMDC_FLAG_SWSURFACE, 0xF800, 0x7E0, 0x1F, 0x00);    
#endif
            LoadBitmap(sdc, &bmp, "selt2.bmp");
            LoadBitmap(sdc, &abmp, "pat.bmp");
            pdr = LockDC(sdc, &r, &w, &h, &p);
            UnlockDC(sdc);
            ReleaseDC(hdc);
            fprintf(stderr, "device depth:%d, color size %d\n", bmp.bmBitsPerPixel, bmp.bmBytesPerPixel);

            ps_color_format fmt;    
            if (bmp.bmBytesPerPixel == 4)
                fmt = COLOR_FORMAT_RGBA;
            else if (bmp.bmBytesPerPixel == 3)
                fmt = COLOR_FORMAT_RGB;
            else if (bmp.bmBytesPerPixel == 2)
#if RGB555
                fmt = COLOR_FORMAT_RGB555;
#else
                fmt = COLOR_FORMAT_RGB565;
#endif

            ps_initialize();

            canvas = ps_canvas_create_with_data(pdr, fmt, w, h, p);
            context = ps_context_create(canvas, 0);
            init_context(context, canvas, pdr);    
               set_image_data(bmp.bmBits, fmt, bmp.bmWidth, bmp.bmHeight, bmp.bmPitch);
               set_pattern_data(abmp.bmBits, fmt, abmp.bmWidth, abmp.bmHeight, abmp.bmPitch);
            SetTimer(hWnd, 33, 10);
            }
            return 0;
        case MSG_TIMER:
            timer_action(context);
            InvalidateRect(hWnd, 0, TRUE);
            break;
        case MSG_ERASEBKGND:
            return 0;
        case MSG_PAINT:
            {
            HDC hdc = BeginPaint(hWnd);
            SetBrushColor(sdc, RGB2Pixel(sdc, 0xFF, 0xFF, 0xFF));
            FillBox(sdc, 0, 0, 640, 480);
            draw_test(0, context);
            BitBlt(sdc, 0, 0, 640, 480, hdc, 0, 0, 0);
            EndPaint(hWnd, hdc);
            }
            return 0;
        case MSG_LBUTTONDOWN:
            InvalidateRect(hWnd, 0, TRUE);
            break;
        case MSG_CLOSE:
            DeleteMemDC(sdc);
            dini_context(context);
            ps_context_unref(context);
            ps_canvas_unref(canvas);
            ps_shutdown();
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static int __argc = 0;
static const char** __argv = NULL;

int argc(void)
{
    return __argc;
}

const char** argv(void)
{
    return __argv;
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

    __argc = argc;
    __argv = argv;
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "loadbmp" , 0 , 0);
#endif
    
    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "test picasso";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = LoadBmpWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 640;
    CreateInfo.by = 480;
    CreateInfo.iBkColor = PIXEL_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow (hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

