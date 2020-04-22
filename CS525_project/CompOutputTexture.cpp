#include "CompOutputTexture.h"



void CompOutputTexture::init()
{
	glGenTextures(1, &this->handle);
	glBindTexture(GL_TEXTURE_2D, this->handle);
	glTexStorage2D(GL_TEXTURE_2D, 1, this->color_mode, this->width, this->height);
}


GLuint CompOutputTexture::get_handle()
{
	return this->handle;
}

void CompOutputTexture::bind(GLenum access_mode, int bind_loc)
{
	glBindImageTexture(bind_loc, this->handle, 0, GL_TRUE, 0, access_mode, this->color_mode);
}

CompOutputTexture::CompOutputTexture(int width, int height, GLenum color_mode)
{
	this->color_mode = color_mode;
	this->width = width;
	this->height = height;

	this->init();
}
