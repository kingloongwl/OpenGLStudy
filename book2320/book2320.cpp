
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
PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;

#ifndef __APPLE__
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;
#endif

GLboolean lowLevelAvailable = GL_FALSE;
GLboolean highLevelAvailable = GL_FALSE;
GLboolean useHighLevel = GL_FALSE;

#define PASS_THROUGH  0
#define BLUR          1
#define SHARPEN       2
#define DILATION      3
#define EROSION       4
#define LAPLACIAN     5
#define SOBEL         6
#define PREWITT       7
#define TOTAL_SHADERS 5

GLuint ids[TOTAL_SHADERS];                                   // low-level shader object names
GLhandleARB fShader[TOTAL_SHADERS], progObj[TOTAL_SHADERS];  // high-level shader object handles
GLboolean needsValidation[TOTAL_SHADERS];
//char *shaderNames[TOTAL_SHADERS] = { "passthrough", "blur", "sharpen", "dilation", "erosion",
//"laplacian", "sobel", "prewitt" };
char *shaderNames[TOTAL_SHADERS] = { "2321", "2322", "2323", "2324", "2325" };

GLint whichShader = PASS_THROUGH;       // current shader

GLint windowWidth = 512;                // window size
GLint windowHeight = 512;
GLint textureWidth = 512;               // texture size
GLint textureHeight = 512;

GLint mainMenu, shaderMenu, passMenu;   // menu handles

GLint maxTexSize;                       // maximum allowed size for 1D/2D texture

GLfloat cameraPos[] = { 100.0f, 75.0f, 150.0f, 1.0f };
GLfloat lightPos[] = { 140.0f, 250.0f, 140.0f, 1.0f };
GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };

GLfloat lightRotation = 0.0f;
GLfloat texCoordOffsets[18];
GLint numPasses = 2;

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
	GLubyte *fsString;

	// Create low-level shader objects and specify shader text
	if ( lowLevelAvailable )
	{
		GLint errorPos;

#ifdef __APPLE__
		sprintf( fullFileName, "%s.fp", shaderNames[shaderNum] );
#else
		sprintf( fullFileName, "..\\..\\book2320\\%s.frag", shaderNames[shaderNum] );
#endif

		fsString = LoadShaderText( fullFileName );
		if ( !fsString )
		{
			fprintf( stderr, "Unable to load \"%s\"\n", fullFileName );
			Sleep( 5000 );
			exit( 0 );
		}
		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, ids[shaderNum] );
		glProgramStringARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, strlen( (char*)fsString ), fsString );
		free( fsString );
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
		const GLcharARB *fsStringPtr[1];
		GLint success;

#ifdef __APPLE__
		sprintf( fullFileName, "%s.fs", shaderNames[shaderNum] );
#else
		sprintf( fullFileName, "..\\..\\book2320\\%s.frag", shaderNames[shaderNum] );
#endif
		fsString = LoadShaderText( fullFileName );
		if ( !fsString )
		{
			fprintf( stderr, "Unable to load \"%s\"\n", fullFileName );
			Sleep( 5000 );
			exit( 0 );
		}
		fShader[shaderNum] = glCreateShaderObjectARB( GL_FRAGMENT_SHADER_ARB );
		fsStringPtr[0] = (GLcharARB*)fsString;
		glShaderSourceARB( fShader[shaderNum], 1, fsStringPtr, NULL );
		free( fsString );

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
			//fprintf(stderr, "High-level fragment shader #%d compile info log: %s\n", shaderNum, infoLog);
		}

		// Create program object, attach shader, then link
		progObj[shaderNum] = glCreateProgramObjectARB();
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
			//fprintf(stderr, "High-level program #%d link info log: %s\n", shaderNum, infoLog);
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
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
	glPopMatrix();

	// Draw plane that the objects rest on
	glColor3f( 0.0f, 0.0f, 0.90f ); // Blue
	glNormal3f( 0.0f, 1.0f, 0.0f );
	glBegin( GL_QUADS );
	glVertex3f( -100.0f, -25.0f, -100.0f );
	glVertex3f( -100.0f, -25.0f, 100.0f );
	glVertex3f( 100.0f, -25.0f, 100.0f );
	glVertex3f( 100.0f, -25.0f, -100.0f );
	glEnd();

	// Draw red cube
	glColor3f( 1.0f, 0.0f, 0.0f );
	glutSolidCube( 48.0f );

	// Draw green sphere
	glColor3f( 0.0f, 1.0f, 0.0f );
	glPushMatrix();
	glTranslatef( -60.0f, 0.0f, 0.0f );
	glutSolidSphere( 25.0f, 50, 50 );
	glPopMatrix();

	// Draw magenta torus
	glColor3f( 1.0f, 0.0f, 1.0f );
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, 60.0f );
	glutSolidTorus( 8.0f, 16.0f, 50, 50 );
	glPopMatrix();

	// Draw yellow cone
	glColor3f( 1.0f, 1.0f, 0.0f );
	glPushMatrix();
	glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
	glTranslatef( 60.0f, 0.0f, -24.0f );
	glutSolidCone( 25.0f, 50.0f, 50, 50 );
	glPopMatrix();

	// Draw cyan teapot
	glColor3f( 0.0f, 1.0f, 1.0f );
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -60.0f );
	glutSolidTeapot( 25.0f );
	glPopMatrix();
}

// Called to draw scene
void RenderScene( void )
{
	GLint pass;

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

	// Redraw from texture w/ fragment shader
	if ( useHighLevel )
	{
		GLint uniformLoc;
		glUseProgramObjectARB( progObj[whichShader] );
		uniformLoc = glGetUniformLocationARB( progObj[whichShader], "sampler0" );
		if ( uniformLoc != -1 )
		{
			glUniform1iARB( uniformLoc, 0 );
		}
		uniformLoc = glGetUniformLocationARB( progObj[whichShader], "tc_offset" );
		if ( uniformLoc != -1 )
		{
			glUniform2fvARB( uniformLoc, 9, texCoordOffsets );
		}
	}
	else
	{
		GLint i;
		glEnable( GL_FRAGMENT_PROGRAM_ARB );
		for ( i = 0; i < 9; i++ )
		{
			glProgramLocalParameter4fARB( GL_FRAGMENT_PROGRAM_ARB, i, texCoordOffsets[i * 2], texCoordOffsets[(i * 2) + 1], 0.0f, 1.0f );
		}
	}

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	for ( pass = 0; pass < numPasses; pass++ )
	{
		// Copy original scene to texture
		glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, (windowWidth - textureWidth) / 2,
			(windowHeight - textureHeight) / 2, textureWidth, textureHeight, 0 );


		glClear( GL_COLOR_BUFFER_BIT );
		glBegin( GL_QUADS );
		glMultiTexCoord2f( GL_TEXTURE0, 0.0f, 0.0f );
		glVertex2f( -((GLfloat)textureWidth / (GLfloat)windowWidth), -((GLfloat)textureHeight / (GLfloat)windowHeight) );
		glMultiTexCoord2f( GL_TEXTURE0, 0.0f, 1.0f );
		glVertex2f( -((GLfloat)textureWidth / (GLfloat)windowWidth), ((GLfloat)textureHeight / (GLfloat)windowHeight) );
		glMultiTexCoord2f( GL_TEXTURE0, 1.0f, 1.0f );
		glVertex2f( ((GLfloat)textureWidth / (GLfloat)windowWidth), ((GLfloat)textureHeight / (GLfloat)windowHeight) );
		glMultiTexCoord2f( GL_TEXTURE0, 1.0f, 0.0f );
		glVertex2f( ((GLfloat)textureWidth / (GLfloat)windowWidth), -((GLfloat)textureHeight / (GLfloat)windowHeight) );
		glEnd();
	}

	glEnable( GL_DEPTH_TEST );

	if ( useHighLevel )
	{
		glUseProgramObjectARB( 0 );
	}
	else
	{
		glDisable( GL_FRAGMENT_PROGRAM_ARB );
	}

	if ( glGetError() != GL_NO_ERROR )
		fprintf( stderr, "GL Error!\n" );

	// Flush drawing commands
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	const GLubyte *version;
	GLint i;

	fprintf( stdout, "Image Processing Demo\n\n" );

	// Make sure required functionality is available!
	if ( gltIsExtSupported( "GL_ARB_fragment_shader" ) &&
		gltIsExtSupported( "GL_ARB_shader_objects" ) &&
		gltIsExtSupported( "GL_ARB_shading_language_100" ) )
	{
		highLevelAvailable = GL_TRUE;
	}

	if ( !lowLevelAvailable && !highLevelAvailable )
	{
		fprintf( stderr, "Neither fragment shader"
			" extension is available!\n" );
		Sleep( 2000 );
		exit( 0 );
	}


	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTexSize );

#ifndef __APPLE__
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)gltGetExtensionPointer( "glActiveTexture" );
	glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)gltGetExtensionPointer( "glMultiTexCoord2f" );
	if ( !glActiveTexture || !glMultiTexCoord2f )
	{
		glActiveTexture = (PFNGLACTIVETEXTUREARBPROC)gltGetExtensionPointer( "glActiveTextureARB" );
		glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FARBPROC)gltGetExtensionPointer( "glMultiTexCoord2fARB" );
	}
	if ( !glActiveTexture || !glMultiTexCoord2f )
	{
		fprintf( stderr, "Not all entrypoints were available!\n" );
		Sleep( 2000 );
		exit( 0 );
	}

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
		glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC)gltGetExtensionPointer( "glUniform2fvARB" );
		glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC)gltGetExtensionPointer( "glUniform3fvARB" );
		glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)gltGetExtensionPointer( "glGetUniformLocationARB" );

		if ( !glCreateShaderObjectARB || !glCreateProgramObjectARB ||
			!glAttachObjectARB || !glDetachObjectARB || !glDeleteObjectARB ||
			!glShaderSourceARB || !glCompileShaderARB || !glLinkProgramARB ||
			!glValidateProgramARB || !glUseProgramObjectARB ||
			!glGetObjectParameterivARB || !glGetInfoLogARB ||
			!glUniform3fvARB || !glUniform1fARB || !glUniform2fvARB ||
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
	fprintf( stdout, "\tR/L arrows\t+/- rotate lights for others shaders\n\n" );
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
	glEnable( GL_LIGHTING );
	glEnable( GL_COLOR_MATERIAL );
	glEnable( GL_NORMALIZE );
	glEnable( GL_LIGHT0 );
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );

	// Texture state
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	if ( lowLevelAvailable )
	{
		glGenProgramsARB( TOTAL_SHADERS, ids );
	}

	// Load and compile low- and high-level shaders
	for ( i = 0; i < TOTAL_SHADERS; i++ )
	{
		PrepareShader( i );
	}

	// Install first shader
	if ( lowLevelAvailable )
	{
		glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, ids[0] );
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
			glutChangeToMenuEntry( 3, "Switch to low-level fragment shader", 1 );
		}
		else
		{
			glutChangeToMenuEntry( 3, "Switch to high-level fragment shader", 1 );
		}
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		numPasses = value - 1;
		{
			char menuEntry[128];
			sprintf( menuEntry, "Choose number of passes (currently %d)", numPasses );
			glutSetMenu( mainMenu );
			glutChangeToSubMenu( 2, menuEntry, passMenu );
		}
		break;

	default:
		whichShader = value - 7;
		{
			char menuEntry[128];
			sprintf( menuEntry, "Choose fragment shader (currently \"%s\")", shaderNames[whichShader] );
			glutSetMenu( mainMenu );
			glutChangeToSubMenu( 1, menuEntry, shaderMenu );
		}
		if ( lowLevelAvailable )
		{
			glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, ids[whichShader] );
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

void ChangeSize( int w, int h )
{
	GLint i, j;
	GLfloat xInc, yInc;

	windowWidth = w;
	windowHeight = h;
	glViewport( 0, 0, windowWidth, windowHeight );

	// find largest power-of-two texture smaller than window
	textureWidth = textureHeight = maxTexSize;
	while ( textureWidth > windowWidth )
		textureWidth >>= 1;
	while ( textureHeight > windowHeight )
		textureHeight >>= 1;

	xInc = 1.0f / (GLfloat)textureWidth;
	yInc = 1.0f / (GLfloat)textureHeight;

	for ( i = 0; i < 3; i++ )
	{
		for ( j = 0; j < 3; j++ )
		{
			texCoordOffsets[(((i * 3) + j) * 2) + 0] = (-1.0f * xInc) + ((GLfloat)i * xInc);
			texCoordOffsets[(((i * 3) + j) * 2) + 1] = (-1.0f * yInc) + ((GLfloat)j * yInc);
		}
	}
}

int main( int argc, char* argv[] )
{
	GLint i;

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( windowWidth, windowHeight );
	glutCreateWindow( "Image Processing Demo" );
	glutReshapeFunc( ChangeSize );
	glutKeyboardFunc( KeyPressFunc );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );

	SetupRC();

	// Create the menus
	shaderMenu = glutCreateMenu( ProcessMenu );
	for ( i = 0; i < TOTAL_SHADERS; i++ )
	{
		char menuEntry[128];
		sprintf( menuEntry, "\"%s\"", shaderNames[i] );
		glutAddMenuEntry( menuEntry, 7 + i );
	}

	passMenu = glutCreateMenu( ProcessMenu );
	glutAddMenuEntry( "1", 2 );
	glutAddMenuEntry( "2", 3 );
	glutAddMenuEntry( "3", 4 );
	glutAddMenuEntry( "4", 5 );
	glutAddMenuEntry( "5", 6 );

	mainMenu = glutCreateMenu( ProcessMenu );
	{
		char menuEntry[128];
		sprintf( menuEntry, "Choose fragment shader (currently \"%s\")", shaderNames[0] );
		glutAddSubMenu( menuEntry, shaderMenu );
		sprintf( menuEntry, "Choose number of passes (currently 2)" );
		glutAddSubMenu( menuEntry, passMenu );
	}
	if ( lowLevelAvailable && highLevelAvailable )
	{
		if ( useHighLevel )
		{
			glutAddMenuEntry( "Switch to low-level fragment shader", 1 );
		}
		else
		{
			glutAddMenuEntry( "Switch to high-level fragment shader", 1 );
		}
	}
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glutMainLoop();

	if ( glDeleteObjectARB )
	{
		for ( i = 0; i < TOTAL_SHADERS; i++ )
		{
			glDeleteObjectARB( progObj[i] );
			glDeleteObjectARB( fShader[i] );
		}
	}
#ifndef __APPLE__
	if ( glDeleteProgramsARB )
	{
		glDeleteProgramsARB( TOTAL_SHADERS, ids );
	}
#endif

	return 0;
}
