
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "GLTools.h"
#include "GLEXT.H"
#include "WGLEXT.H"
PFNGLBEGINQUERYPROC        glBeginQuery;
PFNGLDELETEQUERIESPROC     glDeleteQueries;
PFNGLENDQUERYPROC          glEndQuery;
PFNGLGENQUERIESPROC        glGenQueries;
PFNGLGETQUERYIVPROC        glGetQueryiv;
PFNGLGETQUERYOBJECTIVPROC  glGetQueryObjectiv;

GLint windowWidth = 512;                // window size
GLint windowHeight = 512;

GLint mainMenu, bboxMenu;

GLboolean occlusionDetection = GL_TRUE;
GLboolean showBoundingVolume = GL_FALSE;
GLint boundingVolume = 0;
GLuint queryIDs[27];

GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f};
GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f};
GLfloat lightPos[]     = { 100.0f, 300.0f, 100.0f, 1.0f};
GLfloat cameraPos[]    = { 200.0f, 300.0f, 400.0f, 1.0f};

void DrawOccluder()
{
	glColor3f( 0.5f, 0.25f, 0.0f );
	
	glPushMatrix();
	glScalef( 30.0f, 30.0f, 1.0f );
	glTranslatef( 0.0f, 0.0f, 50.0f );
	glutSolidCube( 10.0f );
	glTranslatef( 0.0f, 0.0f, -100.0f );
	glutSolidCube( 10.0f );
	glPopMatrix();
	
	glPushMatrix();
    glScalef(1.0f, 30.0f, 30.0f);
    glTranslatef(50.0f, 0.0f, 0.0f);
    glutSolidCube(10.0f);
    glTranslatef(-100.0f, 0.0f, 0.0f);
    glutSolidCube(10.0f);
    glPopMatrix();

    glPushMatrix();
    glScalef(30.0f, 1.0f, 30.0f);
    glTranslatef(0.0f, 50.0f, 0.0f);
    glutSolidCube(10.0f);
    glTranslatef(0.0f, -100.0f, 0.0f);
    glutSolidCube(10.0f);
    glPopMatrix();
}

void DrawSphere( GLint sphereNum )
{
	GLboolean occluded = GL_FALSE;
	
	if ( occlusionDetection )
	{
		GLint passingSamples;
		
		glGetQueryObjectiv( queryIDs[sphereNum], GL_QUERY_RESULT, &passingSamples );
		
		if ( passingSamples == 0 )
			occluded = GL_TRUE;
	}
	
	if ( !occluded )
	{
		glutSolidSphere( 50.0f, 100, 100 );
	}
}

void DrawModels()
{
	GLint r, g, b;
	
	if ( occlusionDetection || showBoundingVolume )
	{
		DrawOccluder();
		
		glShadeModel( GL_FLAT );
		glDisable( GL_LIGHTING );
		glDisable( GL_COLOR_MATERIAL );
		glDisable( GL_NORMALIZE );
		
		if ( !showBoundingVolume )
			glColorMask( 0, 0, 0, 0 );
		
		for( r = 0; r < 3; r++ )
		{
			for( g = 0; g < 3; g++ )
			{
				for( b = 0; b < 3; b++ )
				{
					if ( showBoundingVolume )
						glColor3f( r*0.5f, g*0.5f, b*0.5f );
					
					glPushMatrix();
					glTranslatef( 100.0f * r - 100.0f, 100.0f * g - 100.0f, 100.0f * b - 100.0f );
					glBeginQuery( GL_SAMPLES_PASSED, queryIDs[(r*9) + (g*3) + b] );
					
					switch( boundingVolume )
					{
					case 0:
						glutSolidCube( 100.0f );
						break;
					case 1:
						glScalef( 150.0f, 150.0f, 150.0f );
						glutSolidTetrahedron();
						break;
					case 2:
						glScalef( 90.0f, 90.0f, 90.0f );
						glutSolidOctahedron();
						break;
					case 3:
						glScalef( 40.0f, 40.0f, 40.0f );
						glutSolidDodecahedron();
						break;
					case 4:
						glScalef( 65.0f, 65.0f, 65.0f );
						glutSolidIcosahedron();
						break;
					}
					
					glEndQuery( GL_SAMPLES_PASSED );
					glPopMatrix();
				}
			}
		}
		
		if ( !showBoundingVolume )
			glClear( GL_DEPTH_BUFFER_BIT );
		
		glShadeModel( GL_SMOOTH );
		glEnable( GL_LIGHTING );
		glEnable( GL_COLOR_MATERIAL );
		glEnable( GL_NORMALIZE );
		glColorMask( 1, 1, 1, 1 );
	}
	
	DrawOccluder();
	
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );
	

	for( r = 0; r < 3; r++ )
	{
		for( g = 0; g < 3; g++ )
		{
			for( b = 0; b < 3; b++ )
			{
				glColor3f( r * 0.5f, g * 0.5f, b * 0.5f );
				glPushMatrix();
				glTranslatef( 100.0f * r - 100.0f, 100.0f * g - 100.0f, 100.0f * b - 100.0f );
				DrawSphere( (r*9)+(g*3)+b );
				glPopMatrix();
			}
			
		}
		
	}
	
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_TEXTURE_GEN_S );
	glDisable( GL_TEXTURE_GEN_T );
	
}

void RenderScene()
{
   static GLTStopwatch stopWatch;
    static int frameCounter = 0;
    
    // Get initial time
    if (frameCounter == 0)
        gltStopwatchReset(&stopWatch);


    frameCounter++;
    if (frameCounter == 100)
    {
        frameCounter = 0;
        fprintf(stdout, "FPS: %f\n", 100.0f / gltStopwatchRead(&stopWatch));
        gltStopwatchReset(&stopWatch);
    }

    // Track camera angle
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 10.0f, 10000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 
              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw objects in the scene
    DrawModels();

    if (glGetError() != GL_NO_ERROR)
    {
        fprintf(stderr, "GL Error!\n");
        Sleep(5000);
    }

    // Flush drawing commands
    glutSwapBuffers();

    glutPostRedisplay();
}

void SetupRC()
{
	const GLubyte *version;
	GLboolean glVersion15 = GL_FALSE;
	GLint queryCounterBits;
	
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
     // Load the function pointers
    if (glVersion15)
    {
		glBeginQuery = (PFNGLBEGINQUERYPROC)gltGetExtensionPointer( "glBeginQuery" );
		glDeleteQueries = (PFNGLDELETEQUERIESPROC)gltGetExtensionPointer( "glDeleteQueries" );
		glEndQuery = (PFNGLENDQUERYPROC)gltGetExtensionPointer( "glEndQuery" );
		glGenQueries = (PFNGLGENQUERIESPROC)gltGetExtensionPointer( "glGenQueries" );
		glGetQueryiv = (PFNGLGETQUERYIVPROC)gltGetExtensionPointer( "glGetQueryiv" );
		glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)gltGetExtensionPointer( "glGetQueryObjectiv" );
    }
    else
    {
		glBeginQuery = (PFNGLBEGINQUERYPROC)gltGetExtensionPointer( "glBeginQueryARB" );
		glDeleteQueries = (PFNGLDELETEQUERIESPROC)gltGetExtensionPointer( "glDeleteQueriesARB" );
		glEndQuery = (PFNGLENDQUERYPROC)gltGetExtensionPointer( "glEndQueryARB" );
		glGenQueries = (PFNGLGENQUERIESPROC)gltGetExtensionPointer( "glGenQueriesARB" );
		glGetQueryiv = (PFNGLGETQUERYIVPROC)gltGetExtensionPointer( "glGetQueryivARB" );
		glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)gltGetExtensionPointer( "glGetQueryObjectivARB" );
    }

    if (!glBeginQuery || !glDeleteQueries || !glEndQuery || !glGenQueries ||
        !glGetQueryiv || !glGetQueryObjectiv)
    {
        fprintf(stderr, "Not all entrypoints were available!\n");
        Sleep(2000);
        exit(0);
    }

    // Make sure query counter bits is non-zero
    glGetQueryiv(GL_SAMPLES_PASSED, GL_QUERY_COUNTER_BITS, &queryCounterBits);
    if (queryCounterBits == 0)
    {
        fprintf(stderr, "Occlusion queries not really supported!\n");
        fprintf(stderr, "Available query counter bits: 0\n");
        Sleep(2000);
        exit(0);
    }
    
    fprintf(stdout, "Controls:\n");
    fprintf(stdout, "\tRight-click for menu\n\n");
    fprintf(stdout, "\tx/X\t\tMove +/- in x direction\n");
    fprintf(stdout, "\ty/Y\t\tMove +/- in y direction\n");
    fprintf(stdout, "\tz/Z\t\tMove +/- in z direction\n\n");
    fprintf(stdout, "\tq\t\tExit demo\n\n");
    
    // Generate occlusion query names
    glGenQueries(27, queryIDs);
    
    // Black background
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f );

    // Hidden surface removal
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set up some lighting state that never changes
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Set up texturing for spheres
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    {
        GLfloat p[4] = {1.0f/50.0f, 0.0f, 0.0f, -0.5f};
        GLint w, h, c;
        GLenum format;
        GLbyte *texels = gltLoadTGA("logo.tga", &w, &h, &c, &format);
        glTexImage2D(GL_TEXTURE_2D, 0, c, w, h, 0, format, GL_UNSIGNED_BYTE, texels);
        free(texels);
        glTexGenfv(GL_S, GL_OBJECT_PLANE, p);
        p[0] = 0.0f;
        p[1] = 1.0f/50.0f;
        glTexGenfv(GL_T, GL_OBJECT_PLANE, p);
    }
	
}
void ProcessMenu(int value)
{
    switch(value)
    {
    case 1:
        occlusionDetection = !occlusionDetection;
        if (occlusionDetection)
        {
            glutChangeToMenuEntry(1, "Toggle occlusion query (currently ON)", 1);
        }
        else
        {
            glutChangeToMenuEntry(1, "Toggle occlusion query (currently OFF)", 1);
        }
        break;

    case 2:
        showBoundingVolume = !showBoundingVolume;
        if (showBoundingVolume)
        {
            glutChangeToMenuEntry(2, "Toggle bounding volume (currently ON)", 2);
        }
        else
        {
            glutChangeToMenuEntry(2, "Toggle bounding volume (currently OFF)", 2);
        }
        break;

    default:
        boundingVolume = value - 3;
        glutSetMenu(mainMenu);
        switch (boundingVolume)
        {
        case 0:
            glutChangeToSubMenu(3, "Choose bounding volume (currently BOX)", bboxMenu);
            break;
        case 1:
            glutChangeToSubMenu(3, "Choose bounding volume (currently TETRAHEDRON)", bboxMenu);
            break;
        case 2:
            glutChangeToSubMenu(3, "Choose bounding volume (currently OCTAHEDRON)", bboxMenu);
            break;
        case 3:
            glutChangeToSubMenu(3, "Choose bounding volume (currently DODECAHEDRON)", bboxMenu);
            break;
        case 4:
            glutChangeToSubMenu(3, "Choose bounding volume (currently ICOSAHEDRON)", bboxMenu);
            break;
        }
        break;
    }

    // Refresh the Window
    glutPostRedisplay();
}

void KeyPressFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'b':
    case 'B':
        boundingVolume = (boundingVolume+1)%5;
        break;
    case 's':
    case 'S':
        showBoundingVolume = !showBoundingVolume;
        break;
    case 'o':
    case 'O':
        occlusionDetection = !occlusionDetection;
        break;
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
	glutCreateWindow( "Occlusion Query Demo" );
	glutReshapeFunc( ChangeSize );
	glutKeyboardFunc( KeyPressFunc );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );

	bboxMenu = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("BOX (CUBE)", 3);
    glutAddMenuEntry("TETRAHEDRON", 4);
    glutAddMenuEntry("OCTAHEDRON", 5);
    glutAddMenuEntry("DODECAHEDRON", 6);
    glutAddMenuEntry("ICOSAHEDRON", 7);

    mainMenu = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Toggle occlusion query (currently ON)", 1);
    glutAddMenuEntry("Show bounding volume (currently OFF)", 2);
    glutAddSubMenu("Choose bounding volume (currently BOX)", bboxMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
	

	SetupRC();

	glutMainLoop();
	
	if (glDeleteQueries)
        glDeleteQueries(27, queryIDs);
	return 0;
}
