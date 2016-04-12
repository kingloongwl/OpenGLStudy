
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLEXT.H"
#include "WGLEXT.H"
#include "GLTools.h"

#include <math.h>
#include <stdio.h>



//存储图像数据
static GLbyte *pImage = NULL;
static GLint iWidth, iHeight, iComponent;
static GLenum eFormat;
static GLboolean bHistogram = GL_FALSE;

//目标绘图模式
static GLint iRenderMode = 1;

void SetupRC()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	//Targas是按1个字节对齐的，GL_UNPACK_ALIGNMENT指定如何从数据缓冲区对图像数据进行解包
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	//载入图像数据
	pImage = gltLoadTGA( "horse.tga", &iWidth, &iHeight, &iComponent, &eFormat );

}

void ShotdownRC()
{
	free( pImage );
}

void ProcessMenu( int value )
{
	if ( value == 6 )
	{
		bHistogram = GL_TRUE;
		glutPostRedisplay();
		return;
	}
	
	if ( value == 0 )
	{
		gltWriteTGA( "ScreenShot.tga" );
	}
	else
	{
		iRenderMode = value;
	}

	//触发重绘
	glutPostRedisplay();
}

void RenderScene()
{
	GLint i;
	GLint iViewport[ 4 ];
	GLint iLargest;
	static GLubyte invertTable[256][3];
/*
	static GLfloat lumMat[16] = {
		0.3f, 0.3f, 0.3f, 0.0f,
		0.59f, 0.59f, 0.59f, 0.0f,
		0.11f, 0.11f, 0.11f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

	static GLfloat mSharpen[3][3] = {
		{0.0f, -1.0f, 0.0f},
		{-1.0f, 5.0f, -1.0f},
		{0.0f, -1.0f, 0.0f}
	};

	static GLfloat mEmboss[3][3] = {
		{2.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, 0.0f, -1.0f}
	};

	static GLint histoGram[256];

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, 0 );
	glGetIntegerv(GL_VIEWPORT, iViewport);
	glPixelZoom((GLfloat)iViewport[2] / (GLfloat)iWidth, (GLfloat)iViewport[3] / (GLfloat)iHeight);

	if ( bHistogram == GL_TRUE ) {
		glMatrixMode( GL_COLOR );
		glLoadMatrixf( lumMat );
		glMatrixMode( GL_MODELVIEW );

		glHistogram( GL_HISTOGRAM, 256, GL_LUMINANCE, GL_FALSE );
		glEnable( GL_HISTOGRAM );
	}

	switch (iRenderMode) {
		case 5:
			glConvolutionFilter2D( GL_CONVOLUTION_2D, GL_RGB, 3, 3, GL_LUMINANCE, GL_FLOAT, mSharpen );
			glEnable( GL_CONVOLUTION_2D );
			break;
		case 4:
			glConvolutionFilter2D( GL_CONVOLUTION_2D, GL_RGB, 3, 3, GL_LUMINANCE, GL_FLOAT, mEmboss );
			glEnable( GL_CONVOLUTION_2D );
			glMatrixMode( GL_COLOR );
			glLoadMatrixf( lumMat );
			glMatrixMode( GL_MODELVIEW );
			break;
		case 3:
			for ( i = 0; i < 255; i++ ) {
				invertTable[i][0] = (GLubyte)(255 - i);
				invertTable[i][1] = (GLubyte)(255 - i);
				invertTable[i][2] = (GLubyte)(255 - i);
			}

			glColorTable( GL_COLOR_TABLE, GL_RGB, 256, GL_RGB, GL_UNSIGNED_BYTE, invertTable );
			glEnable( GL_COLOR_TABLE );
			break;
		case 2:
			glMatrixMode( GL_COLOR );
			glScalef( 1.25f, 1.25f, 1.25f );
			glMatrixMode( GL_MODELVIEW );
			break;
		case 1:
		default:
			break;
	}

	glDrawPixels( iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage );

	if ( bHistogram == GL_TRUE )
	{
		glGetHistogram( GL_HISTOGRAM, GL_TRUE, GL_LUMINANCE, GL_INT, histoGram );

		iLargest = 0;
		for ( i =0; i < 255; i++) {
			if ( iLargest < histoGram[i] ) {
				iLargest = histoGram[i];
			}
		}

		glColor3f( 1.0f, 1.0f, 1.0f );
		glBegin( GL_LINE_STRIP );
		for ( i = 0; i < 255; i++ ) {
			glVertex2f( (GLfloat)i, (GLfloat)histoGram[i] / (GLfloat)iLargest * 128.0f );
		}

		glEnd();

		bHistogram = GL_FALSE;
		glDisable( GL_HISTOGRAM );
	}

	glMatrixMode( GL_COLOR );
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glDisable( GL_CONVOLUTION_2D );
	glDisable( GL_COLOR_TABLE );
*/
    glutSwapBuffers();
}

void ChangeSize( int w, int h )
{
	if ( h == 0 )
	{
		h = 1;
	}

	glViewport( 0, 0, w, h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluOrtho2D( 0.0f, (GLfloat)w, 0.0, (GLfloat)h );

	glMatrixMode( GL_MODELVIEW );

	glLoadIdentity();
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize( 600, 600 );
	glutCreateWindow( "OpenGL Imaging subset" );

	if ( gltIsExtSupported( "GL_ARB_imaging" ) == 0 )
	{
		printf( "Imaging subset not supported\r\n" );
		return 0;
	}

	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );

	//菜单
	glutCreateMenu( ProcessMenu );
	glutAddMenuEntry( "Save Image", 0 );
	glutAddMenuEntry( "Raw Stretched Image", 1 );
	glutAddMenuEntry( "Increase Contrast", 2 );
	glutAddMenuEntry( "Invert Color", 3 );
	glutAddMenuEntry( "Emboss Image", 4 );
	glutAddMenuEntry( "Sharpen Image", 5 );
	glutAddMenuEntry( "Histogram", 6 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	SetupRC();

	glutMainLoop();

	ShotdownRC();

	return 0;
}
