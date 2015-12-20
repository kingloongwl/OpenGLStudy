// book3101.cpp : Defines the entry point for the console application.
//


#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include <math.h>

void SetupRC()
{
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	glColor3f( 1.0, 0.0, 0.0 );
}

void Timer( int value )
{
//	glutTimerFunc( 50, Timer, 0 );
	glutPostRedisplay(); //重新绘制场景
}

void RenderScene()
{
	static GLdouble dRadius = 0.1;
	static GLdouble dAngle = 0.0;

	if ( dAngle == 0.0 )
	{
		glClear( GL_COLOR_BUFFER_BIT );
	}

	glBegin( GL_POINTS );
	
	glVertex2d( dRadius*cos(dAngle), dRadius*sin(dAngle) );

	glEnd();

	dRadius *= 1.01;
	dAngle += 0.1;

	if ( dAngle > 30.0 )
	{
		dRadius = 0.1;
		dAngle = 0.0;
	}

	glFlush();

	glutTimerFunc( 50, Timer, 0 ); //第一个参数是时间间隔，第二个是函数指针，第三个是传给函数的参数
}

void ChangeSize( int w, int h )
{
	GLfloat nRange = 2.0f;
	
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
	glutInitDisplayMode( GLUT_RGB );
	glutCreateWindow( "SINGLE" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );

	SetupRC();

	glutTimerFunc( 50, Timer, 0 );

	glutMainLoop();

	return 0;
}

