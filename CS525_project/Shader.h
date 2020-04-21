#ifndef __SHADER_H__
#define __SHADER_H__


#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <vector>
#include <iostream>

using namespace std;

class Shader
{
public:
	virtual void init_shader() = 0;
	virtual void bind_shader() = 0;

	bool check_program_link_status(GLuint obj);

};
#endif 
