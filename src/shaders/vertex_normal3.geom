#version 150

precision highp float;

//uniform mat4 modelview;
//uniform mat4 projection;
//uniform vec4 light_pos;

// Declare what type of incoming primitive our geometry shader is receiving
//layout(triangles) in;
layout(triangles_adjacency) in;

// Declare what type of primitives we are creating and the maximum amount of vertices we will output per use of the geometry shader.
// We will be outputting 6 vertices per use of this shader, creating 2 triangles.
//layout(triangle_strip, max_vertices = 24) out;
layout(line_strip, max_vertices = 20) out;
//layout(points, max_vertices = 12) out;

// Inputs to geometry shaders are always as arrays in the quantity of the incoming primitive
// In our case this value is 3, since triangles have 3 vertices
//in vec3 i_normal, i_lightDir;
//in vec3 normal, lightDir;
in vec3 geom_normal[6], geom_lightDir[6];
//out vec3 normal, lightDir;
out float intensity;

/*
vec2 average(vec2[3] array)
{
    vec2 sum = 0;
    int i;
    for(i=0; i<array.length(); i++){
        sum =
    }
}
*/

float facing2d(vec2 v0, vec2 v1, vec2 v2)
{
    return v0.x * (v1.y - v2.y) + v1.x * (v2.y - v0.y) + v2.x * (v0.y - v1.y);
}

// calculating facing of a triangle relative to eye
float facing3d(vec3 v0, vec3 v1, vec3 v2, vec4 eye)
{
    vec3 e0 = v1 - v0;
    vec3 e1 = v2 - v0;
    vec4 p;
    p.xyz = cross(e1, e0);
    p.w = -dot(v0, p.xyz);
//    return dot(p, vec4(0.0, 0.0, 1.0, 0.0));
    return dot(p, eye);
}

void main()
{
    // simple iterator
    int i;

    intensity = 0.0;

//    if(facing(gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[4].gl_Position.xyz) < 0){
//        intensity = 0.0;
//        return;
//    }
    float f;
    vec4 eye = vec4( 0, 0, 0, -1 );

    f = facing3d(gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_in[4].gl_Position.xyz, eye);

    if(f>=0){
//        intensity = 0.0;
        return;
    }
    // Edge 1
    f = facing3d(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, eye);
    if(f>=0){
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        gl_Position = gl_in[2].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
    // Edge 2
    f = facing3d(gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz, gl_in[4].gl_Position.xyz, eye);
    if(f>=0){
        gl_Position = gl_in[2].gl_Position;
        EmitVertex();
        gl_Position = gl_in[4].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
    // Edge 3
    f = facing3d(gl_in[4].gl_Position.xyz, gl_in[5].gl_Position.xyz, gl_in[0].gl_Position.xyz, eye);
    if(f>=0){
        gl_Position = gl_in[4].gl_Position;
        EmitVertex();
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        EndPrimitive();
    }


/*
    if(facing2d(gl_in[0].gl_Position.xy, gl_in[2].gl_Position.xy, gl_in[4].gl_Position.xy) <= 0.0){
        intensity = 0.0;
        return;
    }
*/
/*
    if(false){
// Edge 1
//    if(facing(gl_in[0].gl_Position.xy, gl_in[1].gl_Position.xy, gl_in[2].gl_Position.xy) < 0){
        if(true){
            gl_Position = gl_in[0].gl_Position;
            EmitVertex();
            gl_Position = gl_in[2].gl_Position;
            EmitVertex();
            EndPrimitive();
        }
// Edge 2
//    if(facing(gl_in[2].gl_Position.xy, gl_in[3].gl_Position.xy, gl_in[4].gl_Position.xy) < 0){
        if(true){
            gl_Position = gl_in[2].gl_Position;
            EmitVertex();
            gl_Position = gl_in[4].gl_Position;
            EmitVertex();
            EndPrimitive();
        }
// Edge 3
//    if(facing(gl_in[4].gl_Position.xy, gl_in[5].gl_Position.xy, gl_in[0].gl_Position.xy) < 0){
    if(true){
        gl_Position = gl_in[4].gl_Position;
        EmitVertex();
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        EndPrimitive();
        }
    }
*/
}

