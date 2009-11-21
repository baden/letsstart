#ifndef OBJECT_H_
#define OBJECT_H_

#include"gl/gl.h"
/*
typedef enum {
    TRIANGLE_VERTEX_NORMAL = 0,
    TRIANGLE_VERTEX_NORNAL_UV,
    TRIANGLE_VERTEX_NORMAL_UV_COLOR,
    TRIANGLE_VERTEX_NORMAL_COLOR,
    QUAD_VERTEX_NORMAL,
    QUAD_VERTEX_NORNAL_UV,
    QUAD_VERTEX_NORMAL_UV_COLOR,
    QUAD_VERTEX_NORMAL_COLOR,
} tObjType;

typedef struct{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} tObjectColor;

// TRIANGLE_VERTEX_NORMAL
typedef struct{
    tObjectColor color;
} tObjectVertexNotmal;

typedef struct{
    GLuint vao;
    GLuint vbo[2];
    union{
        tObjectVertexNotmal vertex_normal;
    };
} tObjectItem;

typedef struct{
    int frames;     // количество кадров анимации
    tObjectItem objects[1]; // Массив объектов-кадров
} tObject;
*/

int ObjectLoad(const char *object_file_name);
int ObjectUnloadAll();
int ObjectStatistic();
int ObjectDraw(unsigned int id, int frame);
int ObjectPrepareDraw();
int ObjectDrawArrays();

typedef struct{
    int attribs;
    int vertex_count;
    int index_count;
    GLuint vao;
    GLuint vbo[2];
} tObjectMesh;

typedef struct{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} tObjectColor;

typedef struct{
    tObjectMesh* mesh;
    tObjectColor color;
    GLfloat modelmatrix[16];
} tObjectFrameItem;

typedef struct{
    int meshes_in_frame;
    tObjectFrameItem* framedata;
} tObjectFrame;

typedef struct{
    int meshes_count;       // количество мешей в модели
    int frames_count;       // количество кадров анимации
    tObjectMesh* meshes;    // массив мешей
    tObjectFrame* frames;   // массив анимаций
//    int frames;     // количество кадров анимации
//    tObjectItem objects[1]; // Массив объектов-кадров
} tObject;

#endif

