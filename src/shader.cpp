#include <stdlib.h>
#include <stdio.h>
#include <gl3/gl3.h>

#include "shader.h"
#include "extensions.h"

tShader *shader_current;

tShader shader_vertexnormal;
tShader shader_vertexnormaluv;
tShader shader_vertexnormaluvcolor;
tShader shader_vertexnormalcolor;

tShader shader_vertexuv;        // Maybe not be usable
tShader shader_vertexcolor;

tShader shader_text;

static GLint shader_status(GLuint handle, GLenum param, GLchar * programsrc)
{
	GLint status, length;
	char buffer[1024];

	glGetShaderiv(handle, param, &status);

	if (status != GL_TRUE)
	{
		glGetShaderInfoLog(handle, 1024, &length, (GLchar*)buffer);
		fprintf(stderr, "GLSL fail: %s\n", buffer);

//		fprintf(stderr, "PROGRAM[%s]\n", programsrc);
	}

	return status;
}


/* A simple function that will read a file into an allocated char pointer buffer */
char* filetobuf(const char *file)
{
    FILE *fptr;
    long length;
    char *buf;

    fprintf(stderr, "Loading file: %s\n", file);

    fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file plus a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
}

GLfloat light_pos[4] = {-10.0f, 10.0f, 20.0f, 1.0f};

void initShader(tShader *shader, const char *vertfilename, const char *geomfilename, const char *fragfilename)
{

/*
    vertexsource = filetobuf("tutorial4.vert");
    geometrysource = filetobuf("tutorial4.geom");
    fragmentsource = filetobuf("tutorial4.frag");
*/
    char shaderpath[128];
    sprintf(shaderpath, "shaders\\%s", vertfilename);
    shader->vertexsource = filetobuf(shaderpath);
    if(geomfilename){
        sprintf(shaderpath, "shaders\\%s", geomfilename);
        shader->geometrysource = filetobuf(shaderpath);
    }
    else shader->geometrysource = NULL;
    sprintf(shaderpath, "shaders\\%s", fragfilename);
    shader->fragmentsource = filetobuf(shaderpath);

    /* Assign our handles a "name" to new shader objects */
    shader->vertexshader = glCreateShader(GL_VERTEX_SHADER);
    if(shader->geometrysource) shader->geometryshader = glCreateShader(GL_GEOMETRY_SHADER);
    shader->fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

    /* Associate the source code buffers with each handle */
    glShaderSource(shader->vertexshader, 1, (const GLchar**)&shader->vertexsource, 0);
    if(shader->geometrysource) glShaderSource(shader->geometryshader, 1, (const GLchar**)&shader->geometrysource, 0);
    glShaderSource(shader->fragmentshader, 1, (const GLchar**)&shader->fragmentsource, 0);

    /* Compile our shader objects */
    fprintf(stderr, "Compiling vertex shader.\n");
    glCompileShader(shader->vertexshader);      shader_status(shader->vertexshader, GL_COMPILE_STATUS, shader->vertexsource);
    if(shader->geometrysource) {
        fprintf(stderr, "Compiling geometry shader.\n");
        glCompileShader(shader->geometryshader);    shader_status(shader->geometryshader, GL_COMPILE_STATUS, shader->geometrysource);
    }
    fprintf(stderr, "Compiling fragment shader.\n");
    glCompileShader(shader->fragmentshader);    shader_status(shader->fragmentshader, GL_COMPILE_STATUS, shader->fragmentsource);

    /* Assign our program handle a "name" */
    shader->shaderprogram = glCreateProgram();

    /* Attach our shaders to our program */
    glAttachShader(shader->shaderprogram, shader->vertexshader);
    if(shader->geometrysource) glAttachShader(shader->shaderprogram, shader->geometryshader);
    glAttachShader(shader->shaderprogram, shader->fragmentshader);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
//    switch(model01.attr.data){

    /* Link our program, and set it as being actively used */
    glLinkProgram(shader->shaderprogram);
    glUseProgram(shader->shaderprogram);

    glBindAttribLocation(shader->shaderprogram, 0, "in_Position");
    glBindAttribLocation(shader->shaderprogram, 1, "in_Normal");
    glBindAttribLocation(shader->shaderprogram, 2, "in_TexCoord");
    glBindAttribLocation(shader->shaderprogram, 3, "in_Color");
    glUniform1i(glGetUniformLocation(shader->shaderprogram, "Texture0"), 0);

    shader->projection_u = glGetUniformLocation(shader->shaderprogram, "projection");
    shader->light_pos_u = glGetUniformLocation(shader->shaderprogram, "light_pos");
    shader->colour_u = glGetUniformLocation(shader->shaderprogram, "colour");
    shader->modelview_u = glGetUniformLocation(shader->shaderprogram, "modelview");

//    fprintf(stderr, "SH: %08X - %d, %d, %d, %d\n", shader, shader->projection_u, shader->light_pos_u, shader->colour_u, shader->modelview_u );

//    if(shader->light_pos_u != -1) glUniform4fv(shader->light_pos_u, 1, light_pos);

    shader_current = shader;
}

void doneShader(tShader *shader)
{
    glDetachShader(shader->shaderprogram, shader->vertexshader);
//    glDetachShader(shaderprogram, geometryshader);
    glDetachShader(shader->shaderprogram, shader->fragmentshader);
    glDeleteProgram(shader->shaderprogram);
    glDeleteShader(shader->vertexshader);
//    glDeleteShader(geometryshader);
    glDeleteShader(shader->fragmentshader);

    free(shader->vertexsource);
    if(shader->geometrysource) free(shader->geometrysource);
    free(shader->fragmentsource);
}

void initShaders(void)
{
//    initShader(&shader_vertexnormal, "vertex_normal.vert", "vertex_normal.geom", "vertex_normal.frag");
    initShader(&shader_vertexnormal, "vertex_normal.vert", NULL, "vertex_normal.frag");
    initShader(&shader_vertexnormaluv, "vertex_normal_uv.vert", NULL, "vertex_normal_uv.frag");
    initShader(&shader_vertexnormaluvcolor, "vertex_normal_uv_color.vert", NULL, "vertex_normal_uv_color.frag");
    initShader(&shader_vertexnormalcolor, "vertex_normal_color.vert", NULL, "vertex_normal_color.frag");

    initShader(&shader_text, "text.vert", NULL, "text.frag");
}

void doneShaders(void)
{
    glUseProgram(0);
    doneShader(&shader_vertexnormal);
    doneShader(&shader_vertexnormaluv);
    doneShader(&shader_vertexnormaluvcolor);
    doneShader(&shader_vertexnormalcolor);
    doneShader(&shader_text);
}

void ShaderSetCurent(int shader_program)
{
    tShader *new_shader_current;
    switch(shader_program){
        case SHADER_VERTEX_NORMAL: new_shader_current = &shader_vertexnormal; break;
        case SHADER_VERTEX_NORMAL_UV: new_shader_current = &shader_vertexnormaluv; break;
        case SHADER_VERTEX_NORMAL_UV_COLOR: new_shader_current = &shader_vertexnormaluvcolor; break;
        case SHADER_VERTEX_NORMAL_COLOR: new_shader_current = &shader_vertexnormalcolor; break;
        case SHADER_TEXT: new_shader_current = &shader_text; break;
    }
    if(new_shader_current != shader_current){
        shader_current = new_shader_current;
        glUseProgram(shader_current->shaderprogram);
    }
    glUniform1i(glGetUniformLocation(shader_current->shaderprogram, "Texture0"), 0);
    if(shader_current->light_pos_u != -1) glUniform4fv(shader_current->light_pos_u, 1, light_pos);
}

void ShaderDo(int shader_program, float *projectionmatrix, float *modelmatrix, float *color){
    ShaderSetCurent(shader_program);
    glUniformMatrix4fv(shader_current->projection_u, 1, GL_FALSE, projectionmatrix);
//    glUniform4fv(shader_current->light_pos_u, 1, light_pos);
    glUniform3fv(shader_current->colour_u, 1, color);
    glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, modelmatrix);
}

void ShaderDoProjectionMatrix(float *matrix)
{
    glUniformMatrix4fv(shader_current->projection_u, 1, GL_FALSE, matrix);
}

void ShaderDoModelMatrix(float *modelmatrix)
{
    glUniformMatrix4fv(shader_current->modelview_u, 1, GL_FALSE, modelmatrix);
}

void ShaderDoColor(float *color)
{
    glUniform3fv(shader_current->colour_u, 1, color);
}
