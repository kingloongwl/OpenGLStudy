// book361.cpp : Defines the entry point for the console application.
//


#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include <math.h>

#define GL_PI 3.1415f

void SetupRC()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glColor3f( 1.0f, 0.0f, 0.0f );
}

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT );

	GLfloat x, y, z, angle;

	glPushMatrix();

	/*
	* 根据指定的顶点两两成对画线，若是奇数个点则最后一个点无效
	*/
	glBegin( GL_LINES );

	z = 0.0f;
	for ( angle = 0.0f; angle <= GL_PI; angle += ( GL_PI/20.0f ) )
	{
		x = 50.0f * sin( angle );
		y = 50.0f * cos( angle );
		glVertex3f( x, y, z );

		x = 50.0f * sin( angle + GL_PI );
		y = 50.0f * cos( angle + GL_PI );
		glVertex3f( x, y, z );
	}

	glEnd();

	glPopMatrix();

	glutSwapBuffers();

}

void ChangeSize( int w, int h )
{
	GLfloat nRange = 100.0f;

	glViewport( 0, 0, w, h );

	if ( h == 0 )
	{
		h = 1;
	}

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	if ( w <= h )
	{
		glOrtho( -nRange, nRange, -nRange*h/w, nRange*h/w, nRange, -nRange );
	}
	else
	{
		glOrtho( -nRange*w/h, nRange*w/h, -nRange, nRange, nRange, -nRange );
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}


int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutCreateWindow( "LINES" );
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );

	SetupRC();

	glutMainLoop();
	return 0;
}

