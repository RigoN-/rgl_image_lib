#include "rglTga.h"



int rglLoadTgaHeader(FILE *fp, TGAInfoHeader_t *TgaHeader)
{	
	if (!fread(&TgaHeader->idlength, 1, 1, fp))
		printf("Error reading TGA image header from file .\n");
	if (!fread(&TgaHeader-> colourmaptype, 1, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	if (!fread(&TgaHeader->datatypecode, 1, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	if (!fread(&TgaHeader->colourmaporigin, 2, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	if (!fread(&TgaHeader->colourmaplength, 2, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	if (!fread(&TgaHeader->colourmapdepth, 1, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	if (!fread(&TgaHeader->x_origin, 2, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	if (!fread(&TgaHeader->y_origin, 2, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	if (!fread(&TgaHeader->width, 2, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	if (!fread(&TgaHeader->height, 2, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	
	if (!fread(&TgaHeader->bitsperpixel, 1, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	if (!fread(&TgaHeader->imagedescriptor, 1, 1, fp))
		printf("Error reading TGA image header data from file .\n");
	
	return 0;
}

int rglPrintTgaInfo(TGAInfoHeader_t *TgaHeader)
{
	printf("idlength - \t %i\n", TgaHeader->idlength);
	printf("colourmaptype - \t %i \n", TgaHeader-> colourmaptype);
	printf("datatypecode - \t %i \n", TgaHeader->datatypecode);
	
	printf("colourmaporigin - \t %i \n", TgaHeader->colourmaporigin);
	printf("colourmaplength - \t %i \n", TgaHeader->colourmaplength);
	printf("colourmapdepth - \t %i \n", TgaHeader->colourmapdepth);
	printf("x_origin - \t %i \n", TgaHeader->x_origin);
	printf("y_origin - \t %i \n", TgaHeader->y_origin);
	printf("width - \t %i \n", TgaHeader->width);
	printf("height -\t %i \n", TgaHeader->height);
	
	printf("bitsperpixel - \t %i \n", TgaHeader->bitsperpixel);
	printf("imagedescriptor - \t %i \n", TgaHeader->imagedescriptor);
 return 0;
}

int rglReadTga32bpp(FILE *fp, TGAInfoHeader_t *TgaHeader, rgl_Image_t *img)
{
	img->width=TgaHeader->width;
    img->height=TgaHeader->height;
    img->imgBpp=TgaHeader->bitsperpixel;
	img->imgType=RGL_BGRA;
	unsigned long  size = img->width * img->height * img->imgBpp/8;
	
	img->data = (unsigned char *) malloc(size);
	 if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	

  if ((fread(img->data, size, 1, fp)) != 1) {
		printf("Error reading image data from .\n");
		return 2;
    }
	printf("read image data tga32bit\t OK \n");
	return 0;
}

int rglReadTga24bpp(FILE *fp, TGAInfoHeader_t *TgaHeader, rgl_Image_t *img)
{
	img->width=TgaHeader->width;
    img->height=TgaHeader->height;
    img->imgBpp=TgaHeader->bitsperpixel;
	img->imgType=RGL_BGR;
	unsigned long  size = img->width * img->height * img->imgBpp/8;
	
	img->data = (unsigned char *) malloc(size);
	 if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	

  if ((fread(img->data, size, 1, fp)) != 1) {
		printf("Error reading image data from .\n");
		return 2;
    }
	printf("read image data tga24bit\t OK \n");
	return 0;
}

int rglReadTga16bpp(FILE *fp, TGAInfoHeader_t *TgaHeader, rgl_Image_t *img)
{
	unsigned short int pixcel;
	unsigned short int rMask = 0x7C00;
	unsigned short int gMask = 0x03E0;
	unsigned short int bMask = 0x001F;
	long int i,j;	
	img->width=TgaHeader->width;
    img->height=TgaHeader->height;
    img->imgBpp=TgaHeader->bitsperpixel;
	img->imgType=RGL_RGB;
	unsigned long  size = img->width * img->height * 3;
	
	img->data = (unsigned char *) malloc(size);
	 if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	
	j=0;
	for (i=0;i<(img->width * img->height);i+=1) {		
		if (fread(&pixcel, 2, 1, fp) != 1) {
			printf("Error reading image data from .\n");
			return 2;
		}
		img->data[j]= (unsigned char) ((pixcel&rMask)>>10) * 8; 
		img->data[j+1]= (unsigned char) ((pixcel&gMask)>>5) * 8;
		img->data[j+2]= (unsigned char) ((pixcel&bMask)) *8;
		j+=3;
		//printf("i- %li %i \t %i \t %i \n",i,img->data[j],img->data[j+1],img->data[j+2]);
	}	
	printf("read image data tga16bit\t OK \n");
	return 0;
}


int rglReadTga32bppRle(FILE *fp, TGAInfoHeader_t *TgaHeader, rgl_Image_t *img)
{
	unsigned char packet_header;
	int size_packet;
	unsigned char rgba[4];
	long int i,byte_count;
	
	img->width=TgaHeader->width;
    img->height=TgaHeader->height;
    img->imgBpp=TgaHeader->bitsperpixel;
	img->imgType=RGL_BGRA;
	unsigned long  size = img->width * img->height * img->imgBpp/8;
	
	img->data = (unsigned char *) malloc(size);
	 if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	
	byte_count=0;		
	while (byte_count<size) {
		if (!fread(&packet_header, 1, 1, fp))
			printf("Error reading TGA image data from file .\n");;
		size_packet = 1 + (packet_header & 0x7f);
		if (packet_header & 0x80) {
			if (!fread(&rgba[0], 4, 1, fp))
				printf("Error reading TGA image data from file .\n");
			for (i = 0; i < size_packet; i +=1)  {
				img->data[byte_count] = rgba[0];
				img->data[byte_count+1] = rgba[1];
				img->data[byte_count+2] = rgba[2];
				img->data[byte_count+3] = rgba[3];
				byte_count+=4;
				if  (byte_count==size) break;
			}
		}
		 else {
			for (i = 0; i < size_packet; i += 1) {
				if (!fread(&rgba[0], 4, 1, fp))
					printf("Error reading TGA image data from file .\n");;
				img->data[byte_count] = rgba[0];
				img->data[byte_count+1] = rgba[1];
				img->data[byte_count+2] = rgba[2];
				img->data[byte_count+3] = rgba[3];
				byte_count+=4;
			    if  (byte_count==size) break;
			}
			
		}
	}	
	return 0;
}

int rglReadTga24bppRle(FILE *fp, TGAInfoHeader_t *TgaHeader, rgl_Image_t *img)
{
	unsigned char packet_header;
	int size_packet;
	unsigned char rgba[3];
	long int i,byte_count;
	
	img->width=TgaHeader->width;
    img->height=TgaHeader->height;
    img->imgBpp=TgaHeader->bitsperpixel;
	img->imgType=RGL_BGR;
	unsigned long  size = img->width * img->height * 3;
	
	img->data = (unsigned char *) malloc(size);
	 if (img->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 1;	
    }
	
	byte_count=0;		
	while (byte_count<size) {
		if (!fread(&packet_header, 1, 1, fp))
			printf("Error reading TGA image data from file .\n");;
		size_packet = 1 + (packet_header & 0x7f);
		if (packet_header & 0x80) {
			if (!fread(&rgba[0], 3, 1, fp))
				printf("Error reading TGA image data from file .\n");;
			for (i = 0; i < size_packet; i +=1)  {
				img->data[byte_count] = rgba[0];
				img->data[byte_count+1] = rgba[1];
				img->data[byte_count+2] = rgba[2];				
				byte_count+=3;
				if  (byte_count==size) break;
			}
		}
		 else {
			for (i = 0; i < size_packet; i += 1) {
				if (!fread(&rgba[0], 3, 1, fp))
					printf("Error reading TGA image data from file .\n");;
				img->data[byte_count] = rgba[0];
				img->data[byte_count+1] = rgba[1];
				img->data[byte_count+2] = rgba[2];				
				byte_count+=3;
			    if  (byte_count==size) break;
			}
			
		}
	}	
	return 0;
}




int rglLoadTGA(const char *filename,  rgl_Image_t *img)
{
    FILE *file;
    TGAInfoHeader_t tga;
	
	
	if ((file = fopen(filename, "rb"))==NULL) {
		printf("ERROR File Not Found : %s\n",filename);
		return 1;
		}
	if (rglLoadTgaHeader(file, &tga) == 0) {
		printf("Read tga file header %s. - is OK \n", filename);		
	}
	else {
		return 2;
	}
	/*
	if (rglLog == 1)
		rglPrintTgaInfo(&tga);
	*/
	if (tga.datatypecode==2) {
		if (tga.bitsperpixel==32) {		
			rglReadTga32bpp(file,&tga,img);
		}
		if (tga.bitsperpixel==24) {		
			rglReadTga24bpp(file,&tga,img);
		}
		if (tga.bitsperpixel==16) {		
			rglReadTga16bpp(file,&tga,img);
		}
	}
	if (tga.datatypecode==10) {
		if (tga.bitsperpixel==32) {		
			rglReadTga32bppRle(file,&tga,img);
		}
		if (tga.bitsperpixel==24) {		
			rglReadTga24bppRle(file,&tga,img);
		}
		if (tga.bitsperpixel==16) {		
			//rglReadTga16bpp(file,&tga,img);
		}
	}

	fclose(file);
	
	return 0;
}
