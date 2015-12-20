// book433.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include "GLTools.h"

#include <math.h>



void DrawTorus( GLTMatrix mTransform )
{
	GLfloat majorRadius = 0.35f;
	GLfloat minorRadius = 0.15f;
	GLint numMajor = 40;
	GLint numMinor = 20;
	GLTVector3 objectVertex;
	GLTVector3 transformdVertex;
	double majorStep = 2.0 * GLT_PI / numMajor;
	double minorStep = 2.0 * GLT_PI / numMinor;

	for ( int i = 0; i < numMajor; i++ )
	{
		double a0 = i * majorStep;
		double a1  = a0 + majorStep;
		
		GLfloat x0 = (GLfloat)cos( a0 );
		GLfloat y0 = (GLfloat)sin( a0 );
		GLfloat x1 = (GLfloat)cos( a1 );
		GLfloat y1 = (GLfloat)sin( a1 );

		glBegin( GL_TRIANGLE_STRIP );
		for ( int j = 0; j <= numMinor; j++ )
		{
			double b = j * minorStep;
			GLfloat c = (GLfloat)cos( b );
			GLfloat r = minorRadius * c + majorRadius;
			GLfloat z = minorRadius * (GLfloat)sin( b );

			objectVertex[0] = x0 * r;
			objectVertex[1] = y0 * r;
			objectVertex[2] = z;
			//执行顶点与矩阵的乘法并把转换后的顶点返回到数组中（第三个参数）
			gltTransformPoint( objectVertex, mTransform, transformdVertex );
			glVertex3fv( transformdVertex );

			objectVertex[0] = x1 * r;
			objectVertex[1] = y1 * r;
			objectVertex[2] = z;
			gltTransformPoint( objectVertex, mTransform, transformdVertex );
			glVertex3fv( transformdVertex );
		}
		glEnd();
	}

}

void TimeFunction( int value )
{
	glutPostRedisplay();
}

void RenderScene()
{
	GLTMatrix transformationMatrix; //旋转矩阵的存储空间
	static GLfloat yRot = 0.0f;
	yRot += 0.5f;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//创建一个旋转矩阵
	//第一个参数是弧度，第二个是旋转轴，第三个是要得到的旋转矩阵
	gltRotationMatrix( gltDegToRad( yRot ), 0.0f, 1.0f, 0.0f, transformationMatrix );

	//通过指定这三个成员，说明沿Z轴向下移2.5个单位
	transformationMatrix[12] = 0.0f;
	transformationMatrix[13] = 0.0f;
	transformationMatrix[14] = -2.5f;

#if 0 //TRANSFORM

	DrawTorus( transformationMatrix );

#else //TRANSFORMGL

	glLoadMatrixf( transformationMatrix );
	gltDrawTorus( 0.35, 0.15, 40, 20 );

#endif

	//执行缓冲区交换
	glutSwapBuffers();

	glutTimerFunc( 33, TimeFunction, 0 );
}

void SetupRC()
{
	glClearColor( 0.0f, 0.0f, 0.5f, 1.0f );

	//只画轮廓
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

void ChangeSize( int w, int h )
{
	GLfloat fAspect;

	if ( h == 0 )
	{
		h = 1;
	}

	glViewport( 0, 0, w, h );

	fAspect = (GLfloat)w/(GLfloat)h;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluPerspective( 35.0f, fAspect, 1.0f, 50.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutCreateWindow( "TRANSFORM" );
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );

	SetupRC();

	glutTimerFunc( 33, TimeFunction, 0 );

	glutMainLoop();

	return 0;
}

