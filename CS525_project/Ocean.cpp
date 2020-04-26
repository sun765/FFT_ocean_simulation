#include "Ocean.h"

void Ocean::init()
{
	this->init_shaders();
	this->init_textures();
	this->render_precompute_textures();

}

void Ocean::render()
{
	this->render_hkt();
}

GLuint Ocean::get_h0_k_handle()
{
	return this->h0_k_texture.get_handle();
}

GLuint Ocean::get_h0_minus_k_handle()
{
	return this->h0_minus_k_texture.get_handle();
}

GLuint Ocean::get_hkt_handle()
{
	return this->hkt_texture.get_handle();
}

GLuint Ocean::get_twiddle_handle()
{
	return this->twiddle_factor_texture.get_handle();
}

GLuint Ocean::get_displacement_handle()
{
	return this->displacement_texture.get_handle();
}

GLuint Ocean::get_twiddle_debug_handle()
{
	return this->twiddle_debug_texture.get_handle();
}

Ocean::Ocean()
{
	this->init();
}

void Ocean::render_hkt()
{
	// 1. bind shader
	this->hkt_shader.bind_shader();

	// 2. bind textures
	this->h0_k_texture.bind(GL_READ_ONLY, 0);
	this->h0_minus_k_texture.bind(GL_READ_ONLY, 1);
	this->hkt_texture.bind(GL_WRITE_ONLY, 2);
	
	// 3. update uniform
	float time_ms = float(glutGet(GLUT_ELAPSED_TIME));
	this->hkt_shader.set_uniform_float("time_ms", time_ms);

    // 4. dispatch compute shader
	glDispatchCompute(FFT_DIMENSION / 16, FFT_DIMENSION / 16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Ocean::render_h0()
{
	// 1. bind shader
	h0_shader.bind_shader();

	// 2. bind h0 textures
	int bind_loc = 0;
	this->h0_k_texture.bind(GL_WRITE_ONLY, bind_loc++);
	this->h0_minus_k_texture.bind(GL_WRITE_ONLY, bind_loc++);
	
	// 3. bind noise textures 
	const vector<string> var_names = { "noise_r0" ,"noise_i0", "noise_r1", "noise_i1" };
	const vector<GLenum> texture_ids = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3 };

	for (int i = 0; i<this->noise_textures.size(); i++) {
		h0_shader.set_uniform_int(var_names[i], i);

		glActiveTexture(texture_ids[i]);
		glBindTexture(GL_TEXTURE_2D, this->noise_textures[i]);
	}

	// 4. dispatch compute shader
	glDispatchCompute(FFT_DIMENSION/16 , FFT_DIMENSION/16,  1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void Ocean::render_twiddle_factor()
{
	// 1. bind shader
	this->twiddle_factor_shader.bind_shader();

	// 2. bind textures
	this->twiddle_factor_texture.bind(GL_WRITE_ONLY, 0);

	// 3. dispatch compute  (the size of the work group may be changed?)
	glDispatchCompute(log2(FFT_DIMENSION), FFT_DIMENSION / 16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Ocean::init_shaders()
{
	this->h0_shader             = ComputeShader("Shaders/h0_comp.comp");
	this->debug_shader          = ComputeShader("Shaders/test.comp");
	this->hkt_shader            = ComputeShader("Shaders/hkt_comp.comp");
	this->twiddle_factor_shader = ComputeShader("Shaders/twiddle_factor_comp.comp");
	this->displacement_shader   = ComputeShader("Shaders/displacement_comp.comp");
	this->twiddle_debug_shader  = ComputeShader("Shaders/twiddle_debug_comp.comp");
}

void Ocean::init_textures()
{
	
	// init textures
	this->h0_k_texture           =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION,       GL_RGBA32F);
	this->h0_minus_k_texture     =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION,       GL_RGBA32F);
	this->hkt_texture            =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION,       GL_RGBA32F);
	this->twiddle_factor_texture =  CompOutputTexture(log2(FFT_DIMENSION), FFT_DIMENSION, GL_RGBA32F);
	this->twiddle_debug_texture  =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->displacement_texture   =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION,       GL_RGBA32F);

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

}

void Ocean::render_twiddle_debug()
{
	// 1. bind shader
	this->twiddle_debug_shader.bind_shader();

	// 2. bind textures
	this->twiddle_factor_texture.bind(GL_READ_ONLY, 0);
	this->twiddle_debug_texture.bind(GL_WRITE_ONLY, 1);

	// 3. dispatch compute
	glDispatchCompute(FFT_DIMENSION / 16, FFT_DIMENSION / 16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void Ocean::render_precompute_textures()
{
	this->render_h0();
	this->render_twiddle_factor();
	this->render_twiddle_debug();
}
