#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

class Texture
{
public:
	virtual void init() = 0;
	virtual void bind();

	virtual GLuint get_handle() = 0;

	Texture(int width, int height);
	Texture();

protected:

	GLuint handle;
	int width, height;
};

#endif 

