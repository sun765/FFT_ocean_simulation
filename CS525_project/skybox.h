#ifndef __SKYBOX_H__
#define __SKYBOX_H__



#include <windows.h>
#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "location.h"
#include "InitShader.h"
#include <string>
class skybox
{
public:
	skybox();
	void init();
	void reload_shader();
	
	void draw_sky( GLuint cubemap_id, glm::mat4 V, glm::mat4 P, int pass);
	~skybox();

private:
	GLuint vao;
	GLuint shader;
	std::string vertex_shader;
	std::string fragment_shader;
	void initVao();
	void initShader();


	GLuint create_cube_vao();
	GLuint create_cube_posvbo();
	

};

#endif 

