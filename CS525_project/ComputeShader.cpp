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

GLuint ComputeShader::get_handle()
{
	return this->shader_handle;
}

ComputeShader::ComputeShader(string code_path)
{
	this->compshader_code_path = code_path;
	this->init_shader();
}

ComputeShader::ComputeShader()
{

}
