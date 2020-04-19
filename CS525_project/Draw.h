#pragma once
#include "imgui_impl_glut.h"
#include <GL/freeglut.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>

class Draw
{
public:
	Draw();
	~Draw();
	void draw_imgui();
	void draw_fish(GLuint shader,const float aspect_ratio, glm::mat4 V, glm::mat4 P);
	void draw_surf(GLuint shader,GLuint vao, const float aspect_ratio, glm::mat4 V, glm::mat4 P);
};

