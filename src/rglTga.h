#ifndef _RGL_TGA_H
#define _RGL_TGA_H
#include "rglImage_t.h"

struct  TGAInfoHeader{
   unsigned char  idlength;
   unsigned char  colourmaptype;
   unsigned char  datatypecode;
   short int colourmaporigin;
   short int colourmaplength;
   unsigned char  colourmapdepth;
   short int x_origin;
   short int y_origin;
   short int width;
   short int height;
   unsigned char  bitsperpixel;
   unsigned char  imagedescriptor;
} ;
typedef struct TGAInfoHeader TGAInfoHeader_t;



int rglLoadTgaHeader(FILE *fp, TGAInfoHeader_t *TgaHeader);
//int rglIsVolidBmp(BitmapFileHeader_t bfHeader, InfoHeader_t biHeader);
int rglPrintTgaInfo(TGAInfoHeader_t *TgaHeader);
int rglReadTga24bpp(FILE *fp, TGAInfoHeader_t *TgaHeader, rgl_Image_t *img);
int rglReadTga32bpp(FILE *fp, TGAInfoHeader_t *TgaHeader, rgl_Image_t *img);
int rglReadTga16bpp(FILE *fp, TGAInfoHeader_t *TgaHeader, rgl_Image_t *img);
int rglReadTga32bppRle(FILE *fp, TGAInfoHeader_t *TgaHeader, rgl_Image_t *img);
int rglReadTga24bppRle(FILE *fp, TGAInfoHeader_t *TgaHeader, rgl_Image_t *img);
//int rglReadBmp8bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);
//int rglReadBmp4bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);
//int rglReadBmp8bppRle(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);
//int rglReadBmp4bppRle(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);

int rglLoadTGA(const char *filename, rgl_Image_t *img);



#endif