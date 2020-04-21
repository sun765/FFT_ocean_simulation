#include "ComputeShader.h"

void ComputeShader::init_shader()
{
	// refractor later
	//char* source = readShaderSource(&compshader_code_path[0]);

	shader_handle = glCreateShader(GL_COMPUTE_SHADER);
	//glShaderSource(shader_handle, 1, (const GLchar**)&source, NULL);
	glCompileShader(shader_handle);
	program_handle = glCreateProgram();
	glAttachShader(program_handle, shader_handle);
	glLinkProgram(program_handle);
	check_program_link_status(program_handle);
}

void ComputeShader::bind_shader()
{
}
