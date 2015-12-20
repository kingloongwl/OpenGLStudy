// book431.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

void TimerFunction( int value )
{
	glutPostRedisplay();
}

void RenderScene()
{
	//Χ��ԭ�Ӻ˵���ת�Ƕ�
	static GLfloat fElect1 = 0.0f;

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	//�õ�ǰ�������ɫ�������
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//����ģ����ͼ����
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//�ƶ�������������ͼ�У����ǳ�ʼ����ͼת��
	glTranslatef( 0.0f, 0.0f, -200.0f );

	//��ɫԭ�Ӻ�
	glColor3ub( 255, 0, 0 );
	glutSolidSphere( 10.0f, 15, 15 );

	//��ɫ����
	glColor3ub( 255, 255, 0 );

	//��һ�����ӹ��
	//������ͼת��
	glPushMatrix();

	//����һ����ת�ǶȽ�����ת
	glRotatef( fElect1, 0.0f, 1.0f, 0.0f ); 

	//��ԭ���ƶ����������
	glTranslatef( 90.0f, 0.0f, 0.0f ); 

	//���Ƶ��ӣ��˴���λ������������������Z��ƽ��-200��Y����ת�õ�һ������ϵ��
	//�������������������X��ƽ��,����ڴ�ʱ������ԭ�㴦������
	glutSolidSphere( 6.0f, 15, 15 );

	//�ָ���ͼת��
	glPopMatrix();

	//�ڶ������ӹ��
	glPushMatrix();
	glRotatef( 45.0f, 0.0f, 0.0f, 1.0f );
	glRotatef( fElect1, 0.0f, 1.0f, 0.0f );
	glTranslatef( -70.0f, 0.0f, 0.0f );
	glutSolidSphere( 6.0f, 15, 15 );
	glPopMatrix();

	//���������ӹ��
	glPushMatrix();
	glRotatef( -45.0f, 0.0f, 0.0f, 1.0f );
	glRotatef( fElect1, 0.0f, 1.0f, 0.0f );
	glTranslatef( 0.0f, 0.0f, 60.0f );
	glutSolidSphere( 6.0f, 15, 15 );
	glPopMatrix();

	//������ת�Ƕ�
	fElect1 += 10.0f;
	if ( fElect1 > 360.0f )
	{
		fElect1 = 0.0f;
	}

	glutSwapBuffers();

	glutTimerFunc( 100, TimerFunction, 1 );
}

// void ChangeSize( int w, int h ) //ƽ��ͶӰ�����
// {
// 	GLfloat nRange = 100.0f;
// 
// 	glViewport( 0, 0, w, h );
// 	if ( h == 0 )
// 	{
// 		h = 1;
// 	}
// 
// 	glMatrixMode( GL_PROJECTION );
// 	glLoadIdentity();
// 
// 	if ( w > h )
// 	{
// 		glOrtho( -nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange );
// 	}
// 	else
// 	{
// 		glOrtho( -nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange );
// 	}
// 
// 	glMatrixMode( GL_MODELVIEW );
// 	glLoadIdentity();
// }

void ChangeSize( int w, int h ) //͸��ͶӰ�����
{
	GLfloat fAspect;
	if ( h == 0 )
	{
		h = 1;
	}

	//���ӿ�����Ϊ���ڴ�С
	glViewport( 0, 0, w, h );
	fAspect = (GLfloat)w/(GLfloat)h;

	//��������ϵͳ
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//����͸��ͶӰ
	gluPerspective( 60.0f, fAspect, 1.0, 400.0 ); //�������壬��ֱ�������Ұ�Ƕȣ���߱ȣ���Զ�˲���ƽ��ľ���

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutCreateWindow( "ATOM" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );
	glutTimerFunc( 100, TimerFunction, 1 );

	glutMainLoop();


	return 0;
}

