
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"

//存储图像数据
static GLbyte *pImage;
static GLint iWidth, iHeight, iComponent;
static GLenum eFormat;

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
	GLint iViewport[ 4 ];
	GLbyte *pModefiedBytes = NULL;
	GLfloat invertMap[ 256 ];
	GLint i;

	glClear( GL_COLOR_BUFFER_BIT );

	//设置光栅位置
	glRasterPos2i( 0, 0 );

	switch ( iRenderMode )
	{
	case 2: //翻转像素
		glPixelZoom( -1, -1 );
		glRasterPos2i( iWidth, iHeight );
		break;
	case 3: //放大像素以填充窗口
		glGetIntegerv( GL_VIEWPORT, iViewport );
		glPixelZoom( ( GLfloat )iViewport[ 2 ] / ( GLfloat)iWidth,
			( GLfloat )iViewport[ 3 ] / ( GLfloat )iHeight );
		break;
	case 4: //只保留红色
		glPixelTransferf( GL_RED_SCALE, 1.0f );
		glPixelTransferf( GL_GREEN_SCALE, 0.0f );
		glPixelTransferf( GL_BLUE_SCALE, 0.0f );
		break;
	case 5: //只保留绿色
		glPixelTransferf( GL_RED_SCALE, 0.0f );
		glPixelTransferf( GL_GREEN_SCALE, 1.0f );
		glPixelTransferf( GL_BLUE_SCALE, 0.0f );
		break;
	case 6: //只保留蓝色
		glPixelTransferf( GL_RED_SCALE, 0.0f );
		glPixelTransferf( GL_GREEN_SCALE, 0.0f );
		glPixelTransferf( GL_BLUE_SCALE, 1.0f );
		break;
	case 7: //黑色和白色
		//先绘制到颜色缓冲区
		glDrawPixels( iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage );

		//为强度映射分配空间
		pModefiedBytes = ( GLbyte* ) malloc( iWidth * iHeight );

		//根据NSTC对颜色进行调整
		glPixelTransferf( GL_RED_SCALE, 0.3f );
		glPixelTransferf( GL_GREEN_SCALE, 0.59f );
		glPixelTransferf( GL_BLUE_SCALE, 0.11f );

		//把像素读到缓冲区，GL_LUMINANCE将图像转换为亮度图像
		glReadPixels( 0, 0, iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModefiedBytes );

		//还原颜色调整
		glPixelTransferf( GL_RED_SCALE, 1.0f );
		glPixelTransferf( GL_GREEN_SCALE, 1.0f );
		glPixelTransferf( GL_BLUE_SCALE, 1.0f );
		break;
	case 8: //颜色映射
		for ( i = 1; i < 256; i++ )
		{
			invertMap[ i ] = 1.0f - ( 1.0f / 255.0f * ( GLfloat )i );
		}

		glPixelMapfv( GL_PIXEL_MAP_R_TO_R, 255, invertMap );
		glPixelMapfv( GL_PIXEL_MAP_G_TO_G, 255, invertMap );
		glPixelMapfv( GL_PIXEL_MAP_B_TO_B, 255, invertMap );
		glPixelTransferi( GL_MAP_COLOR, GL_TRUE );
		break;
	case 1:

	default:
		break;
	}

	if ( pModefiedBytes == NULL )
	{
		glDrawPixels( iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage );
	}
	else
	{
		glDrawPixels( iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModefiedBytes );
		free( pModefiedBytes );
	}

	//恢复成默认值
	glPixelTransferi( GL_MAP_COLOR, GL_FALSE );
	glPixelTransferf( GL_RED_SCALE, 1.0f );
	glPixelTransferf( GL_GREEN_SCALE, 1.0f );
	glPixelTransferf( GL_BLUE_SCALE, 1.0f );
	glPixelZoom( 1.0f, 1.0f );

	glutSwapBuffers();
}

void ChangeScene( int w, int h )
{
	if ( h == 0 )
	{
		h = 1;
	}

	glViewport( 0, 0, w, h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluOrtho2D( 0.0f, ( GLfloat )w, 0.0f, ( GLfloat )h );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "OPERATION" );
	glutReshapeFunc( ChangeScene );
	glutDisplayFunc( RenderScene );

	//菜单
	glutCreateMenu( ProcessMenu );
	glutAddMenuEntry( "Save Image", 0 );
	glutAddMenuEntry( "DrawPixel", 1 );
	glutAddMenuEntry( "FlipPixels", 2 );
	glutAddMenuEntry( "ZoomPixels", 3 );
	glutAddMenuEntry( "Just Red Channel", 4 );
	glutAddMenuEntry( "Just Green Channel", 5 );
	glutAddMenuEntry( "Just Blue Channel", 6 );
	glutAddMenuEntry( "Black And White", 7 );
	glutAddMenuEntry( "Invert Color", 8 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	SetupRC();

	glutMainLoop();

	ShotdownRC();

	return 0;
}