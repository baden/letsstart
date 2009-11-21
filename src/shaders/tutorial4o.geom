#version 150

precision highp float;

uniform mat4 modelview;
uniform mat4 projection;

// Declare what type of incoming primitive our geometry shader is receiving
layout(triangles) in;

// Declare what type of primitives we are creating and the maximum amount of vertices we will output per use of the geometry shader.
// We will be outputting 6 vertices per use of this shader, creating 2 triangles.
layout(triangle_strip, max_vertices = 120) out;

// Inputs to geometry shaders are always as arrays in the quantity of the incoming primitive
// In our case this value is 3, since triangles have 3 vertices
in vec3 geom_Color[3];
out vec3 ex_Color;
out float diffuse;

void main() 
{
    // simple iterator
    int i;

//	vec4 vertex    = modelview * vec4(position, 1.0);
//	vec4 direction = normalize(light_pos - vertex);

//	normal    = normalize(mat3(modelview) * normal);
//	diffuse   = clamp(dot(normal, direction.xyz), 0.0, 1.0);
//	texcoords = texcoord;

//    mat4 cmatrix = modelview * projection;
    mat4 cmatrix = projection * modelview;

    // Create our original primitive
    for (i=0; i < gl_in.length(); i++)
    {
#if 0
	gl_Position = gl_in[i].gl_Position;
#endif
#if 1
//	vec4 vertex = modelview * gl_in[i].gl_Position;
//	gl_Position = projection * vertex;

	gl_Position = cmatrix * gl_in[i].gl_Position;

#endif
        ex_Color = geom_Color[i];
        EmitVertex();
    }
    EndPrimitive();

#if 0
    // Create a new primitive with negative x,y,z values
    for (i=0; i < gl_in.length(); i++)
    {
        gl_Position = modelview * vec4(-gl_in[i].gl_Position.xyz + vec3(3.0, 0.0, 0.0), 1.0);
        ex_Color = geom_Color[i];
        EmitVertex();
    }
    EndPrimitive();
#endif

#if 1
    // Create a new primitive with negative x,y,z values
   int j;
   for (j=0; j < 36; j++){
    vec3 trans1 = vec3(sin(j*6.283/36.0)*15.0, cos(j*6.283/36.0)*15.0, cos(j*6.283/18.0)*5.0);
    for (i=0; i < gl_in.length(); i++)
    {
#if 0
	vec4 vertex = modelview * vec4(gl_in[i].gl_Position.xyz*0.2 + trans1, 1.0);
	gl_Position = projection * vertex;
#endif
#if 0
	gl_Position = vec4(gl_in[i].gl_Position.xyz*0.8 + trans1, gl_in[i].gl_Position.w);
#endif
#if 1
//	vec4 vertex = modelview * vec4(gl_in[i].gl_Position.xyz*0.8 + trans1, gl_in[i].gl_Position.w);
//	gl_Position = projection * vertex;

	gl_Position = cmatrix * vec4(gl_in[i].gl_Position.xyz*0.8 + trans1, gl_in[i].gl_Position.w);


#endif

#if 0
//	gl_Position = projection * vertex;
        gl_Position = modelview * vec4(gl_in[i].gl_Position.xyz*0.2 + trans1, 1.0);
//	vec4 tmp = mvpmatrix * vec4(gl_in[i].gl_Position.xyz*0.2 + trans1, 1.0);
//        gl_Position = vec4(tmp.x, tmp.y/10, tmp.z, tmp.w);
//       gl_Position = vec4(gl_in[i].gl_Position.xyz*0.2 + trans1, 1.0);
#endif
        ex_Color = geom_Color[i];
        EmitVertex();
    }
    EndPrimitive();
   }
#endif

#if 1
	gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
        ex_Color = vec3(0.0, 0.0, 0.0);
        EmitVertex();
	gl_Position = vec4(0.5, 1.0, 0.0, 1.0);
        ex_Color = vec3(0.5, 1.0, 1.0);
        EmitVertex();
	gl_Position = vec4(1.0, 0.0, 0.0, 1.0);
        ex_Color = vec3(1.0, 0.0, 1.0);
        EmitVertex();

	gl_Position = vec4(0.5, -0.5, 0.0, 1.0);
        ex_Color = vec3(1.0, 1.0, 1.0);
        EmitVertex();
	gl_Position = vec4(-0.5, -0.5, 0.0, 1.0);
        ex_Color = vec3(1.0, 1.0, 1.0);
        EmitVertex();
	gl_Position = vec4(-0.5, 0.0, 0.0, 1.0);
        ex_Color = vec3(1.0, 1.0, 1.0);
        EmitVertex();
	EndPrimitive();
#endif

}
