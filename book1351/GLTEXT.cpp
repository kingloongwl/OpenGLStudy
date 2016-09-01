// GLRECT.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GLTEXT.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#define MAX_LOADSTRING 100
#define WCHAR
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

GLuint nFontList;
GLfloat  whiteLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat  diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat  specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat lightPos[] = { -100.0f, 200.0f, 50.0f, 1.0f };
void SetupRC( HDC hDC )
{
	//设置字体类型
	HFONT hFont;
	GLYPHMETRICSFLOAT agmf[128];
	LOGFONT logfont;
	logfont.lfHeight = -10;
	logfont.lfWidth = 0;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;
	logfont.lfWeight = FW_BOLD;
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = ANSI_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy( logfont.lfFaceName, "Arial" );
	

	//创建字体和显示列表
	hFont = CreateFontIndirect( &logfont );
	SelectObject( hDC, hFont );

	
#ifndef WCHAR
	//为字形0至128创建显示列表，拉伸设置为0。1，并使用缺省的偏差
	nFontList = glGenLists( 128 );
	wglUseFontOutlines( hDC, 0, 128, nFontList, 0.0f, 0.5f, WGL_FONT_POLYGONS, agmf );
#endif
	
	DeleteObject( hFont );
	
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_COLOR_MATERIAL );

	glEnable( GL_LIGHTING );
	glLightfv( GL_LIGHT0, GL_AMBIENT, whiteLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
	glEnable( GL_LIGHT0 );

	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMateriali( GL_FRONT, GL_SHININESS, 128 );

}


void RenderScene( HDC hDC )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

#ifdef WCHAR
	nFontList = glGenLists( 1 );

	char *pChar = "你好！";
	wchar_t* pWCHAR = NULL;

	//计算pChar所指向的多字节字符串相当于多少个宽字节  
	//DWORD num = MultiByteToWideChar( CP_ACP, 0, pChar, -1, NULL, 0 );
	int num = 0;
	for ( int i = 0; pChar[i] != '\0'; ++i )
	{
		if ( IsDBCSLeadByte( pChar[i] ) )
			++i;
		++num;
	}

	pWCHAR = (wchar_t*)malloc( (num+1)*sizeof( wchar_t ) );

	if ( pWCHAR == NULL )
	{
		free( pWCHAR );
	}

	memset( pWCHAR, 0, (num+1)*sizeof( wchar_t ) );

	//多字节转换为宽字节  
	MultiByteToWideChar( CP_ACP, 0, pChar, -1, pWCHAR, num );
	//pWCHAR[num] = L'\0';

	GLYPHMETRICSFLOAT agmf[128];
#endif
	
	//Text
	glColor3ub( 0, 0, 255 );
	glPushMatrix();
#ifndef WCHAR
	glListBase( nFontList );
	glCallLists( 6, GL_UNSIGNED_BYTE, "OpenGL" );
#else
	for ( int i = 0; i < num; i++ )
	{
		wglUseFontOutlinesW( hDC, pWCHAR[i], 1, nFontList, 0.0f, 0.5f, WGL_FONT_POLYGONS, agmf );
		glCallList( nFontList );
	}
#endif

	glPopMatrix();

#ifdef WCHAR
	free( pWCHAR );
	glDeleteLists( nFontList, 1 );
#endif

}

void ChangeSize( GLsizei w, GLsizei h )
{
	GLfloat nRange = 100.0f;
	GLfloat fAspect;

	// Prevent a divide by zero
	if ( h == 0 )
		h = 1;

	fAspect = (GLfloat)w / (GLfloat)h;

	// Set Viewport to window dimensions
	glViewport( 0, 0, w, h );

	glMatrixMode( GL_PROJECTION );

	// Reset coordinate system
	glLoadIdentity();

	// Setup perspective for viewing
	gluPerspective( 17.5f, fAspect, 1, 300 );

	// Viewing transformation
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( -1.8f, 0.0f, -15.0f );
	glRotatef( -20.0f, 0.0f, 1.0f, 0.0f );
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
		SetupRC( hdc );
		break;
	case WM_SIZE:
		ChangeSize( LOWORD( lParam ), HIWORD( lParam ) );
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
		RenderScene( hdc );
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
