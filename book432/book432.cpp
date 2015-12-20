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
 	glEnable( GL_DEPTH_TEST ); //开启深度测试
 	glFrontFace( GL_CCW ); //逆时针环绕被认为是正面的
 	glEnable( GL_CULL_FACE ); //开启背面剔除
 
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

	//防止被零所除
	if ( h == 0 )
	{
		h = 1;
	}

	//将视口设置为窗口大小
	glViewport( 0, 0, w, h );

	//计算窗口的纵横比
	fAspect = (GLfloat)w/(GLfloat)h;

	//设置透视坐标系统
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//垂直方向45度视野，近和远平面为1.0和425
	gluPerspective( 45.0f, fAspect, 1.0, 425.0 );

	//重置模型视图矩形
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void RenderScene()
{
	//地球和月球的旋转角度
	static float fMoonRot = 0.0f;
	static float fEarthRot = 0.0f;

	//用当前清除颜色清除窗口
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//保存矩阵状态并进行旋转
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	//把整个场景移动到视图中，因为眼睛在原点处
	glTranslatef( 0.0f, 0.0f, -300.0f );

	//设置材料颜色，太阳为黄色
	glDisable( GL_LIGHTING );
	glColor3ub( 255, 255, 0 );
	glutSolidSphere( 15.0f, 30, 17 );
	glEnable( GL_LIGHTING );

	//在绘制太阳之后放置光源
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

	//旋转坐标系统
	glRotatef( fEarthRot, 0.0f, 1.0f, 0.0f );

	//绘制地球
	glColor3ub( 0, 0, 255 );
	glTranslatef( 105.0f, 0.0f, 0.0f );

	//此处的位置是，坐标轴先以Z轴平移-300，再绕Y轴旋转角度，再沿X轴平移105
	//此处的绕坐标轴变换都是以新的坐标轴为准，最终的位置就是新坐标轴的原点
	glutSolidSphere( 15.0f, 30, 17 );

	//根据基于地球的坐标进行旋转，并绘制月球
	glColor3ub( 200, 200, 200 );
	glRotatef( fMoonRot, 0.0f, 1.0f, 0.0f );
	glTranslatef( 30.0f, 0.0f, 0.0f );
	fMoonRot += 15.0f;
	if ( fMoonRot > 360.0f )
	{
		fMoonRot = 0.0f;
	}

	glutSolidSphere( 6.0f, 30, 17 );

	//恢复矩阵状态
	glPopMatrix(); //模型视图矩阵

	//每次绕轨道旋转5度
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

