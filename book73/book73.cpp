#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"

void SetupRC()
{
	
}

void RenderScene()
{
	GLbyte *pImage = NULL;
	GLint iWidth, iHeight, iComponents;
	GLenum eFormat;

	glClear( GL_COLOR_BUFFER_BIT );

	//Targas�ǰ�1���ֽڶ���ģ�GL_UNPACK_ALIGNMENTָ����δ����ݻ�������ͼ�����ݽ��н��
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	//����TGA�ļ�����ȡ��ȣ��߶Ⱥͳɷ֡���ʽ��Ϣ
	pImage = gltLoadTGA( "Fire.tga", &iWidth, &iHeight, &iComponents, &eFormat );

	//ʹ�ô���λ�����ù�դλ�ã�Ҳ���ǻ�ͼʱ���½�������
	glRasterPos2i( 0, 0 );

	//��������ͼ
	if ( pImage != NULL )
	{
		glDrawPixels( iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage );
	}

	//������Ҫͼ������
	free( pImage );

	//��������
	glRasterPos2i( 400, 300 );	//ָ��Ŀ��ص����ʼλ�ã����½�	
	glCopyPixels( 0, 0, 200, 200, GL_COLOR );	//ִ��copy�������Ǿ����������ʽ����ʽ������GL_COLOR,GL_DEPTH,GLSTENCIL


	//���л���������
	glutSwapBuffers();

}

void ChangeSize( int w, int h )
{
	if ( h == 0 )
	{
		h = 1;
	}

	glViewport( 0, 0, w, h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluOrtho2D( 0, w, 0, h );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

}

int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "TGA" );

	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );

	glutMainLoop();

	return 0;
}