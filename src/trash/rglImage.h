#ifndef _RGLIMAGE_LIB_H
#define _RGLIMAGE_LIB_H
#include "rglImage_t.h"

#define TGA_RGB		 2
#define TGA_A		 3
#define TGA_RLE		10

// texture compressed type
#define txNoCompressed 0
#define txS3TC_DXT1 1
#define txS3TC_DXT3 2
#define txS3TC_DXT5 3
#define txGL_COMPRESS 4


struct RGL_TextureImage {
    int width;
    int height;
    int imgChanal;
    unsigned char *data;
} ;
typedef struct RGL_TextureImage RGL_TextureImage;

struct RGL_DDSTextureImage {
    int width;
    int height;
    int imgChanal;
	int mipCount;
	int CompressType;
    unsigned char *data;
} ;
typedef struct RGL_DDSTextureImage RGL_DDSTextureImage;


int rglLoadBMP(char *filename, RGL_TextureImage *image);
int rglLoadTGA(char *filename, RGL_TextureImage *image);
int rglLoadJPG(char *filename, RGL_TextureImage *image);
int rglLoadPNG (char *filename, RGL_TextureImage *image);
int rglLoadDDS (char *filename, RGL_DDSTextureImage *image);
//  LoadImageSDL - TGA, BMP, PNM( Portable Anymap (.pnm)), XPM(X11 Pixmap (.xpm)), XCF(GIMP native (.xcf)), GIF, PCX, 
int rglLoadImageSDL(char *filename, RGL_TextureImage *image);

#endif
