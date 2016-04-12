
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"

#define SUN		1
#define MERCURY	2
#define VENUS	3
#define	EARTH	4
#define MARS	5

void DrawSphere( float radius )
{
	GLUquadricObj *pObj;
	pObj = gluNewQuadric();
	gluQuadricNormals( pObj, GLU_SMOOTH );
	gluSphere( pObj, radius, 26, 13 );
	gluDeleteQuadric( pObj );
}

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode( GL_MODELVIEW );
	
	glPushMatrix();
	
	glTranslatef( 0.0f, 0.0f, -300.0f );
	
	glInitNames();
	glPushName( 0 );
	
	glColor3f( 1.0f, 1.0f, 0.0f );
	glLoadName( SUN );
	DrawSphere( 15.0f );
	
	glColor3f( 0.5f, 0.0f, 0.0f );
	glPushMatrix();
	glTranslatef( 24.0f, 0.0f, 0.0f );
	glLoadName( MERCURY );
	DrawSphere( 2.0f );
	glPopMatrix();
	
	glColor3f( 0.5f, 0.5f, 1.0f );
	glPushMatrix();
	glTranslatef( 60.0f, 0.0f, 0.0f );
    glLoadName( VENUS );
	DrawSphere( 4.0f );
	glPopMatrix();
	
	glColor3f( 0.0f, 0.0f, 1.0f );
	glPushMatrix();
	glTranslatef( 100.0f, 0.0f, 0.0f );
	glLoadName( EARTH );
	DrawSphere( 8.0f );
	glPopMatrix();
	
	glColor3f( 1.0f, 0.0f, 0.0f );
	glPushMatrix();
	glTranslatef( 150.0f, 0.0f, 0.0f );
	glLoadName( MARS );
	DrawSphere( 4.0f );
	glPopMatrix();
	
    glPopMatrix();
	
	glutSwapBuffers();
	
	
}

void ProcessPlanet( GLuint id )
{
	switch ( id )
	{
	case SUN:
		glutSetWindowTitle( "You clicked on the Sun!" );
		break;
	case MERCURY:
		glutSetWindowTitle( "You clicked on the Mercury!" );
		break;
	case VENUS:
		glutSetWindowTitle( "You clicked on the Venus!" );
		break;
	case EARTH:
		glutSetWindowTitle( "You clicked on the Earth!" );
		break;
	case MARS:
		glutSetWindowTitle( "You clicked on the Mars!" );
		break;
	default: 
		glutSetWindowTitle( "Nothing was clicked on!" );
		break;
	}
}

#define BUFFER_LENGTH 64
void ProcessSelection( int xPos, int yPos )
{
	GLfloat fAspect;
	
	static GLuint selectBuff[BUFFER_LENGTH];
	
	GLint hits, viewport[4];
	
	glSelectBuffer( BUFFER_LENGTH, selectBuff );
	
	glGetIntegerv( GL_VIEWPORT, viewport );
	
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	
	glRenderMode( GL_SELECT );
	
	glLoadIdentity();
	gluPickMatrix( xPos, viewport[3]-yPos, 2, 2, viewport );
	
	fAspect = (float)viewport[2] / (float)viewport[3];
	gluPerspective( 45.0f, fAspect, 1.0, 425.0 );
	
	RenderScene();
	
	hits = glRenderMode( GL_RENDER );
	
	if ( hits == 1 )
        ProcessPlanet( selectBuff[3] );
	
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	
	glMatrixMode( GL_MODELVIEW );
}

void MouseCallback( int button, int state, int x, int y )
{
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		ProcessSelection( x, y );
	}
}

void SetupRC()
{
	GLfloat dimLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat sourceLight[] = { 0.65f, 0.65f, 0.65f, 1.0f };
	GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CCW );
	glEnable( GL_CULL_FACE );
	
	glEnable( GL_LIGHTING );
	
	glLightfv( GL_LIGHT0, GL_AMBIENT, dimLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, sourceLight );
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
	glEnable( GL_LIGHT0 );
	
	glEnable( GL_COLOR_MATERIAL );
	
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	
	glClearColor( 0.60f, 0.60f, 0.60f, 1.0f );

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
	gluPerspective( 45.0f, fAspect, 1.0f, 425.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}



int main( int argc, char* argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "PLANTS" );
	glutReshapeFunc( ChangeSize );
	glutMouseFunc( MouseCallback );
	glutDisplayFunc( RenderScene );

	

	SetupRC();

	glutMainLoop();
	return 0;
}
