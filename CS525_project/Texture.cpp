#include "Texture.h"



void Texture::bind()
{
}

void Texture::load_texture(string file_path)
{
	this->handle = LoadTexture(&file_path[0]);
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
