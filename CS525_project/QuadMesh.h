#ifndef  __QUADMESH_H__
#define  __QUADMESH_H__

#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <vector>

#define POS_LOC 0

using namespace std;

class QuadMesh
{
public:

	QuadMesh();
	QuadMesh(int N);

	void render();

private:

	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	int N;

	vector<float> vertices;
	vector<int>   indices;    //index to tell opengl to draw next triangle

	void init_vertices();
	void init_indices();
	void init_vbo();
	void init_vao();
	void init();

	int get_restart_index();
	int get_indices_num();
	int get_vertices_num();


};

#endif 