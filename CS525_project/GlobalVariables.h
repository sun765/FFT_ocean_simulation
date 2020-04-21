#pragma once

#include <windows.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "VideoMux.h"
#include "DebugCallback.h"
#include "VAO.h"
#include "camera.h"
#include "location.h"
#include "water.h"
#include "structs.h"
#include "skybox.h"
#include "gui.h"
#include "screenUI.h"
#include "sun.h"
#include "ComputeShader.h"
#include "layeredRenderingMesh.h"

#define FFT_DIMENSION 256

// bool
bool creating_mode  = false;
bool test           = false;
bool skybox_on      = false;
bool renderScene    = true;
bool is_recording   = false;
bool pause          = false;
bool water_linemode = false;

// int 
int window_width, window_height;
int mesh_nums        = 0;
int current_id       = 0;
int last_id          = 0;
int shading_mode     = 1; // by default it's flat shading

// float
float last_time      = 0;
float time_per_frame = 0;
float height_index   = 0.1;
float time_sec       = 0.0f;   // global time
float time_ms        = 0.0f;
float angle          = 0.0f;
float near_clip      = 0.1;
float far_clip       = 10000.0;
float clip_distance  = far_clip - near_clip;
float aspect_ratio   = 1;

// objects
water* main_water;
skybox* main_sky;
sun* main_sun;
screenUI* tsky;
camera main_camera;
camera reflect_camera;
camera depth_camera;
ComputeShader* comp_shader;

// parameters
terrainP tP;
cameraP mainC;
pointLight p;
directionalLight dl;
fog f;
waterP wP;
quadP qP;
sunP sP;
lensFlareP lP;

// Gluint
GLuint tsky_texture_id;
GLuint texture_ids[] = { -1,-1,-1,-1,-1,-1 };
GLuint depth_texture_id = -1;
GLuint skybox_id = -1;
GLuint fbo = -1;
GLuint rbo = -1;
GLuint texture_width = 1920;
GLuint texture_height = 1080;
GLuint waterTexture_id[] = { -1,-1,-1,-1,-1 };  // 0 is reflection 1 refraction  2 is dudv MAP   3 water normal  4 water depth map
GLenum refraction_buffers[] = { GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT3 };
GLuint terrainpostexture_id = -1;
GLuint test_texture = -1;
GLuint test_shader = -1;
GLuint test_program = -1;


// glm
glm::mat4 P;
glm::vec3 terrain_pos = glm::vec3(9999.0);
glm::vec2 tess_level = glm::vec2(5.0, 5.0);
glm::vec2 previous_campos;


// strings
static const std::string texture_name[]   = { "Textures/blend.png",
											 "Textures/dirt.png",
											 "Textures/flower.png",
											 "Textures/grass.png",
											 "Textures/path.png" ,
											 "Textures/heightmap.png" };
static const std::string skybox_name       = "Textures/skybox";
static const std::string dudv              = "Textures/waterDUDV.png";
static const std::string water_normal      = "Textures/matchingNormalMap.png";
static const std::string tsky_texture_name = "Textures/tsky4.png";


bool load()
{

	std::ifstream in_file;
	std::string header{}; // just for correct position
	std::string line{};
	

	in_file.open("data.txt", std::ios::binary);
	if (!in_file)
	{
		std::cout << " loading data fail!" << std::endl;
		return false;
	}

	std::getline(in_file, line);
	std::istringstream tess_data{ line };
	// ------------------------ tessellation---------------------------------//
	tess_data >> header >> tess_level.x>>tess_level.y;


	std::getline(in_file, line);
	std::istringstream terrain_data{ line };
	// ------------------------ terran---------------------------------//
	terrain_data >>header >> tP.height_level >> tP.seed>> tP.smooth>>tP.roughness>>tP.frequency
				 >> tP.peak_color.x
				 >> tP.peak_color.y
				 >> tP.peak_color.z
				 >> tP.highpart_color.x
				 >> tP.highpart_color.y
				 >> tP.highpart_color.z
				 >> tP.high_height
				 >> tP.middlepart_color.x
				 >> tP.middlepart_color.y
				 >> tP.middlepart_color.z
				 >> tP.middle_height
				 >> tP.underwater_color.x
				 >> tP.underwater_color.y
				 >> tP.underwater_color.z
				 >> tP.under_height
				 >> tP.peak_height;

	// ------------------------ carema---------------------------------//
	std::getline(in_file, line);
	std::istringstream camera_data{ line };
	camera_data  >> header 
			 >> mainC.start_position.x
			 >> mainC.start_position.y
			 >> mainC.start_position.z
			 >> mainC.start_world_up.x
			 >> mainC.start_world_up.y
			 >> mainC.start_world_up.z
			 >> mainC.start_pitch
			 >> mainC.start_yaw
			 >> mainC.start_key_speed
			 >> mainC.start_mouse_speed;

	// ------------------------ pointlight--------------------------------//
	std::getline(in_file, line);
	std::istringstream pointlight_data{ line };

	pointlight_data   >> header
				 >> p.Ka
				 >> p.Kd
				 >> p.Ks
				 >> p.La.x
				 >> p.La.y
				 >> p.La.z
				 >> p.Ld.x
				 >> p.Ld.y
				 >> p.Ld.z
				 >> p.Ls.x
				 >> p.Ls.y
				 >> p.Ls.z
				 >> p.lightPos.x
				 >> p.lightPos.y
				 >> p.lightPos.z;

	// ------------------------ dirlight--------------------------------//
	std::getline(in_file, line);
	std::istringstream dirlight_data{ line };
	   dirlight_data >> header
					 >> dl.Ka
					 >> dl.La.x
					 >> dl.La.y
					 >> dl.La.z
					 >> dl.Kd
					 >> dl.Ld.x
					 >> dl.Ld.y
					 >> dl.Ld.z
					 >> dl.dir.x
					 >> dl.dir.y
					 >> dl.dir.z;

	// ------------------------fog --------------------------------//
	std::getline(in_file, line);
	std::istringstream fog_data{ line };
	fog_data  >> std::left >> header
		 >> f.density
		 >> f.gradient
		 >> f.fog_color.x
		 >> f.fog_color.y
		 >> f.fog_color.z;

	// ------------------------water --------------------------------//
	std::getline(in_file, line);
	std::istringstream water_data{ line };
	water_data    >> std::left >> header
			 >> wP.water_height
			 >> wP.wave_strength
			 >> wP.wave_speed
			 >> wP.water_color.x
			 >> wP.water_color.y
			 >> wP.water_color.z
			 >> wP.water_color_mix
			 >> wP.shineness
			 >> wP.wave_length
			 >> wP.wave_amplitude
			 >> wP.edge_softness
			 >> wP.glitch_offset
		     >> wP.minBlue
			 >> wP.maxBlue
			 >> wP.murkyDepth;

	// ------------------------quad --------------------------------//	
	std::getline(in_file, line);
	std::istringstream quad_data{ line };
	quad_data >> std::left >> header
		 >> qP.N
		 >> qP.repeat;

	//-------------------------sun-------------------------------------//
	std::getline(in_file, line);
	std::istringstream sun_data{ line };
	sun_data >>  std::left >> header
			 >>  sP.sun_pos.x
			 >>  sP.sun_pos.y
			 >>  sP.sun_pos.z
			 >>  sP.sun_scale.x
			 >>  sP.sun_scale.y;

	//-------------------------lens flare-----------------------------//
	std::getline(in_file, line);
	std::istringstream lensflare_data{ line };
	lensflare_data >> std::left >> header
		>> lP.strength;
		 
	for (int i = 0; i < 9; i++)
	{
		lensflare_data >>  lP.scale[i];
	}
	lensflare_data >> lP.fadefactor;



	in_file.close();
	std::cout << " loading data success!" << std::endl;
	return true;
}


bool save()
{

	std::ofstream out_file;
	std::ostringstream tess_data{};
	std::ostringstream terrain_data{};
	std::ostringstream Camera_data{};
	std::ostringstream pointlight_data{};
	std::ostringstream dirlight_data{};
	std::ostringstream fog_data{};
	std::ostringstream water_data{};
	std::ostringstream quad_data{};
	std::ostringstream sun_data{};
	std::ostringstream lensflare_data{};
	;


	out_file.open("data.txt", std::ios::binary);


	if (!out_file) {
		std::cerr << "Problem opening file" << std::endl;
		return false;
	}
	// ------------------------ terran---------------------------------//
	tess_data << std::setw(20) << std::left << "tesselation"
		<< std::setw(10) << tess_level.x
		<< std::setw(10) << tess_level.y;

	out_file << tess_data.str() << std::endl;

	// ------------------------ terran---------------------------------//
	terrain_data << std::setw(20) << std::left << "terrain"
				<< std::setw(10) << tP.height_level
				<< std::setw(10) << tP.seed
				<< std::setw(10) << tP.smooth
				<< std::setw(10) << tP.roughness
				<< std::setw(10) << tP.frequency
				<< std::setw(10) << tP.peak_color.x
				<< std::setw(10) << tP.peak_color.y
				<< std::setw(10) << tP.peak_color.z
				<< std::setw(10) << tP.highpart_color.x
				<< std::setw(10) << tP.highpart_color.y
				<< std::setw(10) << tP.highpart_color.z
				<< std::setw(10) << tP.high_height
				<< std::setw(10) << tP.middlepart_color.x
				<< std::setw(10) << tP.middlepart_color.y
				<< std::setw(10) << tP.middlepart_color.z
				<< std::setw(10) << tP.middle_height
				<< std::setw(10) << tP.underwater_color.x
				<< std::setw(10) << tP.underwater_color.y
				<< std::setw(10) << tP.underwater_color.z
				<< std::setw(10) << tP.under_height
			    << std::setw(10) << tP.peak_height;
	out_file << terrain_data.str() << std::endl;

	// ------------------------ camera---------------------------------//
	Camera_data  << std::setw(20) << std::left << "camera"
			<< std::setw(10) << mainC.start_position.x
			<< std::setw(10) << mainC.start_position.y
			<< std::setw(10) << mainC.start_position.z
			<< std::setw(10) << mainC.start_world_up.x
			<< std::setw(10) << mainC.start_world_up.y
			<< std::setw(10) << mainC.start_world_up.z
			<< std::setw(10) << mainC.start_pitch
			<< std::setw(10) << mainC.start_yaw
			<< std::setw(10) << mainC.start_key_speed
			<< std::setw(10) << mainC.start_mouse_speed;
	out_file << Camera_data.str() << std::endl;


	//------------------------pointlight--------------------------------//

	pointlight_data  << std::setw(20) << std::left << "pointlight"
				<< std::setw(10) << p.Ka
				<< std::setw(10) << p.Kd
				<< std::setw(10) << p.Ks
				<< std::setw(10) << p.La.x
				<< std::setw(10) << p.La.y
				<< std::setw(10) << p.La.z
				<< std::setw(10) << p.Ld.x
				<< std::setw(10) << p.Ld.y
				<< std::setw(10) << p.Ld.z
				<< std::setw(10) << p.Ls.x
				<< std::setw(10) << p.Ls.y
				<< std::setw(10) << p.Ls.z
				<< std::setw(10) << p.lightPos.x
				<< std::setw(10) << p.lightPos.y
				<< std::setw(10) << p.lightPos.z;
	out_file << pointlight_data.str() << std::endl;


	//------------------------dirlight--------------------------------//

	dirlight_data << std::setw(20) << std::left << "dirlight"
		<< std::setw(10) << dl.Ka
		<< std::setw(10) << dl.La.x
		<< std::setw(10) << dl.La.y
		<< std::setw(10) << dl.La.z
		<< std::setw(10) << dl.Kd
		<< std::setw(10) << dl.Ld.x
		<< std::setw(10) << dl.Ld.y
		<< std::setw(10) << dl.Ld.z
		<< std::setw(10) << dl.dir.x
		<< std::setw(10) << dl.dir.y
		<< std::setw(10) << dl.dir.z;

	out_file << dirlight_data.str() << std::endl;


 // ------------------------fog --------------------------------// 
	fog_data << std::setw(20) << std::left << "fog"
		<< std::setw(10) << f.density
		<< std::setw(10) << f.gradient
		<< std::setw(10) << f.fog_color.x
		<< std::setw(10) << f.fog_color.y
		<< std::setw(10) << f.fog_color.z;
	out_file << fog_data.str() << std::endl;

// ------------------------water --------------------------------//
	water_data << std::setw(20) << std::left << "water"
		<< std::setw(10) << wP.water_height
		<< std::setw(10) << wP.wave_strength
		<< std::setw(10) << wP.wave_speed
		<< std::setw(10) << wP.water_color.x
		<< std::setw(10) << wP.water_color.y
		<< std::setw(10) << wP.water_color.z
		<< std::setw(10) << wP.water_color_mix
		<< std::setw(10) << wP.shineness
		<< std::setw(10) << wP.wave_length
		<< std::setw(10) << wP.wave_amplitude
		<< std::setw(10) << wP.edge_softness
		<< std::setw(10) << wP.glitch_offset
		<< std::setw(10) << wP.minBlue
		<< std::setw(10) << wP.maxBlue
		<< std::setw(10) << wP.murkyDepth;
	out_file << water_data.str() << std::endl;
// ------------------------quad --------------------------------//	

	quad_data << std::setw(20) << std::left << "quad"
		<< std::setw(10) << qP.N
		<< std::setw(10) << qP.repeat;
	out_file << quad_data.str() << std::endl;

	// ------------------------sun --------------------------------//	
	sun_data << std::setw(20) << std::left << "sun"
		<< std::setw(10) << sP.sun_pos.x
		<< std::setw(10) << sP.sun_pos.y
		<< std::setw(10) << sP.sun_pos.z
		<< std::setw(10) << sP.sun_scale.x
		<< std::setw(10) << sP.sun_scale.y;

	out_file << sun_data.str() << std::endl;

	// ------------------------lens flare ----------------------------//	
	lensflare_data << std::setw(20) << std::left << "lens_flare"
		<< std::setw(10) << lP.strength;
	for (int i = 0; i < 9; i++)
	{
		lensflare_data << std::setw(10) << lP.scale[i];
	}
	lensflare_data << std::setw(10) << lP.fadefactor;

	out_file << lensflare_data.str() << std::endl;

	//--------------------------mesh class----------------------------//
	


	out_file.close();
	std::cout << " save successfully!" << std::endl;

	return true;

}





