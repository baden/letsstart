#ifndef SHADER_H
#define SHADER_H


//void initShaders(void);
//void doneShaders(void);
typedef struct{
    /* These pointers will receive the contents of our shader source code files */
    GLchar *vertexsource, *geometrysource, *fragmentsource;
    /* These are handles used to reference the shaders */
    GLuint vertexshader, geometryshader, fragmentshader;
    /* This is a handle to the shader program */
    GLuint shaderprogram;

    GLuint modelview_u;
    GLuint projection_u;
    GLuint light_pos_u;
    GLuint colour_u;
} tShader;

//extern tShader *shader_current;
//extern tShader shader_vertexnormal;
//extern tShader shader_vertexnormaluv;
//extern tShader shader_vertexnormaluvcolor;
//extern tShader shader_vertexnormalcolor;

//extern tShader shader_vertexuv;        // Maybe not be usable
//extern tShader shader_vertexcolor;

extern tShader shader_text;

void initShader(tShader *shader);
void doneShader(tShader *shader);
void initShaders(void);
void doneShaders(void);

void ShaderSetCurent(int shader_program);
void ShaderDo(int shader_program, float *projectionmatrix, float *modelmatrix, float *color);
void ShaderDoModelMatrix(float *modelmatrix);
void ShaderDoProjectionMatrix(float *matrix);
void ShaderDoColor(float *color);

#define SHADER_VERTEX_NORMAL            0
#define SHADER_VERTEX_NORMAL_UV         1
#define SHADER_VERTEX_NORMAL_UV_COLOR   2
#define SHADER_VERTEX_NORMAL_COLOR      3
#define SHADER_VERTEX_NORMAL_OUT        5
#define SHADER_TEXT              4

#endif
