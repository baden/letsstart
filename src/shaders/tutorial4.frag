#version 150

precision highp float;

uniform vec3 colour;

//in  vec3 ex_Color;

out vec4 gl_FragColor;
in float diffuse;
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
