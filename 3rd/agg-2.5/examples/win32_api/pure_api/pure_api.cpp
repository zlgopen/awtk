// pure_api.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgba.h"
#include "agg_rasterizer_scanline_aa.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // The title bar text

// Foward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    // TODO: Place code here.
    MSG msg;
    HACCEL hAccelTable;

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_PURE_API, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow)) 
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_PURE_API);

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX); 

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC)WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, (LPCTSTR)IDI_PURE_API);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = (LPCSTR)IDC_PURE_API;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR szHello[MAX_LOADSTRING];
    LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_ABOUT:
                   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
                   break;
                case IDM_EXIT:
                   DestroyWindow(hWnd);
                   break;
                default:
                   return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_PAINT:
            {
                hdc = BeginPaint(hWnd, &ps);
                RECT rt;
                GetClientRect(hWnd, &rt);

                int width = rt.right - rt.left;
                int height = rt.bottom - rt.top;

                //============================================================ 
                //Creating compatible DC and a bitmap to render the image 
                BITMAPINFO bmp_info; 
                bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
                bmp_info.bmiHeader.biWidth = width; 
                bmp_info.bmiHeader.biHeight = height; 
                bmp_info.bmiHeader.biPlanes = 1; 
                bmp_info.bmiHeader.biBitCount = 32; 
                bmp_info.bmiHeader.biCompression = BI_RGB; 
                bmp_info.bmiHeader.biSizeImage = 0; 
                bmp_info.bmiHeader.biXPelsPerMeter = 0; 
                bmp_info.bmiHeader.biYPelsPerMeter = 0; 
                bmp_info.bmiHeader.biClrUsed = 0; 
                bmp_info.bmiHeader.biClrImportant = 0; 

                HDC mem_dc = ::CreateCompatibleDC(hdc); 

                void* buf = 0; 

                HBITMAP bmp = ::CreateDIBSection( 
                    mem_dc, 
                    &bmp_info, 
                    DIB_RGB_COLORS, 
                    &buf, 
                    0, 
                    0 
                ); 

                // Selecting the object before doing anything allows you 
                // to use AGG together with native Windows GDI.
                HBITMAP temp = (HBITMAP)::SelectObject(mem_dc, bmp);


                //============================================================ 
                // AGG lowest level code.
                agg::rendering_buffer rbuf;
                rbuf.attach((unsigned char*)buf, width, height, -width*4); // Use negative stride in order
                                                                           // to keep Y-axis consistent with
                                                                           // WinGDI, i.e., going down.

                // Pixel format and basic primitives renderer
                agg::pixfmt_bgra32 pixf(rbuf);
                agg::renderer_base<agg::pixfmt_bgra32> renb(pixf);

                renb.clear(agg::rgba8(255, 255, 255, 255));

                // Scanline renderer for solid filling.
                agg::renderer_scanline_aa_solid<agg::renderer_base<agg::pixfmt_bgra32> > ren(renb);

                // Rasterizer & scanline
                agg::rasterizer_scanline_aa<> ras;
                agg::scanline_p8 sl;

                // Polygon (triangle)
                ras.move_to_d(20.7, 34.15);
                ras.line_to_d(398.23, 123.43);
                ras.line_to_d(165.45, 401.87);

                // Setting the attrribute (color) & Rendering
                ren.color(agg::rgba8(80, 90, 60));
                agg::render_scanlines(ras, sl, ren);
                //============================================================



                //------------------------------------------------------------ 
                // Display the image. If the image is B-G-R-A (32-bits per pixel)
                // one can use AlphaBlend instead of BitBlt. In case of AlphaBlend
                // one also should clear the image with zero alpha, i.e. rgba8(0,0,0,0)

                ::BitBlt(
                  hdc,  
                  rt.left,      
                  rt.top,      
                  width,  
                  height, 
                  mem_dc,
                  0,
                  0,     
                  SRCCOPY
                );

                // Free resources 
                ::SelectObject(mem_dc, temp); 
                ::DeleteObject(bmp); 
                ::DeleteObject(mem_dc);

                EndPaint(hWnd, &ps);
            }
            break;


        case WM_ERASEBKGND: // Don't forget to do nothing on Erase Background event :-)
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
                return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
    }
    return FALSE;
}
