#ifndef __LENSFLARE_H__
#define __LENSFLARE_H__



#include <windows.h>
#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <algorithm> 

#include "structs.h"
#include "location.h"
#include "InitShader.h"
#include "LoadTexture.h"
#include "screenUI.h"
#include "query.h"


class lensFlare
{
public:
	lensFlare( glm::vec2 sun_screen_pos, sunP *sP, lensFlareP* lP);
	~lensFlare();

	void update(glm::vec2 sun_screen_pos, sunP *sP,lensFlareP* lP);
	void drawFlare();
	void conditional_render();
	void draw_occluder();
	static void reload_shader();
	static void init_window_wh(float w, float h);
	int get_nums();
	void test();
	GLuint get_texture_id(int i);
	

private:


	float coverage;
	int total_samples;
	query sample_query;
	void do_query();

	static float window_w, window_h;
	glm::vec2 sun_screen_pos;

	lensFlareP lP;
	sunP sP;

	std::vector<screenUI> flares;
	std::vector<glm::vec2> flare_pos;
	std::vector<glm::vec2> flare_scale;
	std::vector<GLuint> texture_id;
	screenUI occluder;
	int  nums =0;

	glm::vec2 gui_wh;
	glm::vec2 window_wh;
	glm::vec2 gui_pos;
	std::string texture_name;


	void init_textures();

	void cal_pos();
	float cal_fade(float dist, float fade_factor);
	



	
};

#endif

