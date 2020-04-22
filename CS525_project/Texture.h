#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include "LoadTexture.h"

using namespace std;

class Texture
{
public:
	virtual void init() = 0;
	virtual void bind();

	virtual GLuint get_handle() = 0;

	void load_texture(string file_path);

	Texture(int width, int height);
	Texture();

protected:

	GLuint handle;
	int width, height;
};

#endif 

