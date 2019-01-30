#include "rglBmp.h"


int rglLoadBmpHeader(FILE *fp, BitmapFileHeader_t *fHeader, BMPInfoHeader_t *iHeader)
{	
	if (!fread(&fHeader->bfType, 2, 1, fp))
		printf("Error reading BMP image header from file .\n");
	if (!fread(&fHeader->bfSize, 4, 1, fp))
		printf("Error reading BMP image header from file .\n");
	if (!fread(&fHeader->bfReserved1, 2, 1, fp))
		printf("Error reading BMP image header from file .\n");
	if (!fread(&fHeader->bfReserved2, 2, 1, fp))
		printf("Error reading BMP image header from file .\n");
	if (!fread(&fHeader->bfOffBits, 4, 1, fp))
		printf("Error reading BMP image header from file .\n");
	if (!fread(&iHeader->bcSize, 4, 1, fp))		
		printf("Error reading BMP image header from file .\n");
	if (!fread(&iHeader->bcWidth, 4, 1, fp))	
		printf("Error reading BMP image header from file .\n");
	if (!fread(&iHeader->bcHeight, 4, 1, fp))
		printf("Error reading BMP image header from file .\n");
	if (!fread(&iHeader->bcPlanes, 2, 1, fp))
		printf("Error reading BMP image header from file .\n");
	if (!fread(&iHeader->bcBitCount, 2, 1, fp))
		printf("Error reading BMP image header from file .\n");
	
	if ((iHeader->bcSize>12) && (iHeader->bcSize==40)) {
		if (!fread(&iHeader->biCompression, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->biSizeImage, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->biXPelsPerMeter, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->biYPelsPerMeter, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->biClrUsed, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->biClrImportant, 4, 1, fp))		
			printf("Error reading BMP image header from file .\n");
	}
	if ((iHeader->bcSize>40) && (iHeader->bcSize==108)) {
		if (!fread(&iHeader->bv4RedMask, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bv4GreenMask, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bv4BlueMask, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bv4AlphaMask, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bv4CSType, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bv45Endpoints, 36, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bv4GammaRed, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bv4GammaGreen, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bv4GammaBlue, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
	}
	if ((iHeader->bcSize>108) && (iHeader->bcSize==124)) {
		if (!fread(&iHeader->bV5Intent, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bV5ProfileData, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bV5ProfileSize, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
		if (!fread(&iHeader->bV5Reserved, 4, 1, fp))
			printf("Error reading BMP image header from file .\n");
	}	
	return 0;
}

int rglIsVolidBmp(BitmapFileHeader_t bfHeader, BMPInfoHeader_t biHeader)
{
	if(bfHeader.bfType!=0x4D42){
		printf("Is not volid bfType \n");
		return 1;
	}
	if((biHeader.bcSize!=12) && (biHeader.bcSize!=40) && (biHeader.bcSize!=108)  && (biHeader.bcSize!=124) ){
		printf("Is not volid bfSize \n");
		return 2;
	}
	if((biHeader.bcWidth<1) || (biHeader.bcHeight<1) ){
		printf("Is not volid bcWidth or bcHeight \n");
		return 3;
	}
	if((biHeader.bcPlanes!=1)  ){
		printf("Is not volid bcPlanes \n");
		return 4;
	}
	if((biHeader.biCompression!=0) && (biHeader.biCompression!=1) && (biHeader.biCompression!=2)  &&
		(biHeader.biCompression!=3) ){
		printf("Is not volid biCompression \n");
		return 5;
	}
	if((biHeader.biCompression==3) && (biHeader.bcBitCount!=16) && (biHeader.bcBitCount!=32) ){
		printf("Is not volid biCompression \n");
		return 6;
	}
	if((biHeader.bcBitCount!=1) && (biHeader.bcBitCount!=4) && (biHeader.bcBitCount!=8)  &&
		(biHeader.bcBitCount!=16)  && (biHeader.bcBitCount!=24) && (biHeader.bcBitCount!=32)){
		printf("Is not volid biCompression \n");
		return 5;
	}
	
	return 0;
}

int rglReadBmp24bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img)
{
	img->width=bmp->biHeader.bcWidth;
    img->height=bmp->biHeader.bcHeight;
    img->imgBpp=bmp->biHeader.bcBitCount;
	img->imgType=RGL_BGR;
	unsigned long  size = img->width * img->height * img->imgBpp/8;	
    
	if (ftell(fp)!=bmp->bfHeader.bfOffBits)
		fseek(fp, bmp->bfHeader.bfOffBits, SEEK_SET);
		
	img->data = (unsigned char *) malloc(size);
	
    if (img->data == NULL) {
	printf("Error allocating memory for color-corrected image data");
	return 1;	
    }
	
    if ((fread(img->data, size, 1, fp)) != 1) {
	printf("Error reading BMP image data from .\n");
	return 2;
    }
	printf("Read image data bmp24bit \t - OK \n");
	return 0;
}


int rglReadBmp32bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img)
{
	img->width=bmp->biHeader.bcWidth;
    img->height=bmp->biHeader.bcHeight;
    img->imgBpp=bmp->biHeader.bcBitCount;
	img->imgType=RGL_BGRA;
	unsigned long  size = img->width * img->height * img->imgBpp/8;	
  
	if (ftell(fp)!=bmp->bfHeader.bfOffBits)
		fseek(fp, bmp->bfHeader.bfOffBits, SEEK_SET);
	
   
    img->data = (unsigned char *) malloc(size);
    if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	
    if ((fread(img->data, size, 1, fp)) != 1) {
		printf("Error reading BMP image data from .\n");
		return 2;
    }
	printf("read image data bmp32bit\t OK \n");
	return 0;
}

int rglReadBmp16bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img)
{
	unsigned short int pixcel;
	unsigned short int rMask = 0x7C00;
	unsigned short int gMask = 0x03E0;
	unsigned short int bMask = 0x001F;
	long int i,j;
	img->width=bmp->biHeader.bcWidth;
    img->height=bmp->biHeader.bcHeight;
    img->imgBpp=bmp->biHeader.bcBitCount;
	img->imgType=RGL_RGB;
	unsigned long  size = img->width * img->height * 3;	
   
	if (ftell(fp)!=bmp->bfHeader.bfOffBits)
		fseek(fp, bmp->bfHeader.bfOffBits, SEEK_SET);
	   
    img->data = (unsigned char *) malloc(size);
	
    if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	
	for (i=0;i<(img->width * img->height);i+=1) {		
		if (fread(&pixcel, 2, 1, fp) != 1) {
			printf("Error reading BMP image data from .\n");
			return 2;
		}
		j=i*3;
		img->data[j]= (unsigned char) ((pixcel&rMask)>>10) * 8; 
		img->data[j+1]= (unsigned char) ((pixcel&gMask)>>5) * 8;
		img->data[j+2]= (unsigned char) ((pixcel&bMask)) *8;		
	}	
   
	printf("read image data bmp16bit\t OK \n");
	return 0;
}


int rglReadBmp8bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img)
{
	unsigned char pixcel;
	long int i,byte_count;
	img->width=bmp->biHeader.bcWidth;
    img->height=bmp->biHeader.bcHeight;
    img->imgBpp=bmp->biHeader.bcBitCount;
	img->imgType=RGL_RGB;
	unsigned long  size = img->width * img->height * 3;	
    
	if (ftell(fp)!=bmp->bfHeader.bfOffBits)
		fseek(fp, bmp->bfHeader.bfOffBits, SEEK_SET);
	
    
    img->data = (unsigned char *) malloc(size);
    if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	byte_count=0;
	for (i=0;i<(img->width * img->height);i+=1) {
		if (fread(&pixcel, 1, 1, fp) != 1) {
			printf("Error reading BMP image data from .\n");
			return 2;
		}		
		img->data[byte_count]= pixcel; 
		img->data[byte_count+1]= pixcel;
		img->data[byte_count+2]= pixcel;
		byte_count+=3;		
	}	
   
	printf("read image data bmp8bit\t OK \n");
	return 0;
}


int rglReadBmp4bpp(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img)
{
	unsigned char pixcel, color1, color2;
	long int i,byte_count;
	img->width=bmp->biHeader.bcWidth;
    img->height=bmp->biHeader.bcHeight;
    img->imgBpp=bmp->biHeader.bcBitCount;
	img->imgType=RGL_RGB;
	unsigned long  size = img->width * img->height * 3;	
   
	if (ftell(fp)!=bmp->bfHeader.bfOffBits)
		fseek(fp, bmp->bfHeader.bfOffBits, SEEK_SET);	
   
    img->data = (unsigned char *) malloc(size);
    if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	byte_count=0;
	for (i=0;i<(img->width * img->height);i+=2) {
		if (fread(&pixcel, 1, 1, fp) != 1) {
			printf("Error reading BMP image data from .\n");
			return 2;
		}			
		color1=(pixcel >> 4);
		img->data[byte_count]= color1*16; 
		img->data[byte_count+1]=color1*16;
		img->data[byte_count+2]= color1*16;
		color2=(pixcel & 0x0F);
		img->data[byte_count+3]= color2*16; 
		img->data[byte_count+4]= color2*16;
		img->data[byte_count+5]= color2*16;
		byte_count+=6;		
	}   
	printf("read image data bmp 4 bit\t OK \n");
	return 0;
}



int rglReadBmp8bppRle(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img)
{
	unsigned char byte1, byte2,pixcel;
	long int i,byte_count;
	img->width=bmp->biHeader.bcWidth;
    img->height=bmp->biHeader.bcHeight;
    img->imgBpp=bmp->biHeader.bcBitCount;
	img->imgType=RGL_RGB;
	unsigned long  size = img->width * img->height * 3;	
    
	if (ftell(fp)!=bmp->bfHeader.bfOffBits)
		fseek(fp, bmp->bfHeader.bfOffBits, SEEK_SET);
	
    img->data = (unsigned char *) malloc(size);
    if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	byte_count=0;	
	if (!fread(&byte1, 1, 1, fp))
		printf("1 Error reading RLE BMP 8 bpp image data from file .\n");
	if (!fread(&byte2, 1, 1, fp))
		printf("2 Error reading RLE BMP 8 bpp image data from file .\n");
	
	while (byte_count<size) {
		if ((byte1>=1) && (byte2>=1)){
			for (i=0; i<byte1; i+=1) {
				img->data[byte_count]= byte2; 
				img->data[byte_count+1]= byte2;
				img->data[byte_count+2]= byte2;
				byte_count+=3;
				if  (byte_count==size) break;
			}
		}
		
		if ((byte1==0) && (byte2>2)) {
			for (i=0; i<byte2; i+=1) {
				if (!fread(&pixcel, 1, 1, fp))
					printf(" 3 Error reading RLE BMP 8 bpp image data from file .\n");	
				img->data[byte_count]= pixcel; 
				img->data[byte_count+1]= pixcel;
				img->data[byte_count+2]= pixcel;
				byte_count+=3;
				if  (byte_count==size) break;
			}
			if ((byte2 & 1) == 1)
				if (!fread(&pixcel, 1, 1, fp))
					printf("4 Error reading RLE BMP 8 bpp image data from file .\n");
		}
		
		if ((byte1==0) && (byte2==1)) {
			printf("Command RLE - \t %X \n", byte1);
			break;
		}
		
		if ((byte1==0) && (byte2==2)) {
			printf("Command RLE - \t %X \n", byte1);
			if (!fread(&pixcel, 1, 1, fp))
				printf("5 Error reading RLE BMP 8 bpp image data from file .\n");	;
			if (!fread(&pixcel, 1, 1, fp))
				printf("6 Error reading RLE BMP 8 bpp image data from file .\n");	;
			break;
		}		
		if (!fread(&byte1, 1, 1, fp))
			printf(" 7 Error reading RLE BMP 8 bpp image data from file .\n");
		if (!fread(&byte2, 1, 1, fp))
			printf("8 Error reading RLE BMP 8 bpp image data from file .\n");			
	}   
	printf("read image data bmp8bit RLE 8 bpp\t OK \n");
	return 0;
}


int rglReadBmp4bppRle(FILE *fp, rglBitmap_t *bmp, rgl_Image_t *img)
{
	unsigned char byte1, byte2,pixcel, color1, color2;
	long int i,byte_count;
	img->width=bmp->biHeader.bcWidth;
    img->height=bmp->biHeader.bcHeight;
    img->imgBpp=bmp->biHeader.bcBitCount;
	img->imgType=RGL_RGB;
	unsigned long  size = img->width * img->height * 3;	
   	
	if (ftell(fp)!=bmp->bfHeader.bfOffBits)
		fseek(fp, bmp->bfHeader.bfOffBits, SEEK_SET);	
   
    img->data = (unsigned char *) malloc(size);
    if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	byte_count=0;

	if (!fread(&byte1, 1, 1, fp))
		printf("Error reading RLE BMP 4 bpp image data from file .\n");
	if (!fread(&byte2, 1, 1, fp))
		printf("Error reading RLE BMP 4 bpp image data from file .\n");	
	
	while (byte_count<size) {
	
		if ((byte1>=1) && (byte2>=1)){			
			color1=(byte2 >> 4);color2=(byte2 & 0x0F);			
			for (i=0; i<byte1; i+=1) {				
				if ((i % 2) ==0) {					
					img->data[byte_count]= color1*16; 
					img->data[byte_count+1]=color1*16;
					img->data[byte_count+2]= color1*16;
					byte_count+=3;
				}
				else {					
					img->data[byte_count]= color2*16; 
					img->data[byte_count+1]= color2*16;
					img->data[byte_count+2]= color2*16;
					byte_count+=3;
				}
				if  (byte_count==size) break;
			}
		}
		
		if ((byte1==0) && (byte2>2)) {			
			for (i=0; i<byte2; i+=1) {
				if ((i % 2) ==0) {
					if (!fread(&pixcel, 1, 1, fp))
						printf("Error reading RLE BMP 4 bpp image data from file .\n");;		
					color1=(pixcel >> 4);
					img->data[byte_count]= color1*16; 
					img->data[byte_count+1]=color1*16;
					img->data[byte_count+2]= color1*16;
					byte_count+=3;
				}
				else {
					color2=(pixcel & 0x0F);
					img->data[byte_count]= color2*16; 
					img->data[byte_count+1]= color2*16;
					img->data[byte_count+2]= color2*16;
					byte_count+=3;					
				}
				if  (byte_count==size) break;
			}
			if ((byte2/2 % 2) == 1) 
				if (!fread(&pixcel, 1, 1, fp))
					printf("Error reading RLE BMP 4 bpp image data from file .\n");
		}		
		
		if ((byte1==0) && (byte2==1)) {			
			break;
		}
		
		if ((byte1==0) && (byte2==2)) {			
			if (!fread(&pixcel, 1, 1, fp))
				printf("Error reading RLE BMP 4 bpp image data from file .\n");
			if (!fread(&pixcel, 1, 1, fp))
				printf("Error reading RLE BMP 4 bpp image data from file .\n");
			break;
		}
		
		if (!fread(&byte1, 1, 1, fp))
			printf("Error reading RLE BMP 4 bpp image data from file .\n");
		if (!fread(&byte2, 1, 1, fp))
			printf("Error reading RLE BMP 4 bpp image data from file .\n");;	
	
	}   
	printf("read image data bmp4bit RLE 4 bpp\t OK \n");
	return 0;
}







int rglPrintBmpInfo(rglBitmap_t *bmp)
{
	printf("bfType - \t %c%c %X\n", bmp->bfHeader.bfType, bmp->bfHeader.bfType >> 8, bmp->bfHeader.bfType);
	printf("bfSize - \t %li %li Kbyte\n", bmp->bfHeader.bfSize, bmp->bfHeader.bfSize/1024);
	printf("bfOffBits - \t %li \n", bmp->bfHeader.bfOffBits);
	
	printf("bcSize - \t %li \n", bmp->biHeader.bcSize);
	printf("bcWidth - \t %li \n", bmp->biHeader.bcWidth);
	printf("bcHeight - \t %li \n", bmp->biHeader.bcHeight);
	printf("bcPlanes - \t %d \n", bmp->biHeader.bcPlanes);
	printf("bcBitCount - \t %d \n", bmp->biHeader.bcBitCount);
	printf("biCompression -\t %li \n", bmp->biHeader.biCompression);
	printf("biSizeImage - \t %li \n", bmp->biHeader.biSizeImage);
	return 0;
}


int rglLoadBMP(const char *filename,  rgl_Image_t *img)
{
    FILE *file;
    rglBitmap_t bmp;
	
	
	if ((file = fopen(filename, "rb"))==NULL) {
			printf("ERROR File Not Found : %s\n",filename);
			return 1;
		}
	if (rglLoadBmpHeader(file, &bmp.bfHeader, &bmp.biHeader) == 0) {
		printf("Read bitmap file header %s. - is OK \n", filename);		
	}
	else {
		printf("ERROR Read bitmap file header %s. - is OK \n", filename);	
		return 2;
	}
	/*
	if (rglLog == 1)
		rglPrintBmpInfo(&bmp);
	*/
	if (rglIsVolidBmp(bmp.bfHeader, bmp.biHeader)==0){
	
		switch (bmp.biHeader.bcBitCount)  {
			case 32:
				if((bmp.biHeader.biCompression==0)) {
					//printf("rglReadBmp32bpp \t %s OK \n",filename);
					rglReadBmp32bpp(file, &bmp, img);
				}
				else {
					printf("Value bitmap BitCount and Compression not volid %s.  \n", filename);
				}
				break;
			case 24:
				if((bmp.biHeader.biCompression==0)) {
					//printf("rglReadBmp24bpp \t %s OK \n",filename);
					rglReadBmp24bpp(file, &bmp, img);
				}	
				else {
					printf("Value bitmap BitCount and Compression not volid %s.  \n", filename);
				}
				break;					
			case 16:
				if((bmp.biHeader.biCompression==0)) {
					//printf("rglReadBmp16bpp \t %s OK \n",filename);
					rglReadBmp16bpp(file, &bmp, img);
				}
				else {
					printf("Value bitmap BitCount and Compression not volid %s. \n", filename);
				}
				break;	
			case 8:
				if((bmp.biHeader.biCompression==0)) {
					//printf("rglReadBmp8bpp \t %s OK \n",filename);
					rglReadBmp8bpp(file, &bmp, img);
				}
				else {
					if((bmp.biHeader.biCompression==BI_RLE8)) {
						//printf("rglReadBmp8bppRle \t %s OK \n",filename);
						rglReadBmp8bppRle(file, &bmp, img);
					}
					else {
						printf("Value bitmap BitCount and Compression not volid %s.  \n", filename);
						return 3;
					}
				}
				break;
			case 4:
				if((bmp.biHeader.biCompression==0)) {
					//printf("rglReadBmp4bpp \t %s OK \n",filename);
					rglReadBmp4bpp(file, &bmp, img);
				}
				else {
					if((bmp.biHeader.biCompression==BI_RLE4)) {
						//printf("rglReadBmp4bppRle \t %s OK \n",filename);
						rglReadBmp4bppRle(file, &bmp, img);
					}
					else {
						printf("Value bitmap BitCount and Compression not volid %s.  \n", filename);
					}
				}
				break;
			
			 default: 
				printf("unsupported bitmap BitCount or bad file compression %s.\n", filename);
				break;
		}		
	}
	else {
		printf("Bitmap not volid %s. - \n", filename);
		return 3;
	}
	fclose(file);
	
	return 0;
}
