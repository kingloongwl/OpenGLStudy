
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"
#include "GLEXT.H"

//旋转的数量
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void ChangeSize( int w, int h )
{
	GLfloat fAspect;

	if ( h == 0 )
	{
		h = 1;
	}
	
	glViewport( 0, 0, w, h );
	
	fAspect = ( GLfloat )w / ( GLfloat )h;
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	gluPerspective( 35.0f, fAspect, 1.0, 40.0 );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void SetupRC()
{
	GLbyte *pBytes;
	GLint iWidth, iHeight, iComponents;
	GLenum eFormat;
	
	GLfloat whiteLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat lightPos[] = { -10.f, 5.0f, 5.0f, 1.0f };
	
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
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	pBytes = gltLoadTGA( "Stone.tga", &iWidth, &iHeight, &iComponents, &eFormat );
	glTexImage2D( GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes );
	free( pBytes );
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	/*
		纹理环境模式，把纹理单元颜色与几何图元颜色进行组合
		GL_MODULATE，相乘
		GL_ADD，相加
		GL_REPLACE，替换
		GL_DECAL，替换，如果纹理具有alpha成分则与底层几何图元的alpha成分混合
		GL_BLEND，混合，与一个常量颜色混合，使用时必须指定纹理环境颜色
	*/
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	//GLfloat fColor[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
	//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
	//glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, fColor );

	glEnable( GL_TEXTURE_2D );
}

//对箭头的响应
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
	GLTVector3 vNormal;
	GLTVector3 vCorners[5] = 
	{
		{ 0.0f, 0.8f, 0.0f },
		{ -0.5f, 0.0f, -0.5f },
		{ 0.5f, 0.0f, -0.5f },
		{ 0.5f, 0.0f, 0.5f },
		{ -0.5f, 0.0f, 0.5f }
	};
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glPushMatrix();
	
	glTranslatef( 0.0f, -0.25f, -4.0f );
	glRotatef( xRot, 1.0f, 0.0f, 0.0f );
	glRotatef( yRot, 0.0f, 1.0f, 0.0f );
	
	glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_TRIANGLES );
		//底面部分 - 两个三角形
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3fv( vCorners[2] );
		
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3fv( vCorners[4] );
		
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3fv( vCorners[1] );
		
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3fv( vCorners[2] );
		
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3fv( vCorners[3] );
		
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3fv( vCorners[4] );
		
		//正面
		gltGetNormalVector( vCorners[0], vCorners[4], vCorners[3], vNormal );
		glNormal3fv( vNormal );
		glTexCoord2f( 0.5f, 1.0f );
		glVertex3fv( vCorners[0] );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3fv( vCorners[4] );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3fv( vCorners[3] );
		
		//左面
		gltGetNormalVector( vCorners[0], vCorners[1], vCorners[4], vNormal );
		glNormal3fv( vNormal );
		glTexCoord2f( 0.5f, 1.0f );
		glVertex3fv( vCorners[0] );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3fv( vCorners[1] );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3fv( vCorners[4] );
		
		//背面
		gltGetNormalVector( vCorners[0], vCorners[2], vCorners[1], vNormal );
		glNormal3fv( vNormal );
		glTexCoord2f( 0.5f, 1.0f );
		glVertex3fv( vCorners[0] );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3fv( vCorners[2] );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3fv( vCorners[1] );
		
		//右面
		gltGetNormalVector( vCorners[0], vCorners[3], vCorners[2], vNormal );
		glNormal3fv( vNormal );
		glTexCoord2f( 0.5f, 1.0f );
		glVertex3fv( vCorners[0] );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3fv( vCorners[3] );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3fv( vCorners[2] );
	
	glEnd();
	
	glPopMatrix();
	glutSwapBuffers();
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Textured Pyramid" );
	glutReshapeFunc( ChangeSize );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );
	SetupRC();
	glutMainLoop();
	
	return 0;
}

