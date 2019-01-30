#ifndef _RGL_BMP_H
#define _RGL_BMP_H
#include "rglImage_t.h"

// WORD -	typedef unsigned short WORD;
// DWORD -	typedef unsigned long DWORD
// UINT -	typedef unsigned int UINT;
//
enum Bitmap_info_t {RGLCOREINFO=12,RGLINFOHEADER=40, RGLV4HEADER=108, RGLV5HEADER=124};
enum Bitmap_os_t  {BMP_WIN, BMP_OS2};
enum Bitmap_compression_t { BI_RGB=0,BI_RLE8=1,BI_RLE4= 2,BI_BITFIELDS= 3};
/*
    // RLE byte type constants 
    RLE_COMMAND     = 0,
    RLE_ENDOFLINE   = 0,
    RLE_ENDOFBITMAP = 1,
    RLE_DELTA       = 2,
  };
 */
 
struct BitmapFileHeader {	
	unsigned short	bfType;		
	unsigned long	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned long	bfOffBits;	
};
typedef struct BitmapFileHeader  BitmapFileHeader_t;

struct BMPInfoHeader {
	unsigned long	bcSize;	//тип заголовка инфо core = 12 v3 = 40 v4 = 108 v5 = 124
	long			bcWidth;	
	long			bcHeight;	
	unsigned short	bcPlanes;	
	unsigned short 	bcBitCount;	
	unsigned long  	biCompression;	
	unsigned long  	biSizeImage;	
	long			biXPelsPerMeter;
	long  			biYPelsPerMeter;	
	unsigned long   biClrUsed;
	unsigned long   biClrImportant;
	unsigned long	bv4RedMask;
	unsigned long	bv4GreenMask;
	unsigned long	bv4BlueMask;
	unsigned long	bv4AlphaMask;
	unsigned long	bv4CSType;
	long			bv45Endpoints[9];
	unsigned long	bv4GammaRed;
	unsigned long	bv4GammaGreen;
	unsigned long	bv4GammaBlue;
	unsigned long	bV5Intent;
	unsigned long	bV5ProfileData;
	unsigned long	bV5ProfileSize;
	unsigned long	bV5Reserved;		
};
typedef struct BMPInfoHeader BMPInfoHeader_t;


struct rglBitmap {
	BitmapFileHeader_t	bfHeader;
	unsigned long		binfo;
	BMPInfoHeader_t		biHeader;		
};
typedef struct rglBitmap rglBitmap_t;




int rglLoadBmpHeader(FILE *fp, BitmapFileHeader_t *fHeader, BMPInfoHeader_t *iHeader);
int rglIsVolidBmp(BitmapFileHeader_t bfHeader, BMPInfoHeader_t biHeader);
int rglPrintBmpInfo(rglBitmap_t *bmp);
int rglReadBmp24bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);
int rglReadBmp32bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);
int rglReadBmp16bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);
int rglReadBmp8bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);
int rglReadBmp4bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);
int rglReadBmp8bppRle(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);
int rglReadBmp4bppRle(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img);

int rglLoadBMP(const char *filename, rgl_Image_t *img);



#endif
