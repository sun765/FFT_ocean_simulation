#include "Draw.h"



Draw::Draw()
{
}


Draw::~Draw()
{
}


void Draw::draw_surf(GLuint shader, GLuint vao,const float aspect_ratio, glm::mat4 V, glm::mat4 P)
{
	glUseProgram(shader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	//draw_surf(vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}