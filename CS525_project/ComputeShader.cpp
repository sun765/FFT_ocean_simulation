#include "ComputeShader.h"

void ComputeShader::init_shader()
{
	// refractor later
	GLchar* source = readShaderSource(&compshader_code_path[0]);

	shader_handle = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader_handle, 1, (const GLchar**)&source, NULL);
	glCompileShader(shader_handle);
	program_handle = glCreateProgram();
	glAttachShader(program_handle, shader_handle);
	glLinkProgram(program_handle);
	check_program_link_status(program_handle);
}

void ComputeShader::bind_shader()
{
	glUseProgram(program_handle);
}

void ComputeShader::set_uniform_int(string var_name, int val)
{
	GLuint var_loc = glGetUniformLocation(this->program_handle, &var_name[0]);
	glUniform1i(var_loc, val);
}

void ComputeShader::set_uniform_float(string var_name, float val)
{
	GLuint var_loc = glGetUniformLocation(this->program_handle, &var_name[0]);
	glUniform1f(var_loc, val);
}

GLuint ComputeShader::get_shader_handle()
{
	return this->shader_handle;
}

GLuint ComputeShader::get_program_handle()
{
	return this->program_handle;
}

ComputeShader::ComputeShader(string code_path)
{
	this->compshader_code_path = code_path;
	this->init_shader();
}

ComputeShader::ComputeShader()
{

}
