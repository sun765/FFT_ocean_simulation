#ifndef __VAO_H__
#define __VAO_H__



#include <windows.h>
#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "location.h"

class VAO
{
public:

	VAO();
	VAO(int frequency, int repeat, int vertices, float height_level, int seed, float smooth, float roughness);
	
	GLuint create_terrain_vao();
	GLuint create_terrain_posvbo();
	void draw_terrain(GLuint vao, GLuint shader, glm::mat4 M, glm::mat4 V, glm::mat4 P, GLuint texture_id[], float height_index, int pass, glm::vec4 plane
		             
					, float density, float gradient, glm::vec3 fog_color,glm::vec3 camPos);// fog

	GLuint create_water_vao();
	GLuint create_water_posvbo();
	void draw_water(GLuint vao, GLuint shader, glm::mat4 M, glm::mat4 V, glm::mat4 P,
		GLuint waterTextureID[], int pass, float wave_strength, float wave_speed, float time, glm::vec3 cameraPos,
		float Ks, glm::vec3 Ls, float shinenss, glm::vec3 lightPos, glm::vec3 water_color, float waterColorMix
		, float density, float gradient, glm::vec3 fog_color);

	GLuint create_cube_vao();
	GLuint create_cube_posvbo();
	void draw_cube(GLuint vao, GLuint shader, GLuint cubemap_id, glm::mat4 V, glm::mat4 P, int pass);

	glm::vec2 get_terrain_wh();
	~VAO();



private:

	int repeat = 3;
	int N = 100;   // why this has to be static const?
	int NUM_VERTICES;
	int t;
	int restart_index;
	int NUM_INDEX;
	//static const int Num_INDEX = 0;
	std::vector<int> index;
	std::vector<int> water_index;
	float height_level;
	int seed;
	float smooth;
	float roughness;
	int frequency;
	const float PI = 3.1415926;

	 float noise(int i, int j);
	 float cosInterpolate(float a, float b, float blend);
	 float smoothHeight(int i, int j);
	 float interpolatedHeight(float x, float y);
	 glm::vec2 texCoord(int i, int j);

	// blend 0 -->1   , a-->b ;

	 glm::vec3 terrain(int x, int y);
	 glm::vec3 water(int i, int j);
	 glm::vec3 normal(int i, int j);
};



#endif 


