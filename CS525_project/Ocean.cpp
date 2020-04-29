#include "Ocean.h"

void Ocean::init()
{
	this->init_shaders();
	this->init_textures();
	this->render_precompute_textures();

}

void Ocean::render(glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	this->render_hkt();
	this->compute_IFFT(this->hkt_texture);
	this->compute_IFFT(this->xkt_texture);
	this->compute_IFFT(this->zkt_texture);
	this->render_displacement();

	this->render_shader.bind_shader();
	this->render_shader.set_uniform_mat4("M", M);
	this->render_shader.set_uniform_mat4("V", V);
	this->render_shader.set_uniform_mat4("P", P);
	this->render_shader.set_uniform_vec4("color", this->color);

	this->displacement_texture.bind(GL_READ_ONLY, 0);

	this->ocean_surface.render();
}

void Ocean::reconfig(float amplitude, float windspeed, float alignment, glm::vec2& wind_dir, int choppy_on, float choppy_factor)
{
	this->amplitude     = amplitude;
	this->windspeed     = windspeed;
	this->wind_dir      = wind_dir;
	this->alignment     = alignment;
	this->choppy_on     = choppy_on;
	this->choppy_factor = choppy_factor;
	render_precompute_textures();
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

GLuint Ocean::get_xkt_handle()
{
	return this->xkt_texture.get_handle();
}

GLuint Ocean::get_zkt_handle()
{
	return this->zkt_texture.get_handle();
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

float Ocean::get_amplitude()
{
	return this->amplitude;
}

float Ocean::get_windspeed()
{
	return this->windspeed;
}

float Ocean::get_alignment()
{
	return this->alignment;
}

float Ocean::get_choppy_factor()
{
	return this->choppy_factor;
}

int Ocean::get_choppy_status()
{
	return this->choppy_on;
}

glm::vec2 Ocean::get_wind_dir()
{
	return this->wind_dir;
}

Ocean::Ocean()
{
	this->init();
}

Ocean::Ocean(int dimension)
{
	this->ocean_dimension = dimension;
	this->init();
	this->ocean_surface = QuadMesh(dimension);
}

void Ocean::render_hkt()
{
	// 1. bind shader
	this->hkt_shader.bind_shader();

	// 2. bind textures
	this->h0_k_texture.bind(GL_READ_ONLY, 0);
	this->h0_minus_k_texture.bind(GL_READ_ONLY, 1);
	this->hkt_texture.bind(GL_WRITE_ONLY, 2);
	this->xkt_texture.bind(GL_WRITE_ONLY, 3);
	this->zkt_texture.bind(GL_WRITE_ONLY, 4);
	
	// 3. update uniform
	float time_ms = float(glutGet(GLUT_ELAPSED_TIME));
	this->hkt_shader.set_uniform_float("time_ms", time_ms);
	this->hkt_shader.set_uniform_int("FFT_dimension", FFT_DIMENSION);
	this->hkt_shader.set_uniform_int("ocean_dimension", this->ocean_dimension);

	// 4. bind ssbo
	this->bind_ssbo_float(this->h0data_r, 6);
	this->bind_ssbo_float(this->h0data_i, 7);
	this->bind_ssbo_float(this->wkdata, 8);

    // 5. dispatch compute shader
	glDispatchCompute(FFT_DIMENSION / 16, FFT_DIMENSION / 16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Ocean::render_h0()
{
	// 1. bind shader
	this->h0_shader.bind_shader();

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
	this->h0_shader.set_uniform_int  ("ocean_dimension", this->ocean_dimension);
	this->h0_shader.set_uniform_float("amplitude",       this->amplitude);
	this->h0_shader.set_uniform_float("windspeed",       this->windspeed);
	this->h0_shader.set_uniform_vec2 ("wind_dir",        this->wind_dir);
	this->h0_shader.set_uniform_int  ("FFT_dimension",   FFT_DIMENSION);
	// 5. bind pre computed data to shader

	this->bind_ssbo_float(this->h0data_r, 3);
	this->bind_ssbo_float(this->h0data_i, 4);

	// 6. dispatch compute shader
	glDispatchCompute(FFT_DIMENSION/16 , FFT_DIMENSION/16,  1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

}

void Ocean::compute_h0()
{
	this->h0data_r.clear();
	this->h0data_i.clear();
	this->wkdata.clear();

	std::mt19937 gen;
	std::normal_distribution<> gaussian(0.0, 1.0);
	float L = this->patch_size;
	int start = FFT_DIMENSION / 2;

	glm::vec2 w = this->wind_dir;
	glm::vec2 wn = glm::normalize(w);
	float V = this->windspeed;
	float A = this->amplitude;

	for (int m = 0; m < FFT_DIMENSION; ++m) {
		glm::vec2 k;
		k.y = (2 * PI * (start - m)) / L;

		for (int n = 0; n < FFT_DIMENSION; ++n) {
			k.x = (2 * PI * (start - n)) / L;

			int index = m * (FFT_DIMENSION)+n;
			float sqrt_P_h = 0;

			if (k.x != 0.0f || k.y != 0.0f)
				sqrt_P_h = sqrtf(Phillips(k, wn, V, A));

			float sqrt_2 = sqrtf(2.0);
			this->h0data_r.push_back((float)(sqrt_P_h * gaussian(gen) * sqrt_2));
			this->h0data_i.push_back((float)(sqrt_P_h * gaussian(gen) * sqrt_2));

			//std::cout << "a: " << h0data_r[index] << "b: " <<h0data_i[index] <<std::endl;
			// dispersion relation \omega^2(k) = gk
			this->wkdata.push_back(sqrtf(G * glm::length(k)));
		}
	}

}

void Ocean::bind_ssbo_float(vector<float>& data, int loc)
{
	GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * data.size(), data.data(), GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, loc, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
}

void Ocean::bind_ssbo_int(vector<int>& data, int loc)
{
	GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * data.size(), data.data(), GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, loc, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
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

	//GLuint ssbo;
	//glGenBuffers(1, &ssbo);
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * reversed_bit.size(), reversed_bit.data(), GL_DYNAMIC_COPY);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

	this->bind_ssbo_int(reversed_bit, 1);

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
	this->hkt_texture.bind(GL_READ_ONLY, 0);
	this->xkt_texture.bind(GL_READ_ONLY, 1);
	this->zkt_texture.bind(GL_READ_ONLY, 2);
	this->displacement_texture.bind(GL_WRITE_ONLY, 3);

	this->displacement_shader.set_uniform_int  ("FFT_dimension", FFT_DIMENSION);
	this->displacement_shader.set_uniform_float("choppy_factor", this->choppy_factor);
	this->displacement_shader.set_uniform_int("choppy_on",       this->choppy_on);

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

	this->render_shader         = VertexFragShader("Shaders/ocean_vs.glsl", "Shaders/ocean_fs.glsl");
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
	this->ht_texture             =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->dxy_texture            =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->debug_input_texture    =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->debug_output_texture   =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->xkt_texture            =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);
	this->zkt_texture            =  CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);

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

int  Ocean::reverse_bit(int i, int bit_num)
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

float Ocean::Phillips(const glm::vec2& k, const glm::vec2& w, float V, float A)
{
	float L = (V * V) / G;	// largest possible wave for wind speed V
	float l = L / 1000.0f;					// supress waves smaller than this

	float kdotw = glm::dot(k,w);
	float k2 = glm::dot(k,k);			// squared length of wave vector k

	// k^6 because k must be normalized
	float P_h = A * (expf(-1.0f / (k2 * L * L))) / (k2 * k2 * k2) * pow(kdotw, this->alignment);

	if (kdotw < 0.0f) {
		// wave is moving against wind direction w
		P_h *= 0.07f;
	}

	return P_h * expf(-k2 * l * l);
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
	this->compute_h0();
	this->render_h0();
	this->render_twiddle_factor();
	this->render_twiddle_debug();
}

int  Ocean::compute_IFFT_helper(CompOutputTexture* input_texture, CompOutputTexture* output_texture)
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
		glDispatchCompute(256, FFT_DIMENSION / 16, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		pingpong = (pingpong + 1) % 2;
	}
	
	return pingpong;
}

void Ocean::compute_IFFT(CompOutputTexture& input_texture)
{


	// horizontal pass
	//glBindImageTexture(0, spectrum, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RG32F);
	//glBindImageTexture(1, tempdata, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RG32F);

	input_texture.bind(GL_READ_ONLY, 0);
	this->IFFT_buffer_texture.bind(GL_WRITE_ONLY, 1);

	this->IFFT_shader.bind_shader();
	this->IFFT_shader.set_uniform_int("FFT_dimension", FFT_DIMENSION);

	glDispatchCompute(FFT_DIMENSION, 1, 1);


	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	// vertical pass

	this->IFFT_buffer_texture.bind(GL_READ_ONLY, 0);
	input_texture.bind(GL_WRITE_ONLY, 1);

		//I think this should be  glDispatchCompute(1, DISP_MAP_SIZE, 1);.  why is not????
	this->IFFT_shader.bind_shader();
	this->IFFT_shader.set_uniform_int("FFT_dimension", FFT_DIMENSION);
	glDispatchCompute(FFT_DIMENSION, 1, 1);


}
