#include <vector>
#include "terrain.h"
#include <iostream>

 GLuint terrain::shader =-1;
 std::string terrain::vertex_shader ="Shaders/terrain_vs.glsl";
 std::string terrain::fragment_shader = "Shaders/terrain_fs.glsl";
 std::string terrain::geometry_shader = "Shaders/terrain_gs.glsl";

 GLuint terrain::layered_shader = -1;
 std::string terrain::layered_geometry_shader = "Shaders/terrain_lgs.glsl";
 std::string terrain::layered_vertex_shader = "Shaders/terrain_lvs.glsl";
 std::string terrain::layered_fragment_shader = "Shaders/terrain_lfs.glsl";

terrain::terrain()
{

}
terrain::terrain(float clipDistance, struct terrainP *tP, struct quadP *qP, directionalLight *dl, struct fog *f,camera depth_cam)
{
	
	update(clipDistance,tP, qP, f,dl);
	int N = qP->N;
	NUM_VERTICES = N * N;
	t = 0;
	restart_index = N * N * 10;
	NUM_INDEX = (N - 1)*(2 * N + 1);
	init();
	this->depth_view = depth_cam.get_view_matrice();
	R = glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	T = glm::translate(glm::vec3(-scale*N / 2.0, 0.0, -scale * N / 2.0));
	S = glm::scale(glm::vec3(scale));
	this->M = R *  T*S;
	
}

// procedual generated terrain helper functions
float terrain::noise(int i, int j)
{

	//srand(i * 2362232332 + j * 6425565173 + 565641*tP.seed);
	srand(i * 2643 + j * 6654 +  tP.seed);
	float random = ((float)rand()) / (float)RAND_MAX;
	return 2.0 * random - 1;
}

float terrain::cosInterpolate(float a, float b, float blend)
{
	float theta = 1.0 * PI*blend;
	float f = float(1.0 - cos(theta))*0.5;
	return b * f + a * (1.0 - f);
}

float terrain::smoothHeight(int i, int j)
{
	float corner = (noise(i - 1, j - 1) + noise(i + 1, j + 1) + noise(i - 1, j + 1) + noise(i + 1, j - 1)) / 16.0;
	float neighboor = (noise(i - 1, j) + noise(i + 1, j) + noise(i, j - 1), noise(i, j + 1)) / 8.0;
	float center = noise(i, j) / 4.0;

	return tP.height_level * (corner + neighboor + center);
}

float terrain::interpolatedHeight(float x, float y)
{
	int intX = int(x);
	int intY = int(y);
	float fractX = x - intX;
	float fractY = y - intY;
	float n1 = smoothHeight(intX, intY);
	float n2 = smoothHeight(intX, intY + 1);
	float n3 = smoothHeight(intX + 1, intY + 1);
	float n4 = smoothHeight(intX + 1, intY);
	float l1 = cosInterpolate(n2, n3, fractX);
	float l2 = cosInterpolate(n1, n4, fractX);
	float l = cosInterpolate(l2, l1, fractY);
	return l;

}

glm::vec2 terrain::texCoord(int i, int j)
{
	return glm::vec2(qP.repeat*float(i) / float(qP.N - 1), qP.repeat*float(j) / float(qP.N - 1));
}

// blend 0 -->1   , a-->b ;

glm::vec3 terrain::terrainPos(int x, int z)
{
	float total_height = 0;
	for (int i = 0; i < tP.frequency; i++)
	{
		float frequency_factor = pow(2, tP.frequency - 1 - i);
		float level_factor = pow(tP.roughness, i);
		float h = interpolatedHeight(float(x) / frequency_factor, float(z) / frequency_factor)*level_factor;
		total_height += h;
	}

	//return glm::vec3(float(i) / (smooth), h, float(j) / (smooth));
	return glm::vec3(float(x), total_height, float(z));
	//std::cout << h << std::endl;
}



glm::vec3 terrain::normal(int i, int j)
{
	glm::vec3 du = terrainPos(i + 1, j) - terrainPos(i - 1, j);
	glm::vec3 dv = terrainPos(i, j + 1) - terrainPos(i, j - 1);
	return glm::normalize(glm::cross(du, dv));
}



//--------------------------terrain---------------------------------------//

glm::vec2 terrain::get_terrain_wh()
{
	return glm::vec2(float(scale*qP.N), float(scale*qP.N));
}

GLuint terrain::create_terrain_vao()
{
	GLuint vao;

	//Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenVertexArrays(1, &vao);

	//Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
	glBindVertexArray(vao);

	GLuint vbo = create_terrain_posvbo();
	glEnableVertexAttribArray(AttribLoc::Pos_loc); //Enable the position attribute.
	glEnableVertexAttribArray(AttribLoc::TexCoord_loc);
	glEnableVertexAttribArray(AttribLoc::Normal_loc);
	//Tell opengl how to get the attribute values out of the vbo (stride and offset).
	//In this case, the vertices are at the beginning of the VBO and are tightly packed.
	glVertexAttribPointer(AttribLoc::Pos_loc, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glVertexAttribPointer(AttribLoc::TexCoord_loc, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));// change 
	glVertexAttribPointer(AttribLoc::Normal_loc, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));// change 

	//matrix attribute pointer here
	glBindVertexArray(0);

	return vao;
}

GLuint terrain::create_terrain_posvbo()
{
	int N = qP.N;
	
	terrain_verts.reserve(8 * NUM_VERTICES);
	// create point position and index
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			terrain_verts.push_back(terrainPos(i, j).x);
			terrain_verts.push_back(terrainPos(i, j).y);
			terrain_verts.push_back(terrainPos(i, j).z);
			terrain_verts.push_back(texCoord(i, j).x);
			terrain_verts.push_back(texCoord(i, j).y);
			terrain_verts.push_back(normal(i, j).x);
			terrain_verts.push_back(normal(i, j).y);
			terrain_verts.push_back(normal(i, j).z);
	
		}
	}

	for (int i = 0; i < NUM_INDEX; i++)
	{
		int tail = i % (N * 2 + 1);
		int a = tail % 2;
		int row = (i + 1) / (N * 2 + 1) + 1;
		//i < 2n +1;
		if ((i + 1) % (2 * N + 1) == 0)
		{
			index.push_back(restart_index);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*index.size(), index.data(), GL_STATIC_DRAW);

	GLuint position_buffer;
	glGenBuffers(1, &position_buffer); //Generate vbo to hold vertex attributes for triangle.
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer); //Specify the buffer where vertex attribute data is stored.
	//Upload from main memory to gpu memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*terrain_verts.size(), terrain_verts.data(), GL_STATIC_DRAW);

	return position_buffer;// why doing this ?
}

void terrain::draw_terrain(int shading_mode,  glm::mat4 M, glm::mat4 V, glm::mat4 P, GLuint texture_id[],  int pass, glm::vec4 plane, glm::vec3 camPos)
{

		glUseProgram(shader);
		glUniform1i(UniformLoc::pass_loc, pass);
		glUniform4f(UniformLoc::plane_loc, plane.x, plane.y, plane.z, plane.a);
		glUniformMatrix4fv(UniformLoc::M_loc, 1, false, glm::value_ptr(this->M));
		glUniformMatrix4fv(UniformLoc::V_loc, 1, false, glm::value_ptr(V));
		glUniformMatrix4fv(UniformLoc::P_loc, 1, false, glm::value_ptr(P));
		glUniformMatrix4fv(UniformLoc::depthView_loc, 1, false, glm::value_ptr(depth_view));

		// terrain color
		glUniform3f(UniformLoc::peakcolor_loc, tP.peak_color.x, tP.peak_color.y, tP.peak_color.z);
		glUniform3f(UniformLoc::highpartcolor_loc, tP.highpart_color.x, tP.highpart_color.y, tP.highpart_color.z);
		glUniform1f(UniformLoc::highheight_loc, tP.high_height);
		glUniform3f(UniformLoc::middlepartcolor_loc, tP.middlepart_color.x, tP.middlepart_color.y, tP.middlepart_color.z);
		glUniform1f(UniformLoc::middleheight_loc, tP.middle_height);
		glUniform3f(UniformLoc::underwatercolor_loc, tP.underwater_color.x, tP.underwater_color.y, tP.underwater_color.z);
		glUniform1f(UniformLoc::terrainheight_loc, tP.height_level*scale);
		glUniform1f(UniformLoc::peakheight_loc, tP.peak_height);
		glUniform1f(UniformLoc::underheight_loc, tP.under_height);

		//fog
		glUniform1f(UniformLoc::density_loc, f.density);
		glUniform1f(UniformLoc::gradient_loc, f.gradient);
		glUniform3f(UniformLoc::fogColor_loc, f.fog_color.x, f.fog_color.y, f.fog_color.z);
		glUniform3f(UniformLoc::cameraPos_loc, camPos.x, camPos.y, camPos.z);

		glUniform1f(UniformLoc::height_index_loc, tP.height_level);
		glUniform1i(UniformLoc::shadingMode_loc, shading_mode);
		glUniform1f(UniformLoc::clipDistance_loc, this->clipDistance);

		glUniform1i(UniformLoc::texture_loc[0], 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id[0]);

		glUniform1i(UniformLoc::texture_loc[1], 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_id[1]);

		glUniform1i(UniformLoc::texture_loc[2], 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture_id[2]);

		glUniform1i(UniformLoc::texture_loc[3], 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture_id[3]);

		glUniform1i(UniformLoc::texture_loc[4], 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, texture_id[4]);

		glUniform1i(UniformLoc::texture_loc[5], 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, texture_id[5]);


		//direction light
		glUniform3f(UniformLoc::dir_loc, dl.dir.x, dl.dir.y, dl.dir.z);
		glUniform1f(UniformLoc::dka_loc, dl.Ka);
		glUniform1f(UniformLoc::dkd_loc, dl.Kd);
		glUniform3f(UniformLoc::dLa_loc, dl.La.x, dl.La.y, dl.La.z);
		glUniform3f(UniformLoc::dLd_loc, dl.Ld.x, dl.Ld.y, dl.Ld.z);

		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(this->vao);
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(restart_index);
		glDrawElementsInstanced(GL_TRIANGLE_STRIP, NUM_INDEX, GL_UNSIGNED_INT, NULL, 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUseProgram(0);
	}
	


void terrain::initVao()
{
	this->vao = create_terrain_vao();
}

void terrain::initShader()
{
	shader = InitShader(vertex_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
}

void terrain::update(float clipDistance,terrainP * tP, quadP * qP, fog * f,directionalLight *dl)
{
	this->clipDistance = clipDistance;
	this->tP = *tP;
	this->qP = *qP;
	this->f = *f;
	this->dl = *dl;
}

void terrain::init()
{
	initVao();
}

void terrain::reload_shader()
{
	GLuint new_shader = InitShader(vertex_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
	GLuint new_layered_shader = InitShader(vertex_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
	if (new_shader == -1) // loading failed
	{
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	}
	else
	{
		if (shader != -1)
		{
			glDeleteProgram(shader);
		}
		//if (layered_shader != -1)
		//{
		//	glDeleteProgram(layered_shader);
		//}
		shader = new_shader;
		layered_shader = new_layered_shader;
	}
}

float terrain::get_height(int x, int z)
{
	if (!inTerrainRange(x, z))
	{
		// protect the vector.
		return 10000;
	}
	int N = qP.N;
	int indexX = x + N / 2;
	int indexZ = z + N / 2;
	int index = indexX *N + indexZ;
	int vector_index = index * 8 + 1;
	return terrain_verts.at(vector_index);
}

terrain::~terrain()
{
}

bool terrain::inTerrainRange(int x, int z)
{
	int N = this->qP.N;
	if (x > -N * 0.5&&x < N*0.5&&z>-N * 0.5&&z < N*0.5)
	{
		return true;
	}

	else
	{
		return false;
	}

}

float terrain::get_scale()
{
	return 10.0 ;
}

glm::mat4 terrain::get_M()
{
	return this->M;
}

glm::vec3 terrain::get_transform()
{
	int N = this->qP.N;
	return glm::vec3(-scale * N / 2.0, 0.0, -scale * N / 2.0);
}