// this class must be changed to inheirtance later
#ifndef __LAYEREDRENDERINGMESH_H__
#define __LAYEREDRENDERINGMESH_H__

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


#include "camera.h"
class layeredRenderingMesh
{
public:
	layeredRenderingMesh();
	layeredRenderingMesh(std::string mesh_name, int meshId, fog *f, pointLight *p, transform *t);
	~layeredRenderingMesh();
	GLuint cubemap_id;
	void update(fog *f, pointLight *p, transform *t,GLuint cubemap_id);
	void layered_rendering(int shading_mode, int currentID, glm::mat4 V, glm::mat4 P, glm::vec4 plane, glm::vec3 camPos, int Pass);
	glm::vec3 get_world_pos();
	// static members for all objects


	static void reload_shader();
	static void init_shader();


private:

	camera cubemap_cameras[6];
	cameraP cubemap_cameraP[6];
	static glm::vec3 fronts[6];

	void update_cameras();
	void update_cameraParameters();


	// the same as mesh
	MeshData mesh_data;
	std::string mesh_name;

	int meshID;
	int currentID;

	glm::vec3 test;
	fog f;
	pointLight p;
	transform t;
	void initMesh();

	static int mesh_nums;
	static GLuint shader;
	static std::string vertex_shader;
	static std::string fragment_shader;
	static std::string geometry_shader;

};

#endif




