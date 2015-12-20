
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"
#include "GLEXT.H"

static GLfloat zPos = -60.0f;

#define TEXTURE_BRICK 0
#define TEXTURE_FLOOR 1
#define TEXTURE_CEILING 2
#define TEXTURE_COUNT 3
GLuint textures[TEXTURE_COUNT];
const char *szTextureFiles[TEXTURE_COUNT] = { "brick.tga", "floor.tga", "ceiling.tga" };

//为每个纹理对象个性纹理过滤器
void ProcessMenu( int value )
{
	GLint iLoop;
	
	for ( iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++ )
	{
		glBindTexture( GL_TEXTURE_2D, textures[iLoop] );
		
		switch( value )
		{
		case 0:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			break;
		case 1:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			break;
		case 2:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
			break;
		case 3:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
			break;
		case 4:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
			break;
		case 5:
		default:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			break;
		}
	}
	
	glutPostRedisplay();
}

void SetupRC()
{
	GLbyte *pBytes;
	GLint iWidth, iHeight, iComponents;
	GLenum eFormat;
	GLint iLoop;
	
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	
	glEnable( GL_TEXTURE_2D );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	
	//载入纹理
	glGenTextures( TEXTURE_COUNT, textures );
	for ( iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++ )
	{
		glBindTexture( GL_TEXTURE_2D, textures[iLoop] );
		
		pBytes = gltLoadTGA( szTextureFiles[iLoop], &iWidth, &iHeight, &iComponents, &eFormat );
		gluBuild2DMipmaps( GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes );
		
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		
		free( pBytes );
	}

	//开启各向异性过滤
	//if ( gltIsExtSupported( "GL_EXT_texture_filter_anisotropic" ) )
	//{
	//	GLfloat fLargest;
	//	glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest );

	//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest );
	//}
}

void ShutdownRC()
{
	glDeleteTextures( TEXTURE_COUNT, textures );
}

void SpecialKeys( int key, int x, int y )
{
	if ( key == GLUT_KEY_UP )
	{
		zPos += 1.0f;
	}
	
	if ( key == GLUT_KEY_DOWN )
	{
		zPos -= 1.0f;
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
	
	fAspect = (GLfloat)w / (GLfloat)h;
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	gluPerspective( 90.0f, fAspect, 1, 120 );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void RenderScene()
{
	GLfloat z;
	
	glClear( GL_COLOR_BUFFER_BIT );
	
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, zPos );
	
	for ( z = 60.0f; z >= 0.0f; z -= 10 )
	{
		//地面
		glBindTexture( GL_TEXTURE_2D, textures[TEXTURE_FLOOR] );
		glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -10.0f, -10.0f, z );
		
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 10.0f, -10.0f, z );
		
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 10.0f, -10.0f, z - 10.0f );
		
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -10.0f, -10.0f, z - 10.0f );
		glEnd();
		
		//天花板
		glBindTexture( GL_TEXTURE_2D, textures[TEXTURE_CEILING] );
		glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -10.0f, 10.0f, z - 10.0f );
		
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 10.0f, 10.0f, z - 10.0f );
		
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 10.0f, 10.0f, z );
		
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -10.0f, 10.0f, z );
		glEnd();
		
		//左墙
		glBindTexture( GL_TEXTURE_2D, textures[TEXTURE_BRICK] );
		glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -10.0f, -10.0f, z );
		
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( -10.0f, -10.0f, z - 10.0f );
		
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( -10.0f, 10.0f, z - 10.0f );
		
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -10.0f, 10.0f, z );
		glEnd();
		
		//右墙
		glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( 10.0f, 10.0f, z );
		
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 10.0f, 10.0f, z - 10.0f );
		
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 10.0f, -10.0f, z - 10.0f );
		
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( 10.0f, -10.0f, z );
		glEnd();
	}
	
	glPopMatrix();
	
	glutSwapBuffers();
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Tunnel" );
	glutReshapeFunc( ChangeSize );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );
	
	glutCreateMenu( ProcessMenu );
	glutAddMenuEntry( "GL_NEAREST", 0 );
	glutAddMenuEntry( "GL_LINEAR", 1 );
	glutAddMenuEntry( "GL_NEAREST_MIPMAP_NEAREST", 2 );
	glutAddMenuEntry( "GL_NEAREST_MIPMAP_LINEAR", 3 );
	glutAddMenuEntry( "GL_LINEAR_MIPMAP_NEAREST", 4 );
	glutAddMenuEntry( "GL_LINEAR_MIPMAP_LINEAR", 5 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );
	
	SetupRC();
	glutMainLoop();
	ShutdownRC();
	
	return 0;
}
