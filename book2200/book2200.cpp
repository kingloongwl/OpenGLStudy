
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "GLTools.h"
#include "GLEXT.H"
#include "WGLEXT.H"

#include <fstream>


GLint windowWidth = 512;
GLint windowHeight = 512;

GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f};
GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f};
GLfloat noLight[]      = { 0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightPos[]     = { 100.0f, 300.0f, 100.0f, 1.0f};
GLfloat cameraPos[]    = { 100.0f, 150.0f, 200.0f, 1.0f};

PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB;
PFNGLSHADERSOURCEARBPROC			glShaderSourceARB;
PFNGLCOMPILESHADERARBPROC			glCompileShaderARB;
PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB;
PFNGLATTACHOBJECTARBPROC			glAttachObjectARB;
PFNGLLINKPROGRAMARBPROC				glLinkProgramARB;
PFNGLVALIDATEPROGRAMARBPROC			glValidateProgramARB;
PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgramObjectARB;
PFNGLGETINFOLOGARBPROC				glGetInfoLogARB;
PFNGLGETOBJECTPARAMETERIVARBPROC	glGetObjectParameterivARB;

#define FOG 1

#define MAX_INFO_LOG_SIZE 255

char vertexShaderFile[] = "../../book2200/225.vert";
char fragmentShaderFile[] = "../../book2200/225.frag";

void DrawModels(void)
{
    // Draw plane that the objects rest on
    glColor3f(0.0f, 0.0f, 0.90f); // Blue
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, -25.0f, -100.0f);
        glVertex3f(-100.0f, -25.0f, 100.0f);		
        glVertex3f(100.0f,  -25.0f, 100.0f);
        glVertex3f(100.0f,  -25.0f, -100.0f);
    glEnd();

    // Draw red cube
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(48.0f);

    // Draw green sphere
    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-60.0f, 0.0f, 0.0f);
    glutSolidSphere(25.0f, 50, 50);
    glPopMatrix();

    // Draw yellow cone
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(60.0f, 0.0f, -24.0f);
    glutSolidCone(25.0f, 50.0f, 50, 50);
    glPopMatrix();

    // Draw magenta torus
    glColor3f(1.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 60.0f);
    glutSolidTorus(8.0f, 16.0f, 50, 50);
    glPopMatrix();

    // Draw cyan octahedron
    glColor3f(0.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -60.0f);
    glScalef(25.0f, 25.0f, 25.0f);
    glutSolidOctahedron();
    glPopMatrix();
}

void RenderScene()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, 1.0f, 1.0f, 1000.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( cameraPos[0], cameraPos[1], cameraPos[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
	glViewport( 0, 0, windowWidth, windowHeight );
	
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	

	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	

	//glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
	//glEnable( GL_VERTEX_PROGRAM_POINT_SIZE_ARB );
	//glEnable( GL_POINT_SMOOTH );
	//glEnable( GL_BLEND );

#if FOG
	glEnable( GL_FOG );
#endif

	DrawModels();

	

	if ( glGetError() != GL_NO_ERROR )
		fprintf( stderr, "GL Error!\n" );
	
	glutSwapBuffers();
}

void SetupRC()
{
	if (
		!gltIsExtSupported( "GL_ARB_vertex_shader" ) ||
		!gltIsExtSupported("GL_ARB_fragment_shader") ||
		!gltIsExtSupported("GL_ARB_shader_objects") ||
		!gltIsExtSupported("GL_ARB_shading_language_100")
        )
    {
        fprintf(stderr, "GLSL extension not available!\n");
        Sleep(2000);
        exit(0);
    }

	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)gltGetExtensionPointer( "glCreateShaderObjectARB" );
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)gltGetExtensionPointer( "glShaderSourceARB" );
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)gltGetExtensionPointer( "glCompileShaderARB" );
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)gltGetExtensionPointer( "glCreateProgramObjectARB" );
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)gltGetExtensionPointer( "glAttachObjectARB" );
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)gltGetExtensionPointer( "glLinkProgramARB" );
	glValidateProgramARB = (PFNGLVALIDATEPROGRAMARBPROC)gltGetExtensionPointer( "glValidateProgramARB" );
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)gltGetExtensionPointer( "glUseProgramObjectARB" );
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)gltGetExtensionPointer( "glGetInfoLogARB" );
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)gltGetExtensionPointer( "glGetObjectParameterivARB" );
    
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glPolygonOffset( 4.0, 0.0f );



	GLhandleARB myVertexShader = glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );
	GLhandleARB myFragmentShader = glCreateShaderObjectARB( GL_FRAGMENT_SHADER_ARB );

	std::ifstream infile;
	infile.open( vertexShaderFile, std::ios::binary );
	if ( !infile )
	{
		return;
	}

	infile.seekg( 0, std::ios::end );
	int length = infile.tellg();
	char *vertexShaderText = new char[length + 1];
	infile.seekg( 0, std::ios::beg );
	infile.read( vertexShaderText, length );
	infile.close();
	vertexShaderText[length] = '\0';


	infile.open( fragmentShaderFile, std::ios::binary );
	if ( !infile )
	{
		return;
	}

	infile.seekg( 0, std::ios::end );
	length = infile.tellg();
	char *fragmentShaderText = new char[length + 1];
	infile.seekg( 0, std::ios::beg );
	infile.read( fragmentShaderText, length );
	infile.close();
	fragmentShaderText[length] = '\0';


	const GLcharARB *myStringPtrs[1];
	myStringPtrs[0] = vertexShaderText;
	glShaderSourceARB( myVertexShader, 1, myStringPtrs, NULL );
	myStringPtrs[0] = fragmentShaderText;
	glShaderSourceARB( myFragmentShader, 1, myStringPtrs, NULL );

	GLint success;
	glCompileShaderARB( myVertexShader );
	glGetObjectParameterivARB( myVertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success );
	if ( !success )
	{
		GLcharARB infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB( myVertexShader, MAX_INFO_LOG_SIZE, NULL, infoLog );
		fprintf( stderr, "Error in vertex shader compilation!\n" );
		fprintf( stderr, "Info log: %s\n", infoLog );
		Sleep( 2000 );
		exit( 0 );
	}

	glCompileShaderARB( myFragmentShader );
	glGetObjectParameterivARB( myFragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success );
	if ( !success )
	{
		GLcharARB infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB( myFragmentShader, MAX_INFO_LOG_SIZE, NULL, infoLog );
		fprintf( stderr, "Error in fragment shader compilation!\n" );
		fprintf( stderr, "Info log: %s\n", infoLog );
		Sleep( 2000 );
		exit( 0 );
	}


	GLhandleARB myProgram = glCreateProgramObjectARB();
	glAttachObjectARB( myProgram, myVertexShader );
	glAttachObjectARB( myProgram, myFragmentShader );

	glLinkProgramARB( myProgram );
	glGetObjectParameterivARB( myProgram, GL_OBJECT_COMPILE_STATUS_ARB, &success );
	if ( !success )
	{
		GLcharARB infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB( myProgram, MAX_INFO_LOG_SIZE, NULL, infoLog );
		fprintf( stderr, "Error in program linkage!\n" );
		fprintf( stderr, "Info log: %s\n", infoLog );
		Sleep( 2000 );
		exit( 0 );
	}

	glValidateProgramARB( myProgram );
	glGetObjectParameterivARB( myProgram, GL_OBJECT_COMPILE_STATUS_ARB, &success );
	if ( !success )
	{
		GLcharARB infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB( myProgram, MAX_INFO_LOG_SIZE, NULL, infoLog );
		fprintf( stderr, "Error in program validation!\n" );
		fprintf( stderr, "Info log: %s\n", infoLog );
		Sleep( 2000 );
		exit( 0 );
	}

	glUseProgramObjectARB( myProgram );
	
}

void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        cameraPos[0] -= 5.0f;
		break;
    case GLUT_KEY_RIGHT:
        cameraPos[0] += 5.0f;
		break;
    case GLUT_KEY_UP:
        cameraPos[2] -= 5.0f;
		break;
    case GLUT_KEY_DOWN:
        cameraPos[2] += 5.0f;
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
}

int main( int argc, char* argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( windowWidth, windowHeight );
	glutCreateWindow( "Vertex Shader" );
	glutReshapeFunc( ChangeSize );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );

	SetupRC();

	glutMainLoop();
	
	return 0;
}
