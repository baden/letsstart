#version 140

precision highp float;

uniform mat4 modelview;
uniform mat4 projection;

uniform vec3 colour;

in vec3 in_Position;
//in vec3 in_Normal;
//in vec2 in_TexCoord;
//gl_MultiTexCoord0

out vec3 p_Color;
out vec2 p_TexCoord;

void main(void) {
//    gl_Position = projection * modelview * vec4(in_Position, 1.0);
    gl_Position = modelview * vec4(in_Position, 1.0);
//    p_Color = in_Color * colour;
    //p_TexCoord = in_TexCoord;
    p_TexCoord = in_Position.xy;
//    p_TexCoord = in_Normal.xy;
//    p_TexCoord[0] = gl_MultiTexCoord0;
}
