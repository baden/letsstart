#version 150

precision highp float;

//uniform vec3 colour;
//uniform sampler2D Texture0;

//in  vec3 p_Color;
//in vec2 p_TexCoord;

//in float diffuse;
//in vec3 normal, lightDir;
in float intensity;

out vec4 gl_FragColor;
//in float age;

#define BRICKWIDTH  0.12
#define	BRICKHEIGHT  0.04
#define MORTARTHICKNESS  0.02

#define	BMWIDTH (BRICKWIDTH + MORTARTHICKNESS)
#define	BMHEIGHT (BRICKWIDTH + MORTARTHICKNESS)
#define MWF (MORTARTHICKNESS * 0.5 / BMWIDTH)
#define MHF (MORTARTHICKNESS * 0.5 / BMHEIGHT)

void main(void) {
/*
    vec3 n;
    float intensity;
    float intensity1;
    float intensity2;
    float intensity3;

    n = normalize(normal);
    intensity = dot(lightDir, n);

//    if(intensity>0.70) gl_FragColor = vec4(colour*1.0, 1.0);
//    else if(intensity>0.4) gl_FragColor = vec4(colour*0.5, 1.0);
//    else if(intensity>0.2) gl_FragColor = vec4(colour*0.3, 1.0);
//    else gl_FragColor = vec4(colour*0.2, 1.0);
    intensity1 = smoothstep(0.78, 0.82, intensity)*0.5;
    intensity2 = smoothstep(0.38, 0.42, intensity)*0.3;
    intensity3 = smoothstep(0.18, 0.22, intensity)*0.2;

    intensity = intensity1 + intensity2 + intensity3;
//    if(intensity>0.70) gl_FragColor = vec4(colour*1.0, 1.0);
//    else if(intensity>0.4) gl_FragColor = vec4(colour*0.5, 1.0);
//    else if(intensity>0.2) gl_FragColor = vec4(colour*0.3, 1.0);
//    else gl_FragColor = vec4(colour*0.2, 1.0);

*/
    //gl_FragColor = vec4(colour*intensity, 1.0);
    gl_FragColor = vec4(intensity, intensity, intensity, 1.0);
    //gl_PointSize = 5.0;
    //gl_FragColor =
    //gl_FragColor = vec4(p_Color * intensity, 1.0);
//    gl_FragColor = vec4(p_Color * intensity, 1.0);
//    gl_FragColor = texture2D(Texture0, p_TexCoord) * vec4(p_Color * intensity, 1.0);
//    gl_FragColor = vec4(p_TexCoord, 0.5, 1.0);

}
