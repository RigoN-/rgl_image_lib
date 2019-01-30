#ifndef _RGL_PNG_H
#define _RGL_PNG_H
#include "rglImage_t.h"
#include <setjmp.h>
#include <png.h>


int rglLoadPNG (char *filename, rgl_Image_t *image);
int rglLoadPNG2 (char *filename, rgl_Image_t *image);
int loadPngImage(char *name, rgl_Image_t *image) ;


#endif
