#include <vector>
#include "VAO.h"
#include <iostream>

VAO::VAO()
{

}
VAO::VAO(int frequency,int repeat, int vertices, float height_level, int seed, float smooth, float roughness)
{
	this->repeat = repeat;
	this->N = vertices;
	this->height_level = height_level;
	this->seed = seed;
	this->smooth = smooth;
	this->roughness = roughness;
	this->frequency = frequency;
	NUM_VERTICES = N * N;
	t = 0;
	restart_index = N * N * 10;
	NUM_INDEX = (N - 1)*(2 * N + 1);

}


// procedual generated terrain helper functions
 float VAO::noise(int i, int j)
 {
	 
	 srand(i * 23632 + j * 6425176 + seed);
	float random = ((float)rand()) / (float)RAND_MAX;
	return 2.0 * random - 1;
 }

 float VAO::cosInterpolate(float a, float b, float blend)
 {
	 float theta = 1.0 * PI*blend;
	 float f = float(1.0 - cos(theta))*0.5;
	 return b * f + a * (1.0 - f);
 }

 float VAO::smoothHeight(int i, int j)
 {
	 float corner = (noise(i - 1, j - 1) + noise(i + 1, j + 1) + noise(i - 1, j + 1) + noise(i + 1, j - 1)) / 16.0;
	 float neighboor = (noise(i - 1, j) + noise(i + 1, j) + noise(i, j - 1), noise(i, j + 1)) / 8.0;
	 float center = noise(i, j) / 4.0;
	  
	 return height_level * (corner + neighboor + center);
 }

 float VAO::interpolatedHeight(float x, float y)
 {
	 int intX = int(x);
	 int intY = int(y);
	 float fractX = x - intX;
	 float fractY = y - intY;
	 float n1 = smoothHeight(intX, intY);
	 float n2 = smoothHeight(intX, intY+1);
	 float n3 = smoothHeight(intX+1, intY+1);
	 float n4 = smoothHeight(intX+1, intY);
	 float l1 = cosInterpolate(n2, n3, fractX);
	 float l2 = cosInterpolate(n1, n4, fractX);
	 float l = cosInterpolate(l2, l1, fractY);
	 return l;

 }

 glm::vec2 VAO::texCoord(int i, int j)
{
	return glm::vec2(repeat*float(i) / float(N-1), repeat*float(j) / float(N-1));
}

// blend 0 -->1   , a-->b ;

 glm::vec3 VAO::terrain(int x, int y)
{
	float total_height = 0;
	for (int i = 0; i < frequency; i++)
	{
		float frequency_factor = pow(2, frequency - 1 - i);
		float level_factor = pow(roughness, i);
		float h = interpolatedHeight(float(x) / frequency_factor, float(y) / frequency_factor)*level_factor;
		total_height += h;
	}
	
	//return glm::vec3(float(i) / (smooth), h, float(j) / (smooth));
	return glm::vec3(float(x), total_height, float(y));
	//std::cout << h << std::endl;
}

 glm::vec3 VAO::water(int i, int j)
{
	return glm::vec3(terrain(i,j).x, 0.0, terrain(i, j).z);
}

 glm::vec3 VAO::normal(int i, int j)
{
	glm::vec3 du = terrain(i + 1, j) - terrain(i - 1, j);
	glm::vec3 dv = terrain(i, j + 1) - terrain(i, j - 1);
	return glm::normalize(glm::cross(du, dv));
}



//--------------------------terrain---------------------------------------//

glm::vec2 VAO::get_terrain_wh()
{
	return glm::vec2(float(N),float(N));
}

GLuint VAO::create_terrain_vao()
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

GLuint VAO::create_terrain_posvbo()
{
	std::vector<float> terrain_verts;
	terrain_verts.reserve(8 * NUM_VERTICES);
	// create point position and index
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			terrain_verts.push_back(terrain(i, j).x);
			terrain_verts.push_back(terrain(i, j).y);
			terrain_verts.push_back(terrain(i, j).z);
			terrain_verts.push_back(texCoord(i, j).x);
			terrain_verts.push_back(texCoord(i, j).y);
			terrain_verts.push_back(normal(i,j).x);
			terrain_verts.push_back(normal(i, j).y);
			terrain_verts.push_back(normal(i, j).z);
			//std::cout << terrain(i, j).y << std::endl;
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

void VAO::draw_terrain(GLuint vao, GLuint shader, glm::mat4 M, glm::mat4 V, glm::mat4 P, GLuint texture_id[], float height_index,int pass, glm::vec4 plane
	                 , float density, float gradient, glm::vec3 fog_color,glm::vec3 camPos)
{
	//std::cout << seed << std::endl;
	height_level = height_index;
	glUseProgram(0);
	glUseProgram(shader);
	glUniform1i(UniformLoc::pass_loc, pass);
	glUniform4f(UniformLoc::plane_loc, plane.x, plane.y, plane.z, plane.a);
	glUniformMatrix4fv(UniformLoc::M_loc, 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(UniformLoc::V_loc, 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(UniformLoc::P_loc, 1, false, glm::value_ptr(P));

	glUniform1f(UniformLoc::density_loc, density);
	glUniform1f(UniformLoc::gradient_loc, gradient);
	glUniform3f(UniformLoc::fogColor_loc, fog_color.x, fog_color.y, fog_color.z);
	glUniform3f(UniformLoc::cameraPos_loc, camPos.x, camPos.y, camPos.z);

	glUniform1f(UniformLoc::height_index_loc, height_index);

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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(restart_index);
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, NUM_INDEX, GL_UNSIGNED_INT, NULL, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(0);
}


//--------------------------water---------------------------------------//


GLuint VAO::create_water_vao()
{
	GLuint vao;

	//Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenVertexArrays(1, &vao);

	//Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
	glBindVertexArray(vao);

	GLuint vbo = create_water_posvbo();
	glEnableVertexAttribArray(AttribLoc::Pos_loc); //Enable the position attribute.
	glEnableVertexAttribArray(AttribLoc::TexCoord_loc);
	//Tell opengl how to get the attribute values out of the vbo (stride and offset).
	//In this case, the vertices are at the beginning of the VBO and are tightly packed.
	glVertexAttribPointer(AttribLoc::Pos_loc, 3, GL_FLOAT, false, 5 * sizeof(float), 0);
	glVertexAttribPointer(AttribLoc::TexCoord_loc, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));// change 


	return vao;
}

GLuint VAO::create_water_posvbo()
{
	std::vector<float> terrain_verts;
	terrain_verts.reserve(5 * NUM_VERTICES);
	// create point position and index
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			terrain_verts.push_back(water(i,j).x);
			terrain_verts.push_back(water(i, j).y);
			terrain_verts.push_back(water(i, j).z);
			terrain_verts.push_back(texCoord(i, j).x);
			terrain_verts.push_back(texCoord(i, j).y);
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
			water_index.push_back(restart_index);
		}
		else if (a == 0)
		{
			water_index.push_back(N*(row - 1) + tail / 2);
		}

		else {
			water_index.push_back(N *(row)+tail / 2);
		}
	}



	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*water_index.size(), water_index.data(), GL_STATIC_DRAW);

	GLuint position_buffer;
	glGenBuffers(1, &position_buffer); //Generate vbo to hold vertex attributes for triangle.
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer); //Specify the buffer where vertex attribute data is stored.
	//Upload from main memory to gpu memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*terrain_verts.size(), terrain_verts.data(), GL_STATIC_DRAW);

	return position_buffer;// why doing this ?
}

void VAO::draw_water(GLuint vao, GLuint shader, glm::mat4 M, glm::mat4 V, glm::mat4 P,GLuint waterTextureID[], int pass,float wave_strength, float wave_speed, float time,glm::vec3 cameraPos,
	                float Ks, glm::vec3 Ls, float shinenss, glm::vec3 lightPos, glm::vec3 water_color, float waterColorMix
	              , float density, float gradient, glm::vec3 fog_color)
{
	glUseProgram(0);
	glUseProgram(shader);

	glUniform1i(UniformLoc::waterTexture_loc[0], 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, waterTextureID[0]);

	glUniform1i(UniformLoc::waterTexture_loc[1], 7);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, waterTextureID[1]);

	glUniform1i(UniformLoc::waterTexture_loc[2], 8);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, waterTextureID[2]);

	glUniform1i(UniformLoc::waterTexture_loc[3], 9);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, waterTextureID[3]);

	glUniform1i(UniformLoc::pass_loc, pass);
	glUniform1f(UniformLoc::wave_strength_loc, wave_strength);
	glUniform1f(UniformLoc::time_loc,time);
	glUniform1f(UniformLoc::waveSpeed_loc, wave_speed);
	glUniform3f(UniformLoc::cameraPos_loc, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(UniformLoc::pLs_loc, Ls.x, Ls.y, Ls.z);
	glUniform1f(UniformLoc::pks_loc, Ks);
	glUniform1f(UniformLoc::shiness_loc, shinenss);
	glUniform3f(UniformLoc::lightPos_loc, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(UniformLoc::waterColor_loc, water_color.x, water_color.y, water_color.z);
	glUniform1f(UniformLoc::waterColorMix_loc, waterColorMix);

	//fog 
	glUniform1f(UniformLoc::density_loc, density);
	glUniform1f(UniformLoc::gradient_loc, gradient);
	glUniform3f(UniformLoc::fogColor_loc, fog_color.x, fog_color.y, fog_color.z);
	

	glUniformMatrix4fv(UniformLoc::M_loc, 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(UniformLoc::V_loc, 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(UniformLoc::P_loc, 1, false, glm::value_ptr(P));
	glBindVertexArray(vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(restart_index);
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, NUM_INDEX, GL_UNSIGNED_INT, NULL, 1);
	glUseProgram(0);
}


//--------------------------cube---------------------------------------//
GLuint VAO::create_cube_vao()
{
	GLuint vao;
	//generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo = create_cube_posvbo();

	glEnableVertexAttribArray(AttribLoc::Pos_loc); //enable this attribute
	glVertexAttribPointer(AttribLoc::Pos_loc, 3, GL_FLOAT, false, 0, 0);
	glBindVertexArray(0); //unbind the vao
	return vao;

}

GLuint VAO::create_cube_posvbo()
{
	GLuint vbo;
	const float cube_verts[] = { -1.0f,-1.0f,-1.0f,
								  -1.0f,-1.0f, 1.0f,
								  -1.0f, 1.0f, 1.0f,
								  1.0f, 1.0f,-1.0f,
								  -1.0f,-1.0f,-1.0f,
								  -1.0f, 1.0f,-1.0f,
								  1.0f,-1.0f, 1.0f,
								  -1.0f,-1.0f,-1.0f,
								  1.0f,-1.0f,-1.0f,
								  1.0f, 1.0f,-1.0f,
								  1.0f,-1.0f,-1.0f,
								  -1.0f,-1.0f,-1.0f,
								  -1.0f,-1.0f,-1.0f,
								  -1.0f, 1.0f, 1.0f,
								  -1.0f, 1.0f,-1.0f,
								  1.0f,-1.0f, 1.0f,
								  -1.0f,-1.0f, 1.0f,
								  -1.0f,-1.0f,-1.0f,
								  -1.0f, 1.0f, 1.0f,
								  -1.0f,-1.0f, 1.0f,
								  1.0f,-1.0f, 1.0f,
								  1.0f, 1.0f, 1.0f,
								  1.0f,-1.0f,-1.0f,
								  1.0f, 1.0f,-1.0f,
								  1.0f,-1.0f,-1.0f,
								  1.0f, 1.0f, 1.0f,
								  1.0f,-1.0f, 1.0f,
								  1.0f, 1.0f, 1.0f,
								  1.0f, 1.0f,-1.0f,
								  -1.0f, 1.0f,-1.0f,
								  1.0f, 1.0f, 1.0f,
								  -1.0f, 1.0f,-1.0f,
								  -1.0f, 1.0f, 1.0f,
								  1.0f, 1.0f, 1.0f,
								  -1.0f, 1.0f, 1.0f,
								  1.0f,-1.0f, 1.0f };

	//generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenBuffers(1, &vbo); // Generate vbo to hold vertex attributes for triangle
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //specify the buffer where vertex attribute data is stored
										//upload from main memory to gpu memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), &cube_verts[0], GL_STATIC_DRAW);

	return vbo;

}

void VAO::draw_cube(GLuint vao, GLuint shader,GLuint cubemap_id, glm::mat4 V, glm::mat4 P,int pass)
{


	glUseProgram(0);
	glUseProgram(shader);
	glUniform1i(UniformLoc::pass_loc, pass);
	glDepthMask(GL_FALSE);
	glm::mat4 Msky = glm::scale(glm::vec3(50.0f));
	glm::mat4 PVM = P * V*Msky;
	PVM[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(UniformLoc::PVM_loc, 1, false, glm::value_ptr(PVM));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id);
	glUniform1i(UniformLoc::cubemap_loc, 1); // we bound our texture to texture unit 1

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glUseProgram(0);
}

//Draw the set of points on the surface
VAO::~VAO()
{
}

