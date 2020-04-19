#ifndef __MESH_H__
#define __MESH_H__



#include <windows.h>
#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <iostream>


#include "location.h"
#include "LoadMesh.h"
#include "structs.h"
#include "InitShader.h"

class mesh
{
public:
	mesh();
	mesh(std::string mesh_name,int meshId, fog *f, pointLight *p,directionalLight *dl, transform *t, glm::vec2 level);
	~mesh();

	void update(fog *f, pointLight *p, directionalLight *dl, transform *t);
	void draw_mesh(int shading_mode, int currentID, glm::mat4 V, glm::mat4 P, glm::vec4 plane, glm::vec3 camPos, int Pass, directionalLight *dl, fog *f, bool mode, glm::vec2 level, std::vector<glm::vec3> sub_color,float time,transform *t);
	glm::vec3 get_world_pos();
	// static members for all objects


	static void reloadMeshShader();
	static void init_shader();
	static void update_light(directionalLight *dl);
	void test_pos();
	glm::mat4 get_M();
	int get_submesh_num();
	bool hight_light = false;
	//static void test();

private:

	//int submesh_num;
	
	int sub_mesh_num;
	MeshData mesh_data;
	std::string mesh_name;


	glm::mat4 M;
	glm::vec2 level;
	int meshID;
	int currentID;

	glm::vec3 test;
	fog f;
	pointLight p;
	transform t;
	directionalLight dl;
	void initMesh();

	static int mesh_nums;
	static GLuint shader;
	static std::string vertex_shader;
	static std::string tessellation_control_shader;
	static std::string tessellation_evaluation_shader;
	static std::string fragment_shader;
	static std::string geometry_shader;




	// static members for all objects
	


};
#endif

