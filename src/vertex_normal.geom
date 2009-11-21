#version 150

precision highp float;

//uniform mat4 modelview;
//uniform mat4 projection;
//uniform vec4 light_pos;

// Declare what type of incoming primitive our geometry shader is receiving
layout(triangles) in;

// Declare what type of primitives we are creating and the maximum amount of vertices we will output per use of the geometry shader.
// We will be outputting 6 vertices per use of this shader, creating 2 triangles.
layout(triangle_strip, max_vertices = 12) out;

// Inputs to geometry shaders are always as arrays in the quantity of the incoming primitive
// In our case this value is 3, since triangles have 3 vertices
//in vec3 i_normal, i_lightDir;
out vec3 normal, lightDir;

void main()
{
    // simple iterator
    int i;

    // Create our original primitive
    for (i=0; i < gl_in.length(); i++)
    {
#if 1
    //normal = i_normal;
    //lightDir = i_lightDir;
    normal = vec3(0.5);
    lightDir = vec3(0.5);
	gl_Position = gl_in[i].gl_Position;
#endif
        EmitVertex();
    }
    EndPrimitive();

#if 0
    // Create duplicate primitive
    for (i=0; i < gl_in.length(); i++)
    {
#if 1
	gl_Position = cmatrix * gl_in[i].gl_Position + vec4(1.0, 1.0, 1.0, 1.0);
#endif
//        ex_Color = geom_Color[i];
        //diffuse = gl_in.p_diffuse;
        //diffuse = p_diffuse[i];
        diffuse = 1;
//        age = p_age[i];
  //      diffuse = gl_in[i].diffuse;
        EmitVertex();
    }
    EndPrimitive();
#endif
}

