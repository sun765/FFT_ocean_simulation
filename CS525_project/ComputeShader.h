#ifndef __COMPUTESHADER_H__
#define __COMPUTESHADER_H__

#include "Shader.h"

class ComputeShader : public Shader
{
public :

	// change to private later
	void init_shader() override;
	void bind_shader() override;

	GLuint get_shader_handle();
	GLuint get_program_handle();

	ComputeShader(string code_path);
	ComputeShader();

private:

	GLuint shader_handle;
	GLuint program_handle;

	string compshader_code_path;
	
};

#endif 

