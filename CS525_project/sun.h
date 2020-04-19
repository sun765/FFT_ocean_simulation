#ifndef __SUN_H__
#define __SUN_H__



#include <windows.h>
#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

#include "structs.h"
#include "location.h"
#include "InitShader.h"
#include "LoadTexture.h"
#include "camera.h"

class sun
{
public:
	sun();
	sun(glm::vec2 window_wh, sunP *sP, camera *cam);
	~sun();
	void update(camera *cam,sunP* s);
	void draw_sun( glm::mat4 P);
	glm::vec2 get_screen_coord(glm::mat4 P);
	static void init_shader();
	static void reload_shader();
	void test(glm::mat4 P);

private:

	// may delete later
	GLuint texture_id;
	std::string texture_name;
	camera cam;

	sunP sP;
	glm::vec2 window_wh;
	glm::vec2 sun_screen_coord;
	

	GLuint vao;
	static GLuint shader;
	static std::string vertex_shader;
	static std::string fragment_shader;

	GLuint create_vbo();
	GLuint create_vao();

	
	void init_vao();
	void init();
	void init_texture();
	

};

#endif
