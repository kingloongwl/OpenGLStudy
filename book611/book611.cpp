#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"

#define NUM_SPHERES 30

GLTFrame spheres[ NUM_SPHERES ];
GLTFrame frameCamera;

GLfloat fLightPos[ 4 ] = { -100.0f, 100.0f, 50.0f, 1.0f };
GLfloat fLightPosMirror[ 4 ] = { -100.0f, -100.0f, 50.0f, 1.0f };
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void SetupRC()
{
	int iSphere;

	GLTVector3 vPoints[ 3 ] = 
	{ 
		{ 0.0f, -0.4f, 0.0f },		
		{ 10.0f, -0.4f, 0.0f },
		{ 5.0f, -0.4f, -5.0f }
	};

	glClearColor( fLowLight[ 0 ], fLowLight[ 1 ], fLowLight[ 3 ], fLowLight[ 4 ] );

	/* book62 */
	glEnable( GL_FOG );
	glFogfv( GL_FOG_COLOR, fLowLight );
	glFogf( GL_FOG_START, 0.5f );
	glFogf( GL_FOG_END, 10.0f );
	glFogi( GL_FOG_MODE, GL_LINEAR );
//	glFogf( GL_FOG_DENSITY, 0.5f );

	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, fNoLight );
	glLightfv( GL_LIGHT0, GL_AMBIENT, fLowLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, fBrightLight );
	glLightfv( GL_LIGHT0, GL_SPECULAR, fBrightLight );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glMateriali( GL_FRONT, GL_SHININESS, 128 );


	gltInitFrame( &frameCamera );

	for ( iSphere = 0; iSphere < NUM_SPHERES; iSphere++ )
	{
		gltInitFrame( &spheres[ iSphere ] );

		spheres[ iSphere ].vLocation[ 0 ] = ( float )( rand() % 400 - 200 ) * 0.1f;
		spheres[ iSphere ].vLocation[ 1 ] = 0.0f;
		spheres[ iSphere ].vLocation[ 2 ] = ( float )( rand() % 400 - 200 ) * 0.1f;
	}
}

void TimerFunction( int value )
{
	glutPostRedisplay();
	glutTimerFunc( 3, TimerFunction, 1 );
}

void DrawGround(void)
{
	GLfloat fExtent = 20.0f;
	GLfloat fStep = 0.5f;
	GLfloat y = 0.0f;
	GLfloat fColor;
	GLfloat iStrip, iRun;
	GLint iBounce = 0;

	glShadeModel(GL_FLAT);
	for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
		{
			if((iBounce %2) == 0)
				fColor = 1.0f;
			else
				fColor = 0.0f;

			glColor4f(fColor, fColor, fColor, 0.5f);
			glVertex3f(iStrip, y, iRun);
			glVertex3f(iStrip + fStep, y, iRun);

			iBounce++;
		}
		glEnd();
	}
	glShadeModel(GL_SMOOTH);
}

void DrawInhabitants()
{
	int i;
	static GLfloat yRot = 0.0f;
	yRot += 0.5f;

	glColor3f( 1.0f, 0.0f, 0.0f );

	glPushMatrix();
	gltApplyCameraTransform( &frameCamera );

	glTranslatef( 0.0f, 0.5f, -3.5f );

	//ªÊ÷∆«ÚÃÂ
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

	glutTimerFunc( 33, TimerFunction, 0 );
}

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
	glLightfv( GL_LIGHT0, GL_POSITION, fLightPosMirror );

	glPushMatrix();
	glFrontFace( GL_CW );
	glScalef( 1.0f, -1.0f, 1.0f );
	DrawInhabitants();
	glFrontFace( GL_CCW );
	glPopMatrix();

	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	DrawGround();
	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );

	glLightfv( GL_LIGHT0, GL_POSITION, fLightPos );
	DrawInhabitants();
	glPopMatrix();

	glutSwapBuffers();


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
		gltRotateFrameLocalY( &frameCamera, 0.1f );
	}

	if ( key == GLUT_KEY_RIGHT )
	{
		gltRotateFrameLocalY( &frameCamera, -0.1f );
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

	fAspect = ( GLfloat )w / ( GLfloat )h;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluPerspective( 35.0f, fAspect, 1.0f, 50.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	glTranslatef( 0.0f, -0.4f, 0.0f );
}

int main( int argc, char* argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "OpenGL Sphere World Demo + Lights and Shadow" );
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );
	glutSpecialFunc( SpecialKeys );

	SetupRC();
	glutTimerFunc( 33, TimerFunction, 1 );

	glutMainLoop();
	return 0;
}
