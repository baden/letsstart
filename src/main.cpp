#include <windows.h>
#include <stdio.h>
//#include <GL/glu.h>
#include <gl/gl.h>
#include <gl3/gl3.h>
#include <math.h>
#include "GL/glext.h"
#include "GL/wglext.h"
#include "extensions.h"
#include "matrix.h"
//#include "ply.h"
#include "time.h"

#include "gltext.h"
#include "gldraw.h"
#include "shader.h"
#include "scripts.h"
#include "object.h"

//#define MULTISAMPLE 16
//#define MULTISAMPLE 8
#define MULTISAMPLE 4
//#define MULTISAMPLE 2

#include "math.h"

HWND hWnd = 0;               /* This is the handle for our window */
HDC hDC = 0;
HGLRC hRC = 0;

int swapinterval = 1;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

//BOOL block_quit = FALSE;

/*  Make the class name into a global variable  */
char szClassName[ ] = "CodeBlocksWindowsApp";

float fps;

PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;

//GLuint vao, vbo[2]; /* Create handles for our Vertex Array Object and three Vertex Buffer Objects */

//tPlyModel model01;
//tPlyModel model02;
//tPlyModel model03;
//tPlyModel model04;
//tPlyModel flatfloor;

GLfloat cameramatrix[16] = IDENTITY_MATRIX4;
GLfloat projectionmatrix[16]; /* Our projection matrix starts with all 0s */
GLfloat modelmatrix[16]; /* Our model matrix  */

void DoPrepare()
{

    /* The four vericies of a tetrahedron */
#if 0
    const GLfloat tetrahedron[4][3] = {
    {  1.0,  1.0,  1.0  },   /* index 0 */
    { -1.0, -1.0,  1.0  },   /* index 1 */
    { -1.0,  1.0, -1.0  },   /* index 2 */
    {  1.0, -1.0, -1.0  } }; /* index 3 */

    /* Color information for each vertex */
    const GLfloat colors[4][3] = {
    {  1.0,  0.0,  0.0  },   /* red */
    {  0.0,  1.0,  0.0  },   /* green */
    {  0.0,  0.0,  1.0  },   /* blue */
    {  1.0,  1.0,  1.0  } }; /* white */

    const GLubyte tetraindicies[6] = { 0, 1, 2, 3, 0, 1 };
#endif

//    PlyLoader("bochka.bdm", &model01);
//    PrepareProgram();

    initShaders();

//    glUseProgram(shader_vertexnormal.shaderprogram);
    ShaderSetCurent(SHADER_VERTEX_NORMAL);

//    PlyLoader("blender/char1.bdm", &model01);
//    PlyLoader("../blender/char2c.bdm", &model01);
//    PlyLoader("../blender/char5.bdm", &model02);
//    PlyLoader("../blender/char1.bdm", &model03);
//    PlyLoader("../blender/char6.bdm", &model04);
//        glDisableVertexAttribArray(3);

//    PlyLoader("../blender/flatfloor.bdm", &flatfloor);
//    glUseProgram(shaderprogram);
        //PlyLoader("blender/box.bdm", &model01);
    ShaderSetCurent(SHADER_TEXT);
    textInit();
    scripttest();
/*
    float f1, f2;
    HALF h;

    fprintf(stderr, "Size of GLhalfARB = %d\n", sizeof(GLhalfARB));
    fprintf(stderr, "Size of HALF = %d\n", sizeof(HALF));

    for(f1 = -2.0f; f1 < 2.0f; f1+=0.1){
        h = floatToHALF(f1);
        f2 = HALFToFloat(h);
        fprintf(stderr, "Resilt for %f -> HALF -> float = %f  (diff=%f)\n", f1, f2, f2-f1);
    }
*/
    ObjectLoad("../blender/models/Zabor.bdsm2.gz");
    ObjectLoad("../blender/models/FlatFloor.bdsm2.gz");
    ObjectLoad("../blender/models/Sintel.bdsm2.gz");
    ObjectLoad("../blender/models/Animation.bdsm2.gz");
//    ObjectLoad("../blender/models/Collect1.bdsm2.gz");
    ObjectLoad("../blender/models/Medusa.bdsm2.gz");
//    ObjectLoad("../blender/models/Molot.bdsm2.gz");
//    ObjectLoad("../blender/models/Quad.bdsm2.gz");
//    ObjectLoad("../blender/models/Quad.bdsm2.gz");
//    ObjectLoad("../blender/models/NotEvFrame.bdsm2.gz");
//    ObjectLoad("../blender/models/Wall001.bdsm2.gz");
    ObjectLoad("../blender/models/Animation.bdsm2.gz");
//    ObjectLoad("../blender/models/ManyMeshes.bdsm2.gz");
    ObjectLoad("../blender/models/Kaktus.bdsm2.gz");
    ObjectLoad("../blender/models/Palm.bdsm2.gz");

    ObjectStatistic();

//    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
//    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);                             // Включается плавное затенение
	glEnable(GL_DITHER);
	glEnable(GL_LINE_SMOOTH);                            // Включается сглаживание линий
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              // Выбирается самый качественный
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);              // Выбирается самый качественный
	glEnable(GL_BLEND);                                  // Включается смешение цветов, необходимое
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // для работы сглаживания и задается

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
//	glPolygonMode(GL_BACK,GL_LINE);
//	glPolygonMode(GL_BACK, GL_FILL);
//	glCullFace(GL_FRONT);
	glLineWidth(4);
	//glEnable(GL_LINE_WIDTH);
	glPointSize(4);

}

void DoDone()
{
    ObjectUnloadAll();
    textDone();
//    PlyDone(&flatfloor);
//    PlyDone(&model04);
//    PlyDone(&model03);
//    PlyDone(&model02);
//    PlyDone(&model01);

//    glDeleteBuffers(2, vbo);
//    glDeleteVertexArrays(1, &vao);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    doneShaders();

}

//GLfloat light_pos[4] = {-10.0f, 10.0f, 20.0f, 1.0f};
GLfloat cur_color[3] = {1.0f, 1.0f, 1.0f};

#define CUR_COLOR(r, g, b) {cur_color[0] = r; cur_color[1] = g; cur_color[2] = b;}

void DoDraw(HDC hDC)
{
    static __int64 pr1, pr2, pr3;
    //static float camera[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    static float ix = 0.0f;
    static float iy = 0.0f;
    static float iz = 0.0f;

    static int big_frame = 0;
    big_frame++;
    int frame = big_frame / 2;

    pr1 = my_time();
    const GLfloat identitymatrix[16] = IDENTITY_MATRIX4;

    glClearColor( 0.3f, 0.2f, 0.1f, 1.f );
//    glClear(GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glEnable(GL_CULL_FACE);
    ObjectPrepareDraw();

    perspective(projectionmatrix, 45.0, (float)1024/768, 0.1, 100.0);

    memcpy(cameramatrix, identitymatrix, sizeof(GLfloat) * 16);
    rotate(cameramatrix, 40, X_AXIS);
    rotate(cameramatrix, 00, Y_AXIS);
    //rotate(cameramatrix, 00, Z_AXIS);
    scale(cameramatrix, 0.2, 0.2, 0.2);
    translate(cameramatrix, -4.6, -1.5, -5.0);

//    glUniformMatrix4fv(shader_current->projection_u, 1, GL_FALSE, projectionmatrix);

//    light_pos[0] = 2.0f * sin(ix/10.0);
//    light_pos[1] = 2.0f * sin(ix/12.0);
//    light_pos[2] = 2.0f * sin(ix/18.0);

//	glUniform4fv(shader_current->light_pos_u, 1, light_pos);


/*
    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
    translate(modelmatrix, light_pos[0], light_pos[1], light_pos[2]);
    scale(modelmatrix, 0.1f, 0.1f, 0.1f);
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "modelview"), 1, GL_FALSE, modelmatrix);
    glUniform3f(glGetUniformLocation(shaderprogram, "colour"), 1.0f, 1.0f, 1.0f);
    PlyDraw(&model01);
*/
//    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
//    rotate(modelmatrix, 45, Z_AXIS);
//    rotate(modelmatrix, 40, Y_AXIS);
//    rotate(modelmatrix, 40, Z_AXIS);
//    translate(modelmatrix, -1.3, 0, -15.0);

//    memcpy(modelmatrix, cameramatrix, sizeof(GLfloat) * 16);

#if 1
//    translate(modelmatrix, -1.3, 0, -15.0);

    //multiply4x4(modelmatrix, camera);

    //glUniform3f(shader_current->colour_u, 1.0f, 1.0f, 1.0f);

    //cur_color[0] = 1.0f; , 1.0f, 1.0f};
    //memcpy(commmatrix, identitymatrix, sizeof(GLfloat) * 16);

#define MAP_DX 2.00
#define MAP_DY 2.00
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
    for(int iy=0; iy<16; iy++){
        for(int ix=0; ix<24; ix++){
            CUR_COLOR(0.5f, 0.5f, 0.0f);
//            PlyDraw(&flatfloor);
            ObjectDraw(1, frame);

            if(ix!=2 && ix!=14 && iy && iy!=14){
                rotate_r(modelmatrix, 25.0*(ix+iy*2), Z_AXIS);
//            multiply4x4n(commmatrix, modelmatrix, cameramatrix);
//            glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, commmatrix);
                CUR_COLOR(0.0f, 1.0f, 0.0f);
                //PlyDraw(&model02);
                ObjectDraw(3+((ix+iy)%5), frame);
                rotate_r(modelmatrix, -25.0*(ix+iy*2), Z_AXIS);
            }
            translate(modelmatrix, MAP_DX, 0.0, 0.0);
            //scale(modelmatrix, 0.9, 0.9, 0.9);
        }
        translate(modelmatrix, -MAP_DX*24.0, MAP_DY, 0.0);
    }

    CUR_COLOR(0.5f, 0.5f, 0.5f);
    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
    rotate_r(modelmatrix, 90, Z_AXIS);
    for(int ix=0; ix<24; ix++){
        //PlyDraw(&model04);
        ObjectDraw(0, frame);
        translate(modelmatrix, 0.00, MAP_DY*14, 0.0);
        ObjectDraw(0, frame);
        //PlyDraw(&model04);
        translate(modelmatrix, MAP_DX, -MAP_DY*14, 0.0);
    }
    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
    for(int ix=0; ix<15; ix++){
        //PlyDraw(&model04);
        ObjectDraw(0, frame);
        translate(modelmatrix, MAP_DX*14, 0.00, 0.0);
        //PlyDraw(&model04);
        ObjectDraw(0, frame);
        translate(modelmatrix, -MAP_DX*14, MAP_DY, 0.0);
    }
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

#if 1
    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
    memcpy(cameramatrix, identitymatrix, sizeof(GLfloat) * 16);

    rotate(modelmatrix, 90, X_AXIS);

    //rotate(modelmatrix, ix, X_AXIS);
    rotate(modelmatrix, iy, Y_AXIS);
    //rotate(modelmatrix, iz, Z_AXIS);
    ix+=0.16f;
    iy+=0.12f;
    iz+=0.6f;

    translate(modelmatrix, -1.3, 0, -5.0);
    CUR_COLOR(1.0f, 1.0f, 1.0f);
//    glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, modelmatrix);
//    glUniform3f(shader_current->colour_u, 1.0f, 1.0f, 1.0f);
//    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, 0);
    //glDrawElements(GL_TRIANGLES, model01.index_size, GL_UNSIGNED_SHORT, (GLvoid*)0);
//    PlyDraw(&model01);
    translate(modelmatrix, 0, -1.5, 0);
    scale(modelmatrix, 0.4, 0.4, 0.4);
    ObjectDraw(0, frame);

    translate(modelmatrix, 2.6f, 1.5, 0.0);
    scale(modelmatrix, 2.0f, 2.0f, 2.0f);

    CUR_COLOR(0.0f, 0.0f, 1.0f);
//    glUniform3f(shader_current->colour_u, 0.0f, 0.0f, 1.0f);
//    glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, modelmatrix);
    //glDrawElements(GL_TRIANGLES, model01.index_size, GL_UNSIGNED_SHORT, (GLvoid*)0);
    glColor3f(1.0, 0.0, 0.0);
    //glDisableVertexAttribArray(3);
//    PlyDraw(&model03);
    ObjectDraw(1, frame);

    translate(modelmatrix, -2.8f, -1.7f, 0.5);
    scale(modelmatrix, 0.1f, 0.1f, 0.1f);
    CUR_COLOR(1.0f, 1.0f, 1.0f);
//    glUniform3f(shader_current->colour_u, 0.0f, 1.0f, 0.0f);
//    glUniform3f(glGetUniformLocation(shaderprogram, "in_Color"), 0.0f, 1.0f, 0.0f);
//    glColor3f(1.0, 1.0, 1.0);

    //glDisable(GL_DEPTH_TEST);
    for(int i=0; i<18; i++){
//        glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, modelmatrix);
        //PlyDraw(&model02);
        ObjectDraw(3, frame);
        //glDrawElements(GL_TRIANGLES, model01.index_size, GL_UNSIGNED_SHORT, (GLvoid*)0);
        translate(modelmatrix, 0.2f, 0.0f, 0.0);
    }

#endif

    ObjectDrawArrays();
#if 0
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
//    glDisable(GL_DEPTH_TEST);
    ObjectDrawArrays();
//    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
#if 1
    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
//    multiply4x4n(commmatrix, modelmatrix, cameramatrix);
//    glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, commmatrix);

    pr2 = my_time() - pr1;

    sprintf(draw_text, "PROFILE   DRAWTIME:%06d   IDLETIME:%06d   FPS:%9.1f", (int)pr2, (int)pr3, fps);
    drawText(-0.99, 0.90, 0);

//        sprintf(draw_text, "PROFILE   DRAWTIME:%06d   IDLETIME:%06d   FPS:%9.1f  DUMMY STUPID TEXT DUMMY STUPID TEXT", (int)pr2, (int)pr3, fps);
//    for(float ty=0.8; ty>-0.8; ty-=0.02)
//        drawText(-0.99, ty, 0);

#endif
    //glEnable(GL_DEPTH_TEST);

/*
    glRotatef(1.0, 0.0, 0.0, 1.0);

    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glEnd();
*/
    SwapBuffers( hDC );
    pr3 = my_time() - pr1 - pr2;
}



/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
//        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_SIZE:
            int w = LOWORD (lParam);
            int h = HIWORD (lParam);
            int w1;

            if((w > 0) && (h > 0)){
                w1 = h*4/3;
                glViewport((w-w1)/2, 0, w1, h);
            }
            break;

        case WM_KEYDOWN:
			switch(wParam){
				case VK_SPACE: ; break;
//				case PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
			}
            break;

		case WM_CHAR:
            switch(wParam){
                case 'Q': case 'q': PostQuitMessage (0); break;
                case 'V': case 'v':
                    swapinterval = 1-swapinterval;
                    if(wglSwapIntervalEXT) wglSwapIntervalEXT(swapinterval);
                    break;
            }

            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

// Enable OpenGL
static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;

//#define     _PFD_() \
//    fprintf(stderr, "pfd = [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n", \
//  pfd.nSize, pfd.nVersion, pfd.dwFlags, pfd.iPixelType, pfd.cColorBits, pfd.cRedBits, pfd.cRedShift, pfd.cGreenBits, pfd.cGreenShift, \
//  pfd.cBlueBits, pfd.cBlueShift, pfd.cAlphaBits, pfd.cAlphaShift, pfd.cAccumBits, pfd.cAccumRedBits, pfd.cAccumGreenBits, \
//  pfd.cAccumBlueBits, pfd.cAccumAlphaBits, pfd.cDepthBits, pfd.cStencilBits, pfd.cAuxBuffers, pfd.iLayerType, pfd.bReserved, \
//  pfd.dwLayerMask, pfd.dwVisibleMask, pfd.dwDamageMask)

//void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
void DisableOpenGL();
void CreateGLWindow();
void DestroyGLWindow();

//void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
void EnableOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
//	char* extensions;

	fprintf(stderr, "Enabling OpenGL...\n");

	fprintf(stderr, " Creating temp OpenGL context...\n");

	// get the device context (DC)
	if(!(hDC =  GetDC(hWnd))){	// Did We Get A Device Context?
        fprintf(stderr, " Get Device Context failed.\n");
		ExitProcess(1);
	}

//	fprintf(stderr, "hDC=%08X\n", hDC);

	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.cStencilBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;

//    _PFD_();

	if (!(format = ChoosePixelFormat( hDC, &pfd ))){
	    fprintf(stderr, " ChoosePixelFormat failed.\n");
	    ExitProcess(1);
	}

//    fprintf(stderr, "Pixel format = %d\n", format);
//    _PFD_();

    //format = 56;

	if(!SetPixelFormat( hDC, format, &pfd )){
	    fprintf(stderr, " SetPixelFormat failed.\n");
	    ExitProcess(1);
	}

//    _PFD_();
	/* create temp render context */
	fprintf(stderr, " Creating temp Render context...\n");

	HGLRC temp_render_context = wglCreateContext(hDC);
	if (!temp_render_context || !wglMakeCurrent(hDC, temp_render_context))
	{
		fprintf(stderr, " Creating temp render context fail.\n");
		ExitProcess(1);
	}

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
//        WGL_SAMPLES_ARB, 4 ,            // проверка на 4x тип
		0, 0
	};

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	if (wglCreateContextAttribsARB == NULL)
	{
		fprintf(stderr, " wglGetProcAddress::wglCreateContextAttribsARB fail.\n");
		wglDeleteContext(temp_render_context);
		ExitProcess(1);
	}

#ifdef MULTISAMPLE
  int iAttributes[] = {
    WGL_DRAW_TO_WINDOW_ARB,GL_TRUE, // Истинна, если формат пикселя может быть использован в окне
    WGL_SUPPORT_OPENGL_ARB,GL_TRUE, // Истинна, если поддерживается OpenGL
    WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB, // Полная аппаратная поддержка
    WGL_COLOR_BITS_ARB,24,          // Цветность
    WGL_ALPHA_BITS_ARB,8,           // Размерность альфа-канала
    WGL_DEPTH_BITS_ARB,16,          // Глубина буфера глубины
    WGL_STENCIL_BITS_ARB,0,         // Глубина буфера шаблона
    WGL_DOUBLE_BUFFER_ARB,GL_TRUE,  // Истина, если используется двойная буферизация
    WGL_SAMPLE_BUFFERS_ARB,GL_TRUE, // Что мы и хотим
    WGL_SAMPLES_ARB, MULTISAMPLE ,            // проверка на 2x/4x тип
    0,0};
    // get proc address
    float fAttributes[] = {0,0};

    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
//    if(wglChoosePixelFormatARB) fprintf(stderr, "wglChoosePixelFormatARB is present.\n");


    // destroy & recreate window
//    DisableOpenGL( hWnd, hDC, hRC );
//    block_quit == TRUE;
    DisableOpenGL();
	DestroyGLWindow();
	CreateGLWindow();
//	block_quit == FALSE;

    // destroy context
//    wglMakeCurrent(*hDC, 0);
//    wglDeleteContext(temp_render_context);
//    ReleaseDC( hWnd, *hDC );
    // and make it again
    hDC =  GetDC(hWnd);
//    fprintf(stderr, "hDC=%08X\n", hDC);

	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.cStencilBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;

//    _PFD_();
    int pixelformat;
    unsigned int count;
    wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelformat, &count);
    if(count == 0) fprintf(stderr, "wglChoosePixelFormatARB() failed.\n");
//    fprintf(stderr, "Pixel format = %d (count=%d)\n", pixelformat, count);

    SetPixelFormat(hDC, pixelformat, &pfd);
//    _PFD_();

    temp_render_context = wglCreateContext(hDC);
    if(!temp_render_context) fprintf(stderr, "wglCreateContext() failed");

    if(!wglMakeCurrent(hDC, temp_render_context)) fprintf(stderr, "wglMakeCurrent() failed.\n");
    //glEnable(GL_MULTISAMPLE_ARB);

//        SetPixelFormat(hdc,pixelformat,&pfd);

#endif


//	extensions = (char*)glGetString(GL_EXTENSIONS);

	fprintf(stderr, " Creating OpenGL 3.2 Render context...\n");

	hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
	if(!hRC){
		fprintf(stderr, " wglCreateContextAttribsARB fail.\n");
		wglDeleteContext(temp_render_context);
		ExitProcess(1);
	}

	if(!wglMakeCurrent(hDC, hRC)){
		fprintf(stderr, " wglMakeCurrent fail.\n");
		wglDeleteContext(temp_render_context);
		ExitProcess(1);
	}

	/* delete temp render context */
	wglDeleteContext(temp_render_context);

	fprintf(stderr, " Done.\n");

/*
	fprintf(stderr, "OpenGL 3.0 render context information:\n");
	fprintf(stderr, " Renderer     : %s\n", (const char*)glGetString(GL_RENDERER));
	fprintf(stderr, " Vendor       : %s\n", (const char*)glGetString(GL_VENDOR));
	fprintf(stderr, " Version      : %s\n", (const char*)glGetString(GL_VERSION));
	fprintf(stderr, " GLSL version : %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	fprintf(stderr, "\n");
*/
	// create and enable the render context (RC)
//	*hRC = wglCreateContext( *hDC );
//	wglMakeCurrent( *hDC, *hRC );
//	fprintf(stderr, " GL<3 Extensions: %s\n", extensions);

    PFNGLGETSTRINGIPROC glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
//    PFNGLGETINTEGERVPROC glGetIntegerv = (PFNGLGETINTEGERVPROC)wglGetProcAddress("glGetIntegerv");
    if(glGetStringi){
        fprintf(stderr, " GL3 Extensions:\n");
        GLint n, i;
        glGetIntegerv(GL_NUM_EXTENSIONS, &n);
        for (i = 0; i < n; i++) fprintf(stderr, " %s\n", glGetStringi(GL_EXTENSIONS, i));
    }

	fprintf(stderr, " Configuring extensions...\n");

    if(!initialize_opengl_extensions()){
        fprintf(stderr,"  Unable to create extensions\n");
//        DisableOpenGL(hWnd, *hDC, *hRC);
        DisableOpenGL();
		ExitProcess(1);
    }

    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if(wglSwapIntervalEXT) wglSwapIntervalEXT(swapinterval);
    else fprintf(stderr, " wglSwapIntervalEXT is not supported.");

//    glEnable(GL_ARB_MULTISAMPLE);

    glViewport(0, 0, 1024, 768);
#ifdef MULTISAMPLE
    glEnable(GL_MULTISAMPLE_ARB);
    glEnable(GL_MULTISAMPLE);
#endif
}

// Disable OpenGL

//void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
void DisableOpenGL()
{
	fprintf(stderr, "Disabling OpenGL...\n");
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );
}

//void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);


//void CreateGLWindow(HINSTANCE hThisInstance, int nCmdShow)
void CreateGLWindow()
{
    fprintf(stderr, "Create GL Window.\n");

    HINSTANCE hInstance = GetModuleHandle(NULL);

    if(!hWnd){
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    /* The Window structure */
//    wincl.hInstance = hThisInstance;
    wincl.hInstance = hInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_OWNDC; //CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
//    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
	wincl.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl)){
        fprintf(stderr, "Error registering class.\n");
        return;
    }
    }

    /* The class is registered, let's create the program*/
    hWnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Let's start.",       /* Title Text */
           WS_CAPTION | WS_VISIBLE | WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1025,                 /* The programs width */
           768,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
//           hThisInstance,       /* Program Instance handler */
            hInstance,
           NULL                 /* No Window Creation data */
           );

}

void DestroyGLWindow()
{
	DestroyWindow( hWnd );
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    MSG messages;            /* Here messages to the application are saved */
	BOOL quit = FALSE;

	freopen("logerror.txt", "w", stderr);

//	CreateGLWindow(hThisInstance, nCmdShow);
	CreateGLWindow();
	// enable OpenGL for the window
//	EnableOpenGL( hWnd, &hDC, &hRC );
	EnableOpenGL();

    /* Make the window visible on the screen */
    //ShowWindow (hWnd, nCmdShow);
    ShowWindow (hWnd, SW_SHOW);

	DoPrepare();


    int t1, t2, frames=0;


	while ( !quit )
	{
		// check for messages
		if ( PeekMessage( &messages, NULL, 0, 0, PM_REMOVE )  )
		{

            /*switch(){
                case WM_CLOSE:
                    if(!block_quit) quit = TRUE;
                    break;
            }*/
			// handle or dispatch messages
			if ( messages.message == WM_QUIT )
			{
			    /*if(!block_quit)*/ quit = TRUE;
			}
			else
			{
				TranslateMessage( &messages );
				DispatchMessage( &messages );
			}

//			gDemoApplication->displayCallback();
		// OpenGL animation code goes here

		};

        t1 = my_time();
        //t2 = t1 - t2;
        float newfps = (float)1000000.0/(t1-t2);
        t2 = t1;
        fps = (fps*31 + newfps)/32;

        DoDraw(hDC);
        //DoDraw(wglGetCurrentDC());

        frames++;
	}

//    t2 = my_time();
//    fprintf(stderr, "Frames: %d   Time elapsed: %d    Frames/sec: %15.3f\n", frames, t2-t1, (float)frames*1000000/(t2-t1));

    DoDone();
	// shutdown OpenGL
//	DisableOpenGL( hWnd, hDC, hRC );

	// destroy the window explicitly
//    DisableOpenGL( hWnd, hDC, hRC );
    DisableOpenGL();
	DestroyGLWindow();

    fclose(stderr);
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
