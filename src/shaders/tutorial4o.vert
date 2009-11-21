#version 150

precision highp float;

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 light_pos;

in vec3 in_Position;
in vec3 in_Color;

out float diffuse;
out vec3 geom_Color;

void main(void) {

#if 0
	vec4 vertex    = vec4(in_Position, 1.0);
	vec4 direction = normalize(light_pos - vertex);
	vec3 normal1 = normalize(mat3(modelview) * normal);
	diffuse   = clamp(dot(normal1, direction.xyz), 0.0, 1.0);
#endif

#if 1
    gl_Position = vec4(in_Position, 1.0);
#endif
    geom_Color = in_Color;
}
