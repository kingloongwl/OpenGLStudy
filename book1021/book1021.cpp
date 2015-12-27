
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>


GLint nNumPoints = 4;
GLfloat ctrlPoints[4][3] = {
	{ -4.0f, 0.0f, 0.0f },
	{ -6.0f, 4.0f, 0.0f },
	{ 6.0f, -4.0f, 0.0f },
	{ 4.0f, 0.0f, 0.0f }
	};
	
void DrawPoints()
{
	int i;
	
	glPointSize( 5.0f );
	
	glBegin( GL_POINTS );
		for( i = 0; i < nNumPoints; i++ )
		{
			glVertex2fv( ctrlPoints[i] );
		}
	glEnd();
}

void SetupRC()
{
	glClearColor( 0.25f, 0.25f, 0.5f, 1.0f );
	
	glColor3f( 0.0f, 0.0f, 1.0f );

}


void RenderScene()
{
	int i;
	glClear( GL_COLOR_BUFFER_BIT );
	
	//����Bezier����
	glMap1f( GL_MAP1_VERTEX_3,	//�������ݵ�����
		0.0f,					//uֵ��Χ���½�
		100.0f,					//uֵ��Χ���Ͻ�
		3,						//�����е�ľ���
		nNumPoints,				//���Ƶ������
		&ctrlPoints[0][0] );	//���Ƶ�����
		
	//������ֵ��
	glEnable( GL_MAP1_VERTEX_3 );
	
#if 0

	glBegin( GL_LINE_STRIP );
		for ( i = 0; i <= 100; i++ )
		{
			glEvalCoord1f( (GLfloat)i );
		}
	glEnd();

#else
	glMapGrid1d( 100, 0.0, 100.0 );	//��0��100ӳ��һ������100���������
	glEvalMesh1( GL_LINE, 0, 100 );	//��������ʹ��ֱ��

#endif
	
	DrawPoints();
	
	glutSwapBuffers();
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
	
	gluOrtho2D( -10.0f, 10.0f, -10.0f, 10.0f );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Bezier" );
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );
	
	SetupRC();

	glutMainLoop();
	
	return 0;
}
