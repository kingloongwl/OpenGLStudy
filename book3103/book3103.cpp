// book3103.cpp : Defines the entry point for the console application.
//



#include <Windows.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include <math.h>

/* 方块的初始位置大小 */
GLfloat xs = 0.0f;
GLfloat ys = 0.0f;
GLfloat rsize = 25;

/* 每次移动的象素量 */
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

/* 追踪窗口宽度和高度的变化 */
GLfloat windowWidth;
GLfloat windowHeight;


void TimerFunction( int value )
{
	//在达到左边或右边时翻转方向
	if ( xs > windowWidth-rsize || xs < -windowWidth )
	{
		xstep = -xstep;
	}

	//在达到顶边或底边时翻转方向
	if ( ys > windowHeight || ys < -windowHeight + rsize )
	{
		ystep = -ystep;
	}

	xs += xstep;
	ys += ystep;

	//检查边界。这是为了防止方块在反弹时窗口变小，使方块出现在新的裁剪区域之外
	if ( xs > (windowWidth-rsize+xstep) )
	{
		xs = windowWidth-rsize-1;
	}
	else if ( xs < -(windowWidth+xstep) )
	{
		xs = -windowWidth - 1;
	}

	if ( ys > (windowHeight+ystep) )
	{
		ys = windowHeight - 1;
	}
	else if ( ys < -(windowHeight-rsize+ystep) )
	{
		ys = -windowHeight+rsize-1;
	}

	glutPostRedisplay();
	//	glutTimerFunc( 33, TimerFunction, 1 );
}

void RenderScene()
{
	GLdouble dRadius = 0.1;//最初的螺旋半径
	GLdouble dAngle;//循环变量

	//清除蓝色窗口
	glClearColor( 0.0, 0.0, 1.0, 0.0 );

	//使用0表示清除模板，启用模板测试
	glClearStencil( 0.0f );
	glEnable( GL_STENCIL_TEST );

	//清除颜色和模板缓冲区
	glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	//所有无法通过模板测试的绘图命令并不进行绘图，而是增加模板缓冲区内的值
	glStencilFunc( GL_NEVER, 0x0, 0x0 );
	glStencilOp( GL_INCR, GL_INCR, GL_INCR );

	//螺旋模型将创建模板模型，用白线绘制螺旋模型
	//我们用白色来绘制这些线，以说明模板函数防止了它们被绘制
	glColor3f( 1.0, 1.0, 1.0 );
	glBegin( GL_LINE_STRIP );
	for ( dAngle = 0; dAngle < 400.0; dAngle += 0.1 )
	{
		glVertex2d( dRadius * cos( dAngle ), dRadius *sin( dAngle ) );
		dRadius *= 1.002;
	}
	glEnd();

	//现在，允许进行绘图，但不包括那些模板模型是0x1的地方，并且不会对模板缓冲区进行修改
	glStencilFunc( GL_NOTEQUAL, 0x1, 0x1 );
	glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

	//现在绘制红色反弹方块
	//(x和y）由一个计时器函数修改
	glColor3f( 1.0, 0.0, 0.0 );                //模板缓冲区已经允许绘图，但不能在模板模型等于0x1的地方绘图，这个地方已经
	glRectf( xs, ys, xs+rsize, ys-rsize );     //设定成了螺旋线，所以红色方块中有螺旋线的地方不会被复制

	glutSwapBuffers();

	glutTimerFunc( 33, TimerFunction, 1 );

}

void ChangeSize( GLsizei w, GLsizei h )
{
	GLfloat aspectRatio;

	if ( h == 0 )
	{
		h = 1;
	}

	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	aspectRatio = (GLfloat)w/(GLfloat)h;
	if ( w<=h )
	{
		windowWidth = 100;
		windowHeight = 100/aspectRatio;
		glOrtho( -100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0 );
	}
	else
	{
		windowWidth = 100 * aspectRatio;
		windowHeight = 100;
		glOrtho( -windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0 );
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, argv );

	//必须要申请模板缓冲区
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_STENCIL );
	glutCreateWindow( "STENCIL" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );
	glutTimerFunc( 33, TimerFunction, 1 );

	glutMainLoop();

	return 0;
}

