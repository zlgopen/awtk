/* platform - application framework base on picasso 
 * 
 * Copyright (C) 2009 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "picasso.h"
#include "interface.h"

#define RGB555 0

static int width;
static int height;

HWND hmWnd;

static ps_context *context;
static ps_canvas *canvas;
static ps_color_format fmt;    

static HDC sdc;

static BOOL key_down;

static unsigned key_conv(DWORD flag)
{
    unsigned k = 0;
    if (flag & KS_SHIFT)
        k |= EVT_SHIFT;
    if (flag & KS_CTRL)
        k |= EVT_CONTROL;
    return k;    
}

static int get_virtual_key(DWORD);

picture* load_picture(const char* name)
{
    picture* pic;
    ps_image * img;
    BITMAP* nb;
    char* pname;

    if (!name || !strlen(name))
        return NULL;

    pname = (char*)malloc(strlen(name)+5);
    sprintf(pname, "%s.bmp", name);

    nb = (BITMAP*)malloc(sizeof(BITMAP)); 
    LoadBitmap(sdc, nb, pname);
    free(pname);
    img = ps_image_create_with_data(nb->bmBits, fmt, nb->bmWidth, nb->bmHeight, nb->bmPitch);

    pic = (picture*)malloc(sizeof(picture));
    pic->width = nb->bmWidth;
    pic->height = nb->bmHeight;
    pic->native = nb;
    pic->image = img;

    return pic;
}

void free_picture(picture* p)
{
    if (!p)
        return;

    ps_image_unref(p->image);
    UnloadBitmap(((BITMAP*)p->native));
    free(p->native);
    free(p);
}

unsigned set_timer(unsigned mc)
{
    static int sid = 33;
    SetTimer(hmWnd, ++sid, mc/10);
    return sid;
}

void clear_timer(unsigned id)
{
    KillTimer(hmWnd, id);
}

void refresh(const ps_rect* r)
{
    if (r) {
        RECT rc = {r->x, r->y, r->x+r->w, r->y+r->h};
        InvalidateRect(hmWnd, &rc, TRUE);
    } else {
        InvalidateRect(hmWnd, 0, TRUE);
    }
}
static int PlatformProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CREATE:
            {
                if (!hmWnd)
                    hmWnd = hWnd;

                unsigned char* pdr = 0;
                int w,h,p;
                RECT r;
                key_down = FALSE;
                GetClientRect(hWnd, &r);
#if RGB555
            sdc = CreateMemDC(width, height, 16, MEMDC_FLAG_SWSURFACE, 0x7C00, 0x3E0, 0x1F, 0x8000);    
#else
            sdc = CreateMemDC(width, height, 16, MEMDC_FLAG_SWSURFACE, 0xF800, 0x7E0, 0x1F, 0x00);    
#endif
                pdr = LockDC(sdc, &r, &w, &h, &p);
                UnlockDC(sdc);
#if RGB555
                fmt = COLOR_FORMAT_RGB555;
#else
                fmt = COLOR_FORMAT_RGB565;
#endif
                ps_initialize();

                canvas = ps_canvas_create_with_data(pdr, fmt, w, h, p);
                context = ps_context_create(canvas, 0);
                on_init(context, width, height);    
            }
            break;
        case MSG_TIMER:
            on_timer();
            break;
        case MSG_PAINT:
            {
                HDC hdc = BeginPaint(hWnd);
                on_draw(context);
                BitBlt(sdc, 0, 0, width, height, hdc, 0, 0, 0);
                EndPaint(hWnd, hdc);
            }
            return 0;
        case MSG_ERASEBKGND:
            return 0;
        case MSG_LBUTTONDOWN:
            {
                key_down = TRUE;
                on_mouse_event(LEFT_BUTTON_DOWN, key_conv(wParam), LOSWORD(lParam), HISWORD(lParam));
            }
            break;
        case MSG_LBUTTONUP:
            {
                key_down = FALSE;
                on_mouse_event(LEFT_BUTTON_UP, key_conv(wParam), LOSWORD(lParam), HISWORD(lParam));
            }
            break;
        case MSG_MOUSEMOVE:
            {
                DWORD k = key_conv(wParam);
                if (key_down)
                    k |= EVT_LBUTTON;
                on_mouse_event(MOUSE_MOVE, k, LOSWORD(lParam), HISWORD(lParam));
            }
            break;
        case MSG_KEYDOWN:
            on_key_event(KEY_EVENT_DOWN, get_virtual_key(wParam));
            break;
        case MSG_KEYUP:
            on_key_event(KEY_EVENT_UP, get_virtual_key(wParam));
            break;
        case MSG_SIZECHANGED:
            {
                RECT r;
                int w,h,p;
                unsigned char* pdr = 0;
                ps_canvas* old_canvas = 0;
                RECT* rc = (RECT*)lParam;
                width = RECTWP(rc);
                height = RECTHP(rc);
                if (sdc)
                    DeleteMemDC(sdc);

                if (width < 1)
                    width = 1;
                if (height < 1)
                    height = 1;
#if RGB555
            sdc = CreateMemDC(width, height, 16, MEMDC_FLAG_SWSURFACE, 0x7C00, 0x3E0, 0x1F, 0x8000);    
#else
            sdc = CreateMemDC(width, height, 16, MEMDC_FLAG_SWSURFACE, 0xF800, 0x7E0, 0x1F, 0x00);    
#endif
                GetClientRect(hWnd, &r);
                pdr = LockDC(sdc, &r, &w, &h, &p);
                UnlockDC(sdc);
                canvas = ps_canvas_create_with_data(pdr, fmt, w, h, p);
                old_canvas = ps_context_set_canvas(context, canvas);
                ps_canvas_unref(old_canvas);
                on_size(width, height);
            }
            break;
        case MSG_CLOSE:
            on_term(context);
            ps_context_unref(context);
            ps_canvas_unref(canvas);
            ps_shutdown();
            DeleteMemDC(sdc);
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }
    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

    width = 640;
    height = 480;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "picasso" , 0 , 0);
#endif
    
    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "Picasso Demos";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = PlatformProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = width;
    CreateInfo.by = height+24;
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

typedef struct {
    int vk;
    unsigned pk;
}KeyEntities;

static KeyEntities key_map[] = {
    {KEY_BACK, SCANCODE_BACKSPACE},
    {KEY_TAB, SCANCODE_TAB},
    {KEY_RETURN, SCANCODE_ENTER},
    {KEY_SHIFT, SCANCODE_LEFTSHIFT},
    {KEY_CONTROL, SCANCODE_LEFTCONTROL},
    {KEY_MENU, SCANCODE_MENU},
    {KEY_PAUSE, SCANCODE_PAUSE},
    {KEY_CAPITAL, SCANCODE_CAPSLOCK},
    {KEY_ESCAPE, SCANCODE_ESCAPE},
    {KEY_SPACE, SCANCODE_SPACE},
    {KEY_END, SCANCODE_END},
    {KEY_HOME, SCANCODE_HOME},
    {KEY_LEFT, SCANCODE_CURSORBLOCKLEFT},
    {KEY_UP, SCANCODE_CURSORBLOCKUP},
    {KEY_RIGHT, SCANCODE_CURSORBLOCKRIGHT},
    {KEY_DOWN, SCANCODE_CURSORBLOCKDOWN},
    {KEY_INSERT, SCANCODE_INSERT},
    {KEY_DELETE, SCANCODE_REMOVE},
    {KEY_0, SCANCODE_0},
    {KEY_1, SCANCODE_1},
    {KEY_2, SCANCODE_2},
    {KEY_3, SCANCODE_3},
    {KEY_4, SCANCODE_4},
    {KEY_5, SCANCODE_5},
    {KEY_6, SCANCODE_6},
    {KEY_7, SCANCODE_7},
    {KEY_8, SCANCODE_8},
    {KEY_9, SCANCODE_9},
    {KEY_A, SCANCODE_A},
    {KEY_B, SCANCODE_B},
    {KEY_C, SCANCODE_C},
    {KEY_D, SCANCODE_D},
    {KEY_E, SCANCODE_E},
    {KEY_F, SCANCODE_F},
    {KEY_G, SCANCODE_G},
    {KEY_H, SCANCODE_H},
    {KEY_I, SCANCODE_I},
    {KEY_J, SCANCODE_J},
    {KEY_K, SCANCODE_K},
    {KEY_L, SCANCODE_L},
    {KEY_M, SCANCODE_M},
    {KEY_N, SCANCODE_N},
    {KEY_O, SCANCODE_O},
    {KEY_P, SCANCODE_P},
    {KEY_Q, SCANCODE_Q},
    {KEY_R, SCANCODE_R},
    {KEY_S, SCANCODE_S},
    {KEY_T, SCANCODE_T},
    {KEY_U, SCANCODE_U},
    {KEY_V, SCANCODE_V},
    {KEY_W, SCANCODE_W},
    {KEY_X, SCANCODE_X},
    {KEY_Y, SCANCODE_Y},
    {KEY_Z, SCANCODE_Z},
    {KEY_LWIN, SCANCODE_LEFTWIN},
    {KEY_RWIN, SCANCODE_RIGHTWIN},
    {KEY_OEM_PLUS, SCANCODE_EQUAL},
    {KEY_OEM_MINUS, SCANCODE_MINUS},
    {KEY_F1, SCANCODE_F1},
    {KEY_F2, SCANCODE_F2},
    {KEY_F3, SCANCODE_F3},
    {KEY_F4, SCANCODE_F4},
    {KEY_F5, SCANCODE_F5},
    {KEY_F6, SCANCODE_F6},
    {KEY_F7, SCANCODE_F7},
    {KEY_F8, SCANCODE_F8},
    {KEY_F9, SCANCODE_F9},
    {KEY_F10, SCANCODE_F10},
    {KEY_F11, SCANCODE_F11},
    {KEY_F12, SCANCODE_F12},
    {KEY_LSHIFT, SCANCODE_LEFTSHIFT},
    {KEY_RSHIFT, SCANCODE_RIGHTSHIFT},
    {KEY_LCONTROL, SCANCODE_LEFTCONTROL},
    {KEY_RCONTROL, SCANCODE_RIGHTCONTROL},
};

static int get_virtual_key(DWORD pk)
{
    int i;
    for(i = 0; i < (sizeof(key_map)/sizeof(KeyEntities)); i++)
        if (key_map[i].pk == pk)
            return key_map[i].vk;
    return KEY_UNKNOWN; 
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
