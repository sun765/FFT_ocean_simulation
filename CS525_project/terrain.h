#ifndef __TERRAIN_H__
#define __TERRAIN_H__



#include <windows.h>
#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <vector>

#include "location.h"
#include "structs.h"
#include "InitShader.h"
#include "camera.h"


class terrain
{
public:

	terrainP tP;
	quadP    qP;
	fog       f;
	directionalLight dl;


	terrain();
	terrain(float clipDistance,struct terrainP *tP, struct quadP *qP, directionalLight* dl,struct fog *f,camera depthCam);
	void update(float clipDistance,terrainP *tP, quadP *qP, fog *f, directionalLight *dl);

	GLuint create_terrain_vao();
	GLuint create_terrain_posvbo();
	void draw_terrain( int shading_mode, glm::mat4 M, glm::mat4 V, glm::mat4 P, GLuint texture_id[], int pass, glm::vec4 plane, glm::vec3 camPos);// fog
	//void update(directionalLight *dl);

	void init();
	static void reload_shader();
	static void initShader();
	glm::vec2 get_terrain_wh();
	float get_height(int x, int z);
	float get_scale();
	glm::mat4 get_M();
	glm::vec3 get_transform();
	~terrain();

private:
	
	glm::mat4 R ;
	glm::mat4 T ;
	glm::mat4 S ;
	glm::mat4 M;
	float scale = 10.0;

	GLuint vao;
	static GLuint shader;
	std::vector<float> terrain_verts;
	
	
	static std::string vertex_shader;
	static std::string fragment_shader;
	static std::string geometry_shader;

	static GLuint layered_shader;
	static std::string layered_vertex_shader;
	static std::string layered_geometry_shader;
	static std::string layered_fragment_shader;
	

	//shadered for layered rendering


	const float PI = 3.1415926;
	int NUM_VERTICES;
	int t;
	int restart_index;
	int NUM_INDEX;
	float clipDistance;
	//static const int Num_INDEX = 0;
	std::vector<int> index;
	std::vector<int> water_index;
	glm::mat4 depth_view;

	float noise(int i, int j);
	float cosInterpolate(float a, float b, float blend);
	float smoothHeight(int i, int j);
	float interpolatedHeight(float x, float y);
	glm::vec2 texCoord(int i, int j);
	// blend 0 -->1   , a-->b ;
	glm::vec3 terrainPos(int x, int y);
	glm::vec3 normal(int i, int j);

	bool inTerrainRange(int x, int z);

	void initVao();




	
	//void update();


};


#endif
