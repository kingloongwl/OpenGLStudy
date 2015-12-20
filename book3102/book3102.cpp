// book3102.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

void RenderScene()
{
	//清除蓝色窗口
	glClearColor( 0.0f, 0.0f, 1.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	//现在把裁剪框设置为一个更小的红色子区域
	glClearColor( 1.0f, 0.0f, 0.0f, 0.0f );
	glScissor( 100, 100, 600, 400 );
	glEnable( GL_SCISSOR_TEST );
	glClear( GL_COLOR_BUFFER_BIT );

	//最后设置一个更小的绿色矩形
	glClearColor( 0.0f, 1.0f, 0.0f, 0.0f );
	glScissor( 200, 200, 400, 200 );
	glClear( GL_COLOR_BUFFER_BIT );

	//关闭裁剪测试，以便进行下一次渲染
	glDisable( GL_SCISSOR_TEST );

	glutSwapBuffers();

}

void ChangeSize( int w, int h )
{
	GLfloat nRange = 100;

	glViewport( 0, 0, w, h );

	if ( h == 0 )
	{
		h = 1;
	}

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	if ( w > h )
	{
		glOrtho( -nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange );
	}
	else
	{
		glOrtho( -nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange );
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutCreateWindow( "SCISSOR" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );

	glutMainLoop();

	return 0;
}

