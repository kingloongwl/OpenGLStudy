
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

PFNGLPROGRAMSTRINGARBPROC			glProgramStringARB;
PFNGLBINDPROGRAMARBPROC				glBindProgramARB;
PFNGLDELETEPROGRAMSARBPROC			glDeleteProgramsARB;
PFNGLGENPROGRAMSARBPROC				glGenProgramsARB;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
PFNGLVERTEXATTRIB1FARBPROC			glVertexAttrib1fARB;
PFNGLDELETEOBJECTARBPROC 			glDeleteObjectARB;
PFNGLGETHANDLEARBPROC 				glGetHandleARB;
PFNGLDETACHOBJECTARBPROC 			glDetachObjectARB;
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
PFNGLUNIFORM3FVARBPROC 				glUniform3fvARB;
PFNGLUNIFORMMATRIX4FVARBPROC 		glUniformMatrix3fvARB;
PFNGLUNIFORMMATRIX4FVARBPROC 		glUniformMatrix4fvARB;
PFNGLGETUNIFORMLOCATIONARBPROC 		glGetUniformLocationARB;
PFNGLGETATTRIBLOCATIONARBPROC 		glGetAttribLocationARB;

GLboolean lowLevelAvailable = GL_FALSE;
GLboolean highLevelAvailable = GL_FALSE;

GLboolean useBlending = GL_TRUE;
GLboolean showBones = GL_FALSE;

#define TOTAL_SHADERS 1
GLuint ids[TOTAL_SHADERS];                                   // low-level shader object names
GLhandleARB vShader[TOTAL_SHADERS], progObj[TOTAL_SHADERS];  // high-level shader object handles
GLboolean needsValidation[TOTAL_SHADERS];
char *shaderNames[TOTAL_SHADERS] = {"skinning"};

#define WEIGHT_SLOT        1            // locations for low-level attrib and program local parameters
#define LIGHT_POS_SLOT     0
#define MODELVIEW2_SLOT    1

GLint lightPosLoc, mv2Loc, mv2ITLoc;    // locations for high-level uniforms
GLint weightLoc;                        // locations for high-level attribs

GLint windowWidth = 512;                // window size
GLint windowHeight = 512;

GLint mainMenu;                         // menu handles

GLint maxTexSize;                       // maximum allowed size for 1D/2D texture

GLfloat cameraPos[] = { 50.0f, 75.0f, 50.0f, 1.0f};
GLfloat lightPos[] =  { -50.0f, 100.0f, 50.0f, 1.0f};

GLfloat elbowBend = 45.0f;
GLfloat sphereOfInfluence = 1.0f;       // how much of each limb gets blended

#define MAX_INFO_LOG_SIZE 2048

GLvoid CreatePowMap( GLfloat r, GLfloat g, GLfloat b )
{
	GLfloat texels[ 512 * 4 ];
	GLint texSize = ( maxTexSize > 512 ) ? 512 : maxTexSize;
	GLint x;
	
	for ( x = 0; x < texSize; x++ )
	{
		texels[x*4 + 0] = r * pow(((double)x / (double)(texSize-1)) * 0.125f + 0.875f, 128.0);
		texels[x*4 + 1] = g * pow(((double)x / (double)(texSize-1)) * 0.125f + 0.875f, 128.0);
        texels[x*4 + 2] = b * pow(((double)x / (double)(texSize-1)) * 0.125f + 0.875f, 128.0);
        texels[x*4 + 3] = 1.0f;
	}
	
	texels[0] = texels[1] = texels[2] = 0.0f;
	
	glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA16, texSize, 0, GL_RGBA, GL_FLOAT, texels );
}

GLubyte *LoadShaderText( const char * fileName )
{
	GLubyte *shaderText = NULL;
	GLint shaderLength = 0;
	FILE *fp;
	
	fp = fopen(fileName, "r");
	if ( fp != NULL )
	{
		while( fgetc(fp) != EOF )
		{
			shaderLength++;
		}
		
		rewind( fp );
		shaderText = (GLubyte*)malloc( shaderLength + 1 );
		if ( shaderText != NULL )
		{
			fread( shaderText, 1, shaderLength, fp );
		}
		shaderText[shaderLength] = '\0';
		fclose(fp);
	}
	
	return shaderText;
}

void PrepareShader( GLint shaderNum )
{
	char fullFileName[255];
	GLubyte *vsString;
	
	if ( highLevelAvailable )
	{
		const GLcharARB *vsStringPtr[1];
		GLint success;
		
		sprintf( fullFileName, "../../book2201/%s.vert", shaderNames[shaderNum] );
		vsString = LoadShaderText( fullFileName );
		if ( !vsString )
		{
			fprintf( stderr, "Unable to load \"%s\"\n", fullFileName );
			Sleep( 5000 );
			exit( 0 );
		}
		vShader[shaderNum] = glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );
		vsStringPtr[0] = (GLcharARB*)vsString;
		glShaderSourceARB(vShader[shaderNum], 1, vsStringPtr, NULL);
		free(vsString);
		
		glCompileShaderARB( vShader[shaderNum] );
		glGetObjectParameterivARB( vShader[shaderNum], GL_OBJECT_COMPILE_STATUS_ARB, &success );
		if ( !success )
		{
			GLbyte infoLog[MAX_INFO_LOG_SIZE];
            glGetInfoLogARB(vShader[shaderNum], MAX_INFO_LOG_SIZE, NULL, (GLcharARB*)infoLog);
            fprintf(stderr, "Error in high-level vertex shader #%d compilation!\n", shaderNum);
            fprintf(stderr, "Info log: %s\n", infoLog);
            Sleep(10000);
            exit(0);
		}
		
		progObj[shaderNum] = glCreateProgramObjectARB();
		glAttachObjectARB(progObj[shaderNum], vShader[shaderNum]);
		
		glLinkProgramARB(progObj[shaderNum]);
		glGetObjectParameterivARB(progObj[shaderNum], GL_OBJECT_LINK_STATUS_ARB, &success );
		if (!success)
        {
            GLbyte infoLog[MAX_INFO_LOG_SIZE];
			glGetInfoLogARB( progObj[shaderNum], MAX_INFO_LOG_SIZE, NULL, (GLcharARB*)infoLog );
            fprintf(stderr, "Error in high-level program #%d linkage!\n", shaderNum);
            fprintf(stderr, "Info log: %s\n", infoLog);
            Sleep(10000);
            exit(0);
        }
		
		lightPosLoc = glGetUniformLocationARB( progObj[0], "lightPos" );
		mv2Loc = glGetUniformLocationARB( progObj[0], "mv2" );
		mv2ITLoc = glGetUniformLocationARB( progObj[0], "mv2IT" );
		weightLoc = glGetAttribLocationARB( progObj[0], "weight" );
		
		needsValidation[shaderNum] = GL_TRUE;
	}
}


// Draw a cylinder with supplied dimensions
void DrawCylinder(GLfloat length, GLfloat diameter1, GLfloat diameter2, 
                  GLfloat startWeight, GLfloat endWeight)
{
    int numFacets = 50;
    int numSections = 50;
    GLfloat angleIncr = (2.0f * 3.14159f) / (float)numFacets;
    GLfloat sectionLength = length / numSections;
    GLfloat wEnd, influenceStart;
    int i, j;

    // Determine where our influence starts for this limb
    if (startWeight == 0.5f)
    {
        influenceStart = sphereOfInfluence;
    }
    else
    {
        influenceStart = 1.0f - sphereOfInfluence;
    }

    // Skin
    for (i = 0; i < numFacets; i++)
    {
        // Calculate geometry for this facet
        GLfloat angle1 = i * angleIncr;
        GLfloat angle2 = ((i+1)%numFacets) * angleIncr;
        GLfloat y1AtFirstEnd = sin(angle1) * diameter1;
        GLfloat y1AtOtherEnd = sin(angle1) * diameter2;
        GLfloat z1AtFirstEnd = cos(angle1) * diameter1;
        GLfloat z1AtOtherEnd = cos(angle1) * diameter2;
        GLfloat y2AtFirstEnd = sin(angle2) * diameter1;
        GLfloat y2AtOtherEnd = sin(angle2) * diameter2;
        GLfloat z2AtFirstEnd = cos(angle2) * diameter1;
        GLfloat z2AtOtherEnd = cos(angle2) * diameter2;
        GLfloat n1y = y1AtFirstEnd;
        GLfloat n1z = z1AtFirstEnd;
        GLfloat n2y = y2AtFirstEnd;
        GLfloat n2z = z2AtFirstEnd;

        // One strip per facet
        glBegin(GL_QUAD_STRIP);

        glVertexAttrib1fARB(weightLoc, 
                            useBlending ? startWeight : 1.0f);
        glNormal3f(0.0f, n1y, n1z);
        glVertex3f(0.0f, y1AtFirstEnd, z1AtFirstEnd);
        glNormal3f(0.0f, n2y, n2z);
        glVertex3f(0.0f, y2AtFirstEnd, z2AtFirstEnd);

        for (j = 0; j < numSections; j++)
        {
            // Calculate geometry for end of this quad
            GLfloat paramEnd = (float)(j+1) / (float)numSections;
            GLfloat lengthEnd = paramEnd * length;
            GLfloat y1End = y1AtFirstEnd + (paramEnd * (y1AtOtherEnd - y1AtFirstEnd));
            GLfloat z1End = z1AtFirstEnd + (paramEnd * (z1AtOtherEnd - z1AtFirstEnd));
            GLfloat y2End = y2AtFirstEnd + (paramEnd * (y2AtOtherEnd - y2AtFirstEnd));
            GLfloat z2End = z2AtFirstEnd + (paramEnd * (z2AtOtherEnd - z2AtFirstEnd));

            // Calculate vertex weights
            if (!useBlending)
            {
                wEnd = 1.0f;
            }
            else if (paramEnd <= influenceStart)
            {
                // Map params [0,influenceStart] to weights [0,1]
                GLfloat p = (paramEnd * (1.0f/influenceStart));

                // We're in the first half of the cylinder: startWeight -> 1
                wEnd = startWeight + p * (1.0f - startWeight);
            }
            else
            {
                // Map params [influenceStart,1] to weights [0,1]
                GLfloat p = (paramEnd-influenceStart) * (1.0f/(1.0f-influenceStart));

                // We're in the second half of the cylinder: 1 -> endWeight
                wEnd = 1.0f + p * (endWeight - 1.0f);
            }

            glVertexAttrib1fARB(weightLoc, wEnd);
            glNormal3f(0.0f, n1y, n1z);
            glVertex3f(lengthEnd, y1End, z1End);
            glNormal3f(0.0f, n2y, n2z);
            glVertex3f(lengthEnd, y2End, z2End);
        }

        // End facet strip
        glEnd();
    }

    if (showBones)
    {
        // Skeleton
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertexAttrib1fARB(weightLoc, 1.0f);
        glBegin(GL_LINES);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(length, 0.0f, 0.0f);
        glEnd();
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(length, 0.0f, 0.0f);
        glEnd();
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}

// Called to draw scene objects
void DrawModels(void)
{
    GLTVector3 lightPosEye;
    GLTMatrix mv, mv2;

    // Transform light position to eye space
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    gltTransformPoint(lightPos, mv, lightPosEye);


    glUniform3fvARB(lightPosLoc, 1, lightPosEye);


    // Setup modelview matrices for upper arm
    glPushMatrix();
    glRotatef(elbowBend, 0.0f, 0.0f, 1.0f);
    glTranslatef(-50.0f, 0.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, mv2);
    glPopMatrix();
    glTranslatef(-50.0f, 0.0f, 0.0f);

        GLTMatrix mv2IT_4x4;
        GLfloat mv2IT_3x3[9];
        GLint i;

        glUniformMatrix4fvARB(mv2Loc, 1, GL_FALSE, mv2);

        gltInvertMatrix(mv2, mv2IT_4x4);

        // Take upper left 3x3 for 2nd normal matrix
        for (i = 0; i < 9; i++)
        {
            mv2IT_3x3[i] = mv2IT_4x4[((i/3)*4)+(i%3)];
        }
        glUniformMatrix3fvARB(mv2ITLoc, 1, GL_TRUE, mv2IT_3x3);


    // Draw upper arm cylinder
    glColor3f(0.0f, 0.0f, 0.90f);      // Blue
    // 50 long, 10 shoulder, 9 elbow, with weights applied to second half
    DrawCylinder(50.0f, 15.0f, 9.0f, 1.0f, 0.5f);

    // Setup modelview matrices for forearm
    glTranslatef(50.0f, 0.0f, 0.0f);
    glPushMatrix();
    glGetFloatv(GL_MODELVIEW_MATRIX, mv2);
    glPopMatrix();
	glRotatef( elbowBend, 0.0f, 0.0f, 1.0f );

	{
		GLTMatrix mv2IT_4x4;
		GLfloat mv2IT_3x3[9];
		GLint i;

		glUniformMatrix4fvARB( mv2Loc, 1, GL_FALSE, mv2 );

		gltInvertMatrix( mv2, mv2IT_4x4 );

		// Take upper left 3x3 for 2nd normal matrix
		for ( i = 0; i < 9; i++ )
		{
			mv2IT_3x3[i] = mv2IT_4x4[((i / 3) * 4) + (i % 3)];
		}
		glUniformMatrix3fvARB( mv2ITLoc, 1, GL_TRUE, mv2IT_3x3 );
	}


    // Draw forearm cylinder
    glColor3f(0.9f, 0.0f, 0.0f);       // Red
    // 40 long, 9 elbow, 5 wrist, with weights applied to first half
    DrawCylinder(40.0f, 9.0f, 5.0f, 0.5f, 1.0f);
}

// Called to draw scene
void RenderScene(void)
{
    // Track camera angle
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 
              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glViewport(0, 0, windowWidth, windowHeight);
    
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Validate our shader before first use
    if (needsValidation[0])
    {
        GLint success;

        glValidateProgramARB(progObj[0]);
        glGetObjectParameterivARB(progObj[0], GL_OBJECT_VALIDATE_STATUS_ARB, &success);
        if (!success)
        {
            GLbyte infoLog[MAX_INFO_LOG_SIZE];
            glGetInfoLogARB(progObj[0], MAX_INFO_LOG_SIZE, NULL, (GLcharARB*)infoLog);
            fprintf(stderr, "Error in program validation!\n");
            fprintf(stderr, "Info log: %s\n", infoLog);
            Sleep(10000);
            exit(0);
        }

        needsValidation[0] = GL_FALSE;
    }
    
    // Draw objects in the scene
    DrawModels();
    
    if (glGetError() != GL_NO_ERROR)
        fprintf(stderr, "GL Error!\n");

    // Flush drawing commands
    glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
    const GLubyte *version;
    GLint i;

    fprintf(stdout, "Vertex Blending Demo\n\n");

    // Make sure required functionality is available!
    if (gltIsExtSupported("GL_ARB_vertex_shader") &&
        gltIsExtSupported("GL_ARB_shader_objects") &&
        gltIsExtSupported("GL_ARB_shading_language_100"))
    {
        highLevelAvailable = GL_TRUE;
    }

    if (!highLevelAvailable)
    {
        fprintf(stderr, "Neither vertex shader"
                        " extension is available!\n");
        Sleep(2000);
        exit(0);
    }

    // Make sure we have multitexture, cube maps, and texenv add!
    version = glGetString(GL_VERSION);
    if (((version[0] != '1') || (version[1] != '.') || 
         (version[2] < '3') || (version[2] > '9')) &&   // 1.3+
        (!gltIsExtSupported("GL_ARB_multitexture")     ||
         !gltIsExtSupported("GL_ARB_texture_env_add")))
    {
        fprintf(stderr, "Neither OpenGL 1.3 nor necessary"
                        " extensions are available!\n");
        Sleep(2000);
        exit(0);
    }

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);

    if (highLevelAvailable)
    {
        glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)gltGetExtensionPointer("glCreateShaderObjectARB");
        glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)gltGetExtensionPointer("glCreateProgramObjectARB");
        glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)gltGetExtensionPointer("glAttachObjectARB");
        glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC)gltGetExtensionPointer("glDetachObjectARB");
        glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)gltGetExtensionPointer("glDeleteObjectARB");
        glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)gltGetExtensionPointer("glShaderSourceARB");
        glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)gltGetExtensionPointer("glCompileShaderARB");
        glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)gltGetExtensionPointer("glLinkProgramARB");
        glValidateProgramARB = (PFNGLVALIDATEPROGRAMARBPROC)gltGetExtensionPointer("glValidateProgramARB");
        glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)gltGetExtensionPointer("glUseProgramObjectARB");
        glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)gltGetExtensionPointer("glGetObjectParameterivARB");
        glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)gltGetExtensionPointer("glGetInfoLogARB");
        glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC)gltGetExtensionPointer("glUniform3fvARB");
        glUniformMatrix3fvARB = (PFNGLUNIFORMMATRIX3FVARBPROC)gltGetExtensionPointer("glUniformMatrix3fvARB");
        glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC)gltGetExtensionPointer("glUniformMatrix4fvARB");
        glVertexAttrib1fARB = (PFNGLVERTEXATTRIB1FARBPROC)gltGetExtensionPointer("glVertexAttrib1fARB");
        glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)gltGetExtensionPointer("glGetUniformLocationARB");
        glGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC)gltGetExtensionPointer("glGetAttribLocationARB");

        if (!glCreateShaderObjectARB || !glCreateProgramObjectARB || 
            !glAttachObjectARB || !glDetachObjectARB || !glDeleteObjectARB || 
            !glShaderSourceARB || !glCompileShaderARB || !glLinkProgramARB ||
            !glValidateProgramARB || !glUseProgramObjectARB || 
            !glGetObjectParameterivARB || !glGetInfoLogARB || 
            !glUniformMatrix4fvARB || !glUniformMatrix4fvARB || 
            !glUniform3fvARB || 
            #ifndef __APPLE__
            !glVertexAttrib1fARB ||
            #endif
            !glGetUniformLocationARB || !glGetAttribLocationARB)
        {
            fprintf(stderr, "Not all entrypoints were available!\n");
            Sleep(2000);
            exit(0);
        }

    }

    fprintf(stdout, "Controls:\n");
    fprintf(stdout, "\tRight-click for menu\n\n");
    fprintf(stdout, "\tL/R arrows\tChange sphere of influence\n");
    fprintf(stdout, "\tU/D arrows\tChange angle of forearm\n\n");
    fprintf(stdout, "\tx/X\t\tMove  +/- in x direction\n");
    fprintf(stdout, "\ty/Y\t\tMove  +/- in y direction\n");
    fprintf(stdout, "\tz/Z\t\tMove  +/- in z direction\n\n");
    fprintf(stdout, "\tq\t\tExit demo\n\n");
    
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Hidden surface removal
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Misc.
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(10.0f);
    glLineWidth(5.0f);

    // Texture state
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
    glBindTexture(GL_TEXTURE_1D, 0);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    CreatePowMap(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_1D);

    // Load and compile low- and high-level shaders
    for (i = 0; i < TOTAL_SHADERS; i++)
    {
        PrepareShader(i);
    }


    glUseProgramObjectARB(progObj[0]);

}

void ProcessMenu(int value)
{
    switch(value)
    {
    case 1:
        useBlending = !useBlending;
        if (useBlending)
        {
            glutChangeToMenuEntry(1, "Toggle vertex blending (currently ON)", 1);
        }
        else
        {
            glutChangeToMenuEntry(1, "Toggle vertex blending (currently OFF)", 1);
        }
        break;

    case 2:
        showBones = !showBones;
        if (showBones)
        {
            glutChangeToMenuEntry(2, "Show bones (currently ON)", 2);
        }
        else
        {
            glutChangeToMenuEntry(2, "Show bones (currently OFF)", 2);
        }
        break;

    default:
        break;
    }

    // Refresh the Window
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
        sphereOfInfluence -= 0.05f;
        if (sphereOfInfluence < 0.05f)
            sphereOfInfluence = 0.0f;
        break;
    case GLUT_KEY_RIGHT:
        sphereOfInfluence += 0.05f;
        if (sphereOfInfluence > 0.95f)
            sphereOfInfluence = 1.0f;
        break;
    case GLUT_KEY_UP:
        elbowBend += 5.0f;
        if (elbowBend > 150.0f)
            elbowBend = 150.0f;
        break;
    case GLUT_KEY_DOWN:
        elbowBend -= 5.0f;
        if (elbowBend < -150.0f)
            elbowBend = -150.0f;
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
	GLint i;
	
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( windowWidth, windowHeight );
	glutCreateWindow( "Vertex Blending Demo" );
	glutReshapeFunc( ChangeSize );
	glutKeyboardFunc( KeyPressFunc );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );

	SetupRC();
	
	mainMenu = glutCreateMenu( ProcessMenu );
	glutAddMenuEntry( "Toggle vertex blending (currently ON)", 1 );
	glutAddMenuEntry( "Show bones (currently OFF)", 2 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glutMainLoop();
	
	if ( glDeleteObjectARB )
	{
		for( i = 0; i < TOTAL_SHADERS; i++ )
		{
			glDeleteObjectARB( progObj[i] );
			glDeleteObjectARB( vShader[i] );
		}
	}
	
	return 0;
}
