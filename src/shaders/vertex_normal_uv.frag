#version 140

precision highp float;

uniform vec3 colour;
uniform sampler2D Texture0;

//in vec3 p_Color;
in vec2 p_TexCoord;

//in float diffuse;
in vec3 normal, lightDir;

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
#if 1
    vec3 n;
    float intensity;

    n = normalize(normal);
    intensity = dot(lightDir, n);
    vec2 offx = vec2(0.1, 0.0);
    vec2 offy = vec2(0.0, 0.1);

//    if(intensity>0.70) gl_FragColor = vec4(colour*1.0, 1.0);
//    else if(intensity>0.4) gl_FragColor = vec4(colour*0.5, 1.0);
//    else if(intensity>0.2) gl_FragColor = vec4(colour*0.3, 1.0);
//    else gl_FragColor = vec4(colour*0.2, 1.0);

//    if(intensity>0.70) gl_FragColor = vec4(p_Color*1.0, 1.0);
//    else if(intensity>0.4) gl_FragColor = vec4(p_Color*0.5, 1.0);
//    else if(intensity>0.2) gl_FragColor = vec4(p_Color*0.3, 1.0);
    //else gl_FragColor = vec4(p_Color*0.2, 1.0);

//    gl_FragColor = vec4(colour*intensity, 1.0);
    //gl_FragColor = vec4(p_Color * intensity, 1.0);
//    gl_FragColor = vec4(p_Color * intensity, 1.0);
//    gl_FragColor = texture2D(Texture0, p_TexCoord) * vec4(p_Color * intensity, 1.0);
    vec4 col0 = texture2D(Texture0, p_TexCoord);
    vec4 colpx = texture2D(Texture0, p_TexCoord + offx);
    vec4 colmx = texture2D(Texture0, p_TexCoord - offx);
    vec4 colpy = texture2D(Texture0, p_TexCoord + offy);
    vec4 colmy = texture2D(Texture0, p_TexCoord - offy);

    gl_FragColor = (col0*4 + colpx + colmx + colpy + colmy)/8;

//    gl_FragColor = texture2D(Texture0, p_TexCoord);
#endif

#if 0
    //float intens = ex_Color.r;

    if(diffuse>0.70) gl_FragColor = vec4(colour*1.0, 1.0);
    else if(diffuse>0.4) gl_FragColor = vec4(colour*0.5, 1.0);
    else if(diffuse>0.2) gl_FragColor = vec4(colour*0.3, 1.0);
    else gl_FragColor = vec4(colour*0.2, 1.0);

  //  if(age<0.8) gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

//    gl_FragColor = vec4(ex_Color, 1.0);
#endif

#if 0
    gl_FragColor = vec4(colour * diffuse, 1.0);
#endif

#if 0
//	gl_FragColor = vec4(sin(ex_Color.r*5)/2 + 0.5, sin(ex_Color.g/1)/2 + 0.5, sin(ex_Color.b/1)/2 + 0.5, 1.0);
	float off1 = ex_Color.r - 0.5;
	float off2 = ex_Color.g- 0.5;
	float off3 = off1*off1 + off2*off2;
	if(off3>0.2) {if (off3>0.22) off3 = 1.0; else off3 = 0.0;} else off3 = 0.4;

//	gl_FragColor = vec4(off3, off3, off3, 1.0)* vec4(diffuse, diffuse, diffuse, 1.0);;
	gl_FragColor = vec4(off3, off3, off3, 1.0);
#endif
#if 0
//	gl_FragColor = vec4(sin(ex_Color.r*5)/2 + 0.5, sin(ex_Color.g/1)/2 + 0.5, sin(ex_Color.b/1)/2 + 0.5, 1.0);
	float off1 = ex_Color.r - 0.0;
	float off2 = ex_Color.g - 0.0;
	float off3 = sin((30/pow(off2, off1))*sqrt(off1*off1 + off2*off2));
//	if(off3>0.2) {if (off3>0.22) off3 = 1.0; else off3 = 0.0;} else off3 = 0.4;

//	gl_FragColor = vec4(off3, off3, off3, 1.0)* vec4(diffuse, diffuse, diffuse, 1.0);;
	gl_FragColor = vec4(off3, off3, off3, 1.0);
#endif

#if 0
	float off1;
	float off2;

	vec3 Cbrick = vec3(0.5, 0.15, 0.14);
	vec3 Cmortar = vec3(0.5, 0.5, 0.5);

	off1 = ex_Color.r/BMWIDTH;
	off2 = ex_Color.g/BMHEIGHT;

	if(mod(off2*0.5, 1) > 0.5) off1 += 0.5;

	float tof1 = floor(off1);
//	off2 += 0.5 * (noise(tof1 + 0.5) - 0.5);
	float tof2 = floor(off2);
	off1 -= tof1;
	off2 -= tof2;
	float w = step(MWF, off1) - step(1-MWF, off1);
	float h = step(MHF, off2) - step(1-MHF, off2);
	vec3 Ct = mix(Cmortar, Cbrick, w*h);

	gl_FragColor = vec4(Ct, 1.0);

#endif
}
