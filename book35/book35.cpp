// book35.cpp : Defines the entry point for the console application.
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
	GLfloat x,y,z,angle;
	GLfloat sizes[2];
	GLfloat step;
	GLfloat curSize;

	glClear( GL_COLOR_BUFFER_BIT );

	glPushMatrix();
	glRotatef( xRot, 1.0f, 0.0f, 0.0f );
	glRotatef( yRot, 1.0f, 0.0f, 0.0f );

	glGetFloatv( GL_POINT_SIZE_RANGE, sizes );
	glGetFloatv( GL_POINT_SIZE_GRANULARITY, &step );
	

	curSize = sizes[0];
	z = -50.0f;
	for ( angle = 0.0f; angle <= (2.0f*GL_PI)*3.0f; angle += 0.1f )
	{
		x = 50.0f * sin(angle);
		y = 50.0f * cos(angle);

		glPointSize( curSize ); //必须在glBegin与glEnd之外调用，否则该函数无效

		glBegin( GL_POINTS );
			glVertex3f(x,y,z);
		glEnd();

		z += 0.5f;
		curSize += step;
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

void SpecialKeys( int key, int x, int y )
{
	if ( key == GLUT_KEY_UP )
	{
		xRot -= 0.5f;
	}
	else if ( key == GLUT_KEY_DOWN )
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
	glutInitDisplayMode( GL_DOUBLE | GLUT_RGB );
	glutCreateWindow( "Pointz" );
	glutReshapeFunc( ChangeSize );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );

	SetupRC();

	glutMainLoop();

	return 0;
}

