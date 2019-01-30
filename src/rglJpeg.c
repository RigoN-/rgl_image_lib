#include "rglJpeg.h"


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

int rglLoadJPG(char *filename, rgl_Image_t *image)
 {

struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	FILE * infile;
	JSAMPARRAY buffer;
	int row_stride;
	int cont;

	infile = fopen(filename,"rb");
	if(!infile) {		
      fprintf (stderr, "ERROR: couldn't open \"%s\"!\n", filename);
      return 1;
    }
	
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if(setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return 2;
	}
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo,TRUE);
	jpeg_start_decompress(&cinfo);
	row_stride = cinfo.output_width * cinfo.num_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo,JPOOL_IMAGE,row_stride,1);

	 image ->width =cinfo.image_width;
     image ->height = cinfo.image_height;
     image->imgBpp = cinfo.num_components*8;
     if (image->imgBpp/8 ==4)
		image->imgType=RGL_RGBA;
	 else
		image->imgType=RGL_RGB;
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
    return 0;
   
}
