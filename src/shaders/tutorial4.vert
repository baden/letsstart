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
//in vec3 in_Color;

out float p_diffuse;
//out float p_age;
//out vec3 geom_Color;

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
	vec4 vertex    = vec4(in_Position, 1.0);
	vec4 direction = normalize(light_pos - vertex);
	vec3 normal1 = normalize(mat3(modelview) * in_Normal);
	p_diffuse   = clamp(dot(normal1, direction.xyz), 0.0, 1.0);

//	if(diffuse < size) diffuse = 0.2;
//	else if(diffuse > (size + tsmooth) || tsmooth == 0.0) diffuse = 1.0;
//	else diffuse = /*1.0 - */((diffuse - size)/tsmooth);

#endif

#if 1
    gl_Position = vec4(in_Position, 1.0);
#endif

#if 0
    gl_Position = vec4(in_Position, 1.0) + vec4(sin(in_Position.x*20.0)/20.0, 0.0, 0.0, 0.0);
#endif

#if 0
    geom_Color = in_Color;
#endif
#if 0
    geom_Color = in_Color * diffuse;
#endif
    //geom_Color = colour * diffuse;
//    geom_Color = gl_Color.rgb * diffuse;
}
