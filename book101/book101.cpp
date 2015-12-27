
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void SetupRC()
{
	GLfloat whiteLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat lightPos[] = { -10.0f, 5.0f, 5.0f, 1.0f };
	
	glEnable( GL_DEPTH_TEST );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	
	glEnable( GL_LIGHTING );
	
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, whiteLight );
	glLightfv( GL_LIGHT0, GL_AMBIENT, sourceLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, sourceLight );
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
	glEnable( GL_LIGHT0 );
	
	glEnable( GL_COLOR_MATERIAL );
	
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	
	glClearColor( 0.25f, 0.25f, 0.5f, 1.0f );

}


void RenderScene()
{
	GLUquadricObj *pObj;
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	
	glPushMatrix();
		glTranslatef( 0.0f, -1.0f, -5.0f );
		glRotatef( xRot, 1.0f, 0.0f, 0.0f );
		glRotatef( yRot, 0.0f, 1.0f, 0.0f );
		
		pObj = gluNewQuadric();
		gluQuadricNormals( pObj, GLU_SMOOTH );
		
		glPushMatrix();
			//身体
			glColor3f( 1.0f, 1.0f, 1.0f );
			gluSphere( pObj, 0.4f, 26, 13 );
			
			glTranslatef( 0.0f, 0.55f, 0.0f );
			gluSphere( pObj, 0.3f, 26, 13 );
			
			glTranslatef( 0.0f, 0.45f, 0.0f );
			gluSphere( pObj, 0.24f, 26, 13 );
			
			//眼睛
			glColor3f( 0.0f, 0.0f, 0.0f );
			glTranslatef( 0.1f, 0.1f, 0.21f );
			gluSphere( pObj, 0.02f, 26, 13 );
			
			glTranslatef( -0.2f, 0.0f, 0.0f );
			gluSphere( pObj, 0.02f, 26, 13 );
			
			//鼻子
			glColor3f( 1.0f, 0.3f, 0.3f );
			glTranslatef( 0.1f, -0.12f, 0.0f );
			gluCylinder( pObj, 0.04f, 0.0f, 0.3f, 26, 13 );
			
		glPopMatrix();
		
		//帽子
		glPushMatrix();
			glColor3f( 0.0f, 0.0f, 0.0f );
			glTranslatef( 0.0f, 1.17f, 0.0f );
			glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
			gluCylinder( pObj, 0.17f, 0.17f, 0.4f, 26, 13 );
			
			//帽子边缘
			glDisable( GL_CULL_FACE );
			gluDisk( pObj, 0.17f, 0.28f, 26, 13 );
			glEnable( GL_CULL_FACE );
			
			glTranslatef( 0.0f, 0.0f, 0.4f );
			gluDisk( pObj, 0.0f, 0.17f, 26, 13 );
		glPopMatrix();
		
	glPopMatrix();
	
	glutSwapBuffers();
			
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
	
	gluPerspective( 35.0f, fAspect, 1.0f, 50.0f );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "SNOWMAN" );
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );
	glutSpecialFunc( SpecialKeys );
	
	SetupRC();

	glutMainLoop();
	
	return 0;
}
