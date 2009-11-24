//#include "common.h"
//#include "colours.h"

#include "letters.h"
//#include <GL/gl.h>
#include"gl/gl.h"
#include <gl3/gl3.h>

#include <string.h>

#include "extensions.h"
//#include "ply.h"
//#include "circle.h"
//#include <stdio.h>
//#include "physic.h"
#include "gltext.h"
#include "gldraw.h"
#include "matrix.h"
#include "shader.h"
#include "math.h"

GLuint letters_textures[(MAX_LETTER+1)*2];

#define MAX_FLYTEXT 20

typedef struct{
    float x;
    float y;
    char text[64];
    float alpha;
    float scale;
} tflytext;

#define PIXEL_FORMAT GL_RGBA


tflytext flytexts[MAX_FLYTEXT];
int flytext_count = 0;

typedef struct scolours{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} tcolours;

GLuint charquad_vao;
GLuint charquad_vbo[3];

#define _cq(f) charquadvert[chi++] = floatToHALF(f);

void textInit(void)
{
#if 1
    const GLfloat charquadvert[4*2] = {
        0.0, 0.0,   /* index 0 */
        0.0, 1.0,   /* index 1 */
        1.0, 1.0,   /* index 2 */
        1.0, 0.0    /* index 3 */
    };
    const GLubyte charquadind[6] = { 0, 2, 1, 0, 3, 2 };
#endif

    //glUseProgram(shader_vertexnormaluvcolor.shaderprogram);
    ShaderSetCurent(SHADER_TEXT);

    glGenVertexArrays(1, &charquad_vao);
    glBindVertexArray(charquad_vao);
    glGenBuffers(3, charquad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, charquad_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(charquadvert), charquadvert, GL_STATIC_DRAW);

    glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);    // vertex (in_Position)
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, charquad_vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(charquadind), charquadind, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    glDisableVertexAttribArray(0);

    tcolours symbol[8*8];
    memset(symbol, 0, sizeof(symbol));


//    glBindBuffer(GL_TEXTURE_BUFFER, charquad_vbo[2]);

    glGenTextures((MAX_LETTER+1)*2, letters_textures);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // need this since RGB/BGR images are stored
                                           // without padding.
    for(int l=0; l<=MAX_LETTER; l++){
		for(int j=0; j<BLETTER_H; j++){
			unsigned char bits=bletter[l][BLETTER_H-j-1];
		 	for(int i=0; i<5; i++){
				if(bits&0x10){
				    symbol[j*8+i+9].r = symbol[j*8+i+9].g = symbol[j*8+i+9].b = symbol[j*8+i+9].a = 255;
				} else {
				    symbol[j*8+i+9].r = symbol[j*8+i+9].g = symbol[j*8+i+9].b = symbol[j*8+i+9].a = 0;
				}
				bits<<=1;
			}
		}
        glBindTexture(GL_TEXTURE_2D, letters_textures[l]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, symbol);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, letters_textures[(MAX_LETTER+1) + l]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, symbol);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }

//    glBufferData ( GL_TEXTURE_BUFFER, 16, symbol, GL_STREAM_DRAW );

//    glBindBuffer(GL_TEXTURE_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
//    if(pboSupported){
}

void textDone(void)
{
    glDeleteTextures((MAX_LETTER+1)*2, letters_textures);
    glDeleteBuffers(3, charquad_vbo);
    glDeleteVertexArrays(1, &charquad_vao);
}

char draw_text[128];

extern GLfloat modelmatrix[16]; /* Our model matrix  */

void DrawChar(unsigned char ch, bool smooth)
{
    unsigned char d = 0xFF;
    if((ch==' ')) d = 36;
    else if((ch==':')) d = 37;
    else if((ch=='<')) d = 38;
    else if((ch=='>')) d = 39;
    else if((ch=='.')) d = 40;
    else if((ch==',')) d = 40;
    else if((ch=='+')) d = 41;
    else if((ch=='-')) d = 42;
    else if((ch>='0')&&(ch<='9')) d = ch - '0';
    else if((ch>='A')&&(ch<='Z')) d = ch - 'A' + 10;
    else if((ch>=192)&&(ch<=223)) d = ch - 192 + 43;

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);

    if(d!=0xFF){
        glBindTexture(GL_TEXTURE_2D, letters_textures[d+ (smooth?(MAX_LETTER+1):0)]);

    //glBindBuffer(GL_TEXTURE_BUFFER, charquad_vbo[2]);
//    glBindBuffer(GL_TEXTURE_BUFFER, 0);

    glBindVertexArray(charquad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, charquad_vbo[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, charquad_vbo[1]);

    ShaderDoModelMatrix(modelmatrix);
    //glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, modelmatrix);
    translate(modelmatrix, 0.03, 0.0, 0.0);

//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
//    glEnableVertexAttribArray(2);
//    glDisableVertexAttribArray(3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
#if 0
#if 1
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid*)0);
//    glDrawElements(GL_QUADS, 8, GL_UNSIGNED_SHORT, (GLvoid*)0);
#else
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
#endif
#endif

//        glBegin(GL_QUADS);
//        glTexCoord2f(1.0, 1.0);	glVertex2f(6,6);
//        glTexCoord2f(0.0, 1.0); glVertex2f(0,6);
//        glTexCoord2f(0.0, 0.0); glVertex2f(0,0);
//        glTexCoord2f(1.0, 0.0); glVertex2f(6,0);
//        glEnd();
    }
}

void drawText(float x, float y, int app)
{
	char *text = draw_text;
//	float text_shift = (float)strlen(text)/3.65;

    GLfloat identitymatrix[16] = IDENTITY_MATRIX4;

    ShaderSetCurent(SHADER_TEXT);
    //ShaderSetCurent(SHADER_VERTEX_NORMAL);
    //shader_current = &shader_text;
    //glUseProgram(shader_current->shaderprogram);

    ShaderDoProjectionMatrix(identitymatrix);
    //glUniformMatrix4fv(shader_current->projection_u, 1, GL_FALSE, identitymatrix);
//	glUniform4fv(shader_current->light_pos_u, 1, light_pos);

    GLfloat text_color[3] = {0.0f, 1.0f, 1.0f};
//    glUniform3fv(shader_current->colour_u, 1, text_color);
    ShaderDoColor(text_color);

//    multiply4x4n(commmatrix, modelmatrix, cameramatrix);

    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
    scale(modelmatrix, 0.04, 0.04, 1.0);
    translate(modelmatrix, x, y, -1.0);

//    glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, modelmatrix);
    ShaderDoModelMatrix(modelmatrix);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    if(app == TXT_APP_NORMAL){
//        glPushMatrix();
//        glTranslatef(x+0.05 - text_shift, y-0.05, 0.0f);
//        glScalef(0.1, 0.1, 1.0);

//        glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
        while(*text){
            DrawChar(*text, true);
            //DrawChar(*text, false);
            text++;
//            glTranslatef(5.5f, 0.0f, 0.0f);
        }
//        glPopMatrix();
    }

#if 0
    glPushMatrix();
    glTranslatef(x - text_shift, y, 0.0f);
    glScalef(0.1, 0.1, 1.0);
	text = draw_text;
	if(app == TXT_APP_NORMAL) glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//	else glColor3f(0.0f, 1.0f, 1.0f);
	while(*text){
	    DrawChar(*text, (app == TXT_APP_NORMAL)?false:true);
	    text++;
        glTranslatef(5.5f, 0.0f, 0.0f);
	}
    glPopMatrix();
#endif
//    glBindBuffer(GL_TEXTURE_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
}

void addFlyText(float x, float y, float scale, const char *text)
{
    if(flytext_count < MAX_FLYTEXT-2){
        flytexts[flytext_count].x = x;
        flytexts[flytext_count].y = y;
        strncpy(flytexts[flytext_count].text, text, sizeof(flytexts[0].text)-1);
        flytexts[flytext_count].alpha = 2.0f;
        flytexts[flytext_count].scale = scale;
        flytext_count++;
    }
}
