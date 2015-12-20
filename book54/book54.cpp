// book54.cpp : Defines the entry point for the console application.
//


#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

void SetupRC()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

void RenderScene()
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//启用渐变着色
	glShadeModel( GL_SMOOTH );


	//绘制三角形
	glBegin( GL_TRIANGLES );

	//红色顶点
	glColor3ub( 255, 0, 0 );
	glVertex3f( 0.0f, 200.0f, 0.0f );

	//右下角的绿色顶点
	glColor3ub( 0, 255, 0 );
	glVertex3f( 200.0f, -70.0f, 0.0f );

	//左下角的蓝色顶点
	glColor3ub( 0, 0, 255 );
	glVertex3f( -200.0f, -70.0f, 0.0f );
	glEnd();

	glutSwapBuffers();
}

void ChangeSize( int w, int h )
{
	GLfloat windowHeight,windowWidth;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Window is higher than wide
	if (w <= h) 
	{
		windowHeight = 250.0f*h/w;
		windowWidth = 250.0f;
	}
	else 
	{
		// Window is wider than high
		windowWidth = 250.0f*w/h;
		windowHeight = 250.0f;
	}

	// Set the clipping volume
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glOrtho(-windowWidth, windowWidth, -windowHeight, windowHeight, 1.0f, -1.0f);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutCreateWindow( "TRIANGLE" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );

	SetupRC();

	glutMainLoop();

	return 0;
}

