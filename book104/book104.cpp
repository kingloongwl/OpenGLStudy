
#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include "GLTools.h"
#include "GLEXT.H"

#ifndef WIN32
#define CALLBACK
#endif


#define COAST_POINTS 24
GLdouble vCoast[ COAST_POINTS ][ 3 ] =
{
	{ -70.0, 30.0, 0.0 },
	{ -50.0, 30.0, 0.0 },
	{ -50.0, 27.0, 0.0 },
	{ -5.0, 27.0, 0.0 },
	{ 0.0, 20.0, 0.0 },
	{ 8.0, 10.0, 0.0 },
	{ 12.0, 5.0, 0.0 },
	{ 10.0, 0.0, 0.0 },
	{ 15.0, -10.0, 0.0 },
	{ 20.0, -20.0, 0.0 },
	{ 20.0, -35.0, 0.0 },
	{ 10.0, -40.0, 0.0 },
	{ 0.0, -30.0, 0.0 },
	{ -5.0, -20.0, 0.0 },
	{ -12.0, -10.0, 0.0 },
	{ -13.0, -5.0, 0.0 },
	{ -12.0, 5.0, 0.0 },
	{ -20.0, 10.0, 0.0 },
	{ -30.0, 20.0, 0.0 },
	{ -40.0, 15.0, 0.0 },
	{ -50.0, 15.0, 0.0 },
	{ -55.0, 20.0, 0.0 },
	{ -60.0, 25.0, 0.0 },
	{ -70.0, 25.0, 0.0 }
};

#define LAKE_POINTS 4
GLdouble vLake[LAKE_POINTS][3] =
{
	{ 10.0, -20.0, 0.0 },
	{ 15.0, -25.0, 0.0 },
	{ 10.0, -30.0, 0.0 },
	{ 5.0, -25.0, 0.0 }
};

#define DRAW_LOOPS		0
#define DRAW_CONCAVE	1
#define DRAW_COMPLEX	2
int iMethod = DRAW_LOOPS;

void ProcessMenu( int value )
{
	iMethod = value;
	glutPostRedisplay();
}

void CALLBACK tessError( GLenum error )
{
	const unsigned char *szError = gluErrorString( error );
	
	glutSetWindowTitle( (char*)szError );
}

void SetupRC()
{
	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
	

}


void RenderScene()
{
	int i;
	glClear( GL_COLOR_BUFFER_BIT );
	
	switch ( iMethod )
	{
	case DRAW_LOOPS:
		{
			glColor3f( 0.0f, 0.0f, 0.0f );
			
			glBegin( GL_LINE_LOOP );
				for ( i = 0; i < COAST_POINTS; i++ )
				{
					glVertex3dv( vCoast[i] );
				}
			glEnd();
			
			glBegin( GL_LINE_LOOP );
				for ( int i = 0; i < LAKE_POINTS; i++ )
				{
					glVertex3dv( vLake[i] );
				}
			glEnd();
		}
		break;
	case DRAW_CONCAVE:
		{
			GLUtesselator *pTess;
			
			
			glColor3f( 0.0f, 1.0f, 0.0f );
			
			//������Ƕ������
			pTess = gluNewTess();
			
			//���ûص�����
			gluTessCallback( pTess, GLU_TESS_BEGIN, (void (_stdcall *)(void))glBegin );
			gluTessCallback( pTess, GLU_TESS_END, glEnd );
			gluTessCallback( pTess, GLU_TESS_VERTEX, (void( _stdcall * )(void))glVertex3dv );
			gluTessCallback( pTess, GLU_TESS_ERROR, (void( _stdcall * )(void))tessError );
			
			//��ʼ�����
			gluTessBeginPolygon( pTess, NULL );
			
			//��ʼΨһ��һ��������
			gluTessBeginContour( pTess );
			
			//���붥���б�
			for ( i = 0; i < COAST_POINTS; i++ )
			{
				gluTessVertex( pTess, vCoast[i], vCoast[i] );
			}
			
			//�պ������ߺͶ����
			gluTessEndContour( pTess );
			gluTessEndPolygon( pTess );
			
			gluDeleteTess( pTess );
		}
		break;
	case DRAW_COMPLEX:
		{
			GLUtesselator *pTess;
			
			glColor3f( 0.0f, 1.0f, 0.0f );
			
			//������Ƕ������
			pTess = gluNewTess();
			
			//���ûص�����
			gluTessCallback( pTess, GLU_TESS_BEGIN, (void( _stdcall * )(void))glBegin );
			gluTessCallback( pTess, GLU_TESS_END, glEnd );
			gluTessCallback( pTess, GLU_TESS_VERTEX, (void( _stdcall * )(void))glVertex3dv );
			gluTessCallback( pTess, GLU_TESS_ERROR, (void( _stdcall * )(void))tessError );
			
			//��μ������Ϳ�������
			//GLU_TESS_WINDING_ODD��ʾ�������������߰�Χ�����
			//��ż���������߰�Χ�����
			gluTessProperty( pTess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD );
			
			//��ʼ�����
			gluTessBeginPolygon( pTess, NULL );
			
			//��ʼһ��������
			gluTessBeginContour( pTess );
			for ( i = 0; i < COAST_POINTS; i++ )
			{
				gluTessVertex( pTess, vCoast[i], vCoast[i] );
			}
			gluTessEndContour( pTess );
			
			//��ʼ��һ��������
			gluTessBeginContour( pTess );
			for ( i = 0; i < LAKE_POINTS; i++ )
			{
				gluTessVertex( pTess, vLake[i], vLake[i] );
			}
			gluTessEndContour( pTess );
			
			
			//�պ������ߺͶ����
			gluTessEndPolygon( pTess );
			
			gluDeleteTess( pTess );
		}
		break;
	}
	
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
	
	gluOrtho2D( -80, 35, -50, 50 );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Tesselate" );
	
	glutCreateMenu( ProcessMenu );
	glutAddMenuEntry( "Line Loops", DRAW_LOOPS );
	glutAddMenuEntry( "Concave Polygon", DRAW_CONCAVE );
	glutAddMenuEntry( "Complex Polygon", DRAW_COMPLEX );
	glutAttachMenu( GLUT_RIGHT_BUTTON );
	
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );
	
	SetupRC();

	glutMainLoop();
	
	return 0;
}
