#ifndef __COMPOUTPUTTEXTURE_H__
#define __COMPOUTPUTTEXTURE_H__

#include "Texture.h"

class CompOutputTexture : public Texture
{
public:
	void init() override;
	GLuint get_handle() override;

	void bind(GLenum access_mode, int bind_loc);
	void bind_data(void* data);

	CompOutputTexture(int width, int height, GLenum color_mode);
	CompOutputTexture(int width, int height, GLenum color_mode, void * data_ptr);
	CompOutputTexture();

private:
	GLenum color_mode = GL_RGBA32F;
};

#endif
