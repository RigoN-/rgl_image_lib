//-----------------------------------------------------------------------------
// "Always share your knowledge"
//
// File: ctexture.cpp
//
// Author : JeGX - jegx2002@yahoo.fr - blox.e9h@caramail.com
//
// Date : 31.10.2002
//
// PURPOSE : OpenGL and Direct3D tutorials.
// 
// Desc: Tutorial 10 : Sphere Environment Mapping and Stencil buffer
//
// Copyright (c) 2002 JeGX - BloX.E9h - http://jegx.free.fr
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//
//-----------------------------------------------------------------------------

#include "ctexture.h"



cTexture::cTexture()
{
	m_width=0;
	m_height=0;
	m_pixmap=NULL;
	m_floatPixmap=NULL;
	m_texture_id=-1;
}


cTexture::~cTexture()
{
	if(m_pixmap)
	{
		delete [] m_pixmap;
		m_pixmap=NULL;
	}

	if(m_floatPixmap)
	{
		delete [] m_floatPixmap;
		m_floatPixmap=NULL;
	}

}

int cTexture::isPowerOf2( int n )
{
	return( !(n & (n-1)) );
}


int cTexture::loadFromFile( char *filename )
{
	FILE *fp;
	sBMP_header *header;
	sBMP_info *info;
	Dword_ alignedBytesPerScanline;
	Dword_ fileSize;
	Byte_ *fileBuffer;
	Byte_ *data;
	Dword_ i, x, y;
	cRGB_Byte_Pixel *pixel;

	if( (fp = fopen(filename, "rb")) == NULL )
	{
		return(NULL);
	}

	// Move fp to the end.
	fseek( fp, 0 ,SEEK_END );
	
	// get the file size.
	fileSize = (Dword_)ftell( fp );

	// Move fp to the beginning.
	fseek( fp, 0 ,SEEK_SET );

	// Alloc a buffer to store the whole file
	fileBuffer = (Byte_ *)malloc( fileSize * sizeof(Byte_) );
	if( fileBuffer==NULL )
	{
		return(NULL);
	}

	// Read file entirely.
	fread( fileBuffer, fileSize * sizeof(Byte_), 1, fp );

	// Ok, the is in buffer, so we can close the file.
	fclose( fp );

	// Retrieval of bmp file header structure.
	header = (sBMP_header *)fileBuffer;

	// Retrieval of bmp file info structure.
	info = (sBMP_info *)(fileBuffer + 14 );


	// Check right format. //////////////////////////////////////////////////////

	
	if( !isPowerOf2(info->LargeurImage) || !isPowerOf2(info->HauteurImage) )
	{
		free( fileBuffer );
		MessageBox( NULL, "Loading image failed. Must be power of 2.", "Error", MB_OK );
		return(K_ERR);
	}

	if( info->BitParPixel!=24 ) 
	{
		free( fileBuffer );
		MessageBox( NULL, "Loading image failed. Must be 24 bits/pixels.", "Error", MB_OK );
		return(K_ERR);
	}

	if( info->FormatCompressImage!=0 )
	{
		free( fileBuffer );
		return(K_ERR);
	}

	// Retrieval bmp data.
	data = (Byte_ *)(fileBuffer + 14 + 40 );

	// Allocate a buffer to store the bitmap data.
	m_pixmap = new cRGB_Byte_Pixel[ info->LargeurImage * info->HauteurImage ];
	if( m_pixmap==NULL )
	{
		free( fileBuffer );
		MessageBox( NULL, "Loading image failed. Not enough memory.", "Error", MB_OK );
		return(K_ERR);
	}


	// Compute the aligned bytes per scanline.
	alignedBytesPerScanline = (((info->LargeurImage * 24) + 31) & ~31) >> 3;

	i=0;
	for( y=0; y<info->HauteurImage; y++ )
	{
		pixel = (cRGB_Byte_Pixel *)(data + (y * alignedBytesPerScanline));

		for( x=0; x<info->LargeurImage; x++ )
		{
			( (cRGB_Byte_Pixel*)m_pixmap + i )->r = pixel->b; 
			( (cRGB_Byte_Pixel*)m_pixmap + i )->g = pixel->g; 
			( (cRGB_Byte_Pixel*)m_pixmap + i )->b = pixel->r; 
			i++;
			pixel++;
		}
	}

	m_width = info->LargeurImage;
	m_height = info->HauteurImage;

	free( fileBuffer );

	convert_from_3b_to_4f();

	return(K_OK);
}


cRGB_Byte_Pixel *cTexture::getPixmapPointer()
{
	return(m_pixmap);
}

int cTexture::getWidth()
{
	return(m_width);
}


int cTexture::getHeight()
{
	return(m_height);
}


void cTexture::convert_from_3b_to_4f()
{
	Dword_ i, 
	size = m_width * m_height;

	m_floatPixmap = new cColor[size];

	for( i=0; i<size; i++ )
	{
		m_floatPixmap[i].r = (float)m_pixmap[i].r / 255.0f;
		m_floatPixmap[i].g = (float)m_pixmap[i].g / 255.0f;
		m_floatPixmap[i].b = (float)m_pixmap[i].b / 255.0f;
		m_floatPixmap[i].a = 1.0f;
	}
}

cColor *cTexture::getFloatPixmapPointer()
{
	return(m_floatPixmap);
}


