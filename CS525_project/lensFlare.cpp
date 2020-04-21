#include "lensFlare.h"
//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>


float lensFlare::window_w = 1280.0;
float lensFlare::window_h = 720.0;

namespace filesystem = std::experimental::filesystem;

lensFlare::lensFlare(glm::vec2 sun_screen_pos, sunP* sP, lensFlareP* lP)
{
	//nums = 8;
	
	update(sun_screen_pos,sP, lP);
	init_textures();
	total_samples = int(this->sP.sun_scale.x)*int(this->sP.sun_scale.y);
	//std::cout << total_samples;
	//std::cout << nums;
	for (int i = 0; i < nums; i++)
	{
		flare_pos.push_back(glm::vec2(0.0));
		flare_scale.push_back(glm::vec2(glm::vec2(this->lP.scale[i])));
		flares.push_back (screenUI(window_w, window_h, flare_scale.at(i), flare_pos.at(i), texture_id.at(i)));
	}

	occluder = screenUI(window_w, window_h,this->sP.sun_scale, this->sP.sun_pos, 0);
	sample_query = query(GL_SAMPLES_PASSED);
}


lensFlare::~lensFlare()
{

}

void lensFlare::reload_shader()
{
	screenUI::reload_shader();
}

void lensFlare::update(glm::vec2 sun_screen_pos, sunP *sP, lensFlareP* lP)
{
	this->sP = *sP;
	this->lP = *lP;
	this->sun_screen_pos = sun_screen_pos;
	for (int i = 0; i < nums; i++)
	{
		flare_scale.at(i) = glm::vec2(this->lP.scale[i]);
	}
	cal_pos();
}

void lensFlare::init_textures()
{
	std::string path = "Textures/lensflare/";
	for (const auto & entry : std::experimental::filesystem::v1::directory_iterator(path))
	{
		nums++;
		std::string filename = entry.path().string();
		GLuint id = LoadTexture(filename.c_str());
		texture_id.push_back(id);
	}



}

void lensFlare::cal_pos()
{
	glm::vec2 v = glm::vec2(0.0) - sun_screen_pos;
	float length = 2.5 / float(nums);
	for (int i = 0; i < nums; i++)
	{
		glm::vec2 screen_pos = sun_screen_pos + (i+1) * length * v; //-1 ~ 1
		float x = (screen_pos.x + 1.0) / 2.0*window_w;
		float y = (screen_pos.y + 1.0) / 2.0*window_h;
		glm::vec2 real_screen_pos = glm::vec2(x, y);
		flare_pos.at(i)= real_screen_pos;
	}
}

void lensFlare::drawFlare()
{
	if (sun_screen_pos.x >= 1.1 || sun_screen_pos.x <= -1.1 || sun_screen_pos.y >= 1.1 || sun_screen_pos.y <= -1.1)
	{
		return;
	}
	glm::vec2 v = sun_screen_pos - glm::vec2(0.0);
	float length = glm::length(v);
	float fade = cal_fade(length, lP.fadefactor);
	//float posfactor = sqrt(2.0) - length;
	for (int i = 0; i < nums; i++)
	{
		flares.at(i).drawUI(flare_scale.at(i), flare_pos.at(i), texture_id.at(i), coverage*lP.strength, fade);
	}
}

void lensFlare::init_window_wh(float w, float h)
{
	window_w = w;
	window_h = h;
}

int lensFlare::get_nums()
{
	return nums;
}

GLuint lensFlare::get_texture_id(int i)
{
	return texture_id.at(i);
}

void lensFlare::test()
{
	std::cout << lP.scale[0] << std::endl;
}

float lensFlare::cal_fade(float dist, float fade_factor)
{
	dist /= sqrt(2.0);
	return dist * ( fade_factor-1.0) + 1.0;
}

void lensFlare:: draw_occluder()
{
	
		float x = (this->sun_screen_pos.x + 1.0) / 2.0*window_w;
		float y = (this->sun_screen_pos.y + 1.0) / 2.0*window_h;
		glm::vec2 sun_pos = glm::vec2(x,y);
		//std::cout << this->sun_screen_pos.x << "     " << this->sun_screen_pos.y << std::endl;
		//std::cout << sP.sun_scale.x;
	    occluder.draw_depth_UI(sP.sun_scale, sun_pos);
}



void lensFlare::conditional_render()
{
	do_query();
	drawFlare();
}

void lensFlare::do_query()
{
	//std::cout << sample_query.isResultReady() << "     " << !sample_query.isInUse() << std::endl;
	if (sample_query.isResultReady()) {
		int visibleSamples = sample_query.getResult();
		this->coverage = std::min(float(visibleSamples) / float(total_samples), 1.0f);
		//std::cout << coverage << std::endl;
	}
	if (!sample_query.isInUse()) {
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		sample_query.start();
		draw_occluder();
		sample_query.end();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
	}
}