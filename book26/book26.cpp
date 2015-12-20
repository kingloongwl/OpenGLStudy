// book26.cpp : Defines the entry point for the console application.
//


#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

/* ����ĳ�ʼλ�ô�С */
GLfloat x1 = 0.0f;
GLfloat y1 = 0.0f;
GLfloat rsize = 25;

/* ÿ���ƶ��������� */
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

/* ׷�ٴ��ڿ�Ⱥ͸߶ȵı仯 */
GLfloat windowWidth;
GLfloat windowHeight;



void TimerFunction( int value )
{
	//�ڴﵽ��߻��ұ�ʱ��ת����
	if ( x1 > windowWidth-rsize || x1 < -windowWidth )
	{
		xstep = -xstep;
	}

	//�ڴﵽ���߻�ױ�ʱ��ת����
	if ( y1 > windowHeight || y1 < -windowHeight + rsize )
	{
		ystep = -ystep;
	}

	x1 += xstep;
	y1 += ystep;

	//���߽硣����Ϊ�˷�ֹ�����ڷ���ʱ���ڱ�С��ʹ����������µĲü�����֮��
	if ( x1 > (windowWidth-rsize+xstep) )
	{
		x1 = windowWidth-rsize-1;
	}
	else if ( x1 < -(windowWidth+xstep) )
	{
		x1 = -windowWidth - 1;
	}

	if ( y1 > (windowHeight+ystep) )
	{
		y1 = windowHeight - 1;
	}
	else if ( y1 < -(windowHeight-rsize+ystep) )
	{
		y1 = -windowHeight+rsize-1;
	}

	glutPostRedisplay();
//	glutTimerFunc( 33, TimerFunction, 1 );
}

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT );

	glColor3f( 1.0f, 0.0f, 0.0f );

	glRectf( x1, y1, x1+rsize, y1-rsize );

	/* ˢ�»�ͼָ����н��� */
	glutSwapBuffers();

	glutTimerFunc( 33, TimerFunction, 1 );
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

	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	aspectRatio = (GLfloat)w/(GLfloat)h;
	if ( w<=h )
	{
		windowWidth = 100;
		windowHeight = 100/aspectRatio;
		glOrtho( -100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0 );
	}
	else
	{
		windowWidth = 100 * aspectRatio;
		windowHeight = 100;
		glOrtho( -windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0 );
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, argv );

	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutCreateWindow( "Bounce" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );
	glutTimerFunc( 33, TimerFunction, 1 );


	SetupRC();

	glutMainLoop();

	return 0;
}

