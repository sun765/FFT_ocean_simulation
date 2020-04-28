#ifndef __VERTEXFRAGSHADER_H__
#define __VERTEXFRAGSHADER_H__

#include "Shader.h"

class VertexFragShader : public Shader
{
public:
	void init_shader() override;
	void bind_shader() override;

	GLuint get_program_handle();

	VertexFragShader();
	VertexFragShader(string vertex_shader_path, string frag_shader_path);

private:

	GLuint program_handle;

	string vertex_shader_path;
	string frag_shader_path;
};

#endif
