// book432.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

GLfloat whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

void SetupRC()
{
 	glEnable( GL_DEPTH_TEST ); //������Ȳ���
 	glFrontFace( GL_CCW ); //��ʱ�뻷�Ʊ���Ϊ�������
 	glEnable( GL_CULL_FACE ); //���������޳�
 
 	glEnable( GL_LIGHTING );
 
 	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, whiteLight );
 	glLightfv( GL_LIGHT0, GL_DIFFUSE, sourceLight );
 	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
 
 	glEnable( GL_LIGHT0 );
 
 	glEnable( GL_COLOR_MATERIAL );
 
 	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

void TimeFunction( int value )
{
	glutPostRedisplay();
// 	glutTimerFunc( 100, TimeFunction, 0 );
}

void ChangeSize( int w, int h )
{
	GLfloat fAspect;

	//��ֹ��������
	if ( h == 0 )
	{
		h = 1;
	}

	//���ӿ�����Ϊ���ڴ�С
	glViewport( 0, 0, w, h );

	//���㴰�ڵ��ݺ��
	fAspect = (GLfloat)w/(GLfloat)h;

	//����͸������ϵͳ
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//��ֱ����45����Ұ������Զƽ��Ϊ1.0��425
	gluPerspective( 45.0f, fAspect, 1.0, 425.0 );

	//����ģ����ͼ����
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void RenderScene()
{
	//������������ת�Ƕ�
	static float fMoonRot = 0.0f;
	static float fEarthRot = 0.0f;

	//�õ�ǰ�����ɫ�������
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//�������״̬��������ת
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	//�����������ƶ�����ͼ�У���Ϊ�۾���ԭ�㴦
	glTranslatef( 0.0f, 0.0f, -300.0f );

	//���ò�����ɫ��̫��Ϊ��ɫ
	glDisable( GL_LIGHTING );
	glColor3ub( 255, 255, 0 );
	glutSolidSphere( 15.0f, 30, 17 );
	glEnable( GL_LIGHTING );

	//�ڻ���̫��֮����ù�Դ
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

	//��ת����ϵͳ
	glRotatef( fEarthRot, 0.0f, 1.0f, 0.0f );

	//���Ƶ���
	glColor3ub( 0, 0, 255 );
	glTranslatef( 105.0f, 0.0f, 0.0f );

	//�˴���λ���ǣ�����������Z��ƽ��-300������Y����ת�Ƕȣ�����X��ƽ��105
	//�˴�����������任�������µ�������Ϊ׼�����յ�λ�þ������������ԭ��
	glutSolidSphere( 15.0f, 30, 17 );

	//���ݻ��ڵ�������������ת������������
	glColor3ub( 200, 200, 200 );
	glRotatef( fMoonRot, 0.0f, 1.0f, 0.0f );
	glTranslatef( 30.0f, 0.0f, 0.0f );
	fMoonRot += 15.0f;
	if ( fMoonRot > 360.0f )
	{
		fMoonRot = 0.0f;
	}

	glutSolidSphere( 6.0f, 30, 17 );

	//�ָ�����״̬
	glPopMatrix(); //ģ����ͼ����

	//ÿ���ƹ����ת5��
	fEarthRot += 5.0f;
	if ( fEarthRot > 360.0f )
	{
		fEarthRot = 0.0f;
	}

	glutSwapBuffers();

	glutTimerFunc( 100, TimeFunction, 0 );
}

int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutCreateWindow( "SOLAR" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );
	glutTimerFunc( 100, TimeFunction, 0 );
	SetupRC();

	glutMainLoop();

	return 0;
}

