#ifndef __WATER_H__
#define __WATER_H__



#include <windows.h>
#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <vector>

#include "structs.h"
#include "location.h"
#include "InitShader.h"
#include "camera.h"


class water
{

public:
	pointLight p;
	fog f;
	waterP w;
	quadP q;
	terrainP tP;

	void init();
	water();
	water(float clip_distance, pointLight *p, fog *f, waterP *w, quadP *q, camera depthCam,terrainP *tP);
	void update(float clip_distance,pointLight *p, fog *f, waterP *w, quadP *q, terrainP *tP);
	
	void draw_water(int shading_mode,bool test, glm::mat4 M, glm::mat4 V, glm::mat4 P, GLuint waterTextureID[], int pass, float time, glm::vec3 cameraPos);
	void reload_shader();
	float get_clipDistance();
	glm::vec2 get_terrain_wh();

	~water();



private:

	//need to intitialize



	GLuint vao;
	GLuint shader;
	std::string vertex_shader;
	std::string fragment_shader; 
	std::string geometry_shader;


	int NUM_VERTICES;
	int t;
	int restart_index;
	int NUM_INDEX;

	float clip_distance;
	glm::mat4 depthView;

	std::vector<int> water_index;

	const float PI = 3.1415926;
	glm::vec2 texCoord(int i, int j);
	GLuint create_water_posvbo();
	glm::vec3 waterPos(int i, int j);
	glm::vec3 normal(int i, int j);
	GLuint create_water_vao();

	void initVao();
	void initShader();

};



#endif 


