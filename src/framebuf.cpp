#include <stdlib.h>
#include <stdio.h>
#include "framebuf.h"

#include <gl/gl.h>
#include <gl3/gl3.h>
#include "extensions.h"
#include "shader.h"
#include "matrix.h"

void CheckFramebufferStatus()
{
    GLenum status;
    status = (GLenum) glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE:
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            printf("Unsupported framebuffer format\n");
            fprintf(stderr, "Unsupported framebuffer format");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            printf("Framebuffer incomplete, missing attachment\n");
            fprintf(stderr, "Framebuffer incomplete, missing attachment");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            printf("Framebuffer incomplete, attached images must have same dimensions\n");
            fprintf(stderr, "Framebuffer incomplete, attached images must have same dimensions");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            printf("Framebuffer incomplete, attached images must have same format\n");
            fprintf(stderr, "Framebuffer incomplete, attached images must have same format");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            printf("Framebuffer incomplete, missing draw buffer\n");
            fprintf(stderr, "Framebuffer incomplete, missing draw buffer");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            printf("Framebuffer incomplete, missing read buffer\n");
            fprintf(stderr, "Framebuffer incomplete, missing read buffer");
            break;
        default:
            fprintf(stderr, "Error %x\n", status);
			break;
    }
}

GLuint fb[2];
GLuint textureID[2];
GLuint depth_rb;
int fb_init = 0;
int fb_lazzy = 1;

void initRT(int n, int w, int h)
{
  glBindFramebuffer(GL_FRAMEBUFFER, fb[n]);
  //
  // init texture
  //
  glBindTexture(GL_TEXTURE_2D, textureID[n]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
  //
  // SEEMS VERY IMPORTANT for the FBO to be valid. ARGL.
  //
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID[n], 0);
  CheckFramebufferStatus();
}

GLuint vao;
GLuint vbo[2];

void FrameBufInit(int w, int h)
{
    if(fb_lazzy) return;
    fprintf(stderr, "Init framebuffer.\n");
    fb_init = 1;
    glGenFramebuffers(2, fb);
    glGenTextures(2, textureID);
    glGenRenderbuffers(1, &depth_rb);
    initRT(0, w, h);
    initRT(1, w, h);
  //
  // initialize depth renderbuffer
  //
  glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);
// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, depth_rb);
  CheckFramebufferStatus();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    const GLfloat charquadvert[4*(3+3+2)] = {
        -1.0, -1.0, 0.0,  0.0, 0.0, 1.0,  0.0, 0.0, /* index 0 */
        -1.0,  1.0, 0.0,  0.0, 0.0, 1.0,  0.0, 1.0, /* index 1 */
         1.0,  1.0, 0.0,  0.0, 0.0, 1.0,  1.0, 1.0, /* index 2 */
         1.0, -1.0, 0.0,  0.0, 0.0, 1.0,  1.0, 0.0,  /* index 3 */
    };
    const GLubyte charquadind[6] = { 0, 2, 1, 0, 3, 2 };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(charquadvert), charquadvert, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(charquadind), charquadind, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

//glCheckFramebufferStatus()
void FrameBufDone()
{
    if(fb_lazzy) return;
    if(fb_init){
        fb_init = 0;
        fprintf(stderr, "Done framebuffer.\n");

        glDeleteBuffers(2, vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteRenderbuffers(1, &depth_rb);
        glDeleteTextures(2, textureID);
        glDeleteFramebuffers(2, fb);
    }
}
#include "object.h"
void FrameBufDraw()
{

  glBindFramebuffer(GL_FRAMEBUFFER_EXT, fb[0]);
  glClearColor( 0.3f, 0.2f, 0.1f, 1.f );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ObjectDrawArrays();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);


GLfloat modelmatrix[16] = IDENTITY_MATRIX4; /* Our model matrix  */
GLfloat projectionmatrix[16] = IDENTITY_MATRIX4;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 8*4, (GLvoid*)0);    // vertex (in_Position)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 8*4, (GLvoid*)(3*4));    // vertex (in_Position)
    glEnableVertexAttribArray(1);

//    ShaderSetCurent(SHADER_VERTEX_NORMAL);
    //glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, modelmatrix);
//    translate(modelmatrix, 0.2, 0.2, -0.5);
//    ShaderDoProjectionMatrix(projectionmatrix);
//    scale(modelmatrix, 0.5, 0.5, 0.5);
//    ShaderDoModelMatrix(modelmatrix);
    float color[3] = {1.0, 0.0, 1.0};
//    ShaderDoColor(color);
    ShaderDo(SHADER_VERTEX_NORMAL, projectionmatrix, modelmatrix, color);

    glDisable(GL_DEPTH_TEST);

//  glBindFramebuffer(GL_FRAMEBUFFER, fb[1]);
//  glBindTexture(GL_TEXTURE_2D, 0);

//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
//    glEnableVertexAttribArray(2);
//    glDisableVertexAttribArray(3);
//  glPushAttrib(GL_VIEWPORT_BIT);
 // glViewport(0,0,1024,768);

//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glBindTexture(GL_TEXTURE_2D, textureID[0]);

    //scale(modelmatrix, 0.5, 0.5, 0.5);
//    ShaderDoModelMatrix(modelmatrix);
    color[0] = 0.0;
//    ShaderDoColor(color);
    ShaderDo(SHADER_VERTEX_NORMAL_UV, projectionmatrix, modelmatrix, color);

    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 8*4, (GLvoid*)(6*4));    // vertex (in_Position)
    glEnableVertexAttribArray(2);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
    glEnable(GL_DEPTH_TEST);

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
   glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
