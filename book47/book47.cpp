// book47.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include "GLTools.h"

#define NUM_SPHERES 50
GLTFrame spheres[NUM_SPHERES];
GLTFrame frameCamera;

void SetupRC()
{
	int iSphere;

	glClearColor( 0.0f, 0.0f, 0.5f, 1.0f );

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	gltInitFrame( &frameCamera );

	for ( iSphere = 0; iSphere < NUM_SPHERES; iSphere++ )
	{
		gltInitFrame( &spheres[iSphere] );

		//在-20到20之间挑选一个随机的位置，增量为0.1
		spheres[iSphere].vLocation[0] = (float)( (rand()%400) - 200 ) * 0.1f;
		spheres[iSphere].vLocation[1] = 0.0f;
		spheres[iSphere].vLocation[2] = (float)( (rand()%400) - 200 ) * 0.1f;
	}
}

void TimeFunction( int value )
{
	glutPostRedisplay();
}

//绘制网格状地面
void DrawGround()
{
	GLfloat fExtent = 20.0f;
	GLfloat fStep = 1.0f;
	GLfloat y = -0.4f;
	GLint iLine;

	glBegin( GL_LINES );
	for ( iLine = -fExtent; iLine <= fExtent; iLine += fStep )
	{
		glVertex3f( iLine, y, fExtent );
		glVertex3f( iLine, y, -fExtent );

		glVertex3f( fExtent, y, iLine );
		glVertex3f( -fExtent, y, iLine );
	}
	glEnd();
}

void RenderScene()
{
	int i;
	static GLfloat yRot = 0.0f;
	yRot += 0.5f;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
	gltApplyCameraTransform( &frameCamera );

	//绘制地面
	DrawGround();

	//绘制球体
	for ( i = 0; i < NUM_SPHERES; i++ )
	{
		glPushMatrix();
		gltApplyActorTransform( &spheres[i] );
		glutSolidSphere( 0.1f, 13, 26 );
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -2.5f );

	glPushMatrix();
	glRotatef( -yRot*2.0f, 0.0f, 1.0f, 0.0f );
	glTranslatef( 1.0f, 0.0f, 0.0f );
	glutSolidSphere( 0.1, 13, 26 );
	glPopMatrix();

	glRotatef( yRot, 0.0f, 1.0f, 0.0f );
	gltDrawTorus( 0.35, 0.15, 40, 20 );
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();

	glutTimerFunc( 33, TimeFunction, 0 );
}

void SpecialKeys( int key, int x, int y )
{
	if ( key == GLUT_KEY_UP )
	{
		gltMoveFrameForward( &frameCamera, 0.1f );
	}

	if ( key == GLUT_KEY_DOWN )
	{
		gltMoveFrameForward( &frameCamera, -0.1f );
	}

	if ( key == GLUT_KEY_LEFT )
	{
		gltRotateFrameLocalY( &frameCamera, 0.1 );
	}

	if ( key == GLUT_KEY_RIGHT )
	{
		gltRotateFrameLocalY( &frameCamera, -0.1 );
	}
	
	glutPostRedisplay();
}

void ChangeSize( int w, int h )
{
	GLfloat fAspect;

	if ( h == 0 )
	{
		h = 1;
	}

	glViewport( 0, 0, w, h );

	fAspect = (GLfloat)w/(GLfloat)h;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluPerspective( 35.0f, fAspect, 1.0f, 50.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutCreateWindow( "SPHEREWORLD" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );
	glutSpecialFunc( SpecialKeys );

	SetupRC();

	glutTimerFunc( 33, TimeFunction, 0 );

	glutMainLoop();

	return 0;
}

