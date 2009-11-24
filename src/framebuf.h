#ifndef _FRAMEBUF_H_
#define _FRAMEBUF_H_

void FrameBufInit(int w, int h);
void FrameBufDone();
void FrameBufDraw();
extern int fb_lazzy;



#endif

