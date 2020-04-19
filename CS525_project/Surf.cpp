#include <vector>
#include "Surf.h"
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


static const int N = 100;   // why this has to be static const?
static const int NUM_VERTICES = N * N;
int t = 0;
int restart_index = N * N * 10;
static const int NUM_INDEX = (N - 1)*(2 * N + 1);
//static const int Num_INDEX = 0;
std::vector<int> index;
//float angle = 0.0f;

//Declare a vector to hold N*N vertices

//The sinc surfacev


static glm::vec3 surf(int i, int j)
{
	const float center = 0.5f*N;	
	const float xy_scale = 20.0f / N;
	const float z_scale = 10.0f;

	float x = xy_scale * (i - center);
	float y = xy_scale * (j - center);

	float r = sqrt(x*x + y * y);
	float z = 1.0f;

	if (r != 0.0f)
	{
		z = sin(r) / r;
	}
	z = z * z_scale;

	return 0.05f*glm::vec3(x, y, z);
}


//Sinc surface normals
static glm::vec3 normal(int i, int j)
{
	glm::vec3 du = surf(i + 1, j) - surf(i - 1, j);
	glm::vec3 dv = surf(i, j + 1) - surf(i, j - 1);
	return glm::normalize(glm::cross(du, dv));
}

GLuint create_surf_posvbo()
{
	std::vector<glm::vec3> surf_verts;
	surf_verts.reserve(2*NUM_VERTICES);
	// create point position and index
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			surf_verts.push_back(surf(i,j));
			surf_verts.push_back(normal(i, j));
			//surf_verts.push_back(glm::vec2(float(i / N), float(j / N));
		}
	}

	for (int i = 0; i < NUM_INDEX; i++)
	{	
		int tail = i % (N * 2 + 1);
		int a = tail % 2;
		int row = (i+1) / (N * 2+1) + 1;
		//i < 2n +1;
			if ((i + 1) % (2 * N + 1) == 0)
			{
				index.push_back (restart_index) ;
			}
			else if (a == 0)
			{
				index.push_back(N*(row - 1) + tail / 2);
			}
			
			else {
				index.push_back(N *(row)+tail / 2);
			}

			
	}
	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*index.size() , index.data(), GL_STATIC_DRAW);

	GLuint position_buffer;
	glGenBuffers(1, &position_buffer); //Generate vbo to hold vertex attributes for triangle.
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer); //Specify the buffer where vertex attribute data is stored.
	//Upload from main memory to gpu memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*surf_verts.size(), surf_verts.data(), GL_STATIC_DRAW);//change ;

	// create color matrix;

	//buffer data?

	return position_buffer;// why doing this ?
}

GLuint create_surf_colorvbo()
{
	std::vector<glm::vec4> color;
	color.reserve(9);
	for (int i = 0; i < 9; i++)
	{
		color.push_back(glm::vec4(float((i/3)*0.67), float((i / 3)*0.67), float((i / 3)*0.67), 1.0));
	}
	GLuint color_buffer;
	const int color_loc = 9;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*color.size(), color.data(), GL_STATIC_DRAW);

	return color_buffer;
}

GLuint create_surf_vao()
{
	GLuint vao;

	//Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenVertexArrays(1, &vao);

	//Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
	glBindVertexArray(vao);

	GLuint vbo = create_surf_posvbo();
	const GLint pos_loc = 0;
	const GLint normal_loc = 14;
	glEnableVertexAttribArray(pos_loc); //Enable the position attribute.
	glEnableVertexAttribArray(normal_loc);
	//Tell opengl how to get the attribute values out of the vbo (stride and offset).
	//In this case, the vertices are at the beginning of the VBO and are tightly packed.
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 6 * sizeof(float), 0);// change 
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	GLuint color_buffer = create_surf_colorvbo();
	const GLint color_loc = 9;
	glEnableVertexAttribArray(color_loc);
	glVertexAttribPointer(color_loc, 4, GL_FLOAT,false , 4*sizeof(float), NULL);
	glVertexAttribDivisor(color_loc, 1);


	GLuint matrix_buffer = create_instanced_matrix();
	const GLint matrix_loc = 10;
	for (int i = 0; i < 4; i++)
	{
		// Set up the vertex attribute
		glVertexAttribPointer(matrix_loc + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * i)); // Start offset
		glEnableVertexAttribArray(matrix_loc + i);
		glVertexAttribDivisor(matrix_loc + i, 1);
	}
	//matrix attribute pointer here
	glBindVertexArray(0);

	return vao;
}

GLuint create_instanced_matrix()
{
	std::vector<glm::mat4> instanced_model;
	instanced_model.reserve(9);
	for (int i = 0; i < 9; i++)
	{
		glm::mat4 T = glm::translate(glm::vec3(float(i / 3 - 1), float(i % 3 - 1), 0.0));
		instanced_model.push_back(T);
	}
	
	GLuint matrix_buffer;
	const int matrix_loc = 10;
	glGenBuffers(1, &matrix_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, matrix_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*instanced_model.size(), instanced_model.data(), GL_STATIC_DRAW);// is this right?

	return matrix_buffer;
}

//Draw the set of points on the surface
void draw_surf(GLuint vao)
{
	glBindVertexArray(vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(restart_index);
	//glDrawElements(GL_TRIANGLE_STRIP, NUM_INDEX, GL_UNSIGNED_INT,NULL);
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, NUM_INDEX, GL_UNSIGNED_INT, NULL,9);
}