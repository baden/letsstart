#version 150

precision highp float;

uniform mat4 modelview;
uniform mat4 projection;
uniform vec4 light_pos;

in vec3 in_Position;
in vec3 in_Normal;

out vec3 geom_normal, geom_lightDir;

#define size 0.1
#define tsmooth 0.0

void main(void) {
    vec4 ecPos;
    ecPos = vec4(modelview * vec4(in_Position, 1.0));
    geom_lightDir = normalize(vec3(light_pos) - ecPos.xyz);
    geom_normal = mat3(modelview) * in_Normal;
    gl_Position = projection * modelview * vec4(in_Position, 1.0);
}
