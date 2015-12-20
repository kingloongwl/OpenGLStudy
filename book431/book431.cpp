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
	//围绕原子核的旋转角度
	static GLfloat fElect1 = 0.0f;

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	//用当前的清除颜色清除窗口
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//重置模型视图矩阵
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//移动整个场景到视图中，这是初始的视图转换
	glTranslatef( 0.0f, 0.0f, -200.0f );

	//红色原子核
	glColor3ub( 255, 0, 0 );
	glutSolidSphere( 10.0f, 15, 15 );

	//黄色电子
	glColor3ub( 255, 255, 0 );

	//第一个电子轨道
	//保存视图转换
	glPushMatrix();

	//根据一个旋转角度进行旋转
	glRotatef( fElect1, 0.0f, 1.0f, 0.0f ); 

	//从原点移动到轨道距离
	glTranslatef( 90.0f, 0.0f, 0.0f ); 

	//绘制电子，此处的位置是先让坐标轴绕沿Z轴平移-200，Y轴旋转得到一个坐标系，
	//再让这个坐标轴沿它的X轴平移,最后在此时的坐标原点处画球体
	glutSolidSphere( 6.0f, 15, 15 );

	//恢复视图转换
	glPopMatrix();

	//第二个电子轨道
	glPushMatrix();
	glRotatef( 45.0f, 0.0f, 0.0f, 1.0f );
	glRotatef( fElect1, 0.0f, 1.0f, 0.0f );
	glTranslatef( -70.0f, 0.0f, 0.0f );
	glutSolidSphere( 6.0f, 15, 15 );
	glPopMatrix();

	//第三个电子轨道
	glPushMatrix();
	glRotatef( -45.0f, 0.0f, 0.0f, 1.0f );
	glRotatef( fElect1, 0.0f, 1.0f, 0.0f );
	glTranslatef( 0.0f, 0.0f, 60.0f );
	glutSolidSphere( 6.0f, 15, 15 );
	glPopMatrix();

	//增加旋转角度
	fElect1 += 10.0f;
	if ( fElect1 > 360.0f )
	{
		fElect1 = 0.0f;
	}

	glutSwapBuffers();

	glutTimerFunc( 100, TimerFunction, 1 );
}

// void ChangeSize( int w, int h ) //平行投影情况下
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

void ChangeSize( int w, int h ) //透视投影情况下
{
	GLfloat fAspect;
	if ( h == 0 )
	{
		h = 1;
	}

	//把视口设置为窗口大小
	glViewport( 0, 0, w, h );
	fAspect = (GLfloat)w/(GLfloat)h;

	//重置坐标系统
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//产生透视投影
	gluPerspective( 60.0f, fAspect, 1.0, 400.0 ); //参数含义，垂直方向的视野角度，宽高比，近远端裁切平面的距离

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

