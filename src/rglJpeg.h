#ifndef _RGL_JPEG_H
#define _RGL_JPEG_H
#include "rglImage_t.h"
#include <setjmp.h>
#include <jpeglib.h> 


int rglLoadJPG(char *filename, rgl_Image_t *image);



#endif
