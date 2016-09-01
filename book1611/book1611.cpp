
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "GLTools.h"
#include "GLEXT.H"
#include "WGLEXT.H"
PFNGLBINDBUFFERPROC    glBindBuffer;
PFNGLBUFFERDATAPROC    glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGENBUFFERSPROC    glGenBuffers;
PFNGLMAPBUFFERPROC     glMapBuffer;
PFNGLUNMAPBUFFERPROC   glUnmapBuffer;

GLint windowWidth = 512;                // window size
GLint windowHeight = 512;

GLuint bufferID;

GLboolean useBufferObject = GL_TRUE;
GLboolean mapBufferObject = GL_TRUE;
GLboolean animating =       GL_TRUE;

GLint numSphereVertices = 900000;
GLfloat *sphereVertexArray = NULL;

GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f};
GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f};
GLfloat cameraPos[]    = { 400.0f, 400.0f, 400.0f, 1.0f};

void RegenerateSphere()
{
	GLint i;
	
	if ( mapBufferObject && useBufferObject )
	{
		if( sphereVertexArray )
			free( sphereVertexArray );
		
		glBindBuffer( GL_ARRAY_BUFFER, bufferID );
		
		
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * numSphereVertices * 3, NULL,
						animating ? GL_STREAM_DRAW : GL_STATIC_DRAW );
						
		sphereVertexArray = (GLfloat*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
		
		if( !sphereVertexArray )
		{
			fprintf( stderr, "Unable to map buffer object!\n" );
			Sleep( 2000 );
			exit( 0 );
		}
	}
	
	else if( !sphereVertexArray )
	{
		sphereVertexArray = (GLfloat*)malloc( sizeof(GLfloat) * numSphereVertices * 3 );
		
		if ( !sphereVertexArray )
		{
			fprintf( stderr, "Unable to allocate memory for vertex arrays!\n" );
			Sleep( 2000 );
			exit( 0 );
		}
	}
	
	for( i = 0; i < numSphereVertices; i++ )
	{
		GLfloat r1, r2, r3, scaleFactor;
		
		r1 = (GLfloat)(rand() - (RAND_MAX/2) );
		r2 = (GLfloat)(rand() - (RAND_MAX/2) );
		r3 = (GLfloat)(rand() - (RAND_MAX/2) );
		
		scaleFactor = 1.0f / sqrt( r1 * r1 + r2 * r2 + r3 * r3 );
		
		sphereVertexArray[(i*3)+0] = r1 * scaleFactor;
		sphereVertexArray[(i*3)+1] = r2 * scaleFactor;
		sphereVertexArray[(i*3)+2] = r3 * scaleFactor;
		
	}
	
	
	if ( mapBufferObject && useBufferObject )
	{
		if ( !glUnmapBuffer(GL_ARRAY_BUFFER) )
		{
			RegenerateSphere();
		}
		
		sphereVertexArray = NULL;
	}
}

void SetRenderingMethod()
{
	if ( useBufferObject )
	{
		glBindBuffer( GL_ARRAY_BUFFER, bufferID );
		
		glNormalPointer( GL_FLOAT, 0, 0 );
		
		glVertexPointer( 3, GL_FLOAT, 0, 0 );
		
		if ( !mapBufferObject )
		{
			if ( animating )
			{
				glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * numSphereVertices * 3, sphereVertexArray );
			}
			else
			{
				glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat) * numSphereVertices * 3, sphereVertexArray, GL_STATIC_DRAW );
			}
		}
	}
	else
	{
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glNormalPointer( GL_FLOAT, 0, sphereVertexArray );
		glVertexPointer( 3, GL_FLOAT, 0, sphereVertexArray );
	}
}

void DrawModels()
{
	GLint r, g, b;
	
	for( r = 0; r < 3; r++ )
	{
		for( g = 0; g < 3; g++ )
		{
			for( b = 0; b < 3; b++ )
			{
				glColor3f( r*0.5f, g*0.5f, b*0.5f );
				
				glPushMatrix();
				glTranslatef( 100.0f * r - 100.0f, 100.0f * g - 100.0f, 100.0f * b - 100.0f );
				glScalef( 50.0f, 50.0f, 50.0f );
				glDrawArrays( GL_POINTS, 0, numSphereVertices );
				glPopMatrix();
			}
		}
	}
}

void RenderScene()
{
	static GLTStopwatch stopWatch;
	static int frameCounter = 0;
	
	if ( frameCounter == 0 )
		gltStopwatchReset( &stopWatch );
	
	frameCounter++;
	if ( frameCounter == 100 )
	{
		frameCounter = 0;
		fprintf( stdout, "FPS: %f\n", 100.0f / gltStopwatchRead( &stopWatch ));
		gltStopwatchReset( &stopWatch );
	}
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, 1.0f, 10.0f, 10000.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( cameraPos[0], cameraPos[1], cameraPos[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	if ( animating )
	{
		RegenerateSphere();
		SetRenderingMethod();
	}
	
	DrawModels();
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void SetupRC()
{
	const GLubyte *version;
	GLboolean glVersion15 = GL_FALSE;
	
	fprintf( stdout, "Buffer Object Demo\n\n" );
	
	version = glGetString( GL_VERSION );
	if ( (version[0] == '1') && (version[1] == '.') &&
		 (version[2] >= '5') && (version[2] <= '9') )
	{
		glVersion15 = GL_TRUE;
	}
	
	    if (!glVersion15 && !gltIsExtSupported("GL_ARB_vertex_buffer_object"))
    {
        fprintf(stderr, "Neither OpenGL 1.5 nor GL_ARB_vertex_buffer_object"
                        " extension is available!\n");
        Sleep(2000);
        exit(0);
    }

    // Load the function pointers
    if (glVersion15)
    {
		glBindBuffer = (PFNGLBINDBUFFERPROC)gltGetExtensionPointer( "glBindBuffer" );
		glBufferData = (PFNGLBUFFERDATAPROC)gltGetExtensionPointer( "glBufferData" );
		glBufferSubData = (PFNGLBUFFERSUBDATAPROC)gltGetExtensionPointer( "glBufferSubData" );
		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)gltGetExtensionPointer( "glDeleteBuffers" );
		glGenBuffers = (PFNGLGENBUFFERSPROC)gltGetExtensionPointer( "glGenBuffers" );
		glMapBuffer = (PFNGLMAPBUFFERPROC)gltGetExtensionPointer( "glMapBuffer" );
		glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)gltGetExtensionPointer( "glUnmapBuffer" );
    }
    else
    {
		glBindBuffer = (PFNGLBINDBUFFERPROC)gltGetExtensionPointer( "glBindBufferARB" );
		glBufferData = (PFNGLBUFFERDATAPROC)gltGetExtensionPointer( "glBufferDataARB" );
		glBufferSubData = (PFNGLBUFFERSUBDATAPROC)gltGetExtensionPointer( "glBufferSubDataARB" );
		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)gltGetExtensionPointer( "glDeleteBuffersARB" );
		glGenBuffers = (PFNGLGENBUFFERSPROC)gltGetExtensionPointer( "glGenBuffersARB" );
		glMapBuffer = (PFNGLMAPBUFFERPROC)gltGetExtensionPointer( "glMapBufferARB" );
		glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)gltGetExtensionPointer( "glUnmapBufferARB" );
    }

    if (!glBindBuffer || !glBufferData || !glBufferSubData || 
        !glDeleteBuffers || !glGenBuffers || !glMapBuffer || !glUnmapBuffer)
    {
        fprintf(stderr, "Not all entrypoints were available!\n");
        Sleep(2000);
        exit(0);
    }
	
	sphereVertexArray = (GLfloat*)malloc(sizeof(GLfloat) * numSphereVertices * 3 );
	
	if ( !sphereVertexArray )
	{
		fprintf(stderr, "Unable to allocate system memory for vertex arrays!");
        Sleep(2000);
        exit(0);
	}
	
	fprintf(stdout, "Controls:\n");
    fprintf(stdout, "\tRight-click for menu\n\n");
    fprintf(stdout, "\tx/X\t\tMove +/- in x direction\n");
    fprintf(stdout, "\ty/Y\t\tMove +/- in y direction\n");
    fprintf(stdout, "\tz/Z\t\tMove +/- in z direction\n\n");
    fprintf(stdout, "\tq\t\tExit demo\n\n");
	
	glGenBuffers( 1, &bufferID );
	
	glBindBuffer( GL_ARRAY_BUFFER, bufferID );
	glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat) * numSphereVertices * 3, NULL, GL_STATIC_DRAW );
	
	RegenerateSphere();
	SetRenderingMethod();
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_VERTEX_ARRAY );
	
	glClearColor( 0.3f, 0.3f, 0.3f, 1.0f );
	
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
	
	glShadeModel( GL_SMOOTH );
	glEnable( GL_LIGHTING );
	glEnable( GL_COLOR_MATERIAL );
	glEnable( GL_NORMALIZE );
	glEnable( GL_LIGHT0 );
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	
}

void ProcessMenu( int value )
{
	switch(value)
	{
	case 1:
		useBufferObject = !useBufferObject;
		RegenerateSphere();
		SetRenderingMethod();
		if ( useBufferObject )
		{
			glutChangeToMenuEntry( 1, "Toggle buffer object usage (currently ON)", 1 );
		}
		else
		{
			glutChangeToMenuEntry( 1, "Toggle buffer object usage (currently OFF)", 1 );
		}
	    break;
	case 2:
		mapBufferObject = !mapBufferObject;
		RegenerateSphere();
		SetRenderingMethod();
		if ( mapBufferObject )
		{
			glutChangeToMenuEntry( 2, "Toggle mapped buffer object (currently ON)", 2 );
		}
		else
		{
			glutChangeToMenuEntry( 2, "Toggle mapped buffer object (currently OFF)", 2 );
		}
		break;
	case 3:
		animating = !animating;
		if ( animating )
		{
			glutChangeToMenuEntry( 3, "Toggle animation (currently ON)", 3 );
			
			glBindBuffer( GL_ARRAY_BUFFER, bufferID );
			glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat) * numSphereVertices * 3, NULL, GL_STREAM_DRAW );
		}
		else
		{
			glutChangeToMenuEntry( 3, "Toggle animation (currently OFF)", 3 );
			glBindBuffer( GL_ARRAY_BUFFER, bufferID );
			glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat) * numSphereVertices * 3, NULL, GL_STATIC_DRAW );
			RegenerateSphere();
			SetRenderingMethod();
		}
		break;
	default:
		break;
	}
	
	glutPostRedisplay();
}

void KeyPressFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'x':
        cameraPos[0] += 5.0f;
        break;
    case 'X':
        cameraPos[0] -= 5.0f;
        break;
    case 'y':
        cameraPos[1] += 5.0f;
        break;
    case 'Y':
        cameraPos[1] -= 5.0f;
        break;
    case 'z':
        cameraPos[2] += 5.0f;
        break;
    case 'Z':
        cameraPos[2] -= 5.0f;
        break;
    case 'q':
    case 'Q':
    case 27 : /* ESC */
        exit(0);
    }

    // Refresh the Window
    glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        cameraPos[0] += 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        cameraPos[0] -= 5.0f;
        break;
    case GLUT_KEY_UP:
        cameraPos[1] += 5.0f;
        break;
    case GLUT_KEY_DOWN:
        cameraPos[1] -= 5.0f;
        break;
    default:
        break;
    }

    // Refresh the Window
    glutPostRedisplay();
}

void ChangeSize(int w, int h)
{
    windowWidth = w;
    windowHeight = h;

    glViewport(0, 0, windowWidth, windowHeight);
}

int main( int argc, char* argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( windowWidth, windowHeight );
	glutCreateWindow( "Buffer Object Demo" );
	glutReshapeFunc( ChangeSize );
	glutKeyboardFunc( KeyPressFunc );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );

	glutCreateMenu( ProcessMenu );
	glutAddMenuEntry("Toggle buffer object usage (currently ON)", 1);
    glutAddMenuEntry("Toggle mapped buffer object (currently ON)", 2);
    glutAddMenuEntry("Toggle animation (currently ON)", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
	

	SetupRC();

	glutMainLoop();
	
	if ( sphereVertexArray )
		free( sphereVertexArray );
	if ( glDeleteBuffers )
		glDeleteBuffers( 1, &bufferID );
	return 0;
}
