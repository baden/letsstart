#version 140

precision highp float;

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 light_pos;

//uniform vec3 colour;

//in vec3 gl_Color;
//in vec4 gl_Color;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
//in vec3 in_Color;

//out float diffuse;
out vec3 normal, lightDir;
//out float p_age;
//out vec3 p_Color;
out vec2 p_TexCoord;

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
    vec4 ecPos;
    ecPos = vec4(modelview * vec4(in_Position, 1.0));
    lightDir = normalize(vec3(light_pos) - ecPos.xyz);
    normal = normalize(mat3(modelview) * in_Normal);
    gl_Position = projection * modelview * vec4(in_Position, 1.0);
//    p_Color = in_Color * colour; // 256.0;
    p_TexCoord = in_TexCoord;
}
