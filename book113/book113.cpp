
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"

#include <stdio.h>

#define NUM_SPHERES 30

GLTFrame spheres[ NUM_SPHERES ];
GLTFrame frameCamera;

GLfloat fLightPos[ 4 ] = { -100.0f, 100.0f, 50.0f, 1.0f };
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLTMatrix mShadowMatrix;

GLint groundList;
GLint iMethod = 0;

void DrawGround()
{
	GLfloat fExtent = 20.0f;
	GLfloat fStep = 1.0f;
	GLfloat y = -0.4f;
	GLint iStrip, iRun;

	for ( iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep )
	{
		glBegin( GL_TRIANGLE_STRIP );
		glNormal3f( 0.0f, 1.0f, 0.0f );

		for ( iRun = fExtent; iRun >= -fExtent; iRun -= fStep )
		{
			glVertex3f( iStrip, y, iRun );
			glVertex3f( iStrip + fStep, y, iRun );
		}
		glEnd();
	}
}

void DrawInhabitants( GLint nShadow )
{
	static GLfloat yRot = 0.0f;

	if ( nShadow == 0 )
	{
		yRot += 0.5f;
		glColor3f( 0.0f, 1.0f, 0.0f );
	}
	else
	{
		glColor3f( 0.0f, 0.0f, 0.0f );
	}

	for ( int i = 0; i < NUM_SPHERES; i++ )
	{
		glPushMatrix();
		gltApplyActorTransform( &spheres[ i ] );
		glutSolidSphere( 0.3f, 17, 9 );
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef( 0.0f, 0.1f, -2.5f );

	if ( nShadow == 0 )
	{
		glColor3f( 0.0f, 0.0f, 1.0f );
	}

	glPushMatrix();
	glRotatef( -yRot * 2.0f, 0.0f, 1.0f, 0.0f );
	glTranslatef( 1.0f, 0.0f, 0.0f );
	glutSolidSphere( 0.1f, 17, 9 );
	glPopMatrix();

	if ( nShadow == 0 )
	{
		glColor3f( 1.0f, 0.0f, 0.0f );
		glMaterialfv( GL_FRONT, GL_SPECULAR, fBrightLight );
	}

	glRotatef( yRot, 0.0f, 1.0f, 0.0f );
	gltDrawTorus( 0.35, 0.15, 61, 37 );
	glMaterialfv( GL_FRONT, GL_SPECULAR, fNoLight );

	glPopMatrix();
}

void ProcessMenu( int value )
{
	iMethod = value;
	glutPostRedisplay();
}

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

	gltMakeShadowMatrix( vPoints, fLightPos, mShadowMatrix );

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
	
	groundList = glGenLists( 1 );
	glNewList( groundList, GL_COMPILE );
	DrawGround();
	glEndList();
}

void RenderScene()
{
	static int iFrames = 0;
	static GLTStopwatch frameTimer;
	
	//第一次重置秒表
	if ( iFrames == 0 )
	{
		gltStopwatchReset( &frameTimer );
		iFrames++;
	}
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
	gltApplyCameraTransform( &frameCamera );

	glLightfv( GL_LIGHT0, GL_POSITION, fLightPos );

	glColor3f( 0.6f, 0.4f, 0.1f );
	
	if ( iMethod == 0 )
		DrawGround();
	else
		glCallList( groundList );

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glPushMatrix();
	glMultMatrixf( mShadowMatrix );
	DrawInhabitants( 1 );
	glPopMatrix();
	glEnable( GL_LIGHTING );
	glEnable( GL_DEPTH_TEST );

	DrawInhabitants( 0 );

	glPopMatrix();

	glutSwapBuffers();
	
	iFrames++;
	if ( iFrames == 101 )
	{
		float fps;
		char cBuffer[64];
		
		fps = 100.0f / gltStopwatchRead( &frameTimer );
		if ( iMethod == 0 )
			sprintf( cBuffer, "OpenGL SphereWorld without Display Lists %.1f fps", fps );
		else
			sprintf( cBuffer, "OpenGL SphereWorld with Display Lists %.1f fps", fps );
		
		glutSetWindowTitle( cBuffer );
		gltStopwatchReset( &frameTimer );
		iFrames = 1;
	}
	
	glutPostRedisplay();
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

void TimerFunction( int value )
{
	glutPostRedisplay();
	glutTimerFunc( 3, TimerFunction, 1 );
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
	
	glutCreateMenu( ProcessMenu );
	glutAddMenuEntry( "Without DisplayList", 0 );
	glutAddMenuEntry( "With DisplayList", 1 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	SetupRC();
	glutTimerFunc( 33, TimerFunction, 1 );

	glutMainLoop();
	return 0;
}
