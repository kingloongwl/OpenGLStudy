// book253.cpp : Defines the entry point for the console application.
//



#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/glut.h>

void RenderScene()
{
	/* 用当前清除颜色清除窗口 */
	glClear( GL_COLOR_BUFFER_BIT );

	/* 刷新绘图指令，执行所有未执行的OpenGL命令 */
	glFlush();
}

void SetupRC()
{
	/* 设置用于清除窗口的颜色 */
	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
}

int main(int argc, char* argv[])
{
	/* 使用单缓冲窗口和RGBA颜色模式 */
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );

	/* 创建OpenGL窗口 */
	glutCreateWindow( "simple" );

	/* 设置显示回调函数，即当窗口需要重新绘制（改变大小）时所要调用的函数 */
	glutDisplayFunc( RenderScene );

	SetupRC();

	glutMainLoop();

	return 0;
}

