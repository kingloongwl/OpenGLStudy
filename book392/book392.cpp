// book392.cpp : Defines the entry point for the console application.
//


#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

void SetupRC()
{
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	glColor3f( 0.0f, 1.0f, 0.0f );

}

void RenderScene()
{
	glClear( GL_COLOR_BUFFER_BIT );

	/* ���������ֻ���� */
	glPolygonMode( GL_FRONT, GL_LINE );

	glPushMatrix();

	glBegin( GL_TRIANGLES );
	glVertex2f( 0, 50 ); //��
	glEdgeFlag( false ); //�ڴ˾�֮����ĵ���Ϊ��㵽��һ�����ǲ����ߵ�
	glVertex2f( -20, 20 );
	glEdgeFlag( true );
	glVertex2f( 20, 20 );

	glVertex2f( 0, -50 ); //��
	glEdgeFlag( false );
	glVertex2f( 20, -20 );
	glEdgeFlag( true );
	glVertex2f( -20, -20 );

	glVertex2f( -50, 0 ); //��
	glEdgeFlag( false );
	glVertex2f( -20, -20 );
	glEdgeFlag( true );
	glVertex2f( -20, 20 );

	glVertex2f( 50, 0 ); //��
	glEdgeFlag( false );
	glVertex2f( 20, 20 );
	glEdgeFlag( true );
	glVertex2f( 20, -20 );

	glEdgeFlag( false );
	glVertex2f( -20, 20 ); //����
	glVertex2f( -20, -20 );
	glVertex2f( 20, 20 );

	glVertex2f( 20, -20 ); //����
	glVertex2f( 20, 20 );
	glVertex2f( -20, -20 );
	glEdgeFlag( true );
	glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void ChangeSize( int w, int h )
{
	GLfloat nRange = 100.0f;

	glViewport( 0, 0, w, h );

	if ( h == 0 )
	{
		h = 1;
	}

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	if ( w > h )
	{
		glOrtho( -nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange );
	}
	else
	{
		glOrtho( -nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange );
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}


int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutCreateWindow( "STAR" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );

	SetupRC();

	glutMainLoop();

	return 0;
}

