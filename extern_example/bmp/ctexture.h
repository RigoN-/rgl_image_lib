//-----------------------------------------------------------------------------
// "Always share your knowledge"
//
// File: ctexture.h
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

#ifndef _CTEXTURE_H
#define _CTEXTURE_H


#include "common.h"

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< BMP Structures START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
struct sBMP_header
{
	Word_ codmag;
	Dword_ taille;
	Word_ reserv1;
	Word_ reserv2;
	Dword_ offsetImage;
};

struct sBMP_info
{
	Dword_ TailleZoneInfo;
	Dword_ LargeurImage;
	Dword_ HauteurImage;
	Word_ Nb_plans;							
	Word_ BitParPixel;						
	Dword_ FormatCompressImage;
	Dword_ TailleOctets;
	Dword_ ResolutionHorizon;				
	Dword_ ResolutionVertical;				
	Dword_ NbCouleurUtil;					
	Dword_ NbIndexCouleur;					
};

struct sImageBMP
{
	sBMP_header header;
	sBMP_info info;
	cRGB_Byte_Pixel *data;
};
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< BMP Structures END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

class cTexture
{

public:

	cTexture();
	~cTexture();

	int loadFromFile( char *filename );

	cRGB_Byte_Pixel *getPixmapPointer();
	int getWidth();
	int getHeight();
	
	cColor *getFloatPixmapPointer();
	
	int isPowerOf2( int n );
	
	unsigned int m_texture_id;

private:

	int m_width;
	int m_height;
	cRGB_Byte_Pixel *m_pixmap;
	
	void convert_from_3b_to_4f();
	cColor *m_floatPixmap;
};


#endif
