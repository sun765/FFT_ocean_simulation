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
	this->compute_IFFT();
	this->render_displacement();
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

GLuint Ocean::get_ht_handle()
{
	return this->ht_texture.get_handle();
}

GLuint Ocean::get_ifft_buffer_handle()
{
	return this->IFFT_buffer_texture.get_handle();
}

GLuint Ocean::get_dxy_handle()
{
	return this->dxy_texture.get_handle();
}

GLuint Ocean::get_debug_input_handle()
{
	return this->debug_input_texture.get_handle();
}

GLuint Ocean::get_debug_output_handle()
{
	return this->debug_output_texture.get_handle();
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
	this->hkt_shader.set_uniform_int("FFT_dimension", FFT_DIMENSION);
	this->hkt_shader.set_uniform_int("ocean_dimension", this->ocean_dimension);

    // 4. dispatch compute shader
	glDispatchCompute(FFT_DIMENSION / 16, FFT_DIMENSION / 16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Ocean::render_h0()
{
	// 1. bind shader
	h0_shader.bind_shader();

	// 2. bind h0 textures
	this->h0_k_texture.bind(GL_WRITE_ONLY, 0);
	this->h0_minus_k_texture.bind(GL_WRITE_ONLY, 1);
	
	// 3. bind noise textures and uniform variable
	const vector<string> var_names = { "noise_r0" ,"noise_i0", "noise_r1", "noise_i1" };
	const vector<GLenum> texture_ids = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3 };

	for (int i = 0; i<this->noise_textures.size(); i++) {
		h0_shader.set_uniform_int(var_names[i], i);

		glActiveTexture(texture_ids[i]);
		glBindTexture(GL_TEXTURE_2D, this->noise_textures[i]);
	}
	h0_shader.set_uniform_int  ("ocean_dimension", this->ocean_dimension);
	h0_shader.set_uniform_float("amplitude",       this->amplitude);
	h0_shader.set_uniform_float("windspeed",       this->windspeed);
	h0_shader.set_uniform_vec2 ("wind_dir",        this->wind_dir);

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

	// 3. calc bit reverse array and bind to ssbo
	vector<int> reversed_bit;
	for (int i = 0; i <FFT_DIMENSION; i++) {
		reversed_bit.push_back(this->reverse_bit(i, log2(FFT_DIMENSION)));
		//cout << i << " " << reversed_bit[i] << endl;
	}

	GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * reversed_bit.size(), reversed_bit.data(), GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

	this->twiddle_factor_shader.set_uniform_int("FFT_dimension", FFT_DIMENSION);

	// 4. dispatch compute  (the size of the work group may be changed?)
	glDispatchCompute(log2(FFT_DIMENSION), FFT_DIMENSION / 16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Ocean::render_displacement()
{
	// 1. bind shader
	this->displacement_shader.bind_shader();

	// 2. bind textures
	this->ht_texture.bind(GL_READ_ONLY, 0);
	this->displacement_texture.bind(GL_WRITE_ONLY, 1);

	// 3. dispatch compute
	glDispatchCompute(FFT_DIMENSION / 16, FFT_DIMENSION / 16, 1);
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
	this->IFFT_shader           = ComputeShader("Shaders/ifft_comp.comp");
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
	this->IFFT_buffer_texture    =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->ht_texture             = CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->dxy_texture            = CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->debug_input_texture    = CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->debug_output_texture   = CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
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

int Ocean::reverse_bit(int i, int bit_num)
{
	int ans = 0;

	while (bit_num >0) {
		ans <<= 1;
		if ((i & 1) == 1)
			ans ^= 1;
		i >>= 1;
		bit_num--;
	}
	return ans;
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

int Ocean::compute_IFFT_helper(CompOutputTexture* input_texture, CompOutputTexture* output_texture)
{
	// 0. bind shader
	this->IFFT_shader.bind_shader();

	// 1. Create a buffer texture
	int pingpong = 0;
	int step = log2(FFT_DIMENSION);
	input_texture->bind(GL_READ_WRITE, 0);
	output_texture->bind(GL_READ_WRITE, 1);
	this->twiddle_factor_texture.bind(GL_READ_ONLY, 2);

	// 2. horizontal logN loops (use 1 true and 0 flase as bool )
	IFFT_shader.set_uniform_int("horizontal", 1);
	IFFT_shader.set_uniform_int("vertical", 0);
	for (int i = 0; i < step; i++) {
		this->IFFT_shader.set_uniform_int("pingpong", pingpong);
		this->IFFT_shader.set_uniform_int("stage", i);
		glDispatchCompute(FFT_DIMENSION/16, FFT_DIMENSION/16, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		pingpong = (pingpong + 1) % 2;
	}
	
	// 3. vertical logN loops
	IFFT_shader.set_uniform_int("horizontal", 0);
	IFFT_shader.set_uniform_int("vertical", 1);
	for (int i = 0; i < step; i++) {
		this->IFFT_shader.set_uniform_int("pingpong", pingpong);
		this->IFFT_shader.set_uniform_int("stage", i);
		glDispatchCompute(FFT_DIMENSION/16, FFT_DIMENSION / 16, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		pingpong = (pingpong + 1) % 2;
	}
	
	return pingpong;
}

void Ocean::compute_IFFT()
{
	// ifft on y direction 
	int pingpong = compute_IFFT_helper(&this->hkt_texture, &this->ht_texture);
	// swap input and output
	if (pingpong == 0) {
		CompOutputTexture temp = this->hkt_texture;
		this->hkt_texture = this->ht_texture;
		this->ht_texture = temp;
	}

	// ifft on x, z direction
}
