#ifndef GL99_EXTENSIONS_H
#define GL99_EXTENSIONS_H

//#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include <stdbool.h>

#include <GL3/gl3.h>
//#include <GL/glu.h>
//#include "GL/glext.h"
//#include "GL/wglext.h"

//#include "logger.h"

extern PFNGLCREATEPROGRAMPROC               glCreateProgram;
extern PFNGLCREATESHADERPROC                glCreateShader;
extern PFNGLDELETEPROGRAMPROC               glDeleteProgram;
extern PFNGLDELETESHADERPROC                glDeleteShader;
extern PFNGLATTACHSHADERPROC                glAttachShader;
extern PFNGLDETACHSHADERPROC                glDetachShader;
extern PFNGLCOMPILESHADERPROC               glCompileShader;
extern PFNGLSHADERSOURCEPROC                glShaderSource;
extern PFNGLUSEPROGRAMPROC                  glUseProgram;
extern PFNGLLINKPROGRAMPROC                 glLinkProgram;
extern PFNGLGETPROGRAMIVPROC                glGetProgramiv;
extern PFNGLGETSHADERIVPROC                 glGetShaderiv;
extern PFNGLGETPROGRAMINFOLOGPROC           glGetProgramInfoLog;
extern PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog;
extern PFNGLVALIDATEPROGRAMPROC             glValidateProgram;
extern PFNGLGETUNIFORMLOCATIONPROC          glGetUniformLocation;
extern PFNGLGETATTRIBLOCATIONPROC           glGetAttribLocation;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC     glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC    glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC         glVertexAttribPointer;
extern PFNGLACTIVETEXTUREPROC               glActiveTexture;
extern PFNGLUNIFORMMATRIX4FVPROC            glUniformMatrix4fv;
extern PFNGLUNIFORM1IPROC                   glUniform1i;
extern PFNGLUNIFORM4FVPROC                  glUniform4fv;
extern PFNGLUNIFORM3FVPROC                  glUniform3fv;
extern PFNGLUNIFORM3FPROC                   glUniform3f;

extern PFNGLGENVERTEXARRAYSPROC	glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC	glBindVertexArray;
extern PFNGLGENBUFFERSPROC	glGenBuffers;
extern PFNGLBINDBUFFERPROC	glBindBuffer;
extern PFNGLBUFFERDATAPROC	glBufferData;
extern PFNGLBINDATTRIBLOCATIONPROC	glBindAttribLocation;
//extern PFNGLCLEARCOLORPROC	glClearColor;
//extern PFNGLCLEARPROC	glClear;
//extern PFNGLDRAWARRAYSPROC	glDrawArrays;
extern PFNGLDELETEBUFFERSPROC	glDeleteBuffers;
extern PFNGLDELETEVERTEXARRAYSPROC	glDeleteVertexArrays;



bool initialize_opengl_extensions();

/*
GLAPI void APIENTRY glClearColor (GLclampf, GLclampf, GLclampf, GLclampf);
GLAPI void APIENTRY glClear (GLbitfield);
GLAPI void APIENTRY glDrawArrays (GLenum, GLint, GLsizei);
GLAPI void APIENTRY glEnable (GLenum);
GLAPI void APIENTRY glDepthFunc (GLenum);
GLAPI void APIENTRY glDrawElements (GLenum, GLsizei, GLenum, const GLvoid *);
*/

#endif /* GL99_EXTENSIONS_H */
