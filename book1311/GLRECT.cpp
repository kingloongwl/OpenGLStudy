// GLRECT.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GLRECT.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GLRECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GLRECT));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;


	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GLRECT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCE(IDC_GLRECT);
	wcex.lpszClassName	= szWindowClass;

	return RegisterClass(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


/* 方块的初始位置大小 */
GLfloat x1 = 0.0f;
GLfloat y1 = 0.0f;
GLfloat rsize = 25;

/* 每次移动的象素量 */
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

/* 追踪窗口宽度和高度的变化 */
GLfloat windowWidth;
GLfloat windowHeight;

void IdleFunction()
{
	//在达到左边或右边时翻转方向
	if ( x1 > windowWidth - rsize || x1 < -windowWidth )
	{
		xstep = -xstep;
	}

	//在达到顶边或底边时翻转方向
	if ( y1 > windowHeight || y1 < -windowHeight + rsize )
	{
		ystep = -ystep;
	}

	x1 += xstep;
	y1 += ystep;

	//检查边界。这是为了防止方块在反弹时窗口变小，使方块出现在新的裁剪区域之外
	if ( x1 >( windowWidth - rsize + xstep ) )
	{
		x1 = windowWidth - rsize - 1;
	}
	else if ( x1 < -(windowWidth + xstep) )
	{
		x1 = -windowWidth - 1;
	}

	if ( y1 >( windowHeight + ystep ) )
	{
		y1 = windowHeight - 1;
	}
	else if ( y1 < -(windowHeight - rsize + ystep) )
	{
		y1 = -windowHeight + rsize - 1;
	}
}

void RenderScene()
{
	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	glColor3f( 1.0f, 0.0f, 0.0f );

	glRectf( x1, y1, x1 + rsize, y1 - rsize );

}

void ChangeSize( GLsizei w, GLsizei h )
{
	GLfloat aspectRatio;

	if ( h == 0 )
	{
		h = 1;
	}

	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	aspectRatio = (GLfloat)w / (GLfloat)h;
	if ( w <= h )
	{
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho( -100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0 );
	}
	else
	{
		windowWidth = 100 * aspectRatio;
		windowHeight = 100;
		glOrtho( -windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0 );
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void SetDCPixelFormat( HDC hDC )
{
	int nPixelFormat;
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof( PIXELFORMATDESCRIPTOR ),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0, 0,
		0, 0, 0, 0, 0,
		16,
		0,
		0,
		0,
		0,
		0, 0, 0
	};

	nPixelFormat = ChoosePixelFormat( hDC, &pfd );
	SetPixelFormat( hDC, nPixelFormat, &pfd );
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc = NULL;
	static HGLRC hrc = NULL;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC( hWnd );
		SetDCPixelFormat( hdc );
		hrc = wglCreateContext( hdc );
		wglMakeCurrent( hdc, hrc );

		SetTimer( hWnd, 33, 1, NULL );
		break;
	case WM_SIZE:
		ChangeSize( LOWORD( lParam ), HIWORD( lParam ) );
		break;
	case WM_TIMER:
		IdleFunction();
		InvalidateRect( hWnd, NULL, FALSE );
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
//		EndPaint(hWnd, &ps);
		RenderScene();
		SwapBuffers( hdc );
		ValidateRect( hWnd, NULL );
		break;
	case WM_DESTROY:
		KillTimer( hWnd, 101 );
		wglMakeCurrent( hdc, NULL );
		wglDeleteContext( hrc );
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
