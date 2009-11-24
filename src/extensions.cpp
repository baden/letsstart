#include<stdio.h>

#include "extensions.h"

#define INIT_EXTENSION(p,n) \
	n = (p)wglGetProcAddress(#n); \
	if (NULL == n) \
	{ \
		fprintf(stderr, "%s fail\n", #n); \
		return false; \
	} else { \
		fprintf(stderr, "%s ok\n", #n); \
	}

PFNGLCREATEPROGRAMPROC               glCreateProgram              = NULL;
PFNGLCREATESHADERPROC                glCreateShader               = NULL;
PFNGLDELETEPROGRAMPROC               glDeleteProgram              = NULL;
PFNGLDELETESHADERPROC                glDeleteShader               = NULL;
PFNGLATTACHSHADERPROC                glAttachShader               = NULL;
PFNGLDETACHSHADERPROC                glDetachShader               = NULL;
PFNGLCOMPILESHADERPROC               glCompileShader              = NULL;
PFNGLSHADERSOURCEPROC                glShaderSource               = NULL;
PFNGLUSEPROGRAMPROC                  glUseProgram                 = NULL;
PFNGLLINKPROGRAMPROC                 glLinkProgram                = NULL;
PFNGLGETPROGRAMIVPROC                glGetProgramiv               = NULL;
PFNGLGETSHADERIVPROC                 glGetShaderiv                = NULL;
PFNGLGETPROGRAMINFOLOGPROC           glGetProgramInfoLog          = NULL;
PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog           = NULL;
PFNGLVALIDATEPROGRAMPROC             glValidateProgram            = NULL;
PFNGLGETUNIFORMLOCATIONPROC          glGetUniformLocation         = NULL;
PFNGLGETATTRIBLOCATIONPROC           glGetAttribLocation          = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC     glEnableVertexAttribArray    = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC    glDisableVertexAttribArray   = NULL;
PFNGLVERTEXATTRIBPOINTERPROC         glVertexAttribPointer        = NULL;
PFNGLACTIVETEXTUREPROC               glActiveTexture              = NULL;
PFNGLUNIFORMMATRIX4FVPROC            glUniformMatrix4fv           = NULL;
PFNGLUNIFORM1IPROC                   glUniform1i                  = NULL;
PFNGLUNIFORM4FVPROC                  glUniform4fv                 = NULL;
PFNGLUNIFORM3FVPROC                  glUniform3fv                 = NULL;
PFNGLUNIFORM3FPROC                   glUniform3f = NULL;

PFNGLGENVERTEXARRAYSPROC	glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC	glBindVertexArray = NULL;
PFNGLGENBUFFERSPROC	glGenBuffers = NULL;
PFNGLBINDBUFFERPROC	glBindBuffer = NULL;
PFNGLBUFFERDATAPROC	glBufferData = NULL;
PFNGLBINDATTRIBLOCATIONPROC	glBindAttribLocation = NULL;
//PFNGLCLEARCOLORPROC	glClearColor = NULL;
//PFNGLCLEARPROC	glClear = NULL;
//PFNGLDRAWARRAYSPROC	glDrawArrays = NULL;
PFNGLDELETEBUFFERSPROC	glDeleteBuffers = NULL;
PFNGLDELETEVERTEXARRAYSPROC	glDeleteVertexArrays = NULL;

PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = NULL;
PFNGLGENFRAMEBUFFERSPROC  glGenFramebuffers = NULL;
PFNGLDELETEFRAMEBUFFERSPROC  glDeleteFramebuffers = NULL;
PFNGLGENRENDERBUFFERSPROC  glGenRenderbuffers = NULL;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = NULL;

bool initialize_opengl_extensions()
{
	INIT_EXTENSION(PFNGLCREATEPROGRAMPROC,              glCreateProgram);
	INIT_EXTENSION(PFNGLCREATESHADERPROC,               glCreateShader);
	INIT_EXTENSION(PFNGLDELETEPROGRAMPROC,              glDeleteProgram);
	INIT_EXTENSION(PFNGLDELETESHADERPROC,               glDeleteShader);
	INIT_EXTENSION(PFNGLATTACHSHADERPROC,               glAttachShader);
	INIT_EXTENSION(PFNGLDETACHSHADERPROC,               glDetachShader);
	INIT_EXTENSION(PFNGLCOMPILESHADERPROC,              glCompileShader);
	INIT_EXTENSION(PFNGLSHADERSOURCEPROC,               glShaderSource);
	INIT_EXTENSION(PFNGLUSEPROGRAMPROC,                 glUseProgram);
	INIT_EXTENSION(PFNGLLINKPROGRAMPROC,                glLinkProgram);
	INIT_EXTENSION(PFNGLGETPROGRAMIVPROC,               glGetProgramiv);
	INIT_EXTENSION(PFNGLGETSHADERIVPROC,                glGetShaderiv);
	INIT_EXTENSION(PFNGLGETPROGRAMINFOLOGPROC,          glGetProgramInfoLog);
	INIT_EXTENSION(PFNGLGETSHADERINFOLOGPROC,           glGetShaderInfoLog);
	INIT_EXTENSION(PFNGLVALIDATEPROGRAMPROC,            glValidateProgram);
	INIT_EXTENSION(PFNGLGETUNIFORMLOCATIONPROC,         glGetUniformLocation);
	INIT_EXTENSION(PFNGLGETATTRIBLOCATIONPROC,          glGetAttribLocation);
	INIT_EXTENSION(PFNGLENABLEVERTEXATTRIBARRAYPROC,    glEnableVertexAttribArray);
	INIT_EXTENSION(PFNGLDISABLEVERTEXATTRIBARRAYPROC,   glDisableVertexAttribArray);
	INIT_EXTENSION(PFNGLVERTEXATTRIBPOINTERPROC,        glVertexAttribPointer);
	INIT_EXTENSION(PFNGLACTIVETEXTUREPROC,              glActiveTexture);
	INIT_EXTENSION(PFNGLUNIFORMMATRIX4FVPROC,           glUniformMatrix4fv);
	INIT_EXTENSION(PFNGLUNIFORM1IPROC,                  glUniform1i);
	INIT_EXTENSION(PFNGLUNIFORM4FVPROC,                 glUniform4fv);
	INIT_EXTENSION(PFNGLUNIFORM3FVPROC,                 glUniform3fv);
	INIT_EXTENSION(PFNGLUNIFORM3FPROC,                 glUniform3f);

	INIT_EXTENSION(PFNGLGENVERTEXARRAYSPROC,                glGenVertexArrays);
	INIT_EXTENSION(PFNGLBINDVERTEXARRAYPROC,	glBindVertexArray);
	INIT_EXTENSION(PFNGLGENBUFFERSPROC,	glGenBuffers);
	INIT_EXTENSION(PFNGLBINDBUFFERPROC,	glBindBuffer);
	INIT_EXTENSION(PFNGLBUFFERDATAPROC,	glBufferData);
	INIT_EXTENSION(PFNGLBINDATTRIBLOCATIONPROC,	glBindAttribLocation);
//	INIT_EXTENSION(PFNGLCLEARCOLORPROC,	glClearColor);
//	INIT_EXTENSION(PFNGLCLEARPROC,	glClear);
//	INIT_EXTENSION(PFNGLDRAWARRAYSPROC,	glDrawArrays);
	INIT_EXTENSION(PFNGLDELETEBUFFERSPROC,	glDeleteBuffers);
	INIT_EXTENSION(PFNGLDELETEVERTEXARRAYSPROC,	glDeleteVertexArrays);

	INIT_EXTENSION(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus);
	INIT_EXTENSION(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
	INIT_EXTENSION(PFNGLDELETEFRAMEBUFFERSPROC,  glDeleteFramebuffers);
	INIT_EXTENSION(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers);
	INIT_EXTENSION(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers);
    INIT_EXTENSION(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
    INIT_EXTENSION(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
    INIT_EXTENSION(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer);
    INIT_EXTENSION(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage);
    INIT_EXTENSION(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer);


	return true;
}
