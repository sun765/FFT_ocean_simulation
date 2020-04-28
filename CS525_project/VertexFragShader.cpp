#include "VertexFragShader.h"

void VertexFragShader::init_shader()
{
	bool error = false;
	struct Shader
	{
		const char* filename;
		GLenum       type;
		GLchar* source;
	}  shaders[2] =
	{
	   { &this->vertex_shader_path[0], GL_VERTEX_SHADER, NULL },
	   { &this->frag_shader_path[0]  , GL_FRAGMENT_SHADER, NULL }
	};

	this->program_handle = glCreateProgram();

	for (int i = 0; i < 2; ++i)
	{
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == NULL)
		{
			std::cerr << "Failed to read " << s.filename << std::endl;
			error = true;
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
		glCompileShader(shader);

		GLint  compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			std::cerr << s.filename << " failed to compile:" << std::endl;
			//printShaderCompileError(shader);
			error = true;
		}

		delete[] s.source;

		glAttachShader(this->program_handle, shader);
	}

	/* link  and error check */
	glLinkProgram(this->program_handle);

	GLint  linked;
	glGetProgramiv(this->program_handle, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		std::cerr << "Shader program failed to link" << std::endl;
		//printProgramLinkError(program);
		error = true;
	}

	check_program_link_status(this->program_handle);
}

void VertexFragShader::bind_shader()
{
	glUseProgram(this->program_handle);
}

GLuint VertexFragShader::get_program_handle()
{
	return this->program_handle;
}

VertexFragShader::VertexFragShader()
{
	this->program_handle = -1;
}

VertexFragShader::VertexFragShader(string vertex_shader_path, string frag_shader_path)
{
	this->vertex_shader_path = vertex_shader_path;
	this->frag_shader_path = frag_shader_path;
	this->init_shader();
}
