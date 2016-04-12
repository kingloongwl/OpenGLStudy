
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

GLint headList, shaftList, threadList;

void RenderHead()
{
	float x, y, angle;
	float height = 25.0f;
	float diameter = 30.0f;
	GLTVector3 vNormal, vCorners[4];
	float step = ( 3.1415f / 3.0f );
	
	glColor3f( 0.0f, 0.0f, 0.7f );
	
	glFrontFace( GL_CCW );
	glBegin( GL_TRIANGLE_FAN );
		glNormal3f( 0.0f, 0.0f, 1.0f );
		
		glVertex3f( 0.0f, 0.0f, height/2.0f );
		
		glVertex3f( 0.0f, diameter, height/2.0f );
		
		for ( angle = (2.0f*3.1415f)-step; angle >= 0; angle -= step )
		{
			x = diameter * (float)sin(angle);
			y = diameter * (float)cos(angle);
			
			glVertex3f( x, y, height/2.0f );
		}
		
		glVertex3f( 0.0f, diameter, height/2.0f );
		
	glEnd();
	
	glBegin( GL_TRIANGLE_FAN );
		glNormal3f( 0.0f, 0.0f, -1.0f );
		
		glVertex3f( 0.0f, 0.0f, -height/2.0f );
		
		
		for ( angle = 0.0; angle < (2.0f*3.1415f); angle += step )
		{
			x = diameter * (float)sin(angle);
			y = diameter * (float)cos(angle);
			
			glVertex3f( x, y, -height/2.0f );
		}
		
		glVertex3f( 0.0f, diameter, -height/2.0 );
		
	glEnd();
	
	glBegin( GL_QUADS );
		for ( angle = 0.0f; angle < (2.0f*3.1415f); angle += step )
		{
			x = diameter * (float)sin( angle );
			y = diameter * (float)cos( angle );
			
			vCorners[0][0] = x;
			vCorners[0][1] = y;
			vCorners[0][2] = -height/2.0f;
			
			vCorners[1][0] = x;
			vCorners[1][1] = y;
			vCorners[1][2] = height/2.0f;
			
			x = diameter * (float)sin(angle + step);
			y = diameter * (float)cos(angle + step);
			
			if ( angle + step < 3.1415 * 2.0f )
			{
				vCorners[2][0] = x;
				vCorners[2][1] = y;
				vCorners[2][2] = height/2.0f;
				
				vCorners[3][0] = x;
				vCorners[3][1] = y;
				vCorners[3][2] = -height / 2.0f;
			}
			else
			{
				vCorners[2][0] = 0.0;
				vCorners[2][1] = diameter;
				vCorners[2][2] = height/2.0f;
				
				vCorners[3][0] = 0.0;
				vCorners[3][1] = diameter;
				vCorners[3][2] = -height / 2.0f;
			}
			
			gltGetNormalVector( vCorners[0], vCorners[1], vCorners[2], vNormal );
			glNormal3fv( vNormal );
			
			glVertex3fv( vCorners[0] );
			glVertex3fv( vCorners[1] );
			glVertex3fv( vCorners[2] );
			glVertex3fv( vCorners[3] );
		}
	glEnd();
}

void RenderShaft()
{
	float x, z, angle;
	float height = 75.0f;
	float diameter = 20.0f;
	GLTVector3 vNormal, vCorners[2];
	float step = (3.1415f/50.0f);
	
	glColor3f( 0.0f, 0.0f, 0.7f );
	
	glFrontFace( GL_CCW );
	glBegin( GL_QUAD_STRIP );
		for ( angle = (2.0f*3.1415f); angle > 0.0f; angle -= step )
		{
			x = diameter * (float)sin( angle );
			z = diameter * (float)cos( angle );
			
			vCorners[0][0] = x;
			vCorners[0][1] = -height/2.0f;
			vCorners[0][2] = z;
			
			vCorners[1][0] = x;
			vCorners[1][1] = height/2.0;
			vCorners[1][2] = z;
			
			vNormal[0] = vCorners[1][0];
			vNormal[1] = 0.0f;
			vNormal[2] = vCorners[1][2];
			
			gltNormalizeVector( vNormal );
			glNormal3fv( vNormal );
			glVertex3fv( vCorners[0] );
			glVertex3fv( vCorners[1] );
		}
		
		glVertex3f( diameter * (float)sin( 2.0f*3.1415f ), -height/2.0f, diameter*(float)cos(2.0f*3.1415f) );
		glVertex3f( diameter * (float)sin( 2.0f*3.1415f ), height/2.0f, diameter*(float)cos(2.0f*3.1415f) );
	glEnd();
	
	glBegin( GL_TRIANGLE_FAN );
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glVertex3f( 0.0f, -height/2.0f, 0.0f );
		for ( angle = (2.0f*3.1415f); angle > 0.0f; angle -= step )
		{
			x = diameter * (float)sin(angle);
			z = diameter * (float)cos(angle);
			
			glVertex3f( x, -height/2.0f, z );
		}
		
		glVertex3f( diameter * (float)sin(2.0f*3.1415f), -height/2.0f, diameter*(float)cos(2.0f*3.1415f) );
	glEnd();
}

void RenderThread()
{
	float x, y, z, angle;
	float height = 75.0f;
	float diameter = 20.0f;
	GLTVector3 vNormal, vCorners[4];
	float step = 3.1415f/32.0f;
	float revolutions = 7.0f;
	float threadWidth = 2.0f;
	float threadThick = 3.0f;
	float zstep = 0.125f;
	
	glColor3f( 0.0f, 0.0f, 0.4f );
	z = -height / 2.0f + 2.0f;
	
	for ( angle = 0.0f; angle < GLT_PI * 2.0f * revolutions; angle += step )
	{
		x = diameter * (float)sin( angle );
		y = diameter * (float)cos( angle );
		
		vCorners[0][0] = x;
		vCorners[0][1] = y;
		vCorners[0][2] = z;
		
		x = (diameter+threadWidth)*(float)sin( angle );
		y = (diameter+threadWidth)*(float)cos( angle );
		
		vCorners[1][0] = x;
		vCorners[1][1] = y;
		vCorners[1][2] = z;
		
		x = (diameter+threadWidth)*(float)sin( angle+step );
		y = (diameter+threadWidth)*(float)cos( angle+step );
		
		vCorners[2][0] = x;
		vCorners[2][1] = y;
		vCorners[2][2] = z + zstep;
		
		x = diameter*(float)sin( angle+step );
		y = diameter*(float)cos( angle+step );
		
		vCorners[3][0] = x;
		vCorners[3][1] = y;
		vCorners[3][2] = z + zstep;
		
		glFrontFace( GL_CCW );
		glBegin( GL_TRIANGLES );
			gltGetNormalVector( vCorners[0], vCorners[1], vCorners[2], vNormal );
			glNormal3fv( vNormal );
			
			glVertex3fv( vCorners[0] );
			glVertex3fv( vCorners[1] );
			glVertex3fv( vCorners[2] );
			
			glVertex3fv( vCorners[2] );
			glVertex3fv( vCorners[3] );
			glVertex3fv( vCorners[0] );
		glEnd();
			
		vCorners[0][2] += threadThick;
		vCorners[3][2] += threadThick;
			
		gltGetNormalVector( vCorners[0], vCorners[1], vCorners[2], vNormal );
		vNormal[0] = -vNormal[0];
		vNormal[1] = -vNormal[1];
		vNormal[2] = -vNormal[2];
		
		glFrontFace( GL_CW );
		
		glBegin( GL_TRIANGLES );
			glNormal3fv( vNormal );
			
			glVertex3fv( vCorners[0] );
			glVertex3fv( vCorners[1] );
			glVertex3fv( vCorners[2] );
			
			glVertex3fv( vCorners[2] );
			glVertex3fv( vCorners[3] );
			glVertex3fv( vCorners[0] );
		glEnd();
		
		z += zstep;
	}
}



void SetupRC()
{
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	
	headList = glGenLists( 3 );
	shaftList = headList + 1;
	threadList = headList + 2;

	glNewList( headList, GL_COMPILE );
	RenderHead();
	glEndList();

	glNewList( shaftList, GL_COMPILE );
	RenderShaft();
	glEndList();

	glNewList( threadList, GL_COMPILE );
	RenderThread();
	glEndList();
}

void SpecialKeys( int key, int x, int y )
{
	if ( key == GLUT_KEY_UP )
	{
		xRot -= 5.0f;
	}
	
	if ( key == GLUT_KEY_DOWN )
	{
		xRot += 5.0f;
	}
	
	if ( key == GLUT_KEY_LEFT )
	{
		yRot -= 5.0f;
	}
	
	if ( key == GLUT_KEY_RIGHT )
	{
		yRot += 5.0f;
	}
	
	xRot = (GLfloat)((const int)xRot % 360);
	yRot = (GLfloat)((const int)yRot % 360);
	glutPostRedisplay();
}


void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	
	glRotatef( xRot, 1.0f, 0.0f, 0.0f );
	glRotatef( yRot, 0.0f, 0.0f, 1.0f );
	
//	RenderShaft();
	glCallList( shaftList );
	
	glPushMatrix();
	glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
//	RenderThread();
	glCallList( threadList );
	
	glTranslatef( 0.0f, 0.0f, 45.0f );
//	RenderHead();
	glCallList( headList );
	
	glPopMatrix();
	glPopMatrix();
	
	glutSwapBuffers();
}

void ChangeSize( int w, int h )
{
	GLfloat fAspect;
	
	if ( h == 0 )
	{
		h = 1;
	}
	
	glViewport( 0, 0, w, h );
	
	fAspect = (GLfloat)w / (GLfloat)h;
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	gluPerspective( 35.0f, fAspect, 1.0f, 500.0f );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glTranslatef( 0.0f, 0.0f, -300.0f );
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "BOLT" );
	
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );
	glutSpecialFunc( SpecialKeys );
	
	SetupRC();

	glutMainLoop();
	
	return 0;
}
