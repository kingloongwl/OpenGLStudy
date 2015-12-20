// book254.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT );

	/* ���û�ͼ��ɫ */
	glColor3f( 1.0f, 0.0f, 0.0f );

	/* �û�ͼ��ɫ����һ�������Σ����������Ͻ���������½����� */
	/* �˴������Ķ�����Ҫ���Բü�����Ϊ��׼���ü�������ȷ���ݺ�� */
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

	/* �����ӿڴ�СΪ���ڴ�С */
	glViewport( 0, 0, w, h );

	/* ��������ϵͳ */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	/*
	* �����ü����򣬴˴������Ĳü����򱣳��ݺ�ȣ���ֱ��ˮƽ��λ��������������֮�ȣ�����
	* �ڻ��Ƶ�ʱ��glRectf( -25.0f, 25.0f, 25.0f, -25.0f );����֤��25����λ��������һ����ģ�ȷ����һ��������
	*/
	aspectRatio = ( GLfloat )w / (GLfloat)h;
	if ( w <= h )
	{
		/*
		* ������200����λ����Ҫ�����ݺ��������Ӧ��200/aspectRatio����λ
		* ���Ƕ������ܿ���������������������
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

