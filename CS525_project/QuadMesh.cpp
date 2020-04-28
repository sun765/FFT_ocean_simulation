#include "QuadMesh.h"

QuadMesh::QuadMesh()
{
	this->N = 0;
	this->vao = -1;
	this->vbo = -1;

}

QuadMesh::QuadMesh(int N)
{
	this->N = N;
	this->vao = -1;
	this->vbo = -1;

	this->init();
}

void QuadMesh::render()
{
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(this->vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(this->get_restart_index());
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, this->get_indices_num(), GL_UNSIGNED_INT, NULL, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void QuadMesh::init_vbo()
{
	glGenBuffers(1, &this->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * this->indices.size(), this->indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &this->vbo); //Generate vbo to hold vertex attributes for triangle.
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo); //Specify the buffer where vertex attribute data is stored.
	//Upload from main memory to gpu memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);

}

void QuadMesh::init_vao()
{

	//Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenVertexArrays(1, &this->vao);

	//Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
	glBindVertexArray(this->vao);

	glEnableVertexAttribArray(POS_LOC); //Enable the position attribute.

	//Tell opengl how to get the attribute values out of the vbo (stride and offset).
	//In this case, the vertices are at the beginning of the VBO and are tightly packed.
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, false, 3 * sizeof(float), 0);

	//matrix attribute pointer here
	glBindVertexArray(0);

}

void QuadMesh::init()
{
	this->init_vertices();
	this->init_indices();
	this->init_vbo();
	this->init_vao();
}

int QuadMesh::get_restart_index()
{
	return  N * N * 10;
}

int QuadMesh::get_indices_num()
{
	return (this->N - 1)* (2 * this->N + 1);
}

void QuadMesh::init_vertices()
{
	int vertices_num = this->N * this->N;
	for (int x = 0; x < N; x++)
	{
		for (int z = 0; z < N; z++)
		{
			this->vertices.push_back(float(x)); // x
			this->vertices.push_back(0.0);      // y 
			this->vertices.push_back(float(z)); // z
		}
	}
}

void QuadMesh::init_indices()
{
	int indices_num   = this->get_indices_num();
	int restart_index = this->get_restart_index();

	for (int i = 0; i < indices_num; i++)
	{
		int tail = i % (this->N * 2 + 1);
		int a = tail % 2;
		int row = (i + 1) / (this->N * 2 + 1) + 1;
		//i < 2n +1;
		if ((i + 1) % (2 * this->N + 1) == 0)
		{
			indices.push_back(restart_index);
		}
		else if (a == 0)
		{
			indices.push_back(this->N * (row - 1) + tail / 2);
		}

		else {
			indices.push_back(this->N * (row)+tail / 2);
		}
	}
}
