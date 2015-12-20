
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"
#include "GLEXT.H"


static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

GLuint toTextures[2];
int iRenderMode = 3;

void ProcessMenu( int value )
{
	GLfloat zPlane[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	
	iRenderMode = value;
	
	switch( value )
	{
	case 1:
		//物体线性
		glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
		glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
		glTexGenfv( GL_S, GL_OBJECT_PLANE, zPlane );
		glTexGenfv( GL_T, GL_OBJECT_PLANE, zPlane );
		break;
	case 2:
		//视觉线性
		glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
		glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
		glTexGenfv( GL_S, GL_EYE_PLANE, zPlane );
		glTexGenfv( GL_T, GL_EYE_PLANE, zPlane );
		break;
	case 3:
	default:
		//球体贴图
		glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
		glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
		break;
	}
	
	glutPostRedisplay();
}

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0.0f, 1.0f, 0.0f, 1.0f );
	
	glMatrixMode( GL_MODELVIEW );
	glBindTexture( GL_TEXTURE_2D, toTextures[1] );
	
	glDisable( GL_TEXTURE_GEN_S );
	glDisable( GL_TEXTURE_GEN_T );
	
	//背景绘图无需写入深度缓冲区
	glDepthMask( GL_FALSE );
	
	//背景图像
	glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f( 0.0f, 0.0f );
		
		glTexCoord2f( 1.0f, 0.0f );
		glVertex2f( 1.0f, 0.0f );
		
		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f( 1.0f, 1.0f );
		
		glTexCoord2f( 0.0f, 1.0f );
		glVertex2f( 0.0f, 1.0f );
	glEnd();
	
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	
	//打开纹理生成和深度缓冲区
	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );
	glDepthMask( GL_TRUE );
	
	if ( iRenderMode != 3 )
	{
		glBindTexture( GL_TEXTURE_2D, toTextures[0] );
	}
	
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -2.0f );
	glRotatef( xRot, 1.0f, 0.0f, 0.0f );
	glRotatef( yRot, 0.0f, 1.0f, 0.0f );
	
	gltDrawTorus( 0.35, 0.15, 61, 37 );
	
	glPopMatrix();
	glutSwapBuffers();
}

void SetupRC()
{
	GLbyte *pBytes;
	GLint iComponents, iWidth, iHeight;
	GLenum eFormat;
	
	glEnable( GL_DEPTH_TEST );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	
	glGenTextures( 2, toTextures );
	
	//载入主纹理
	glBindTexture( GL_TEXTURE_2D, toTextures[0] );
	pBytes = gltLoadTGA( "stripes.tga", &iWidth, &iHeight, &iComponents, &eFormat );
	glTexImage2D( GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, (void*)pBytes );
	free( pBytes );
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glEnable( GL_TEXTURE_2D );
	
	//载入这个纹理贴图
	glBindTexture( GL_TEXTURE_2D, toTextures[1] );
	pBytes = gltLoadTGA( "Environment.tga", &iWidth, &iHeight, &iComponents, &eFormat );
	glTexImage2D( GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, (void*)pBytes );
	free( pBytes );
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glEnable( GL_TEXTURE_2D );
	
	
	
	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );
	
	glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
	glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
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
	
	if ( key > 356.0f )
	{
		xRot = 0.0f;
		yRot = 0.0f;
	}
	
	if ( key < -1.0f )
	{
		xRot = 355.0f;
		yRot = 355.0f;
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
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	fAspect = (GLfloat)w / (GLfloat)h;
	gluPerspective( 45.0f, fAspect, 1.0f, 225.0f );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Texture Coordinate Generation" );
	glutReshapeFunc( ChangeSize );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );
	SetupRC();
	
	glutCreateMenu( ProcessMenu );
	glutAddMenuEntry( "Object Linear", 1 );
	glutAddMenuEntry( "Eye Linear", 2 );
	glutAddMenuEntry( "Sphere Map", 3 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );
	
	glutMainLoop();
	
	glDeleteTextures( 2, toTextures );
	
	return 0;
}
