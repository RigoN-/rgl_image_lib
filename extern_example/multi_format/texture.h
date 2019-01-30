/* Texture
 *
 * Copyright (C) 2003-2005, Alexander Zaprjagaev <frustum@frustum.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>

/*
 */
class Texture {
public:
	
	enum {
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_RECT = GL_TEXTURE_RECTANGLE_ARB,
		TEXTURE_CUBE = GL_TEXTURE_CUBE_MAP,
		TEXTURE_3D = GL_TEXTURE_3D,
	};
	
	enum {
		// format
		LUMINANCE = 1 << 0,
		LUMINANCE_ALPHA = 1 << 1,
		RGB = 1 << 2,
		RGBA = 1 << 3,
		DEPTH = 1 << 4,
		
		// flags
		FLOAT_16 = 1 << 5,
		FLOAT_32 = 1 << 6,
		
		// compression
		DXT_1 = 1 << 7,
		DXT_3 = 1 << 8,
		DXT_5 = 1 << 9,
		
		// clamp
		CLAMP = 1 << 10,
		CLAMP_TO_EDGE = 1 << 11,
		
		// filter
		NEAREST = 1 << 12,
		LINEAR = 1 << 13,
		NEAREST_MIPMAP_NEAREST = 1 << 14,
		LINEAR_MIPMAP_NEAREST = 1 << 15,
		LINEAR_MIPMAP_LINEAR = 1 << 16,
		
		// anisotropy
		ANISOTROPY_1 = 1 << 17,
		ANISOTROPY_2 = 1 << 18,
		ANISOTROPY_4 = 1 << 19,
		ANISOTROPY_8 = 1 << 20,
		ANISOTROPY_16 = 1 << 21,
	};

	Texture(int width,int height,GLuint target = TEXTURE_2D,int flags = RGB | LINEAR_MIPMAP_LINEAR);
	Texture(const char *name,GLuint target = TEXTURE_2D,int flags = RGB | LINEAR_MIPMAP_LINEAR);
	~Texture();
	
	void load(const char *name,GLuint target = TEXTURE_2D,int flags = RGB | LINEAR_MIPMAP_LINEAR);
	
	void enable(Texture *old = NULL);
	void disable();
	void bind(Texture *old = NULL);
	
	void enable(int unit);
	void enable(int unit,Texture *old);
	void disable(int unit);
	void bind(int unit);
	void bind(int unit,Texture *old);
	
	void copy(GLuint target = 0);
	void render(float x0 = -1.0,float y0 = -1.0,float x1 = 1.0,float y1 = 1.0);
	
	// 2d textures
	static unsigned char *load(const char *name,int &width,int &height);
	static int save(const char *name,const unsigned char *data,int width,int height);

	static unsigned char *load_tga(const char *name,int &width,int &height);
	static unsigned char *load_png(const char *name,int &width,int &height);
	static unsigned char *load_jpeg(const char *name,int &width,int &height);
	static unsigned char *load_dds(const char *name,int &width,int &height);
	
	static int save_tga(const char *name,const unsigned char *data,int width,int height);
	static int save_jpeg(const char *name,const unsigned char *data,int width,int height,int quality);
	
	static unsigned char *rgba2luminance(unsigned char *data,int width,int height);
	static unsigned char *rgba2luminance_alpha(unsigned char *data,int width,int height);
	static unsigned char *rgba2rgb(unsigned char *data,int width,int height);
	
	static float *rgba2luminance_float(unsigned char *data,int width,int height);
	static float *rgba2luminance_alpha_float(unsigned char *data,int width,int height);
	static float *rgba2rgb_float(unsigned char *data,int width,int height);
	static float *rgba2rgba_float(unsigned char *data,int width,int height);
	
	// 3d textures
	static unsigned char *load_3d(const char *name,int &width,int &height,int &depth,int &format);
	static int save_3d(const char *name,const unsigned char *data,int width,int height,int depth,int format);
	
	int width,height,depth;
	int flags;
	
	GLuint target;
	
	GLuint type;
	GLuint internalformat;
	GLuint format;
	
	GLuint id;
};

#endif /* __TEXTURE_H__ */
