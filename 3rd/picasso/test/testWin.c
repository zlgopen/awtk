
#include <windows.h>


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include "picasso.h"
#include "drawFunc.h"
#include "timeuse.h"

static int CBYTE;
static int CBIT; 

HINSTANCE hInst;                                // current instance

// Foward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


BITMAP bmp;
BYTE* buffer;
HBITMAP hbmp;

BITMAP ibmp;
HBITMAP sbmp;

BITMAP abmp;
HBITMAP tbmp;
static ps_context *context;
static ps_canvas *canvas;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    MSG msg;
    MyRegisterClass(hInstance);
    
    if (!InitInstance (hInstance, nCmdShow)) 
    {
        return FALSE;
    }
    
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
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
    wcex.lpszClassName    = L"testbmp";
    wcex.hIconSm        = 0;
    
    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    unsigned int bsize;
    unsigned int tsize;
    hInst = hInstance; // Store instance handle in our global variable
    
    sbmp = (HBITMAP)LoadImage(hInstance, L"selt2.bmp", IMAGE_BITMAP, 0 , 0, LR_LOADFROMFILE | LR_DEFAULTCOLOR);
    
    GetObject(sbmp, sizeof(BITMAP), (LPVOID)&ibmp);
    
    tbmp = (HBITMAP)LoadImage(hInstance, L"pat.bmp", IMAGE_BITMAP, 0 , 0, LR_LOADFROMFILE | LR_DEFAULTCOLOR);
    
    GetObject(tbmp, sizeof(BITMAP), (LPVOID)&abmp);
    
    bsize = ibmp.bmWidthBytes * ibmp.bmHeight;
    ibmp.bmBits = calloc(1, bsize);
    GetBitmapBits(sbmp, bsize, ibmp.bmBits);
    
    tsize = abmp.bmWidthBytes * abmp.bmHeight;
    abmp.bmBits = calloc(1, tsize);
    GetBitmapBits(tbmp, tsize, abmp.bmBits);
    
    hWnd = CreateWindow(L"testbmp", L"Test picasso", WS_OVERLAPPEDWINDOW,
        0, 0, 640, 480, NULL, NULL, hInstance, NULL);
    
    if (!hWnd)
    {
        return FALSE;
    }
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
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
            ps_color_format fmt;    
            hdc = GetDC(hWnd);
            CBIT = GetDeviceCaps(hdc, BITSPIXEL);    
            CBYTE = CBIT/8;
            ReleaseDC(hWnd, hdc);
            fprintf(stderr, "device depth:%d, color size %d\n", CBIT, CBYTE);
            
            buffer = malloc(640*480*CBYTE);
            bmp.bmType = 0;
            bmp.bmWidth = 640;
            bmp.bmHeight = 480;
            bmp.bmWidthBytes = 640*CBYTE;
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
            
            canvas = ps_canvas_create_with_data(buffer, fmt, 640, 480, 640*CBYTE);
            context = ps_context_create(canvas, 0);
            init_context(context, canvas, buffer);    
            set_image_data(ibmp.bmBits, fmt, ibmp.bmWidth, ibmp.bmHeight, ibmp.bmWidth*CBYTE);
            set_pattern_data(abmp.bmBits, fmt, abmp.bmWidth, abmp.bmHeight, abmp.bmWidth*CBYTE);
            SetTimer(hWnd, 33, 100, 0);
        }
        break;
    case WM_TIMER:
        timer_action(context);
        InvalidateRect(hWnd, 0, TRUE);
        break;
    case WM_ERASEBKGND:
        break;
    case WM_PAINT:
        {
            clocktime_t t1, t2;
            HDC mdc;
            HGDIOBJ h;
            hdc = BeginPaint(hWnd, &ps);
            memset(buffer, 0xff, 640*480*CBYTE);
            
            t1 = get_clock(); 
            draw_test(0, context);
            t2 = get_clock(); 

            mdc = CreateCompatibleDC(hdc);
            hbmp = CreateBitmapIndirect(&bmp);
            h = SelectObject(mdc, hbmp);
            BitBlt(hdc, 0,0,640,480,mdc,0,0,SRCCOPY);
            SelectObject(mdc, h);
            DeleteObject(hbmp);
            DeleteDC(mdc);
            EndPaint(hWnd, &ps);
            fprintf(stderr, "draw a frame use %.4f ms\n", get_clock_used_ms(t1, t2));
        }
        break;
    case WM_LBUTTONDOWN:
        InvalidateRect(hWnd, 0, FALSE);
        break;
    case WM_DESTROY:
        dini_context(context);
        ps_context_unref(context);
        ps_canvas_unref(canvas);
        ps_shutdown();
        free(buffer);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
