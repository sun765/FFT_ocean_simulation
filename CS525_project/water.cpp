#include <vector>
#include "water.h"
#include <iostream>

GLuint water::shader = -1;
std::string water::vertex_shader = "Shaders/water_vs.glsl";
std::string water::fragment_shader = "Shaders/water_fs.glsl";
std::string water::geometry_shader = "Shaders/water_gs.glsl";

water::water()
{

}
water::water(float clip_distance,pointLight *p, fog *f,waterP *w, quadP *q, camera depthCam,terrainP *tP)
{
	
	update(clip_distance,p, f, w, q,tP);
	NUM_VERTICES = this->q.N * this->q.N;
	t = 0;
	restart_index = this->q.N * this->q.N * 10;
	NUM_INDEX = (this->q.N - 1)*(2 * this->q.N + 1);
	init();
	this->depthView = depthCam.get_view_matrice();
	//vao = create_water_vao();
}


glm::vec2 water::texCoord(int i, int j)
{
	return glm::vec2(q.repeat*float(i) / float(q.N - 1), q.repeat*float(j) / float(q.N - 1));
}

// blend 0 -->1   , a-->b ;

glm::vec3 water::waterPos(int i, int j)
{
	return glm::vec3(float(i), 0.0, float(j));
}

glm::vec3 water::normal(int i, int j)
{
	glm::vec3 du = waterPos(i + 1, j) - waterPos(i - 1, j);
	glm::vec3 dv = waterPos(i, j + 1) - waterPos(i, j - 1);
	return glm::normalize(glm::cross(du, dv));
}

glm::vec2 water::get_terrain_wh()
{
	return glm::vec2(float(q.N), float(q.N));
}

GLuint water::create_water_vao()
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
	glBindVertexArray(0);

	return vao;
}

GLuint water::create_water_posvbo()
{
	std::vector<float> terrain_verts;
	terrain_verts.reserve(5 * NUM_VERTICES);
	// create point position and index
	for (int i = 0; i < q.N; i++)
	{
		for (int j = 0; j < q.N; j++)
		{
			terrain_verts.push_back(waterPos(i, j).x);
			terrain_verts.push_back(waterPos(i, j).y);
			terrain_verts.push_back(waterPos(i, j).z);
			terrain_verts.push_back(texCoord(i, j).x);
			terrain_verts.push_back(texCoord(i, j).y);
		}
	}
	for (int i = 0; i < NUM_INDEX; i++)
	{
		int tail = i % (q.N * 2 + 1);
		int a = tail % 2;
		int row = (i + 1) / (q.N * 2 + 1) + 1;
		//i < 2n +1;
		if ((i + 1) % (2 * q.N + 1) == 0)
		{
			water_index.push_back(restart_index);
		}
		else if (a == 0)
		{
			water_index.push_back(q.N*(row - 1) + tail / 2);
		}

		else {
			water_index.push_back(q.N *(row)+tail / 2);
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

void water::draw_water(int shading_mode,bool test, glm::mat4 M, glm::mat4 V, glm::mat4 P, GLuint waterTextureID[], int pass, float time, glm::vec3 cameraPos)
{
	glUseProgram(0);
	glUseProgram(shader);
	//std::cout <<"water shader"<< shader << std::endl;


	// water texture
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

	glUniform1i(UniformLoc::depthTexture_loc, 10);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, waterTextureID[4]);

	glUniform1i(UniformLoc::test_loc, test);
	//water parameters;
	glUniform1i(UniformLoc::pass_loc, pass);
	glUniform1f(UniformLoc::wave_strength_loc, w.wave_strength);
	glUniform1f(UniformLoc::time_loc, time);
	glUniform1f(UniformLoc::waveSpeed_loc, w.wave_speed);
	glUniform3f(UniformLoc::cameraPos_loc, cameraPos.x, cameraPos.y, cameraPos.z);

	glUniform3f(UniformLoc::waterColor_loc,w.water_color.x, w.water_color.y, w.water_color.z);
	glUniform1f(UniformLoc::waterColorMix_loc, w.water_color_mix);
	glUniform1f(UniformLoc::waveLength_loc, w.wave_length);
	glUniform1f(UniformLoc::waveAmplitude_loc, w.wave_amplitude);
	glUniform1f(UniformLoc::edgeSoftness_loc, w.edge_softness);
	glUniform1f(UniformLoc::minblue_loc, w.minBlue);
	glUniform1f(UniformLoc::maxblue_loc, w.maxBlue);
	glUniform1f(UniformLoc::murkyDepth_loc, w.murkyDepth);
	glUniform1i(UniformLoc::shadingMode_loc,shading_mode);
	glUniform1f(UniformLoc::clipDistance_loc, clip_distance);
	glUniform1f(UniformLoc::height_index_loc, tP.height_level);

	//light

	glUniform3f(UniformLoc::pLa_loc, p.La.x, p.La.y, p.La.z);
	glUniform1f(UniformLoc::pka_loc, p.Ka);
	glUniform3f(UniformLoc::pLd_loc, p.Ld.x, p.Ld.y, p.Ld.z);
	glUniform1f(UniformLoc::pkd_loc, p.Kd);
	glUniform3f(UniformLoc::pLs_loc, p.Ls.x, p.Ls.y, p.Ls.z);
	glUniform1f(UniformLoc::pks_loc, p.Ks);
	glUniform1f(UniformLoc::shiness_loc, w.shineness);
	glUniform3f(UniformLoc::lightPos_loc, p.lightPos.x, p.lightPos.y, p.lightPos.z);

	//fog 
	glUniform1f(UniformLoc::density_loc, f.density);
	glUniform1f(UniformLoc::gradient_loc, f.gradient);
	glUniform3f(UniformLoc::fogColor_loc, f.fog_color.x, f.fog_color.y, f.fog_color.z);


	glUniformMatrix4fv(UniformLoc::M_loc, 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(UniformLoc::V_loc, 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(UniformLoc::P_loc, 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(UniformLoc::depthView_loc, 1, false, glm::value_ptr(depthView));
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(this->vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(restart_index);
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, NUM_INDEX, GL_UNSIGNED_INT, NULL, 1);
	glDisable(GL_BLEND);
	glUseProgram(0);
}

void water::initVao()
{
	this->vao = create_water_vao();  
}

void water::init_shader()
{
	
	std::string vertex_shader=  "Shaders/water_vs.glsl" ;
	std::string fragment_shader =  "Shaders/water_fs.glsl" ;
	std::string geometry_shader = "Shaders/water_gs.glsl";
	shader = InitShader(vertex_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
}

void water::update(float clip_distance, pointLight * p, fog * f, waterP * w, quadP * q,terrainP *tP)
{
	this->clip_distance = clip_distance;
	this->p = *p;
	this->f = *f;
	this->w = *w;
	this->q = *q;
	this->tP = *tP;
}

void water::reload_shader()
{
	GLuint new_shader = InitShader(vertex_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
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
		shader = new_shader;
	}
}

void water::init()
{
	initVao();
}

float water::get_clipDistance()
{
	return clip_distance;
}

//Draw the set of points on the surface
water::~water()
{
}

