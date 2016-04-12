
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"

static GLfloat corners[] = {
	-25.0f, 25.0, 25.0,
	25.0f, 25.0, 25.0,
	25.0f, -25.0, 25.0,
	-25.0f, -25.0, 25.0,
	-25.0f, 25.0, -25.0,
	25.0f, 25.0, -25.0,
	25.0f, -25.0, -25.0,
	-25.0f, -25.0, -25.0,
};

static GLubyte indexes[] = {
	0, 1, 2, 3,
	4, 5, 1, 0,
	5, 4, 7, 6,
	6, 7, 3, 2,
	4, 0, 3, 7,
	1, 5, 6, 2
};

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -200.0f );
	
	glRotatef( xRot, 1.0f, 0.0f, 0.0f );
	glRotatef( yRot, 0.0f, 0.0f, 1.0f );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, corners );
	
	glDrawElements( GL_QUADS, 24, GL_UNSIGNED_BYTE, indexes );
	
	glPopMatrix();
	
	glutSwapBuffers();
}

void SpecialKeys( int k, int x, int y )
{
	if ( k == GLUT_KEY_UP )
	{
		xRot -= 5.0f;
	}

	if ( k == GLUT_KEY_DOWN )
	{
		xRot += 5.0f;
	}

	if ( k == GLUT_KEY_LEFT )
	{
		yRot -= 5.0f;
	}

	if ( k == GLUT_KEY_RIGHT )
	{
		yRot += 5.0f;
	}

	if ( k > 356.0f )
	{
		xRot = 0.0f;
		yRot = 0.0f;
	}

	if ( k < -1.0f )
	{
		xRot = 355.0f;
		yRot = 355.0f;
	}

	glutPostRedisplay();
}

void SetupRC()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

}

void ChangeSize( int w, int h )
{ 
	GLfloat fAspect;

	if ( h == 0 )
		h = 1;

	glViewport( 0, 0, w, h );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	fAspect = (GLfloat)w / (GLfloat)h;
	gluPerspective( 35.0f, fAspect, 1.0f, 500.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -250.0f );
}



int main( int argc, char* argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "CUBEDX" );
	glutReshapeFunc( ChangeSize );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );

	

	SetupRC();

	glutMainLoop();
	return 0;
}
