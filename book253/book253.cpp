// book253.cpp : Defines the entry point for the console application.
//



#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/glut.h>

void RenderScene()
{
	/* �õ�ǰ�����ɫ������� */
	glClear( GL_COLOR_BUFFER_BIT );

	/* ˢ�»�ͼָ�ִ������δִ�е�OpenGL���� */
	glFlush();
}

void SetupRC()
{
	/* ��������������ڵ���ɫ */
	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
}

int main(int argc, char* argv[])
{
	/* ʹ�õ����崰�ں�RGBA��ɫģʽ */
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );

	/* ����OpenGL���� */
	glutCreateWindow( "simple" );

	/* ������ʾ�ص�����������������Ҫ���»��ƣ��ı��С��ʱ��Ҫ���õĺ��� */
	glutDisplayFunc( RenderScene );

	SetupRC();

	glutMainLoop();

	return 0;
}

