#include "Ocean.h"

void Ocean::init()
{
	this->init_shaders();
	this->init_textures();
	this->render_precompute_textures();

}

GLuint Ocean::get_h0_k_handle()
{
	return this->h0_k_texture.get_handle();
}

GLuint Ocean::get_h0_minus_k_handle()
{
	return this->h0_minus_k_texture.get_handle();
}


Ocean::Ocean()
{
	this->init();
}

void Ocean::render_h0()
{
	// 1. bind two texture
	//h0_shader.bind_shader();
	debug_shader.bind_shader();

	// 2. bind h0 textures
	int bind_loc = 0;
	h0_k_texture.bind(GL_WRITE_ONLY, bind_loc++);
	h0_minus_k_texture.bind(GL_WRITE_ONLY, bind_loc++);
	
	// 3. bind noise textures 
	const vector<string> var_names = { "noise_r0" ,"noise_i0", "noise_r1", "noise_i1" };
	const vector<GLenum> texture_ids = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3 };
	vector<GLuint> locs = { 0,0,0,0 };



	for (int i = 0; i < this->noise_textures.size(); i++) {
		locs[i] = glGetUniformLocation(debug_shader.get_program_handle(), &var_names[i][0]);
	}
	for (int i = 0; i < 4; i++) {
		cout << "locs: " << locs[i] << endl;
		glUniform1i(locs[i], i);
	}
	for (int i = 0; i<this->noise_textures.size(); i++) {
		glActiveTexture(texture_ids[i]);
		glBindTexture(GL_TEXTURE_2D, this->noise_textures[i]);
		cout << "ts " << this->noise_textures.size()<<endl;
	}

	// 4. dispatch compute shader
	glDispatchCompute(FFT_DIMENSION / 16, FFT_DIMENSION / 16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void Ocean::init_shaders()
{
	this->h0_shader    = ComputeShader("Shaders/h0_comp.comp");

	this->debug_shader = ComputeShader("Shaders/test.comp");
}

void Ocean::init_textures()
{
	// init h0 textures
	this->h0_k_texture       =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->h0_minus_k_texture =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);

	// init noise textures
	const vector<string> noise_texture_paths = { "Textures/Noise256_0.jpg",
											     "Textures/Noise256_1.jpg",
											     "Textures/Noise256_2.jpg",
											     "Textures/Noise256_3.jpg",
											   };

	for (int i = 0; i < noise_texture_paths.size(); i++) {
		GLuint noise_texture = LoadTexture(&noise_texture_paths[i][0]);
		noise_textures.push_back(noise_texture);
	}

	cout << noise_texture_paths.size();
}

void Ocean::render_precompute_textures()
{
	this->render_h0();
}
