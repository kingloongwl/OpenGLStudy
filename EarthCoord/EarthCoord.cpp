// EarthCoord.cpp : Defines the entry point for the console application.
//


#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include <math.h>

#define GL_PI 3.1415

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

static GLfloat MaxRadius = 80.0f; 

static GLfloat CenterX = 0.0f;
static GLfloat CenterY = 0.0f;
static GLfloat CenterZ = 0.0f;

static GLfloat step = GL_PI/10;


/* 画纬度，angle范围从-GL_PI/2到GL_PI/2 */
void drawParallel( GLfloat angle )
{
	if ( fabs( angle ) == GL_PI/2 )
	{
		glBegin( GL_POINTS );
		glVertex3f( CenterX, CenterY + MaxRadius*sin(angle), CenterZ );
		glEnd();
	}
	else
	{
		GLfloat circleRadius = MaxRadius * cos( angle );

		GLfloat circleCenterX = CenterX;
		GLfloat circleCenterY = CenterY + MaxRadius * sin( angle );
		GLfloat circleCenterZ = CenterZ;

		glBegin( GL_LINE_STRIP );
		for ( GLfloat circleAngle = 0.0; circleAngle <= 2.0f * GL_PI; circleAngle += GL_PI/20 )
		{
			glVertex3f( circleCenterX + circleRadius*cos( circleAngle ),
				circleCenterY, circleCenterZ + circleRadius*sin( circleAngle ) );
		}

		glEnd();
	}
}

/* 画经度，angle范围从0到2*GL_PI */
void drawLongitude( GLfloat angle )
{
	glBegin( GL_LINE_STRIP );
	for ( GLfloat circleAngle = 0.0; circleAngle <= 2.0f * GL_PI; circleAngle += GL_PI/20 )
	{
		glVertex3f( CenterX + MaxRadius*cos( circleAngle )*cos( angle ),
			CenterY + MaxRadius*sin( circleAngle ) ,
			CenterZ + MaxRadius*cos( circleAngle )*sin( angle ) );
	}
	glEnd();
}

void SetupRC()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glColor3f( 1.0f, 0.0f, 0.0f );
}

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT );

	glPushMatrix();
	glRotatef( xRot, 1.0f, 0.0f, 0.0f );
	glRotatef( yRot, 0.0f, 1.0f, 0.0f );

	for ( GLfloat angle = -GL_PI/2; angle <= GL_PI/2; angle += step )
	{
		drawParallel( angle );
	}

	for ( GLfloat angle = 0.0f; angle <= 2*GL_PI; angle += step )
	{
		drawLongitude( angle );
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
		glOrtho( -nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange );
	}
	else
	{
		glOrtho( -nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange );
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

}

void SpecialKeys( int key, int x, int y )
{
	if ( key == GLUT_KEY_DOWN )
	{
		xRot -= 0.5f;
	}
	else if ( key == GLUT_KEY_UP )
	{
		xRot += 0.5f;
	}
	else if ( key == GLUT_KEY_LEFT )
	{
		yRot -= 0.5f;
	}
	else if ( key == GLUT_KEY_RIGHT )
	{
		yRot += 0.5f;
	}
	else if ( key > 356.0f )
	{
		xRot = 0.0f;
		yRot = 0.0f;
	}
	else if ( key < -1.0f )
	{
		xRot = 355.0f;
		yRot = 355.0f;
	}

	glutPostRedisplay();

}


int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutCreateWindow( "EarthCoord" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );
	glutSpecialFunc( SpecialKeys );

	SetupRC();

	glutMainLoop();

	return 0;
}

