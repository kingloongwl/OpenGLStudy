
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"
#include "GLEXT.H"

#include <math.h>

#define NUM_SPHERES 30

GLTFrame spheres[ NUM_SPHERES ];
GLTFrame frameCamera;

GLfloat fLightPos[4] = { -100.0f, 100.0f, 50.0f, 1.0f };
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

#define GROUND_TEXTURE	0
#define SPHERE_TEXTURE	1
#define WOOD_TEXTURE	2
#define CUBE_MAP		3
#define NUM_TEXUTRES	4

GLuint textureObjects[NUM_TEXUTRES];

const char *szTextureFiles[] = { "grass.tga", "orb.tga", "wood.tga" };
const char *szCubeFaces[6] = { "right.tga", "left.tga", "up.tga", "down.tga", "backward.tga", "forward.tga" };
GLenum cube[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

#ifdef WIN32
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
#endif

void SetupRC()
{
	int iSphere;
	int i;

#ifdef WIN32
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)gltGetExtensionPointer( "glActiveTexture" );
#endif
	
	glClearColor( fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3] );
	
	
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, fNoLight );
	glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );
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
		gltInitFrame( &spheres[iSphere] );
		spheres[ iSphere ].vLocation[0] = (float)((rand() % 400) - 200) * 0.1f;
		spheres[ iSphere ].vLocation[1] = 0.0f;
		spheres[ iSphere ].vLocation[2] = (float)((rand() % 400) - 200) * 0.1f;
	}
	
	glEnable( GL_TEXTURE_2D );
	glGenTextures( NUM_TEXUTRES, textureObjects );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	
	for ( i = 0; i < CUBE_MAP; i++ )
	{
		GLbyte *pBytes;
		GLint iWidth, iHeight, iComponents;
		GLenum eFormat;
		
		glBindTexture( GL_TEXTURE_2D, textureObjects[i] );
		
		glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
		pBytes = gltLoadTGA( szTextureFiles[i], &iWidth, &iHeight, &iComponents, &eFormat );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes );
		free( pBytes );
		
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}
	
	glActiveTexture( GL_TEXTURE1 );
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_TEXTURE_CUBE_MAP );
	glBindTexture( GL_TEXTURE_CUBE_MAP, textureObjects[CUBE_MAP] );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	
	glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP );
	glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP );
	glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP );
	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );
	glEnable( GL_TEXTURE_GEN_R );
	
	glBindTexture( GL_TEXTURE_CUBE_MAP, textureObjects[CUBE_MAP] );
	
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT );
	
	for ( i = 0; i < 6; i++ )
	{
		GLbyte *pBytes;
		GLint iWidth, iHeight, iComponents;
		GLenum eFormat;
		
		pBytes = gltLoadTGA( szCubeFaces[i], &iWidth, &iHeight, &iComponents, &eFormat );
		
		glTexImage2D( cube[i], 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes );
		free( pBytes );
	}
}

void ShutdownRC()
{
	glDeleteTextures( NUM_TEXUTRES, textureObjects );
}

void DrawGround()
{
	GLfloat fExtent = 20.0f;
	GLfloat fStep = 1.0f;
	GLfloat y = -0.4f;
	GLint iStrip, iRun;
	GLfloat s = 0.0f;
	GLfloat t = 0.0f;
	GLfloat texStep = 1.0f / ( fExtent * .075f );
	
	glBindTexture( GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE] );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	
	for ( iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep )
	{
		t = 0.0f;
		glBegin( GL_TRIANGLE_STRIP );
			for ( iRun = fExtent; iRun >= -fExtent; iRun -= fStep )
			{
				glTexCoord2f( s, t );
				glNormal3f( 0.0f, 1.0f, 0.0f );
				glVertex3f( iStrip, y, iRun );
				
				glTexCoord2f( s + texStep, t );
				glNormal3f( 0.0f, 1.0f, 0.0f );
				glVertex3f( iStrip + fStep, y, iRun );
				
				t += texStep;
			}
		glEnd();
		
		s += texStep;
	}
}

void DrawInhabitants()
{
	static GLfloat yRot = 0.0f;
	GLint i;
	
	yRot += 0.5f;
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	
	glBindTexture( GL_TEXTURE_2D, textureObjects[SPHERE_TEXTURE] );
	for ( i = 0; i < NUM_SPHERES; i++ )
	{
		glPushMatrix();
		gltApplyActorTransform( &spheres[i] );
		gltDrawSphere( 0.3f, 21, 11 );
		glPopMatrix();
	}
	
	glPushMatrix();
		glTranslatef( 0.0f, 0.1f, -2.5f );
		
		glMaterialfv( GL_FRONT, GL_SPECULAR, fBrightLight );
		glRotatef( yRot, 0.0f, 1.0f, 0.0f );
		
		glBindTexture( GL_TEXTURE_2D, textureObjects[WOOD_TEXTURE] );
		
		glActiveTexture( GL_TEXTURE1 );
		glEnable( GL_TEXTURE_CUBE_MAP );
		
		glActiveTexture( GL_TEXTURE0 );
		gltDrawTorus( 0.35, 0.15, 41, 17 );
		
		glActiveTexture( GL_TEXTURE1 );
		glDisable( GL_TEXTURE_CUBE_MAP );
		glActiveTexture( GL_TEXTURE0 );
		
		glMaterialfv( GL_FRONT, GL_SPECULAR, fNoLight );
	glPopMatrix();
}

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glPushMatrix();
	gltApplyCameraTransform( &frameCamera );
	
	glLightfv( GL_LIGHT0, GL_POSITION, fLightPos );
	
	glColor3f( 1.0f, 1.0f, 1.0f );
	DrawGround();
	
	DrawInhabitants();
	
	glPopMatrix();
	
	glutSwapBuffers();

	glutPostRedisplay();	//必须添加，否则time函数不启作用
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
	glutCreateWindow( "OpenGL SphereWorld Demo + Multitexture" );
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );
	glutSpecialFunc( SpecialKeys );
	
	SetupRC();
	
	glutTimerFunc( 33, TimerFunction, 1 );
	
	glutMainLoop();
	
	ShutdownRC();
	
	return 0;
}
