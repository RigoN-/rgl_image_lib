#include "rglImage.h"




/////////////////////////////////////////////////////////////////////////////////////////////////
//										TEXTURE LOADER
/////////////////////////////////////////////////////////////////////////////////////////////////
int rglLoadImage(char *filename,rgl_Image_t *image)
{   
    int result=1;	
	if(!filename)  return 1;
	
	if(strstr(filename, ".bmp") || strstr(filename, ".BMP")) {
			result=rglLoadBMP(filename, image);			
		}	
		
	if(strstr(filename, ".tga") || strstr(filename, ".TGA")) {
			result=rglLoadTGA(filename, image);			
		}
	if(strstr(filename, ".png") || strstr(filename, ".PNG")) {
			result=rglLoadPNG(filename, image);
			//loadPngImage(filename, image);
			//rglLoadPNG2(filename, image);			
		}
	if(strstr(filename, ".jpg") || strstr(filename, ".JPG") || strstr(filename, ".jpeg") || strstr(filename, ".JPEG")) {
			result=rglLoadJPG(filename, image);			
		}
	if (result == 0)
	   printf("Load image from %s \t - OK \n", filename);
   else
	   printf("ERROR Load image from %s  \n", filename);
		
  	return result;
}

