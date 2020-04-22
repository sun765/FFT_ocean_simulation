#include "Texture.h"



void Texture::bind()
{
}

Texture::Texture(int width, int height)
{
	this->width  = width;
	this->height = height;
}

Texture::Texture()
{
	this->width = 0;
	this->height = 0;
}
