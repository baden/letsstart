#ifndef _INC_PLY_
#define _INC_PLY_

#include"gl/gl.h"

typedef struct{
    GLfloat x;
    GLfloat y;
    GLfloat z;
} tPlyVertex;

typedef struct{
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
} tPlyNormal;

typedef struct{
    GLfloat u;
    GLfloat v;
} tPlyUV;

typedef struct{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} tPlyColor;

typedef struct{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
} tPlyVertexNormal;

typedef struct{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    GLfloat u;
    GLfloat v;
} tPlyVertexNormalUV;

typedef struct{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} tPlyVertexNormalColor;

typedef struct{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat nx;
    GLfloat ny;
    GLfloat nz;
    GLfloat u;
    GLfloat v;
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} tPlyVertexNormalUVColor;

typedef GLushort tPlyIndex;

typedef union{
    unsigned data;
    struct{
        unsigned isnormal:1;
        unsigned isuv:1;
        unsigned iscolor:1;
    };
} tPlyAtr;

#define PA_VERTEX       0
#define PA_VERTEXNORMAL (0 | (1<<0))
#define PA_VERTEXNORMALUV (0 | (1<<0) | (1<<1))
#define PA_VERTEXNORMALUVCOLOR (0 | (1<<0) | (1<<1) | (1<<2))
#define PA_VERTEXNORMALCOLOR (0 | (1<<0) | (1<<2))

typedef struct{
    int vertex_size;
    int index_size;
    tPlyAtr attr;
    GLuint vao;
    GLuint vbo[2];

    union{
        tPlyVertex *vertex;     // vertices & normals
        tPlyVertexNormal *vertexnormal;     // vertices & normals
        tPlyVertexNormalUV *vertexnormaluv;     // vertices & normals & uv
        tPlyVertexNormalUVColor *vertexnormaluvcolor;     // vertices & normals & uv & color
        tPlyVertexNormalColor *vertexnormalcolor;     // vertices & normals & color
    };

    //tPlyVertex *normal;
    tPlyIndex *index;
} tPlyModel;

void PlyLoader(const char *filename, tPlyModel *model);
void PlyDone(tPlyModel *model);
void PlyDraw(tPlyModel *model);

#endif
