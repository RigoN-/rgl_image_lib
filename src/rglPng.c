#include "rglPng.h"


void rglGetPNGColorInfo (int color_type, rgl_Image_t *image)
{
  switch (color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
    	 image->imgBpp = 1*8;
      break;

    case PNG_COLOR_TYPE_GRAY_ALPHA:
       image->imgBpp  = 2*8;
      break;

    case PNG_COLOR_TYPE_RGB:
      image->imgBpp  = 3*8;
      break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
    	image->imgBpp  = 4*8;
      break;

    default:
      /* Badness */
      break;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int rglLoadPNG (char *filename, rgl_Image_t *image)
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
      fprintf (stderr, "ERROR: couldn't open \"%s\"!\n", filename);
      return 1;
    }

  /* Read magic number */
 // fread (magic, 1, sizeof (magic), fp);
  
  if ( !fread (magic, 1, sizeof (magic), fp))
	{
	printf("Error read number from %s.\n", filename);
	return 2;
    }

  /* Check for valid magic number */
  if (!png_check_sig (magic, sizeof (magic)))
    {
      fprintf (stderr, "error: \"%s\" is not a valid PNG image!\n",
	       filename);
      fclose (fp);
      return 3;
    }

  /* Create a png read struct */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
    {
      fclose (fp);
      return 4;
    }

  /* Create a png info struct */
  info_ptr = png_create_info_struct (png_ptr);
  if (!info_ptr)
    {
      fclose (fp);
      png_destroy_read_struct (&png_ptr, NULL, NULL);
      return 5;
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

      return 6;
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
 // if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
   // png_set_gray_1_2_4_to_8 (png_ptr);

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
	 if (image->imgBpp/8 ==4)
		image->imgType=RGL_RGBA;
	 else
		image->imgType=RGL_RGB;
     image -> data = (unsigned char *) malloc(image ->width *  image ->height *  image->imgBpp/8);
     /*
     if (rglLog == 1) {
		 printf("image ->width = %i\n",  image ->width);
		 printf(" image ->height = %i\n",  image ->height);
		 printf(" image->imgChanal = %i\n",   image->imgBpp);
		}
		
       Setup a pointer array.  Each one points at the begening of a row. */
  row_pointers = (png_bytep *)malloc (sizeof (png_bytep) *  image ->height);

  for (i = 0; i < image ->height; ++i)
    {
      row_pointers[i] = (png_bytep)( image -> data +((image ->height - (i + 1)) * image ->width * image->imgBpp/8));
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
  return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int rglLoadPNG2 (char *filename, rgl_Image_t *image)
{
	FILE *fp = NULL;
	png_byte header[8];
	png_structp png_ptr;
	png_infop info_ptr;
	//int bit_depth, color_type;
	png_bytep *row_pointers = NULL;
	 png_uint_32 w, h;
	 int bit_depth, color_type,i;
	
	
	 
/* Open image file */
	fp = fopen (filename, "rb");
	if (!fp) {
		fprintf (stderr, "ERROR: couldn't open \"%s\"!\n", filename);
		return 1;
    } 
/* Read magic number */
// fread (magic, 1, sizeof (magic), fp);
  
	if ( !fread (header, 1, sizeof (header), fp)){
		printf("ERROR: File %s is not read\n", filename);
		return 2;
	}
	
/* Check for valid magic number */
	if (!png_check_sig (header, sizeof (header)))	{
		printf("ERROR: File %s is not recognized as a PNG file\n", filename);
		fclose (fp);
		return 3;
	}	
	
/* Create a png read struct */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		printf("ERROR: png_create_read_struct failed\n");
		fclose (fp);
		return 4;
	}	

/* Create a png info struct */
	info_ptr = png_create_info_struct (png_ptr);
	if (!info_ptr){
		printf("ERROR: png_create_info_struct failed\n");
		fclose (fp);
		png_destroy_read_struct (&png_ptr, NULL, NULL);
		return 5;
	}

/* Initialize the setjmp for returning properly after a libpng
error occured */
	if (setjmp (png_jmpbuf (png_ptr)))	{
		fclose (fp);
		png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
		if (row_pointers)
			free (row_pointers);
		return 6;
	}

/* Setup libpng for using standard C fread() function
 with our FILE pointer */
	png_init_io (png_ptr, fp);

/* Tell libpng that we have already read the magic number */
	png_set_sig_bytes (png_ptr, sizeof (header));

/* Read png info */
	png_read_info (png_ptr, info_ptr);
//	 png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
/* Update info structure to apply transformations */
  //png_read_update_info (png_ptr, info_ptr);

  /* Retrieve updated information */
  png_get_IHDR (png_ptr, info_ptr, &w, &h, &bit_depth,&color_type, NULL, NULL, NULL);

  /* We can now allocate memory for storing pixel data */
	 image ->width =w;
     image ->height = h;   
	 /* Get image format and components per pixel */
	 rglGetPNGColorInfo(color_type,image);
	  if (image->imgBpp/8 ==4)
		image->imgType=RGL_RGBA;
	 else
		image->imgType=RGL_RGB;
     image -> data = (unsigned char *) malloc(image ->width *  image ->height *  image->imgBpp/8);
	 /*
     if (rglLog == 1) {
		 printf("image ->width = %i\n",  image ->width);
		 printf(" image ->height = %i\n",  image ->height);
		 printf(" image->imgChanal = %i\n",   image->imgBpp);
		}
		
       Setup a pointer array.  Each one points at the begening of a row. */
  row_pointers = (png_bytep *)malloc (sizeof (png_bytep) *  image ->height);

  for (i = 0; i < image ->height; ++i)
    {
      row_pointers[i] = (png_bytep)( image -> data +((image ->height - (i + 1)) * image ->width * image->imgBpp/8));
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
  return 0;
}


int loadPngImage(char *name, rgl_Image_t *image) 
{
png_structp png_ptr;
png_infop info_ptr;
unsigned int sig_read = 0;
int color_type, interlace_type;
FILE *fp;
if ((fp = fopen(name, "rb")) == NULL)
return 1;

/* Create and initialize the png_struct
* with the desired error handler
* functions. If you want to use the
* default stderr and longjump method,
* you can supply NULL for the last
* three parameters. We also supply the
* the compiler header file version, so
* that we know if the application
* was compiled with a compatible version
* of the library. REQUIRED
*/
png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
NULL, NULL, NULL);
if (png_ptr == NULL) {
fclose(fp);
return 1;
}
/* Allocate/initialize the memory
* for image information. REQUIRED. */
info_ptr = png_create_info_struct(png_ptr);
if (info_ptr == NULL) {
fclose(fp);
png_destroy_read_struct(&png_ptr, NULL, NULL);
return 1;
}
/* Set error handling if you are
* using the setjmp/longjmp method
* (this is the normal method of
* doing things with libpng).
* REQUIRED unless you set up
* your own error handlers in
* the png_create_read_struct()
* earlier.
*/
if (setjmp(png_jmpbuf(png_ptr))) {
/* Free all of the memory associated
* with the png_ptr and info_ptr */
png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
fclose(fp);
/* If we get here, we had a
* problem reading the file */
return 1;
}
/* Set up the output control if
* you are using standard C streams */
png_init_io(png_ptr, fp);
/* If we have already
* read some of the signature */
png_set_sig_bytes(png_ptr, sig_read);
/*
* If you have enough memory to read
* in the entire image at once, and
* you need to specify only
* transforms that can be controlled
* with one of the PNG_TRANSFORM_*
* bits (this presently excludes
* dithering, filling, setting
* background, and doing gamma
* adjustment), then you can read the
* entire image (including pixels)
* into the info structure with this
* call
*
* PNG_TRANSFORM_STRIP_16 |
* PNG_TRANSFORM_PACKING forces 8 bit
* PNG_TRANSFORM_EXPAND forces to
* expand a palette into RGB
*/
png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
png_uint_32 width, height;
int bit_depth;
png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
image ->width = width;
image ->height= height;
rglGetPNGColorInfo(color_type,image);
 if (image->imgBpp/8 ==4)
		image->imgType=RGL_RGBA;
	 else
		image->imgType=RGL_RGB;
unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
image->data = (unsigned char*) malloc(row_bytes * image ->height);
png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
for (int i = 0; i < image ->height; i++) {
// note that png is ordered top to
// bottom, but OpenGL expect it bottom to top
// so the order or swapped
memcpy(image->data+(row_bytes * (image ->height-1-i)), row_pointers[i], row_bytes);
}
/* Clean up after the read,
* and free any memory allocated */
png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
/* Close the file */
fclose(fp);
/* That's it */
return 0;
} 

