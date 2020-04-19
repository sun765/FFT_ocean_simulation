#ifndef __SCREENUI_H__
#define __SCREENUI_H__



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
class screenUI
{
public:
	screenUI();
	screenUI(float window_w, float window_h, glm::vec2 wh, glm::vec2 gui_pos, GLuint texture_id);
	
	~screenUI();
	
	void drawUI(glm::vec2 gui_wh, glm::vec2 gui_pos,GLuint texture_id,float strength, float fade_factor);
	void draw_depth_UI(glm::vec2 gui_wh, glm::vec2 gui_pos);
	void update(glm::vec2 wh, glm::vec2 gui_pos);
	static void reload_shader();
	static void init_shader();
	


private:
	glm::vec2 gui_wh;
	glm::vec2 window_wh;
	glm::vec2 gui_pos;
	std::string texture_name;
	GLuint texture_id;


	// don't do it this way!

	GLuint vao;
	static GLuint shader;
	static std::string vertex_shader;
	static std::string fragment_shader;

	GLuint create_vbo();
	GLuint create_vao();

	void init_vao();

	
	void init();
	


	
};
#endif

