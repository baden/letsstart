#version 150

precision highp float;

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 light_pos;

//uniform vec3 colour;

//in vec3 gl_Color;
//in vec4 gl_Color;

in vec3 in_Position;
in vec3 in_Normal;
//in vec2 in_TexCoord;
//in vec3 in_Color;

//out float diffuse;
out vec3 normal, lightDir;
//out float p_age;
//out vec3 p_Color;
//out vec2 p_TexCoord;

/*
void shade_diffuse_toon(vec3 n, vec3 l, vec3 v, float size, float tsmooth, out float is)
{
	float rslt = dot(n, l);
	float ang = acos(rslt);

	if(ang < size) is = 1.0;
	else if(ang > (size + tsmooth) || tsmooth == 0.0) is = 0.0;
	else is = 1.0 - ((ang - size)/tsmooth);
}
*/

#define size 0.1
#define tsmooth 0.0

void main(void) {
//    mat4 cmatrix = projection * modelview;

//	vec3 normal0 = normalize(mat3(projection * modelview) * in_Normal);
//    p_age = clamp(dot(normal0, vec3(0.0, 0.0, -5000.0)), 0.0, 1.0);
#if 1
    vec4 ecPos;
    ecPos = vec4(modelview * vec4(in_Position, 1.0));
//    ecPos = vec4(vec4(in_Position, 1.0));
    lightDir = normalize(vec3(light_pos) - ecPos.xyz);
    normal = mat3(modelview) * in_Normal;
    gl_Position = projection * modelview * vec4(in_Position, 1.0);
//    p_Color = in_Color * colour; // 256.0;
//    p_TexCoord = in_TexCoord;
    //p_Color = vec3(gl_Vertex);
//    p_Color = colour * in_Color / 256.0;
#endif
}
