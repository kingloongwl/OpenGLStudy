// GLRECT.cpp : Defines the entry point for the application.
//

#include <Windows.h>
#include <stdio.h>
#include "stdafx.h"
#include "SPHEREWORLD32.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/glut.h>
#include "GLTools.h"
#include "GLEXT.H"
#include "WGLEXT.H"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

struct STARTUPOPTIONS
{
	DEVMODE	devMode;
	int nPixelFormat;
	int nPixelFormatMS;
	BOOL bFullScreen;
	BOOL bFSAA;
	BOOL bVerticalSync;
};

static HPALETTE hPalette = NULL;
static HINSTANCE ghInstance = NULL;
static LPCTSTR lpszAppName = "SphereWorld32";
static GLint nFontList;
static struct STARTUPOPTIONS startupOptions;
static LARGE_INTEGER CounterFrequency;
static LARGE_INTEGER FPSCount;
static LARGE_INTEGER CameraTimer;
#define NUM_SPHERES 30
GLTFrame spheres[NUM_SPHERES];
GLTFrame frameCamera;

GLfloat fLightPos[4] = { -100.0f, 100.0f, 50.0f, 1.0f };
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLTMatrix mShadowMatrix;

#define GROUND_TEXTURE 0
#define TORUS_TEXTURE 1
#define SPHERE_TEXTURE 2
#define NUM_TEXTURES 3
GLuint textureObjects[NUM_TEXTURES];
const char *szTextureFiles[] = { "grass.tga", "wood.tga", "orb.rga" };
GLuint lTorusList, lSphereList;
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL APIENTRY StartupDlgProc( HWND hDlg, UINT message, UINT wParam, LONG lParam );
void FindBestPF( HDC hDC, int *nRegularFormat, int *nMSFormat );
BOOL ShowStartupOptions();
void ChangeSize( GLsizei w, GLsizei h );
void RenderScene();
void SetupRC();
HPALETTE GetOpenGLPalette( HDC hDC );
void DrawInhabitants( GLint nShadow );
void DrawGround();

PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;
PFNGLWINDOWPOS2IPROC glWindowPos2i = NULL;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;

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
	gluPerspective( 35.0f, aspectRatio, 1.0f, 50.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}


void DrawGround()
{
	GLfloat fExtent = 20.0f;
	GLfloat fStep = 1.0f;
	GLfloat y = -0.4f;
	GLint iStrip, iRun;
	GLfloat s = 0.0f;
	GLfloat t = 0.0f;
	GLfloat texStep = 1.0f / (fExtent * 0.075f);

	glBindTexture( GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE] );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	for ( iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep )
	{
		t = 0.0f;
		glBegin( GL_TRIANGLE_STRIP );

		for ( iRun = fExtent; iRun >= -fExtent; iRun -= fStep )
		{
			glTexCoord2f( s, t );
			glNormal3f( 0.0f, 1.0f, 0.0f );
			glVertex3f( iStrip, y, iRun );

			glTexCoord2f( s + texStep, t );
			glNormal3f( 0.0f, 1.0f, 0.0f );
			glVertex3f( iStrip + fStep, y, iRun );

			t += texStep;
		}
		glEnd();

		s += texStep;
	}
}

void DrawInhabitants( GLint nShadow )
{
	static GLfloat yRot = 0.0f;

	if ( nShadow == 0 )
	{
		yRot += 0.5f;
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	}
	else
	{
		glColor4f( 0.0f, 0.0f, 0.0f, 0.75f );
	}

	glBindTexture( GL_TEXTURE_2D, textureObjects[SPHERE_TEXTURE] );

	for ( int i = 0; i < NUM_SPHERES; i++ )
	{
		glPushMatrix();
		gltApplyActorTransform( &spheres[i] );
		glCallList( lSphereList );
		glPopMatrix();
	}

	glPushMatrix();
	glRotatef( -yRot * 2.0f, 0.0f, 1.0f, 0.0f );
	glTranslatef( 1.0f, 0.0f, 0.0f );
	glCallList( lSphereList );
	glPopMatrix();

	if ( nShadow == 0 )
	{
		glMaterialfv( GL_FRONT, GL_SPECULAR, fBrightLight );
	}


	glRotatef( yRot, 0.0f, 1.0f, 0.0f );
	glBindTexture( GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE] );
	glCallList( lTorusList );
	glMaterialfv( GL_FRONT, GL_SPECULAR, fNoLight );

	glPopMatrix();
}

void RenderScene()
{
	static int iFrames = 0;
	static float fps = 0.0f;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	glPushMatrix();
	gltApplyCameraTransform( &frameCamera );

	glLightfv( GL_LIGHT0, GL_POSITION, fLightPos );

	glColor3f( 1.0f, 1.0f, 1.0f );
	DrawGround();

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_STENCIL_TEST );
	glPushMatrix();
	glMultMatrixf( mShadowMatrix );
	DrawInhabitants( 1 );
	glPopMatrix();
	glDisable( GL_STENCIL_TEST );
	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );

	DrawInhabitants( 0 );

	glPopMatrix();

	iFrames++;
	if ( iFrames = 100 )
	{
		float fTime;

		LARGE_INTEGER lCurrent;
		QueryPerformanceCounter( &lCurrent );
		fTime = (float)(lCurrent.QuadPart - FPSCount.QuadPart) / (float)CounterFrequency.QuadPart;
		fps = (float)iFrames / fTime;

		iFrames = 0;
		QueryPerformanceCounter( &FPSCount );
	}

	if ( glWindowPos2i != NULL )
	{
		int iRow = 10;
		char cBuffer[64];

		glDisable( GL_DEPTH_TEST );
		glDisable( GL_LIGHTING );
		glDisable( GL_TEXTURE_2D );
		glColor3f( 1.0f, 1.0f, 1.0f );

		glWindowPos2i( 0, iRow );
		glListBase( nFontList );
		glCallLists( 13, GL_UNSIGNED_BYTE, "OpenGL Rocks!" );
		iRow += 20;

		if ( startupOptions.bFSAA == TRUE && startupOptions.nPixelFormatMS != 0 )
		{
			glWindowPos2i( 0, iRow );
			glCallLists( 25, GL_UNSIGNED_BYTE, "Multisampled Frame Buffer" );
			iRow += 20;
		}

		if ( wglSwapIntervalEXT != NULL && startupOptions.bVerticalSync == TRUE )
		{
			glWindowPos2i( 0, iRow );
			glCallLists( 9, GL_UNSIGNED_BYTE, "VSync On" );
			iRow += 20;
		}

		glEnable( GL_DEPTH_TEST );
		glEnable( GL_LIGHTING );
		glEnable( GL_TEXTURE_2D );
	}

}

void SetupRC( HDC hDC )
{
	GLTVector3 vPoints[3] = 
	{ 
		{0.0f, -0.4f, 0.0f},
		{10.0f, 0.4f, 0.0f},
		{5.0f, -0.4f, -5.0f}
	};

	int iSphere;
	int i;

	HFONT hFont;
	LOGFONT logfont;

	logfont.lfHeight = -20;
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
	
	hFont = CreateFontIndirect( &logfont );
	SelectObject( hDC, hFont );

	nFontList = glGenLists( 128 );
	wglUseFontBitmaps( hDC, 0, 128, nFontList );

	DeleteObject( hFont );

	glClearColor( fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3] );

	glStencilOp( GL_INCR, GL_INCR, GL_INCR );
	glClearStencil( 0 );
	glStencilFunc( GL_EQUAL, 0x0, 0x01 );

	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, fNoLight );
	glLightfv( GL_LIGHT0, GL_AMBIENT, fLowLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, fBrightLight );
	glLightfv( GL_LIGHT0, GL_SPECULAR, fBrightLight );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	gltMakeShadowMatrix( vPoints, fLightPos, mShadowMatrix );

	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glMateriali( GL_FRONT, GL_SHININESS, 128 );
	
	gltInitFrame( &frameCamera );
	for ( iSphere = 0; iSphere < NUM_SPHERES; iSphere++ )
	{
		gltInitFrame( &spheres[iSphere] );
		spheres[iSphere].vLocation[0] = (float)((rand() % 400) - 200) * 0.1f;
		spheres[iSphere].vLocation[1] = 0.0f;
		spheres[iSphere].vLocation[2] = (float)((rand() % 400) - 200) * 0.1f;

	}

	glEnable( GL_TEXTURE_2D );
	glGenTextures( NUM_TEXTURES, textureObjects );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	
	for ( i = 0; i < NUM_TEXTURES; i++ )
	{
		GLbyte *pBytes;
		GLint iWidth, iHeight, iComponents;
		GLenum eFormat;

		glBindTexture( GL_TEXTURE_2D, textureObjects[i] );

		pBytes = gltLoadTGA( szTextureFiles[i], &iWidth, &iHeight, &iComponents, &eFormat );
		gluBuild2DMipmaps( GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes );

		free( pBytes );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}

	glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress( "glWindowPos2i" );
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

	if ( wglSwapIntervalEXT != NULL && startupOptions.bVerticalSync == TRUE )
		wglSwapIntervalEXT( 1 );
	if ( startupOptions.bFSAA == TRUE && startupOptions.nPixelFormatMS != 0 )
		glEnable( GL_MULTISAMPLE_ARB );
	if ( gltIsExtSupported( "GL_EXT_separate_specular_color" ) )
		glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

	QueryPerformanceFrequency( &CounterFrequency );
	QueryPerformanceCounter( &FPSCount );
	CameraTimer = FPSCount;

	lTorusList = glGenLists( 2 );
	lSphereList = lTorusList + 1;

	glNewList( lTorusList, GL_COMPILE );
	gltDrawTorus( 0.35f, 0.15f, 61, 37 );
	glEndList();

	glNewList( lSphereList, GL_COMPILE );
	gltDrawSphere( 0.3f, 31, 16 );
	glEndList();
}

void ShutdownRC()
{
	glDeleteLists( nFontList, 128 );
	glDeleteLists( lTorusList, 2 );
	glDeleteTextures( NUM_TEXTURES, textureObjects );
}

HPALETTE GetOpenGLPalette( HDC hDC )
{
	HPALETTE hRetPal = NULL;
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE *pPal;
	int nPixelFormat;
	int nColors;
	int i;
	BYTE RedRange, GreenRange, BlueRange;

	nPixelFormat = GetPixelFormat( hDC );
	DescribePixelFormat( hDC, nPixelFormat, sizeof( PIXELFORMATDESCRIPTOR ), &pfd );

	if ( !(pfd.dwFlags & PFD_NEED_PALETTE) )
		return NULL;

	nColors = 1 << pfd.cColorBits;

	pPal = (LOGPALETTE*)malloc( sizeof( LOGPALETTE ) + nColors * sizeof( PALETTEENTRY ) );

	pPal->palVersion = 0x300;
	pPal->palNumEntries = nColors;

	RedRange = (1 << pfd.cRedBits) - 1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) - 1;

	for ( i = 0; i < nColors; i++ )
	{
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)((double)pPal->palPalEntry[i].peRed * 255.0 / RedRange);
		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)((double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);
		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)((double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char)NULL;
	}

	hRetPal = CreatePalette( pPal );

	SelectPalette( hDC, hRetPal, FALSE );
	RealizePalette( hDC );

	free( pPal );

	return hRetPal;

}


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);

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
	WNDCLASS wc;
	HWND hWnd;
	UINT uiStyle, uiStyleX;
	ghInstance = hInstance;

	if ( ShowStartupOptions() == FALSE )
		return 0;

	if ( startupOptions.bFullScreen == TRUE )
		if ( ChangeDisplaySettings( &startupOptions.devMode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
		{
			MessageBox( NULL, TEXT( "Cannot change to selected desktop resolution." ), NULL, MB_OK | MB_ICONSTOP );
			return -1;
		}

	


	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;//MAKEINTRESOURCE(IDC_GLRECT);
	wc.lpszClassName = lpszAppName;

	if ( RegisterClass( &wc ) == 0 )
		return FALSE;

	if ( startupOptions.bFullScreen == TRUE )
	{
		uiStyle = WS_POPUP;
		uiStyleX = WS_EX_TOPMOST;
	}
	else
	{
		uiStyle = WS_OVERLAPPEDWINDOW;
		uiStyleX = 0;
	}

	hWnd = CreateWindowEx( uiStyleX, wc.lpszClassName, lpszAppName, uiStyle, 0, 0, startupOptions.devMode.dmPelsWidth, startupOptions.devMode.dmPelsHeight, NULL, NULL, hInstance, NULL );

	if ( hWnd == NULL )
		return NULL;

	

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );


	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if ( startupOptions.bFullScreen == TRUE )
		ChangeDisplaySettings( NULL, 0 );

	return (int) msg.wParam;
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
	static HDC hDC = NULL;
	static HGLRC hRC = NULL;

	switch ( message )
	{
	case WM_CREATE:
		hDC = GetDC( hWnd );
		FindBestPF( hDC, &startupOptions.nPixelFormat, &startupOptions.nPixelFormatMS );
		if ( startupOptions.bFSAA == TRUE && (startupOptions.nPixelFormatMS != 0) )
			SetPixelFormat( hDC, startupOptions.nPixelFormatMS, NULL );
		else
			SetPixelFormat( hDC, startupOptions.nPixelFormat, NULL );

		hRC = wglCreateContext( hDC );
		wglMakeCurrent( hDC, hRC );

		hPalette = GetOpenGLPalette( hDC );

		SetupRC( hDC );

		//SetTimer( hWnd, 33, 1, NULL );
		break;
	case WM_CHAR:
		if ( wParam == 27 )
			DestroyWindow( hWnd );
		break;
	case WM_ACTIVATE:
		if ( startupOptions.bFullScreen == TRUE )
		{
			WINDOWPLACEMENT wndPlacement;
			wndPlacement.length = sizeof( WINDOWPLACEMENT );
			wndPlacement.flags = WPF_RESTORETOMAXIMIZED;
			wndPlacement.ptMaxPosition.x = 0;
			wndPlacement.ptMaxPosition.y = 0;
			wndPlacement.ptMinPosition.x = 0;
			wndPlacement.ptMinPosition.y = 0;
			wndPlacement.rcNormalPosition.bottom = startupOptions.devMode.dmPelsHeight;
			wndPlacement.rcNormalPosition.left = 0;
			wndPlacement.rcNormalPosition.top = 0;
			wndPlacement.rcNormalPosition.right = startupOptions.devMode.dmPelsWidth;

			if ( LOWORD( wParam ) == WA_INACTIVE )
			{
				wndPlacement.showCmd = SW_SHOWMINNOACTIVE;
				SetWindowPlacement( hWnd, &wndPlacement );
				ShowCursor( TRUE );
			}
			else
			{
				wndPlacement.showCmd = SW_RESTORE;
				SetWindowPlacement( hWnd, &wndPlacement );
				ShowCursor( FALSE );
			}
		}
		break;
	case WM_DESTROY:
		ShutdownRC();
		//KillTimer( hWnd, 101 );
		wglMakeCurrent( hDC, NULL );
		wglDeleteContext( hRC );
		if ( hPalette != NULL )
			DeleteObject( hPalette );
		PostQuitMessage( 0 );
		break;
	case WM_SIZE:
		ChangeSize( LOWORD( lParam ), HIWORD( lParam ) );
		break;
	case WM_PAINT:
	{
		if ( GetFocus() == hWnd )
		{
			float fTime;
			float fLinear, fAngular;
			LARGE_INTEGER lCurrent;
			QueryPerformanceCounter( &lCurrent );

			fTime = (float)(lCurrent.QuadPart - CameraTimer.QuadPart) / (float)(CounterFrequency.QuadPart);
			CameraTimer = lCurrent;

			fLinear = fTime * 1.0f;
			fAngular = (float)gltDegToRad( 60.0f * fTime );

			if ( GetAsyncKeyState( VK_UP ) )
				gltMoveFrameForward( &frameCamera, fLinear );
			if ( GetAsyncKeyState( VK_DOWN ) )
				gltMoveFrameForward( &frameCamera, -fLinear );
			if ( GetAsyncKeyState( VK_LEFT ) )
				gltRotateFrameLocalY( &frameCamera, fAngular );
			if ( GetAsyncKeyState( VK_UP ) )
				gltRotateFrameLocalY( &frameCamera, -fAngular );
		}
		RenderScene();
		SwapBuffers( hDC );
		ValidateRect( hWnd, NULL );
		break;
	}
	case WM_QUERYNEWPALETTE:
		if ( hPalette )
		{
			int nRet;

			SelectPalette( hDC, hPalette, FALSE );

			nRet = RealizePalette( hDC );

			InvalidateRect( hWnd, NULL, FALSE );
			return nRet;
		}
		break;
	case WM_PALETTECHANGED:
		if ( (hPalette != NULL) && ((HWND)wParam != hWnd) )
		{
			SelectPalette( hDC, hPalette, FALSE );

			RealizePalette( hDC );

			UpdateColors( hDC );
			return 0;
		}
		break;
	case WM_TIMER:
		InvalidateRect( hWnd, NULL, FALSE );
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL APIENTRY StartupDlgProc( HWND hDlg, UINT message, UINT wParam, LONG lParam )
{
	switch ( message )
	{
	case WM_INITDIALOG:
	{
		int nPF;
		HDC hDC;
		HGLRC hRC;
		DEVMODE devMode;
		unsigned int iMode;
		unsigned int nWidth;
		unsigned int nHeight;
		char cBuffer[64];
		HWND hListBox;

		PIXELFORMATDESCRIPTOR pfd = {
			sizeof( PIXELFORMATDESCRIPTOR ),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0,
			16,
			8,
			0, 0, 0, 0, 0, 0
		};

		startupOptions.bFSAA = FALSE;
		startupOptions.bFullScreen = FALSE;
		startupOptions.bVerticalSync = FALSE;

		hDC = GetDC( hDlg );
		nPF = ChoosePixelFormat( hDC, &pfd );
		SetPixelFormat( hDC, nPF, &pfd );
		DescribePixelFormat( hDC, nPF, sizeof( PIXELFORMATDESCRIPTOR ), &pfd );

		hRC = wglCreateContext( hDC );
		wglMakeCurrent( hDC, hRC );

		SetDlgItemText( hDlg, IDC_VENDOR, (const char*)glGetString( GL_VENDOR ) );
		SetDlgItemText( hDlg, IDC_RENDERER, (const char*)glGetString( GL_RENDERER ) );
		SetDlgItemText( hDlg, IDC_VERSION, (const char*)glGetString( GL_VERSION ) );

		if ( gltIsExtSupported( "WGL_EXT_swap_control" ) )
		{
			EnableWindow( GetDlgItem( hDlg, IDC_VSYNC_CHECK ), TRUE );
		}

		FindBestPF( hDC, &startupOptions.nPixelFormat, &startupOptions.nPixelFormatMS );

		wglMakeCurrent( hDC, NULL );
		wglDeleteContext( hRC );

		iMode = 0;
		nWidth = GetSystemMetrics( SM_CXSCREEN );
		nHeight = GetSystemMetrics( SM_CYSCREEN );
		hListBox = GetDlgItem( hDlg, IDC_DISPLAY_COMBO );
		while ( EnumDisplaySettings( NULL, iMode, &devMode ) )
		{
			int iItem;
			sprintf( cBuffer, "%d x %d x %dbpp @%dhz", devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency );

			iItem = SendMessage( hListBox, CB_ADDSTRING, 0, (LPARAM)cBuffer );
			SendMessage( hListBox, CB_SETITEMDATA, iItem, iMode );

			if ( devMode.dmPelsHeight == nHeight && devMode.dmPelsWidth == nWidth )
				SendMessage( hListBox, CB_SETCURSEL, iItem, 0 );
			iMode++;
		}

		CheckDlgButton( hDlg, IDC_FS_CHECK, BST_CHECKED );

		if ( startupOptions.nPixelFormatMS != 0 )
		{
			EnableWindow( GetDlgItem( hDlg, IDC_MULTISAMPLED_CHECK ), TRUE );
		}

		return TRUE;

	}
	break;
	case WM_COMMAND:
	{
		if ( LOWORD( wParam ) == IDOK )
		{
			HWND hListBox = GetDlgItem( hDlg, IDC_DISPLAY_COMBO );
			int iMode = SendMessage( hListBox, CB_GETCURSEL, 0, 0 );
			iMode = SendMessage( hListBox, CB_GETITEMDATA, iMode, 0 );
			EnumDisplaySettings( NULL, iMode, &startupOptions.devMode );

			if ( IsDlgButtonChecked( hDlg, IDC_FS_CHECK ) )
			{
				startupOptions.bFullScreen = TRUE;
			}
			else
			{
				startupOptions.bFullScreen = FALSE;
			}

			if ( IsDlgButtonChecked( hDlg, IDC_MULTISAMPLED_CHECK ) )
			{
				startupOptions.bFSAA = TRUE;
			}
			else
			{
				startupOptions.bFSAA = FALSE;
			}

			if ( IsDlgButtonChecked( hDlg, IDC_VSYNC_CHECK ) )
			{
				startupOptions.bVerticalSync = TRUE;
			}
			else
			{
				startupOptions.bVerticalSync = FALSE;
			}
			EndDialog( hDlg, TRUE );
		}

		if ( LOWORD( wParam ) == IDCANCEL )
			EndDialog( hDlg, FALSE );
	}
	break;
	case WM_CLOSE:
		EndDialog( hDlg, FALSE );
		break;
	}

	return FALSE;
}

BOOL ShowStartupOptions()
{
	return DialogBox( ghInstance, MAKEINTRESOURCE( IDD_DLG_INTRO ), NULL, StartupDlgProc );
}

void FindBestPF( HDC hDC, int *nRegularFormat, int *nMSFormat )
{
	*nRegularFormat = 0;
	*nMSFormat = 0;

	if ( gltIsWGLExtSupported( hDC, "WGL_ARB_pixel_format" ) )
		if ( wglGetPixelFormatAttribivARB == NULL )
			wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress( "wglGetPixelFormatAttribivARB" );

	if ( wglGetPixelFormatAttribivARB != NULL )
	{
		int nBestMS = 0;
		int i;
		int iResults[9];
		int iAttributes[9] = {
			WGL_SUPPORT_OPENGL_ARB,
			WGL_ACCELERATION_ARB,
			WGL_DRAW_TO_WINDOW_ARB,
			WGL_DOUBLE_BUFFER_ARB,
			WGL_PIXEL_TYPE_ARB,
			WGL_DEPTH_BITS_ARB,
			WGL_STENCIL_BITS_ARB,
			WGL_SAMPLE_BUFFERS_ARB,
			WGL_SAMPLES_ARB
		};

		int nFormatCount[] = { 0 };
		int attrib[] = { WGL_NUMBER_PIXEL_FORMATS_ARB };
		wglGetPixelFormatAttribivARB( hDC, 1, 0, 1, attrib, nFormatCount );

		for ( int i = 0; i < nFormatCount[0]; i++ )
		{
			wglGetPixelFormatAttribivARB( hDC, i + 1, 0, 9, iAttributes, iResults );

			if ( iResults[0] == 1 && iResults[1] == WGL_FULL_ACCELERATION_ARB && iResults[2] == 1 )
				if ( iResults[3] == 1 )
					if ( iResults[4] == WGL_TYPE_RGBA_ARB )
						if ( iResults[5] >= 16 )
							if ( iResults[6] > 0 )
							{
								if ( iResults[7] == 1 )
								{
									if ( iResults[8] > nBestMS )
									{
										*nMSFormat = i;
										nBestMS = iResults[8];
									}
								}
								else
								{
									*nRegularFormat = i;
								}
							}
		}
	}
	else
	{
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof( PIXELFORMATDESCRIPTOR ),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0,
			24,
			8,
			0, 0, 0, 0, 0, 0
		};

		*nRegularFormat = ChoosePixelFormat( hDC, &pfd );
	}
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
