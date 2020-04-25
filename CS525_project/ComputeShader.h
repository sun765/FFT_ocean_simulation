#ifndef __COMPUTESHADER_H__
#define __COMPUTESHADER_H__

#include "Shader.h"

class ComputeShader : public Shader
{
public :

	// change to private later
	void init_shader() override;
	void bind_shader() override;

	void set_uniform_int(string var_name, int val);
	void set_uniform_float(string var_name, float val);

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

