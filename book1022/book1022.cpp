
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>


GLint nNumPoints = 3;
GLfloat ctrlPoints[3][3][3] = {
	{
		{ -4.0f, 0.0f, 4.0f },
		{ -2.0f, 4.0f, 4.0f },
		{ 4.0f, 0.0f, 4.0f }
	},

	{
		{ -4.0f, 0.0f, 0.0f },
		{ -2.0f, 4.0f, 0.0f },
		{ 4.0f, 0.0f, 0.0f }
	},

	{
		{ -4.0f, 0.0f, -4.0f },
		{ -2.0f, 4.0f, -4.0f },
		{ 4.0f, 0.0f, -4.0f }
	}

	};
	
void DrawPoints()
{
	int i, j;
	
	glPointSize( 5.0f );
	
	glBegin( GL_POINTS );
		for( i = 0; i < nNumPoints; i++ )
		{
			for ( j = 0; j < 3; j++ )
			{
				glVertex3fv( ctrlPoints[i][j] );
			}
			
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
	
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	
	glRotatef( 45.0f, 0.0f, 1.0f, 0.0f );
	glRotatef( 60.0f, 1.0f, 0.0f, 0.0f );
	
	//����Bezier����
	glMap2f( GL_MAP2_VERTEX_3,	//�������ݵ�����
		0.0f,					//uֵ��Χ���½�
		10.0f,					//uֵ��Χ���Ͻ�
		3,						//�����е�ľ���
		3,						//���Ƶ������
		0.0f,					//vֵ��Χ���½�
		10.0f,					//vֵ��Χ���Ͻ�
		9,						//�����е�ľ���
		3,						//���Ƶ������
		&ctrlPoints[0][0][0] );	//���Ƶ�����
		
	//������ֵ��
	glEnable( GL_MAP2_VERTEX_3 );
	
	glMapGrid2f( 10.0, 0.0f, 10.0f, 10, 0.0f, 10.0f );	//��0��10ӳ��һ������10���������

#if 1
	glEvalMesh2( GL_LINE, 0, 10, 0, 10 );	//��������ʹ��ֱ��
#else
	glEvalMesh2( GL_FILL, 0, 10, 0, 10 );
	glEnable( GL_AUTO_NORMAL );
#endif
	
	DrawPoints();
	
	glPopMatrix();
	
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
	
	glOrtho( -10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f  );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Bezier3D" );
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );
	
	SetupRC();

	glutMainLoop();
	
	return 0;
}
