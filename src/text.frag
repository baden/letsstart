#version 140

precision highp float;

uniform vec3 colour;
uniform sampler2D Texture0;

in vec2 p_TexCoord;

out vec4 gl_FragColor;

void main(void) {
    vec3 n;
    float intensity;

    gl_FragColor = texture2D(Texture0, p_TexCoord) * vec4(colour, 1.0);
//    gl_FragColor = texture2D(Texture0, p_TexCoord);
    //gl_FragColor = vec4(colour, 1.0);
}
