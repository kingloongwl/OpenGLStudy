
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
	
	//设置Bezier表面
	glMap2f( GL_MAP2_VERTEX_3,	//生成数据的类型
		0.0f,					//u值范围的下界
		10.0f,					//u值范围的上界
		3,						//数据中点的距离
		3,						//控制点的数量
		0.0f,					//v值范围的下界
		10.0f,					//v值范围的上界
		9,						//数据中点的距离
		3,						//控制点的数量
		&ctrlPoints[0][0][0] );	//控制点数组
		
	//启用求值器
	glEnable( GL_MAP2_VERTEX_3 );
	
	glMapGrid2f( 10.0, 0.0f, 10.0f, 10, 0.0f, 10.0f );	//从0到10映射一个包含10个点的网格

#if 1
	glEvalMesh2( GL_LINE, 0, 10, 0, 10 );	//计算网格，使用直线
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
