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
#include "terrain.h"
#include "water.h"
#include "structs.h"
#include "skybox.h"
#include "mesh.h"
#include "gui.h"

//----------------------------------------------------------hard code data-----------------------------------------------------------------------------//
static const std::string vertex_shader[]{ "Shaders/terrain_vs.glsl" ,"Shaders/water_vs.glsl","Shaders/skybox_vs.glsl","Shaders/mesh_vs.glsl" };
static const std::string fragment_shader[]{ "Shaders/terrain_fs.glsl","Shaders/water_fs.glsl","Shaders/skybox_fs.glsl","Shaders/mesh_fs.glsl" };

GLuint terrain_shader = -1;
GLuint water_shader = -1;
GLuint skybox_shader = -1;
GLuint mesh_shader = -1;


GLuint texture_id[] = { -1,-1,-1,-1,-1,-1 }; 

GLuint skybox_id = -1;

//names of the mesh and texture files to load
int texture_num = sizeof(texture_id)/sizeof(texture_id[0]);


//static const std::string mesh_name = "Amago0.obj";
static const std::string texture_name[] = { "Textures/blend.png",
											"Textures/dirt.png",
											"Textures/flower.png",
											"Textures/grass.png",
											"Textures/path.png" ,
											"Textures/heightmap.png" };
static const std::string skybox_name = "Textures/skybox";
static const std::string DuDv = "Textures/waterDUDV.png";
static const std::string water_normal = "Textures/matchingNormalMap.png";

float height_index = 0.1;

//test mesh
static const std::string mesh_name = "Meshes/palm1.obj";

float time_sec = 0.0f;
float time_ms = 0.0f;
float angle = 0.0f;
bool recording = false;
int precise = 100;


//FBO and water  , don't change

GLuint fbo = -1;
GLuint rbo = -1;
GLuint texture_width = 1920;
GLuint texture_height = 1080;
GLuint waterTexture_id[] = { -1,-1,-1,-1 };  // 0 is reflection 1 refraction  2 is dudv MAP   3 water normal




// window width and height
int w ,h;

//tes
bool renderScene = true;

// pause
bool pause = false;

water* main_water;
terrain* main_terrain ;
skybox* main_sky ;

std::vector<mesh*> mesh_list;
int tree_nums = 2;


// list of entities to render
std::vector<int> list;

// pick objects in the scene
GLuint pickTexture_id = -1;
int mesh_nums = 0;
int currentId = 0;
int lastID;

//---------------------------------------------------------------data to be saved and loaded-----------------------------------------------------------------------------//
// terrain
int random[] = {23632,6425176};
float terrain_height;

//camera
cameraP mainC =
{
	glm::vec3(0.0f, 1000.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	0.0f,
	0.0f,
	50.0f,
	0.50f
};
camera main_camera;
camera reflect_camera;
glm::vec2 previous_campos;

terrainP tP
{
	30.0,      //height
	rand() % 1000000000, //seed
	16.0,   //smooth
	0.3,    //roughness
	4    //frequency
};

pointLight p =
{
	0.1,
	0.7,
	0.5,
	glm::vec3(1.0),
	glm::vec3(1.0),
	glm::vec3(0.7, 0.8, 0.5),  //Ls
	glm::vec3(-20.0, 10.0, 0.0)
};

fog f =
{
	 0.0,              //density
	 1.5,               //gradient
	 glm::vec3(0.7)    //fog color
};

waterP wP =
{
	0.15f,   // water height
	0.02,     // wave strength
	0.007,    //wave speed
	glm::vec3(0.0, 0.0, 0.3),    //water color
	0.2,      //water color mix
	20.0    //shiness
};

quadP qP
{
	100,
	3
};


transform t[] =
{
	{
	glm::vec3(0.0),
	glm::vec3(0.05),
	0.0,
	1
},
{
	glm::vec3(-20.0,0.0,20.0),
	glm::vec3(0.05),
	0.0,
	1
}
};


//water* main_water=new water(&p, &f, &wP, &qP);


bool load()
{

	std::ifstream in_file;

	std::string line;

	in_file.open("data.txt", std::ios::binary);
	if (!in_file)
	{
		std::cout << " loading data fail!" << std::endl;
		return false;
	}

	std::getline(in_file, line);
	std::istringstream iss{ line };
	std::string  header;
	iss >>header >> random[0] >> random[1]>> terrain_height;
	std::cout << std::setw(10) << std::left << random[0]
		<< std::setw(5) << random[1]
		<< std::setw(10) << terrain_height << std::endl;

	in_file.close();
	std::cout << " loading data success!" << std::endl;
	return true;
}


bool save()
{

	std::ofstream out_file;
	std::ostringstream oss{};

	out_file.open("data.txt", std::ios::binary);


	if (!out_file) {
		std::cerr << "Problem opening file" << std::endl;
		return false;
	}

	oss << std::setw(10) << std::left << "terrain"
		<< std::setw(10) << random[0]
		<< std::setw(10) << random[1]
		<< std::setw(10) << terrain_height << std::endl;
	std::cout << oss.str() << std::endl;
	out_file << oss.str() << std::endl;

	//out_file << oss.str() << std::endl;
	out_file.close();

	return true;

}





