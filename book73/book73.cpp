#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"

void SetupRC()
{
	
}

void RenderScene()
{
	GLbyte *pImage = NULL;
	GLint iWidth, iHeight, iComponents;
	GLenum eFormat;

	glClear( GL_COLOR_BUFFER_BIT );

	//Targas是按1个字节对齐的，GL_UNPACK_ALIGNMENT指定如何从数据缓冲区对图像数据进行解包
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	//载入TGA文件，获取宽度，高度和成分、格式信息
	pImage = gltLoadTGA( "Fire.tga", &iWidth, &iHeight, &iComponents, &eFormat );

	//使用窗口位标设置光栅位置，也就是画图时左下角在哪里
	glRasterPos2i( 0, 0 );

	//绘制像素图
	if ( pImage != NULL )
	{
		glDrawPixels( iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage );
	}

	//不再需要图像数据
	free( pImage );

	//复制像素
	glRasterPos2i( 400, 300 );	//指定目标地点的起始位置，左下角	
	glCopyPixels( 0, 0, 200, 200, GL_COLOR );	//执行copy，参数是矩形区域与格式，格式可以是GL_COLOR,GL_DEPTH,GLSTENCIL


	//进行缓冲区交换
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

	gluOrtho2D( 0, w, 0, h );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

}

int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "TGA" );

	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );

	glutMainLoop();

	return 0;
}