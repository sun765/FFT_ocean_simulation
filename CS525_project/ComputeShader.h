#ifndef __COMPUTESHADER_H__
#define __COMPUTESHADER_H__

#include "Shader.h"

class ComputeShader : public Shader
{
public :

	void init_shader() override;
	void bind_shader() override;

	ComputeShader(string code_path);
	ComputeShader();

private:

	GLuint shader_handle;
	GLuint program_handle;

	string compshader_code_path;
	
};

#endif 

