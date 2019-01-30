#ifndef _RGLIMAGE_LIB_H
#define _RGLIMAGE_LIB_H
#include "rglImage_t.h"


int rglLoadBMP(const char *filename, rgl_Image_t *img);
int rglLoadJPG(char *filename, rgl_Image_t *image);
int rglLoadPNG (char *filename, rgl_Image_t *image);
int rglLoadPNG2 (char *filename, rgl_Image_t *image);
int loadPngImage(char *name, rgl_Image_t *image) ;
int rglLoadTGA(const char *filename, rgl_Image_t *img);

int rglLoadImage(char *filename,rgl_Image_t *image);

#endif
