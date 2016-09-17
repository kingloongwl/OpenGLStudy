
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

#ifndef __APPLE__
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
PFNGLBINDPROGRAMARBPROC glBindProgramARB;
PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
#endif

PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
PFNGLGETHANDLEARBPROC glGetHandleARB;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
PFNGLUNIFORM1FARBPROC glUniform1fARB;
PFNGLUNIFORM1IARBPROC glUniform1iARB;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;

GLboolean lowLevelAvailable = GL_FALSE;
GLboolean highLevelAvailable = GL_FALSE;
GLboolean useHighLevel = GL_FALSE;

#define CHECKERBOARD      0
#define BEACHBALL         1
#define TOYBALL           2
#define TOTAL_SHADER_SETS 3

GLuint v_ids[TOTAL_SHADER_SETS];        // low-level shader object names
GLuint f_ids[TOTAL_SHADER_SETS];
GLhandleARB fShader[TOTAL_SHADER_SETS]; // high-level shader object handles
GLhandleARB vShader[TOTAL_SHADER_SETS];
GLhandleARB progObj[TOTAL_SHADER_SETS];
GLboolean needsValidation[TOTAL_SHADER_SETS];
char *shaderNames[TOTAL_SHADER_SETS] = { "2341", "2342", "2343" };

GLint whichShader = CHECKERBOARD;       // current shader

#define LIGHTPOS_SLOT  0                // locations for low-level program local parameters

GLint windowWidth = 512;                // window size
GLint windowHeight = 512;

GLint mainMenu, shaderMenu;             // menu handles

GLfloat cameraPos[] = { 50.0f, 50.0f, 150.0f, 1.0f };
GLfloat lightPos[] = { 140.0f, 250.0f, 140.0f, 1.0f };

GLfloat lightRotation = 0.0f;
GLint tess = 75;

#define MAX_INFO_LOG_SIZE 2048

// Load shader from disk into a null-terminated string
GLubyte *LoadShaderText( const char *fileName )
{
	GLubyte *shaderText = NULL;
	GLint shaderLength = 0;
	FILE *fp;

	fp = fopen( fileName, "r" );
	if ( fp != NULL )
	{
		while ( fgetc( fp ) != EOF )
		{
			shaderLength++;
		}
		rewind( fp );
		shaderText = (GLubyte *)malloc( shaderLength + 1 );
		if ( shaderText != NULL )
		{
			fread( shaderText, 1, shaderLength, fp );
		}
		shaderText[shaderLength] = '\0';
		fclose( fp );
	}

	return shaderText;
}

// Compile shaders
void PrepareShader( GLint shaderNum )
{
	char fullFileName[255];
	GLubyte *shString;

	// Create low-level shader objects and specify shader text
	if ( lowLevelAvailable )
	{
		GLint errorPos;

#ifdef __APPLE__
		sprintf( fullFileName, "%s.vp", shaderNames[shaderNum] );
#else
		sprintf( fullFileName, "..\\..\\book2340\\%s.vert", shaderNames[shaderNum] );
#endif
		shString = LoadShaderText( fullFileName );
		if ( !shString )
		{
			fprintf( stderr, "Unable to load \"%s\"\n", fullFileName );
			Sleep( 5000 );
			exit( 0 );
		}
		glBindProgramARB( GL_VERTEX_PROGRAM_ARB, v_ids[shaderNum] );
		glProgramStringARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, strlen( (char*)shString ), shString );
		free( shString );
		glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB, &errorPos );
		if ( errorPos != -1 )
		{
			fprintf( stderr, "Error in low-level vertex shader #%d at position %d!\n", shaderNum, errorPos );
			fprintf( stderr, "Error string: %s\n", glGetString( GL_PROGRAM_ERROR_STRING_ARB ) );
			Sleep( 5000 );
			exit( 0 );
		}

#ifdef __APPLE__
		sprintf( fullFileName, "%s.fp", shaderNames[shaderNum] );
#else
		sprintf( fullFileName, "..\\..\\book2340\\%s.frag", shaderNames[shaderNum] );
#endif
		shString = LoadShaderText( fullFileName );
		if ( !shString )
		{
			fprintf( stderr, "Unable to load \"%s\"\n", fullFileName );
			Sleep( 5000 );
			exit( 0 );
		}
		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, f_ids[shaderNum] );
		glProgramStringARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, strlen( (char*)shString ), shString );
		free( shString );
		glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB, &errorPos );
		if ( errorPos != -1 )
		{
			fprintf( stderr, "Error in low-level fragment shader #%d at position %d!\n", shaderNum, errorPos );
			fprintf( stderr, "Error string: %s\n", glGetString( GL_PROGRAM_ERROR_STRING_ARB ) );
			Sleep( 5000 );
			exit( 0 );
		}
	}

	// Create high-level shader objects and specify shader text
	if ( highLevelAvailable )
	{
		const GLcharARB *shStringPtr[1];
		GLint success;

#ifdef __APPLE__
		sprintf( fullFileName, "%s.vs", shaderNames[shaderNum] );
#else
		sprintf( fullFileName, "..\\..\\book2340\\%s.vert", shaderNames[shaderNum] );
#endif
		shString = LoadShaderText( fullFileName );
		if ( !shString )
		{
			fprintf( stderr, "Unable to load \"%s\"\n", fullFileName );
			Sleep( 5000 );
			exit( 0 );
		}
		vShader[shaderNum] = glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );
		shStringPtr[0] = (GLcharARB*)shString;
		glShaderSourceARB( vShader[shaderNum], 1, shStringPtr, NULL );
		free( shString );

		// Compile shaders and check for any errors
		glCompileShaderARB( vShader[shaderNum] );
		glGetObjectParameterivARB( vShader[shaderNum], GL_OBJECT_COMPILE_STATUS_ARB, &success );
		if ( !success )
		{
			GLcharARB infoLog[MAX_INFO_LOG_SIZE];
			glGetInfoLogARB( vShader[shaderNum], MAX_INFO_LOG_SIZE, NULL, infoLog );
			fprintf( stderr, "Error in high-level vertex shader #%d compilation!\n", shaderNum );
			fprintf( stderr, "Info log: %s\n", infoLog );
			Sleep( 10000 );
			exit( 0 );
		}

#ifdef __APPLE__
		sprintf( fullFileName, "%s.fs", shaderNames[shaderNum] );
#else
		sprintf( fullFileName, "..\\..\\book2340\\%s.frag", shaderNames[shaderNum] );
#endif
		shString = LoadShaderText( fullFileName );
		if ( !shString )
		{
			fprintf( stderr, "Unable to load \"%s\"\n", fullFileName );
			Sleep( 5000 );
			exit( 0 );
		}
		fShader[shaderNum] = glCreateShaderObjectARB( GL_FRAGMENT_SHADER_ARB );
		shStringPtr[0] = (GLcharARB*)shString;
		glShaderSourceARB( fShader[shaderNum], 1, shStringPtr, NULL );
		free( shString );

		// Compile shaders and check for any errors
		glCompileShaderARB( fShader[shaderNum] );
		glGetObjectParameterivARB( fShader[shaderNum], GL_OBJECT_COMPILE_STATUS_ARB, &success );
		if ( !success )
		{
			GLcharARB infoLog[MAX_INFO_LOG_SIZE];
			glGetInfoLogARB( fShader[shaderNum], MAX_INFO_LOG_SIZE, NULL, infoLog );
			fprintf( stderr, "Error in high-level fragment shader #%d compilation!\n", shaderNum );
			fprintf( stderr, "Info log: %s\n", infoLog );
			Sleep( 10000 );
			exit( 0 );
		}
		else
		{
			GLcharARB infoLog[MAX_INFO_LOG_SIZE];
			glGetInfoLogARB( fShader[shaderNum], MAX_INFO_LOG_SIZE, NULL, infoLog );
			//fprintf(stderr, "High-level fragment shader #%d info log: %s\n", shaderNum, infoLog);
		}

		// Create program object, attach shader, then link
		progObj[shaderNum] = glCreateProgramObjectARB();
		glAttachObjectARB( progObj[shaderNum], vShader[shaderNum] );
		glAttachObjectARB( progObj[shaderNum], fShader[shaderNum] );

		glLinkProgramARB( progObj[shaderNum] );
		glGetObjectParameterivARB( progObj[shaderNum], GL_OBJECT_LINK_STATUS_ARB, &success );
		if ( !success )
		{
			GLcharARB infoLog[MAX_INFO_LOG_SIZE];
			glGetInfoLogARB( progObj[shaderNum], MAX_INFO_LOG_SIZE, NULL, infoLog );
			fprintf( stderr, "Error in high-level program #%d linkage!\n", shaderNum );
			fprintf( stderr, "Info log: %s\n", infoLog );
			Sleep( 10000 );
			exit( 0 );
		}
		else
		{
			GLcharARB infoLog[MAX_INFO_LOG_SIZE];
			glGetInfoLogARB( progObj[shaderNum], MAX_INFO_LOG_SIZE, NULL, infoLog );
			//fprintf(stderr, "High-level program #%d info log: %s\n", shaderNum, infoLog);
		}

		// Program object has changed, so we should revalidate
		needsValidation[shaderNum] = GL_TRUE;
	}
}

// Called to draw scene objects
void DrawModels( void )
{
	GLTVector3 lightPosEye;
	GLTMatrix mv;

	// Transform light position to eye space
	glPushMatrix();
	glRotatef( lightRotation, 0.0, 1.0, 0.0 );
	glGetFloatv( GL_MODELVIEW_MATRIX, mv );
	gltTransformPoint( lightPos, mv, lightPosEye );
	glPopMatrix();

	if ( useHighLevel )
	{
		GLint uniformLoc = glGetUniformLocationARB( progObj[whichShader], "lightPos" );
		if ( uniformLoc != -1 )
		{
			glUniform3fvARB( uniformLoc, 1, lightPosEye );
		}
	}
	else
	{
		glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, LIGHTPOS_SLOT, lightPosEye );
	}

	// Draw sphere
	glutSolidSphere( 50.0f, tess, tess );
}

// Called to draw scene
void RenderScene( void )
{
	// Track camera angle
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, 1.0f, 1.0f, 1000.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( cameraPos[0], cameraPos[1], cameraPos[2],
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	// Clear the window with current clearing color
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Validate our shader before first use
	if ( needsValidation[whichShader] )
	{
		GLint success;

		glValidateProgramARB( progObj[whichShader] );
		glGetObjectParameterivARB( progObj[whichShader], GL_OBJECT_VALIDATE_STATUS_ARB, &success );
		if ( !success )
		{
			GLcharARB infoLog[MAX_INFO_LOG_SIZE];
			glGetInfoLogARB( progObj[whichShader], MAX_INFO_LOG_SIZE, NULL, infoLog );
			fprintf( stderr, "Error in program #%d validation!\n", whichShader );
			fprintf( stderr, "Info log: %s\n", infoLog );
			Sleep( 10000 );
			exit( 0 );
		}

		needsValidation[whichShader] = GL_FALSE;
	}

	// Draw objects in the scene
	DrawModels();

	if ( glGetError() != GL_NO_ERROR )
		fprintf( stderr, "GL Error!\n" );

	// Flush drawing commands
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	GLint i;

	fprintf( stdout, "Procedural Texture Mapping Demo\n\n" );

	// Make sure required functionality is available!
	if ( gltIsExtSupported( "GL_ARB_fragment_shader" ) &&
		gltIsExtSupported( "GL_ARB_vertex_shader" ) &&
		gltIsExtSupported( "GL_ARB_shader_objects" ) &&
		gltIsExtSupported( "GL_ARB_shading_language_100" ) )
	{
		highLevelAvailable = GL_TRUE;
	}

	if ( !lowLevelAvailable && !highLevelAvailable )
	{
		fprintf( stderr, "Neither set of shader"
			" extensions is available!\n" );
		Sleep( 2000 );
		exit( 0 );
	}

#ifndef __APPLE__
	if ( lowLevelAvailable )
	{
		glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC)gltGetExtensionPointer( "glGenProgramsARB" );
		glBindProgramARB = (PFNGLBINDPROGRAMARBPROC)gltGetExtensionPointer( "glBindProgramARB" );
		glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC)gltGetExtensionPointer( "glProgramStringARB" );
		glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARBPROC)gltGetExtensionPointer( "glDeleteProgramsARB" );
		glProgramLocalParameter4fARB = (PFNGLPROGRAMLOCALPARAMETER4FARBPROC)gltGetExtensionPointer( "glProgramLocalParameter4fARB" );
		glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)gltGetExtensionPointer( "glProgramLocalParameter4fvARB" );

		if ( !glGenProgramsARB || !glBindProgramARB ||
			!glProgramStringARB || !glDeleteProgramsARB ||
			!glProgramLocalParameter4fvARB ||
			!glProgramLocalParameter4fARB )
		{
			fprintf( stderr, "Not all entrypoints were available!\n" );
			Sleep( 2000 );
			exit( 0 );
		}
	}
#endif

	if ( highLevelAvailable )
	{
		glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)gltGetExtensionPointer( "glCreateShaderObjectARB" );
		glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)gltGetExtensionPointer( "glCreateProgramObjectARB" );
		glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)gltGetExtensionPointer( "glAttachObjectARB" );
		glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC)gltGetExtensionPointer( "glDetachObjectARB" );
		glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)gltGetExtensionPointer( "glDeleteObjectARB" );
		glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)gltGetExtensionPointer( "glShaderSourceARB" );
		glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)gltGetExtensionPointer( "glCompileShaderARB" );
		glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)gltGetExtensionPointer( "glLinkProgramARB" );
		glValidateProgramARB = (PFNGLVALIDATEPROGRAMARBPROC)gltGetExtensionPointer( "glValidateProgramARB" );
		glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)gltGetExtensionPointer( "glUseProgramObjectARB" );
		glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)gltGetExtensionPointer( "glGetObjectParameterivARB" );
		glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)gltGetExtensionPointer( "glGetInfoLogARB" );
		glUniform1fARB = (PFNGLUNIFORM1FARBPROC)gltGetExtensionPointer( "glUniform1fARB" );
		glUniform1iARB = (PFNGLUNIFORM1IARBPROC)gltGetExtensionPointer( "glUniform1iARB" );
		glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC)gltGetExtensionPointer( "glUniform3fvARB" );
		glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)gltGetExtensionPointer( "glGetUniformLocationARB" );

		if ( !glCreateShaderObjectARB || !glCreateProgramObjectARB ||
			!glAttachObjectARB || !glDetachObjectARB || !glDeleteObjectARB ||
			!glShaderSourceARB || !glCompileShaderARB || !glLinkProgramARB ||
			!glValidateProgramARB || !glUseProgramObjectARB ||
			!glGetObjectParameterivARB || !glGetInfoLogARB ||
			!glUniform3fvARB || !glUniform1fARB ||
			!glUniform1iARB || !glGetUniformLocationARB )
		{
			fprintf( stderr, "Not all entrypoints were available!\n" );
			Sleep( 2000 );
			exit( 0 );
		}

		useHighLevel = GL_TRUE;
	}

	fprintf( stdout, "Controls:\n" );
	fprintf( stdout, "\tRight-click for menu\n\n" );
	fprintf( stdout, "\tR/L arrows\t+/- rotate light\n\n" );
	fprintf( stdout, "\tU/D arrows\t+/- increase/decrease tesselation\n\n" );
	fprintf( stdout, "\tx/X\t\tMove +/- in x direction\n" );
	fprintf( stdout, "\ty/Y\t\tMove +/- in y direction\n" );
	fprintf( stdout, "\tz/Z\t\tMove +/- in z direction\n\n" );
	fprintf( stdout, "\tq\t\tExit demo\n\n" );

	// Black background
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	// Misc. state
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel( GL_SMOOTH );

	if ( lowLevelAvailable )
	{
		glGenProgramsARB( TOTAL_SHADER_SETS, v_ids );
		glGenProgramsARB( TOTAL_SHADER_SETS, f_ids );
		// Low-level will always be enabled, but high-level 
		// will take precedence if they're both enabled
		glEnable( GL_VERTEX_PROGRAM_ARB );
		glEnable( GL_FRAGMENT_PROGRAM_ARB );
	}

	// Load and compile low- and high-level shaders
	for ( i = 0; i < TOTAL_SHADER_SETS; i++ )
	{
		PrepareShader( i );
	}

	// Install first shader
	if ( lowLevelAvailable )
	{
		glBindProgramARB( GL_VERTEX_PROGRAM_ARB, v_ids[0] );
		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, f_ids[0] );
	}
	if ( useHighLevel )
	{
		glUseProgramObjectARB( progObj[whichShader] );
	}
}

void ProcessMenu( int value )
{
	switch ( value )
	{
	case 1:
		useHighLevel = !useHighLevel;
		if ( useHighLevel )
		{
			glutChangeToMenuEntry( 2, "Switch to low-level shaders", 1 );
			glUseProgramObjectARB( progObj[whichShader] );
		}
		else
		{
			glutChangeToMenuEntry( 2, "Switch to high-level shaders", 1 );
			glUseProgramObjectARB( 0 );
		}
		break;

	default:
		whichShader = value - 2;
		{
			char menuEntry[128];
			sprintf( menuEntry, "Choose shader set (currently \"%s\")", shaderNames[whichShader] );
			glutSetMenu( mainMenu );
			glutChangeToSubMenu( 1, menuEntry, shaderMenu );
		}
		if ( lowLevelAvailable )
		{
			glBindProgramARB( GL_VERTEX_PROGRAM_ARB, v_ids[whichShader] );
			glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, f_ids[whichShader] );
		}
		if ( useHighLevel )
		{
			glUseProgramObjectARB( progObj[whichShader] );
		}
		break;
	}

	// Refresh the Window
	glutPostRedisplay();
}

void KeyPressFunc( unsigned char key, int x, int y )
{
	switch ( key )
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
	case 27: /* ESC */
		exit( 0 );
	}

	// Refresh the Window
	glutPostRedisplay();
}

void SpecialKeys( int key, int x, int y )
{
	switch ( key )
	{
	case GLUT_KEY_LEFT:
		lightRotation -= 5.0f;
		break;
	case GLUT_KEY_RIGHT:
		lightRotation += 5.0f;
		break;
	case GLUT_KEY_UP:
		tess += 5;
		break;
	case GLUT_KEY_DOWN:
		tess -= 5;
		if ( tess < 5 ) tess = 5;
		break;
	default:
		break;
	}

	// Refresh the Window
	glutPostRedisplay();
}

void ChangeSize( int w, int h )
{
	windowWidth = w;
	windowHeight = h;
	glViewport( 0, 0, windowWidth, windowHeight );
}

int main( int argc, char* argv[] )
{
	GLint i;

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( windowWidth, windowHeight );
	glutCreateWindow( "Lighting Demo" );
	glutReshapeFunc( ChangeSize );
	glutKeyboardFunc( KeyPressFunc );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );

	SetupRC();

	// Create the menus
	shaderMenu = glutCreateMenu( ProcessMenu );
	for ( i = 0; i < TOTAL_SHADER_SETS; i++ )
	{
		char menuEntry[128];
		sprintf( menuEntry, "\"%s\"", shaderNames[i] );
		glutAddMenuEntry( menuEntry, 2 + i );
	}

	mainMenu = glutCreateMenu( ProcessMenu );
	{
		char menuEntry[128];
		sprintf( menuEntry, "Choose fragment shader (currently \"%s\")", shaderNames[0] );
		glutAddSubMenu( menuEntry, shaderMenu );
	}
	if ( lowLevelAvailable && highLevelAvailable )
	{
		if ( useHighLevel )
		{
			glutAddMenuEntry( "Switch to low-level shaders", 1 );
		}
		else
		{
			glutAddMenuEntry( "Switch to high-level shaders", 1 );
		}
	}
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glutMainLoop();

	if ( glDeleteObjectARB )
	{
		for ( i = 0; i < TOTAL_SHADER_SETS; i++ )
		{
			glDeleteObjectARB( progObj[i] );
			glDeleteObjectARB( vShader[i] );
			glDeleteObjectARB( fShader[i] );
		}
	}

#ifndef __APPLE__
	if ( glDeleteProgramsARB )
	{
		glDeleteProgramsARB( TOTAL_SHADER_SETS, v_ids );
		glDeleteProgramsARB( TOTAL_SHADER_SETS, f_ids );
	}
#endif

	return 0;
}

