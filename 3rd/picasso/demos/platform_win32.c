/* platform - application framework base on picasso 
 * 
 * Copyright (C) 2009 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include <stdio.h>
#include <windows.h>

#include "picasso.h"
#include "interface.h"


HINSTANCE hInst;    
HWND hmWnd;
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

static int CBYTE;
static int CBIT; 
static int width;
static int height;

BYTE* buffer;
BITMAP bmp;
HBITMAP hbmp;

static ps_context *context;
static ps_canvas *canvas;
static ps_color_format fmt;    

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    MSG msg;
    MyRegisterClass(hInstance);

    width = 640;
    height = 480;
    
    if (!InitInstance (hInstance, nCmdShow)) 
    {
        return FALSE;
    }

    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    
    wcex.cbSize = sizeof(WNDCLASSEX); 
    
    wcex.style            = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC)WndProc;
    wcex.cbClsExtra        = 0;
    wcex.cbWndExtra        = 0;
    wcex.hInstance        = hInstance;
    wcex.hIcon            = 0;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName    = 0;
    wcex.lpszClassName    = L"AppDemo";
    wcex.hIconSm        = 0;
    
    return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    hmWnd = CreateWindow(L"AppDemo", L"Picasso Demos", WS_OVERLAPPEDWINDOW,
        0, 0, width, height, NULL, NULL, hInstance, NULL);
    if (!hmWnd)
    {
        return FALSE;
    }
    
    ShowWindow(hmWnd, nCmdShow);
    UpdateWindow(hmWnd);
    
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    PAINTSTRUCT ps;
    HDC hdc;
    
    switch (message) 
    {
    case WM_CREATE:
        {
            if (!hmWnd)
                hmWnd = hWnd;

            hdc = GetDC(hWnd);
            CBIT = GetDeviceCaps(hdc, BITSPIXEL);    
            CBYTE = CBIT/8;
            ReleaseDC(hWnd, hdc);
            
            buffer = malloc(width*height*CBYTE);
            bmp.bmType = 0;
            bmp.bmWidth = width;
            bmp.bmHeight = height;
            bmp.bmWidthBytes = width*CBYTE;
            bmp.bmPlanes = 1;
            bmp.bmBitsPixel = CBIT;
            bmp.bmBits = buffer;
            
            if (CBYTE == 4)
                fmt = COLOR_FORMAT_BGRA;
            else if (CBYTE == 3)
                fmt = COLOR_FORMAT_BGR;
            else if (CBYTE == 2)
                fmt = COLOR_FORMAT_RGB565;

            ps_initialize();

            canvas = ps_canvas_create_with_data(buffer, fmt, width, height, width*CBYTE);
            context = ps_context_create(canvas, 0);
            on_init(context, width, height);    
        }
        break;
    case WM_PAINT:
        {
            HDC mdc;
            HGDIOBJ h;
            hdc = BeginPaint(hWnd, &ps);
            
            on_draw(context);
            
            mdc = CreateCompatibleDC(hdc);
            hbmp = CreateBitmapIndirect(&bmp);
            h = SelectObject(mdc, hbmp);
            BitBlt(hdc, 0, 0, width, height, mdc, 0, 0, SRCCOPY);
            SelectObject(mdc, h);
            DeleteObject(hbmp);
            DeleteDC(mdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
        on_timer();
        break;
    case WM_SIZE:
        {
            ps_canvas* old_canvas = 0;
            width = LOWORD(lParam);
            height = HIWORD(lParam);
            free(buffer);

            if (width < 1)
                width = 1;
            if (height < 1)
                height = 1;

            buffer = malloc(width*height*CBYTE);
            bmp.bmType = 0;
            bmp.bmWidth = width;
            bmp.bmHeight = height;
            bmp.bmWidthBytes = width*CBYTE;
            bmp.bmPlanes = 1;
            bmp.bmBitsPixel = CBIT;
            bmp.bmBits = buffer;

            canvas = ps_canvas_create_with_data(buffer, fmt, width, height, width*CBYTE);
            old_canvas = ps_context_set_canvas(context, canvas);
            ps_canvas_unref(old_canvas);
            on_size(width, height);
        }
        break;
    case WM_ERASEBKGND:
        break;
    case WM_LBUTTONDOWN:
            on_mouse_event(LEFT_BUTTON_DOWN, (int)wParam, LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MOUSEMOVE:
            on_mouse_event(MOUSE_MOVE, (int)wParam, LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_KEYDOWN:
            on_key_event(KEY_EVENT_DOWN, (int)wParam);
        break;
    case WM_KEYUP:
            on_key_event(KEY_EVENT_UP, (int)wParam);
        break;
    case WM_DESTROY:
        {
            on_term(context);
            ps_context_unref(context);
            ps_canvas_unref(canvas);
            ps_shutdown();
            free(buffer);
            PostQuitMessage(0);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

unsigned set_timer(unsigned mc)
{
    static int sid = 33;
    SetTimer(hmWnd, ++sid, mc, 0);
    return sid;
}

void clear_timer(unsigned id)
{
    KillTimer(hmWnd, id);
}

void refresh(const ps_rect* r)
{
    if (r) {
        RECT rc;
        rc.left = (int)r->x;
        rc.top = (int)r->y;
        rc.right = (int)(r->x+r->w);
        rc.bottom = (int)(r->y+r->h);
        InvalidateRect(hmWnd, &rc, TRUE);
    } else {
        InvalidateRect(hmWnd, 0, TRUE);
    }
}

picture* load_picture(const char* name)
{
    HBITMAP hbmp;
    picture* pic;
    ps_image * img;
    BITMAP* nb;
    unsigned size;
    char* pname;

    if (!name || !strlen(name))
        return NULL;

    pname = (char*)malloc(strlen(name)+5);
    sprintf(pname, "%s.bmp", name);

    nb = (BITMAP*)malloc(sizeof(BITMAP)); 
    hbmp = (HBITMAP)LoadImageA(hInst, pname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTCOLOR);
    GetObject(hbmp, sizeof(BITMAP), (LPVOID)nb);

    size = nb->bmWidthBytes * nb->bmHeight; 
    nb->bmBits = calloc(1, size);
    GetBitmapBits(hbmp, size, nb->bmBits);
    DeleteObject(hbmp);
    free(pname);
    img = ps_image_create_with_data(nb->bmBits, fmt, nb->bmWidth, nb->bmHeight, nb->bmWidthBytes);

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
    free(((BITMAP*)p->native)->bmBits);
    free(p->native);
    free(p);
}
