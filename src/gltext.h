#ifndef GLTEXT_H_INCLUDED
#define GLTEXT_H_INCLUDED

//#include<GL/gl.h>
void textInit(void);
void textDone(void);

extern char draw_text[128];

void drawText(GLfloat x, GLfloat y, int app);
void addFlyText(float x, float y, float scale, const char *text);
void drawFlyTexts(void);


#define TXT_APP_NORMAL  0
#define TXT_APP_GLOW  1



#endif // GLTEXT_H_INCLUDED
