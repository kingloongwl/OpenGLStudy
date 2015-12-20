// book363.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include <math.h>

#define GL_PI 3.1415f

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void SetupRC()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glColor3f( 1.0f, 0.0f, 0.0f );
}

void RenderScene()
{
	GLfloat y;
	GLfloat fSizes[2];
	GLfloat fCurSize;

	glClear( GL_COLOR_BUFFER_BIT );
	glPushMatrix();

	glGetFloatv( GL_LINE_WIDTH_RANGE, fSizes );
	fCurSize = fSizes[0];

	for ( y = -90.0f; y < 90.0f; y += 20.0f )
	{
		glLineWidth( fCurSize );

		glBegin( GL_LINES );
		glVertex2f( -80.0f, y );
		glVertex2f( 80.0f, y );
		glEnd();

		fCurSize += 1.0f;
	}

	glPopMatrix();

	glutSwapBuffers();
}

void ChangeSize( int w, int h )
{
	GLfloat nRange = 100.0f;

	if ( h == 0 )
	{
		h = 1;
	}

	glViewport( 0, 0, w, h );

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
	glutInitDisplayMode( GL_DOUBLE | GLUT_RGB );
	glutCreateWindow( "LINESW" );
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );

	SetupRC();

	glutMainLoop();

	return 0;
}

