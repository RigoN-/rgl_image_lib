#ifndef _RGL_IMAGE_T_H
#define _RGL_IMAGE_T_H
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <GL/gl.h>

#define RGL_BGR		0x80E0
#define RGL_BGRA	0x80E1
#define RGL_RGB		0x1907
#define RGL_RGBA	0x1908

struct rgl_Image {
    int width;
    int height;
    int imgBpp;
	int imgType;
    unsigned char *data;
} ;
typedef struct rgl_Image rgl_Image_t;



#endif
