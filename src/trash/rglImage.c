#include "rglImage.h" 



int rglLoadBMP(char *filename, RGL_TextureImage *image)
{
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
 //   char temp;                          // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL)
    {
	printf("File Not Found : %s\n",filename);
	return 0;
    }
    
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);
    // read the width
    if ((i = fread(&image->width, 4, 1, file)) != 1) {
	printf("Error reading width from %s.\n", filename);
	return 0;
    }
      // read the height 
    if ((i = fread(&image->height, 4, 1, file)) != 1) {
	printf("Error reading height from %s.\n", filename);
	return 0;
    }
   
    
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
  
    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
	printf("Error reading planes from %s.\n", filename);
	return 0;
    }
    if (planes != 1) {
	printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
	printf("Error reading bpp from %s.\n", filename);
	return 0;
    }
    if (bpp != 24) {
	printf("Bpp from %s is not 24: %u\n", filename, bpp);
	return 0;
    }
     image->imgChanal=bpp/8;
     
      size = image->width * image->height * bpp/8;	
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data. 
    image->data = (unsigned char *) malloc(size);
    if (image->data == NULL) {
	printf("Error allocating memory for color-corrected image data");
	return 0;	
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
	printf("Error reading image data from %s.\n", filename);
	return 0;
    }

printf("Image BMP load OK %s \n", filename);
    return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int rglLoadTGA(char *filename, RGL_TextureImage *image)
 {
	unsigned char rep,*ptr,info[18];
	int i,j,k,l,components,size;     
	FILE *file = fopen(filename,"rb");
	
	if(!file) {
		printf("Don't open file : %s", filename);
		return 0;
	}
	
	if (fread(&info,1,18,file) != 18)
	{
	printf("Error read header image from %s.\n", filename);
	return 0;
    }
      
	switch(info[16]) {
		case 32: components = 4; break;
		case 24: components = 3; break;
		default: fclose(file); return 0;
	}
	
		image->imgChanal=components;
		image -> width = info[12] + info[13] * 256;
		image -> height = info[14] + info[15] * 256;
		size = image -> width * image -> height * components;
		   
		image -> data = (unsigned char*) malloc(size);
 
		fseek(file,info[0],SEEK_CUR);     
	switch(info[2]) {
		case 2:
			if (fread(image -> data,1,size,file) !=size)
				{
					printf("Error read data image from %s.\n", filename);
					return 0;
				} 
		break;
        case 10:
			i = 0;
			ptr = image -> data;
			while(i < size) {
				if (fread(&rep,1,1,file) != 1)
					{
					printf("Error read data image from %s.\n", filename);
					return 0;
					} 
				if(rep & 0x80) {
					rep ^= 0x80;
					if (fread(ptr,1,components,file) !=components)
						{
							printf("Error read data image from %s.\n", filename);
							return 0;
						} 
					ptr += components;
					for(j = 0; j < rep * components; j++) {
						*ptr = *(ptr - components);
						ptr ++;
					}
					i += components * (rep + 1);
				} else {
					k = components * (rep + 1);
					if (fread(ptr,1,k,file)  !=k )
						{
							printf("Error read data image from %s.\n", filename);
							return 0;
						} 
					ptr += k;
					i += k;
				}
			}
			break;
		default:
			fclose(file);
			free(image -> data);
			return 0;
	}
	if((info[17] & 0x20)!=0)
		for(j = 0, k = image -> width * components; j < image -> height / 2; j ++)
			for(i = 0; i < image -> width * components; i ++) {
				l = image -> data[j * k + i];
				image -> data[j * k + i] = image -> data[(image -> height - j - 1) * k + i];
				image -> data[(image -> height - j - 1) * k + i] = l;
			}
      
	fclose(file);
	printf("Image TGA load OK %s \n", filename);
	return 1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

static void my_error_exit(j_common_ptr cinfo) {
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer,1);
}

int rglLoadJPG(char *filename, RGL_TextureImage *image) {

struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	FILE * infile;
	JSAMPARRAY buffer;
	int row_stride;
	int cont;


	if(!(infile = fopen(filename,"rb"))) return 0;
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if(setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return 0;
	}
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo,TRUE);
	jpeg_start_decompress(&cinfo);
	row_stride = cinfo.output_width * cinfo.num_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo,JPOOL_IMAGE,row_stride,1);

	 image ->width =cinfo.image_width;
     image ->height = cinfo.image_height;
     image->imgChanal = cinfo.num_components;
     image -> data = (unsigned char *) malloc(image ->width *  image ->height * cinfo.num_components);
     cont = cinfo.output_height-1;
	while(cinfo.output_scanline < cinfo.output_height)
	 {
		jpeg_read_scanlines(&cinfo,buffer,1);
		memcpy(image -> data + cinfo.image_width * cinfo.num_components * cont,buffer[0],row_stride);
 		cont--; 		
		}	
		fclose(infile);  
	jpeg_finish_decompress(&cinfo);
	
	jpeg_destroy_decompress(&cinfo);

	
	printf("Image JPG load OK %s \n", filename);
    return 1;
   
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void rglGetPNGColorInfo (int color_type, RGL_TextureImage *image)
{
  switch (color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
    	 image->imgChanal = 1;
      break;

    case PNG_COLOR_TYPE_GRAY_ALPHA:
       image->imgChanal  = 2;
      break;

    case PNG_COLOR_TYPE_RGB:
      image->imgChanal  = 3;
      break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
    	image->imgChanal  = 4;
      break;

    default:
      /* Badness */
      break;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int rglLoadPNG (char *filename, RGL_TextureImage *image)
{
  png_byte magic[8];
  png_structp png_ptr;
  png_infop info_ptr;
  int bit_depth, color_type;
  FILE *fp = NULL;
  png_bytep *row_pointers = NULL;
  png_uint_32 w, h;
  int i;

  /* Open image file */
  fp = fopen (filename, "rb");
  if (!fp)
    {
      fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
      return 0;
    }

  /* Read magic number */
 // fread (magic, 1, sizeof (magic), fp);
  
  if ( !fread (magic, 1, sizeof (magic), fp))
	{
	printf("Error read number from %s.\n", filename);
	return 0;
    }

  /* Check for valid magic number */
  if (!png_check_sig (magic, sizeof (magic)))
    {
      fprintf (stderr, "error: \"%s\" is not a valid PNG image!\n",
	       filename);
      fclose (fp);
      return 0;
    }

  /* Create a png read struct */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
    {
      fclose (fp);
      return 0;
    }

  /* Create a png info struct */
  info_ptr = png_create_info_struct (png_ptr);
  if (!info_ptr)
    {
      fclose (fp);
      png_destroy_read_struct (&png_ptr, NULL, NULL);
      return 0;
    }

  /* Create our OpenGL texture object */


  /* Initialize the setjmp for returning properly after a libpng
     error occured */
  if (setjmp (png_jmpbuf (png_ptr)))
    {
		  fclose (fp);
		  png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

		  if (row_pointers)
				free (row_pointers);

      return 0;
    }

  /* Setup libpng for using standard C fread() function
     with our FILE pointer */
  png_init_io (png_ptr, fp);

  /* Tell libpng that we have already read the magic number */
  png_set_sig_bytes (png_ptr, sizeof (magic));

  /* Read png info */
  png_read_info (png_ptr, info_ptr);

  /* Get some usefull information from header */
  bit_depth = png_get_bit_depth (png_ptr, info_ptr);
  color_type = png_get_color_type (png_ptr, info_ptr);

  /* Convert index color images to RGB images */
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png_ptr);

  /* Convert 1-2-4 bits grayscale images to 8 bits
     grayscale. */
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_gray_1_2_4_to_8 (png_ptr);

  if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha (png_ptr);

  if (bit_depth == 16)
    png_set_strip_16 (png_ptr);
  else if (bit_depth < 8)
    png_set_packing (png_ptr);

  /* Update info structure to apply transformations */
  png_read_update_info (png_ptr, info_ptr);

  /* Retrieve updated information */
  png_get_IHDR (png_ptr, info_ptr, &w, &h, &bit_depth,&color_type, NULL, NULL, NULL);

  /* We can now allocate memory for storing pixel data */
	 image ->width =w;
     image ->height = h;   
	 /* Get image format and components per pixel */
	 rglGetPNGColorInfo(color_type,image);
     image -> data = (unsigned char *) malloc(image ->width *  image ->height *  image->imgChanal);
	 printf("image ->width = %i\n",  image ->width);
	 printf(" image ->height = %i\n",  image ->height);
	 printf(" image->imgChanal = %i\n",   image->imgChanal);

  /* Setup a pointer array.  Each one points at the begening of a row. */
  row_pointers = (png_bytep *)malloc (sizeof (png_bytep) *  image ->height);

  for (i = 0; i < image ->height; ++i)
    {
      row_pointers[i] = (png_bytep)( image -> data +((image ->height - (i + 1)) * image ->width * image->imgChanal));
    }

  /* Read pixel data using row pointers */
  png_read_image (png_ptr, row_pointers);

  /* Finish decompression and release memory */
  png_read_end (png_ptr, NULL);
  png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

  /* We don't need row pointers anymore */
  free (row_pointers);

  fclose (fp);
  printf("Image PNG load OK %s \n", filename);
  return 1;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DirectDraw's structures 

struct DDPixelFormat
{
  unsigned int size;
  unsigned int flags;
  unsigned int fourCC;
  unsigned int bpp;
  unsigned int redMask;
  unsigned int greenMask;
  unsigned int blueMask;
  unsigned int alphaMask;
};

struct DDSCaps
{
  unsigned int caps;
  unsigned int caps2;
  unsigned int caps3;
  unsigned int caps4;
};

struct DDColorKey
{
  unsigned int lowVal;
  unsigned int highVal;
};

struct DDSurfaceDesc
{
  unsigned int size;
  unsigned int flags;
  unsigned int height;
  unsigned int width;
  unsigned int pitch;
  unsigned int depth;
  unsigned int mipMapLevels;
  unsigned int alphaBitDepth;
  unsigned int reserved;
  unsigned int surface;

  struct DDColorKey ckDestOverlay;
  struct DDColorKey ckDestBlt;
  struct DDColorKey ckSrcOverlay;
  struct DDColorKey ckSrcBlt;

  struct DDPixelFormat format;
  struct DDSCaps caps;

  unsigned int textureStage;
};

#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
  (unsigned int)( \
    (((unsigned int)(unsigned char)(ch3) << 24) & 0xFF000000) | \
    (((unsigned int)(unsigned char)(ch2) << 16) & 0x00FF0000) | \
    (((unsigned int)(unsigned char)(ch1) <<  8) & 0x0000FF00) | \
     ((unsigned int)(unsigned char)(ch0)        & 0x000000FF) )

#define FOURCC_DXT1 MAKEFOURCC('D', 'X', 'T', '1')
#define FOURCC_DXT3 MAKEFOURCC('D', 'X', 'T', '3')
#define FOURCC_DXT5 MAKEFOURCC('D', 'X', 'T', '5')

// texture Id 
//------------unsigned int texId;


int rglLoadDDS (char *filename, RGL_DDSTextureImage *image)
{
  struct DDSurfaceDesc ddsd;
  //struct gl_texture_t *texinfo;
  FILE *fp;
  char magic[4];
  int mipmapFactor;
  long bufferSize, curr, end;

  // Open the file
  fp = fopen (filename, "rb");
  if (!fp)
    {
      fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
      return 0;
    }

  // Read magic number and check if valid .dds file 
  //fread (&magic, sizeof (char), 4, fp);

  if (!fread (&magic, sizeof (char), 4, fp))
    {
      fprintf (stderr, "error: couldn't get magic number and check if valid .dds file \"%s\"!\n", filename);
      return 0;
    }

  if (strncmp (magic, "DDS ", 4) != 0)
    {
      fprintf (stderr, "the file \"%s\" doesn't appear to be"
	       "a valid .dds file!\n", filename);
      fclose (fp);
      return 0;
    }

  // Get the surface descriptor 
  //fread (&ddsd, sizeof (ddsd), 1, fp);

 if (!fread (&ddsd, sizeof (ddsd), 1, fp))
    {
      fprintf (stderr, "error: couldn't Get the surface descriptor \"%s\"!\n", filename);
      return 0;
    }

//--------  texinfo = (struct gl_texture_t *) calloc (sizeof (struct gl_texture_t), 1);
//--------  texinfo->width = ddsd.width;
//--------  texinfo->height = ddsd.height;
//--------  texinfo->numMipmaps = ddsd.mipMapLevels;

	 image ->width =ddsd.width;
     image ->height = ddsd.height;   
	 // Get image format and components per pixel /
	 image ->mipCount=ddsd.mipMapLevels;
   //  image -> data = (unsigned char *) malloc(image ->width *  image ->height *  image->imgChanal);

  switch (ddsd.format.fourCC)
    {
    case FOURCC_DXT1:
      // DXT1's compression ratio is 8:1 
      image->CompressType = txS3TC_DXT1;
      image->imgChanal = 3;
      mipmapFactor = 2;
      break;

    case FOURCC_DXT3:
      // DXT3's compression ratio is 4:1 
       image->CompressType = txS3TC_DXT3;
      image->imgChanal = 4;
      mipmapFactor = 4;
      break;

    case FOURCC_DXT5:
      // DXT5's compression ratio is 4:1 
      image->CompressType = txS3TC_DXT5;
      image->imgChanal = 4;
      mipmapFactor = 4;
      break;

    default:
      // Bad fourCC, unsupported or bad format 
      fprintf (stderr, "the file \"%s\" doesn't appear to be"
	       "compressed using DXT1, DXT3, or DXT5! [%i]\n",
	       filename, ddsd.format.fourCC);
    
      fclose (fp);
      return 0;
    }

  // Calculate pixel data size 
  curr = ftell (fp);
  fseek (fp, 0, SEEK_END);
  end = ftell (fp);
  fseek (fp, curr, SEEK_SET);
  bufferSize = end - curr;

  // Read pixel data with mipmaps 
  image -> data  = (unsigned char *)malloc (bufferSize * sizeof (unsigned char));
 
 // fread ( image -> data, sizeof (unsigned char), bufferSize, fp);
 if (!fread ( image -> data, sizeof (unsigned char), bufferSize, fp))
    {
      fprintf (stderr, "error: couldn't Get Read pixel data with mipmaps \"%s\"!\n", filename);
      return 0;
    }

  // Close the file 
  fclose (fp);
  	 printf("image ->width = %i\n",  image ->width);
	 printf(" image ->height = %i\n",  image ->height);
	 printf(" image->imgChanal = %i\n",   image->imgChanal);
   printf("Image DDS load OK %s \n", filename);
  return 1;
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int rglLoadImageSDL(char *filename, RGL_TextureImage *image)
 {
 	unsigned int rmask = 0x00ff0000;
    unsigned int gmask = 0x0000ff00;
    unsigned int bmask = 0x000000ff;
    unsigned int amask = 0x00000000;
 	SDL_Surface* result_image=NULL;
 	SDL_Surface* temp_image = IMG_Load(filename);
 	SDL_PixelFormat *format = temp_image->format;
     
     if (format->BytesPerPixel ==4)
		{
			image->imgChanal =4;
			amask = 0xff000000;
			result_image=SDL_CreateRGBSurface(SDL_HWSURFACE,temp_image->w,temp_image->h,32, rmask,gmask,bmask,amask);
		}
	 if (format->BytesPerPixel ==3)
		{
			image->imgChanal =3;	
			result_image=SDL_CreateRGBSurface(SDL_HWSURFACE,temp_image->w,temp_image->h,24, rmask,gmask,bmask,amask); 
		}
  
    SDL_BlitSurface(temp_image, 0, result_image, 0);
    
 	image ->width =result_image->w;
    image ->height = result_image->h;	
    
	image -> data = (unsigned char *) malloc(image ->width *  image ->height * image->imgChanal);	  
	  
	 unsigned char  *rowlo,  *rowdata;
     rowlo = (unsigned char *)result_image->pixels + (result_image->h-1) * result_image->pitch;   //last line of the image
     rowdata =(unsigned char *) image ->data;
    
  	int  i=0;
    do {
        memcpy(rowdata, rowlo, result_image->pitch);        
        rowdata += result_image->pitch;
        rowlo -= result_image->pitch;
		++i;  		
	}	
  	while ( i< result_image->h);
  
	SDL_FreeSurface(temp_image);	
	SDL_FreeSurface(result_image);
	
	 printf("image ->width = %i\n",  image ->width);
	 printf(" image ->height = %i\n",  image ->height);
	 printf(" image->imgChanal = %i\n",   image->imgChanal);
	
	return 1;
}
