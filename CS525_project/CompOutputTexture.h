#ifndef __COMPOUTPUTTEXTURE_H__
#define __COMPOUTPUTTEXTURE_H__

#include "Texture.h"

class CompOutputTexture 
{
public:
	void init() ;
	GLuint get_handle() ;

	void bind(GLenum access_mode, int bind_loc);

	CompOutputTexture(int width, int height, GLenum color_mode);

private:
	GLenum color_mode;
	int width, height;
	GLuint handle;
};

#endif
