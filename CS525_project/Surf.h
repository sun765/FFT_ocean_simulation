
#ifndef __SURF_H__
#define __SURF_H__

#include <windows.h>
#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

//Call this in initOpenGl() 
//GLuint create_surf_vao(const int N);

GLuint create_surf_vao();
GLuint create_surf_posvbo();
GLuint create_surf_colorvbo();
GLuint create_instanced_matrix();

//Call this in display()
//void draw_surf(GLuint vao,const int N);
void draw_surf(GLuint vao);

#endif 

