
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <math.h>

#include "GLTools.h"

struct Rectangle
{
	int top;
	int bottom;
	int left;
	int right;
};

#define TORUS 	1
#define SPHERE	20

struct Rectangle boundingRect;
GLuint selectedObject = 0;
float fAspect;

#ifndef M_PI
#define M_PI 3.14159265f
#endif

void DrawTorus( int numMajor, int numMinor )
{
	float majorRadius = 0.35f;
	float minorRadius = 0.15f;
	double majorStep = 2.0f * M_PI / numMajor;
	double minorStep = 2.0f * M_PI / numMinor;
	
	int i, j;
	float c, r, z;
	
	glEnable( GL_NORMALIZE );
	for ( i = 0; i < numMajor; ++i )
	{
		double a0 = i * majorStep;
		double a1 = a0 + majorStep;
		GLfloat x0 = (GLfloat)cos( a0 );
		GLfloat y0 = (GLfloat)sin( a0 );
		GLfloat x1 = (GLfloat)cos( a1 );
		GLfloat y1 = (GLfloat)sin( a1 );
		
		glBegin( GL_TRIANGLE_STRIP );
		for ( j = 0; j <= numMinor; ++j )
		{
			double b = j * minorStep;
			c = (float) cos( b );
			r = minorRadius * c + majorRadius;
			z = minorRadius * (GLfloat) sin( b );
			
			glTexCoord2f( (float)i/(float)numMajor, (float)j/(float)numMinor );
			glNormal3f( x0 * c, y0 * c, z/ minorRadius );
			glVertex3f( x0 * r, y0 * r, z );
			
			glTexCoord2f( (float)(i+1)/(float)numMajor, (float)j/(float)numMinor );
			glNormal3f( x1 * c, y1 * c, z/ minorRadius );
			glVertex3f( x1 * r, y1 * r, z );
		}
		
		glEnd();
	}
	
	glDisable( GL_NORMALIZE );
}


void DrawSphere( float radius )
{
	GLUquadricObj *pObj;
	pObj = gluNewQuadric();
	gluQuadricNormals( pObj, GLU_SMOOTH );
	gluSphere( pObj, radius, 26, 13 );
	gluDeleteQuadric( pObj );
}

void DrawObjects()
{
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	
	glTranslatef( -0.75f, 0.0f, -2.5f );
	
	glInitNames();
    glPushName(0);
	
	glColor3f( 1.0f, 1.0f, 1.0f );
	glLoadName( TORUS );
	glPassThrough( (GLfloat)TORUS );
    DrawTorus( 40, 20 );
	
	glColor3f( 0.5f, 0.0f, 0.0f );
	glTranslatef( 1.5f, 0.0f, 0.0f );
	glLoadName( SPHERE );
	glPassThrough( (GLfloat)SPHERE );
	DrawSphere( 0.5f );
	
	glPopMatrix();
}

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	DrawObjects();
	
	if ( selectedObject != 0 )
	{
		int viewport[4];
		
		glGetIntegerv( GL_VIEWPORT, viewport );
		
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
		
		glOrtho( viewport[0], viewport[2], viewport[3], viewport[1], -1, 1 );
		glMatrixMode( GL_MODELVIEW );
		
		glDisable( GL_LIGHTING );
		glColor3f( 1.0f, 0.0f, 0.0f );
		glBegin( GL_LINE_LOOP );
			glVertex2i( boundingRect.left, boundingRect.top );
			glVertex2i( boundingRect.left, boundingRect.bottom );
			glVertex2i( boundingRect.right, boundingRect.bottom );
			glVertex2i( boundingRect.right, boundingRect.top );
		glEnd();
		glEnable( GL_LIGHTING );
	}
	
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	
	glutSwapBuffers();
	
	
}

#define FEED_BUFF_SIZE 32768
void MakeSelection( int nChoice )
{
	static GLfloat feedBackBuff[FEED_BUFF_SIZE];
	
	int size, i, j, count;
	
	boundingRect.right = boundingRect.bottom = -999999.0f;
	boundingRect.left = boundingRect.top = 999999.0f;
	
	glFeedbackBuffer( FEED_BUFF_SIZE, GL_2D, feedBackBuff );
	
    glRenderMode( GL_FEEDBACK );
	
	DrawObjects();
	
	size = glRenderMode( GL_RENDER );
	
	i = 0;
	while( i < size )
	{
		if ( feedBackBuff[i] == GL_PASS_THROUGH_TOKEN )
		{
			if ( feedBackBuff[ i + 1 ] == (GLfloat)nChoice )
			{
				i += 2;
				while( i < size && feedBackBuff[i] != GL_PASS_THROUGH_TOKEN )
				{
					if ( feedBackBuff[i] == GL_POLYGON_TOKEN )
					{
						count = (int)feedBackBuff[++i];
						i++;
						
						for ( j = 0; j < count; j ++ )
						{
							if ( feedBackBuff[i] > boundingRect.right )
								boundingRect.right = feedBackBuff[i];
							if ( feedBackBuff[i] < boundingRect.left )
								boundingRect.left = feedBackBuff[i];
							i++;
							
							if ( feedBackBuff[i] > boundingRect.bottom )
								boundingRect.bottom = feedBackBuff[i];
							if ( feedBackBuff[i] < boundingRect.top )
                                boundingRect.top = feedBackBuff[i];
							i++;
						}
					}
					else
					{
						i++;
					}
				}
				break;
			}
		}
		
		i++;
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
	gluPerspective( 60.0f, fAspect, 1.0, 425.0 );
	
	DrawObjects();
	
	hits = glRenderMode( GL_RENDER );
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	
	if ( hits == 1 )
	{
		MakeSelection(selectBuff[3]);
		if ( selectedObject == selectBuff[3] )
			selectedObject = 0;
		else
            selectedObject = selectBuff[3];
	}

    glutPostRedisplay();
}

void SetupRC()
{
	GLfloat dimLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat sourceLight[] = { 0.65f, 0.65f, 0.65f, 1.0f };
	GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CCW );
	
	glEnable( GL_LIGHTING );
	
	glLightfv( GL_LIGHT0, GL_AMBIENT, dimLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, sourceLight );
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
	glEnable( GL_LIGHT0 );
	
	glEnable( GL_COLOR_MATERIAL );
	
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	
	glClearColor( 0.60f, 0.60f, 0.60f, 1.0f );

	glLineWidth( 2.0f );
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

void MouseCallback( int button, int state, int x, int y )
{
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
		ProcessSelection( x, y );
}


int main( int argc, char* argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Select an Object" );
	glutReshapeFunc( ChangeSize );
	glutMouseFunc( MouseCallback );
	glutDisplayFunc( RenderScene );

	

	SetupRC();

	glutMainLoop();
	return 0;
}
