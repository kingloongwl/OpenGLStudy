
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"

#include <math.h>

#define SMALL_STARS 100
GLTVector2 vSmallStars[ SMALL_STARS ];

#define MEDIUM_STARS 40
GLTVector2 vMediumStars[ MEDIUM_STARS ];

#define LARGE_STARS 15
GLTVector2 vLargeStars[ LARGE_STARS ];

#define SCREEN_X 800
#define SCREEN_Y 600

void ProcessMenu( int value )
{
	switch ( value )
	{
	case 1:	//防锯齿
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_BLEND );
		glEnable( GL_POINT_SMOOTH );
		glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
		glEnable( GL_LINE_SMOOTH );
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
		glEnable( GL_POLYGON_SMOOTH );
		glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
		break;
	case 2:
		glDisable( GL_BLEND );
		glDisable( GL_LINE_SMOOTH );
		glDisable( GL_POINT_SMOOTH );
		glDisable( GL_POLYGON_SMOOTH );
		break;
	default:
		break;
	}
	
	glutPostRedisplay();
}

void SetupRC()
{
	int i;
	
	for ( i = 0; i < SMALL_STARS; i++ )
	{
		vSmallStars[i][0] = (GLfloat)( rand() % SCREEN_X );
		vSmallStars[i][1] = (GLfloat)( rand() % ( SCREEN_Y - 100 ) ) + 100.0f;
	}
	
	for ( i = 0; i < MEDIUM_STARS; i++ )
	{
		vMediumStars[i][0] = (GLfloat)( rand() % SCREEN_X * 10 ) / 10.0f;
		vMediumStars[i][1] = (GLfloat)( rand() % ( SCREEN_Y - 100 ) ) + 100.0f;
	}
	
	for ( i = 0; i < LARGE_STARS; i++ )
	{
		vLargeStars[i][0] = (GLfloat)( rand() % SCREEN_X * 10 ) / 10.0f;
		vLargeStars[i][1] = (GLfloat)( rand() % ( SCREEN_Y - 100 ) * 10.0f ) / 10.0f + 100.0f;
	}
	
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	
	glColor3f( 0.0f, 0.0f, 0.0f );
	
}

void RenderScene()
{
	GLfloat x = 700.0f;
	GLfloat y = 500.0f;
	GLfloat r = 50.0f;
	GLfloat angle = 0.0f;
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glColor3f( 1.0f, 1.0f, 1.0f );
	
	//开启顶点数组
	glEnableClientState( GL_VERTEX_ARRAY );
	
	glPointSize( 1.0f );
	glVertexPointer( 2, GL_FLOAT, 0, vSmallStars );
	glDrawArrays( GL_POINTS, 0, SMALL_STARS );
	
	glPointSize( 3.05f );
	glVertexPointer( 2, GL_FLOAT, 0, vMediumStars );
	glDrawArrays( GL_POINTS, 0, MEDIUM_STARS );
	
	glPointSize( 5.5f );
	glVertexPointer( 2, GL_FLOAT, 0, vLargeStars );
	glDrawArrays( GL_POINTS, 0, LARGE_STARS );
	
	glBegin( GL_TRIANGLE_FAN );
		glVertex2f( x, y );
		for ( angle = 0; angle < 2.0f * 3.141592f; angle += 0.1f )
			glVertex2f( x + (float)cos(angle) * r, y + (float)sin(angle) * r );
		glVertex2f( x + r, y );
	glEnd();
	
	glLineWidth( 3.5 );
	glBegin( GL_LINE_STRIP );
	    glVertex2f(0.0f, 25.0f);
        glVertex2f(50.0f, 100.0f);
        glVertex2f(100.0f, 25.0f);
        glVertex2f(225.0f, 125.0f);
        glVertex2f(300.0f, 50.0f);
        glVertex2f(375.0f, 100.0f);
        glVertex2f(460.0f, 25.0f);
        glVertex2f(525.0f, 100.0f);
        glVertex2f(600.0f, 20.0f);
        glVertex2f(675.0f, 70.0f);
        glVertex2f(750.0f, 25.0f);
        glVertex2f(800.0f, 90.0f);    
    glEnd();
	
	glutSwapBuffers();
}

void ChangeSize( int w, int h )
{ 
	if ( h == 0 )
		h = 1;
	
	glViewport( 0, 0, w, h );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	gluOrtho2D( 0.0, SCREEN_X, 0.0, SCREEN_Y );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}



int main( int argc, char* argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "STARFIELD" );
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );

	
	glutCreateMenu( ProcessMenu );
	glutAddMenuEntry( "Antialiased Rendering", 1 );
	glutAddMenuEntry( "Normal Rendering", 2 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	SetupRC();

	glutMainLoop();
	return 0;
}
