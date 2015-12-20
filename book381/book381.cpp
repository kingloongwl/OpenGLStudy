// book381.cpp : Defines the entry point for the console application.
//

#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include <math.h>

#define GL_PI 3.1415

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;


void SetupRC()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glColor3f( 1.0f, 0.0f, 0.0f );

	/* 设置多边形的颜色模式，以下参数设置成单调着色，GL_SMOOTH设置的是渐变着色 */
	glShadeModel( GL_FLAT );

	/* 设置顺时针环绕的多边形为正面 */
//	glFrontFace( GL_CW );

	/* 开启深度测试 */
	glEnable( GL_DEPTH_TEST );

	/* 开启背面剔除 */
//	glEnable( GL_CULL_FACE );

	/* 设置多边形的填充模式，以下是设置成只绘制边框 */
	glPolygonMode( GL_BACK, GL_LINE );
}

void RenderScene()
{
	GLfloat bottomRadius = 100.0f;
	GLfloat height = 100.0f;

	/* 由于开启了深度测试所以必须清除深度缓冲区 */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
	glRotatef( xRot, 1.0f, 0.0f, 0.0f );
	glRotatef( yRot, 0.0f, 1.0f, 0.0f );

	glBegin( GL_TRIANGLE_FAN );
	glVertex3f( 0.0f, 0.0, height );
//	glVertex3f( bottomRadius*cos(GL_PI/6), bottomRadius*sin(-GL_PI/6), 0.0f );

	GLint index = 0;
	for ( GLfloat angle = 0.0f; angle <= 2*GL_PI; angle += GL_PI/8,index+=1 )
	{
		if ( index % 2 == 0 )
		{
			glColor3f( 1.0f, 0.0f, 0.0f );
		}
		else
		{
			glColor3f( 0.0f, 1.0f, 0.0f );
		}

		glVertex3f( bottomRadius*cos( angle ), bottomRadius*sin( angle ), 0.0f );
	}

	glEnd();
	glPopMatrix();

	glutSwapBuffers();

}

void ChangeSize( int w, int h )
{
	GLfloat nRange = 200.0f;

	if ( h == 0 )
	{
		h = 1;
	}

	glViewport( 0, 0, w, h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	if ( w <= h )
	{
		glOrtho( -nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange );
	}
	else
	{
		glOrtho( -nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange );
	}

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

}

void SpecialKeys( int key, int x, int y )
{
	if ( key == GLUT_KEY_DOWN )
	{
		xRot -= 0.5f;
	}
	else if ( key == GLUT_KEY_UP )
	{
		xRot += 0.5f;
	}
	else if ( key == GLUT_KEY_LEFT )
	{
		yRot -= 0.5f;
	}
	else if ( key == GLUT_KEY_RIGHT )
	{
		yRot += 0.5f;
	}
	else if ( key > 356.0f )
	{
		xRot = 0.0f;
		yRot = 0.0f;
	}
	else if ( key < -1.0f )
	{
		xRot = 355.0f;
		yRot = 355.0f;
	}

	glutPostRedisplay();

}

int main(int argc, char* argv[])
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutCreateWindow( "TRANGLE" );
	glutDisplayFunc( RenderScene );
	glutReshapeFunc( ChangeSize );
	glutSpecialFunc( SpecialKeys );

	SetupRC();

	glutMainLoop();

	return 0;
}



// 
// 
// // Define a constant for the value of PI
// #define GL_PI 3.1415f
// 
// // Rotation amounts
// static GLfloat xRot = 0.0f;
// static GLfloat yRot = 0.0f;
// 
// // Flags for effects
// // Flags for effects
// int iCull = 0;
// int iOutline = 0;
// int iDepth = 0;
// 
// ///////////////////////////////////////////////////////////////////////////////
// // Reset flags as appropriate in response to menu selections
// void ProcessMenu(int value)
// {
// 	switch(value)
// 	{
// 	case 1:
// 		iDepth = !iDepth;
// 		break;
// 
// 	case 2:
// 		iCull = !iCull;
// 		break;
// 
// 	case 3:
// 		iOutline = !iOutline;
// 	default:
// 		break;
// 	}
// 
// 	glutPostRedisplay();
// }
// 
// 
// // Called to draw scene
// void RenderScene(void)
// {
// 	GLfloat x,y,angle;  // Storage for coordinates and angles
// 	int iPivot = 1;		// Used to flag alternating colors
// 
// 	// Clear the window and the depth buffer
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
// 	// Turn culling on if flag is set
// 	if(iCull)
// 		glEnable(GL_CULL_FACE);
// 	else
// 		glDisable(GL_CULL_FACE);
// 
// 	// Enable depth testing if flag is set
// 	if(iDepth)
// 		glEnable(GL_DEPTH_TEST);
// 	else
// 		glDisable(GL_DEPTH_TEST);
// 
// 	// Draw back side as a polygon only, if flag is set
// 	if(iOutline)
// 		glPolygonMode(GL_BACK,GL_LINE);
// 	else
// 		glPolygonMode(GL_BACK,GL_FILL);
// 
// 
// 	// Save matrix state and do the rotation
// 	glPushMatrix();
// 	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
// 	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
// 
// 
// 	// Begin a triangle fan
// 	glBegin(GL_TRIANGLE_FAN);
// 
// 	// Pinnacle of cone is shared vertex for fan, moved up Z axis
// 	// to produce a cone instead of a circle
// 	glVertex3f(0.0f, 0.0f, 75.0f);
// 
// 	// Loop around in a circle and specify even points along the circle
// 	// as the vertices of the triangle fan
// 	for(angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI/8.0f))
// 	{
// 		// Calculate x and y position of the next vertex
// 		x = 50.0f*sin(angle);
// 		y = 50.0f*cos(angle);
// 
// 		// Alternate color between red and green
// 		if((iPivot %2) == 0)
// 			glColor3f(0.0f, 1.0f, 0.0f);
// 		else
// 			glColor3f(1.0f, 0.0f, 0.0f);
// 
// 		// Increment pivot to change color next time
// 		iPivot++;
// 
// 		// Specify the next vertex for the triangle fan
// 		glVertex2f(x, y);
// 	}
// 
// 	// Done drawing fan for cone
// 	glEnd();
// 
// 
// 	// Begin a new triangle fan to cover the bottom
// 	glBegin(GL_TRIANGLE_FAN);
// 
// 	// Center of fan is at the origin
// 	glVertex2f(0.0f, 0.0f);
// 	for(angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI/8.0f))
// 	{
// 		// Calculate x and y position of the next vertex
// 		x = 50.0f*sin(angle);
// 		y = 50.0f*cos(angle);
// 
// 		// Alternate color between red and green
// 		if((iPivot %2) == 0)
// 			glColor3f(0.0f, 1.0f, 0.0f);
// 		else
// 			glColor3f(1.0f, 0.0f, 0.0f);
// 
// 		// Increment pivot to change color next time
// 		iPivot++;
// 
// 		// Specify the next vertex for the triangle fan
// 		glVertex2f(x, y);
// 	}
// 
// 	// Done drawing the fan that covers the bottom
// 	glEnd();
// 
// 	// Restore transformations
// 	glPopMatrix();
// 
// 
// 	// Flush drawing commands
// 	glutSwapBuffers();
// }
// 
// // This function does any needed initialization on the rendering
// // context. 
// void SetupRC()
// {
// 	// Black background
// 	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
// 
// 	// Set drawing color to green
// 	glColor3f(0.0f, 1.0f, 0.0f);
// 
// 	// Set color shading model to flat
// 	glShadeModel(GL_FLAT);
// 
// 	// Clock wise wound polygons are front facing, this is reversed
// 	// because we are using triangle fans
// 	glFrontFace(GL_CW);
// }
// 
// void SpecialKeys(int key, int x, int y)
// {
// 	if(key == GLUT_KEY_UP)
// 		xRot-= 5.0f;
// 
// 	if(key == GLUT_KEY_DOWN)
// 		xRot += 5.0f;
// 
// 	if(key == GLUT_KEY_LEFT)
// 		yRot -= 5.0f;
// 
// 	if(key == GLUT_KEY_RIGHT)
// 		yRot += 5.0f;
// 
// 	if(key > 356.0f)
// 		xRot = 0.0f;
// 
// 	if(key < -1.0f)
// 		xRot = 355.0f;
// 
// 	if(key > 356.0f)
// 		yRot = 0.0f;
// 
// 	if(key < -1.0f)
// 		yRot = 355.0f;
// 
// 	// Refresh the Window
// 	glutPostRedisplay();
// }
// 
// 
// void ChangeSize(int w, int h)
// {
// 	GLfloat nRange = 100.0f;
// 
// 	// Prevent a divide by zero
// 	if(h == 0)
// 		h = 1;
// 
// 	// Set Viewport to window dimensions
// 	glViewport(0, 0, w, h);
// 
// 	// Reset projection matrix stack
// 	glMatrixMode(GL_PROJECTION);
// 	glLoadIdentity();
// 
// 	// Establish clipping volume (left, right, bottom, top, near, far)
// 	if (w <= h) 
// 		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
// 	else 
// 		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
// 
// 	// Reset Model view matrix stack
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();
// }
// 
// int main(int argc, char* argv[])
// {
// 	glutInit(&argc, argv);
// 	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
// 	glutCreateWindow("Triangle Culling Example");
// 
// 	// Create the Menu
// 	glutCreateMenu(ProcessMenu);
// 	glutAddMenuEntry("Toggle depth test",1);
// 	glutAddMenuEntry("Toggle cull backface",2);
// 	glutAddMenuEntry("Toggle outline back",3);
// 	glutAttachMenu(GLUT_RIGHT_BUTTON);
// 
// 	glutReshapeFunc(ChangeSize);
// 	glutSpecialFunc(SpecialKeys);
// 	glutDisplayFunc(RenderScene);
// 	SetupRC();
// 	glutMainLoop();
// 
// 	return 0;
// }