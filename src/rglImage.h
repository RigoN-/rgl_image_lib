#ifndef _RGL_IMAGE_H
#define _RGL_IMAGE_H

#include "rglImage_t.h"
#include "rglBmp.h"
#include "rglTga.h"
#include "rglPng.h"
#include "rglJpeg.h"

int rglLoadImage(char *filename,rgl_Image_t *image);

#endif
