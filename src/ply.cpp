#include<windows.h>
#include<stdlib.h>
#include<stdio.h>
#include<strings.h>

#define PLY_MAX_VERTEX  256*1024
#define PLY_MAX_FACE  256*1024

#include <gl3/gl3.h>

#include "ply.h"
#include "extensions.h"
#include "shader.h"
#include "matrix.h"

const char BDSM_header[4] = {'B', 'D', 'S', 'M'};
const char VERT_header[4] = {'V', 'E', 'R', 'T'};
const char FACE_header[4] = {'F', 'A', 'C', 'E'};

#define VF_NORMAL (1<<0)
#define VF_UV (1<<1)
#define VF_COLOR (1<<2)

void PlyLoader(const char *filename, tPlyModel *model)
{
    FILE *in;
    char str[1024];

    if((in=fopen(filename, "rb"))==NULL){
        MessageBox(NULL, "Error model loading", "Error", 0);
        return;
    }

    char header[10];
    fread(header, 4, 1, in);
    if(strncmp(header, BDSM_header, 4)){
        MessageBox(NULL, "Error model format, 'BDSM'-header is expected.", "Error", 0);
        fclose(in);
        return;
    }

    fprintf(stderr, "Model loader log.\nLoading [%s]...\n", filename);

    fread(header, 4, 1, in);
    if(strncmp(header, VERT_header, 4)){
        MessageBox(NULL, "Error model format, 'VERT'-header is expected.", "Error", 0);
        fclose(in);
        return;
    }

    int vertex;
    fread(&vertex, 4, 1, in);
    //vertex = atoi(&str[6]);
    fprintf(stderr, "Vertexes: %d\n", vertex);

    if((vertex<=0) || (vertex > PLY_MAX_VERTEX)){
        MessageBox(NULL, "Error model format. Vertexes count is wrong.", "Error", 0);
        fclose(in);
        return;
    }


    //fgets( str, sizeof(str)-2, in );
    int vertex_form;
    fread(&vertex_form, 4, 1, in);
    fprintf(stderr, "Vertex format [%04X]: VERTEX", vertex_form);
    if(vertex_form & VF_NORMAL) {fprintf(stderr, "+NORMAL"); model->attr.isnormal = 1;}
    if(vertex_form & VF_UV) {fprintf(stderr, "+UV"); model->attr.isuv = 1;}
    if(vertex_form & VF_COLOR) {fprintf(stderr, "+COLOR"); model->attr.iscolor = 1;}
    fprintf(stderr, "\n");

    model->vertex_size = vertex;

    int vdatasize = sizeof(tPlyVertex);
    if(model->attr.isnormal) vdatasize += sizeof(tPlyNormal);
    if(model->attr.isuv) vdatasize += sizeof(tPlyUV);
    if(model->attr.iscolor) vdatasize += sizeof(tPlyColor);

    fprintf(stderr, "Allocating: %d bytes\n", vdatasize * vertex);
    model->vertex = (tPlyVertex*)malloc(vdatasize * vertex);
    //model->normal = (tPlyVertex*)malloc(sizeof(tPlyVertex) * vertex);

//    fprintf(stderr, "Size of attr.data=%d\n", sizeof(tPlyAtr));
//    fprintf(stderr, "Size of tPlyModel=%d\n", sizeof(tPlyModel));
//    fprintf(stderr, "Size of tPlyVertexNormalColor=%d\n", sizeof(tPlyVertexNormalColor));

    fprintf(stderr, "Vertex data:\n", str);
    for(int i=0; i<vertex; i++){
        tPlyVertex vdata;
        tPlyVertex ndata;
        tPlyUV udata;
        tPlyColor cdata;

        fread(&vdata, sizeof(tPlyVertex), 1, in);
//        fprintf(stderr, "X:%8.4f\tY:%8.4f\tZ:%8.4f", vdata.x, vdata.y, vdata.z);

        switch(model->attr.data){
            case PA_VERTEX:
                model->vertex[i].x = vdata.x;
                model->vertex[i].y = vdata.y;
                model->vertex[i].z = vdata.z;

                break;

            case PA_VERTEXNORMAL:
                fread(&ndata, sizeof(tPlyVertex), 1, in);
                model->vertexnormal[i].x = vdata.x;
                model->vertexnormal[i].y = vdata.y;
                model->vertexnormal[i].z = vdata.z;
                model->vertexnormal[i].nx = ndata.x;
                model->vertexnormal[i].ny = ndata.y;
                model->vertexnormal[i].nz = ndata.z;
//                fprintf(stderr, "\tNX:%8.4f\tNY:%8.4f\tNZ:%8.4f",
//                    ndata.x, ndata.y, ndata.z);
                break;

            case PA_VERTEXNORMALUV:
                fread(&ndata, sizeof(tPlyVertex), 1, in);
                fread(&udata, sizeof(tPlyUV), 1, in);
                model->vertexnormaluv[i].x = vdata.x;
                model->vertexnormaluv[i].y = vdata.y;
                model->vertexnormaluv[i].z = vdata.z;
                model->vertexnormaluv[i].nx = ndata.x;
                model->vertexnormaluv[i].ny = ndata.y;
                model->vertexnormaluv[i].nz = ndata.z;
                model->vertexnormaluv[i].u = udata.u;
                model->vertexnormaluv[i].v = udata.v;
//                fprintf(stderr, "\tNX:%8.4f\tNY:%8.4f\tNZ:%8.4f\tU:%8.4f\tV:%8.4f",
//                    ndata.x, ndata.y, ndata.z, udata.u, udata.v);
                break;

            case PA_VERTEXNORMALUVCOLOR:
                fread(&ndata, sizeof(tPlyVertex), 1, in);
                fread(&udata, sizeof(tPlyUV), 1, in);
                fread(&cdata, 3, 1, in);
                model->vertexnormaluvcolor[i].x = vdata.x;
                model->vertexnormaluvcolor[i].y = vdata.y;
                model->vertexnormaluvcolor[i].z = vdata.z;
                model->vertexnormaluvcolor[i].nx = ndata.x;
                model->vertexnormaluvcolor[i].ny = ndata.y;
                model->vertexnormaluvcolor[i].nz = ndata.z;
                model->vertexnormaluvcolor[i].u = udata.u;
                model->vertexnormaluvcolor[i].v = udata.v;
                model->vertexnormaluvcolor[i].r = cdata.r;
                model->vertexnormaluvcolor[i].g = cdata.g;
                model->vertexnormaluvcolor[i].b = cdata.b;
                model->vertexnormaluvcolor[i].a = 255;
//                fprintf(stderr, "\tNX:%8.4f\tNY:%8.4f\tNZ:%8.4f\tU:%8.4f\tV:%8.4f\tR:%3d\tG:%3d\tB:%3d",
//                    ndata.x, ndata.y, ndata.z, udata.u, udata.v, cdata.r, cdata.g, cdata.b);
                break;

            case PA_VERTEXNORMALCOLOR:
                fread(&ndata, sizeof(tPlyVertex), 1, in);
                fread(&cdata, 3, 1, in);
                model->vertexnormalcolor[i].x = vdata.x;
                model->vertexnormalcolor[i].y = vdata.y;
                model->vertexnormalcolor[i].z = vdata.z;
                model->vertexnormalcolor[i].nx = ndata.x;
                model->vertexnormalcolor[i].ny = ndata.y;
                model->vertexnormalcolor[i].nz = ndata.z;
                model->vertexnormalcolor[i].r = cdata.r;
                model->vertexnormalcolor[i].g = cdata.g;
                model->vertexnormalcolor[i].b = cdata.b;
                model->vertexnormalcolor[i].a = 255;
//                fprintf(stderr, "\tNX:%8.4f\tNY:%8.4f\tNZ:%8.4f\tR:%3d\tG:%3d\tB:%3d",
//                    ndata.x, ndata.y, ndata.z, cdata.r, cdata.g, cdata.b);
                break;
        }

        //sscanf( str, "%f %f %f %f %f %f", &x, &y, &z, &nx, &ny, &nz);
//        fprintf(stderr, "\n");
//        model->normal[i].x = nx;
//        model->normal[i].y = ny;
//        model->normal[i].z = nz;
    }


    fread(header, 4, 1, in);
    if(strncmp(header, FACE_header, 4)){
        MessageBox(NULL, "Error model format, 'FACE'-header is expected.", "Error", 0);
        fclose(in);
        return;
    }

    int face;
    fread(&face, 4, 1, in);
    //vertex = atoi(&str[6]);
    fprintf(stderr, "Faces: %d\n", face);

    if((face<=0) || (face > PLY_MAX_FACE)){
        MessageBox(NULL, "Error model format. face count is wrong.", "Error", 0);
        fclose(in);
        return;
    }

    // For beginning allocate for QUADS (who have two TRIANGLES)
    fprintf(stderr, "Allocating: %d bytes\n", sizeof(tPlyIndex) * face * 3 * 2);
    //tPlyIndex*tt = (tPlyIndex*)malloc(sizeof(tPlyIndex) * face * 3 * 2);
    //return;
    model->index = (tPlyIndex*)malloc(sizeof(tPlyIndex) * face * 3 * 2);

        fprintf(stderr, "---\n");
        fflush(stderr);

    int max_index = 0;
    for(int i=0; i<face; i++){
        int n, vi[4];

        fread(&n, 4, 1, in);

        if(n==3){
            fread(vi, 4*3, 1, in);

//            fprintf(stderr, "TRIANGLE: %d %d %d\n", vi[0], vi[1], vi[2]);
            model->index[max_index++] = vi[0];
            model->index[max_index++] = vi[1];
            model->index[max_index++] = vi[2];
        } else if(n==4){
            fread(&vi, 4*4, 1, in);

//            fprintf(stderr, "QUAD: %d %d %d %d -> two TRIANGLES\n", vi[0], vi[1], vi[2], vi[3]);
            model->index[max_index++] = vi[0];
            model->index[max_index++] = vi[1];
            model->index[max_index++] = vi[2];

            model->index[max_index++] = vi[0];
            model->index[max_index++] = vi[2];
            model->index[max_index++] = vi[3];

        } else {
            fprintf(stderr, "n=%d [ignoring]\n", n);
        }

        //model->vertex->x = x;
        //model->vertex->y = y;
        //model->vertex->z = z;
    }

    model->index_size = max_index;

    if(max_index != (face * 3 * 2)){
        model->index = (tPlyIndex*)realloc(model->index, sizeof(tPlyIndex)*max_index);
        fprintf(stderr, "Real indexes: %d (reallocating memory)\n", max_index);
    }

    fclose(in);

    /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &model->vao);
    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(model->vao);
    /* Allocate and assign three Vertex Buffer Objects to our handle */
    glGenBuffers(2, model->vbo);
    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates, normals, uv, colours) */
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo[0]);

    fprintf(stderr, "PlyBuffers: %d indexes  [va:%d v:%d i:%d]\n", model->index_size, model->vao, model->vbo[0], model->vbo[1]);

    /* Copy the vertex data from tetrahedron to our buffer */
//    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tetrahedron, GL_STATIC_DRAW);
    switch(model->attr.data){
        case PA_VERTEX:
            fprintf(stderr, "PA_VERTEX\n");
            glBufferData(GL_ARRAY_BUFFER, model->vertex_size * sizeof(tPlyVertex), model->vertex, GL_STATIC_DRAW);

            glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(tPlyVertex), 0);
            glEnableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(3);

            break;
        case PA_VERTEXNORMAL:
            fprintf(stderr, "PA_VERTEXNORMAL\n");
            glBufferData(GL_ARRAY_BUFFER, model->vertex_size * sizeof(tPlyVertexNormal), model->vertexnormal, GL_STATIC_DRAW);

            glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(tPlyVertexNormal), 0);   // vertex (in_Position)
            glEnableVertexAttribArray(0);
            glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(tPlyVertexNormal), (GLvoid*)sizeof(tPlyVertex)); // normal (in_Normal)
            glEnableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(3);

            break;
        case PA_VERTEXNORMALUV:
            fprintf(stderr, "PA_VERTEXNORMALUV\n");
            glBufferData(GL_ARRAY_BUFFER, model->vertex_size * sizeof(tPlyVertexNormalUV), model->vertexnormaluv, GL_STATIC_DRAW);

            glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(tPlyVertexNormalUV), (GLvoid*)0);    // vertex (in_Position)
            glEnableVertexAttribArray(0);
            glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(tPlyVertexNormalUV), (GLvoid*)sizeof(tPlyVertex));   // normal (in_Normal)
            glEnableVertexAttribArray(1);
            glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, sizeof(tPlyVertexNormalUV), (GLvoid*)(sizeof(tPlyVertexNormal))); // uv (in_UV)
            glEnableVertexAttribArray(2);
            glDisableVertexAttribArray(3);

            break;
        case PA_VERTEXNORMALUVCOLOR:
            fprintf(stderr, "PA_VERTEXNORMALUVCOLOR\n");
            glBufferData(GL_ARRAY_BUFFER, model->vertex_size * sizeof(tPlyVertexNormalUVColor), model->vertexnormaluvcolor, GL_STATIC_DRAW);

            glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(tPlyVertexNormalUVColor), (GLvoid*)0);   // vertex (in_Position)
            glEnableVertexAttribArray(0);
            glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(tPlyVertexNormalUVColor), (GLvoid*)sizeof(tPlyVertex));  // normal (in_Normal)
            glEnableVertexAttribArray(1);
            glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, sizeof(tPlyVertexNormalUVColor), (GLvoid*)(sizeof(tPlyVertexNormal)));  // uv (in_UV)
            glEnableVertexAttribArray(2);
            glVertexAttribPointer((GLuint)3, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(tPlyVertexNormalUVColor), (GLvoid*)(sizeof(tPlyVertexNormalUV)));    // color (in_Color)
            glEnableVertexAttribArray(3);

            break;
        case PA_VERTEXNORMALCOLOR:
            fprintf(stderr, "PA_VERTEXNORMALCOLOR (TBD)\n");
            glBufferData(GL_ARRAY_BUFFER, model->vertex_size * sizeof(tPlyVertexNormalColor), model->vertexnormalcolor, GL_STATIC_DRAW);
            glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(tPlyVertexNormalColor), (GLvoid*)0);   // vertex (in_Positon)
            glEnableVertexAttribArray(0);
            glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(tPlyVertexNormalColor), (GLvoid*)sizeof(tPlyVertex));    // normal (in_Normal)
            glEnableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glVertexAttribPointer((GLuint)3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(tPlyVertexNormalColor), (GLvoid*)(sizeof(tPlyVertexNormal)));    // color (in_Color)
            glEnableVertexAttribArray(3);
            break;
    }
    /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
//    glBindBuffer(GL_ARRAY_BUFFER, model->vbo[1]);
    /* Bind our third VBO as being the active buffer and storing vertex array indicies */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->vbo[1]);
    /* Copy the index data to our buffer */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->index_size * sizeof(tPlyIndex), model->index, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PlyDone(tPlyModel *model)
{
    free(model->index);
//    free(model->normal);
    free(model->vertex);

    glDeleteBuffers(2, model->vbo);
    glDeleteVertexArrays(1, &model->vao);
}

extern GLfloat cameramatrix[16];
extern GLfloat projectionmatrix[16]; /* Our projection matrix starts with all 0s */
extern GLfloat modelmatrix[16]; /* Our model matrix  */
extern GLfloat light_pos[4];
extern GLfloat cur_color[3];

void PlyDraw(tPlyModel *model)
{
    GLfloat commmatrix[16];

    int shader_program = SHADER_VERTEX_NORMAL;
    switch(model->attr.data){
        case PA_VERTEX:
            //shader_current = &shader_vertexnormal;
            shader_program = SHADER_VERTEX_NORMAL;
            break;
        case PA_VERTEXNORMAL:
            //shader_current = &shader_vertexnormal;
            shader_program = SHADER_VERTEX_NORMAL;
            break;
        case PA_VERTEXNORMALUV:
            //shader_current = &shader_vertexnormaluv;
            shader_program = SHADER_VERTEX_NORMAL_UV;
            break;
        case PA_VERTEXNORMALUVCOLOR:
            //shader_current = &shader_vertexnormaluvcolor;
            shader_program = SHADER_VERTEX_NORMAL_UV_COLOR;
            break;
        case PA_VERTEXNORMALCOLOR:
            //shader_current = &shader_vertexnormalcolor;
            shader_program = SHADER_VERTEX_NORMAL_COLOR;
            break;
    }
//    fprintf(stderr, "PlyDraw: %d indexes  [v:%d i:%d]\n", model->index_size, model->vbo[0], model->vbo[1]);
//    glUseProgram(shader_current->shaderprogram);
//    glUniformMatrix4fv(shader_current->projection_u, 1, GL_FALSE, projectionmatrix);
//	glUniform4fv(shader_current->light_pos_u, 1, light_pos);
//    glUniform3fv(shader_current->colour_u, 1, cur_color);

    multiply4x4n(commmatrix, modelmatrix, cameramatrix);
//    glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, commmatrix);
    ShaderDo(shader_program, projectionmatrix, commmatrix, cur_color);

    glBindVertexArray(model->vao);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->vbo[1]);
    glDrawElements(GL_TRIANGLES, model->index_size, GL_UNSIGNED_SHORT, (GLvoid*)0);
}
