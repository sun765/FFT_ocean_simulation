#include "skybox.h"
#include "iostream"



skybox::skybox()
{
	init();
}


skybox::~skybox()
{
}


GLuint skybox::create_cube_vao()
{
	GLuint vao;
	//generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo = create_cube_posvbo();

	glEnableVertexAttribArray(AttribLoc::Pos_loc); //enable this attribute
	glVertexAttribPointer(AttribLoc::Pos_loc, 3, GL_FLOAT, false, 0, 0);
	glBindVertexArray(0); //unbind the vao
	return vao;

}

GLuint skybox::create_cube_posvbo()
{
	GLuint vbo;
	const float cube_verts[] = { -1.0f,-1.0f,-1.0f,
								  -1.0f,-1.0f, 1.0f,
								  -1.0f, 1.0f, 1.0f,
								  1.0f, 1.0f,-1.0f,
								  -1.0f,-1.0f,-1.0f,
								  -1.0f, 1.0f,-1.0f,
								  1.0f,-1.0f, 1.0f,
								  -1.0f,-1.0f,-1.0f,
								  1.0f,-1.0f,-1.0f,
								  1.0f, 1.0f,-1.0f,
								  1.0f,-1.0f,-1.0f,
								  -1.0f,-1.0f,-1.0f,
								  -1.0f,-1.0f,-1.0f,
								  -1.0f, 1.0f, 1.0f,
								  -1.0f, 1.0f,-1.0f,
								  1.0f,-1.0f, 1.0f,
								  -1.0f,-1.0f, 1.0f,
								  -1.0f,-1.0f,-1.0f,
								  -1.0f, 1.0f, 1.0f,
								  -1.0f,-1.0f, 1.0f,
								  1.0f,-1.0f, 1.0f,
								  1.0f, 1.0f, 1.0f,
								  1.0f,-1.0f,-1.0f,
								  1.0f, 1.0f,-1.0f,
								  1.0f,-1.0f,-1.0f,
								  1.0f, 1.0f, 1.0f,
								  1.0f,-1.0f, 1.0f,
								  1.0f, 1.0f, 1.0f,
								  1.0f, 1.0f,-1.0f,
								  -1.0f, 1.0f,-1.0f,
								  1.0f, 1.0f, 1.0f,
								  -1.0f, 1.0f,-1.0f,
								  -1.0f, 1.0f, 1.0f,
								  1.0f, 1.0f, 1.0f,
								  -1.0f, 1.0f, 1.0f,
								  1.0f,-1.0f, 1.0f };

	//generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenBuffers(1, &vbo); // Generate vbo to hold vertex attributes for triangle
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //specify the buffer where vertex attribute data is stored
										//upload from main memory to gpu memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), &cube_verts[0], GL_STATIC_DRAW);

	return vbo;

}

void skybox::draw_sky(  GLuint cubemap_id, glm::mat4 V, glm::mat4 P, int pass)
{


	glUseProgram(0);
	glUseProgram(shader);
	glUniform1i(UniformLoc::pass_loc, pass);
	glDepthMask(GL_FALSE);
	glm::mat4 Msky = glm::scale(glm::vec3(50.0f));
	glm::mat4 PVM = P * V*Msky;
	PVM[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(UniformLoc::PVM_loc, 1, false, glm::value_ptr(PVM));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id);
	glUniform1i(UniformLoc::cubemap_loc, 1); // we bound our texture to texture unit 1
	//glActiveTexture(GL_TEXTURE12);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id);
	//glUniform1i(UniformLoc::dcubemap_loc, 12); // we bound our texture to texture unit 12

	//std::cout << cubemap_id << std::endl;

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glUseProgram(0);
}
void skybox::initVao()
{
	this->vao = create_cube_vao();
}

void skybox::initShader()
{
	vertex_shader = "Shaders/skybox_vs.glsl";
	fragment_shader = "Shaders/skybox_fs.glsl";
	shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());
}

void skybox::init()
{
	initShader();
	initVao();

}

void skybox::reload_shader()
{
	GLuint new_shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());
	if (new_shader == -1) // loading failed
	{
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	}
	else
	{
		if (shader != -1)
		{
			glDeleteProgram(shader);
		}
		shader = new_shader;
	}
}
//Draw the set of points on the surface