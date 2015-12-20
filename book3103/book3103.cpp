// book3103.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include <math.h>

/* ����ĳ�ʼλ�ô�С */
GLfloat xs = 0.0f;
GLfloat ys = 0.0f;
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
	if ( xs > windowWidth-rsize || xs < -windowWidth )
	{
		xstep = -xstep;
	}

	//�ڴﵽ���߻�ױ�ʱ��ת����
	if ( ys > windowHeight || ys < -windowHeight + rsize )
	{
		ystep = -ystep;
	}

	xs += xstep;
	ys += ystep;

	//���߽硣����Ϊ�˷�ֹ�����ڷ���ʱ���ڱ�С��ʹ����������µĲü�����֮��
	if ( xs > (windowWidth-rsize+xstep) )
	{
		xs = windowWidth-rsize-1;
	}
	else if ( xs < -(windowWidth+xstep) )
	{
		xs = -windowWidth - 1;
	}

	if ( ys > (windowHeight+ystep) )
	{
		ys = windowHeight - 1;
	}
	else if ( ys < -(windowHeight-rsize+ystep) )
	{
		ys = -windowHeight+rsize-1;
	}

	glutPostRedisplay();
	//	glutTimerFunc( 33, TimerFunction, 1 );
}

void RenderScene()
{
	GLdouble dRadius = 0.1;//����������뾶
	GLdouble dAngle;//ѭ������

	//�����ɫ����
	glClearColor( 0.0, 0.0, 1.0, 0.0 );

	//ʹ��0��ʾ���ģ�壬����ģ�����
	glClearStencil( 0.0f );
	glEnable( GL_STENCIL_TEST );

	//�����ɫ��ģ�建����
	glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	//�����޷�ͨ��ģ����ԵĻ�ͼ��������л�ͼ����������ģ�建�����ڵ�ֵ
	glStencilFunc( GL_NEVER, 0x0, 0x0 );
	glStencilOp( GL_INCR, GL_INCR, GL_INCR );

	//����ģ�ͽ�����ģ��ģ�ͣ��ð��߻�������ģ��
	//�����ð�ɫ��������Щ�ߣ���˵��ģ�庯����ֹ�����Ǳ�����
	glColor3f( 1.0, 1.0, 1.0 );
	glBegin( GL_LINE_STRIP );
	for ( dAngle = 0; dAngle < 400.0; dAngle += 0.1 )
	{
		glVertex2d( dRadius * cos( dAngle ), dRadius *sin( dAngle ) );
		dRadius *= 1.002;
	}
	glEnd();

	//���ڣ�������л�ͼ������������Щģ��ģ����0x1�ĵط������Ҳ����ģ�建���������޸�
	glStencilFunc( GL_NOTEQUAL, 0x1, 0x1 );
	glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

	//���ڻ��ƺ�ɫ��������
	//(x��y����һ����ʱ�������޸�
	glColor3f( 1.0, 0.0, 0.0 );                //ģ�建�����Ѿ������ͼ����������ģ��ģ�͵���0x1�ĵط���ͼ������ط��Ѿ�
	glRectf( xs, ys, xs+rsize, ys-rsize );     //�趨���������ߣ����Ժ�ɫ�������������ߵĵط����ᱻ����

	glutSwapBuffers();

	glutTimerFunc( 33, TimerFunction, 1 );

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

	//����Ҫ����ģ�建����
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_STENCIL );
	glutCreateWindow( "STENCIL" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );
	glutTimerFunc( 33, TimerFunction, 1 );

	glutMainLoop();

	return 0;
}

