// book55.cpp : Defines the entry point for the console application.
//


#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

static GLfloat xRot = 0.0;
static GLfloat yRot = 0.0;


void SetupRC()
{
	glClearColor( 0.0, 0.0, 1.0, 1.0 );

	glEnable( GL_DEPTH_TEST );

	GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //5.7节AMBIENT

	glEnable( GL_LIGHTING );
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambientLight );

	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

}

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	GLfloat vertex1[] = { 0.0f, 0.0f, 0.0f };
	GLfloat vertex2[] = { 0.0, 10.0, -15.0 };
	GLfloat vertex3[] = { 5.0, 0.0, -15.0 };
	GLfloat vertex4[] = { -5.0, 0.0, -15.0 };

	GLfloat vertex5[] = { 0.0, 1.0, -20.0 };
	GLfloat vertex6[] = { 0.0, 5.0, -25.0 };
	GLfloat vertex7[] = { 15.0, 1.0, -25.0 };
	GLfloat vertex8[] = { -15.0, 1.0, -25.0 };

	GLfloat vertex9[] = { 0.0, 0.0, -40.0 };

	GLfloat vertex10[] = { 0.0, 0.0, -35.0 };
	GLfloat vertex11[] = { 0.0, 5.0, -50.0 };
	GLfloat vertex12[] = { 10.0, 0.0, -50.0 };
	GLfloat vertex13[] = { -10.0, 0.0, -50.0 };

	GLfloat vertex14[] = { 0.0, 20.0, -55.0 };
	GLfloat vertex15[] = { 2, 0.0, -50.0 };
	GLfloat vertex16[] = { -2, 0.0, -50.0 };

	glPushMatrix();
	glRotatef( xRot, 1.0, 0.0, 0.0 );
	glRotatef( yRot, 0.0, 1.0, 0.0 );

	glBegin( GL_TRIANGLES );

	//机头
	glColor3ub( 255, 0, 0 );
	glVertex3fv( vertex1 );
	glVertex3fv( vertex2 );
	glVertex3fv( vertex3 );

	glColor3ub( 0, 255, 0 );
	glVertex3fv( vertex1 );
	glVertex3fv( vertex2 );
	glVertex3fv( vertex4 );

	glColor3ub( 100, 100, 100 );
	glVertex3fv( vertex1 );
	glVertex3fv( vertex3 );
	glVertex3fv( vertex4 );

	//机身
	glColor3ub( 0, 255, 100  );
	glVertex3fv( vertex2 );
	glVertex3fv( vertex3 );
	glVertex3fv( vertex9 );

	glColor3ub( 255, 255, 0 );
	glVertex3fv( vertex2 );
	glVertex3fv( vertex4 );
	glVertex3fv( vertex9 );

	glColor3ub( 255, 255, 255 );
	glVertex3fv( vertex3 );
	glVertex3fv( vertex4 );
	glVertex3fv( vertex9 );

	//机翅
	glColor3ub( 255, 0, 255 );
	glVertex3fv( vertex5 );
	glVertex3fv( vertex6 );
	glVertex3fv( vertex7 );

	glVertex3fv( vertex5 );
	glVertex3fv( vertex6 );
	glVertex3fv( vertex8 );

	glColor3ub( 0, 0, 0 );
	glVertex3fv( vertex6 );
	glVertex3fv( vertex7 );
	glVertex3fv( vertex8 );

	glColor3ub( 255, 255, 100 );
	glVertex3fv( vertex5 );
	glVertex3fv( vertex7 );
	glVertex3fv( vertex8 );

	//机尾下
	glColor3ub( 0, 255, 255 );
	glVertex3fv( vertex10 );
	glVertex3fv( vertex11 );
	glVertex3fv( vertex12 );

	glVertex3fv( vertex10 );
	glVertex3fv( vertex11 );
	glVertex3fv( vertex13 );

	glColor3ub( 0, 0, 0 );
	glVertex3fv( vertex11 );
	glVertex3fv( vertex12 );
	glVertex3fv( vertex13 );

	glColor3ub( 100, 100, 100 );
	glVertex3fv( vertex10 );
	glVertex3fv( vertex12 );
	glVertex3fv( vertex13 );

	//机尾上
	glColor3ub( 255, 0, 0 );
	glVertex3fv( vertex10 );
	glVertex3fv( vertex14 );
	glVertex3fv( vertex15 );

	glColor3ub( 0, 255, 0 );
	glVertex3fv( vertex10 );
	glVertex3fv( vertex14 );
	glVertex3fv( vertex16 );

	glColor3ub( 0, 0, 0 );
	glVertex3fv( vertex14 );
	glVertex3fv( vertex15 );
	glVertex3fv( vertex16 );

	glEnd();
	glPopMatrix();

	glutSwapBuffers();

}

void SpecialKeys( int key, int x, int y )
{
	if ( key == GLUT_KEY_DOWN )
	{
		xRot -= 2.f;
	}
	else if ( key == GLUT_KEY_UP )
	{
		xRot += 2.f;
	}
	else if ( key == GLUT_KEY_LEFT )
	{
		yRot -= 2.f;
	}
	else if ( key == GLUT_KEY_RIGHT )
	{
		yRot += 2.f;
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

	if ( w > h )
	{
		glOrtho( -nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange );
	}
	else
	{
		glOrtho( -nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange );
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

}


int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutCreateWindow( "Jet" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );
	glutSpecialFunc( SpecialKeys );

	SetupRC();

	glutMainLoop();

	return 0;
}

