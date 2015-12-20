
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"
#include "GLEXT.H"

#include <math.h>

void toonDrawTorus( GLfloat majorRadius, GLfloat minorRadius, int numMajor, int numMinor, GLTVector3 vLightDir )
{
	GLTMatrix mModelViewMatrix;
	GLTVector3 vNormal, vTransformedNormal;
	double majorStep = 2.0f * GLT_PI / numMajor;
	double minorStep = 2.0f * GLT_PI / numMinor;
	int i, j;
	
	//取模型视图矩阵
	glGetFloatv( GL_MODELVIEW_MATRIX, mModelViewMatrix );
	
	//对光照向量进行规范化
	gltNormalizeVector( vLightDir );
	
	//用一系列三角形带绘制圆环面
	for ( i = 0; i < numMajor; ++i )
	{
		double a0 = i * majorStep;
		double a1 = a0 + majorStep;
		GLfloat x0 = (GLfloat)cos( a0 );
		GLfloat y0 = (GLfloat)sin( a0 );
		GLfloat x1 = (GLfloat)cos( a1 );
		GLfloat y1 = (GLfloat)sin( a1 );
		
		glBegin( GL_TRIANGLE_STRIP );
		for ( j = 0; j <= numMinor; j++ )
		{
			double b = j * minorStep;
			GLfloat c = (GLfloat)cos(b);
			GLfloat r = minorRadius * c + majorRadius;
			GLfloat z = minorRadius * (GLfloat)sin(b);
			
			//第一个点
			vNormal[0] = x0 * c;
			vNormal[1] = y0 * c;
			vNormal[2] = z / minorRadius;
			gltNormalizeVector( vNormal );
			gltRotateVector( vNormal, mModelViewMatrix, vTransformedNormal );
			
			//纹理坐标根据光照强度设置
			glTexCoord1f( gltVectorDotProduct( vLightDir, vTransformedNormal ) );
			glVertex3f( x0 * r, y0 * r, z );
			
			//第二个点
			vNormal[0] = x1 * c;
			vNormal[1] = y1 * c;
			vNormal[2] = z / minorRadius;
			gltNormalizeVector( vNormal );
			gltRotateVector( vNormal, mModelViewMatrix, vTransformedNormal );
			
			//纹理坐标根据光照强度设置
			glTexCoord1f( gltVectorDotProduct( vLightDir, vTransformedNormal ) );
			glVertex3f( x1 * r, y1 * r, z );
		}
		glEnd();
	}
}

void RenderScene()
{
	static GLfloat yRot = 0.0f;
	GLTVector3 vLightDir = { -1.0f, 1.0f, 1.0f };
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -2.5f );
	glRotatef( yRot, 0.0f, 1.0f, 0.0f );
	toonDrawTorus( 0.35f, 0.15f, 50, 25, vLightDir );
	glPopMatrix();
	
	glutSwapBuffers();
	glutPostRedisplay();	//必须添加，否则time函数不启作用
	
	yRot += 0.5f;
}

void SetupRC()
{
	//用toon着色值载入一个1D纹理
	GLbyte toonTable[4][3] =
	{
		{ 0, 32, 0 },
		{ 0, 64, 0 },
		{ 0, 128, 0 },
		{ 0, 192, 0 }
	};
	
	glClearColor( 0.0f, 0.0f, 0.5f, 1.0f );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, toonTable );
	
	glEnable( GL_TEXTURE_1D );

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
	glutCreateWindow( "Toon/Cell Shading Deom" );
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );
	glutTimerFunc( 33, TimerFunction, 1 );
	
	SetupRC();
	glutMainLoop();
	
	return 0;
}
