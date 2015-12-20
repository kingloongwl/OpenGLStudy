// book254.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT );

	/* 设置绘图颜色 */
	glColor3f( 1.0f, 0.0f, 0.0f );

	/* 用绘图颜色绘制一个填充矩形，参数是左上角坐标和右下角坐标 */
	/* 此处参数的度量主要是以裁剪区域为标准，裁剪区域负责确定纵横比 */
	glRectf( -25.0f, 25.0f, 25.0f, -25.0f );

	glFlush();
}

void SetupRC()
{
	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
}

void ChangeSize( GLsizei w, GLsizei h )
{
	GLfloat aspectRatio;

	if ( h == 0 )
	{
		h = 1;
	}

	/* 设置视口大小为窗口大小 */
	glViewport( 0, 0, w, h );

	/* 重置坐标系统 */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	/*
	* 建立裁剪区域，此处建立的裁剪区域保持纵横比（垂直和水平单位长度内象素数量之比）不变
	* 在绘制的时候glRectf( -25.0f, 25.0f, 25.0f, -25.0f );，保证了25个单位的象素是一样多的，确保是一个正方形
	*/
	aspectRatio = ( GLfloat )w / (GLfloat)h;
	if ( w <= h )
	{
		/*
		* 横向共有200个单位，若要保持纵横比则纵向应有200/aspectRatio个单位
		* 就是定义所能看到的区域的六个面的坐标
		*/
		glOrtho( -100.0, 100.0, -100/aspectRatio, 100.0/aspectRatio, 1.0, -1.0 );
	}
	else
	{
		glOrtho( -100.0*aspectRatio, 100.0*aspectRatio, -100.0, 100.0, 1.0, -1.0 );
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

}

int main(int argc, char* argv[])
{

	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
	glutCreateWindow( "GLRect" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );

	SetupRC();

	glutMainLoop();

	return 0;
}

