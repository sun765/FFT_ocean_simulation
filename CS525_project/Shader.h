#ifndef __SHADER_H__
#define __SHADER_H__


#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Shader
{
public:
	virtual void init_shader() = 0;
	virtual void bind_shader() = 0;

	bool check_program_link_status(GLuint obj);

	static char* readShaderSource(const char* shaderFile);

};
#endif 
