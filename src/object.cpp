#include <vector>
#include <zlib.h>
#include <gl3/gl3.h>
#include "extensions.h"
#include "object.h"

#define MAX_MESHES 5000
#define MAX_VERTEXES (1*1024*1024)
#define MAX_INEXES (1*1024*1024)
#define MAX_FRAMES 250

#define MESH_QUAD (1<<0)
#define MESH_TEXTURE (1<<1)
#define MESH_COLOR (1<<2)
#define MESH_VS (1<<3)
#define MESH_VI (1<<4)
#define MESH_IS (1<<5)
#define MESH_II (1<<6)

#include "matrix.h"
#include "shader.h"
#include "math.h"

#define USE_MESHSORT        // Применить сортировку мешей по типу.

#define USE_ADJ     // Применить преобразование моделей для TRIANGLES_ADJACENCY

std::vector <tObject> objectList;

#ifdef USE_MESHSORT
typedef struct{
    tObjectMesh *mesh;
    GLfloat modelmatrix[16];
    GLfloat color[3];
} tDrawMesh;

std::vector <tDrawMesh> drawVertexNormalMeshes;
std::vector <tDrawMesh> drawVertexNormalColorMeshes;
#endif

int ObjectInit()
{
    return 1;
}

const char BDM2_header[4] = {'B', 'D', 'M', '2'};
const char ANIM_header[4] = {'A', 'N', 'I', 'M'};
const char VBOD_header[4] = {'V', 'B', 'O', 'D'};
//const char VERT_header[4] = {'V', 'E', 'R', 'T'};
//const char FACE_header[4] = {'F', 'A', 'C', 'E'};

typedef struct{
    float x;
    float y;
    float z;
} vec3;

#define TRESHOLD 0.001f

//#define VO if(attribs & MESH_TEXTURE) ((unsigned char*)abuf)
//#define VI if(attribs & MESH_TEXTURE) ((unsigned char*)ibuf)

int FindAdj(tObjectMesh *mesh, void *abuf, void *vbuf, void *ibuf, int i, int i0, int i1, int i2)
{
    int attribs = mesh->attribs;

    fprintf(stderr, "[F%d:%d=", i0, i1);
//    int i2 = -1;
    for(int k=0; k<mesh->index_count; k+=3) if(k!=i){
        int v0;
        int v1;
        int v2;
        if(attribs & MESH_VI){
            v0 = ((unsigned int *)ibuf)[k+0];
            v1 = ((unsigned int *)ibuf)[k+1];
            v2 = ((unsigned int *)ibuf)[k+2];
        } else if(attribs & MESH_VS){
            v0 = ((unsigned short *)ibuf)[k+0];
            v1 = ((unsigned short *)ibuf)[k+1];
            v2 = ((unsigned short *)ibuf)[k+2];
        } else {
            v0 = ((unsigned char *)ibuf)[k+0];
            v1 = ((unsigned char *)ibuf)[k+1];
            v2 = ((unsigned char *)ibuf)[k+2];
        }

        if(v0 == i0){
            if(v1 == i1){
                i2 = v2;                break;
            } else if(v2 == i1){
                i2 = v1;                break;
            }
        }else if(v1 == i0){
            if(v0 == i1){
                i2 = v2;                break;
            } else if(v2 == i1){
                i2 = v0;                break;
            }
        }else if(v2 == i0){
            if(v0 == i1){
                i2 = v1;                break;
            } else if(v1 == i1){
                i2 = v0;                break;
            }
        }
    }
    fprintf(stderr, "%d]", i2);
//    if(i2==-1) i2 = _oi2;
    return i2;
}

void MeshAdj(tObjectMesh *mesh, void *abuf, void *vbuf, void *ibuf, int idatasize)
{
    fprintf(stderr, "       Triangles to triangles_adjacency translation.\n");
    fflush(stderr);

/*
        Необходимо для треугольников
               1
              ^\
              | \
              |  \
            e0|   \ e1
              |    \
              |     \
              |      v
              0<------2
                  e2

        Получить следующее:

       1 - - - 2- - ->3
              ^\
        \     | \     |
              |  \
          \ e0|   \ e1|
              |    \
            \ |     \ |
              |      v
              0<------4
                  e2
                \     |

                  \   |

                    \ |

                      5
*/
    int attribs = mesh->attribs;

//    unsigned char *bbuf = (unsigned char *)abuf;
//    unsigned short *sbuf = (unsigned short *)abuf;
//    unsigned int *ibuf = (unsigned int *)abuf;

    int strip = 6; // x,y,z,nx,ny,nz
    if(attribs & MESH_TEXTURE) strip+=2;  // +u,v
    if(attribs & MESH_COLOR) strip+=2;    // +rgba == 2halffloat

    fprintf(stderr, "         {");
    if(attribs & MESH_VI) fprintf(stderr, "I");
    else if(attribs & MESH_VS) fprintf(stderr, "S");
    else fprintf(stderr, "B");
    fprintf(stderr, ":%d}\n", idatasize*2);

    fflush(stderr);

    for(int i=0, j=0; i<mesh->index_count; i+=3, j+=6){
        int i0;
        int i1;
        int i2;
        int o0, o1, o2, o3, o4, o5;
        if(attribs & MESH_VI){
            i0 = ((unsigned int *)ibuf)[i+0];
            i1 = ((unsigned int *)ibuf)[i+1];
            i2 = ((unsigned int *)ibuf)[i+2];
        } else if(attribs & MESH_VS){
            i0 = ((unsigned short *)ibuf)[i+0];
            i1 = ((unsigned short *)ibuf)[i+1];
            i2 = ((unsigned short *)ibuf)[i+2];
        } else {
            i0 = ((unsigned char *)ibuf)[i+0];
            i1 = ((unsigned char *)ibuf)[i+1];
            i2 = ((unsigned char *)ibuf)[i+2];
        }
        fprintf(stderr, "      tri: (%d, %d, %d) -> ", i0, i1, i2);

        // Оригинальный треугольник
        o0 = i0;
        o2 = i1;
        o4 = i2;

        // Соседние треугольники
        o1 = FindAdj(mesh, abuf, vbuf, ibuf, i, i0, i1, i2);
//        if(iA == -1) iA = i2;
        o3 = FindAdj(mesh, abuf, vbuf, ibuf, i, i1, i2, i0);
        o5 = FindAdj(mesh, abuf, vbuf, ibuf, i, i2, i0, i1);
        fprintf(stderr, "(%d, %d, %d, %d, %d, %d)\n", o0, o1, o2, o3, o4, o5);

        if(attribs & MESH_VI){
            ((unsigned int *)abuf)[j+0] = o0;
            ((unsigned int *)abuf)[j+1] = o1;
            ((unsigned int *)abuf)[j+2] = o2;
            ((unsigned int *)abuf)[j+3] = o3;
            ((unsigned int *)abuf)[j+4] = o4;
            ((unsigned int *)abuf)[j+5] = o5;
        } else if(attribs & MESH_VS){
            ((unsigned short *)abuf)[j+0] = o0;
            ((unsigned short *)abuf)[j+1] = o1;
            ((unsigned short *)abuf)[j+2] = o2;
            ((unsigned short *)abuf)[j+3] = o3;
            ((unsigned short *)abuf)[j+4] = o4;
            ((unsigned short *)abuf)[j+5] = o5;
        } else{
            ((unsigned char *)abuf)[j+0] = o0;
            ((unsigned char *)abuf)[j+1] = o1;
            ((unsigned char *)abuf)[j+2] = o2;
            ((unsigned char *)abuf)[j+3] = o3;
            ((unsigned char *)abuf)[j+4] = o4;
            ((unsigned char *)abuf)[j+5] = o5;
        }
    }
/*

            // Ищем треугольник для грани 0-1
            fprintf(stderr, "      tri: (%d, %d, %d) ", bbuf[j+0], bbuf[j+2], bbuf[j+4]);

            vec3 v0 = {HALFToFloat(((HALF*)vbuf)[bbuf[j+0]*strip+0]), HALFToFloat(((HALF*)vbuf)[bbuf[j+0]*strip+1]), HALFToFloat(((HALF*)vbuf)[bbuf[j+0]*strip+2])};
            fprintf(stderr, " (%.1f,%.1f,%.1f) ", v0.x, v0.y, v0.z);
            vec3 v1 = {HALFToFloat(((HALF*)vbuf)[bbuf[j+2]*strip+0]), HALFToFloat(((HALF*)vbuf)[bbuf[j+2]*strip+1]), HALFToFloat(((HALF*)vbuf)[bbuf[j+2]*strip+2])};
            fprintf(stderr, " (%.1f,%.1f,%.1f) ", v1.x, v1.y, v1.z);
            vec3 v2 = {HALFToFloat(((HALF*)vbuf)[bbuf[j+4]*strip+0]), HALFToFloat(((HALF*)vbuf)[bbuf[j+4]*strip+1]), HALFToFloat(((HALF*)vbuf)[bbuf[j+4]*strip+2])};
            fprintf(stderr, " (%.1f,%.1f,%.1f) ", v2.x, v2.y, v2.z);

            int fi = -1;
            int i1 = ((unsigned char *)buf)[i+0];
            int i2 = ((unsigned char *)buf)[i+1];
            for(int k=0; k<idatasize; k+=3) if(k!=i){
                if(((unsigned char *)buf)[k+0] == i1){          // v0 = i1
                    if(((unsigned char *)buf)[k+1] == i2){          // v1 = i2
                        fi = ((unsigned char *)buf)[k+2];               // i3 = v2
                        fprintf(stderr, "[%d]", fi);
                        break;
                    } else if(((unsigned char *)buf)[k+2] == i2){   // v2 = i2
                        fi = ((unsigned char *)buf)[k+1];               // i3 = v1
                        fprintf(stderr, "[%d]", fi);
                        break;
                    }
                }else if(((unsigned char *)buf)[k+1] == i1){    // v1 = i1
                    if(((unsigned char *)buf)[k+0] == i2){          // v0 = i2
                        fi = ((unsigned char *)buf)[k+2];               // i3 = v2
                        fprintf(stderr, "[%d]", fi);
                        break;
                    } else if(((unsigned char *)buf)[k+2] == i2){   // v2 = i2
                        fi = ((unsigned char *)buf)[k+0];               // i3 = v0
                        fprintf(stderr, "[%d]", fi);
                        break;
                    }
                }else if(((unsigned char *)buf)[k+2] == i1){    // v2 = i1
                    if(((unsigned char *)buf)[k+0] == i2){          // v0 = i2
                        fi = ((unsigned char *)buf)[k+1];               // i3 = v1
                        fprintf(stderr, "[%d]", fi);
                        break;
                    } else if(((unsigned char *)buf)[k+1] == i2){   // v1 = i2
                        fi = ((unsigned char *)buf)[k+0];               // i3 = v0
                        fprintf(stderr, "[%d]", fi);
                        break;
                    }
                }
            }
            if(fi == -1){   // Индекс не найден
            }
            //fprintf(stderr, "\n");
            bbuf[j+1] = ((unsigned char *)buf)[i+2];    // Тот-же треугольник, но задом наперед
            bbuf[j+3] = ((unsigned char *)buf)[i+0];    // Тот-же треугольник, но задом наперед
            bbuf[j+5] = ((unsigned char *)buf)[i+1];    // Тот-же треугольник, но задом наперед
            fprintf(stderr, "\n");
        }
    }
*/
    mesh->index_count *= 2;
}

int ObjectLoad(const char *filename)
{
	gzFile in;

    fprintf(stderr, "Object Loading: %s\n", filename);

	if((in = gzopen(filename, "rb")) == NULL) {
		fprintf(stderr, "  Can't gzopen file\n");
        return -1;
	}
    //fprintf(stderr, "  decompressing...\n");

    char header[5];
    gzread(in, header, 4 );
    if(strncmp(header, BDM2_header, 4)){
        fprintf(stderr, "   Error model format, 'BDM2'-header is expected.\n");
        gzclose(in);
        return -1;
    }

    int meshes_count;
    gzread(in, &meshes_count, 4 );
    fprintf(stderr, "  Meshes: %d\n", meshes_count);

    if((meshes_count<=0) || (meshes_count >= MAX_MESHES)){
        fprintf(stderr, "   Error model. Meshes count is out of range.\n");
        gzclose(in);
        return -1;
    }

    tObjectMesh *meshes;
    fprintf(stderr, "  Allocating memory for meshes (%d bytes)...", sizeof(tObjectMesh) * meshes_count);
    if((meshes = (tObjectMesh *)malloc(sizeof(tObjectMesh) * meshes_count))==NULL){
        fprintf(stderr, "fail.\n");
        gzclose(in);
        return -1;
    } else fprintf(stderr, "ok.\n");

    for(int m = 0; m<meshes_count; m++){
        unsigned char attribute;
        int vertexes;
        int indexes;
        tObjectMesh *mesh = &meshes[m];

        fprintf(stderr, "   [%d]:", m);
        gzread(in, &attribute, 1 );
        mesh->attribs = attribute;
        if(attribute & MESH_QUAD)
            fprintf(stderr, " [quads]");
        else
            fprintf(stderr, " [triangles]");
        if(attribute & MESH_TEXTURE) fprintf(stderr, " [texture coordinates]");
        if(attribute & MESH_COLOR) fprintf(stderr, " [vertex colors]");

        if(attribute & MESH_VI){
            fprintf(stderr, " [VI]");
            gzread(in, &vertexes, 4 );
        } else if(attribute & MESH_VS){
            fprintf(stderr, " [VS]");
            unsigned short t_vertexes;
            gzread(in, &t_vertexes, 2 );
            vertexes = t_vertexes;
        } else {
            fprintf(stderr, " [VB]");
            unsigned char t_vertexes;
            gzread(in, &t_vertexes, 1 );
            vertexes = t_vertexes;
        }

        if(attribute & MESH_II){
            fprintf(stderr, " [II]");
            gzread(in, &indexes, 4 );
        } else if(attribute & MESH_IS){
            fprintf(stderr, " [IS]");
            unsigned short t_indexes;
            gzread(in, &t_indexes, 2 );
            indexes = t_indexes;
        } else {
            fprintf(stderr, " [IB]");
            unsigned char t_indexes;
            gzread(in, &t_indexes, 1 );
            indexes = t_indexes;
        }
        fprintf(stderr, "\n");
        fprintf(stderr, "    Vertexes: %d\n", vertexes);
        fprintf(stderr, "    Indexes: %d\n", indexes);
        mesh->vertex_count = vertexes;
        mesh->index_count = indexes;
    }

    gzread(in, header, 4 );
    if(strncmp(header, ANIM_header, 4)){
        fprintf(stderr, "   Error model format, 'ANIM'-header is expected.\n");
        free(meshes);
        gzclose(in);
        return -1;
    }

    int frames_count;
    gzread(in, &frames_count, 4 );
    fprintf(stderr, "  Animation frames: %d\n", frames_count);

    if((frames_count<=0) || (frames_count >= MAX_FRAMES)){
        fprintf(stderr, "   Error model. Frames count is out of range.\n");
        free(meshes);
        gzclose(in);
        return -1;
    }

    tObjectFrame *frames;
    fprintf(stderr, "  Allocating memory for animation (%d bytes)...", sizeof(tObjectFrame) * frames_count);
    if((frames = (tObjectFrame *)malloc(sizeof(tObjectFrame) * frames_count))==NULL){
        fprintf(stderr, "fail.\n");
        free(meshes);
        gzclose(in);
        return -1;
    } else fprintf(stderr, "ok.\n");

    memset(frames, 0, sizeof(tObjectFrame) * frames_count); // заполним значениями NULL все указатели

    for(int f = 0; f<frames_count; f++){
        int meshes_in_frame;
        tObjectFrame *frame = &frames[f];

        fprintf(stderr, "   [%d]:\n", f);

        if(meshes_count<256){
            unsigned char t_meshes_in_frame;
            gzread(in, &t_meshes_in_frame, 1 );
            meshes_in_frame = t_meshes_in_frame;
        } else if(meshes_count<65536){
            unsigned short t_meshes_in_frame;
            gzread(in, &t_meshes_in_frame, 2 );
            meshes_in_frame = t_meshes_in_frame;
        } else {
            gzread(in, &meshes_in_frame, 4 );
        }
        fprintf(stderr, "      Meshes in frame: %d\n", meshes_in_frame);
        if((meshes_in_frame<0)||(meshes_in_frame>meshes_count)){
            fprintf(stderr, "      Error meshes in frame count.\n");

            for(int ff=0; ff<frames_count; ff++)
                if(frames[ff].framedata) free(frames[ff].framedata);
            free(frames);
            free(meshes);
            gzclose(in);
            return -1;
        }

        frame->meshes_in_frame = meshes_in_frame;
        fprintf(stderr, "      Allocating memory for frame data (%d bytes).\n", sizeof(tObjectFrameItem)*meshes_in_frame);
        frame->framedata = (tObjectFrameItem*)malloc(sizeof(tObjectFrameItem)*meshes_in_frame);
//        frame->meshes = malloc()
        for(int m=0; m<meshes_in_frame; m++){
            int mesh_id;
            //tObjectColor mesh_color;
            tObjectFrameItem *framedata = &frame->framedata[m];

            if(meshes_count<256){
                unsigned char t_mesh_id;
                gzread(in, &t_mesh_id, 1 );
                mesh_id = t_mesh_id;
            } else if(meshes_count<65536){
                unsigned short t_mesh_id;
                gzread(in, &t_mesh_id, 2 );
                mesh_id = t_mesh_id;
            } else {
                gzread(in, &mesh_id, 4 );
            }
            gzread(in, &framedata->color, 4 );
            gzread(in, &framedata->modelmatrix, 16*sizeof(GLfloat) );

            fprintf(stderr, "      Mesh id [%d]", mesh_id);
            fprintf(stderr, "      Mesh color [%d, %d, %d, %d]\n", framedata->color.r, framedata->color.g, framedata->color.b, framedata->color.a );
            fprintf(stderr, "      Model matrix:[");
            for(int i=0; i<16; i++)
                fprintf(stderr, " %.1f", framedata->modelmatrix[i] );
            fprintf(stderr, " ]\n");
            framedata->mesh = &meshes[mesh_id];
        }
    }

    gzread(in, header, 4 );
    if(strncmp(header, VBOD_header, 4)){
        fprintf(stderr, "   Error model format, 'VBOD'-header is expected.\n");
        for(int ff=0; ff<frames_count; ff++) if(frames[ff].framedata) free(frames[ff].framedata);
        free(frames);
        free(meshes);
        gzclose(in);
        return -1;
    }

    fprintf(stderr, "    Loading VBO data.\n");
    for(int m = 0; m<meshes_count; m++){
        fprintf(stderr, "    [%d]:\n", m);
        int vdatasize = 3*2 + 3*2;  // 3 HALF_FLOAT + 3 HALF_FLOAT  - coordinate + normal
        if(meshes[m].attribs & MESH_TEXTURE) vdatasize += 2*2; // + 2 HALF_FLOAT - texture coordinate
        if(meshes[m].attribs & MESH_COLOR) vdatasize += 4; // + 4 BYTE - vertex color
        int stride = vdatasize;
        vdatasize *= meshes[m].vertex_count;
        fprintf(stderr, "       Vertex data size = %d bytes.\n", vdatasize);

        int idatasize = 0;
        if(meshes[m].attribs & MESH_VI) idatasize = 4;
        else if(meshes[m].attribs & MESH_VS) idatasize = 2;
        else idatasize = 1;
        idatasize *= meshes[m].index_count;

        glGenVertexArrays(1, &meshes[m].vao);   /* Allocate and assign a Vertex Array Object to our handle */
        glBindVertexArray(meshes[m].vao);   /* Bind our Vertex Array Object as the current used object */
        glGenBuffers(2, meshes[m].vbo); /* Allocate and assign three Vertex Buffer Objects to our handle */
        glBindBuffer(GL_ARRAY_BUFFER, meshes[m].vbo[0]);    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates, normals, uv, colours) */

        fprintf(stderr, "       Index data size = %d bytes.\n", idatasize);
        //fprintf(stderr, "       Allocating temp buffer for vbo data.");
        void *vbuf;
        if((vbuf = malloc(vdatasize))==NULL){
            break;
        }
        gzread(in, vbuf, vdatasize);
        glBufferData(GL_ARRAY_BUFFER, vdatasize, vbuf, GL_STATIC_DRAW);

        glVertexAttribPointer((GLuint)0, 3, GL_HALF_FLOAT, GL_TRUE, stride, (GLvoid*)0);    // vertex coordinates
        glEnableVertexAttribArray(0);
        glVertexAttribPointer((GLuint)1, 3, GL_HALF_FLOAT, GL_TRUE, stride, (GLvoid*)(3*2));  // normals
        glEnableVertexAttribArray(1);
        if(meshes[m].attribs & MESH_TEXTURE){
            glVertexAttribPointer((GLuint)2, 2, GL_HALF_FLOAT, GL_TRUE, stride, (GLvoid*)(3*2+3*2));  // texture coordinares
            glEnableVertexAttribArray(2);
            if(meshes[m].attribs & MESH_COLOR){
                glVertexAttribPointer((GLuint)3, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid*)(3*2+3*2+2*2));  // color
                glEnableVertexAttribArray(3);
            }
        } else {
                glVertexAttribPointer((GLuint)3, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid*)(3*2+3*2));  // color
                glEnableVertexAttribArray(3);
        }

#if 0
        char *blog = (char *)vbuf;
        for(int v=0; v<meshes[m].vertex_count; v++){
            fprintf(stderr, "       v[%d]=", v);
            //float f1, f2;
            HALF x = ((HALF*)blog)[0]; blog+=2;
            HALF y = ((HALF*)blog)[0]; blog+=2;
            HALF z = ((HALF*)blog)[0]; blog+=2;
            fprintf(stderr, " v:(%.3f, %.3f, %.3f)", HALFToFloat(x), HALFToFloat(y), HALFToFloat(z));
            x = ((HALF*)blog)[0]; blog+=2;
            y = ((HALF*)blog)[0]; blog+=2;
            z = ((HALF*)blog)[0]; blog+=2;
            fprintf(stderr, " n:(%.3f, %.3f, %.3f)", HALFToFloat(x), HALFToFloat(y), HALFToFloat(z));
            if(meshes[m].attribs & MESH_TEXTURE){
                x = ((HALF*)blog)[0]; blog+=2;
                y = ((HALF*)blog)[0]; blog+=2;
                fprintf(stderr, " uv:(%.3f, %.3f)", HALFToFloat(x), HALFToFloat(y));
            }
            if(meshes[m].attribs & MESH_COLOR){
                unsigned char cr = blog[0]; blog++;
                unsigned char cg = blog[0]; blog++;
                unsigned char cb = blog[0]; blog++;
                fprintf(stderr, " c:(%d, %d, %d)", cr, cg, cb);
            }
            fprintf(stderr, "\n");
        }
#endif
        void *ibuf;
        if((ibuf = malloc(idatasize))==NULL) break;

        gzread(in, ibuf, idatasize);
#ifdef USE_ADJ
        void *abuf;
        if((abuf = malloc(idatasize*2))==NULL) break;
        MeshAdj(&(meshes[m]), abuf, vbuf, ibuf, idatasize);
        void *tbuf = ibuf;
        ibuf = abuf;
        idatasize*=2;
        free(tbuf);
#endif

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[m].vbo[1]);   /* Bind our third VBO as being the active buffer and storing vertex array indicies */
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, idatasize, ibuf, GL_STATIC_DRAW); /* Copy the index data to our buffer */

#if 0
        blog = (char *)buf;
        fprintf(stderr, "       I:");
        for(int idx=0; idx<meshes[m].index_count; idx++){
            if(meshes[m].vertex_count < 256){
                fprintf(stderr, " %d", blog[0]); blog++;
            } else if(meshes[m].vertex_count < 65536){
                fprintf(stderr, " %d", ((unsigned short*)blog)[0]); blog+=2;
            } else {
                fprintf(stderr, " %d", ((unsigned int*)blog)[0]); blog+=4;
            }
        }
        fprintf(stderr, "\n");
#endif
        free(ibuf);
        free(vbuf);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

	if (gzclose(in) != Z_OK) fprintf(stderr, "  failed gzclose");

    fprintf(stderr, "    Creating object id=%d.\n", objectList.size());
    //
    tObject tempObject;
    tempObject.meshes_count = meshes_count;
    tempObject.frames_count = frames_count;
    tempObject.meshes = meshes;
    tempObject.frames = frames;
    objectList.push_back(tempObject);

//    for(int ff=0; ff<frames_count; ff++) if(frames[ff].framedata) free(frames[ff].framedata);
//    free(frames);
//    free(meshes);
    return objectList.size()-1;
}

int ObjectStatistic()
{
//    vector<tObject>::iterator ObIt;

//    for( ObIt = objectList.begin(); ti < objectList.end(); it++){
    fprintf(stderr, "Object statistic\n");
    for(unsigned int i = 0; i<objectList.size(); i++){
        tObject object = objectList.at(i);
        fprintf(stderr, "  Object ID=%d\n", i);
        fprintf(stderr, "    Meshes count = %d\n", object.meshes_count);
        fprintf(stderr, "    Frames count = %d\n", object.frames_count);

        for(int m=0; m<object.meshes_count; m++){
            tObjectMesh *mesh = &object.meshes[m];
            fprintf(stderr, "    Mesh id [%d]", m);
            fprintf(stderr, "  attrib: 0x%02X", mesh->attribs);
            fprintf(stderr, "  vertex count: %d", mesh->vertex_count);
            fprintf(stderr, "  index count: %d", mesh->index_count);
            fprintf(stderr, "  VAO = %d", mesh->vao);
            fprintf(stderr, "  VBO = (%d, %d)", mesh->vbo[0], mesh->vbo[1]);
            fprintf(stderr, "\n");
            //void *data =
            //for(int v=0; v<mesh->vertex_count; v++){
            //    fprintf(stderr, "      v[%d]", v);
            //    fprintf(stderr, "\n");
            //}
        }
        for(int f=0; f<object.frames_count; f++){
            tObjectFrame *frame = &object.frames[f];
            fprintf(stderr, "    Frame [%d]", f);
            fprintf(stderr, " meshes %d [", frame->meshes_in_frame);
            for(int m=0; m<frame->meshes_in_frame; m++){
                fprintf(stderr, " %04X", frame->framedata[m].mesh);
                fprintf(stderr, " %04X", frame->framedata[m].color);
            }
            fprintf(stderr, "\n");
        }
    }
    return 1;
}

int ObjectUnloadAll()
{
    fprintf(stderr, "Unloading all objects...\n");

    drawVertexNormalMeshes.clear();
    drawVertexNormalColorMeshes.clear();

    int oc = objectList.size();
    for(int i=objectList.size()-1; i>=0; i--){
        fprintf(stderr, "  Object ID=%d\n", --oc);
        tObject *object = &objectList[i];

        for(int ff=0; ff<object->frames_count; ff++) if(object->frames[ff].framedata) free(object->frames[ff].framedata);

        for(int m=object->meshes_count-1; m>=0; m--){
            glDeleteBuffers(2, object->meshes[m].vbo);
            glDeleteVertexArrays(1, &object->meshes[m].vao);
        }
        free(object->frames);
        free(object->meshes);
//        objectList.at(i).
//        free(objectList.at(i));
//
    }
    objectList.clear();
    return 1;
}

extern GLfloat cameramatrix[16];
extern GLfloat projectionmatrix[16]; /* Our projection matrix starts with all 0s */
extern GLfloat modelmatrix[16]; /* Our model matrix  */
//extern GLfloat light_pos[4];
//extern GLfloat cur_color[3];

//    tObjectMesh *mesh;
//    GLfloat modelmatrix[16];
#ifdef USE_MESHSORT
int ObjectAddDraw(tObjectMesh *mesh, GLfloat *modelmatrix, GLfloat *color)
{
    tDrawMesh newmesh;
    newmesh.mesh = mesh;
    memcpy(newmesh.modelmatrix, modelmatrix, 16*sizeof(GLfloat));
    newmesh.color[0] = color[0];
    newmesh.color[1] = color[1];
    newmesh.color[2] = color[2];

    if(mesh->attribs & MESH_COLOR) drawVertexNormalColorMeshes.push_back(newmesh);
    else drawVertexNormalMeshes.push_back(newmesh);
    return 1;
}

int ObjectPrepareDraw()
{
    drawVertexNormalMeshes.clear();
    drawVertexNormalColorMeshes.clear();
    return 1;
}

int ObjectDrawArrays()
{
//    vector<tObject>::iterator ObIt;
//    std::vector <tDrawMesh> drawVertexNormalMeshes;

//    for( ObIt = objectList.begin(); ti < objectList.end(); it++){
/*
        GLuint shader_program = SHADER_VERTEX_NORMAL;
//        shader_current = &shader_vertexnormal;
        if(mesh->attribs & MESH_TEXTURE){
            if(mesh->attribs & MESH_COLOR){
                shader_program = SHADER_VERTEX_NORMAL_UV_COLOR;
            } else {
                shader_program = SHADER_VERTEX_NORMAL_UV;
            }
        } else {
            if(mesh->attribs & MESH_COLOR){
                shader_program = SHADER_VERTEX_NORMAL_COLOR;
            }
        }
*/
    int oc = drawVertexNormalMeshes.size();
#if 1
    for(int i = 0; i<oc; i++){
        tObjectMesh *mesh = drawVertexNormalMeshes[i].mesh;// frame->framedata[m].mesh;

//        GLfloat commmatrix[16];
//        multiply4x4n(commmatrix, drawVertexNormalMeshes[i].modelmatrix, mesh.);
        ShaderDo(SHADER_VERTEX_NORMAL, projectionmatrix, drawVertexNormalMeshes[i].modelmatrix, drawVertexNormalMeshes[i].color);
//        ShaderDo(SHADER_VERTEX_NORMAL, projectionmatrix, , drawVertexNormalMeshes[i].color);

        glBindVertexArray(mesh->vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);

        int stride = (3*2+3*2) + 0;
        glVertexAttribPointer((GLuint)0, 3, GL_HALF_FLOAT, GL_FALSE, stride, (GLvoid*)0);    // vertex coordinates
        glEnableVertexAttribArray(0);
        glVertexAttribPointer((GLuint)1, 3, GL_HALF_FLOAT, GL_FALSE, stride, (GLvoid*)(3*2));  // normals
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[1]);
        int indexcount = mesh->index_count;
        GLenum index_type = GL_UNSIGNED_BYTE;

        if (mesh->vertex_count<256) index_type = GL_UNSIGNED_BYTE;
        else if (mesh->vertex_count<65536) index_type = GL_UNSIGNED_SHORT;
        else index_type = GL_UNSIGNED_INT;

#ifdef USE_ADJ
        glDrawElements(GL_TRIANGLES_ADJACENCY, indexcount, index_type, (GLvoid*)0);
#else
        glDrawElements(GL_TRIANGLES, indexcount, index_type, (GLvoid*)0);
#endif
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
#endif

#if 1
//    glDisable(GL_DEPTH_TEST);
//	glDisable(GL_FRAGMENT_PROGRAM_NV);
//	glDisable(GL_VERTEX_PROGRAM_ARB);
// Outlines
    glDepthFunc(GL_LEQUAL);
    for(int i = 0; i<oc; i++){
        tObjectMesh *mesh = drawVertexNormalMeshes[i].mesh;// frame->framedata[m].mesh;

//        GLfloat commmatrix[16];
//        multiply4x4n(commmatrix, drawVertexNormalMeshes[i].modelmatrix, mesh.);
        ShaderDo(SHADER_VERTEX_NORMAL_OUT, projectionmatrix, drawVertexNormalMeshes[i].modelmatrix, drawVertexNormalMeshes[i].color);
//        ShaderDo(SHADER_VERTEX_NORMAL, projectionmatrix, , drawVertexNormalMeshes[i].color);

        glBindVertexArray(mesh->vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);

        int stride = (3*2+3*2) + 0;
        glVertexAttribPointer((GLuint)0, 3, GL_HALF_FLOAT, GL_TRUE, stride, (GLvoid*)0);    // vertex coordinates
        glEnableVertexAttribArray(0);
        glVertexAttribPointer((GLuint)1, 3, GL_HALF_FLOAT, GL_TRUE, stride, (GLvoid*)(3*2));  // normals
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[1]);
        int indexcount = mesh->index_count;
        GLenum index_type = GL_UNSIGNED_BYTE;

        if (mesh->vertex_count<256) index_type = GL_UNSIGNED_BYTE;
        else if (mesh->vertex_count<65536) index_type = GL_UNSIGNED_SHORT;
        else index_type = GL_UNSIGNED_INT;

#ifdef USE_ADJ
        glDrawElements(GL_TRIANGLES_ADJACENCY, indexcount, index_type, (GLvoid*)0);
#else
        glDrawElements(GL_TRIANGLES, indexcount, index_type, (GLvoid*)0);
#endif


        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
//        glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
#endif

    oc = drawVertexNormalColorMeshes.size();
    for(int i = 0; i<oc; i++){
        tObjectMesh *mesh = drawVertexNormalColorMeshes[i].mesh;// frame->framedata[m].mesh;

        ShaderDo(SHADER_VERTEX_NORMAL_COLOR, projectionmatrix, drawVertexNormalColorMeshes[i].modelmatrix, drawVertexNormalColorMeshes[i].color);

        glBindVertexArray(mesh->vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);

        int stride = (3*2+3*2) + 4;
        glVertexAttribPointer((GLuint)0, 3, GL_HALF_FLOAT, GL_FALSE, stride, (GLvoid*)0);    // vertex coordinates
        glEnableVertexAttribArray(0);
        glVertexAttribPointer((GLuint)1, 3, GL_HALF_FLOAT, GL_FALSE, stride, (GLvoid*)(3*2));  // normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer((GLuint)2, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid*)(3*2+3*2));  // color
        glEnableVertexAttribArray(2);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[1]);
        int indexcount = mesh->index_count;
        GLenum index_type = GL_UNSIGNED_BYTE;

        if (mesh->vertex_count<256) index_type = GL_UNSIGNED_BYTE;
        else if (mesh->vertex_count<65536) index_type = GL_UNSIGNED_SHORT;
        else index_type = GL_UNSIGNED_INT;

#ifdef USE_ADJ
        glDrawElements(GL_TRIANGLES_ADJACENCY, indexcount, index_type, (GLvoid*)0);
#else
        glDrawElements(GL_TRIANGLES, indexcount, index_type, (GLvoid*)0);
#endif
//        glDrawElements(GL_TRIANGLES, indexcount, index_type, (GLvoid*)0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(3);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    return 1;
}
#else
int ObjectPrepareDraw(){ return 1;}
int ObjectDrawArrays(){ return 1;}

#endif

int ObjectDraw(unsigned int id, int nframe)
{
    GLfloat commmatrix[16];
    multiply4x4n(commmatrix, modelmatrix, cameramatrix);

    if(id >= objectList.size()) return 0;
    if(id < 0) return 0;

    int maxframes = objectList.at(id).frames_count;
    if(nframe>=maxframes) nframe = nframe % maxframes;

    tObjectFrame* frame = &objectList.at(id).frames[nframe];

//    frame->framedata[m].mesh;

    for(int m=0; m<frame->meshes_in_frame; m++){
//        tObjectMesh *mesh = &objectList.at(id).meshes[m];
        tObjectMesh *mesh = frame->framedata[m].mesh;

        GLfloat cur_color[3];
#ifdef USE_MESHSORT
        cur_color[0] = (float)frame->framedata[m].color.r/255.0;
        cur_color[1] = (float)frame->framedata[m].color.g/255.0;
        cur_color[2] = (float)frame->framedata[m].color.b/255.0;

        GLfloat fcommmatrix[16];
        multiply4x4n(fcommmatrix, frame->framedata[m].modelmatrix, commmatrix);
        ObjectAddDraw(mesh, fcommmatrix, cur_color);
#else
        GLenum mode = GL_TRIANGLES;
        //GLenum mode = GL_QUADS;

        if(mesh->attribs & MESH_QUAD) mode = GL_QUADS;
//        if(mesh->attribs & MESH_QUAD) mode = GL_LINES;

        GLuint shader_program = SHADER_VERTEX_NORMAL;
//        shader_current = &shader_vertexnormal;
        if(mesh->attribs & MESH_TEXTURE){
            if(mesh->attribs & MESH_COLOR){
                shader_program = SHADER_VERTEX_NORMAL_UV_COLOR;
            } else {
                shader_program = SHADER_VERTEX_NORMAL_UV;
            }
        } else {
            if(mesh->attribs & MESH_COLOR){
                shader_program = SHADER_VERTEX_NORMAL_COLOR;
            }
        }

        cur_color[0] = (float)frame->framedata[m].color.r/255.0;
        cur_color[1] = (float)frame->framedata[m].color.g/255.0;
        cur_color[2] = (float)frame->framedata[m].color.b/255.0;
        ShaderDo(shader_program, projectionmatrix, commmatrix, cur_color);

        glBindVertexArray(mesh->vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);

/*
        int vdatasize = 3*2 + 3*2;  // 3 HALF_FLOAT + 3 HALF_FLOAT  - coordinate + normal
        if(mesh->attribs & MESH_TEXTURE) vdatasize += 2*2; // + 2 HALF_FLOAT - texture coordinate
        if(mesh->attribs & MESH_COLOR) vdatasize += 4; // + 4 BYTE - vertex color
        int stride = vdatasize;
        glVertexAttribPointer((GLuint)0, 3, GL_HALF_FLOAT, GL_FALSE, stride, (GLvoid*)0);    // vertex coordinates
        glEnableVertexAttribArray(0);
        glVertexAttribPointer((GLuint)1, 3, GL_HALF_FLOAT, GL_FALSE, stride, (GLvoid*)(3*2));  // normals
        glEnableVertexAttribArray(1);
        if(mesh->attribs & MESH_TEXTURE){
            glVertexAttribPointer((GLuint)2, 2, GL_HALF_FLOAT, GL_FALSE, stride, (GLvoid*)(3*2+3*2));  // texture coordinares
            glEnableVertexAttribArray(2);
            if(mesh->attribs & MESH_COLOR){
                glVertexAttribPointer((GLuint)3, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid*)(3*2+3*2+2*2));  // color
                glEnableVertexAttribArray(3);
            }
        } else {
                glVertexAttribPointer((GLuint)3, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid*)(3*2+3*2));  // color
                glEnableVertexAttribArray(3);
        }
*/
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[1]);
        int indexcount = mesh->index_count;
        GLenum index_type = GL_UNSIGNED_BYTE;

        if (mesh->vertex_count<256) index_type = GL_UNSIGNED_BYTE;
        else if (mesh->vertex_count<65536) index_type = GL_UNSIGNED_SHORT;
        else index_type = GL_UNSIGNED_INT;

        glDrawElements(mode, indexcount, index_type, (GLvoid*)0);
#endif
    }
    return 1;
}
